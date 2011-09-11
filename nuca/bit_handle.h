/*
bit_handle.h: Nucleotides Compression Algorithms
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

#ifndef BIT_HANDLE
#define BIT_HANDLE

#include <iostream>
#include <biopp/bio_molecular/bio_molecular.h>

template<class UpperLayer, class LowerLayer>
class ConvertDataType;

using namespace std;

template<class LowerLayer>
class CompressingBitHandle : public LowerLayer
{
    size_t free_bits;
    unsigned char buffer;
    typedef unsigned int Bits;
    static const Bits DataSize = 2;
    static const Bits TotalFreeBits = sizeof(buffer) << 3;
    void flush();

public:
    typedef unsigned char DataType;

    CompressingBitHandle()
        : free_bits(TotalFreeBits),
          buffer(0)
    {
    }

    ~CompressingBitHandle()
    {
        flush();
    }

    void receiveData(DataType);
    void end(DataType);
};


template<class LowerLayer>
inline void CompressingBitHandle<LowerLayer>::flush()
{
    if (free_bits < TotalFreeBits)
    {
        LowerLayer::receiveData(ConvertDataType<CompressingBitHandle<LowerLayer>, LowerLayer>::convert(buffer));
        free_bits = TotalFreeBits;
    }
    buffer = 0;
}

template<class LowerLayer>
inline void CompressingBitHandle<LowerLayer>::receiveData(DataType data)
{
    if (free_bits < DataSize)
        flush();

    buffer |= (data << (TotalFreeBits - free_bits));     // optimize out!
    free_bits -= DataSize;
}

template<class LowerLayer>
inline void CompressingBitHandle<LowerLayer>::end(DataType size)
{
    flush();
    LowerLayer::end(size);
}

//  ======================== Decompressing Bit Handle ==============================

template<class UpperLayer, class LowerLayer>
class ConvertDataType;

template<class LowerLayer>
class DecompressingBitHandle : public virtual Fsm, public LowerLayer
{
    typedef unsigned char Byte;
    size_t current_bit;
    Byte buffer;
    static const Byte DataSize = 2;
    std::queue<Byte> pending;

    void flush();
public:
    typedef Byte DataType;

    DecompressingBitHandle()
    { }

    void end(DataType);
    void receiveData(DataType);
};

template<class LowerLayer>
void DecompressingBitHandle<LowerLayer>::flush()
{
    buffer = pending.front();
    pending.pop();

    while (current_bit < 8)
    {
        LowerLayer::receiveData(ConvertDataType<DecompressingBitHandle<LowerLayer>, LowerLayer>::convert(valueToNuc(buffer & 0x3)));
        buffer >>= DataSize;
        current_bit += DataSize;
    }
}

template<class LowerLayer>
void DecompressingBitHandle<LowerLayer>::receiveData(DataType data)
{
    if (!pending.empty())
    {
        current_bit = 0;
        flush();
    }

    pending.push(data);
}

template<class LowerLayer>
void DecompressingBitHandle<LowerLayer>::end(DataType size)
{
    current_bit = 8 - (size << 1);
    flush();
    LowerLayer::end();
}

#endif
