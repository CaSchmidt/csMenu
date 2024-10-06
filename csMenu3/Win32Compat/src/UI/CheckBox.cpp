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

#include "Win32/UI/CheckBox.h"

namespace ui {

  ////// public //////////////////////////////////////////////////////////////

  CheckBox::CheckBox(HWND_t wnd, const ctor_tag&) noexcept
    : Window(wnd)
  {
  }

  CheckBox::CheckBox(HWND_t dlg, int idDlgItem, const ctor_tag&) noexcept
    : Window(dlg, idDlgItem)
  {
  }

  CheckBox::~CheckBox() noexcept
  {
  }

  bool CheckBox::isChecked() const
  {
    return sendMessage(BM_GETCHECK, 0, 0) == BST_CHECKED;
  }

  void CheckBox::setChecked(const bool on)
  {
    const WPARAM wParam = on
                          ? BST_CHECKED
                          : BST_UNCHECKED;
    sendMessage(BM_SETCHECK, wParam, 0);
  }

  LRESULT_t CheckBox::onCommand(WPARAM_t /*wParam*/, LPARAM_t /*lParam*/)
  {
    if( isChecked() ) {
      setChecked(false);
    } else {
      setChecked(true);
    }
    return sendMessage(BM_GETCHECK, 0, 0);
  }

  WindowPtr CheckBox::create(HWND_t wnd)
  {
    return std::make_unique<CheckBox>(wnd);
  }

  WindowPtr CheckBox::create(HWND_t dlg, int idDlgItem)
  {
    return std::make_unique<CheckBox>(dlg, idDlgItem);
  }

} // namespace ui
