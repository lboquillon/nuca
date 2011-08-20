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

#ifndef COMPRESSING_FSM
#define COMPRESSING_FSM

#include "fsm.h"

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
        virtual const State* stimulusRareSeqChar() const = 0;
        virtual ~State() { }
    };

    class StateInitial : public State
    {
    private:
        const State* stimulusNotN(char) const;
        const State* stimulusN() const;
        const State* stimulusEndSeq() const;
        const State* stimulusRareSeqChar() const;
    public:
        StateInitial(CompressingFSM* m) : State(m)
        { }
    };

    class StateNotN : public State
    {
    private:
        const State* stimulusNotN(char) const;
        const State* stimulusN() const;
        const State* stimulusEndSeq() const;
        const State* stimulusRareSeqChar() const;
    public:
        StateNotN(CompressingFSM* m) : State(m)
        { }
    };

    class StateN : public State
    {
    private:
        const State* stimulusNotN(char) const;
        const State* stimulusN() const;
        const State* stimulusEndSeq() const;
        const State* stimulusRareSeqChar() const;
    public:
        StateN(CompressingFSM* m) : State(m)
        { }
    };

    class StateRareSequenceChar : public State
    {
    private:
        const State* stimulusNotN(char) const;
        const State* stimulusN() const;
        const State* stimulusEndSeq() const;
        const State* stimulusRareSeqChar() const;
    public:
        StateRareSequenceChar(CompressingFSM* m) : State(m)
        { }
    };


    const State* const stateInitial;
    const State* const stateNotN;
    const State* const stateN;
    const State* const stateRareSequenceChar;
    const State* current;
    size_t ns;
    size_t stimuliOrder;

    void makeEscapeSequence()
    {
        char numb[5] = { '\0'};
        char nuc[4] = { 'A', 'T', 'C', 'G' };

        for (int i = 3; i >= 0; i--)
        {
            numb[i] = nuc[ns & 3];
            ns >>= 2;
        }

        outSeq += rareSeq + std::string(numb);
    }

public:
    CompressingFSM(std::string& out)
        : Fsm(out)
        , stateInitial(new StateInitial(this))
        , stateNotN(new StateNotN(this))
        , stateN(new StateN(this))
        , stateRareSequenceChar(new StateRareSequenceChar(this))
        , current(stateInitial)
        , stimuliOrder(0)
    {
    }

    ~CompressingFSM()
    {
        delete stateInitial;
        delete stateNotN;
        delete stateN;
        delete stateRareSequenceChar;
    }

    void stimulate(char sti)
    {
        if (current == NULL)
            throw "Invalid State";

        if (sti == rareSeq[stimuliOrder])
            current = current->stimulusRareSeqChar();
        else
        {
            switch (sti)
            {
                case 'N':
                    current = current->stimulusN();
                    break;
                default:
                    current = current->stimulusNotN(sti);
            }
        }
    }

    void stimulate(EndSeqStimulus sti)
    {
        if (sti == EndSeq)
            current->stimulusEndSeq();
    }

};

const CompressingFSM::State* CompressingFSM::StateInitial::stimulusNotN(char c) const
{
    fsm->outSeq += c;
    return fsm->stateNotN;
}

const CompressingFSM::State* CompressingFSM::StateInitial::stimulusN() const
{
    fsm->ns = 1;
    return fsm->stateN;
}


const CompressingFSM::State* CompressingFSM::StateInitial::stimulusEndSeq() const
{
    return NULL;
}

const CompressingFSM::State* CompressingFSM::StateInitial::stimulusRareSeqChar() const
{
    fsm->stimuliOrder = 1;
    fsm->ns = 0;
    return fsm->stateRareSequenceChar;
}

const CompressingFSM::State* CompressingFSM::StateNotN::stimulusNotN(char c) const
{
    fsm->outSeq += c;
    return this;
}

const CompressingFSM::State* CompressingFSM::StateNotN::stimulusN() const
{
    fsm->ns = 1;
    return fsm->stateN;
}

const CompressingFSM::State* CompressingFSM::StateNotN::stimulusEndSeq() const
{
    return NULL;
}

const CompressingFSM::State* CompressingFSM::StateNotN::stimulusRareSeqChar() const
{
    fsm->stimuliOrder = 1;
    fsm->ns = 0;
    return fsm->stateRareSequenceChar;

}

const CompressingFSM::State* CompressingFSM::StateN::stimulusNotN(char c) const
{
    fsm->makeEscapeSequence();
    fsm->ns = 0;
    fsm->outSeq += c;
    return fsm->stateNotN;
}


const CompressingFSM::State* CompressingFSM::StateN::stimulusN() const
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

const CompressingFSM::State* CompressingFSM::StateN::stimulusEndSeq() const
{
    fsm->makeEscapeSequence();
    return NULL;
}

const CompressingFSM::State* CompressingFSM::StateN::stimulusRareSeqChar() const
{
    fsm->stimuliOrder = 1;
    fsm->makeEscapeSequence();
    fsm->ns = 0;
    return fsm->stateRareSequenceChar;
}

const CompressingFSM::State* CompressingFSM::StateRareSequenceChar::stimulusNotN(char c) const
{
    fsm->addMissingNuc(fsm->stimuliOrder);
    fsm->stimuliOrder = 0;
    fsm->outSeq += c;
    return fsm->stateNotN;
}

const CompressingFSM::State* CompressingFSM::StateRareSequenceChar::stimulusN() const
{
    fsm->addMissingNuc(fsm->stimuliOrder);
    fsm->stimuliOrder = 0;
    fsm->ns = 1;
    return fsm->stateN;
}


const CompressingFSM::State* CompressingFSM::StateRareSequenceChar::stimulusEndSeq() const
{
    fsm->addMissingNuc(fsm->stimuliOrder);
    fsm->stimuliOrder = 0;
    return NULL;
}

const CompressingFSM::State* CompressingFSM::StateRareSequenceChar::stimulusRareSeqChar() const
{
    const State* state;

    if (fsm->stimuliOrder < fsm->rareSeq.size() - 1)
    {
        fsm->stimuliOrder++;
        state = this;
    }

    else
    {
        fsm->stimuliOrder = 0;
        fsm->makeEscapeSequence();
        state = fsm->stateInitial;
    }

    return state;
}

#endif
