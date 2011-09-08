/*
ostream_saver.h: Nucleotides Compression Algorithms
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

#ifndef SAVER
#define SAVER

#include <fstream>
#include <string>
#include <iostream>

template<class UpperLayer, class LowerLayer>
class ConvertDataType;

template<class LowerLayer>
class OstreamSaver : public LowerLayer
{
    std::ostream* of;
public:
    typedef char DataType;

    OstreamSaver()
        : of(NULL)
    {}

	void setOstream (std::ostream &);
    void receiveData(DataType);
    void end(DataType);
};

template<class LowerLayer>
inline void OstreamSaver<LowerLayer>::receiveData(DataType buffer)
{
    for (size_t byte = 0; byte < sizeof(buffer); ++byte)
    {
        of->put(static_cast<char>(buffer & 0xff));
        //cerr << std::hex << (buffer & 0xff) << endl;
        buffer >>= 8;
    }
}

template<class LowerLayer>
inline void OstreamSaver<LowerLayer>::end(DataType n)
{
    receiveData(n);
    //of->close(); close only for ofstream, what is going to do?
}

template<class LowerLayer>
inline void OstreamSaver<LowerLayer>::setOstream (std::ostream & ostr)
{
	of = &ostr;
}

#endif
