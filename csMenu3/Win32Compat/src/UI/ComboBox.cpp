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

#include "Win32/UI/ComboBox.h"

namespace ui {

  ////// Private /////////////////////////////////////////////////////////////

  constexpr LRESULT_t ZERO = 0;

  ////// public //////////////////////////////////////////////////////////////

  ComboBox::ComboBox(HWND_t wnd, const ctor_tag&) noexcept
    : Window(wnd)
  {
  }

  ComboBox::ComboBox(HWND_t dlg, int idDlgItem, const ctor_tag&) noexcept
    : Window(dlg, idDlgItem)
  {
  }

  ComboBox::~ComboBox() noexcept
  {
  }

  bool ComboBox::addItem(const std::wstring& item)
  {
    if( item.empty() ) {
      return false;
    }
    return sendMessage(CB_ADDSTRING, 0, reinterpret_cast<LPARAM_t>(item.data())) >= ZERO;
  }

  void ComboBox::clear()
  {
    sendMessage(CB_RESETCONTENT, 0, 0);
  }

  LRESULT_t ComboBox::count() const
  {
    const LRESULT_t result = sendMessage(CB_GETCOUNT, 0, 0);
    if( result < ZERO ) {
      return 0;
    }
    return result;
  }

  LRESULT_t ComboBox::currentIndex() const
  {
    return sendMessage(CB_GETCURSEL, 0, 0);
  }

  bool ComboBox::setCurrentIndex(const WPARAM_t index)
  {
    return sendMessage(CB_SETCURSEL, index, 0) >= ZERO;
  }

  std::wstring ComboBox::currentText() const
  {
    const LRESULT_t index = sendMessage(CB_GETCURSEL, 0, 0);
    if( index < ZERO ) {
      return std::wstring();
    }

    const LRESULT_t length = sendMessage(CB_GETLBTEXTLEN, index, 0);
    if( length <= ZERO ) {
      return std::wstring();
    }

    std::wstring result;
    try {
      result.resize(length);
    } catch( ... ) {
      return std::wstring();
    }

    if( sendMessage(CB_GETLBTEXT, index, reinterpret_cast<LPARAM_t>(result.data())) <= ZERO ) {
      return std::wstring();
    }

    return result;
  }

  ////// public static ///////////////////////////////////////////////////////

  WindowPtr ComboBox::create(HWND_t wnd)
  {
    return std::make_unique<ComboBox>(wnd);
  }

  WindowPtr ComboBox::create(HWND_t dlg, int idDlgItem)
  {
    return std::make_unique<ComboBox>(dlg, idDlgItem);
  }

} // namespace ui
