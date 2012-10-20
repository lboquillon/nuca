#include <iostream>
#include <fstream>
#include "service.h"
#include "nuca/nuca.h"

void Service::compression(const std::string& fileIn, const std::string& fileOut)
{
    std::ofstream os(fileOut.c_str(), std::ios_base::binary);
    FastaLoader<RemoveNs<CompressingBitHandler<NucaFormatWriter<EndLayer> > > > compressor;
    compressor.setOstream(os);
    compressor.setSeqNameString(sequenceName);
    compressor.setFastaFile(fileIn);
    compressor.run();
}

void Service::decompression(const std::string& fileIn, const std::string& fileOut)
{
    NucaFormatLoader<DecompressingBitHandler<AddNs<FastaWriter<EndLayer> > > > decompressor;

    std::ifstream is(fileIn.c_str(), std::ios_base::binary);

    decompressor.setSeqName(sequenceName);
    decompressor.setFastaFileOut(fileOut);
    decompressor.setIstream(is);
    decompressor.run();
}
