/****************************************************************************
** Copyright (c) 2024, Carsten Schmidt. All rights reserved.
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

#pragma once

#include <string>

#include "Win32/UI/Window.h"

namespace ui {

  class EditText : public Window {
  private:
    struct ctor_tag {
      ctor_tag() noexcept = default;
    };

  public:
    EditText(HWND_t wnd, const ctor_tag& = ctor_tag()) noexcept;
    EditText(HWND_t dlg, int idDlgItem, const ctor_tag& = ctor_tag()) noexcept;
    ~EditText() noexcept;

    void setPasswordChar(const wchar_t ch);
    bool setReadOnly(const bool on);

    std::wstring text() const;
    bool setText(const std::wstring& text);

    static WindowPtr create(HWND_t wnd);
    static WindowPtr create(HWND_t dlg, int idDlgItem);
  };

  inline EditText *EDITTEXT(const Window *w)
  {
    return dynamic_cast<EditText *>(const_cast<Window *>(w));
  }

  inline EditText *EDITTEXT(const WindowPtr& p)
  {
    return dynamic_cast<EditText *>(p.get());
  }

} // namespace ui
