// Copyright 2024 N-GINN LLC. All rights reserved.
// Use of this source code is governed by a BSD-3 Clause license that can be found in the LICENSE file.


#if defined(_WIN32) || defined(_WIN64)
#include "nau/shared/platform/win/utils.h"

#include <windows.h>
#include "ShellAPI.h";
#include <cstdio>
#include <iostream>
#include <stdexcept>

#include "nau/shared/platform/win/process.h"
#include "objbase.h"
#include "objidl.h"
#include "shlguid.h"
#include "shobjidl.h"
#include "winnls.h"

#include <stdlib.h>  

namespace nau
{
    void WindowsUtils::openFolder(const std::string& path)
    {
        ShellExecute(NULL, NULL, path.c_str(), NULL, NULL, SW_SHOWNORMAL);
    }
    bool WindowsUtils::createLink(const std::string& from, const std::string& to)
    {
        CoInitialize(NULL);

        HRESULT hres;
        IShellLink* psl;

        hres = CoCreateInstance(CLSID_ShellLink, NULL, CLSCTX_INPROC_SERVER, IID_IShellLink, (LPVOID*)&psl);
        if (SUCCEEDED(hres))
        {
            IPersistFile* ppf;

            psl->SetPath(from.c_str());

            hres = psl->QueryInterface(IID_IPersistFile, (LPVOID*)&ppf);

            if (SUCCEEDED(hres))
            {
                WCHAR wsz[MAX_PATH];

                MultiByteToWideChar(CP_ACP, 0, to.c_str(), -1, wsz, MAX_PATH);

                hres = ppf->Save(wsz, TRUE);
                ppf->Release();
            }
            psl->Release();
        }

        CoUninitialize();

        return SUCCEEDED(hres);
    }

    void WindowsUtils::appendPathEnv(const std::string& path)
    {
		std::string envPath = getenv("PATH");
		envPath += ";";
		envPath += path;
		setenv("PATH", envPath.c_str(), 1);
    }

    int WindowsUtils::setenv(const char* name, const char* value, int overwrite)
    {
        int errcode = 0;
        if (!overwrite)
        {
            size_t envsize = 0;
            errcode = getenv_s(&envsize, NULL, 0, name);
            if (errcode || envsize)
                return errcode;
        }
        return _putenv_s(name, value);
    }
}  // namespace nau
#endif