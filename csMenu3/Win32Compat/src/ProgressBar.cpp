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

#include "Win32/ProgressBar.h"

////// Implementation ////////////////////////////////////////////////////////

namespace impl_prog {

  void errorMessage(const wchar_t *text)
  {
    MessageBoxW(nullptr, text, L"Error", MB_OK | MB_ICONERROR);
  }

} // namespace impl_prog

////// Private ///////////////////////////////////////////////////////////////

class ProgressBarPrivate {
public:
  ProgressBarPrivate() noexcept
  {
  }

  ~ProgressBarPrivate() noexcept
  {
    if( hMainWnd != nullptr ) {
      DestroyWindow(hMainWnd);
    }
    hMainWnd = hProgWnd = nullptr;
  }

  static LRESULT CALLBACK WindowProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
  {
    ProgressBarPrivate *d = reinterpret_cast<ProgressBarPrivate *>(GetWindowLongPtrW(hWnd, 0));

    LRESULT result = 0;
    switch( msg ) {
    case WM_CLOSE:
      DestroyWindow(d->hMainWnd);
      break;

    case WM_DESTROY:
      d->hMainWnd = d->hProgWnd = nullptr;
      PostQuitMessage(0);
      break;

    default:
      result = DefWindowProcW(hWnd, msg, wParam, lParam);
      break;
    }

    return result;
  }

  HWND hMainWnd{nullptr};
  HWND hProgWnd{nullptr};
};

////// private ///////////////////////////////////////////////////////////////

ProgressBar::ctor_tag::ctor_tag() noexcept = default;

////// public ////////////////////////////////////////////////////////////////

ProgressBar::ProgressBar(const ctor_tag&) noexcept
{
  try {
    d = std::make_unique<ProgressBarPrivate>();
  } catch( ... ) {
    d.reset();
  }
}

ProgressBar::~ProgressBar() noexcept
{
}

void ProgressBar::close() const
{
  PostMessageW(d->hMainWnd, WM_CLOSE, 0, 0);
}

void ProgressBar::show() const
{
  ShowWindow(d->hMainWnd, SW_SHOWNORMAL);
}

void ProgressBar::setRange(const int lo, const int hi)
{
  if( lo < 0 || hi < 0 || hi <= lo ) {
    return;
  }

  PostMessageW(d->hProgWnd, PBM_SETRANGE32, (WPARAM)lo, (LPARAM)hi);
  PostMessageW(d->hProgWnd, PBM_SETPOS, (WPARAM)lo, 0);
  PostMessageW(d->hProgWnd, PBM_SETSTEP, (WPARAM)1, 0);
}

void ProgressBar::step() const
{
  PostMessageW(d->hProgWnd, PBM_STEPIT, 0, 0);
}

////// public static /////////////////////////////////////////////////////////

ProgressBarPtr ProgressBar::make(const HANDLE_t ptrInstance, const int width, const int height)
{
  if( ptrInstance == nullptr || width < 1 || height < 1 ) {
    return ProgressBarPtr{};
  }
  HINSTANCE hInstance = reinterpret_cast<HINSTANCE>(ptrInstance);

  // (1) Create Instance /////////////////////////////////////////////////////

  ProgressBarPtr result;
  try {
    result = std::make_unique<ProgressBar>(ctor_tag{});
  } catch( ... ) {
    return ProgressBarPtr{};
  }

  if( !result->d ) {
    return ProgressBarPtr{};
  }

  // (2) Create Window Class /////////////////////////////////////////////////

  if( !registerWindowClass(ptrInstance) ) {
    return ProgressBarPtr{};
  }

  // (3) Create Main Window //////////////////////////////////////////////////

  result->d->hMainWnd = CreateWindowExW(0, windowClassName(), L"Progress", WS_BORDER,
                                        CW_USEDEFAULT, CW_USEDEFAULT,
                                        width, height + GetSystemMetrics(SM_CYSIZE),
                                        nullptr, nullptr, hInstance, nullptr);
  if( result->d->hMainWnd == nullptr ) {
    impl_prog::errorMessage(L"CreateWindowExW(Main)");
    return ProgressBarPtr{};
  }

  SetWindowLongPtrW(result->d->hMainWnd, 0, reinterpret_cast<LONG_PTR>(result->d.get()));

  // (4) Initialize Common Controls //////////////////////////////////////////

  INITCOMMONCONTROLSEX iccex;
  iccex.dwSize = sizeof(iccex);
  iccex.dwICC  = ICC_PROGRESS_CLASS;
  if( InitCommonControlsEx(&iccex) == FALSE ) {
    impl_prog::errorMessage(L"InitCommonControlsEx()");
    return ProgressBarPtr{};
  }

  // (5) Create Progress Bar /////////////////////////////////////////////////

  RECT rect;
  GetClientRect(result->d->hMainWnd, &rect);

  result->d->hProgWnd = CreateWindowExW(0, PROGRESS_CLASSW, nullptr, WS_CHILD | WS_VISIBLE | PBS_SMOOTH,
                                        rect.left, rect.top, rect.right - rect.left, rect.bottom - rect.top,
                                        result->d->hMainWnd, nullptr, hInstance, nullptr);
  if( result->d->hProgWnd == nullptr ) {
    impl_prog::errorMessage(L"CreateWindowExW(Prog)");
    return ProgressBarPtr{};
  }

  return result;
}

UINT_t ProgressBar::getStepItMessage()
{
  return PBM_STEPIT;
}

const wchar_t *ProgressBar::windowClassName()
{
  return L"Win32::ProgressBar";
}

////// private static ////////////////////////////////////////////////////////

bool ProgressBar::registerWindowClass(const HANDLE_t ptrInstance)
{
  // (0) Sanity Check ////////////////////////////////////////////////////////

  if( ptrInstance == nullptr ) {
    return false;
  }
  HINSTANCE hInstance = reinterpret_cast<HINSTANCE>(ptrInstance);

  // (1) Check for Existing Class ////////////////////////////////////////////

  WNDCLASSEXW wcex;
  if( GetClassInfoExW(hInstance, windowClassName(), &wcex) != FALSE ) {
    return true;
  }

  // (2) Setup Class /////////////////////////////////////////////////////////

  wcex.cbSize        = sizeof(wcex);
  wcex.style         = 0;
  wcex.lpfnWndProc   = ProgressBarPrivate::WindowProc;
  wcex.cbClsExtra    = 0;
  wcex.cbWndExtra    = sizeof(LONG_PTR);
  wcex.hInstance     = hInstance;
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
