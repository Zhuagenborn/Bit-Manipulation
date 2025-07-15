/**
 * @file bit_manip.h
 * @brief A header-only bit manipulation library.
 *
 * @details
 * It supports:
 *
 * - Getting bits, bytes, words or double words in an integral value.
 * - Clearing bits, bytes, words or double words in an integral value.
 * - Setting bits, bytes, words or double words in an integral value.
 * - Filling bits, bytes, words or double words in an integral value.
 * - Combining bits, bytes, words or double words to a larger integral value.
 * - Writing or reading bytes of an integral or a float value using the specified endianness.
 *
 * @par GitHub
 * https://github.com/Zhuagenborn
 */

#pragma once

#include <algorithm>
#include <array>
#include <bit>
#include <climits>
#include <concepts>
#include <cstdint>
#include <initializer_list>
#include <ranges>
#include <span>

namespace bit {

//! Get the specified bits in an integral value.
constexpr auto GetBits(const std::integral auto val, const std::size_t begin,
                       const std::size_t count) noexcept {
    constexpr auto total_bits {sizeof(val) * CHAR_BIT};
    if (begin >= total_bits) [[unlikely]] {
        return static_cast<decltype(val)>(0);
    } else if (count < total_bits) [[likely]] {
        const auto mask {((static_cast<decltype(val)>(1) << count) - 1) << begin};
        return static_cast<decltype(val)>((val & mask) >> begin);
    } else {
        return val;
    }
}

//! @overload
constexpr auto GetBits(const std::byte byte, const std::size_t begin,
                       const std::size_t count) noexcept {
    return GetBits(std::to_integer<std::uint8_t>(byte), begin, count);
}

//! Clear the specified bits in an integral value.
constexpr void ClearBits(std::integral auto& val, const std::size_t begin,
                         const std::size_t count) noexcept {
    constexpr auto total_bits {sizeof(val) * CHAR_BIT};
    if (begin >= total_bits) [[unlikely]] {
        return;
    } else if (count < total_bits) [[likely]] {
        const auto mask {((static_cast<std::decay_t<decltype(val)>>(1) << count) - 1) << begin};
        val &= ~mask;
    } else {
        val = 0;
    }
}

//! @overload
constexpr void ClearBits(std::byte& byte, const std::size_t begin,
                         const std::size_t count) noexcept {
    auto val {std::to_integer<std::uint8_t>(byte)};
    ClearBits(val, begin, count);
    byte = static_cast<std::byte>(val);
}

//! Set the value of the specified bits in an integral value.
template <std::integral Bits>
constexpr void SetBits(std::integral auto& val, const Bits bits, const std::size_t begin,
                       const std::size_t count = sizeof(Bits) * CHAR_BIT) noexcept {
    constexpr auto total_bits {sizeof(val) * CHAR_BIT};
    if (begin >= total_bits) [[unlikely]] {
        return;
    } else if (count < total_bits && begin + count <= total_bits) [[likely]] {
        ClearBits(val, begin, count);
        const auto mask {((static_cast<std::decay_t<decltype(val)>>(1) << count) - 1) << begin};
        val |= (static_cast<std::decay_t<decltype(val)>>(bits) << begin) & mask;
    } else {
        ClearBits(val, begin, total_bits - begin);
        const auto mask {(~static_cast<std::decay_t<decltype(val)>>(0)) << begin};
        val |= (static_cast<std::decay_t<decltype(val)>>(bits) << begin) & mask;
    }
}

//! @overload
template <typename Bits>
    requires std::integral<Bits> || std::same_as<Bits, std::byte>
constexpr void SetBits(std::byte& byte, const Bits bits, const std::size_t begin,
                       const std::size_t count) noexcept {
    auto val {std::to_integer<std::uint8_t>(byte)};
    SetBits(val, static_cast<std::uint8_t>(bits), begin, count);
    byte = static_cast<std::byte>(val);
}

//! Fill the specified bits in an integral value.
constexpr void FillBits(std::integral auto& val, const std::size_t begin,
                        const std::size_t count) noexcept {
    SetBits(val, static_cast<std::decay_t<decltype(val)>>(-1), begin, count);
}

//! @overload
constexpr void FillBits(std::byte& byte, const std::size_t begin,
                        const std::size_t count) noexcept {
    auto val {std::to_integer<std::uint8_t>(byte)};
    FillBits(val, begin, count);
    byte = static_cast<std::byte>(val);
}

//! Check if a bit is set in an integral value.
constexpr bool IsBitSet(const std::integral auto val, const std::size_t idx) noexcept {
    return idx < sizeof(val) * CHAR_BIT ? val & (static_cast<decltype(val)>(1) << idx) : false;
}

//! @overload
constexpr bool IsBitSet(const std::byte byte, const std::size_t idx) noexcept {
    return IsBitSet(std::to_integer<std::uint8_t>(byte), idx);
}

//! Set a bit in an integral value.
constexpr void SetBit(std::integral auto& val, const std::size_t idx) noexcept {
    if (idx < sizeof(val) * CHAR_BIT) [[likely]] {
        val |= (static_cast<std::decay_t<decltype(val)>>(1) << idx);
    }
}

//! @overload
constexpr void SetBit(std::byte& byte, const std::size_t idx) noexcept {
    auto val {std::to_integer<std::uint8_t>(byte)};
    SetBit(val, idx);
    byte = static_cast<std::byte>(val);
}

//! Clear a bit in an integral value.
constexpr void ClearBit(std::integral auto& val, const std::size_t idx) noexcept {
    if (idx < sizeof(val) * CHAR_BIT) [[likely]] {
        val &= ~(static_cast<std::decay_t<decltype(val)>>(1) << idx);
    }
}

//! @overload
constexpr void ClearBit(std::byte& byte, const std::size_t idx) noexcept {
    auto val {std::to_integer<std::uint8_t>(byte)};
    ClearBit(val, idx);
    byte = static_cast<std::byte>(val);
}

//! @overload
constexpr void ClearBits(std::integral auto& val,
                         const std::initializer_list<std::size_t> idxs) noexcept {
    std::ranges::for_each(idxs, [&val](const auto idx) noexcept { ClearBit(val, idx); });
}

//! @overload
constexpr void ClearBits(std::byte& byte, const std::initializer_list<std::size_t> idxs) noexcept {
    auto val {std::to_integer<std::uint8_t>(byte)};
    ClearBits(val, idxs);
    byte = static_cast<std::byte>(val);
}

//! @overload
constexpr void FillBits(std::integral auto& val,
                        const std::initializer_list<std::size_t> idxs) noexcept {
    std::ranges::for_each(idxs, [&val](const auto idx) noexcept { SetBit(val, idx); });
}

//! @overload
constexpr void FillBits(std::byte& byte, const std::initializer_list<std::size_t> idxs) noexcept {
    auto val {std::to_integer<std::uint8_t>(byte)};
    FillBits(val, idxs);
    byte = static_cast<std::byte>(val);
}

//! Check if any specified bit is set in an integral value.
constexpr bool IsAnyBitSet(const std::integral auto val,
                           const std::initializer_list<std::size_t> idxs) noexcept {
    return std::ranges::any_of(idxs, [val](const auto idx) noexcept { return IsBitSet(val, idx); });
}

//! @overload
constexpr bool IsAnyBitSet(const std::byte byte,
                           const std::initializer_list<std::size_t> idxs) noexcept {
    return IsAnyBitSet(std::to_integer<std::uint8_t>(byte), idxs);
}

//! Check if all specified bits are set in an integral value.
constexpr bool AreAllBitsSet(const std::integral auto val,
                             const std::initializer_list<std::size_t> idxs) noexcept {
    return std::ranges::all_of(idxs, [val](const auto idx) noexcept { return IsBitSet(val, idx); });
}

//! @overload
constexpr bool AreAllBitsSet(const std::byte byte,
                             const std::initializer_list<std::size_t> idxs) noexcept {
    return AreAllBitsSet(std::to_integer<std::uint8_t>(byte), idxs);
}

//! Check if none of the specified bits is set in an integral value.
constexpr bool IsNoneBitSet(const std::integral auto val,
                            const std::initializer_list<std::size_t> idxs) noexcept {
    return std::ranges::none_of(idxs,
                                [val](const auto idx) noexcept { return IsBitSet(val, idx); });
}

//! @overload
constexpr bool IsNoneBitSet(const std::byte byte,
                            const std::initializer_list<std::size_t> idxs) noexcept {
    return IsNoneBitSet(std::to_integer<std::uint8_t>(byte), idxs);
}

//! Get a byte from an integral value.
constexpr std::uint8_t GetByte(const std::integral auto val, const std::size_t begin) noexcept {
    static_assert(sizeof(decltype(val)) > sizeof(std::uint8_t));
    return static_cast<std::uint8_t>(GetBits(val, begin, sizeof(std::uint8_t) * CHAR_BIT));
}

//! Get a word from an integral value.
constexpr std::uint16_t GetWord(const std::integral auto val, const std::size_t begin) noexcept {
    static_assert(sizeof(decltype(val)) > sizeof(std::uint16_t));
    return static_cast<std::uint16_t>(GetBits(val, begin, sizeof(std::uint16_t) * CHAR_BIT));
}

//! Get a double word from an integral value.
constexpr std::uint32_t GetDword(const std::integral auto val, const std::size_t begin) noexcept {
    static_assert(sizeof(decltype(val)) > sizeof(std::uint32_t));
    return static_cast<std::uint32_t>(GetBits(val, begin, sizeof(std::uint32_t) * CHAR_BIT));
}

//! Set the value of a byte in an integral value.
constexpr void SetByte(std::integral auto& val, const std::uint8_t byte,
                       const std::size_t begin) noexcept {
    static_assert(sizeof(std::decay_t<decltype(val)>) > sizeof(std::uint8_t));
    SetBits(val, byte, begin);
}

//! Set the value of a word in an integral value.
constexpr void SetWord(std::integral auto& val, const std::uint16_t word,
                       const std::size_t begin) noexcept {
    static_assert(sizeof(std::decay_t<decltype(val)>) > sizeof(std::uint16_t));
    SetBits(val, word, begin);
}

//! Set the value of a double word in an integral value.
constexpr void SetDword(std::integral auto& val, const std::uint32_t dword,
                        const std::size_t begin) noexcept {
    static_assert(sizeof(std::decay_t<decltype(val)>) > sizeof(std::uint32_t));
    SetBits(val, dword, begin);
}

//! Clear a byte in an integral value.
constexpr void ClearByte(std::integral auto& val, const std::size_t begin) noexcept {
    static_assert(sizeof(std::decay_t<decltype(val)>) > sizeof(std::uint8_t));
    ClearBits(val, begin, sizeof(std::uint8_t) * CHAR_BIT);
}

//! Clear a word in an integral value.
constexpr void ClearWord(std::integral auto& val, const std::size_t begin) noexcept {
    static_assert(sizeof(std::decay_t<decltype(val)>) > sizeof(std::uint16_t));
    ClearBits(val, begin, sizeof(std::uint16_t) * CHAR_BIT);
}

//! Clear a double word in an integral value.
constexpr void ClearDword(std::integral auto& val, const std::size_t begin) noexcept {
    static_assert(sizeof(std::decay_t<decltype(val)>) > sizeof(std::uint32_t));
    ClearBits(val, begin, sizeof(std::uint32_t) * CHAR_BIT);
}

//! Fill a byte in an integral value.
constexpr void FillByte(std::integral auto& val, const std::size_t begin) noexcept {
    static_assert(sizeof(std::decay_t<decltype(val)>) > sizeof(std::uint8_t));
    FillBits(val, begin, sizeof(std::uint8_t) * CHAR_BIT);
}

//! Fill a word in an integral value.
constexpr void FillWord(std::integral auto& val, const std::size_t begin) noexcept {
    static_assert(sizeof(std::decay_t<decltype(val)>) > sizeof(std::uint16_t));
    FillBits(val, begin, sizeof(std::uint16_t) * CHAR_BIT);
}

//! Fill a double word in an integral value.
constexpr void FillDword(std::integral auto& val, const std::size_t begin) noexcept {
    static_assert(sizeof(std::decay_t<decltype(val)>) > sizeof(std::uint32_t));
    FillBits(val, begin, sizeof(std::uint32_t) * CHAR_BIT);
}

//! Combine low bits and high bits into a new integral value.
template <std::integral Ret, std::integral Bits>
constexpr Ret CombineBits(const Bits high, const Bits low) noexcept {
    static_assert(sizeof(Ret) >= 2 * sizeof(Bits));
    return (static_cast<Ret>(high) << (sizeof(Bits) * CHAR_BIT)) + low;
}

//! Combine a low byte and a high byte into a new word.
constexpr std::uint16_t CombineBytes(const std::uint8_t high, const std::uint8_t low) noexcept {
    return CombineBits<std::uint16_t>(high, low);
}

//! @overload
constexpr std::uint16_t CombineBytes(const std::byte high, const std::byte low) noexcept {
    return CombineBytes(std::to_integer<std::uint8_t>(high), std::to_integer<std::uint8_t>(low));
}

//! Combine a low word and a high word into a new double word.
constexpr std::uint32_t CombineWords(const std::uint16_t high, const std::uint16_t low) noexcept {
    return CombineBits<std::uint32_t>(high, low);
}

//! Combine a low double word and a high double word into a new quad word.
constexpr std::uint64_t CombineDwords(const std::uint32_t high, const std::uint32_t low) noexcept {
    return CombineBits<std::uint64_t>(high, low);
}

//! Get the low byte from a word.
constexpr std::uint8_t GetLowByte(const std::uint16_t val) noexcept {
    return GetByte(val, 0);
}

//! Get the low word from a double word.
constexpr std::uint16_t GetLowWord(const std::uint32_t val) noexcept {
    return GetWord(val, 0);
}

//! Get the low double word from a quad word.
constexpr std::uint32_t GetLowDword(const std::uint64_t val) noexcept {
    return GetDword(val, 0);
}

//! Get the high byte from a word.
constexpr std::uint8_t GetHighByte(const std::uint16_t val) noexcept {
    return GetByte(val, sizeof(std::uint8_t) * CHAR_BIT);
}

//! Get the high word from a double word.
constexpr std::uint16_t GetHighWord(const std::uint32_t val) noexcept {
    return GetWord(val, sizeof(std::uint16_t) * CHAR_BIT);
}

//! Get the high double word from a quad word.
constexpr std::uint32_t GetHighDword(const std::uint64_t val) noexcept {
    return GetDword(val, sizeof(std::uint32_t) * CHAR_BIT);
}

//! Set the value of the low byte in a word.
constexpr void SetLowByte(std::uint16_t& val, const std::uint8_t byte) noexcept {
    SetByte(val, byte, 0);
}

//! Set the value of the low word in a double word.
constexpr void SetLowWord(std::uint32_t& val, const std::uint16_t word) noexcept {
    SetWord(val, word, 0);
}

//! Set the value of the low double word in a quad word.
constexpr void SetLowDword(std::uint64_t& val, const std::uint32_t dword) noexcept {
    SetDword(val, dword, 0);
}

//! Clear the low byte in a word.
constexpr void ClearLowByte(std::uint16_t& val) noexcept {
    ClearByte(val, 0);
}

//! Clear the low word in a double word.
constexpr void ClearLowWord(std::uint32_t& val) noexcept {
    ClearWord(val, 0);
}

//! Clear the low double word in a quad word.
constexpr void ClearLowDword(std::uint64_t& val) noexcept {
    ClearDword(val, 0);
}

//! Fill the low byte in a word.
constexpr void FillLowByte(std::uint16_t& val) noexcept {
    FillByte(val, 0);
}

//! Fill the low word in a double word.
constexpr void FillLowWord(std::uint32_t& val) noexcept {
    FillWord(val, 0);
}

//! Fill the low double word in a quad word.
constexpr void FillLowDword(std::uint64_t& val) noexcept {
    FillDword(val, 0);
}

//! Set the value of the high byte in a word.
constexpr void SetHighByte(std::uint16_t& val, const std::uint8_t byte) noexcept {
    SetByte(val, byte, sizeof(std::uint8_t) * CHAR_BIT);
}

//! Set the value of the high word in a double word.
constexpr void SetHighWord(std::uint32_t& val, const std::uint16_t word) noexcept {
    SetWord(val, word, sizeof(std::uint16_t) * CHAR_BIT);
}

//! Set the value of the high double word in a quad word.
constexpr void SetHighDword(std::uint64_t& val, const std::uint32_t dword) noexcept {
    SetDword(val, dword, sizeof(std::uint32_t) * CHAR_BIT);
}

//! Fill the high byte in a word.
constexpr void FillHighByte(std::uint16_t& val) noexcept {
    FillByte(val, sizeof(std::uint8_t) * CHAR_BIT);
}

//! Fill the high word in a double word.
constexpr void FillHighWord(std::uint32_t& val) noexcept {
    FillWord(val, sizeof(std::uint16_t) * CHAR_BIT);
}

//! Fill the high double word in a quad word.
constexpr void FillHighDword(std::uint64_t& val) noexcept {
    FillDword(val, sizeof(std::uint32_t) * CHAR_BIT);
}

//! Clear the high byte in a word.
constexpr void ClearHighByte(std::uint16_t& val) noexcept {
    ClearByte(val, sizeof(std::uint8_t) * CHAR_BIT);
}

//! Clear the high word in a double word.
constexpr void ClearHighWord(std::uint32_t& val) noexcept {
    ClearWord(val, sizeof(std::uint16_t) * CHAR_BIT);
}

//! Clear the high double word in a quad word.
constexpr void ClearHighDword(std::uint64_t& val) noexcept {
    ClearDword(val, sizeof(std::uint32_t) * CHAR_BIT);
}

/**
 * @brief Writes bytes of a value to a buffer using the specified endianness.
 *
 * @return
 * Whether the buffer is large enough to store the value.
 * If it is @p false, bytes are written to the buffer from the least significant byte.
 */
template <typename T>
    requires std::is_arithmetic_v<T> || std::same_as<T, std::byte>
constexpr bool WriteBytes(const T& from, const std::span<std::byte> to,
                          const std::endian endian = std::endian::native) noexcept {
    if constexpr (sizeof(T) > sizeof(std::byte)) {
        auto tmp_buf {std::bit_cast<std::array<std::byte, sizeof(T)>>(from)};
        const auto size {std::min(sizeof(T), to.size())};
        if (endian != std::endian::native) [[likely]] {
            std::ranges::reverse(tmp_buf);
            std::ranges::copy(std::span {tmp_buf}.last(size), to.begin());
        } else {
            std::ranges::copy(std::span {tmp_buf}.first(size), to.begin());
        }

        return to.size() >= sizeof(T);
    } else {
        if (!to.empty()) [[likely]] {
            to.front() = static_cast<std::byte>(from);
            return true;
        } else {
            return false;
        }
    }
}

/**
 * @brief Read bytes from a buffer to a value using the specified endianness.
 *
 * @return
 * Whether the buffer is large enough to load the value from.
 * If it is @p false, bytes are read from the buffer from the least significant byte.
 */
template <typename T>
    requires std::is_arithmetic_v<T> || std::same_as<T, std::byte>
constexpr bool ReadBytes(const std::span<const std::byte> from, T& to,
                         const std::endian endian = std::endian::native) noexcept {
    if constexpr (sizeof(T) > sizeof(std::byte)) {
        alignas(T) std::array<std::byte, sizeof(T)> tmp_buf {};
        const auto size {std::min(sizeof(T), from.size())};
        if (endian != std::endian::native) [[likely]] {
            std::ranges::copy(from.first(size), tmp_buf.end() - size);
            std::ranges::reverse(tmp_buf);
        } else {
            std::ranges::copy(from.first(size), tmp_buf.begin());
        }

        to = std::bit_cast<T>(tmp_buf);
        return from.size() >= sizeof(T);
    } else {
        if (!from.empty()) [[likely]] {
            to = static_cast<T>(from.front());
            return true;
        } else {
            return false;
        }
    }
}

}  // namespace bit