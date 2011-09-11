/*
istream_loader.h: Nucleotides Compression Algorithms
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

#ifndef ISTREAM_LOADER
#define ISTREAM_LOADER

#include <iostream>
#include <fstream>

template<class UpperLayer, class LowerLayer>
class ConvertDataType;

template<class LowerLayer>
class IstreamLoader : public LowerLayer
{
    std::istream* loader;
public:
    typedef char DataType;

    IstreamLoader()
    { }

    void setIstream(std::istream&);
    void run();
};

template<class LowerLayer>
void IstreamLoader<LowerLayer>::setIstream(std::istream& ist)
{
    loader = &ist;
}

template<class LowerLayer>
void IstreamLoader<LowerLayer>::run()
{
    DataType last;
    DataType current;
    loader->get(last);

    while (loader->get(current))
    {
        LowerLayer::receiveData(ConvertDataType<IstreamLoader<LowerLayer>, LowerLayer>::convert(last));
        last = current;
    }

    LowerLayer::end(last);
}

#endif
