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

#define NOMINMAX
#include <olectl.h>
#include <Windows.h>

#include <winrt/windows.foundation.h>

#include <cs/System/FileSystem.h>

#include "CommandEnum.h"
#include "CommandFlag.h"
#include "CommandInvoke.h"
#include "CommandSeparator.h"
#include "GUIDs.h"
#include "MenuFlags.h"
#include "Register.h"
#include "Settings.h"
#include "Win32/ProgressBar.h"
#include "Win32/Registry.h"
#include "Win32/Window.h"

////// Global ////////////////////////////////////////////////////////////////

HINSTANCE g_hInstDLL = nullptr;

HANDLE_t getInstDLL()
{
  return g_hInstDLL;
}

////// Context Menu //////////////////////////////////////////////////////////

void buildScriptsMenu(CommandEnum *rootMenu)
{
  constexpr std::size_t MAX_ScriptMenus    = 8;
  constexpr std::size_t MAX_ScriptsPerMenu = 8;

  if( rootMenu == nullptr ) {
    return;
  }

  const std::wstring scriptsPath = reg::readCurrentUserString(KEY_CSMENU, NAME_SCRIPTS);
  if( scriptsPath.empty() ) {
    return;
  }

  const cs::PathListFlags flags = cs::PathListFlag::File | cs::PathListFlag::SelectFilename;
  const cs::PathList scripts    = cs::list(scriptsPath, flags);
  if( scripts.empty() ) {
    return;
  }

  rootMenu->append(winrt::make<CommandSeparator>());

  if( scripts.size() <= MAX_ScriptsPerMenu ) {
    CommandId id = static_cast<CommandId>(Command::ScriptsMenu);
    auto menu    = winrt::make<CommandEnum>(static_cast<Command>(id));
    rootMenu->append(menu);

    CommandEnum *cmdEnum = dynamic_cast<CommandEnum *>(menu.get());
    for( const std::filesystem::path& script : scripts ) {
      cmdEnum->append(winrt::make<CommandInvoke>(static_cast<Command>(++id), script.wstring()));
    }

  } else {
    CommandId id               = static_cast<CommandId>(Command::ScriptsMenu);
    cs::ConstPathListIter iter = scripts.begin();
    std::wstring title{L"Scripts #0"};

    for( std::size_t i = 0; i < MAX_ScriptMenus && iter != scripts.end(); i++ ) {
      title.back() += wchar_t{1};
      auto menu     = winrt::make<CommandEnum>(static_cast<Command>(id), title);
      rootMenu->append(menu);

      CommandEnum *cmdEnum = dynamic_cast<CommandEnum *>(menu.get());
      for( std::size_t j = 0; j < MAX_ScriptsPerMenu && iter != scripts.end(); j++ ) {
        cmdEnum->append(winrt::make<CommandInvoke>(static_cast<Command>(++id), iter->wstring()));

        ++iter;
      } // For Each Script

      id++;
    } // For Each Menu
  }
}

void buildSettingsMenu(CommandEnum *menu)
{
  if( menu == nullptr ) {
    return;
  }

  const MenuFlags flags = readFlags();
  menu->append(winrt::make<CommandFlag>(flags.testAny(MenuFlag::BatchProcessing), Command::CheckBatchProcessing));
  menu->append(winrt::make<CommandFlag>(flags.testAny(MenuFlag::ParallelExecution), Command::CheckParallelExecution));
  menu->append(winrt::make<CommandFlag>(flags.testAny(MenuFlag::ResolveUncPaths), Command::CheckResolveUncPaths));
  menu->append(winrt::make<CommandFlag>(flags.testAny(MenuFlag::UnixPathSeparators), Command::CheckUnixPathSeparators));
}

void buildRootMenu(CommandEnum *menu)
{
  if( menu == nullptr ) {
    return;
  }

  menu->append(winrt::make<CommandInvoke>(Command::List));
  menu->append(winrt::make<CommandInvoke>(Command::ListPath));
  menu->append(winrt::make<CommandInvoke>(Command::ListPathTabular));

  // Settings ////////////////////////////////////////////////////////////////

  menu->append(winrt::make<CommandSeparator>());

  auto settings = winrt::make<CommandEnum>(Command::SettingsMenu);
  menu->append(settings);
  buildSettingsMenu(dynamic_cast<CommandEnum *>(settings.get()));

  // Scripts /////////////////////////////////////////////////////////////////

  buildScriptsMenu(menu);
}

////// Class Factory /////////////////////////////////////////////////////////

struct ClassFactory
  : public winrt::implements<ClassFactory, IClassFactory> {
  IFACEMETHODIMP CreateInstance(IUnknown *pUnkOuter, REFIID riid, void **ppvObject)
  {
    if( pUnkOuter != nullptr ) {
      return CLASS_E_NOAGGREGATION;
    }

    try {
      auto root = winrt::make<CommandEnum>(Command::RootMenu);
      buildRootMenu(dynamic_cast<CommandEnum *>(root.get()));

      return root->QueryInterface(riid, ppvObject);
    } catch( ... ) {
      return winrt::to_hresult();
    }

    return E_NOINTERFACE;
  }

  IFACEMETHODIMP LockServer(BOOL fLock)
  {
    if( fLock ) {
      ++winrt::get_module_lock();
    } else {
      --winrt::get_module_lock();
    }

    return S_OK;
  }
};

// cf. guid.cpp
struct __declspec(uuid("3d92630b-2959-4551-8a55-ffb508ef3791")) ClassFactory;

////// DLL ///////////////////////////////////////////////////////////////////

HRESULT WINAPI DllCanUnloadNow()
{
  if( winrt::get_module_lock() ) {
    return S_FALSE;
  }

  winrt::clear_factory_cache();

  return S_OK;
}

HRESULT WINAPI DllGetClassObject(REFCLSID rclsid, REFIID riid, LPVOID *ppv)
{
  if( ppv == nullptr ) {
    return E_POINTER;
  }

  if( riid != IID_IUnknown && riid != IID_IClassFactory ) {
    return E_NOINTERFACE;
  }

  if( rclsid != __uuidof(ClassFactory) ) {
    return CLASS_E_CLASSNOTAVAILABLE;
  }

  try {
    return winrt::make<ClassFactory>()->QueryInterface(riid, ppv);
  } catch( ... ) {
    return winrt::to_hresult();
  }

  return CLASS_E_CLASSNOTAVAILABLE;
}

BOOL WINAPI DllMain(HINSTANCE hInstDLL, DWORD fdwReason, LPVOID lpvReserved)
{
  UNREFERENCED_PARAMETER(lpvReserved);

  switch( fdwReason ) {
  case DLL_PROCESS_ATTACH:
    g_hInstDLL = hInstDLL;
    DisableThreadLibraryCalls(hInstDLL);
    break;
  case DLL_PROCESS_DETACH:
    window::unregisterClass(hInstDLL, ProgressBar::windowClassName());
    g_hInstDLL = nullptr;
    break;
  case DLL_THREAD_ATTACH:
    break;
  case DLL_THREAD_DETACH:
    break;
  default:
    break;
  }

  return TRUE;
}

#define CSMENU3_NAME L"CS::Menu3"
#define CSMENU3_VERB L"csMenu3Verb"

HRESULT WINAPI DllRegisterServer()
{
  return registerExplorerCommand(g_csMenu3GUID, g_hInstDLL, CSMENU3_VERB, CSMENU3_NAME, true, true);
}

HRESULT WINAPI DllUnregisterServer()
{
  return unregisterExplorerCommand(g_csMenu3GUID, CSMENU3_VERB);
}
