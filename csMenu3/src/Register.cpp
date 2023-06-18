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

#include <olectl.h>

#include "Register.h"

#include "Win32/GUID.h"
#include "Win32/Module.h"
#include "Win32/Registry.h"
#include "Win32/Shell.h"

////// Macros ////////////////////////////////////////////////////////////////

#define BEGKEY_CLSID L"Software\\Classes\\CLSID\\"
#define RELKEY_INPROCSERVER32 L"\\InprocServer32"

#define BEGKEY_FILEVERB L"Software\\Classes\\*\\Shell\\"

#define BEGKEY_DIRECTORYVERB L"Software\\Classes\\Directory\\Shell\\"

////// Public ////////////////////////////////////////////////////////////////

HRESULT registerExplorerCommand(const GUID& refGuid, const HMODULE module,
                                const std::wstring& verb, const std::wstring& name)
{
  if( module == nullptr || verb.empty() || name.empty() ) {
    return SELFREG_E_CLASS;
  }

  const std::wstring filename = getModuleFileName(module);
  const std::wstring guid = GUIDasString(&refGuid);
  if( filename.empty() || guid.empty() ) {
    return SELFREG_E_CLASS;
  }

  std::wstring key;

  // (1) Register Class ID (GUID) ////////////////////////////////////////////

  key = BEGKEY_CLSID;
  key += guid;
  if( !reg::writeLocalMachineString(key.data(), nullptr, name.data()) ) {
    return SELFREG_E_CLASS;
  }

  key += RELKEY_INPROCSERVER32;
  if( !reg::writeLocalMachineString(key.data(), nullptr, filename.data()) ) {
    return SELFREG_E_CLASS;
  }
  if( !reg::writeLocalMachineString(key.data(), L"ThreadingModel", L"Apartment") ) {
    return SELFREG_E_CLASS;
  }

  // (2) Register with Files /////////////////////////////////////////////////

  key = BEGKEY_FILEVERB;
  key += verb;
  if( !reg::writeLocalMachineString(key.data(), nullptr, name.data()) ) {
    return SELFREG_E_CLASS;
  }
  if( !reg::writeLocalMachineString(key.data(), L"ExplorerCommandHandler", guid.data()) ) {
    return SELFREG_E_CLASS;
  }

  // (3) Register with Directories ///////////////////////////////////////////

  key = BEGKEY_DIRECTORYVERB;
  key += verb;
  if( !reg::writeLocalMachineString(key.data(), nullptr, name.data()) ) {
    return SELFREG_E_CLASS;
  }
  if( !reg::writeLocalMachineString(key.data(), L"ExplorerCommandHandler", guid.data()) ) {
    return SELFREG_E_CLASS;
  }

  // (4) Notify Shell of Changes /////////////////////////////////////////////

  shell::notifyAssocChanged();

  return S_OK;
}

HRESULT unregisterExplorerCommand(const GUID& refGuid, const std::wstring& verb)
{
  if( verb.empty() ) {
    return SELFREG_E_CLASS;
  }

  std::wstring guid = GUIDasString(&refGuid);
  if( guid.empty() ) {
    return SELFREG_E_CLASS;
  }

  std::wstring key;

  // (1) Unregister Class ID (GUID) //////////////////////////////////////////

  key = BEGKEY_CLSID;
  key += guid;
  if( !reg::deleteLocalMachineTree(key.data()) ) {
    return SELFREG_E_CLASS;
  }

  // (2) Unregister Files ////////////////////////////////////////////////////

  key = BEGKEY_FILEVERB;
  key += verb;
  if( !reg::deleteLocalMachineTree(key.data()) ) {
    return SELFREG_E_CLASS;
  }

  // (3) Unregister Directories //////////////////////////////////////////////

  key = BEGKEY_DIRECTORYVERB;
  key += verb;
  if( !reg::deleteLocalMachineTree(key.data()) ) {
    return SELFREG_E_CLASS;
  }

  // (4) Notify Shell of Changes /////////////////////////////////////////////

  shell::notifyAssocChanged();

  return S_OK;
}
