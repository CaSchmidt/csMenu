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

#include "CommandInvoke.h"

#include "Invoke.h"

////// Private ///////////////////////////////////////////////////////////////

namespace impl_cmdinvoke {

  cs::PathList makeFileList(IShellItemArray *items)
  {
    if( items == nullptr ) {
      return cs::PathList{};
    }

    DWORD count = 0;
    if( FAILED(items->GetCount(&count)) ) {
      return cs::PathList{};
    }

    cs::PathList list;

    wchar_t *displayName = nullptr;
    try {
      for( DWORD i = 0; i < count; i++ ) {
        winrt::com_ptr<IShellItem> item;
        if( FAILED(items->GetItemAt(i, item.put())) ) {
          continue;
        }

        displayName = nullptr;
        if( FAILED(item->GetDisplayName(SIGDN_FILESYSPATH, &displayName)) ) {
          continue;
        }

        list.push_back(displayName);
        ::CoTaskMemFree(displayName);
        displayName = nullptr;
      } // For Each Item

      list.sort();
    } catch( ... ) {
      ::CoTaskMemFree(displayName);
      displayName = nullptr;
      return cs::PathList{};
    }

    return list;
  }

} // namespace impl_cmdinvoke

////// public ////////////////////////////////////////////////////////////////

CommandInvoke::CommandInvoke(const Command cmd, const std::wstring& icon) noexcept
  : CommandBase{cmd, icon}
{
}

CommandInvoke::~CommandInvoke() noexcept
{
}

IFACEMETHODIMP_(HRESULT) CommandInvoke::Invoke(IShellItemArray *psiItemArray, IBindCtx *pbc)
{
  UNREFERENCED_PARAMETER(pbc);

  const cs::PathList files = impl_cmdinvoke::makeFileList(psiItemArray);

  invokeCommandId(_id, files);

  return S_OK;
}
