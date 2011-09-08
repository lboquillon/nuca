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
#endif
