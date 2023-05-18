/****************************************************************************
** Copyright (c) 2023, Carsten Schmidt. All rights reserved.
**
** Redistribution and use in source and binary forms, with or without
** modification, are permitted provided that the following conditions
** are met:
**
** 1. Redistributions of source code must retain the above copyright
**    notice, this list of conditions and the following disclaimer.
**
** 2. Redistributions in binary form must reproduce the above copyright
**    notice, this list of conditions and the following disclaimer in the
**    documentation and/or other materials provided with the distribution.
**
** 3. Neither the name of the copyright holder nor the names of its
**    contributors may be used to endorse or promote products derived from
**    this software without specific prior written permission.
**
** THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
** "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
** LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
** A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
** HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
** SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
** LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
** DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
** THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
** (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
** OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*****************************************************************************/

#include <Windows.h>

#include "Win32/Registry.h"

namespace reg {

  namespace impl_reg {

    constexpr bool IS_READ = false;
    constexpr bool IS_WRITE = true;

    HKEY createKey(const HKEY rootKey, const wchar_t *subKey, const bool is_write)
    {
      const REGSAM rights = is_write
                            ? KEY_WRITE
                            : KEY_READ;

      HKEY key = nullptr;
      const HRESULT hr = HRESULT_FROM_WIN32(RegCreateKeyExW(rootKey, subKey,
                                                            0, nullptr, REG_OPTION_NON_VOLATILE,
                                                            rights, nullptr, &key, nullptr));

      return SUCCEEDED(hr)
             ? key
             : nullptr;
    }

    DWORD readDWord(const HKEY rootKey, const wchar_t *subKey, const wchar_t *name,
                    const DWORD defValue, bool *ok)
    {
      if( ok != nullptr ) {
        *ok = false;
      }

      const HKEY key = createKey(rootKey, subKey, IS_READ);
      if( key == nullptr ) {
        return defValue;
      }

      DWORD value = 0;
      DWORD size = sizeof(value);
      const HRESULT hr = HRESULT_FROM_WIN32(RegGetValueW(key, nullptr, name,
                                                         RRF_RT_DWORD, nullptr,
                                                         &value, &size));
      RegCloseKey(key);

      if( FAILED(hr) ) {
        return defValue;
      }

      if( ok != nullptr ) {
        *ok = true;
      }

      return value;
    }

    bool writeDWord(const HKEY rootKey, const wchar_t *subKey, const wchar_t *name, const DWORD value)
    {
      const HKEY key = createKey(rootKey, subKey, IS_WRITE);
      if( key == nullptr ) {
        return false;
      }

      const HRESULT hr = HRESULT_FROM_WIN32(RegSetValueExW(key, name, 0, REG_DWORD,
                                                           reinterpret_cast<const BYTE *>(&value),
                                                           sizeof(value)));
      RegCloseKey(key);

      if( FAILED(hr) ) {
        return false;
      }

      return true;
    }

  } // namespace impl_reg

  DWORD_t readLocalMachineDWord(const wchar_t *key, const wchar_t *name,
                                const DWORD_t defValue, bool *ok)
  {
    return impl_reg::readDWord(HKEY_LOCAL_MACHINE, key, name, defValue, ok);
  }

  DWORD_t readCurrentUserDWord(const wchar_t *key, const wchar_t *name,
                               const DWORD_t defValue, bool *ok)
  {
    return impl_reg::readDWord(HKEY_CURRENT_USER, key, name, defValue, ok);
  }

  bool writeLocalMachineDWord(const wchar_t *key, const wchar_t *name,
                              const DWORD_t value)
  {
    return impl_reg::writeDWord(HKEY_LOCAL_MACHINE, key, name, value);
  }

  bool writeCurrentUserDWord(const wchar_t *key, const wchar_t *name,
                             const DWORD_t value)
  {
    return impl_reg::writeDWord(HKEY_CURRENT_USER, key, name, value);
  }

} // namespace reg
