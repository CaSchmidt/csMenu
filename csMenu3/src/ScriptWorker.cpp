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

#include <cs/Concurrent/Map.h>

#include "ScriptWorker.h"

#include "FileName.h"
#include "Win32/Message.h"
#include "Win32/MessageBox.h"
#include "Win32/ProgressBar.h"
#include "Win32/Shell.h"
#include "Win32/WindowUtil.h"

////// Imports ///////////////////////////////////////////////////////////////

namespace conc = cs::concurrent;

extern HANDLE_t getInstDLL(); // main.cpp

////// Parallel //////////////////////////////////////////////////////////////

namespace impl_parallel {

  class Worker {
  public:
    Worker(const std::filesystem::path& script,
           const ProgressBar *progress = nullptr) noexcept
      : _progress(progress)
      , _script(script)
    {
    }

    ~Worker()
    {
    }

    void operator()(const std::filesystem::path& filename) const
    {
      if( !_script.empty() && !filename.empty() ) {
        const std::wstring exec = _script;
        const std::wstring arg  = quotedFileName(filename);
        shell::execute(exec.data(), arg.data());
      }

      if( _progress != nullptr ) {
        _progress->step();

        if( _progress->position() == _progress->range().second ) {
          _progress->close();
        }
      }
    }

  private:
    Worker() noexcept = delete;

    const ProgressBar *_progress{nullptr};
    std::filesystem::path _script{};
  };

} // namespace impl_parallel

////// Public ////////////////////////////////////////////////////////////////

void batch_work(WorkContext ctx)
{
  const std::wstring exec = ctx.script;
  const std::wstring args = joinFileNames(ctx.files);
  shell::execute(exec.data(), args.data());

  messagebox::information(L"Done! (Batch)");
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

  progress->setPostQuitOnDestroy(true);
  progress->setRange(0, static_cast<int>(ctx.files.size()));
  progress->show();

  using Worker = impl_parallel::Worker;
  auto f       = conc::mapAsync(ctx.numThreads, ctx.files.begin(), ctx.files.end(),
                                Worker(ctx.script, progress.get()));
  message::loop();
  f.get();

  messagebox::information(L"Done! (Parallel)");
}

void sequential_work(WorkContext ctx)
{
  const std::wstring exec = ctx.script;
  for( const std::filesystem::path& path : ctx.files ) {
    const std::wstring arg = quotedFileName(path);
    shell::execute(exec.data(), arg.data());
  }

  messagebox::information(L"Done! (Sequential)");
}
