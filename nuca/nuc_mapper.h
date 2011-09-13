#ifndef NUC_MAPPER_H
#define NUC_MAPPER_H

#include <stdint.h>
#include <limits>

namespace nuca
{
template<size_t nucNumber>
struct NucMapper;

template<>
struct NucMapper<4>
{
    typedef uint8_t NucSizeType;
    static const uint8_t max;
};

const uint8_t NucMapper<4>::max = std::numeric_limits<uint8_t>::max();

template<>
struct NucMapper<8>
{
    typedef uint16_t NucSizeType;
    static const uint16_t max;
};

const uint16_t NucMapper<8>::max = std::numeric_limits<uint16_t>::max();

template<>
struct NucMapper<16>
{
    typedef uint32_t NucSizeType;
    static const uint32_t max;
};

const uint32_t NucMapper<16>::max = std::numeric_limits<uint32_t>::max();

template<>
struct NucMapper<32>
{
    typedef uint64_t NucSizeType;
    static const uint64_t max;
};

const uint64_t NucMapper<32>::max = std::numeric_limits<uint64_t>::max();
}

#endif
