// Copyright 2024 N-GINN LLC. All rights reserved.
// Use of this source code is governed by a BSD-3 Clause license that can be found in the LICENSE file.


#include "nau/memory/bytes_buffer.h"

#include "nau/diag/assertion.h"
#include "nau/utils/scope_guard.h"

// #define TRACK_BUFFER_ALLOCATIONS

namespace nau
{

    namespace
    {

        constexpr size_t Alignment = sizeof(ptrdiff_t);
        constexpr size_t AllocationGranularity = 128;
        constexpr size_t BigAllocationGranularity = 1024;
        constexpr size_t BigAllocationThreshold = 4096;

#ifdef TRACK_BUFFER_ALLOCATIONS
        class BufferAllocationTracker
        {
        public:
            ~BufferAllocationTracker()
            {
            }

            void notifyAllocated(size_t size)
            {
            }

            void notifyFree(size_t size)
            {
            }

        private:
            std::mutex m_mutex;
            size_t m_currentAllocationSize = 0;
        };
#endif

    }  // namespace

    /**
     */
    struct alignas(Alignment) BufferBase::Header
    {
        size_t allocatedSize;
        size_t size;
        uint32_t refs;
        uint32_t flags;
        IMemAllocator::Ptr allocator;
    };

    namespace
    {
        constexpr size_t HeaderSize = sizeof(std::aligned_storage_t<sizeof(BufferBase::Header)>);
        constexpr ptrdiff_t ClientDataOffset = static_cast<ptrdiff_t>(HeaderSize);

        inline uint32_t AtomicIncrement(uint32_t& value)
        {
            NAU_ASSERT(value > 0);

#ifdef NAU_PLATFORM_WIN32
            return InterlockedIncrement(&value);
#else
            return __sync_fetch_and_add(&value, 1);
#endif
        }

        inline uint32_t AtomicDecrement(uint32_t& value)
        {
            NAU_ASSERT(value > 0);

#ifdef NAU_PLATFORM_WIN32
            return InterlockedDecrement(&value);
#else
            return __sync_fetch_and_sub(&value, 1);
#endif
        }

        inline size_t allocatedSize(const std::byte* ptr)
        {
            return ptr ? reinterpret_cast<const BufferBase::Header*>(ptr)->allocatedSize : 0;
        }

        inline size_t clientSize(const std::byte* ptr)
        {
            return ptr ? reinterpret_cast<const BufferBase::Header*>(ptr)->size : 0;
        }

        inline uint32_t refsCount(const std::byte* ptr)
        {
            return ptr == nullptr ? 0 : reinterpret_cast<const BufferBase::Header*>(ptr)->refs;
        }

        inline std::byte* clientData(std::byte* ptr)
        {
            const BufferBase::Header* const header_ = reinterpret_cast<const BufferBase::Header*>(ptr);
            return header_ && header_->size > 0 ? (ptr + ClientDataOffset) : nullptr;
        }

        // inline const std::byte* clientData(const std::byte* ptr)
        //{
        //	const BufferBase::Header* const header_ = reinterpret_cast<const BufferBase::Header*>(ptr);
        //	return header_ && header_->size > 0 ? (ptr + ClientDataOffset) : nullptr;
        // }

        // ComPtr<Allocator>& defaultBlockAllocator() {
        //	static ComPtr<Allocator> allocator = createPoolAllocator({true, AllocationGranularity, AllocationGranularity * 4});
        //
        //	return (allocator);
        // }
        //
        // ComPtr<Allocator>& bigBlockAllocator() {
        //	static ComPtr<Allocator> allocator = createPoolAllocator({true, AllocationGranularity, AllocationGranularity * 4});
        //
        //	return (allocator);
        // }

    }  // namespace

    std::byte* BufferStorage::allocate(size_t size, IMemAllocator::Ptr allocator)
    {
        using Header = BufferBase::Header;

        const size_t granuleSize = (size < BigAllocationThreshold) ? AllocationGranularity : BigAllocationGranularity;
        const size_t storageSize = alignedSize(HeaderSize + size, granuleSize);
        const size_t allocatedSize = storageSize - HeaderSize;

        void* storage = nullptr;
        if (allocator)
        {
            storage = allocator->allocate(storageSize);
        }
        else
        {
            storage = getDefaultAllocator()->allocate(storageSize);
        }
        NAU_ASSERT(storage);

        Header* header = new(storage) Header;

        header->allocatedSize = allocatedSize;
        header->size = size;
        header->refs = 1;
        header->allocator = allocator ? allocator : nullptr;
        header->flags = 0;
        // header.marker = {'A', 'B', 'C', 'D'};

        return reinterpret_cast<std::byte*>(storage);
    }

