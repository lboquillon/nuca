/*
decompressing_fsm.h: Nucleotides Compression Algorithms
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
#ifndef DECOMPRESSING_FSM_H
#define DECOMPRESSING_FSM_H

#include "fsm.h"

class DecompressingFSM : private Fsm
{
private:
    class State
    {
    protected:
        DecompressingFSM* const fsm;
    public:
        State(DecompressingFSM* dcfsm)
            : fsm(dcfsm)
        {
        }
        virtual ~State() { }
        virtual const State* stimulusNuc(char) const = 0;
        virtual const State* stimulusEndSeq() const = 0;
    };

    class StateInitial : public State
    {
    private:
        const State* stimulusNuc(char) const;
        const State* stimulusEndSeq() const;
    public:
        StateInitial(DecompressingFSM* dcfsm)
            : State(dcfsm)
        {
        }
    };

    class StateNuc : public State
    {
    private:
        const State* stimulusNuc(char) const;
        const State* stimulusEndSeq() const;
    public:
        StateNuc(DecompressingFSM* dcfsm)
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
        StateEscapeSequenceChar(DecompressingFSM* dcfsm)
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
        StateReadingNCount(DecompressingFSM* dcfsm)
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
        StateReadingEscapeSequence(DecompressingFSM* dcfsm)
            : State(dcfsm)
        {
        }
    };

    const State* const stateInitial;
    const State* const stateNuc;
    const State* const stateReadingEscapeChar;
    const State* const stateReadBit;
    const State* current;
    size_t genericCounter;
    size_t nCounter;
    size_t bc;

    void addNs()
    {
        if (0 == nCounter)
            outSeq += rareSeq;
        else
            outSeq += std::string(nCounter, 'N');
    }

public:
    DecompressingFSM(std::string& out)
        : Fsm(out),
          stateInitial(new StateInitial(this)),
          stateNuc(new StateNuc(this)),
          stateReadingEscapeChar(new StateReadingEscapeSequence(this)),
          stateReadBit(new StateReadingNCount(this)),
          current(stateInitial),
          genericCounter(0),
          nCounter(0)
    {
    }

    ~DecompressingFSM()
    {
        delete stateInitial;
        delete stateNuc;
        delete stateReadingEscapeChar;
        delete stateReadBit;
    }

    void stimulate(char sti)
    {
        if (current == NULL)
            throw "Invalid State";

        current = current->stimulusNuc(sti);
    }

    void stimulate(EndSeqStimulus)
    {
        current->stimulusEndSeq();
    }
};

inline const DecompressingFSM::State* DecompressingFSM::StateInitial::stimulusNuc(char n) const
{
    const State* state;

    if (n == fsm->rareSeq[0])
    {
        fsm->genericCounter = 1;
        state = fsm->stateReadingEscapeChar;
    }
    else
    {
        fsm->outSeq += n;
        state = fsm->stateNuc;
    }

    return state;
}

inline const DecompressingFSM::State* DecompressingFSM::StateInitial::stimulusEndSeq() const
{
    return NULL;
}

inline const DecompressingFSM::State* DecompressingFSM::StateNuc::stimulusNuc(char n) const
{
    const State* state;

    if (n == fsm->rareSeq[0])
    {
        fsm->genericCounter = 1;
        state = fsm->stateReadingEscapeChar;
    }
    else
    {
        fsm->outSeq += n;
        state = this;
    }

    return state;
}

inline const DecompressingFSM::State* DecompressingFSM::StateNuc::stimulusEndSeq() const
{
    return NULL;
}

inline const DecompressingFSM::State* DecompressingFSM::StateReadingEscapeSequence::stimulusNuc(char n) const
{
    const State* state;

    if (n == fsm->rareSeq[fsm->genericCounter])
    {
        if (fsm->genericCounter < fsm->rareSeq.size() - 1)
        {
            fsm->genericCounter++;
            state = this;
        }
        else
        {
            fsm->genericCounter = 1;
            state = fsm->stateReadBit;
        }
    }
    else
    {
        fsm->addMissingNuc(fsm->genericCounter);
        fsm->outSeq += n;
        fsm->genericCounter = 0;
        state = fsm->stateNuc;
    }

    return state;
}

inline const DecompressingFSM::State* DecompressingFSM::StateReadingEscapeSequence::stimulusEndSeq() const
{
    fsm->addMissingNuc(fsm->genericCounter);
    return NULL;
}

inline const DecompressingFSM::State* DecompressingFSM::StateReadingNCount::stimulusNuc(char n) const
{
    const State* state;
    char val;

    switch (n)
    {
        case 'A':
            val = 0;
            break;

        case 'T':
            val = 1;
            break;

        case 'C':
            val = 2;
            break;

        case 'G':
            val = 3;
            break;

        default:
            val = 0;
    }


    fsm->nCounter = (fsm->nCounter << 2) | val;

    if (fsm->genericCounter < 4)
    {
        ++fsm->genericCounter;
        state = this;
    }
    else
    {
        fsm->addNs();
        fsm->genericCounter = 0;
        fsm->nCounter = 0;
        state = fsm->stateInitial;
    }

    return state;
}
inline const DecompressingFSM::State* DecompressingFSM::StateReadingNCount::stimulusEndSeq() const
{
    return NULL;
}

#endif
