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

#include <Windows.h>

#include <winrt/windows.foundation.h>

#include "CommandEnum.h"
#include "CommandFlag.h"
#include "CommandInvoke.h"
#include "CommandSeparator.h"

////// Global ////////////////////////////////////////////////////////////////

HINSTANCE g_hInstance = nullptr;

////// Private ///////////////////////////////////////////////////////////////

void buildMenu(CommandEnum *menu)
{
  if( menu == nullptr ) {
    return;
  }

  menu->append(winrt::make<CommandInvoke>(Command::List));
  menu->append(winrt::make<CommandInvoke>(Command::ListPath));
  menu->append(winrt::make<CommandInvoke>(Command::ListPathTabular));

  menu->append(winrt::make<CommandSeparator>());

  menu->append(winrt::make<CommandFlag>(false, Command::CheckBatchProcessing));
  menu->append(winrt::make<CommandFlag>(true, Command::CheckParallelExecution));
  menu->append(winrt::make<CommandFlag>(false, Command::CheckResolveUncPaths));
  menu->append(winrt::make<CommandFlag>(true, Command::CheckUnixPathSeparators));
}

////// Class Factory /////////////////////////////////////////////////////////

struct ClassFactory
  : public winrt::implements<ClassFactory, IClassFactory> {
  IFACEMETHODIMP_(HRESULT) CreateInstance(IUnknown *pUnkOuter, REFIID riid, void **ppvObject)
  {
    if( pUnkOuter != nullptr ) {
      return CLASS_E_NOAGGREGATION;
    }

    try {
      auto root = winrt::make<CommandEnum>(Command::Root);
      buildMenu(dynamic_cast<CommandEnum *>(root.get()));

      return root->QueryInterface(riid, ppvObject);
    } catch( ... ) {
      return winrt::to_hresult();
    }

    return E_NOINTERFACE;
  }

  IFACEMETHODIMP_(HRESULT) LockServer(BOOL fLock)
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

BOOL WINAPI DllMain(HINSTANCE hinstDLL, DWORD fdwReason, LPVOID lpvReserved)
{
  UNREFERENCED_PARAMETER(lpvReserved);

  switch( fdwReason ) {
  case DLL_PROCESS_ATTACH:
    g_hInstance = hinstDLL;
    DisableThreadLibraryCalls(hinstDLL);
    break;
  case DLL_PROCESS_DETACH:
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

HRESULT WINAPI DllRegisterServer()
{
  return S_OK;
}

HRESULT WINAPI DllUnregisterServer()
{
  return S_OK;
}