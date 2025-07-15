# Bit Manipulation

![C++](docs/badges/C++.svg)
[![CMake](docs/badges/Made-with-CMake.svg)](https://cmake.org)
![GitHub Actions](docs/badges/Made-with-GitHub-Actions.svg)
![License](docs/badges/License-MIT.svg)

## Introduction

A header-only bit manipulation library written in *C++23*, supporting:

- Getting bits, bytes, words or double words in an integral value.
- Clearing bits, bytes, words or double words in an integral value.
- Setting bits, bytes, words or double words in an integral value.
- Filling bits, bytes, words or double words in an integral value.
- Combining bits, bytes, words or double words to a larger integral value.
- Writing or reading bytes of an integral or a float value using the specified endianness.

## Unit Tests

### Prerequisites

- Install *GoogleTest*.
- Install *CMake*.

### Building

Go to the project folder and run:

```bash
mkdir -p build
cd build
cmake -DBIT_MANIP_BUILD_TESTS=ON ..
cmake --build .
```

### Running

Go to the `build` folder and run:

```bash
ctest -VV
```

## Examples

See more examples in `tests/bit_manip_tests.cpp`.

```c++
// Get the specified bits in an integral value.
constexpr std::uint32_t val {0x12345678};
EXPECT_EQ(GetBits(val, 0, sizeof(std::uint8_t) * CHAR_BIT), 0x78);
EXPECT_EQ(GetBits(val, 0, sizeof(std::uint16_t) * CHAR_BIT), 0x5678);
```

```c++
// Set the value of the specified bits in an integral value.
std::uint32_t val {0x12345678};
SetBits(val, 0xFFFF, 0, sizeof(std::uint16_t) * CHAR_BIT);
EXPECT_EQ(val, 0x1234FFFF);
```

```c++
// Fill the specified bits in an integral value.
std::uint32_t val {0x12345678};
FillBits(val, 0, sizeof(std::uint8_t) * CHAR_BIT);
EXPECT_EQ(val, 0x123456FF);
```

```c++
// Clear the specified bits in an integral value.
std::uint32_t val {0x12345678};
ClearBits(val, 0, sizeof(std::uint16_t) * CHAR_BIT);
EXPECT_EQ(val, 0x12340000);
```

```c++
// Combine low bits and high bits into a new integral value.
EXPECT_EQ((CombineBits<std::uint16_t, std::uint8_t>(0x12, 0x34)), 0x1234);
EXPECT_EQ((CombineBits<std::uint32_t, std::uint16_t>(0x1234, 0x5678)), 0x12345678);
```

```c++
// Check if a bit is set in an integral value.
constexpr std::uint8_t val {0b0000'0001};
EXPECT_TRUE(IsBitSet(val, 0));
EXPECT_FALSE(IsBitSet(val, 1));
```

```c++
// Read bytes using the specified endianness.
constexpr int buf {0x12345678};
int val {0};
ReadBytes(std::as_bytes(std::span {&buf, 1}), val, GetOppositeEndian());
EXPECT_EQ(val, std::byteswap(buf));
```

## License

Distributed under the *MIT License*. See `LICENSE` for more information.