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

#include <cs/Text/StringUtil.h>

#include "Invoke.h"

#include "MenuFlags.h"
#include "Win32/Clipboard.h"

////// Private ///////////////////////////////////////////////////////////////

namespace impl_invoke {

  namespace fs = std::filesystem;

  constexpr wchar_t SEP_NATIVE = L'\\';
  constexpr wchar_t SEP_UNIX = L'/';

  void appendFilename(std::wstring *text, const std::wstring& filename, const CommandId id)
  {
    constexpr std::size_t NPOS = std::wstring::npos;
    constexpr std::size_t ONE = 1;

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

  void invokeList(const CommandId id, const cs::PathList& files)
  {
    if( files.empty() ) {
      return;
    }

    const MenuFlags flags = readFlags();

    std::wstring text;
    try {
      for( const fs::path& file : files ) {
        appendFilename(&text, file.wstring(), id);
      }
    } catch( ... ) {
      return;
    }

    if( flags.testAny(MenuFlag::UnixPathSeparators) ) {
      cs::replaceAll(&text, SEP_NATIVE, SEP_UNIX);
    }

    setClipboardText(text.data());
  }

} // namespace impl_invoke

////// Public ////////////////////////////////////////////////////////////////

void invokeCommandId(const CommandId id, const cs::PathList& files)
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
  }
}
