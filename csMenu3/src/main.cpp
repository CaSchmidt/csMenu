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

#include "GUIDs.h"
#include "MainMenuFactory.h"
#include "Register.h"
#include "ScriptMenuFactory.h"
#include "Win32/ProgressBar.h"
#include "Win32/Window.h"

////// Global ////////////////////////////////////////////////////////////////

HINSTANCE g_hInstDLL = nullptr;

HANDLE_t getInstDLL()
{
  return g_hInstDLL;
}

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

  try {
    if( rclsid == __uuidof(MainMenuFactory) ) {
      return winrt::make<MainMenuFactory>()->QueryInterface(riid, ppv);
    } else if( rclsid == __uuidof(ScriptMenuFactory) ) {
      return winrt::make<ScriptMenuFactory>()->QueryInterface(riid, ppv);
    }
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

#define CSSCRIPT3_NAME L"CS::Script3"
#define CSSCRIPT3_VERB L"csScript3Verb"

HRESULT WINAPI DllRegisterServer()
{
  HRESULT result = E_FAIL;

  result = registerExplorerCommand(g_csMenu3GUID, g_hInstDLL, CSMENU3_VERB, CSMENU3_NAME, true, true);
  if( FAILED(result) ) {
    return result;
  }

  result = registerExplorerCommand(g_csScript3GUID, g_hInstDLL, CSSCRIPT3_VERB, CSSCRIPT3_NAME, true, false);
  if( FAILED(result) ) {
    return result;
  }

  return S_OK;
}

HRESULT WINAPI DllUnregisterServer()
{
  HRESULT result = E_FAIL;

  result = unregisterExplorerCommand(g_csMenu3GUID, CSMENU3_VERB);
  if( FAILED(result) ) {
    return result;
  }

  result = unregisterExplorerCommand(g_csScript3GUID, CSSCRIPT3_VERB);
  if( FAILED(result) ) {
    return result;
  }

  return S_OK;
}
