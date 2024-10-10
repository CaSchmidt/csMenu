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

#include <utility>

#include <cs/Text/StringUtil.h>

#include "Rename.h"

////// Private ///////////////////////////////////////////////////////////////

namespace fs = std::filesystem;

using FileName = std::pair<std::wstring, std::wstring>;

namespace impl_rename {

  constexpr wchar_t DOT = L'.';

  constexpr std::size_t NPOS = std::wstring::npos;
  constexpr std::size_t ONE  = 1;
  constexpr std::size_t ZERO = 0;

  fs::path join(const FileName& filename)
  {
    return fs::path(filename.first + DOT + filename.second);
  }

  FileName split(const fs::path& path)
  {
    const std::wstring filename = path.filename().wstring();

    const std::size_t posDot = filename.rfind(DOT);
    if( posDot == ZERO || posDot == NPOS ) {
      return FileName(filename, std::wstring());
    }

    return FileName(filename.substr(ZERO, posDot),
                    filename.substr(posDot + ONE));
  }

} // namespace impl_rename

////// public ////////////////////////////////////////////////////////////////

Rename::Rename(const Mode mode,
               const std::wstring& pattern,
               const std::wstring& replace,
               const bool isExtension) noexcept
  : mode{mode}
  , pattern(pattern)
  , replace(replace)
  , isExtension{isExtension}
{
  if( mode == Replace && replace.empty() ) {
    this->mode = Remove;
  }
}

bool Rename::isValid() const
{
  return mode != Invalid && !pattern.empty();
}

void Rename::operator()(const std::filesystem::path& oldPath) const
{
  // (1) Extract filename & extension to work item ///////////////////////////

  FileName filename = impl_rename::split(oldPath);

  std::wstring item = isExtension
                      ? filename.second
                      : filename.first;

  // (2) Rename work item ////////////////////////////////////////////////////

  if( mode == Append ) {
    item.append(pattern);
  } else if( mode == Prepend ) {
    item.insert(impl_rename::ZERO, pattern);
  } else if( mode == Remove ) {
    cs::removeAll(item, pattern);
  } else if( mode == Replace ) {
    cs::replaceAll(item, pattern, replace);
  }

  // (3) Re-Build filename & sanity check ////////////////////////////////////

  if( isExtension ) {
    filename.second = item;
  } else {
    filename.first = item;
  }

  if( filename.first.empty() ) {
    return;
  }

  // (4) Execute rename //////////////////////////////////////////////////////

  const fs::path newPath = oldPath.parent_path() / impl_rename::join(filename);
  if( newPath == oldPath ) {
    return;
  }

  std::error_code ec;
  fs::rename(oldPath, newPath, ec);
}
