#include <iostream>
#include <fstream>
#include "service.h"
#include "nuca/nuca.h"

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
            if (!str.empty() && str[0] != '>')
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
        throw "The file was not found";
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
