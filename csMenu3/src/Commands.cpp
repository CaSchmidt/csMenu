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

#include "Commands.h"

////// Public ////////////////////////////////////////////////////////////////

std::wstring titleFromId(const CommandId id)
{
  if( id == Command::None ) {
    return std::wstring();
  } else if( id == Command::MainMenu ) {
    return std::wstring{L"CS::Menu"};
  } else if( id == Command::List ) {
    return std::wstring{L"List"};
  } else if( id == Command::ListPath ) {
    return std::wstring{L"List (path)"};
  } else if( id == Command::ListPathTabular ) {
    return std::wstring{L"List (path, tabular)"};
  } else if( id == Command::Rename ) {
    return std::wstring(L"Rename...");
  } else if( id == Command::CheckBatchProcessing ) {
    return std::wstring{L"Batch processing"};
  } else if( id == Command::CheckParallelExecution ) {
    return std::wstring{L"Parallel execution"};
  } else if( id == Command::CheckResolveUncPaths ) {
    return std::wstring{L"Resolve UNC paths"};
  } else if( id == Command::CheckUnixPathSeparators ) {
    return std::wstring{L"UN*X path separators"};
  } else if( id == Command::HashMenu ) {
    return std::wstring(L"CS::Sum");
  } else if( id == Command::HashCrc32 ) {
    return std::wstring(L"CRC-32");
  } else if( id == Command::HashMd5 ) {
    return std::wstring(L"MD5");
  } else if( id == Command::HashSha1 ) {
    return std::wstring(L"SHA-1");
  } else if( id == Command::HashSha224 ) {
    return std::wstring(L"SHA-224");
  } else if( id == Command::HashSha256 ) {
    return std::wstring(L"SHA-256");
  } else if( id == Command::HashSha384 ) {
    return std::wstring(L"SHA-384");
  } else if( id == Command::HashSha512 ) {
    return std::wstring(L"SHA-512");
  } else if( id == Command::ScriptMenu ) {
    return std::wstring{L"CS::Run"};
  }

  return std::wstring();
}
