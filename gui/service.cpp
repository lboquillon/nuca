#include "service.h"

Service::Service()
{
}

void Service::compression(const std::string& fileIn, const std::string& fileOut)
{
    std::ofstream os(fileOut.c_str(), std::ios_base::binary);
    RemoveNs<CompressingBitHandler<OstreamSaver<EndLayer> > > compressor;
    compressor.setOstream(os);

    std::ifstream is(fileIn.c_str());
    char c;

    while (is.get(c))
    {
        c = toupper(c);

        if (c == 'C' || c == 'T' || c == 'G' || c == 'A' || c == 'N')
            compressor.receiveData(c);
    }

    compressor.receiveData(nuca::EndSeq);
}

void Service::decompression(const std::string& fileIn, const std::string& fileOut)
{
    IstreamLoader<DecompressingBitHandler<AddNs<OstreamSaver<EndLayer> > > > decompressor;

    std::ifstream is(fileIn.c_str(), std::ios_base::binary);
    std::ofstream os(fileOut.c_str());

    decompressor.setIstream(is);
    decompressor.setOstream(os);

    decompressor.run();
}
