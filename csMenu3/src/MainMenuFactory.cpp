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

#include "MainMenuFactory.h"

#include "CommandEnum.h"
#include "CommandFlag.h"
#include "CommandInvoke.h"
#include "CommandSeparator.h"
#include "MenuFlags.h"
#include "resource.h"

////// Private ///////////////////////////////////////////////////////////////

namespace impl_menu {

  void buildContextMenu(CommandEnum *menu)
  {
    if( menu == nullptr ) {
      return;
    }

    menu->setIcon(-IDI_blue);

    menu->append(winrt::make<CommandInvoke>(Command::List));
    menu->append(winrt::make<CommandInvoke>(Command::ListPath));
    menu->append(winrt::make<CommandInvoke>(Command::ListPathTabular));

    // Settings ////////////////////////////////////////////////////////////////

    menu->append(winrt::make<CommandSeparator>());

    const MenuFlags flags = readFlags();
    menu->append(winrt::make<CommandFlag>(flags.testAny(MenuFlag::BatchProcessing), Command::CheckBatchProcessing));
    menu->append(winrt::make<CommandFlag>(flags.testAny(MenuFlag::ParallelExecution), Command::CheckParallelExecution));
    menu->append(winrt::make<CommandFlag>(flags.testAny(MenuFlag::ResolveUncPaths), Command::CheckResolveUncPaths));
    menu->append(winrt::make<CommandFlag>(flags.testAny(MenuFlag::UnixPathSeparators), Command::CheckUnixPathSeparators));
  }

} // namespace impl_menu

////// public ////////////////////////////////////////////////////////////////

IFACEMETHODIMP MainMenuFactory::CreateInstance(IUnknown *pUnkOuter, REFIID riid, void **ppvObject)
{
  if( pUnkOuter != nullptr ) {
    return CLASS_E_NOAGGREGATION;
  }

  try {
    auto menu = winrt::make<CommandEnum>(Command::MainMenu);
    impl_menu::buildContextMenu(dynamic_cast<CommandEnum *>(menu.get()));

    return menu->QueryInterface(riid, ppvObject);
  } catch( ... ) {
    return winrt::to_hresult();
  }

  return E_NOINTERFACE;
}

IFACEMETHODIMP MainMenuFactory::LockServer(BOOL fLock)
{
  if( fLock ) {
    ++winrt::get_module_lock();
  } else {
    --winrt::get_module_lock();
  }

  return S_OK;
}
