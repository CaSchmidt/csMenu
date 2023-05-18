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

#include <Unknwn.h>

#include "CommandEnum.h"

////// public ////////////////////////////////////////////////////////////////

CommandEnum::CommandEnum(const Command cmd, const std::wstring& icon) noexcept
  : CommandBase{cmd, icon}
{
}

CommandEnum::~CommandEnum() noexcept
{
}

void CommandEnum::append(winrt::com_ptr<IExplorerCommand> ptr)
{
  if( !ptr ) {
    return;
  }
  _cmds.push_back(ptr);
}

////// public - IExplorerCommand /////////////////////////////////////////////

/*
 * NOTE: For QueryInterface() to work, cf. to "Interoperating with the ABI's GUID struct" (C++/WinRT):
 * https://learn.microsoft.com/en-us/windows/uwp/cpp-and-winrt-apis/interop-winrt-abi#interoperating-with-the-abis-guid-struct
 */

IFACEMETHODIMP_(HRESULT) CommandEnum::EnumSubCommands(IEnumExplorerCommand **ppEnum)
{
  if( _cmds.empty() ) {
    *ppEnum = nullptr;
    return E_NOTIMPL;
  }

  _curCmd = _cmds.cbegin();
  AddRef();
  return QueryInterface(IID_PPV_ARGS(ppEnum));
}

IFACEMETHODIMP_(HRESULT) CommandEnum::GetFlags(EXPCMDFLAGS *pFlags)
{
  *pFlags = ECF_HASSUBCOMMANDS;
  return S_OK;
}

////// public - IEnumExplorerCommand /////////////////////////////////////////

IFACEMETHODIMP_(HRESULT) CommandEnum::Next(ULONG celt, IExplorerCommand **pUICommand, ULONG *pceltFetched)
{
  ULONG cntFetched{0};
  for( ULONG i = 0; i < celt && _curCmd != _cmds.cend(); i++ ) {
    _curCmd->copy_to(&pUICommand[cntFetched]);
    ++_curCmd;
    cntFetched += 1;
  }

  if( pceltFetched != nullptr ) {
    *pceltFetched = cntFetched;
  }

  return cntFetched == celt
         ? S_OK
         : S_FALSE;
}

IFACEMETHODIMP_(HRESULT) CommandEnum::Reset()
{
  _curCmd = _cmds.cbegin();
  return S_OK;
}