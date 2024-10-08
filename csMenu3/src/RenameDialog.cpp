/****************************************************************************
** Copyright (c) 2024, Carsten Schmidt. All rights reserved.
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

#define UNICODE
#include <Windows.h>

#include "Win32/UI/CheckBox.h"
#include "Win32/UI/ComboBox.h"
#include "Win32/UI/EditText.h"

#include "RenameDialog.h"

#include "csMenu3Resource.h"
#include "RenameDialogResource.h"

////// public ////////////////////////////////////////////////////////////////

RenameDialog::RenameDialog() noexcept
{
}

RenameDialog::~RenameDialog() noexcept
{
}

////// protected /////////////////////////////////////////////////////////////

LRESULT_t RenameDialog::onInitDialog(HWND_t wnd, LPARAM_t lParam)
{
  const LRESULT_t result = Dialog::onInitDialog(wnd, lParam);

  _controls.push_back(ui::CheckBox::create(wnd, ID_CheckExtension));

  ui::WindowPtr modeCombo = ui::ComboBox::create(wnd, ID_ComboMode);
  ui::COMBOBOX(modeCombo)->addItem(L"Append");
  ui::COMBOBOX(modeCombo)->addItem(L"Prepend");
  ui::COMBOBOX(modeCombo)->addItem(L"Remove");
  ui::COMBOBOX(modeCombo)->addItem(L"Replace");
  ui::COMBOBOX(modeCombo)->setCurrentIndex(0);
  _controls.push_back(std::move(modeCombo));

  _controls.push_back(ui::EditText::create(wnd, ID_EditPattern));

  _controls.push_back(ui::EditText::create(wnd, ID_EditText));

  return result;
}

const wchar_t *RenameDialog::dialogName() const
{
  return MAKEINTRESOURCEW(IDD_RenameDialog);
}

const wchar_t *RenameDialog::iconName() const
{
  return MAKEINTRESOURCEW(IDI_blue);
}

const wchar_t *RenameDialog::wndClassName() const
{
  return WNDCLASS_RenameDialog;
}
