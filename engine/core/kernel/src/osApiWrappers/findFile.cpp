// Copyright 2024 N-GINN LLC. All rights reserved.

// Copyright (C) 2024  Gaijin Games KFT.  All rights reserved
#include <io.h>
#include <nau/osApiWrappers/basePath.h>
#include <nau/osApiWrappers/dag_direct.h>
#include <nau/threading/critical_section.h>
#include <nau/utils/dag_globDef.h>
#include <string.h>

#include "fs_hlp.h"

namespace nau::hal
{
    // TODO: Replace with string fucntions
    char* wcs_to_utf8(const wchar_t* wcs_str, char* utf8_buf, int utf8_buf_len)
    {
        int cnt = WideCharToMultiByte(CP_UTF8, 0, wcs_str, -1, utf8_buf, utf8_buf_len, NULL, NULL);
        if(!cnt)
            return NULL;
        utf8_buf[cnt < utf8_buf_len ? cnt : utf8_buf_len - 1] = L'\0';
        return utf8_buf;
    }
    wchar_t* utf8_to_wcs(const char* utf8_str, wchar_t* wcs_buf, int wcs_buf_len)
    {
        NAU_ASSERT(wcs_buf_len > 0);
        int cnt = MultiByteToWideChar(CP_UTF8, 0, utf8_str, -1, wcs_buf, wcs_buf_len);
        wcs_buf[wcs_buf_len - 1] = L'\0';
        if (!cnt)
            return NULL;
        if (cnt < wcs_buf_len)
            wcs_buf[cnt] = L'\0';
        return wcs_buf;
    }
    int utf8_to_wcs_ex(const char* utf8_str, int utf8_len, wchar_t* wcs_buf, int wcs_buf_len)
    {
        if(utf8_len == 0)
        {
            if(wcs_buf_len > 0)
                wcs_buf[0] = L'\0';
            return 0;
        }

        int cnt = MultiByteToWideChar(CP_UTF8, 0, utf8_str, utf8_len, wcs_buf, wcs_buf_len);
        if(!cnt)
            return 0;
        wcs_buf[cnt < wcs_buf_len ? cnt : wcs_buf_len - 1] = L'\0';
        return cnt;
    }
    wchar_t* convert_path_to_u16_c(wchar_t* dest_u16, int dest_sz, const char* s, int len = -1)
    {
        if(len < 0)
            len = (int)strlen(s);

        if(len == 0 && dest_sz >= 1)
        {
            dest_u16[0] = 0;
            return dest_u16;
        }

#if _TARGET_PC_WIN && defined(_MSC_VER)
        int new_sz = MultiByteToWideChar(CP_UTF8, MB_ERR_INVALID_CHARS, s, len + 1, dest_u16, dest_sz);
        if(!new_sz)
            new_sz = MultiByteToWideChar(CP_ACP, 0, s, len + 1, dest_u16, dest_sz);
#else
        int new_sz = utf8_to_wcs_ex(s, len, dest_u16, dest_sz);
#endif
        return new_sz ? dest_u16 : NULL;
    }

    struct RealFind
    {
        intptr_t h;
        struct _wfinddata_t fd;
        char subdir;

        RealFind()
        {
            memset(this, 0, sizeof(*this));
            h = -1;
        }

    public:
        static constexpr int MAX_RF = 32;
        static RealFind rfPool[MAX_RF];
        static dag::CriticalSection critSec;

        static RealFind* open()
        {
            dag::CSAutoLock lock(critSec);
            for(int i = 0; i < MAX_RF; i++)
                if(rfPool[i].h < 0)
                {
                    memset(&rfPool[i], 0, sizeof(rfPool[i]));
                    return &rfPool[i];
                }
            return NULL;
        }
        static bool close(RealFind* rd)
        {
            dag::CSAutoLock lock(critSec);
            if(rd < &rfPool[0] || rd >= &rfPool[MAX_RF])
                return false;

            int i = rd - &rfPool[0];
            NAU_ASSERT((i >= 0) && (i < MAX_RF));
            if(rfPool[i].h < 0)
                return false;

            if(rfPool[i].h > 0)
                _findclose(rfPool[i].h);
            memset(&rfPool[i], 0, sizeof(rfPool[i]));
            rfPool[i].h = -1;
            return true;
        }

        void copyTo(alefind_t* fs) const
        {
            fs->attr = 0;
#define CVT_BIT(a, b) \
    if(fd.attrib & a) \
    fs->attr |= b
            CVT_BIT(_A_SUBDIR, DA_SUBDIR);
            CVT_BIT(_A_RDONLY, DA_READONLY);
            CVT_BIT(_A_HIDDEN, DA_HIDDEN);
            CVT_BIT(_A_SYSTEM, DA_SYSTEM);
#undef CVT_BIT
            wcs_to_utf8(fd.name, fs->name, sizeof(fs->name));
            fs->size = fd.size;
            fs->atime = fd.time_access;
            fs->mtime = fd.time_write;
            fs->ctime = fd.time_create;
        }
    };

