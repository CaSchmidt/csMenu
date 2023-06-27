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

#include <cstring>

#include <ShlObj.h>
#include <Windows.h>

#include "Win32/Shell.h"

namespace shell {

  void execute(const wchar_t *executable, const wchar_t *arguments, const wchar_t *directory)
  {
    SHELLEXECUTEINFOW sei;
    std::memset(&sei, 0, sizeof(sei));

    sei.cbSize       = sizeof(sei);
    sei.fMask        = SEE_MASK_NOASYNC | SEE_MASK_NOCLOSEPROCESS;
    sei.lpVerb       = L"open";
    sei.lpFile       = executable;
    sei.lpParameters = arguments;
    sei.lpDirectory  = directory;
    sei.nShow        = SW_SHOWNORMAL;

    if( ShellExecuteExW(&sei) != TRUE ) {
      return;
    }

    if( sei.hProcess != nullptr ) {
      WaitForSingleObject(sei.hProcess, INFINITE);
      CloseHandle(sei.hProcess);
    }
  }

  void notifyAssocChanged()
  {
    SHChangeNotify(SHCNE_ASSOCCHANGED, SHCNF_IDLIST, NULL, NULL);
  }

} // namespace shell
