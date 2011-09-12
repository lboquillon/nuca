/*
tests_layers.h: Nucleotides Compression Algorithms
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

#ifndef TEST_LAYERS_H
#define TEST_LAYERS_H

#include <iostream>

template<class UpperLayer, class LowerLayer>
class ConvertDataType;

template<class LowerLayer>
class StringTestLayer : public LowerLayer
{

private:
    std::string* out;

public:
    typedef char DataType;
    void setString(std::string&);
    void receiveData(DataType);
    void end(DataType);
};

template<class LowerLayer>
void StringTestLayer<LowerLayer>::setString(std::string& str)
{
    out = &str;
}

template<class LowerLayer>
void StringTestLayer<LowerLayer>::receiveData(DataType data)
{
    out->operator+= (data);

    LowerLayer::receiveData(ConvertDataType<StringTestLayer<LowerLayer>, LowerLayer>::convert(data));
}

template<class LowerLayer>
void StringTestLayer<LowerLayer>::end(DataType n)
{
    LowerLayer::end(n);
}

#endif
