#include <thread>

#define NOMINMAX
#include <Windows.h>

#include "Win32/Message.h"
#include "Win32/ProgressBar.h"

void thread_work(const int i, const ProgressBar *pb)
{
  Sleep(i * 1000);
  pb->step();
}

struct Dispatch {
  Dispatch() noexcept = delete;

  Dispatch(const int numItems, const ProgressBar *pb) noexcept
    : i{0}
    , numItems{numItems}
    , pb{pb}
  {
  }

  void operator()(const UINT_t message)
  {
    if( message != ProgressBar::getStepItMessage() ) {
      return;
    }

    if( i < numItems ) {
      i += 1;
      std::thread{thread_work, i, pb}.detach();
    } else {
      message::postQuit(0);
    }
  }

  int i{0};
  int numItems{0};
  const ProgressBar *pb{nullptr};
};

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PWSTR pCmdLine, int nCmdShow)
{
  constexpr int MIN_ITEM  = 0;
  constexpr int NUM_ITEMS = 4;

  ProgressBarPtr pb = ProgressBar::make(hInstance, 480, 48);
  pb->setRange(MIN_ITEM, MIN_ITEM + NUM_ITEMS);
  pb->show();

  Dispatch dispatch{NUM_ITEMS, pb.get()};
  dispatch(ProgressBar::getStepItMessage());

  message::loop(dispatch);

  return 0;
}
