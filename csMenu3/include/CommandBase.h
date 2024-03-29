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

#pragma once

#define NOMINMAX
#include <ShObjIdl.h>
#include <Windows.h>

#include <winrt/windows.foundation.h>

#include "Commands.h"

class CommandBase
  : public winrt::implements<CommandBase,
                             IExplorerCommand, IEnumExplorerCommand> {
public:
  CommandBase(const Command cmd, const std::wstring& title = std::wstring{}) noexcept;
  ~CommandBase() noexcept;

  void setIcon(const std::wstring& icon);
  void setIcon(const int id);

  // IExplorerCommand

  IFACEMETHODIMP_(HRESULT) EnumSubCommands(IEnumExplorerCommand **ppEnum);
  IFACEMETHODIMP_(HRESULT) GetCanonicalName(GUID *pguidCommandName);
  IFACEMETHODIMP_(HRESULT) GetFlags(EXPCMDFLAGS *pFlags);
  IFACEMETHODIMP_(HRESULT) GetIcon(IShellItemArray *psiItemArray, LPWSTR *ppszIcon);
  IFACEMETHODIMP_(HRESULT) GetState(IShellItemArray *psiItemArray, BOOL fOkToBeSlow, EXPCMDSTATE *pCmdState);
  IFACEMETHODIMP_(HRESULT) GetTitle(IShellItemArray *psiItemArray, LPWSTR *ppszName);
  IFACEMETHODIMP_(HRESULT) GetToolTip(IShellItemArray *psiItemArray, LPWSTR *ppszInfotip);
  IFACEMETHODIMP_(HRESULT) Invoke(IShellItemArray *psiItemArray, IBindCtx *pbc);

  // IEnumExplorerCommand

  IFACEMETHODIMP_(HRESULT) Clone(IEnumExplorerCommand **ppenum);
  IFACEMETHODIMP_(HRESULT) Next(ULONG celt, IExplorerCommand **pUICommand, ULONG *pceltFetched);
  IFACEMETHODIMP_(HRESULT) Reset();
  IFACEMETHODIMP_(HRESULT) Skip(ULONG celt);

protected:
  CommandId _id;
  std::wstring _title;

private:
  std::wstring _icon;
};
