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

#include <vector>

#define NOMINMAX
#include <Windows.h>

#include "Win32/Registry.h"

#include "Win32/String.h"

namespace reg {

  namespace impl_reg {

    using Buffer = std::vector<uint8_t>;

    constexpr bool IS_READ  = false;
    constexpr bool IS_WRITE = true;

    // Access ////////////////////////////////////////////////////////////////

    HKEY createKey(const HKEY rootKey, const wchar_t *subKey, const bool is_write)
    {
      const REGSAM rights = is_write
                            ? KEY_WRITE
                            : KEY_READ;

      HKEY key      = nullptr;
      const bool ok = RegCreateKeyExW(rootKey, subKey,
                                      0, nullptr, REG_OPTION_NON_VOLATILE,
                                      rights, nullptr, &key, nullptr)
                   == ERROR_SUCCESS;

      return ok
             ? key
             : nullptr;
    }

    // Deletion //////////////////////////////////////////////////////////////

    bool deleteTree(const HKEY rootKey, const wchar_t *subKey)
    {
      return RegDeleteTreeW(rootKey, subKey) == ERROR_SUCCESS;
    }

    // Read //////////////////////////////////////////////////////////////////

    DWORD readDWord(const HKEY rootKey, const wchar_t *subKey, const wchar_t *name,
                    const DWORD defValue, bool *ok)
    {
      if( ok != nullptr ) {
        *ok = false;
      }

      DWORD value = 0;
      DWORD size  = sizeof(value);
      if( RegGetValueW(rootKey, subKey, name, RRF_RT_DWORD, nullptr, &value, &size)
          != ERROR_SUCCESS ) {
        return defValue;
      }

      if( ok != nullptr ) {
        *ok = true;
      }

      return value;
    }

    std::wstring readString(const HKEY rootKey, const wchar_t *subKey, const wchar_t *name,
                            const std::wstring defValue, bool *ok)
    {
      if( ok != nullptr ) {
        *ok = false;
      }

      DWORD size{0};
      if( RegGetValueW(rootKey, subKey, name, RRF_RT_REG_SZ, nullptr, nullptr, &size)
          != ERROR_SUCCESS ) {
        return defValue;
      }

      Buffer buffer;
      try {
        buffer.resize(size, 0);
      } catch( ... ) {
        return defValue;
      }

      if( RegGetValueW(rootKey, subKey, name, RRF_RT_REG_SZ, nullptr, buffer.data(), &size)
          != ERROR_SUCCESS ) {
        return defValue;
      }

      std::wstring value;
      try {
        value.assign(reinterpret_cast<const wchar_t *>(buffer.data()));
      } catch( ... ) {
        return defValue;
      }

      if( ok != nullptr ) {
        *ok = true;
      }

      return value;
    }

    // Write /////////////////////////////////////////////////////////////////

    bool writeDWord(const HKEY rootKey, const wchar_t *subKey, const wchar_t *name,
                    const DWORD value)
    {
      const HKEY key = createKey(rootKey, subKey, IS_WRITE);
      if( key == nullptr ) {
        return false;
      }

      if( RegSetValueExW(key, name, 0, REG_DWORD,
                         reinterpret_cast<const BYTE *>(&value), sizeof(value))
          != ERROR_SUCCESS ) {
        RegCloseKey(key);
        return false;
      }

      RegCloseKey(key);

      return true;
    }

    bool writeString(const HKEY rootKey, const wchar_t *subKey, const wchar_t *name,
                     const wchar_t *value)
    {
      constexpr std::size_t ONE = 1;

      const std::size_t length = value != nullptr
                                 ? StringLength(value)
                                 : 0;
      if( length < ONE ) {
        return false;
      }

      const HKEY key = createKey(rootKey, subKey, IS_WRITE);
      if( key == nullptr ) {
        return false;
      }

      if( RegSetValueExW(key, name, 0, REG_SZ,
                         reinterpret_cast<const BYTE *>(value), (length + ONE) * sizeof(wchar_t))
          != ERROR_SUCCESS ) {
        RegCloseKey(key);
        return false;
      }

      RegCloseKey(key);

      return true;
    }

  } // namespace impl_reg

  ////// Public //////////////////////////////////////////////////////////////

  // HKEY_CURRENT_USER ///////////////////////////////////////////////////////

  bool deleteCurrentUserTree(const wchar_t *key)
  {
    return impl_reg::deleteTree(HKEY_CURRENT_USER, key);
  }

  DWORD_t readCurrentUserDWord(const wchar_t *key, const wchar_t *name,
                               const DWORD_t defValue, bool *ok)
  {
    return impl_reg::readDWord(HKEY_CURRENT_USER, key, name, defValue, ok);
  }

  std::wstring readCurrentUserString(const wchar_t *key, const wchar_t *name,
                                     const std::wstring& defValue, bool *ok)
  {
    return impl_reg::readString(HKEY_CURRENT_USER, key, name, defValue, ok);
  }

  bool writeCurrentUserDWord(const wchar_t *key, const wchar_t *name,
                             const DWORD_t value)
  {
    return impl_reg::writeDWord(HKEY_CURRENT_USER, key, name, value);
  }

  bool writeCurrentUserString(const wchar_t *key, const wchar_t *name,
                              const wchar_t *value)
  {
    return impl_reg::writeString(HKEY_CURRENT_USER, key, name, value);
  }

  // HKEY_LOCAL_MACHINE //////////////////////////////////////////////////////

  bool deleteLocalMachineTree(const wchar_t *key)
  {
    return impl_reg::deleteTree(HKEY_LOCAL_MACHINE, key);
  }

  DWORD_t readLocalMachineDWord(const wchar_t *key, const wchar_t *name,
                                const DWORD_t defValue, bool *ok)
  {
    return impl_reg::readDWord(HKEY_LOCAL_MACHINE, key, name, defValue, ok);
  }

  std::wstring readLocalMachineString(const wchar_t *key, const wchar_t *name,
                                      const std::wstring& defValue, bool *ok)
  {
    return impl_reg::readString(HKEY_LOCAL_MACHINE, key, name, defValue, ok);
  }

  bool writeLocalMachineDWord(const wchar_t *key, const wchar_t *name,
                              const DWORD_t value)
  {
    return impl_reg::writeDWord(HKEY_LOCAL_MACHINE, key, name, value);
  }

  bool writeLocalMachineString(const wchar_t *key, const wchar_t *name,
                               const wchar_t *value)
  {
    return impl_reg::writeString(HKEY_LOCAL_MACHINE, key, name, value);
  }

} // namespace reg
