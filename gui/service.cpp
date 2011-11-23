#include <iostream>
#include <fstream>
#include "service.h"

void compression(const std::string& fileIn, const std::string& fileOut)
{
    std::ofstream os(fileOut.c_str(), std::ios_base::binary);
    RemoveNs<CompressingBitHandler<OstreamSaver<EndLayer> > > compressor;
    compressor.setOstream(os);

    std::ifstream is(fileIn.c_str());
    if (is)
    {

        std::string str;

        while (getline(is, str))
        {
            if (str.size() > 0 && str[0] != '>')
                for (size_t i = 0; i < str.size(); ++i)
                {
                    char c = toupper(str[i]);
                    if (c == 'C' || c  == 'T' || c == 'G' || c == 'A' || c == 'N')
                        compressor.receiveData(c);
                }
        }

        compressor.receiveData(nuca::EndSeq);
    }
    else
    {
        throw "An error has occurred";
    }

}

void decompression(const std::string& fileIn, const std::string& fileOut)
{
    IstreamLoader<DecompressingBitHandler<AddNs<OstreamSaver<EndLayer> > > > decompressor;

    std::ifstream is(fileIn.c_str(), std::ios_base::binary);
    std::ofstream os(fileOut.c_str());

    decompressor.setIstream(is);
    decompressor.setOstream(os);

    decompressor.run();
}
