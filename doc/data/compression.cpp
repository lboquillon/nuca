#include "nuca.h"
#include <iostream>
#include <fstream>

typedef RemoveNs<CompressingBitHandler<OstreamSaver<EndLayer> > > Compressor;
typedef IstreamLoader<DecompressingBitHandler<AddNs<OstreamSaver<EndLayer> > > > Decompressor;

void compress(char* orig, char* dest)
{
    Compressor cmpr;
    std::ifstream is;
    std::ofstream os(dest, std::ios_base::binary);
    char c;

    is.open(orig);
    cmpr.setOstream(os);

    while (is.get(c))
        cmpr.receiveData(c);

    cmpr.receiveData(nuca::EndSeq);
}

void decompress(char* orig, char* dest)
{
    Decompressor dcmpr;

    std::ifstream is(orig, std::ios_base::binary);
    std::ofstream os(dest);

    dcmpr.setIstream(is);
    dcmpr.setOstream(os);

    dcmpr.run();
}

int main(int argc, char* argv[])
{
    if (!strcmp(argv[1], "-c"))
    {
        compress(argv[2], argv[3]);
    }
    else if (!strcmp(argv[1], "-d"))
    {
        decompress(argv[2], argv[3]);
    }

    return 0;
}
