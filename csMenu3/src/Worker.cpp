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

#include <algorithm>
#include <atomic>
#include <thread>

#include <cs/Core/Container.h>

#include "Worker.h"

#include "FileName.h"
#include "Win32/Message.h"
#include "Win32/MessageBox.h"
#include "Win32/ProgressBar.h"
#include "Win32/Shell.h"
#include "Win32/Window.h"

////// Imports ///////////////////////////////////////////////////////////////

extern HANDLE_t getInstDLL(); // main.cpp

////// Parallel //////////////////////////////////////////////////////////////

namespace impl_parallel {

  void dispatch_thread(std::atomic_size_t& counter,
                       const std::filesystem::path script, const std::filesystem::path filename,
                       const ProgressBar *progress)
  {
    counter.fetch_add(1, std::memory_order::relaxed);

    const std::wstring exec = script;
    const std::wstring arg  = quotedFileName(filename);
    shell::execute(exec.data(), arg.data());

    counter.fetch_sub(1, std::memory_order::relaxed);

    progress->step();
  }

  struct Dispatcher {
  public:
    Dispatcher() noexcept = delete;

    Dispatcher(WorkContext ctx, const ProgressBar *progress) noexcept
      : ctx{std::move(ctx)}
      , progress{progress}
    {
    }

    void start()
    {
      const std::size_t numStartJobs = std::min(ctx.files.size(), ctx.numThreads);
      for( std::size_t i = 0; i < numStartJobs; i++ ) {
        operator()(ProgressBar::getStepItMessage());
      }
    }

    void operator()(const UINT_t message)
    {
      if( message != ProgressBar::getStepItMessage() ) {
        return;
      }

      if( !ctx.files.empty() ) {
        std::thread{dispatch_thread, std::ref(counter), ctx.script, cs::takeFirst(&ctx.files), progress}.detach();

      } else {
        if( counter.load(std::memory_order::relaxed) == 0 ) {
          message::postQuit(0);
        }
      }
    }

    std::atomic_size_t counter{0};
    WorkContext ctx{};
    const ProgressBar *progress{nullptr};
  };

} // namespace impl_parallel

////// Public ////////////////////////////////////////////////////////////////

void batch_work(WorkContext ctx)
{
  const std::wstring exec = ctx.script;
  const std::wstring args = joinFileNames(ctx.files);
  shell::execute(exec.data(), args.data());
}

void parallel_work(WorkContext ctx)
{
  if( !window::makeGUIThread() ) {
    messagebox::error(L"makeGUIThread()");
    return;
  }

  ProgressBarPtr progress = ProgressBar::make(getInstDLL(), 480, 48);
  if( !progress ) {
    messagebox::error(L"ProgressBar::make()");
    return;
  }

  progress->setRange(0, static_cast<int>(ctx.files.size()));
  progress->show();

  impl_parallel::Dispatcher dispatch{std::move(ctx), progress.get()};
  dispatch.start();

  auto dispatch_wrapper = [&](const UINT_t message) -> void {
    dispatch(message);
  };

  message::loop(dispatch_wrapper);

  messagebox::information(L"Done!");
}

void sequential_work(WorkContext ctx)
{
  const std::wstring exec = ctx.script;
  for( const std::filesystem::path& path : ctx.files ) {
    const std::wstring arg = quotedFileName(path);
    shell::execute(exec.data(), arg.data());
  }
}
