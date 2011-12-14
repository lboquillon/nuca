/*
fasta_format_saver.h: Nucleotides Compression Algorithms
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

#ifndef FASTA_FORMAT_SAVER_H
#define FASTA_FORMAT_SAVER_H

#include <fstream>
#include <string>
#include <iostream>

template<class UpperLayer, class LowerLayer>
class ConvertDataType;

template<class LowerLayer>
class FastaFormatSaver : public LowerLayer
{
    std::ostream* of;
    size_t currentCharNumber;
    static const size_t lineLimit = 50;
public:
    typedef unsigned char DataType;

    FastaFormatSaver()
        : of(NULL),
          currentCharNumber(0)
    {}

    void setOstream(std::ostream&);
    void receiveData(DataType);
    void end(DataType);
};

template<class LowerLayer>
inline void FastaFormatSaver<LowerLayer>::receiveData(DataType buffer)
{

    for (size_t byte = 0; byte < sizeof(buffer); ++byte)
    {
        LowerLayer::receiveData(ConvertDataType<FastaFormatSaver<LowerLayer>, LowerLayer>::convert(buffer & 0xff));
        of->put(static_cast<DataType>(buffer & 0xff));
        buffer >>= 8;
    }

    if (++currentCharNumber == lineLimit)
    {
        *(of) << std::endl;
        currentCharNumber = 0;
    }
}

template<class LowerLayer>
inline void FastaFormatSaver<LowerLayer>::end(DataType n)
{
    receiveData(n);
}

template<class LowerLayer>
inline void FastaFormatSaver<LowerLayer>::setOstream(std::ostream& ostr)
{
    of = &ostr;
}

#endif