    void BufferStorage::reallocate(std::byte*& buffer, size_t size)
    {
        NAU_ASSERT(buffer != nullptr);

        using Header = BufferBase::Header;

        const Header header = *reinterpret_cast<const Header*>(buffer);
        NAU_ASSERT(header.refs == 1);
        // NAU_ASSERT((oldHeader.flags & ReadOnlyFlag) != ReadOnlyFlag)

        if (header.allocatedSize >= size)
        {
            reinterpret_cast<Header*>(buffer)->size = size;
            return;
        }

        const size_t granuleSize = (size < BigAllocationThreshold) ? AllocationGranularity : BigAllocationGranularity;
        const size_t storageSize = alignedSize(size + HeaderSize, granuleSize);  // sizeAlignedByGranularity__(size);
        const size_t allocatedSize = storageSize - HeaderSize;

        void* storage = nullptr;
        // auto* const allocator = header.allocator ? header.allocator : getCrtAllocator().get();

        if (header.allocator)
        {
            storage = header.allocator->reallocate(buffer, storageSize);
        }
        else
        {
            storage = getDefaultAllocator()->reallocate(buffer, storageSize);
        }

        NAU_ASSERT(storage);

        Header& newHeader = *reinterpret_cast<Header*>(storage);
        newHeader = header;
        newHeader.size = size;
        newHeader.allocatedSize = allocatedSize;

        buffer = reinterpret_cast<std::byte*>(storage);
    }

    void BufferStorage::release(std::byte*& storage)
    {
        if (!storage)
        {
            return;
        }

        using Header = BufferBase::Header;

        Header& header = *reinterpret_cast<Header*>(storage);
        if (AtomicDecrement(header.refs) == 0)
        {
            if (IMemAllocator::Ptr const allocator = header.allocator; allocator)
            {
                // allocator->free(storage, HeaderSize + header.allocatedSize);
                header.allocator = nullptr;
                allocator->deallocate(storage);
            }
            else
            {
                getDefaultAllocator()->deallocate(storage);
            }
        }

        storage = nullptr;
    }

    std::byte* BufferStorage::takeOut(BufferBase&& buffer)
    {
        if (!buffer.m_storage)
        {
            return nullptr;
        }

        std::byte* const storage = buffer.m_storage;
        buffer.m_storage = nullptr;
        NAU_ASSERT(refsCount(storage) == 1);
        return storage;
    }

    std::byte* BufferStorage::data(std::byte* storage)
    {
        return clientData(storage);
    }

    size_t BufferStorage::size(const std::byte* storage)
    {
        return clientSize(storage);
    }

    BytesBuffer BufferStorage::bufferFromStorage(std::byte* storage)
    {
        NAU_ASSERT(storage);
        return BytesBuffer(storage);
    }

    BytesBuffer BufferStorage::bufferFromClientData(std::byte* ptr, std::optional<size_t> size)
    {
        NAU_ASSERT(ptr);

        std::byte* const storage = ptr - ClientDataOffset;
        // NAU_ASSERT(!size || *size <= allocatedSize(storage), "Specified size ({0}) does not fit into buffer. bufferFromClientData can not grow buffer size")
        NAU_ASSERT(!size || *size <= allocatedSize(storage));

        BytesBuffer buffer{storage};
        if (size)
        {
            buffer.resize(*size);
        }

        return buffer;
    }

    BufferBase::BufferBase() :
        m_storage(nullptr)
    {
    }

    BufferBase::BufferBase(std::byte* data_) :
        m_storage(data_)
    {
        // 32bit alignment required by interlocked operations.
        static_assert((offsetof(BufferBase::Header, refs) * 8) % 32 == 0);
        NAU_ASSERT(!m_storage || header().refs > 0);
    }

    BufferBase::~BufferBase()
    {
        release();
    }

