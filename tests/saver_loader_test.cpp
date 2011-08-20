/*
saver_loader_test.cpp: Nucleotides Compression Algorithms
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
#include <biopp/bio_molecular/bio_molecular.h>
#include "nuca.h"

TEST(saver_loader, equalStr1)
{
    const NucSequence in("ATTCCTTGTGGTTTCCAAGGTTCTCTCTAGTCCTGAC");
    NucSequence out;
    Saver saver("archivo.out");
    Storer<Saver> storer(saver, in.length());
    storeSeq(storer, in);

    Loader loader("archivo.out");
    Retriever<Loader> retriever(loader);
    loadSeq(retriever, out);

    ASSERT_EQ(in, out);
}

TEST(saver_loader, equalStr2)
{
    const NucSequence in("CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAACCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTGGGGGGGGGGGGGGGGGGGGGGGGGGGACTGAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAACCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGTCAGTCA");
    NucSequence out;
    Saver saver("archivo.out");
    Storer<Saver> storer(saver, in.length());
    storeSeq(storer, in);

    Loader loader("archivo.out");
    Retriever<Loader> retriever(loader);
    loadSeq(retriever, out);

    ASSERT_EQ(in, out);
}
