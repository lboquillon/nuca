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

TEST(CompressingDecompressing, compressingDecompressing1)
{
    const std::string in = "CTGANNNNNNTTCGAAANNNNNNT";
    std::string strcm;
    std::string out;

    DecompressingFSM dcpr(out);
    CompressingFSM cmpr(strcm);

    for (std::string::const_iterator it = in.begin(); it < in.end(); ++it)
        cmpr.stimulate(*it);

    cmpr.stimulate(EndSeq);

    for (std::string::iterator it = strcm.begin(); it < strcm.end(); ++it)
        dcpr.stimulate(*it);

    dcpr.stimulate(EndSeq);

    ASSERT_EQ(in, out);
}

TEST(CompressingDecompressing, compressingDecompressing2)
{
    const std::string in = "CTGANNNNNNTTCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAATTTTGAAANNNNNNT";
    std::string strcm;
    std::string out;

    DecompressingFSM dcpr(out);
    CompressingFSM cmpr(strcm);

    for (std::string::const_iterator it = in.begin(); it < in.end(); ++it)
        cmpr.stimulate(*it);

    cmpr.stimulate(EndSeq);

    for (std::string::iterator it = strcm.begin(); it < strcm.end(); ++it)
        dcpr.stimulate(*it);

    dcpr.stimulate(EndSeq);

    ASSERT_EQ(in, out);
}
