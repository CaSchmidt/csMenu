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

#include <filesystem>
#include <thread>

#include <cs/System/FileSystem.h>
#include <cs/Text/StringUtil.h>

#include "Invoke.h"

#include "HashWorker.h"
#include "MenuFlags.h"
#include "ScriptWorker.h"
#include "Win32/Clipboard.h"
#include "Win32/Network.h"

////// Private ///////////////////////////////////////////////////////////////

namespace impl_invoke {

  namespace fs = std::filesystem;

  constexpr wchar_t SEP_NATIVE = L'\\';
  constexpr wchar_t SEP_UNIX   = L'/';

  void appendFilename(std::wstring *text, const std::wstring& filename, const CommandId id)
  {
    constexpr std::size_t NPOS = std::wstring::npos;
    constexpr std::size_t ONE  = 1;

    if( text == nullptr || filename.empty() ) {
      return;
    }

    const std::size_t pos = filename.rfind(SEP_NATIVE);
    if( pos != NPOS ) {
      if( id == Command::List ) {
        text->append(filename.data() + pos + ONE);
      } else if( id == Command::ListPathTabular ) {
        text->append(filename.data(), pos + ONE);
        text->append(ONE, L'\t');
        text->append(filename.data() + pos + ONE);
      } else { // Command::ListPath  AKA  "as-is"
        text->append(filename);
      }
    } else {
      text->append(filename);
    }

    if( cs::isDirectory(filename) ) {
      text->append(ONE, SEP_NATIVE);
    }

    text->append(L"\r\n");
  }

  cs::Hash::Function idToHashFunction(const CommandId id)
  {
    if( id == Command::HashCrc32 ) {
      return cs::Hash::CRC32;
    } else if( id == Command::HashMd5 ) {
      return cs::Hash::MD5;
    } else if( id == Command::HashSha1 ) {
      return cs::Hash::SHA1;
    } else if( id == Command::HashSha224 ) {
      return cs::Hash::SHA224;
    } else if( id == Command::HashSha256 ) {
      return cs::Hash::SHA256;
    } else if( id == Command::HashSha384 ) {
      return cs::Hash::SHA384;
    } else if( id == Command::HashSha512 ) {
      return cs::Hash::SHA512;
    }
    return cs::Hash::Invalid;
  }

  void invokeFlags(const CommandId id)
  {
    MenuFlags flags = readFlags();

    if( id == Command::CheckBatchProcessing ) {
      flags.toggle(MenuFlag::BatchProcessing);
    } else if( id == Command::CheckParallelExecution ) {
      flags.toggle(MenuFlag::ParallelExecution);
    } else if( id == Command::CheckResolveUncPaths ) {
      flags.toggle(MenuFlag::ResolveUncPaths);
    } else if( id == Command::CheckUnixPathSeparators ) {
      flags.toggle(MenuFlag::UnixPathSeparators);
    }

    writeFlags(flags);
  }

  void invokeHash(const CommandId id, const cs::PathList& files)
  {
    WorkContext ctx;

    if( !ctx.setFiles(files) ) {
      return;
    }

    const cs::Hash::Function func = idToHashFunction(id);
    if( func == cs::Hash::Invalid ) {
      return;
    }

    std::thread(hash_work, func, std::move(ctx)).detach();
  }

  void invokeList(const CommandId id, const cs::PathList& files)
  {
    if( files.empty() ) {
      return;
    }

    const MenuFlags flags = readFlags();
    const bool is_unc     = flags.testAny(MenuFlag::ResolveUncPaths) && id != Command::List;

    std::wstring text;
    try {
      for( const fs::path& file : files ) {
        std::wstring uncName;
        if( is_unc && !(uncName = resolveUniversalName(file.wstring().data())).empty() ) {
          appendFilename(&text, uncName, id);
        } else {
          appendFilename(&text, file.wstring(), id);
        }
      }
    } catch( ... ) {
      return;
    }

    if( flags.testAny(MenuFlag::UnixPathSeparators) ) {
      cs::replaceAll(&text, SEP_NATIVE, SEP_UNIX);
    }

    setClipboardText(text.data());
  }

  void invokeScript(const std::wstring& script, const cs::PathList& files)
  {
    constexpr std::size_t ONE = 1;

    WorkContext ctx;
    if( !ctx.setScript(script) || !ctx.setFiles(files) ) {
      return;
    }

    const MenuFlags flags  = readFlags();
    const bool is_batch    = flags.testAny(MenuFlag::BatchProcessing);
    const bool is_parallel = ctx.numThreads > ONE && flags.testAny(MenuFlag::ParallelExecution);

    if( is_batch ) {
      std::thread{batch_work, std::move(ctx)}.detach();
    } else {
      if( is_parallel ) {
        std::thread{parallel_work, std::move(ctx)}.detach();
      } else {
        std::thread{sequential_work, std::move(ctx)}.detach();
      }
    }
  }

} // namespace impl_invoke

////// Public ////////////////////////////////////////////////////////////////

void invokeCommandId(const CommandId id, const std::wstring& script, const cs::PathList& files)
{
  if( id == Command::List || id == Command::ListPath || id == Command::ListPathTabular ) {
    impl_invoke::invokeList(id, files);
  } else if( id == Command::CheckBatchProcessing ) {
    impl_invoke::invokeFlags(id);
  } else if( id == Command::CheckParallelExecution ) {
    impl_invoke::invokeFlags(id);
  } else if( id == Command::CheckResolveUncPaths ) {
    impl_invoke::invokeFlags(id);
  } else if( id == Command::CheckUnixPathSeparators ) {
    impl_invoke::invokeFlags(id);
  } else if( id > Command::HashMenu && id < Command::ScriptMenu ) {
    impl_invoke::invokeHash(id, files);
  } else if( id > Command::ScriptMenu ) {
    impl_invoke::invokeScript(script, files);
  }
}
