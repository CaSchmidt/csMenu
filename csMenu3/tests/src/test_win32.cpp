#include <algorithm>
#include <format>
#include <iterator>
#include <string>
#include <thread>

#define NOMINMAX
#include <Windows.h>

#include "Win32/Message.h"
#include "Win32/MessageBox.h"
#include "Win32/ProgressBar.h"
#include "Win32/UI/Dialog.h"

// cf. https://learn.microsoft.com/en-us/windows/win32/controls/cookbook-overview
#pragma comment(linker, "\"/manifestdependency:type='win32' \
name='Microsoft.Windows.Common-Controls' version='6.0.0.0' \
processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")

////// Dialog ////////////////////////////////////////////////////////////////

std::wstring widen(const std::string_view& str)
{
  constexpr auto lambda_widen = [](const char ch) -> wchar_t {
    return static_cast<wchar_t>(ch);
  };

  std::wstring result;
  result.reserve(str.size());
  std::transform(str.cbegin(), str.cend(), std::back_inserter(result), lambda_widen);
  return result;
}

void test_dialog(const HINSTANCE hInstance)
{
  ui::Dialog d;
  const bool b          = d.exec(hInstance);
  const std::string s   = std::format("result: {}", b);
  const std::wstring ws = widen(s);
  messagebox::information(ws.data());
}

////// ProgressBar ///////////////////////////////////////////////////////////

void thread_func(const ProgressBar *progress)
{
  int i = 0;

  while( progress->position() != progress->range().second ) {
    i++;

    Sleep(i * 1000);
    progress->step();
  }

  progress->close();
}

void test_progress(const HINSTANCE hInstance)
{
  constexpr int MIN_ITEM  = 0;
  constexpr int NUM_ITEMS = 4;

  ProgressBarPtr pb = ProgressBar::make(hInstance, 480, 48);
  pb->setPostQuitOnDestroy(true);
  pb->setRange(MIN_ITEM, MIN_ITEM + NUM_ITEMS);
  pb->show();

  std::thread t(thread_func, pb.get());
  message::loop();
  t.join();

  messagebox::information(L"Done!");
}

////// Main //////////////////////////////////////////////////////////////////

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PWSTR pCmdLine, int nCmdShow)
{
  test_dialog(hInstance);
  // test_progress(hInstance);

  return 0;
}
