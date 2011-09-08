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

#ifndef CONVERT_TYPE
#define CONVERT_TYPE

#include "end_layer.h"
#include "ostream_saver.h"
#include "bit_handle.h"
#include "tests_layers.h"
#include "remove_ns.h"
#include <biopp/bio_molecular/bio_molecular.h>

template<class UpperLayer, class LowerLayer>
class ConvertDataType;

template<>
class ConvertDataType<StringTestLayer<CompressingBitHandle<OstreamSaver<EndLayer> > >, CompressingBitHandle<OstreamSaver<EndLayer> > >
{
private:
    typedef CompressingBitHandle<OstreamSaver<EndLayer> > Lower;
public:
    static typename Lower::DataType convert(typename CompressingBitHandle<Lower>::DataType data)
    {
        return typename Lower::DataType(to_nuc(data));
    }
};

template<>
class ConvertDataType<RemoveNs<StringTestLayer<CompressingBitHandle<OstreamSaver<EndLayer> > > >, StringTestLayer<CompressingBitHandle<OstreamSaver<EndLayer> > > >
{
private:
    typedef StringTestLayer<CompressingBitHandle<OstreamSaver<EndLayer> > > Lower;
public:
    static typename Lower::DataType convert(typename CompressingBitHandle<Lower>::DataType data)
    {
        return typename Lower::DataType(data);
    }
};

template<>
class ConvertDataType<RemoveNs<CompressingBitHandle<OstreamSaver<EndLayer> > >, CompressingBitHandle< OstreamSaver<EndLayer> > >
{
private:
    typedef CompressingBitHandle< OstreamSaver<EndLayer> > Lower;

public:
    static typename Lower::DataType convert(typename RemoveNs<Lower>::DataType data)
    {
        return typename Lower::DataType(to_nuc(data));
    }
};

template<>
class ConvertDataType<CompressingBitHandle<OstreamSaver<EndLayer> >, OstreamSaver<EndLayer> >
{
private:
    typedef OstreamSaver<EndLayer> Lower;
public:
    static typename Lower::DataType convert(typename CompressingBitHandle<Lower>::DataType data)
    {
        return typename Lower::DataType(data);
    }
};

#endif
