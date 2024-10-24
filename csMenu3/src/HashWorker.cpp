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

#include <cs/Concurrent/MapReduce.h>
#include <cs/Convert/BufferUtil.h>
#include <cs/Core/Container.h>
#include <cs/Crypto/CryptoUtil.h>
#include <cs/Text/StringUtil.h>

#include "HashWorker.h"

#include "Util.h"
#include "Win32/Clipboard.h"
#include "Win32/Message.h"
#include "Win32/MessageBox.h"
#include "Win32/ProgressBar.h"
#include "Win32/WindowUtil.h"

////// Imports ///////////////////////////////////////////////////////////////

namespace conc = cs::concurrent;

extern HANDLE_t getInstDLL(); // main.cpp

////// Private ///////////////////////////////////////////////////////////////

namespace impl_hash {

  struct HashReduce {
    HashReduce() noexcept
    {
    }

    void operator()(std::wstring& result, const std::wstring& hash) const
    {
      try {
        result += hash;
      } catch( ... ) {
        return;
      }
    }
  };

  class Worker {
  public:
    Worker(const cs::Hash::Function func,
           const ProgressBar *progress = nullptr) noexcept
      : _func{func}
      , _progress{progress}
    {
    }

    ~Worker()
    {
    }

    std::wstring operator()(const std::filesystem::path& filename) const
    {
      std::wstring result;

      cs::File file;
      if( file.open(filename) ) {
        const cs::Buffer bindigest  = cs::sum(file, _func);
        const std::string strdigest = cs::toString(bindigest);

        result = cs::widen(strdigest);
      }

      if( !result.empty() ) {
        try {
          result += L" *";
          result += filename.filename();
          result += EOL;
        } catch( ... ) {
          result.clear();
        }
      }

      if( _progress != nullptr ) {
        _progress->step();

        if( _progress->position() == _progress->range().second ) {
          _progress->close();
        }
      }

      return result;
    }

  private:
    Worker() noexcept = delete;

    cs::Hash::Function _func{cs::Hash::Invalid};
    const ProgressBar *_progress{nullptr};
  };

} // namespace impl_hash

using Reduce = impl_hash::HashReduce;
using Worker = impl_hash::Worker;

////// Public ////////////////////////////////////////////////////////////////

void hash_work(const cs::Hash::Function func, WorkContext ctx)
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

  auto future = conc::mapReduceUnsortedAsync<std::wstring>(ctx.numThreads, ctx.files.begin(), ctx.files.end(),
                                                           Worker(func, progress.get()), Reduce());
  message::loop();
  const std::wstring result = future.get();

  messagebox::information(L"Done! (Hash)");

  setClipboardText(result.data());
}
