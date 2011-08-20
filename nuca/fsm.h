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

enum EndSeqStimulus
{
    EndSeq
};

class Fsm
{
protected:
    const std::string rareSeq;
    std::string& outSeq;

    void addMissingNuc(size_t n)
    {
        outSeq += rareSeq.substr(0, n);
    }

public:
    Fsm(std::string& out)
        : rareSeq("TTT"),
          outSeq(out)
    {
        outSeq = "";
    }

};

#endif