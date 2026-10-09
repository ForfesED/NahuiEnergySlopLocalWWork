// Copyright 2024 N-GINN LLC. All rights reserved.
// Use of this source code is governed by a BSD-3 Clause license that can be found in the LICENSE file.


#pragma once
#include "nau/kernel/kernel_config.h"
#include "nau/memory/mem_allocator.h"
#include "nau/memory/mem_section_ptr.h"
#include "nau/threading/thread_local_value.h"
#include "nau/rtti/ptr.h"
#include <thread>
#include <mutex>
#include <EASTL/shared_ptr.h>
#include <EASTL/stack.h>
#include <EASTL/set.h>
#include <EASTL/string.h>
#include <EASTL/unordered_map.h>

namespace nau
{ 
    /**
     * @brief Heap allocator for managing memory sections.
     */
    class NAU_KERNEL_EXPORT HeapAllocator final
    {
    public:

        /**
         * @brief Gets the singleton instance of the HeapAllocator.
         * 
         * @return Reference to the HeapAllocator instance.
         */
        [[nodiscard]] static HeapAllocator& instance();

        /**
         * @brief Gets a memory section of the specified kind.
         * 
         * @param kind The kind of memory section to get.
         * @return A MemSectionPtr to the memory section.
         */
        [[nodiscard]] MemSectionPtr getSection(const eastl::string& kind);

        /**
         * @brief Releases a memory section.
         * 
         * @param ptr A MemSectionPtr to the memory section to release.
         */
        void releaseSection(MemSectionPtr& ptr);

    private:
        HeapAllocator()
            : m_allocs([](auto& val) {val = 0; })
        {}
        ~HeapAllocator() = default;

        using SectionMap = eastl::unordered_map<eastl::string, MemSection>;
        using SectionMapPtr = eastl::shared_ptr<SectionMap>;
        using SectionStack = eastl::stack<SectionMapPtr>;
        using MemSectionStack = eastl::stack<MemSection*>;
        using MemSectionsMap = eastl::unordered_map<eastl::string, MemSectionStack>;
        using MemSectionsMapPtr = eastl::shared_ptr<MemSectionsMap>;
        
        eastl::set<SectionMapPtr> m_sections;
        bool m_readyToRelease = false;
        ThreadLocalValue<int> m_allocs;

        std::mutex m_sync;
        ThreadLocalValue<MemSectionsMapPtr> m_freeSectionsPool;
        ThreadLocalValue<SectionMapPtr> m_freeSectionsMaps;

        MemSectionsMap& freeMemSectionsPool();
        SectionMap& getSectionsMap();
        void releasePools();
    };

} // namespace nau

