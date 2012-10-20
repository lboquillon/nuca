/*
fsm.h: Nucleotides Compression Algorithms
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

    NOTE: This file is in prototype stage, and is under active development.
*/

#ifndef FSM_H
#define FSM_H

#include <string>
#include "biopp/biopp.h"

namespace nuca
{
enum EndSeqStimulus
{
    EndSeq
};

/*
    justification of sequence choice

    Sequence   Chr 12      Chr 8
    ACTG       573121     630235
    AGTC       390835     429101
    CGTA        56132      63786
    CGAT        66736      70548
    CTAG       356350     388702
    GCTA       349476     375216
    CTGA       656105     716670
    ATCG        67100      70490
    TACG        55883      63289
    TGCA       648464     717338
    TCGA        69483      71394
    GTCA       405210     452050
    ACGT        95910     110463
*/

static const std::string rareSeq = "UACGAC";

inline char valueToNuc(size_t value)
{
    return biopp::Nucleotide(biopp::Nucleotide::Alpha(value)).as_char();
}
}

#endif
