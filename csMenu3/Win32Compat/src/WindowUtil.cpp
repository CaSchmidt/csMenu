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

#include "Win32/WindowUtil.h"

namespace window {

  HICON_t loadIcon(const HINSTANCE_t instance, const wchar_t *name)
  {
    return LoadIconW(reinterpret_cast<HINSTANCE>(instance), name);
  }

  bool makeGUIThread()
  {
    const BOOL result = IsGUIThread(TRUE);
    return result != FALSE && result != ERROR_NOT_ENOUGH_MEMORY;
  }

  std::wstring text(const HWND_t wnd)
  {
    const int length = GetWindowTextLengthW(reinterpret_cast<HWND>(wnd));
    if( length < 1 ) {
      return std::wstring();
    }

    std::wstring result;
    try {
      result.resize(length);
    } catch( ... ) {
      return std::wstring();
    }

    if( GetWindowTextW(reinterpret_cast<HWND>(wnd), result.data(), length + 1) < 1 ) {
      return std::wstring();
    }

    return result;
  }

  bool setText(HWND_t wnd, const wchar_t *text)
  {
    return SetWindowTextW(reinterpret_cast<HWND>(wnd), text) != FALSE;
  }

  void unregisterClass(const HINSTANCE_t instance, const wchar_t *name)
  {
    if( instance == nullptr || name == nullptr ) {
      return;
    }
    HINSTANCE hInstance = reinterpret_cast<HINSTANCE>(instance);

    WNDCLASSEXW wcex;
    if( GetClassInfoExW(hInstance, name, &wcex) != FALSE ) {
      UnregisterClassW(name, hInstance);
    }
  }

} // namespace window
