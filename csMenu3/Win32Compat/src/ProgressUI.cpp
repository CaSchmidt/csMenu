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
#include <CommCtrl.h>

#include "Win32/ProgressUI.h"

////// Implementation ////////////////////////////////////////////////////////

namespace impl_prog {

  void errorMessage(const wchar_t *text)
  {
    MessageBoxW(nullptr, text, L"Error", MB_OK | MB_ICONERROR);
  }

} // namespace impl_prog

////// private ///////////////////////////////////////////////////////////////

ProgressUI::ctor_tag::ctor_tag() noexcept = default;

////// public ////////////////////////////////////////////////////////////////

ProgressUI::ProgressUI(const ctor_tag&) noexcept
  : _hMainWnd{nullptr}
  , _hProgBar{nullptr}
{
}

ProgressUI::~ProgressUI() noexcept
{
}

void ProgressUI::close()
{
  if( _hMainWnd == nullptr ) {
    return;
  }
  HWND hWnd = reinterpret_cast<HWND>(_hMainWnd);

  PostMessageW(hWnd, WM_CLOSE, 0, 0);
}

void ProgressUI::destroy()
{
  if( _hMainWnd == nullptr ) {
    return;
  }
  HWND hWnd = reinterpret_cast<HWND>(_hMainWnd);

  DestroyWindow(hWnd);
}

void ProgressUI::show()
{
  if( _hMainWnd == nullptr ) {
    return;
  }
  HWND hWnd = reinterpret_cast<HWND>(_hMainWnd);

  ShowWindow(hWnd, SW_SHOWNORMAL);
}

void ProgressUI::setRange(const int lo, const int hi)
{
  if( lo < 0 || hi < 0 || hi <= lo ) {
    return;
  }

  if( _hProgBar == nullptr ) {
    return;
  }
  HWND hWnd = reinterpret_cast<HWND>(_hProgBar);

  PostMessageW(hWnd, PBM_SETRANGE32, (WPARAM)lo, (LPARAM)hi);
  PostMessageW(hWnd, PBM_SETPOS, (WPARAM)lo, 0);
  PostMessageW(hWnd, PBM_SETSTEP, (WPARAM)1, 0);
}

void ProgressUI::step()
{
  if( _hProgBar == nullptr ) {
    return;
  }
  HWND hWnd = reinterpret_cast<HWND>(_hProgBar);

  PostMessageW(hWnd, PBM_STEPIT, 0, 0);
}

////// public static /////////////////////////////////////////////////////////

ProgressUIptr ProgressUI::make(const HANDLE_t hInstance, const int width, const int height)
{
  if( hInstance == nullptr || width < 1 || height < 1 ) {
    return ProgressUIptr{};
  }
  HINSTANCE hInst = reinterpret_cast<HINSTANCE>(hInstance);

  // (1) Create Instance /////////////////////////////////////////////////////

  ProgressUIptr result;
  try {
    result = std::make_unique<ProgressUI>(ctor_tag{});
  } catch( ... ) {
    return ProgressUIptr{};
  }

  // (2) Create Window Class /////////////////////////////////////////////////

  if( !registerWindowClass(hInstance) ) {
    return ProgressUIptr{};
  }

  // (3) Create Main Window //////////////////////////////////////////////////

  result->_hMainWnd = CreateWindowExW(0, windowClassName(), L"Progress", WS_BORDER,
                                      CW_USEDEFAULT, CW_USEDEFAULT,
                                      width, height + GetSystemMetrics(SM_CYSIZE),
                                      nullptr, nullptr, hInst, nullptr);
  if( result->_hMainWnd == nullptr ) {
    impl_prog::errorMessage(L"CreateWindowExW(Main)");
    return ProgressUIptr{};
  }

  // (4) Initialize Common Controls //////////////////////////////////////////

  INITCOMMONCONTROLSEX iccex;
  iccex.dwSize = sizeof(iccex);
  iccex.dwICC  = ICC_PROGRESS_CLASS;
  if( InitCommonControlsEx(&iccex) == FALSE ) {
    result->destroy();
    impl_prog::errorMessage(L"InitCommonControlsEx()");
    return ProgressUIptr{};
  }

  // (5) Create Progress Bar /////////////////////////////////////////////////

  RECT rect;
  GetClientRect(reinterpret_cast<HWND>(result->_hMainWnd), &rect);

  result->_hProgBar = CreateWindowExW(0, PROGRESS_CLASSW, nullptr, WS_CHILD | WS_VISIBLE | PBS_SMOOTH,
                                      rect.left, rect.top, rect.right - rect.left, rect.bottom - rect.top,
                                      reinterpret_cast<HWND>(result->_hMainWnd), nullptr, hInst, nullptr);
  if( result->_hProgBar == nullptr ) {
    result->destroy();
    impl_prog::errorMessage(L"CreateWindowExW(Prog)");
    return ProgressUIptr{};
  }

  return result;
}

const wchar_t *ProgressUI::windowClassName()
{
  return L"Win32::ProgressUI";
}

////// private static ////////////////////////////////////////////////////////

bool ProgressUI::registerWindowClass(const HANDLE_t hInstance)
{
  // (0) Sanity Check ////////////////////////////////////////////////////////

  if( hInstance == nullptr ) {
    return false;
  }
  HINSTANCE hInst = reinterpret_cast<HINSTANCE>(hInstance);

  // (1) Check for Existing Class ////////////////////////////////////////////

  WNDCLASSEXW wcex;
  if( GetClassInfoExW(hInst, windowClassName(), &wcex) != FALSE ) {
    return true;
  }

  // (2) Setup Class /////////////////////////////////////////////////////////

  wcex.cbSize        = sizeof(wcex);
  wcex.style         = 0;
  wcex.lpfnWndProc   = DefWindowProc; // TODO: Custom Window Procedure...
  wcex.cbClsExtra    = 0;
  wcex.cbWndExtra    = 0; // TODO: Allocate Extra Data...
  wcex.hInstance     = hInst;
  wcex.hIcon         = LoadIcon(nullptr, IDI_APPLICATION);
  wcex.hCursor       = LoadCursor(nullptr, IDC_ARROW);
  wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
  wcex.lpszMenuName  = nullptr;
  wcex.lpszClassName = windowClassName();
  wcex.hIconSm       = LoadIcon(nullptr, IDI_APPLICATION);

  // (3) Register Class //////////////////////////////////////////////////////

  if( RegisterClassExW(&wcex) == 0 ) {
    impl_prog::errorMessage(L"RegisterClassExW()");
    return false;
  }

  return true;
}