    RealFind RealFind::rfPool[MAX_RF];
    dag::CriticalSection RealFind::critSec;

    static int bp_find_first(const char* mask, char attr, alefind_t* fs, const char* basepath)
    {
        if(!fs)
            return 0;
        fs->data = NULL;
        RealFind* rf = RealFind::open();
        if(!rf)
            return 0;

        static constexpr int MASK_SZ = DAGOR_MAX_PATH;
        static wchar_t msk[MASK_SZ];
        static wchar_t mask_w[MASK_SZ];
        if(!convert_path_to_u16_c(msk, MASK_SZ, basepath))
        {
            RealFind::close(rf);
            return 0;
        }
        if(!convert_path_to_u16_c(mask_w, MASK_SZ, mask))
        {
            RealFind::close(rf);
            return 0;
        }
        msk[MASK_SZ - 2] = 0;
        wcsncat(msk, mask_w, MASK_SZ - 1 - wcslen(msk) - 1);  //-V645
        intptr_t h = _wfindfirst(msk, &rf->fd);
        if(h < 0)
        {
            RealFind::close(rf);
            return 0;
        }
        rf->h = h;
        while((rf->fd.attrib & _A_SUBDIR) && is_special_dir(rf->fd.name))
        {
            if(_wfindnext(rf->h, &rf->fd))
            {
                RealFind::close(rf);
                return 0;
            }
        }
        if(!(attr & DA_SUBDIR))
        {
            while(rf->fd.attrib & _A_SUBDIR)
            {
                if(_wfindnext(rf->h, &rf->fd))
                {
                    RealFind::close(rf);
                    return 0;
                }
            }
        }
        rf->subdir = (attr & DA_SUBDIR) ? 1 : 0;
        fs->data = rf;
        rf->copyTo(fs);
        return 1;
    }

    static int bp_find_next(alefind_t* fs)
    {
        RealFind* rf = (RealFind*)fs->data;
        if(!rf)
            return 0;
        if(_wfindnext(rf->h, &rf->fd))
            return 0;
        while((rf->fd.attrib & _A_SUBDIR) && is_special_dir(rf->fd.name))
        {
            if(_wfindnext(rf->h, &rf->fd))
                return 0;
        }
        if(!rf->subdir)
        {
            while(rf->fd.attrib & _A_SUBDIR)
            {
                if(_wfindnext(rf->h, &rf->fd))
                    return 0;
            }
        }
        rf->copyTo(fs);
        return 1;
    }

    static int bp_find_close(alefind_t* fs)
    {
        if(!fs)
            return 0;
        if(fs->data)
        {
            RealFind* rf = (RealFind*)fs->data;
            RealFind::close(rf);
            fs->data = NULL;
        }
        return 1;
    }

    extern "C" int dd_find_first(const char* mask, char attr, alefind_t* fs)
    {
        if(!fs)
            return 0;
        fs->grp = -1;
        fs->fattr = attr;

        resolve_named_mount_s(fs->fmask, sizeof(fs->fmask), mask);
        mask = fs->fmask;

        if(is_path_abs(mask))
        {
            if(bp_find_first(mask, attr, fs, ""))
            {
                fs->grp = 0;
                return 1;
            }
        }
        else
            for(int i = 0; i < DF_MAX_BASE_PATH_NUM; i++)
                if(df_base_path[i])
                {
                    if(bp_find_first(mask, attr, fs, df_base_path[i]))
                    {
                        fs->grp = i;
                        return 1;
                    }
                }
                else
                    break;

        bp_find_close(fs);
        return 0;
    }

    extern "C" int dd_find_next(alefind_t* fs)
    {
        if(bp_find_next(fs))
            return 1;

        if(is_path_abs(fs->fmask))
            return 0;

        static alefind_t f;
        f = *fs;

        for(int i = fs->grp + 1; i < DF_MAX_BASE_PATH_NUM; i++)
            if(df_base_path[i])
            {
                if(bp_find_first(fs->fmask, fs->fattr, &f, df_base_path[i]))
                {
                    bp_find_close(fs);
                    *fs = f;
                    fs->grp = i;
                    return 1;
                }
            }
            else
                break;
        return 0;
    }

    extern "C" int dd_find_close(alefind_t* fs)
    {
        if(fs->grp < 0 || fs->grp >= DF_MAX_BASE_PATH_NUM)
            return 0;
        return bp_find_close(fs);
    }
}  // namespace nau::hal

NAU_KERNEL_EXPORT wchar_t* utf8_to_wcs(const char* utf8_str, wchar_t* wcs_buf, int wcs_buf_len)
{
    return nau::hal::utf8_to_wcs(utf8_str, wcs_buf, wcs_buf_len);
}
