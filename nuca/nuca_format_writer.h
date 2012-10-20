/*
nuca_format_writer.h: Nucleotides Compression Algorithms
    Copyright (C) 2011 Daniel Gutson and Leonardo Boquillon, FuDePAN

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

#ifndef NUCA_FORMAT_WRITER_H
#define NUCA_FORMAT_WRITER_H

#include <fstream>
#include <string>
#include <iostream>

template<class UpperLayer, class LowerLayer>
class ConvertDataType;

template<class LowerLayer>
class NucaFormatWriter : public LowerLayer
{
    std::ostream* of;
public:
    typedef char DataType;

    NucaFormatWriter()
        : of(NULL)
    {}

    void setOstream(std::ostream&);
    void receiveData(DataType);
    void end(DataType);
};

template<class LowerLayer>
inline void NucaFormatWriter<LowerLayer>::receiveData(DataType buffer)
{
    for (size_t byte = 0; byte < sizeof(buffer); ++byte)
    {
        of->put(static_cast<char>(buffer & 0xff));
        LowerLayer::receiveData(ConvertDataType<NucaFormatWriter<LowerLayer>, LowerLayer>::convert(buffer & 0xff));
        buffer >>= 8;
    }
}

template<class LowerLayer>
inline void NucaFormatWriter<LowerLayer>::end(DataType n)
{
    receiveData(n);
    //of->close(); close only for ofstream, what is going to do?
}

template<class LowerLayer>
inline void NucaFormatWriter<LowerLayer>::setOstream(std::ostream& ostr)
{
    of = &ostr;
}

#endif
