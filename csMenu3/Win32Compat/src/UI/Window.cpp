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

#include <Windows.h>

#include "Win32/UI/Window.h"

namespace ui {

  ////// public //////////////////////////////////////////////////////////////

  Window::~Window() noexcept
  {
  }

  bool Window::isNull() const
  {
    return _wnd == nullptr;
  }

  HWND_t Window::handle() const
  {
    return _wnd;
  }

  int Window::controlId() const
  {
    return GetDlgCtrlID(reinterpret_cast<HWND>(_wnd));
  }

  LRESULT_t Window::sendMessage(UINT_t msg, WPARAM_t wParam, LPARAM_t lParam) const
  {
    return SendMessageW(reinterpret_cast<HWND>(_wnd), msg, wParam, lParam);
  }

  LONG_PTR_t Window::userData() const
  {
    return GetWindowLongPtrW(reinterpret_cast<HWND>(_wnd), GWLP_USERDATA);
  }

  void Window::setUserData(LONG_PTR_t data)
  {
    SetWindowLongPtrW(reinterpret_cast<HWND>(_wnd), GWLP_USERDATA, data);
  }

  void Window::setIcon(HICON_t icon)
  {
    SetWindowLongPtrW(reinterpret_cast<HWND>(_wnd), GCLP_HICON, reinterpret_cast<LONG_PTR>(icon));
  }

  LRESULT_t Window::onCommand(WPARAM_t /*wParam*/, LPARAM_t /*lParam*/)
  {
    return FALSE;
  }

  ////// protected ///////////////////////////////////////////////////////////

  Window::Window(HWND_t wnd) noexcept
    : _wnd{wnd}
  {
  }

  Window::Window(HWND_t dlg, int idDlgItem) noexcept
  {
    _wnd = GetDlgItem(reinterpret_cast<HWND>(dlg), idDlgItem);
  }

  void Window::setHandle(HWND_t wnd)
  {
    _wnd = wnd;
  }

} // namespace ui
