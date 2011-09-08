#ifndef TEST_LAYERS
#define TEST_LAYERS

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
