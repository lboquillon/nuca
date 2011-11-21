/*
compressing_decompressing_file_test.cpp:: Nucleotides Compression Algorithms
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

    This is a test file.
*/

#include <gtest/gtest.h>
#include "nuca/nuca.h"

typedef RemoveNs<CompressingBitHandler<OstreamSaver<CompressionEventsCounterLayer<EndLayer> > > > TestCompressor;
typedef IstreamLoader<DecompressingBitHandler<AddNs<StringTestLayer<EndLayer> > > > TestDecompressor;

TEST(CompressingDecompressingFile, compressingDecompressingFile1)
{
    std::ofstream of("file.out", std::ios_base::binary);

    std::string out;
    const std::string sequence = "TACGACNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNACG";
    TestCompressor cmpr;
    cmpr.setOstream(of);

    for (std::string::const_iterator it = sequence.begin(); it < sequence.end(); ++it)
        cmpr.receiveData(*it);

    cmpr.receiveData(nuca::EndSeq);
    of.close();

    std::ifstream is("file.out", std::ios_base::binary);
    TestDecompressor decompressor;
    decompressor.setIstream(is);
    decompressor.setOutputString(out);
    decompressor.run();

    is.close();

    ASSERT_EQ(sequence, out);
}

TEST(CompressingDecompressingFile, compressingDecompressingFile2)
{
    std::ofstream of("file.out", std::ios_base::binary);

    std::string out;
    const std::string sequence = "TACGACNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNACG";
    TestCompressor cmpr;
    cmpr.setOstream(of);

    for (std::string::const_iterator it = sequence.begin(); it < sequence.end(); ++it)
        cmpr.receiveData(*it);

    cmpr.receiveData(nuca::EndSeq);
    of.close();

    std::ifstream is("file.out", std::ios_base::binary);
    TestDecompressor decompressor;
    decompressor.setIstream(is);
    decompressor.setOutputString(out);
    decompressor.run();

    is.close();

    ASSERT_EQ(sequence, out);
}

TEST(CompressingDecompressingFile, compressingDecompressingFile3)
{
    std::ofstream of("file.out", std::ios_base::binary);

    std::string out;
    const std::string sequence = "ACNTACGACGAGGTTGCAGTGAGCCGACACAGTGCCTACGACAACACTAACTAGCCTGGGCATACGACTACGACN";
    TestCompressor cmpr;
    cmpr.setOstream(of);

    for (std::string::const_iterator it = sequence.begin(); it < sequence.end(); ++it)
        cmpr.receiveData(*it);

    cmpr.receiveData(nuca::EndSeq);
    of.close();

    std::ifstream is("file.out", std::ios_base::binary);
    TestDecompressor decompressor;
    decompressor.setIstream(is);
    decompressor.setOutputString(out);
    decompressor.run();

    is.close();

    ASSERT_EQ(sequence, out);
}

