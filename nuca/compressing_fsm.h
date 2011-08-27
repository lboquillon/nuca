/*
compressing_fsm.h: Nucleotides Compression Algorithms
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

#ifndef COMPRESSING_FSM_H
#define COMPRESSING_FSM_H

#include "fsm.h"
#include <stack>

class CompressingFSM : private Fsm
{
private:

    class State
    {
    protected:
        CompressingFSM* const fsm;
    public:
        State(CompressingFSM* m) : fsm(m)
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
        StateNotN(CompressingFSM* m) : State(m)
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
        StateN(CompressingFSM* m) : State(m)
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
        StateReadingSequence(CompressingFSM* m) : State(m)
        {
        }
    };

    const State* const stateNotN;
    const State* const stateN;
    const State* const stateReadingSequence;
    const State* current;
    size_t ns;
    size_t stimuliOrder;
    std::stack<const State*> stiStack;

    void makeEscapeSequence()
    {
        char numb[5] = { '\0'};

        for (int i = 3; i >= 0; i--)
        {
            numb[i] = valueToNuc(ns & 3);
            ns >>= 2;
        }

        outSeq += rareSeq + std::string(numb);
    }

public:
    CompressingFSM(std::string& out)
        : Fsm(out),
          stateNotN(new StateNotN(this)),
          stateN(new StateN(this)),
          stateReadingSequence(new StateReadingSequence(this)),
          current(stateNotN),
          stimuliOrder(0)
    {
    }

    ~CompressingFSM()
    {
        delete stateNotN;
        delete stateN;
        delete stateReadingSequence;
    }

    void stimulate(char sti)
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

    void stimulate(EndSeqStimulus)
    {
        current->stimulusEndSeq();
    }

};

inline const CompressingFSM::State* CompressingFSM::StateNotN::stimulusNotN(char c) const
{
    const State* state;

    if (c == fsm->rareSeq[0])
    {
        fsm->ns = 0;
        fsm->stimuliOrder = 1;
        state = fsm->stateReadingSequence;
    }
    else
    {
        fsm->outSeq += c;
        state = this;
    }

    return state;
}

inline const CompressingFSM::State* CompressingFSM::StateNotN::stimulusN() const
{
    fsm->ns = 1;
    return fsm->stateN;
}

inline const CompressingFSM::State* CompressingFSM::StateNotN::stimulusEndSeq() const
{
    return NULL;
}

inline const CompressingFSM::State* CompressingFSM::StateN::stimulusNotN(char c) const
{
    const State* state;
    bool b;

    if (c == fsm->rareSeq[0])
    {
        fsm->stiStack.push(fsm->stateNotN);
        b = false;
    }
    else
    {
        b = true;
    }

    fsm->makeEscapeSequence();
    fsm->ns = 0;
    state = fsm->stateNotN;

    if (true == b)
        fsm->outSeq += c;

    return state;
}


inline const CompressingFSM::State* CompressingFSM::StateN::stimulusN() const
{
    if (fsm->ns < 255)
    {
        fsm->ns++;
    }
    else
    {
        fsm->makeEscapeSequence();
        fsm->ns = 1;
    }

    return this;
}

inline const CompressingFSM::State* CompressingFSM::StateN::stimulusEndSeq() const
{
    fsm->makeEscapeSequence();
    return NULL;
}

inline const CompressingFSM::State* CompressingFSM::StateReadingSequence::stimulusNotN(char c) const
{
    const State* state;

    if (c == fsm->rareSeq[fsm->stimuliOrder])
    {
        if (fsm->stimuliOrder < fsm->rareSeq.size() - 1)
        {
            fsm->stimuliOrder++;
            state = this;
        }
        else
        {
            fsm->stimuliOrder = 0;
            fsm->makeEscapeSequence();
            state = fsm->stateNotN;
        }
    }
    else
    {
        fsm->addMissingNuc(fsm->stimuliOrder);

        if (c == fsm->rareSeq[0])
        {
            fsm->stimuliOrder = 0;
            fsm->stiStack.push(fsm->stateNotN);
        }
        else
        {
            fsm->outSeq += c;
            state = fsm->stateNotN;
        }
    }

    return state;
}

inline const CompressingFSM::State* CompressingFSM::StateReadingSequence::stimulusN() const
{
    fsm->addMissingNuc(fsm->stimuliOrder);
    fsm->stimuliOrder = 0;
    fsm->ns = 1;
    return fsm->stateN;
}


inline const CompressingFSM::State* CompressingFSM::StateReadingSequence::stimulusEndSeq() const
{
    fsm->addMissingNuc(fsm->stimuliOrder);
    fsm->stimuliOrder = 0;
    return NULL;
}

#endif
