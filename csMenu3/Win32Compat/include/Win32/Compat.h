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

#include <cstdint>

/*
 * References:
 *
 * [MS-DTYP] Windows Data Types
 *
 * https://learn.microsoft.com/en-us/windows/win32/winprog/windows-data-types
 */

////// Implementation ////////////////////////////////////////////////////////

namespace impl_compat {

  template <typename Type32, typename Type64, int BITS>
  struct select3264 {
    // SFINAE
  };

  template <typename Type32, typename Type64>
  struct select3264<Type32, Type64, 32> {
    using type = Type32;
  };

  template <typename Type32, typename Type64>
  struct select3264<Type32, Type64, 64> {
    using type = Type64;
  };

  template <typename Type32, typename Type64>
  using select_t = typename select3264<Type32, Type64, sizeof(void *) * 8>::type;

  using int3264_t = select_t<int32_t, int64_t>;

  using uint3264_t = select_t<uint32_t, uint64_t>;

} // namespace impl_compat

////// Windows Data Types ////////////////////////////////////////////////////

using DWORD_t = uint32_t;

using HANDLE_t = void *;

using HINSTANCE_t = HANDLE_t;

using HWND_t = HANDLE_t;

using LONG_PTR_t = impl_compat::int3264_t;

using LPARAM_t = LONG_PTR_t;

using LRESULT_t = LONG_PTR_t;

using UINT_t = unsigned int;

using UINT_PTR_t = impl_compat::uint3264_t;

using WPARAM_t = UINT_PTR_t;
