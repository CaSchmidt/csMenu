#include <thread>

#define NOMINMAX
#include <Windows.h>

#include "Win32/Message.h"
#include "Win32/MessageBox.h"
#include "Win32/ProgressBar.h"

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

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PWSTR pCmdLine, int nCmdShow)
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

  return 0;
}
