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

#ifndef REMOVE_NS_H
#define REMOVE_NS_H

#include <stack>
#include "fsm.h"
#include "nuc_mapper.h"

using namespace std;

template<class UpperLayer, class LowerLayer>
class ConvertDataType;

template<class LowerLayer, size_t nucsNumber = 4>
class RemoveNs : public LowerLayer
{
private:

    class State
    {
    protected:
        RemoveNs<LowerLayer, nucsNumber>* const fsm;
    public:
        State(RemoveNs<LowerLayer, nucsNumber>* m) : fsm(m)
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
        StateNotN(RemoveNs<LowerLayer, nucsNumber>* m) : State(m)
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
        StateN(RemoveNs<LowerLayer, nucsNumber>* m) : State(m)
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
        StateReadingSequence(RemoveNs<LowerLayer, nucsNumber>* m) : State(m)
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
    enum
    {
        oneCharSize = 1,
        bitPerNuc = 2
    };

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
    void receiveData(nuca::EndSeqStimulus);
    void flush(char);
    void end();
};

template<class LowerLayer, size_t nucsNumber>
inline void RemoveNs<LowerLayer, nucsNumber>::addMissingNuc(size_t n)
{
    for (size_t i = 0; i < n; ++i)
        flush(nuca::rareSeq[i]);
}

template<class LowerLayer, size_t nucsNumber>
inline void RemoveNs<LowerLayer, nucsNumber>::makeEscapeSequence()
{
    for (size_t i = 0; i < nuca::rareSeq.size(); ++i)
        flush(nuca::rareSeq[i]);

    char numb[nucsNumber] = { '\0'};

    for (int i = nucsNumber - 1; i >= 0; i--)
    {
        numb[i] = nuca::valueToNuc(ns & 3);
        ns >>= bitPerNuc;
    }

    for (size_t i = 0; i < nucsNumber; ++i)
        flush(numb[i]);
}

template<class LowerLayer, size_t nucsNumber>
inline void RemoveNs<LowerLayer, nucsNumber>::receiveData(DataType sti)
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

template<class LowerLayer, size_t nucsNumber>
inline void RemoveNs<LowerLayer, nucsNumber>::receiveData(nuca::EndSeqStimulus)
{
    current->stimulusEndSeq();
    end();
}

template<class LowerLayer, size_t nucsNumber>
inline void RemoveNs<LowerLayer, nucsNumber>::end()
{
    LowerLayer::end(sizeNuc == 0 ? 4 : sizeNuc);
}

template<class LowerLayer, size_t nucsNumber>
inline void RemoveNs<LowerLayer, nucsNumber>::flush(char c)
{
    // (n1 + n2) % k = ((n1 % k) + (n2 % k)) % k
    sizeNuc = (sizeNuc + (oneCharSize % nucsNumber)) % nucsNumber;
    LowerLayer::receiveData(ConvertDataType<RemoveNs<LowerLayer, nucsNumber>, LowerLayer>::convert(c));
}

template<class LowerLayer, size_t nucsNumber>
inline const typename RemoveNs<LowerLayer, nucsNumber>::State* RemoveNs<LowerLayer, nucsNumber>::StateNotN::stimulusNotN(char c) const
{
    const State* state;

    if (c == nuca::rareSeq[0])
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

template<class LowerLayer, size_t nucsNumber>
inline const typename RemoveNs<LowerLayer, nucsNumber>::State* RemoveNs<LowerLayer, nucsNumber>::StateNotN::stimulusN() const
{
    this->fsm->ns = 1;
    return this->fsm->stateN;
}

template<class LowerLayer, size_t nucsNumber>
inline const typename RemoveNs<LowerLayer, nucsNumber>::State* RemoveNs<LowerLayer, nucsNumber>::StateNotN::stimulusEndSeq() const
{
    return NULL;
}

template<class LowerLayer, size_t nucsNumber>
inline const typename RemoveNs<LowerLayer, nucsNumber>::State* RemoveNs<LowerLayer, nucsNumber>::StateN::stimulusNotN(char c) const
{
    const State* state;
    bool isNotSeqChar;

    if (c == nuca::rareSeq[0])
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


template<class LowerLayer, size_t nucsNumber>
inline const typename RemoveNs<LowerLayer, nucsNumber>::State* RemoveNs<LowerLayer, nucsNumber>::StateN::stimulusN() const
{
    if (this->fsm->ns < nuca::NucMapper<nucsNumber>::max)
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

template<class LowerLayer, size_t nucsNumber>
inline const typename RemoveNs<LowerLayer, nucsNumber>::State* RemoveNs<LowerLayer, nucsNumber>::StateN::stimulusEndSeq() const
{
    this->fsm->makeEscapeSequence();
    return NULL;
}

template<class LowerLayer, size_t nucsNumber>
inline const typename RemoveNs<LowerLayer, nucsNumber>::State* RemoveNs<LowerLayer, nucsNumber>::StateReadingSequence::stimulusNotN(char c) const
{
    const State* state;

    if (c == nuca::rareSeq[this->fsm->stimuliOrder])
    {
        if (this->fsm->stimuliOrder < nuca::rareSeq.size() - 1)
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

        if (c == nuca::rareSeq[0])
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

template<class LowerLayer, size_t nucsNumber>
inline const typename RemoveNs<LowerLayer, nucsNumber>::State* RemoveNs<LowerLayer, nucsNumber>::StateReadingSequence::stimulusN() const
{
    this->fsm->addMissingNuc(this->fsm->stimuliOrder);
    this->fsm->stimuliOrder = 0;
    this->fsm->ns = 1;
    return this->fsm->stateN;
}


template<class LowerLayer, size_t nucsNumber>
inline const typename RemoveNs<LowerLayer, nucsNumber>::State* RemoveNs<LowerLayer, nucsNumber>::StateReadingSequence::stimulusEndSeq() const
{
    this->fsm->addMissingNuc(this->fsm->stimuliOrder);
    this->fsm->stimuliOrder = 0;
    return NULL;
}

#endif
