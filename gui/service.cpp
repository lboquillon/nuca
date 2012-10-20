#include <iostream>
#include <fstream>
#include "service.h"
#include "nuca/nuca.h"

void Service::compression(const std::string& fileIn, const std::string& fileOut, bool& multipleSequence)
{
    std::string name;
    std::ofstream os(fileOut.c_str(), std::ios_base::binary);
    FastaLoader<RemoveNs<CompressingBitHandler<NucaFormatWriter<EndLayer> > > > compressor;
    compressor.setOstream(os);
    compressor.setFastaFile(fileIn);
    compressor.setSeqNameString(name);
    compressor.run(multipleSequence);
}

void Service::decompression(const std::string& fileIn, const std::string& fileOut)
{
    NucaFormatLoader<DecompressingBitHandler<AddNs<FastaWriter<EndLayer> > > > decompressor;

    std::ifstream is(fileIn.c_str(), std::ios_base::binary);

    decompressor.setSeqName("Sequence decompressed with NUCA");
    decompressor.setFastaFileOut(fileOut);
    decompressor.setIstream(is);
    decompressor.run();
}
