#ifndef CONVERT_TYPE
#define CONVERT_TYPE

#include "end_layer.h"
#include "saver.h"
#include "bit_handle.h"
#include "tests_layers.h"
#include "remove_ns.h"
#include <biopp/bio_molecular/bio_molecular.h>

template<class UpperLayer, class LowerLayer>
class ConvertDataType;

template<>
class ConvertDataType<StringTestLayer<CompressingBitHandle<Saver<EndLayer> > >, CompressingBitHandle<Saver<EndLayer> > >
{
private:
    typedef CompressingBitHandle<Saver<EndLayer> > Lower;
public:
    static typename Lower::DataType convert(typename CompressingBitHandle<Lower>::DataType data)
    {
        return typename Lower::DataType(to_nuc(data));
    }
};

template<>        // RemoveNs<StringTestLayer<CompressingBitHandle<Saver<EndLayer> > > > b;
class ConvertDataType<RemoveNs<StringTestLayer<CompressingBitHandle<Saver<EndLayer> > > >, StringTestLayer<CompressingBitHandle<Saver<EndLayer> > > >
{
private:
    typedef StringTestLayer<CompressingBitHandle<Saver<EndLayer> > > Lower;
public:
    static typename Lower::DataType convert(typename CompressingBitHandle<Lower>::DataType data)
    {
        return typename Lower::DataType(data);
    }
};

template<>
class ConvertDataType<RemoveNs<CompressingBitHandle<Saver<EndLayer> > >, CompressingBitHandle< Saver<EndLayer> > >
{
private:
    typedef CompressingBitHandle< Saver<EndLayer> > Lower;

public:
    static typename Lower::DataType convert(typename RemoveNs<Lower>::DataType data)
    {
        return typename Lower::DataType(to_nuc(data));
    }
};

template<>
class ConvertDataType<CompressingBitHandle<Saver<EndLayer> >, Saver<EndLayer> >
{
private:
    typedef Saver<EndLayer> Lower;
public:
    static typename Lower::DataType convert(typename CompressingBitHandle<Lower>::DataType data)
    {
        return typename Lower::DataType(data);
    }
};

#endif
