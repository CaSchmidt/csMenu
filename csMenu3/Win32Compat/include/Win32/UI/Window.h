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

#include <list>
#include <memory>

#include "Win32/Compat.h"

namespace ui {

  using WindowPtr = std::unique_ptr<class Window>;

  class Window {
  public:
    virtual ~Window() noexcept;

    bool isNull() const;

    const HWND_t handle() const;
    HWND_t handle();

    int controlId() const;

    LRESULT_t sendMessage(UINT_t msg, WPARAM_t wParam, LPARAM_t lParam) const;

    LONG_PTR_t userData() const;
    void setUserData(LONG_PTR_t data);

    void setIcon(HICON_t icon);

    virtual LRESULT_t onCommand(WPARAM_t wParam, LPARAM_t lParam); // WM_COMMAND

  protected:
    Window(HWND_t wnd) noexcept;
    Window(HWND_t dlg, int idDlgItem) noexcept;

    void setHandle(HWND_t wnd);

  private:
    HWND_t _wnd{nullptr};
  };

  using Windows = std::list<WindowPtr>;

} // namespace ui
