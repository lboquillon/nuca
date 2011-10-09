#include "nuca.h"
#include <iostream>
#include <fstream>

typedef RemoveNs<CompressingBitHandler<OstreamSaver<EndLayer> > > Compressor;

int main(int argc, char* argv[])
{
    Compressor cmpr;
    std::ifstream is;
    std::ofstream os(argv[2], std::ios_base::binary);
    char c;

    is.open(argv[1]);
    cmpr.setOstream(os);

    while (is.get(c))
        cmpr.receiveData(c);

    cmpr.receiveData(nuca::EndSeq);

    return 0;
}
