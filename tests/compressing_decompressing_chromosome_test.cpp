/*
compressing_decompressing_test.cpp: Nucleotides Compression Algorithms
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
#include "nuca.h"

/*
    The files chr7.fa and chr8.fa are not found within the project
    These can be obtained from http://hgdownload.cse.ucsc.edu/goldenPath/hg19/chromosomes/
*/

typedef RemoveNs<AddNs<ExceptionTestLayer<EndLayer> > > TestCompressor;

TEST(CompressingDecompressingChromosome, chromosome7)
{
    TestCompressor cmpr;
    std::ifstream chr("chr7.fa");

    char c;
    while (chr.get(c))
    {
        c = toupper(c);

        // delete blank space, tabs and others chars

        if (c == 'N' || c == 'A' || c == 'C' || c == 'T' || c == 'G')
        {
            cmpr.receiveData(c);
            cmpr.setNucTest(c);
        }
    }

    chr.close();
    cmpr.receiveData(nuca::EndSeq);
}

TEST(CompressingDecompressingChromosome, chromosome8)
{
    TestCompressor cmpr;
    std::ifstream chr("chr8.fa");

    char c;
    while (chr.get(c))
    {
        c = toupper(c);

        if (c == 'N' || c == 'A' || c == 'C' || c == 'T' || c == 'G')
        {
            cmpr.receiveData(c);
            cmpr.setNucTest(c);
        }
    }

    chr.close();
    cmpr.receiveData(nuca::EndSeq);
}