    size_t BufferBase::size() const
    {
        return m_storage ? header().size : 0;
    }

    bool BufferBase::empty() const
    {
        return !m_storage || header().size == 0;
    }

    BufferBase::operator bool() const
    {
        return m_storage != nullptr;
    }

    void BufferBase::release()
    {
        BufferStorage::release(this->m_storage);
        NAU_ASSERT(m_storage == nullptr);
    }

    bool BufferBase::sameBufferObject(const BufferBase& buffer_) const
    {
        return buffer_.m_storage && this->m_storage && (buffer_.m_storage == m_storage);
    }

    bool BufferBase::sameBufferObject(const BufferView& view) const
    {
        return view && sameBufferObject(view.underlyingBuffer());
    }

    BufferBase::Header& BufferBase::header()
    {
        NAU_ASSERT(m_storage != nullptr);
        return *reinterpret_cast<Header*>(m_storage);
    }

    const BufferBase::Header& BufferBase::header() const
    {
        NAU_ASSERT(m_storage != nullptr);
        return *reinterpret_cast<const Header*>(m_storage);
    }

    BytesBuffer::BytesBuffer() = default;

    BytesBuffer::BytesBuffer(size_t size, IMemAllocator::Ptr allocator)
    {
        m_storage = BufferStorage::allocate(size, std::move(allocator));
    }

    BytesBuffer::BytesBuffer(BytesBuffer&& buffer) noexcept :
        BufferBase(buffer.m_storage)
    {
        buffer.m_storage = nullptr;
    }

    BytesBuffer::BytesBuffer(BufferView&& buffer) noexcept
    {
        this->operator=(std::move(buffer));
    }

    BytesBuffer& BytesBuffer::operator=(BytesBuffer&& buffer) noexcept
    {
        this->release();
        std::swap(buffer.m_storage, this->m_storage);
        return *this;
    }

    BytesBuffer& BytesBuffer::operator=(BufferView&& bufferView) noexcept
    {
        scope_on_leave
        {
            bufferView = BufferView{};
        };

        this->release();

        if (const size_t viewSize = bufferView.size(); viewSize > 0)
        {
            if (BufferUtils::refsCount(bufferView) == 1 && bufferView.offset() == 0)
            {
                this->operator=(bufferView.m_buffer.toBuffer());
                this->resize(viewSize);
            }
            else
            {
                m_storage = BufferStorage::allocate(viewSize, bufferView.m_buffer.header().allocator);
                memcpy(this->data(), bufferView.data(), viewSize);
            }
        }

        return *this;
    }

    BytesBuffer& BytesBuffer::operator=(std::nullptr_t) noexcept
    {
        this->release();
        return *this;
    }

    std::byte* BytesBuffer::data() const
    {
        return clientData(m_storage);
    }

    std::byte* BytesBuffer::append(size_t count)
    {
        const size_t offset = size();
        resize(offset + count);
        return data() + offset;
    }

    void BytesBuffer::resize(size_t newSize)
    {
        if (m_storage == nullptr)
        {
            m_storage = BufferStorage::allocate(newSize);
            return;
        }

        if (refsCount(m_storage) == 1)
        {
            BufferStorage::reallocate(m_storage, newSize);
            return;
        }

        auto allocator = header().allocator;
        auto storage = BufferStorage::allocate(newSize, allocator);
        if (!storage)
        {
            return;
        }

        if (const std::byte* const ptr = this->data(); ptr)
        {
            NAU_ASSERT(header().size > 0);

            const size_t copySize = std::min(header().size, newSize);
            memcpy(clientData(storage), ptr, copySize);
        }

        this->release();
        m_storage = storage;

        NAU_ASSERT(header().size == newSize);
    }

    ReadOnlyBuffer BytesBuffer::toReadOnly()
    {
        return ReadOnlyBuffer{std::move(*this)};
    }

    void BytesBuffer::operator+=(BufferView&& source) noexcept
    {
        if (source.size() == 0)
        {
            return;
        }

        if (!m_storage)
        {
            *this = std::move(source);
        }
        else if (header().allocator == source.m_buffer.header().allocator && this->empty() && source.offset() == 0)
        {
            *this = std::move(source);
        }
        else
        {
            std::byte* const ptr = this->append(source.size());
            memcpy(ptr, source.data(), source.size());

            source.release();
        }
    }

