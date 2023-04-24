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

#include <Shlwapi.h>

#include "CommandBase.h"

////// public ////////////////////////////////////////////////////////////////

CommandBase::CommandBase(const Command cmd, const std::wstring& icon) noexcept
  : _id{static_cast<CommandId>(cmd)}
  , _icon{icon}
{
  _title = titleFromId(_id);
}

CommandBase::~CommandBase() noexcept
{
}

////// public - IExplorerCommand /////////////////////////////////////////////

IFACEMETHODIMP_(HRESULT) CommandBase::EnumSubCommands(IEnumExplorerCommand **ppEnum)
{
  UNREFERENCED_PARAMETER(ppEnum);
  return E_NOTIMPL;
}

IFACEMETHODIMP_(HRESULT) CommandBase::GetCanonicalName(GUID *pguidCommandName)
{
  UNREFERENCED_PARAMETER(pguidCommandName);
  return E_NOTIMPL;
}

IFACEMETHODIMP_(HRESULT) CommandBase::GetFlags(EXPCMDFLAGS *pFlags)
{
  *pFlags = ECF_DEFAULT;
  return S_OK;
}

IFACEMETHODIMP_(HRESULT) CommandBase::GetIcon(IShellItemArray *psiItemArray, LPWSTR *ppszIcon)
{
  UNREFERENCED_PARAMETER(psiItemArray);
  if( _icon.empty() ) {
    *ppszIcon = nullptr;
    return E_NOTIMPL;
  }
  return ::SHStrDupW(_icon.data(), ppszIcon);
}

IFACEMETHODIMP_(HRESULT) CommandBase::GetState(IShellItemArray *psiItemArray, BOOL fOkToBeSlow, EXPCMDSTATE *pCmdState)
{
  UNREFERENCED_PARAMETER(psiItemArray);
  UNREFERENCED_PARAMETER(fOkToBeSlow);
  *pCmdState = ECS_ENABLED;
  return S_OK;
}

IFACEMETHODIMP_(HRESULT) CommandBase::GetTitle(IShellItemArray *psiItemArray, LPWSTR *ppszName)
{
  UNREFERENCED_PARAMETER(psiItemArray);
  if( _title.empty() ) {
    *ppszName = nullptr;
    return E_NOTIMPL;
  }
  return ::SHStrDupW(_title.data(), ppszName);
}

IFACEMETHODIMP_(HRESULT) CommandBase::GetToolTip(IShellItemArray *psiItemArray, LPWSTR *ppszInfotip)
{
  UNREFERENCED_PARAMETER(psiItemArray);
  UNREFERENCED_PARAMETER(ppszInfotip);
  return E_NOTIMPL;
}

IFACEMETHODIMP_(HRESULT) CommandBase::Invoke(IShellItemArray *psiItemArray, IBindCtx *pbc)
{
  UNREFERENCED_PARAMETER(psiItemArray);
  UNREFERENCED_PARAMETER(pbc);
  return E_NOTIMPL;
}

////// public - IEnumExplorerCommand /////////////////////////////////////////

IFACEMETHODIMP_(HRESULT) CommandBase::Clone(IEnumExplorerCommand **ppenum)
{
  UNREFERENCED_PARAMETER(ppenum);
  return E_NOTIMPL;
}

IFACEMETHODIMP_(HRESULT) CommandBase::Next(ULONG celt, IExplorerCommand **pUICommand, ULONG *pceltFetched)
{
  UNREFERENCED_PARAMETER(celt);
  UNREFERENCED_PARAMETER(pUICommand);
  UNREFERENCED_PARAMETER(pceltFetched);
  return E_NOTIMPL;
}

IFACEMETHODIMP_(HRESULT) CommandBase::Reset()
{
  return E_NOTIMPL;
}

IFACEMETHODIMP_(HRESULT) CommandBase::Skip(ULONG celt)
{
  UNREFERENCED_PARAMETER(celt);
  return E_NOTIMPL;
}
