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

#define NOMINMAX
#include <Windows.h>

#include <cs/Text/StringRange.h>

#include "Win32/Clipboard.h"

////// Public ////////////////////////////////////////////////////////////////

bool clearClipboard()
{
  if( OpenClipboard(nullptr) == FALSE ) {
    return false;
  }

  const bool ok = EmptyClipboard() != FALSE;
  CloseClipboard();

  return ok;
}

bool setClipboardText(const wchar_t *text)
{
  const std::size_t length = cs::length(text);
  if( length < 1 ) {
    return false;
  }

  if( !clearClipboard() ) {
    return false;
  }

  if( OpenClipboard(nullptr) == FALSE ) {
    return false;
  }

  HGLOBAL globalMem = GlobalAlloc(GMEM_MOVEABLE, (length + 1) * sizeof(wchar_t));
  if( globalMem == nullptr ) {
    CloseClipboard();
    return false;
  }

  wchar_t *globalText = reinterpret_cast<wchar_t *>(GlobalLock(globalMem));
  CopyMemory(globalText, text, length * sizeof(std::size_t));
  globalText[length] = L'\0';
  GlobalUnlock(globalMem);

  const bool ok = SetClipboardData(CF_UNICODETEXT, globalMem) != nullptr;
  CloseClipboard();

  return ok;
}