    ReadOnlyBuffer::ReadOnlyBuffer() = default;

    ReadOnlyBuffer::ReadOnlyBuffer(BytesBuffer&& buffer) noexcept
    {
        NAU_ASSERT(!buffer || BufferUtils::refsCount(buffer) == 1);
        *this = std::move(buffer);
    }

    ReadOnlyBuffer::ReadOnlyBuffer(const ReadOnlyBuffer& other) :
        BufferBase(other.m_storage)
    {
        if (m_storage)
        {
            AtomicIncrement(header().refs);
        }
    }

    ReadOnlyBuffer::ReadOnlyBuffer(ReadOnlyBuffer&& other) noexcept :
        BufferBase(other.m_storage)
    {
        other.m_storage = nullptr;
    }

    ReadOnlyBuffer& ReadOnlyBuffer::operator=(BytesBuffer&& other) noexcept
    {
        this->release();
        std::swap(this->m_storage, other.m_storage);

        return *this;
    }

    ReadOnlyBuffer& ReadOnlyBuffer::operator=(const ReadOnlyBuffer& other)
    {
        this->release();
        if (m_storage = other.m_storage; m_storage)
        {
            AtomicIncrement(header().refs);
        }

        return *this;
    }

    ReadOnlyBuffer& ReadOnlyBuffer::operator=(ReadOnlyBuffer&& other) noexcept
    {
        this->release();
        std::swap(this->m_storage, other.m_storage);
        return *this;
    }

    ReadOnlyBuffer& ReadOnlyBuffer::operator=(std::nullptr_t) noexcept
    {
        this->release();
        return *this;
    }

    const std::byte* ReadOnlyBuffer::data() const
    {
        return clientData(m_storage);
    }

    BytesBuffer ReadOnlyBuffer::toBuffer()
    {
        if (!this->m_storage)
        {
            return {};
        }

        if (refsCount(this->m_storage) == 1)
        {
            std::byte* storage = nullptr;
            std::swap(storage, m_storage);
            return BytesBuffer{storage};
        }

        BytesBuffer buffer = BufferUtils::copy(*this);
        this->release();
        return buffer;
    }

    BufferView::BufferView() :
        m_offset(0),
        m_size(0)
    {
    }

    BufferView::BufferView(BytesBuffer&& buffer) noexcept
        :
        m_buffer(buffer.toReadOnly()),
        m_offset(0)
    {
        m_size = m_buffer.size();
    }

    BufferView::BufferView(const ReadOnlyBuffer& buffer, size_t offset_, std::optional<size_t> size_) :
        m_buffer(buffer),
        m_offset(offset_),
        m_size(size_ ? *size_ : m_buffer.size() - offset_)
    {
        if (!m_buffer)
        {
            NAU_ASSERT(m_offset == 0 && (!size_ || *size_ == 0), "Invalid parameters while construct BufferView from empty ReadOnlyBuffer");
        }
        else
        {
            NAU_ASSERT(m_offset + m_size <= m_buffer.size());
        }
    }

    BufferView::BufferView(ReadOnlyBuffer&& buffer, size_t offset_, std::optional<size_t> size_) :
        m_buffer(std::move(buffer)),
        m_offset(offset_),
        m_size(size_ ? *size_ : m_buffer.size() - offset_)
    {
        if (!m_buffer)
        {
            NAU_ASSERT(m_offset == 0 && (!size_ || *size_ == 0), "Invalid parameters while construct BufferView from empty ReadOnlyBuffer");
        }
        else
        {
            NAU_ASSERT(m_offset + m_size <= m_buffer.size());
        }
    }

    BufferView::BufferView(const BufferView& buffer, size_t offset_, std::optional<size_t> size_) :
        m_buffer(buffer.m_buffer),
        m_offset(buffer.m_offset + offset_),
        m_size(size_ ? *size_ : buffer.m_size - offset_)
    {
        NAU_ASSERT((m_offset + m_size) <= m_buffer.size());
    }

