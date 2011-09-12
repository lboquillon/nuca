/*
remove_ns.h: Nucleotides Compression Algorithms
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

#ifndef REMOVE_NS
#define REMOVE_NS

#include <stack>
#include "fsm.h"

using namespace std;

template<class UpperLayer, class LowerLayer>
class ConvertDataType;

template<class LowerLayer>
class RemoveNs : public LowerLayer
{
private:

    class State
    {
    protected:
        RemoveNs<LowerLayer>* const fsm;
    public:
        State(RemoveNs<LowerLayer>* m) : fsm(m)
        {
        }
        virtual const State* stimulusNotN(char) const = 0;
        virtual const State* stimulusN() const = 0;
        virtual const State* stimulusEndSeq() const = 0;
        virtual ~State() { }
    };

    class StateNotN : public State
    {
    private:
        const State* stimulusNotN(char) const;
        const State* stimulusN() const;
        const State* stimulusEndSeq() const;
    public:
        StateNotN(RemoveNs<LowerLayer>* m) : State(m)
        {
        }
    };

    class StateN : public State
    {
    private:
        const State* stimulusNotN(char) const;
        const State* stimulusN() const;
        const State* stimulusEndSeq() const;
    public:
        StateN(RemoveNs<LowerLayer>* m) : State(m)
        {
        }
    };

    class StateReadingSequence : public State
    {
    private:
        const State* stimulusNotN(char) const;
        const State* stimulusN() const;
        const State* stimulusEndSeq() const;
    public:
        StateReadingSequence(RemoveNs<LowerLayer>* m) : State(m)
        {
        }
    };

    template<class T> friend class AddLayerTest;

    const State* const stateNotN;
    const State* const stateN;
    const State* const stateReadingSequence;
    const State* current;
    size_t ns;
    size_t stimuliOrder;
    std::stack<const State*> stiStack;
    char sizeNuc;
    static const size_t oneCharSize = 1;

    void addMissingNuc(size_t);
    void makeEscapeSequence();

public:
    typedef char DataType;

    RemoveNs()
        : stateNotN(new StateNotN(this)),
          stateN(new StateN(this)),
          stateReadingSequence(new StateReadingSequence(this)),
          current(stateNotN),
          stimuliOrder(0),
          sizeNuc(0)
    {
    }

    ~RemoveNs()
    {
        delete stateNotN;
        delete stateN;
        delete stateReadingSequence;
    }

    void receiveData(char);
    void receiveData(EndSeqStimulus);
    void flush(char);
    void end();
};

template<class LowerLayer>
inline void RemoveNs<LowerLayer>::addMissingNuc(size_t n)
{
    for (size_t i = 0; i < n; ++i)
        flush(RareSequence::rareSeq[i]);
}

template<class LowerLayer>
inline void RemoveNs<LowerLayer>::makeEscapeSequence()
{
    for (size_t i = 0; i < RareSequence::rareSeq.size(); ++i)
        flush(RareSequence::rareSeq[i]);

    char numb[5] = { '\0'};

    for (int i = 3; i >= 0; i--)
    {
        numb[i] = valueToNuc(ns & 3);
        ns >>= 2;
    }

    for (size_t i = 0; i < 4; ++i)
        flush(numb[i]);
}

template<class LowerLayer>
inline void RemoveNs<LowerLayer>::receiveData(DataType sti)
{
    if (current == NULL)
        throw "Invalid State";

    if (sti == 'N')
    {
        current = current->stimulusN();
    }
    else
    {
        current = current->stimulusNotN(sti);
    }

    while (!stiStack.empty())
    {
        current = (stiStack.top())->stimulusNotN(sti);
        stiStack.pop();
    }
}

template<class LowerLayer>
inline void RemoveNs<LowerLayer>::receiveData(EndSeqStimulus)
{
    current->stimulusEndSeq();
    end();
}

template<class LowerLayer>
inline void RemoveNs<LowerLayer>::end()
{
    LowerLayer::end(sizeNuc == 0 ? 4 : sizeNuc);
}

template<class LowerLayer>
inline void RemoveNs<LowerLayer>::flush(char c)
{
    // (n1 + n2) % k = ((n1 % k) + (n2 % k)) % k
    sizeNuc = (sizeNuc + (oneCharSize % 4)) % 4;
    LowerLayer::receiveData(ConvertDataType<RemoveNs<LowerLayer>, LowerLayer>::convert(c));
}

template<class LowerLayer>
inline const typename RemoveNs<LowerLayer>::State* RemoveNs<LowerLayer>::StateNotN::stimulusNotN(char c) const
{
    const State* state;

    if (c == RareSequence::rareSeq[0])
    {
        this->fsm->ns = 0;
        this->fsm->stimuliOrder = 1;
        state = this->fsm->stateReadingSequence;
    }
    else
    {
        this->fsm->flush(c);
        state = this;
    }

    return state;
}

template<class LowerLayer>
inline const typename RemoveNs<LowerLayer>::State* RemoveNs<LowerLayer>::StateNotN::stimulusN() const
{
    this->fsm->ns = 1;
    return this->fsm->stateN;
}

template<class LowerLayer>
inline const typename RemoveNs<LowerLayer>::State* RemoveNs<LowerLayer>::StateNotN::stimulusEndSeq() const
{
    return NULL;
}

template<class LowerLayer>
inline const typename RemoveNs<LowerLayer>::State* RemoveNs<LowerLayer>::StateN::stimulusNotN(char c) const
{
    const State* state;
    bool isNotSeqChar;

    if (c == RareSequence::rareSeq[0])
    {
        this->fsm->stiStack.push(this->fsm->stateNotN);
        isNotSeqChar = false;
    }
    else
    {
        isNotSeqChar = true;
    }

    this->fsm->makeEscapeSequence();
    this->fsm->ns = 0;
    state = this->fsm->stateNotN;

    if (isNotSeqChar)
        this->fsm->flush(c);

    return state;
}


template<class LowerLayer>
inline const typename RemoveNs<LowerLayer>::State* RemoveNs<LowerLayer>::StateN::stimulusN() const
{
    if (this->fsm->ns < 255)
    {
        this->fsm->ns++;
    }
    else
    {
        this->fsm->makeEscapeSequence();
        this->fsm->ns = 1;
    }

    return this;
}

template<class LowerLayer>
inline const typename RemoveNs<LowerLayer>::State* RemoveNs<LowerLayer>::StateN::stimulusEndSeq() const
{
    this->fsm->makeEscapeSequence();
    return NULL;
}

template<class LowerLayer>
inline const typename RemoveNs<LowerLayer>::State* RemoveNs<LowerLayer>::StateReadingSequence::stimulusNotN(char c) const
{
    const State* state;

    if (c == RareSequence::rareSeq[this->fsm->stimuliOrder])
    {
        if (this->fsm->stimuliOrder < RareSequence::rareSeq.size() - 1)
        {
            this->fsm->stimuliOrder++;
            state = this;
        }
        else
        {
            this->fsm->stimuliOrder = 0;
            this->fsm->makeEscapeSequence();
            state = this->fsm->stateNotN;
        }
    }
    else
    {
        this->fsm->addMissingNuc(this->fsm->stimuliOrder);

        if (c == RareSequence::rareSeq[0])
        {
            this->fsm->stimuliOrder = 0;
            this->fsm->stiStack.push(this->fsm->stateNotN);
        }
        else
        {
            this->fsm->flush(c);
            state = this->fsm->stateNotN;
        }
    }

    return state;
}

template<class LowerLayer>
inline const typename RemoveNs<LowerLayer>::State* RemoveNs<LowerLayer>::StateReadingSequence::stimulusN() const
{
    this->fsm->addMissingNuc(this->fsm->stimuliOrder);
    this->fsm->stimuliOrder = 0;
    this->fsm->ns = 1;
    return this->fsm->stateN;
}


template<class LowerLayer>
inline const typename RemoveNs<LowerLayer>::State* RemoveNs<LowerLayer>::StateReadingSequence::stimulusEndSeq() const
{
    this->fsm->addMissingNuc(this->fsm->stimuliOrder);
    this->fsm->stimuliOrder = 0;
    return NULL;
}

#endif
