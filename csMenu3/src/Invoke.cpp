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

#include "Invoke.h"

#include "MenuFlags.h"

////// Private ///////////////////////////////////////////////////////////////

namespace impl_invoke {

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

} // namespace impl_invoke

////// Public ////////////////////////////////////////////////////////////////

void invokeCommandId(const CommandId id, const FileList& files)
{
  if( id == Command::CheckBatchProcessing ) {
    impl_invoke::invokeFlags(id);
  } else if( id == Command::CheckParallelExecution ) {
    impl_invoke::invokeFlags(id);
  } else if( id == Command::CheckResolveUncPaths ) {
    impl_invoke::invokeFlags(id);
  } else if( id == Command::CheckUnixPathSeparators ) {
    impl_invoke::invokeFlags(id);
  }
}
