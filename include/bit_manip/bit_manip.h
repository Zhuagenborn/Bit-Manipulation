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
 *
 * @par GitHub
 * https://github.com/Zhuagenborn
 */

#pragma once

#include <algorithm>
#include <climits>
#include <concepts>
#include <cstdint>
#include <initializer_list>
#include <ranges>

namespace bit {

//! Get the specified bits in an integral value.
constexpr auto GetBits(const std::integral auto val, const std::size_t begin,
                       const std::size_t count) noexcept {
    if (count < sizeof(val) * CHAR_BIT) {
        const auto mask {((static_cast<decltype(val)>(1) << count) - 1) << begin};
        return static_cast<decltype(val)>((val & mask) >> begin);
    } else {
        return val;
    }
}

//! Clear the specified bits in an integral value.
constexpr void ClearBits(std::integral auto& val, const std::size_t begin,
                         const std::size_t count) noexcept {
    if (count < sizeof(val) * CHAR_BIT) {
        const auto mask {((static_cast<std::decay_t<decltype(val)>>(1) << count) - 1) << begin};
        val &= ~mask;
    } else {
        val = 0;
    }
}

//! Set the value of the specified bits in an integral value.
template <std::integral Bits>
constexpr void SetBits(std::integral auto& val, const Bits bits, const std::size_t begin,
                       const std::size_t count = sizeof(Bits) * CHAR_BIT) noexcept {
    if (count < sizeof(val) * CHAR_BIT) {
        ClearBits(val, begin, count);
        const auto mask {((static_cast<std::decay_t<decltype(val)>>(1) << count) - 1) << begin};
        val |= (static_cast<std::decay_t<decltype(val)>>(bits) << begin) & mask;
    } else {
        val = static_cast<std::decay_t<decltype(val)>>(bits);
    }
}

//! Fill the specified bits in an integral value.
constexpr void FillBits(std::integral auto& val, const std::size_t begin,
                        const std::size_t count) noexcept {
    SetBits(val, static_cast<std::decay_t<decltype(val)>>(-1), begin, count);
}

//! Check if a bit is set in an integral value.
constexpr bool IsBitSet(const std::integral auto val, const std::size_t idx) noexcept {
    return val & (static_cast<decltype(val)>(1) << idx);
}

//! Set a bit in an integral value.
constexpr void SetBit(std::integral auto& val, const std::size_t idx) noexcept {
    val |= (static_cast<std::decay_t<decltype(val)>>(1) << idx);
}

//! Clear a bit in an integral value.
constexpr void ClearBit(std::integral auto& val, const std::size_t idx) noexcept {
    val &= ~(static_cast<std::decay_t<decltype(val)>>(1) << idx);
}

//! @overload
constexpr void ClearBits(std::integral auto& val,
                         const std::initializer_list<std::size_t> idxs) noexcept {
    std::ranges::for_each(idxs, [&val](const auto idx) noexcept { ClearBit(val, idx); });
}

//! @overload
constexpr void FillBits(std::integral auto& val,
                        const std::initializer_list<std::size_t> idxs) noexcept {
    std::ranges::for_each(idxs, [&val](const auto idx) noexcept { SetBit(val, idx); });
}

//! Check if any specified bit is set in an integral value.
constexpr bool IsAnyBitSet(const std::integral auto val,
                           const std::initializer_list<std::size_t> idxs) noexcept {
    return std::ranges::any_of(idxs, [val](const auto idx) noexcept { return IsBitSet(val, idx); });
}

//! Check if all specified bits are set in an integral value.
constexpr bool AreAllBitsSet(const std::integral auto val,
                             const std::initializer_list<std::size_t> idxs) noexcept {
    return std::ranges::all_of(idxs, [val](const auto idx) noexcept { return IsBitSet(val, idx); });
}

//! Check if none of the specified bits is set in an integral value.
constexpr bool IsNoneBitSet(const std::integral auto val,
                            const std::initializer_list<std::size_t> idxs) noexcept {
    return std::ranges::none_of(idxs,
                                [val](const auto idx) noexcept { return IsBitSet(val, idx); });
}

//! Get a byte from an integral value.
constexpr std::uint8_t GetByte(const std::integral auto val, const std::size_t begin) noexcept {
    static_assert(sizeof(decltype(val)) >= sizeof(std::uint8_t));
    return static_cast<std::uint8_t>(GetBits(val, begin, sizeof(std::uint8_t) * CHAR_BIT));
}

//! Get a word from an integral value.
constexpr std::uint16_t GetWord(const std::integral auto val, const std::size_t begin) noexcept {
    static_assert(sizeof(decltype(val)) >= sizeof(std::uint16_t));
    return static_cast<std::uint16_t>(GetBits(val, begin, sizeof(std::uint16_t) * CHAR_BIT));
}

//! Get a double word from an integral value.
constexpr std::uint32_t GetDword(const std::integral auto val, const std::size_t begin) noexcept {
    static_assert(sizeof(decltype(val)) >= sizeof(std::uint32_t));
    return static_cast<std::uint32_t>(GetBits(val, begin, sizeof(std::uint32_t) * CHAR_BIT));
}

//! Set the value of a byte in an integral value.
constexpr void SetByte(std::integral auto& val, const std::uint8_t byte,
                       const std::size_t begin) noexcept {
    static_assert(sizeof(std::decay_t<decltype(val)>) >= sizeof(std::uint8_t));
    SetBits(val, byte, begin);
}

//! Set the value of a word in an integral value.
constexpr void SetWord(std::integral auto& val, const std::uint16_t word,
                       const std::size_t begin) noexcept {
    static_assert(sizeof(std::decay_t<decltype(val)>) >= sizeof(std::uint16_t));
    SetBits(val, word, begin);
}

//! Set the value of a double word in an integral value.
constexpr void SetDword(std::integral auto& val, const std::uint32_t dword,
                        const std::size_t begin) noexcept {
    static_assert(sizeof(std::decay_t<decltype(val)>) >= sizeof(std::uint32_t));
    SetBits(val, dword, begin);
}

//! Clear a byte in an integral value.
constexpr void ClearByte(std::integral auto& val, const std::size_t begin) noexcept {
    static_assert(sizeof(std::decay_t<decltype(val)>) >= sizeof(std::uint8_t));
    ClearBits(val, begin, sizeof(std::uint8_t) * CHAR_BIT);
}

//! Clear a word in an integral value.
constexpr void ClearWord(std::integral auto& val, const std::size_t begin) noexcept {
    static_assert(sizeof(std::decay_t<decltype(val)>) >= sizeof(std::uint16_t));
    ClearBits(val, begin, sizeof(std::uint16_t) * CHAR_BIT);
}

//! Clear a double word in an integral value.
constexpr void ClearDword(std::integral auto& val, const std::size_t begin) noexcept {
    static_assert(sizeof(std::decay_t<decltype(val)>) >= sizeof(std::uint32_t));
    ClearBits(val, begin, sizeof(std::uint32_t) * CHAR_BIT);
}

//! Fill a byte in an integral value.
constexpr void FillByte(std::integral auto& val, const std::size_t begin) noexcept {
    static_assert(sizeof(std::decay_t<decltype(val)>) >= sizeof(std::uint8_t));
    FillBits(val, begin, sizeof(std::uint8_t) * CHAR_BIT);
}

//! Fill a word in an integral value.
constexpr void FillWord(std::integral auto& val, const std::size_t begin) noexcept {
    static_assert(sizeof(std::decay_t<decltype(val)>) >= sizeof(std::uint16_t));
    FillBits(val, begin, sizeof(std::uint16_t) * CHAR_BIT);
}

//! Fill a double word in an integral value.
constexpr void FillDword(std::integral auto& val, const std::size_t begin) noexcept {
    static_assert(sizeof(std::decay_t<decltype(val)>) >= sizeof(std::uint32_t));
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

}  // namespace bit