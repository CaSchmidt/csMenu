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
#include <winnetwk.h>

#include "Win32/Network.h"

std::wstring resolveUniversalName(const wchar_t *filename)
{
  using Buffer = std::vector<uint8_t>;

  UNIVERSAL_NAME_INFOW info;
  DWORD sizInfo = sizeof(info);
  if( WNetGetUniversalNameW(filename, UNIVERSAL_NAME_INFO_LEVEL,
                            &info, &sizInfo)
      != ERROR_MORE_DATA ) {
    return std::wstring{};
  }
  sizInfo += sizeof(wchar_t);

  Buffer buffer;
  try {
    buffer.resize(sizInfo, 0);
  } catch( ... ) {
    return std::wstring{};
  }

  if( WNetGetUniversalNameW(filename, UNIVERSAL_NAME_INFO_LEVEL,
                            buffer.data(), &sizInfo)
      != NO_ERROR ) {
    return std::wstring{};
  }

  const UNIVERSAL_NAME_INFOW *ptrInfo = reinterpret_cast<const UNIVERSAL_NAME_INFOW *>(buffer.data());
  const std::wstring result{ptrInfo->lpUniversalName};

  return result;
}