    BufferView::BufferView(BufferView&& buffer, size_t offset_, std::optional<size_t> size_) :
        m_buffer(std::move(buffer.m_buffer)),
        m_offset(buffer.m_offset + offset_),
        m_size(size_ ? *size_ : buffer.m_size - offset_)
    {
        NAU_ASSERT((m_offset + m_size) <= m_buffer.size());
    }

    BufferView::BufferView(const BufferView&) = default;

    BufferView::BufferView(BufferView&& other) noexcept
        :
        m_buffer(std::move(other.m_buffer)),
        m_offset(other.m_offset),
        m_size(other.m_size)
    {
        other.m_offset = 0;
        other.m_size = 0;
    }

    BufferView& BufferView::operator=(const BufferView&) = default;

    BufferView& BufferView::operator=(BufferView&& other) noexcept
    {
        m_buffer = std::move(other.m_buffer);
        m_offset = other.m_offset;
        m_size = other.m_size;

        other.m_offset = 0;
        other.m_size = 0;

        return *this;
    }

    bool BufferView::operator==(const BufferView& other) const
    {
        return m_buffer.sameBufferObject(other.m_buffer) && m_size == other.m_size && m_offset == other.m_offset;
    }

    bool BufferView::operator!=(const BufferView& other) const
    {
        return !m_buffer.sameBufferObject(other.m_buffer) || m_size != other.m_size || m_offset != other.m_offset;
    }

    void BufferView::release()
    {
        if (m_buffer)
        {
            *this = BufferView{};
        }
    }

    const std::byte* BufferView::data() const
    {
        return m_buffer ? m_buffer.data() + m_offset : nullptr;
    }

    size_t BufferView::size() const
    {
        return m_size;
    }

    size_t BufferView::offset() const
    {
        return m_offset;
    }

    BufferView::operator bool() const
    {
        return static_cast<bool>(m_buffer);
    }

    ReadOnlyBuffer BufferView::underlyingBuffer() const
    {
        return m_buffer;
    }

    BytesBuffer BufferView::toBuffer()
    {
        if (!m_buffer || m_size == 0)
        {
            return {};
        }

        scope_on_leave
        {
            if (m_buffer)
            {
                m_buffer.release();
            }
            m_size = 0;
            m_offset = 0;
        };

        if (m_offset == 0 && BufferUtils::refsCount(m_buffer) == 1)
        {
            BytesBuffer buffer = m_buffer.toBuffer();
            buffer.resize(m_size);
            return buffer;
        }

        return BufferUtils::copy(*this);
    }

    // BufferView BufferView::merge(const BufferView& buffer) const
    //{
    //	return {};
    // }
    //
    // BufferView BufferView::merge(const BufferView& buffer1, const BufferView& buffer2)
    //{
    //	return {};
    // }

    uint32_t BufferUtils::refsCount(const BufferBase& buffer)
    {
        return nau::refsCount(buffer.m_storage);
    }

    uint32_t BufferUtils::refsCount(const BufferView& buffer)
    {
        return BufferUtils::refsCount(buffer.m_buffer);
    }

    BytesBuffer BufferUtils::copy(const BufferBase& source, size_t offset, std::optional<size_t> size)
    {
        NAU_ASSERT(offset <= source.size());
        NAU_ASSERT(!size || offset + *size <= source.size());

        if (!source)
        {
            return {};
        }

        const size_t targetSize = size ? *size : (source.size() - offset);
        std::byte* const targetStorage = BufferStorage::allocate(targetSize, source.header().allocator);
        BytesBuffer buffer{targetStorage};

        const std::byte* const sourceData = clientData(source.m_storage);
        memcpy(buffer.data(), sourceData + offset, targetSize);
        return buffer;
    }

    BytesBuffer BufferUtils::copy(const BufferView& source, size_t offset, std::optional<size_t> size)
    {
        NAU_ASSERT(offset <= source.size());
        NAU_ASSERT(!size || offset + *size <= source.size());

        if (!source)
        {
            return {};
        }

        const ReadOnlyBuffer& sourceBuffer = source.m_buffer;

        const size_t targetSize = size ? *size : (source.size() - offset);
        std::byte* const targetStorage = BufferStorage::allocate(targetSize, sourceBuffer.header().allocator);
        BytesBuffer buffer{targetStorage};

        memcpy(buffer.data(), source.data() + offset, targetSize);
        return buffer;
    }

}  // namespace nau
