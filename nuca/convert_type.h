/*
convert_type.h: Nucleotides Compression Algorithms
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

#ifndef CONVERT_TYPE_H
#define CONVERT_TYPE_H

#include "end_layer.h"
#include "ostream_saver.h"
#include "bit_handler.h"
#include "tests_layers.h"
#include "remove_ns.h"
#include "add_ns.h"
#include "istream_loader.h"

#include <biopp/bio_molecular/bio_molecular.h>

template<class UpperLayer, class LowerLayer>
class ConvertDataType;

template<class UpperLayer, class LowerLayer>
class ConvertDataType
{
public:
	static typename LowerLayer::DataType convert (typename UpperLayer::DataType data)
	{
			return typename LowerLayer::DataType (data);
	}
};

template<class UpperLayer, class LowerLayer>
class ConvertDataType<UpperLayer, CompressingBitHandler<LowerLayer> >
{
private:
    typedef CompressingBitHandler<LowerLayer> Lower;
public:
    static typename Lower::DataType convert(typename UpperLayer::DataType data)
    {
        return typename Lower::DataType(to_nuc(data));
    }
};

template<>
class ConvertDataType<StringTestLayer<EndLayer>, EndLayer>
{
public:
    static char convert(typename StringTestLayer<EndLayer>::DataType data)
    {
        return char(data);
    }
};

#endif
