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

#include "Win32/Window.h"

namespace window {

  class Dialog : public Window {
  public:
    Dialog() noexcept;
    ~Dialog() noexcept;

    LRESULT_t onCommand(WPARAM_t wParam, LPARAM_t lParam);
    virtual LRESULT_t onInitDialog(HWND_t wnd, LPARAM_t lParam); // WM_INITDIALOG
    virtual LRESULT_t onOk(WPARAM_t wParam, LPARAM_t lParam);
    virtual LRESULT_t onCancel(WPARAM_t wParam, LPARAM_t lParam);

    bool exec(HINSTANCE_t instance, HWND_t parent = nullptr);

  protected:
    static bool registerWindowClass(HINSTANCE_t instance,
                                    const wchar_t *className, const wchar_t *iconName);

    Window *getControl(const int id) const;
    virtual const wchar_t *dialogName() const;
    virtual const wchar_t *iconName() const;
    virtual const wchar_t *wndClassName() const;

    Windows _controls;
  };

} // namespace window
