/*
statistics_layer.h: Nucleotides Compression Algorithms
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

#ifndef STATISTICS_LAYER_H
#define STATISTICS_LAYER_H

#include <stdint.h>

template<class LowerLayer>
class CompressionEventsCounterLayer : public LowerLayer
{
public:
    typedef char DataType;
    typedef uint64_t CounterType;

private:

    CounterType nsNumber;
    CounterType escapeSequenceNumber;
    CounterType escapeSequenceZero;
    CounterType inputNumber;
    CounterType outputNumber;

protected:
    void countEscapeSequence(uint32_t);
    void countOutput();
    void countInput();

public:

    CompressionEventsCounterLayer()
        : nsNumber(0),
          escapeSequenceNumber(0),
          escapeSequenceZero(0),
          inputNumber(0),
          outputNumber(0)
    { }

    void end(DataType);
    CounterType getNsNumber() const;
    CounterType getTotalEscapeSequence() const;
    CounterType getEscapeSequenceZero() const;
    CounterType getInputNucNumber() const;
    CounterType getOutputNucNumber() const;

};

template<class LowerLayer>
inline void CompressionEventsCounterLayer<LowerLayer>::countInput()
{
    ++inputNumber;
}

template<class LowerLayer>
inline void CompressionEventsCounterLayer<LowerLayer>::countEscapeSequence(uint32_t n)
{
    if (0 == n)
        ++escapeSequenceZero;

    nsNumber += n;
    ++escapeSequenceNumber;
}

template<class LowerLayer>
inline void CompressionEventsCounterLayer<LowerLayer>::countOutput()
{
    ++outputNumber;
}

template<class LowerLayer>
inline void CompressionEventsCounterLayer<LowerLayer>::end(DataType size)
{
    LowerLayer::end(size);
}

template<class LowerLayer>
inline typename CompressionEventsCounterLayer<LowerLayer>::CounterType CompressionEventsCounterLayer<LowerLayer>::getNsNumber() const
{
    return nsNumber;
}

template<class LowerLayer>
inline typename CompressionEventsCounterLayer<LowerLayer>::CounterType CompressionEventsCounterLayer<LowerLayer>::getTotalEscapeSequence() const
{
    return escapeSequenceNumber;
}

template<class LowerLayer>
inline typename CompressionEventsCounterLayer<LowerLayer>::CounterType CompressionEventsCounterLayer<LowerLayer>::getEscapeSequenceZero() const
{
    return escapeSequenceZero;
}

template<class LowerLayer>
inline typename CompressionEventsCounterLayer<LowerLayer>::CounterType CompressionEventsCounterLayer<LowerLayer>::getInputNucNumber() const
{
    return inputNumber;
}

template<class LowerLayer>
inline typename CompressionEventsCounterLayer<LowerLayer>::CounterType CompressionEventsCounterLayer<LowerLayer>::getOutputNucNumber() const
{
    return outputNumber;
}

#endif
