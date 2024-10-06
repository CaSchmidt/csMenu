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

#define UNICODE
#include <Windows.h>

#include "Win32/UI/Dialog.h"

#include "DialogResource.h"

namespace ui {

  ////// Private /////////////////////////////////////////////////////////////

  LRESULT CALLBACK DialogProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
  {
    Dialog *ui = reinterpret_cast<Dialog *>(GetWindowLongPtrW(hWnd, GWLP_USERDATA));

    switch( uMsg ) {
    case WM_INITDIALOG:
      ui = reinterpret_cast<Dialog *>(lParam);
      return ui->onInitDialog(hWnd, lParam);

    case WM_COMMAND:
      /*
       * cf. https://docs.microsoft.com/en-us/windows/win32/menurc/wm-command
       *
       * HIWORD(wParam) == Control-defined notification code
       * LOWORD(wParam) == Control identifier
       *        lParam  == Handle to the control window
       */
      if( LOWORD(wParam) == IDOK ) {
        EndDialog(hWnd, IDOK);
        return ui->onOk(wParam, lParam);
      } else if( LOWORD(wParam) == IDCANCEL ) {
        EndDialog(hWnd, IDCANCEL);
        return ui->onCancel(wParam, lParam);
      } else {
        return ui->onCommand(wParam, lParam);
      }
      break; // WM_COMMAND

    default:
      break;
    }

    return FALSE;
  }

  ////// public //////////////////////////////////////////////////////////////

  Dialog::Dialog() noexcept
    : Window(nullptr)
  {
  }

  Dialog::~Dialog() noexcept
  {
  }

  LRESULT_t Dialog::onCommand(WPARAM_t wParam, LPARAM_t lParam)
  {
    for( const WindowPtr& p : _controls ) {
      if( p->controlId() == LOWORD(wParam) ) {
        return p->onCommand(wParam, lParam);
      }
    }
    return FALSE;
  }

  LRESULT_t Dialog::onInitDialog(HWND_t wnd, LPARAM_t lParam)
  {
    setHandle(wnd);
    setUserData(lParam);
    return TRUE;
  }

  LRESULT_t Dialog::onOk(WPARAM_t /*wParam*/, LPARAM_t /*lParam*/)
  {
    return FALSE;
  }

  LRESULT_t Dialog::onCancel(WPARAM_t /*wParam*/, LPARAM_t /*lParam*/)
  {
    return FALSE;
  }

  bool Dialog::exec(HINSTANCE_t instance, HWND_t parent)
  {
    if( !registerWindowClass(instance, wndClassName(), iconName()) ) {
      return false;
    }
    return DialogBoxParamW(HINSTANCE(instance), dialogName(), HWND(parent),
                           DialogProc, reinterpret_cast<LPARAM>(this))
        == IDOK;
  }

  ////// protected ///////////////////////////////////////////////////////////

  bool Dialog::registerWindowClass(HINSTANCE_t instance,
                                   const wchar_t *className, const wchar_t *iconName)
  {
    const HINSTANCE hInst = reinterpret_cast<HINSTANCE>(instance);

    WNDCLASSEXW wcex;
    if( GetClassInfoExW(hInst, className, &wcex) != FALSE ) {
      return true; // Window class is already registered!
    }

    wcex.cbSize        = sizeof(WNDCLASSEXW);
    wcex.style         = 0;
    wcex.lpfnWndProc   = DefDlgProcW;
    wcex.cbClsExtra    = 0;
    wcex.cbWndExtra    = DLGWINDOWEXTRA + sizeof(void *);
    wcex.hInstance     = hInst;
    wcex.hIcon         = LoadIconW(hInst, iconName);
    wcex.hCursor       = LoadCursorW(nullptr, IDC_ARROW);
    wcex.hbrBackground = reinterpret_cast<HBRUSH>(COLOR_WINDOW + 1);
    wcex.lpszMenuName  = nullptr;
    wcex.lpszClassName = className;
    wcex.hIconSm       = LoadIconW(hInst, iconName);

    if( RegisterClassExW(&wcex) == 0 ) {
      MessageBoxW(nullptr, L"RegisterClassExW()", L"Error", MB_ICONERROR | MB_OK);
      return false;
    }

    return true;
  }

  Window *Dialog::getControl(const int id) const
  {
    for( const WindowPtr& p : _controls ) {
      if( p->controlId() == id ) {
        return p.get();
      }
    }
    return nullptr;
  }

  const wchar_t *Dialog::dialogName() const
  {
    return MAKEINTRESOURCEW(IDD_Dialog);
  }

  const wchar_t *Dialog::iconName() const
  {
    return IDI_APPLICATION;
  }

  const wchar_t *Dialog::wndClassName() const
  {
    return WNDCLASS_Dialog;
  }

} // namespace ui
