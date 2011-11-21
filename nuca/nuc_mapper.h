/*
nuc_mapper.h: Nucleotides Compression Algorithms
    Copyright (C) 2011 Leonardo Boquillon and Daniel Gutson, FuDePAN

    This file is part of Nuca.

    Nuca is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    Nuca is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with Nuca. If not, see <http://www.gnu.org/licenses/>.

    NOTE: This file is in prototype stage, and is under active development.
*/

#ifndef NUC_MAPPER_H
#define NUC_MAPPER_H

#include <stdint.h>
#include <climits>

namespace nuca
{
template<size_t nucNumber>
struct NucMapper;

template<>
struct NucMapper<4>
{
    typedef uint8_t NucSizeType;
    static const uint8_t max = UCHAR_MAX;
};

template<>
struct NucMapper<8>
{
    typedef uint16_t NucSizeType;
    static const uint16_t max = USHRT_MAX;
};

template<>
struct NucMapper<16>
{
    typedef uint32_t NucSizeType;
    static const uint32_t max = UINT_MAX;
};

template<>
struct NucMapper<32>
{
    typedef uint64_t NucSizeType;
    static const uint64_t max = ULONG_MAX;
};

}

#endif
