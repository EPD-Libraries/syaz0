/**
 * Copyright (C) 2019 leoetlino <leo@leolam.fr>
 *
 * This file is part of syaz0.
 *
 * syaz0 is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 2 of the License, or
 * (at your option) any later version.
 *
 * syaz0 is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with syaz0.  If not, see <http://www.gnu.org/licenses/>.
 */

#pragma once

#include <array>
#include <optional>
#include <span>
#include <type_traits>
#include <vector>

#include <types.h>
#include <util/bit_utils.h>
#include <util/swap.h>

namespace util {

/// A simple binary data reader that automatically byteswaps and avoids undefined behaviour.
class BinaryReader final {
public:
  BinaryReader() = default;
  BinaryReader(std::span<const u8> data, Endianness endian) : m_data{data}, m_endian{endian} {}

  const auto& span() const { return m_data; }
  size_t Tell() const { return m_offset; }
  void Seek(size_t offset) { m_offset = offset; }

  Endianness Endian() const { return m_endian; }
  void SetEndian(Endianness endian) { m_endian = endian; }

  template <typename T, bool Safe = true>
  std::optional<T> Read(std::optional<size_t> offset = std::nullopt) {
    if (offset)
      Seek(*offset);
    static_assert(std::is_standard_layout<T>());
    if constexpr (Safe) {
      if (m_offset + sizeof(T) > m_data.size())
        return std::nullopt;
    }
    T value = util::BitCastPtr<T>(&m_data[m_offset]);
    util::SwapIfNeededInPlace(value, m_endian);
    m_offset += sizeof(T);
    return value;
  }

private:
  std::span<const u8> m_data{};
  size_t m_offset = 0;
  Endianness m_endian = Endianness::Big;
};

}  // namespace util