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

////// Helper ////////////////////////////////////////////////////////////////

namespace impl_compat {

  template <typename T, typename U>
  inline constexpr bool is_both_integral_v = std::is_integral_v<T> && std::is_integral_v<U>;

  template <typename T, typename U>
  inline constexpr bool is_both_pointer_v = std::is_pointer_v<T> && std::is_pointer_v<U>;

  template <typename T, typename U>
  inline constexpr bool is_both_signed_v = is_both_integral_v<T, U> && std::is_signed_v<T> && std::is_signed_v<U>;

  template <typename T, typename U>
  inline constexpr bool is_both_unsigned_v = is_both_integral_v<T, U> && std::is_unsigned_v<T> && std::is_unsigned_v<U>;

  template <typename T, typename U>
  inline constexpr bool is_same_signedness_v = is_both_signed_v<T, U> || is_both_unsigned_v<T, U>;

} // namespace impl_compat

template <typename Type, typename Win32Type>
inline constexpr bool is_compat_integral_v = sizeof(Type) == sizeof(Win32Type) && impl_compat::is_same_signedness_v<Type, Win32Type>;

template <typename Type, typename Win32Type>
inline constexpr bool is_compat_pointer_v = sizeof(Type) == sizeof(Win32Type) && impl_compat::is_both_pointer_v<Type, Win32Type>;

////// DWORD /////////////////////////////////////////////////////////////////

static_assert(is_compat_integral_v<DWORD_t, DWORD>);

////// HANDLE ////////////////////////////////////////////////////////////////

static_assert(is_compat_pointer_v<HANDLE_t, HANDLE>);

////// HINSTANCE /////////////////////////////////////////////////////////////

static_assert(is_compat_pointer_v<HINSTANCE_t, HINSTANCE>);

////// HWND //////////////////////////////////////////////////////////////////

static_assert(is_compat_pointer_v<HWND_t, HWND>);

////// LONG_PTR //////////////////////////////////////////////////////////////

static_assert(is_compat_integral_v<LONG_PTR_t, LONG_PTR>);

////// LPARAM ////////////////////////////////////////////////////////////////

static_assert(is_compat_integral_v<LPARAM_t, LPARAM>);

////// LRESULT ///////////////////////////////////////////////////////////////

static_assert(is_compat_integral_v<LRESULT_t, LRESULT>);

////// UINT //////////////////////////////////////////////////////////////////

static_assert(is_compat_integral_v<UINT_t, UINT>);

////// UINT_PTR //////////////////////////////////////////////////////////////

static_assert(is_compat_integral_v<UINT_PTR_t, UINT_PTR>);

////// WPARAM ////////////////////////////////////////////////////////////////

static_assert(is_compat_integral_v<WPARAM_t, WPARAM>);
