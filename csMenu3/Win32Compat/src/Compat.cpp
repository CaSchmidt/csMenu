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

#include <type_traits>

#include <Windows.h>

#include "Win32/Compat.h"

////// DWORD /////////////////////////////////////////////////////////////////

static_assert( sizeof(DWORD_t) == sizeof(DWORD) );

static_assert( std::is_unsigned_v<DWORD_t> && std::is_unsigned_v<DWORD> );

////// HANDLE ////////////////////////////////////////////////////////////////

static_assert( sizeof(HANDLE_t) == sizeof(HANDLE) );

static_assert( std::is_pointer_v<HANDLE_t> && std::is_pointer_v<HANDLE> );

////// UINT //////////////////////////////////////////////////////////////////

static_assert( sizeof(UINT_t) == sizeof(UINT) );

static_assert( std::is_unsigned_v<UINT_t> && std::is_unsigned_v<UINT> );

////// WPARAM ////////////////////////////////////////////////////////////////

static_assert( sizeof(WPARAM_t) == sizeof(WPARAM) );

static_assert( std::is_unsigned_v<WPARAM_t> && std::is_unsigned_v<WPARAM> );

static_assert( !std::is_pointer_v<WPARAM_t> && !std::is_pointer_v<WPARAM> );

////// LPARAM ////////////////////////////////////////////////////////////////

static_assert( sizeof(LPARAM_t) == sizeof(LPARAM) );

static_assert( std::is_signed_v<LPARAM_t> && std::is_signed_v<LPARAM> );

static_assert( !std::is_pointer_v<LPARAM_t> && !std::is_pointer_v<LPARAM> );
