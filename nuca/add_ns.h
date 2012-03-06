/*
add_ns.h: Nucleotides Compression Algorithms
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

#ifndef ADD_NS_H
#define ADD_NS_H

#include <stack>
#include "fsm.h"
#include "nuc_mapper.h"

#include <biopp/biopp.h>

template<class UpperLayer, class LowerLayer>
class ConvertDataType;

template<class LowerLayer, size_t nucsNumber = 4>
class AddNs : public LowerLayer
{
private:
    class State
    {
    protected:
        AddNs* const fsm;
    public:
        State(AddNs* dcfsm)
            : fsm(dcfsm)
        {
        }
        virtual ~State()
        {
        }
        virtual const State* stimulusNuc(char) const = 0;
        virtual const State* stimulusEndSeq() const = 0;
    };

    class StateNuc : public State
    {
    private:
        const State* stimulusNuc(char) const;
        const State* stimulusEndSeq() const;
    public:
        StateNuc(AddNs* dcfsm)
            : State(dcfsm)
        {
        }
    };

    class StateEscapeSequenceChar : public State
    {
    private:
        const State* stimulusNuc(char) const;
        const State* stimulusEndSeq() const;
    public:
        StateEscapeSequenceChar(AddNs* dcfsm)
            : State(dcfsm)
        {
        }
    };

    class StateReadingNCount : public State
    {
    private:
        const State* stimulusNuc(char) const;
        const State* stimulusEndSeq() const;
    public:
        StateReadingNCount(AddNs* dcfsm)
            : State(dcfsm)
        {
        }
    };

    class StateReadingEscapeSequence : public State
    {
    private:
        const State* stimulusNuc(char) const;
        const State* stimulusEndSeq() const;
    public:
        StateReadingEscapeSequence(AddNs* dcfsm)
            : State(dcfsm)
        {
        }
    };

    const State* const stateNuc;
    const State* const stateReadingEscapeChar;
    const State* const stateReadBit;
    const State* current;
    size_t genericCounter;
    enum
    {
        bitPerNuc = 2
    };
    typename nuca::NucMapper<nucsNumber>::NucSizeType nCounter;
    std::stack<const State*> stiStack;

    void addNs();
    void addMissingNuc(size_t);
    void flush(char);

public:
    AddNs()
        : stateNuc(new StateNuc(this)),
          stateReadingEscapeChar(new StateReadingEscapeSequence(this)),
          stateReadBit(new StateReadingNCount(this)),
          current(stateNuc),
          genericCounter(0),
          nCounter(0)
    {
    }

    ~AddNs()
    {
        delete stateNuc;
        delete stateReadingEscapeChar;
        delete stateReadBit;
    }

    void receiveData(char);
    void receiveData(nuca::EndSeqStimulus);
    void end(char);
};

template<class LowerLayer, size_t nucsNumber>
inline void AddNs<LowerLayer, nucsNumber>::addNs()
{
    if (0 == nCounter)
    {
        for (size_t i = 0; i < nuca::rareSeq.size(); ++i)
            flush(nuca::rareSeq[i]);
    }
    else
    {
        for (size_t i = 0; i < nCounter; ++i)
            flush('N');
    }
}

template<class LowerLayer, size_t nucsNumber>
inline void AddNs<LowerLayer, nucsNumber>::flush(char c)
{
    LowerLayer::receiveData(ConvertDataType<AddNs<LowerLayer, nucsNumber>, LowerLayer>::convert(c));
}

template<class LowerLayer, size_t nucsNumber>
inline void AddNs<LowerLayer, nucsNumber>::addMissingNuc(size_t n)
{
    for (size_t i = 0; i < n; ++i)
        flush(nuca::rareSeq[i]);
}

template<class LowerLayer, size_t nucsNumber>
inline void AddNs<LowerLayer, nucsNumber>::receiveData(char sti)
{
    if (current == NULL)
        throw "Invalid State";

    current = current->stimulusNuc(sti);

    while (!stiStack.empty())
    {
        current = (stiStack.top())->stimulusNuc(sti);
        stiStack.pop();
    }
}

template<class LowerLayer, size_t nucsNumber>
inline void AddNs<LowerLayer, nucsNumber>::end(char)
{
    current->stimulusEndSeq();

}

template<class LowerLayer, size_t nucsNumber>
inline void AddNs<LowerLayer, nucsNumber>::receiveData(nuca::EndSeqStimulus)
{
    current->stimulusEndSeq();

}

template<class LowerLayer, size_t nucsNumber>
inline const typename AddNs<LowerLayer, nucsNumber>::State* AddNs<LowerLayer, nucsNumber>::StateNuc::stimulusNuc(char n) const
{
    const State* state;

    if (n == nuca::rareSeq[0])
    {
        this->fsm->genericCounter = 1;
        state = this->fsm->stateReadingEscapeChar;
    }
    else
    {
        this->fsm->flush(n);
        state = this;
    }

    return state;
}

template<class LowerLayer, size_t nucsNumber>
inline const typename AddNs<LowerLayer, nucsNumber>::State* AddNs<LowerLayer, nucsNumber>::StateNuc::stimulusEndSeq() const
{
    return NULL;
}

template<class LowerLayer, size_t nucsNumber>
inline const typename AddNs<LowerLayer, nucsNumber>::State* AddNs<LowerLayer, nucsNumber>::StateReadingEscapeSequence::stimulusNuc(char n) const
{
    const State* state;

    if (n == nuca::rareSeq[this->fsm->genericCounter])
    {
        if (this->fsm->genericCounter < nuca::rareSeq.size() - 1)
        {
            this->fsm->genericCounter++;
            state = this;
        }
        else
        {
            this->fsm->genericCounter = 1;
            state = this->fsm->stateReadBit;
        }
    }
    else
    {
        this->fsm->addMissingNuc(this->fsm->genericCounter);

        if (n == nuca::rareSeq[0])
        {
            this->fsm->stiStack.push(this->fsm->stateNuc);
        }

        else
        {
            this->fsm->flush(n);
        }

        this->fsm->genericCounter = 0;
        state = this->fsm->stateNuc;
    }

    return state;
}

template<class LowerLayer, size_t nucsNumber>
inline const typename AddNs<LowerLayer, nucsNumber>::State* AddNs<LowerLayer, nucsNumber>::StateReadingEscapeSequence::stimulusEndSeq() const
{
    this->fsm->addMissingNuc(this->fsm->genericCounter);
    return NULL;
}

template<class LowerLayer, size_t nucsNumber>
inline const typename AddNs<LowerLayer, nucsNumber>::State* AddNs<LowerLayer, nucsNumber>::StateReadingNCount::stimulusNuc(char n) const
{
    const State* state;

    this->fsm->nCounter = (this->fsm->nCounter << bitPerNuc) | biopp::to_nuc(n);

    if (this->fsm->genericCounter < nucsNumber)
    {
        ++this->fsm->genericCounter;
        state = this;
    }
    else
    {
        this->fsm->addNs();
        this->fsm->genericCounter = 0;
        this->fsm->nCounter = 0;
        state = this->fsm->stateNuc;
    }

    return state;
}

template<class LowerLayer, size_t nucsNumber>
inline const typename AddNs<LowerLayer, nucsNumber>::State* AddNs<LowerLayer, nucsNumber>::StateReadingNCount::stimulusEndSeq() const
{
    return NULL;
}

#endif
