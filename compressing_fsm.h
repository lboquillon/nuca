/*
compressing_fsm.h: Nucleotides Compression Algorithms
    Copyright (C) 2011 Daniel Gutson and Leonardo Boquillon, FuDePAN

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

#ifndef _COMPRESSING_FSM_INCLUDE_
#define _COMPRESSING_FSM_INCLUDE_

#include <string>

class CompressingFSM
{
private:

    class State
    {
    protected:
        CompressingFSM* const fsm;
    public:
        State(CompressingFSM* m) : fsm(m) {}
        virtual State* stimulaNotN(char) const = 0;
        virtual State* stimulaN() const = 0;
        virtual State* stimulaEndSeq() const = 0;
        virtual State* stimulaRareSeqChar() const = 0;
        virtual ~State() { }
    };

    class StateInitial : public State
    {
    private:
        State* stimulaNotN(char) const;
        State* stimulaN() const;
        State* stimulaEndSeq() const;
        State* stimulaRareSeqChar() const;
    public:
        StateInitial(CompressingFSM* m) : State(m)
        { }
        ~StateInitial() { }
    };

    class StateNotN : public State
    {
    private:
        State* stimulaNotN(char) const;
        State* stimulaN() const;
        State* stimulaEndSeq() const;
        State* stimulaRareSeqChar() const;
    public:
        StateNotN(CompressingFSM* m) : State(m)
        { }
        ~StateNotN() { }
    };

    class StateN : public State
    {
    private:
        State* stimulaNotN(char) const;
        State* stimulaN() const;
        State* stimulaEndSeq() const;
        State* stimulaRareSeqChar() const;
    public:
        StateN(CompressingFSM* m) : State(m)
        { }
        ~StateN() { }
    };

    class StateRareSequenceChar : public State
    {
    private:
        State* stimulaNotN(char) const;
        State* stimulaN() const;
        State* stimulaEndSeq() const;
        State* stimulaRareSeqChar() const;
    public:
        StateRareSequenceChar(CompressingFSM* m) : State(m)
        { }
        ~StateRareSequenceChar() { }
    };


    State* stateInitial;
    State* stateNotN;
    State* stateN;
    State* stateRareSequenceChar;
    State* current;
    std::string& outSeq;
    std::string rareSeq;
    size_t ns;
    size_t stimuliOrder;

    void addMissingNuc()
    {
        outSeq += std::string(rareSeq.begin(), rareSeq.begin() + stimuliOrder);
        stimuliOrder = 0;
    }

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
        : stateInitial(new StateInitial(this))
        , stateNotN(new StateNotN(this))
        , stateN(new StateN(this))
        , stateRareSequenceChar(new StateRareSequenceChar(this))
        , current(stateInitial)
        , outSeq(out)
        , rareSeq("TTT")
        , stimuliOrder(0)
    {
        outSeq = "";
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
        if (sti == rareSeq[stimuliOrder])
            current = current->stimulaRareSeqChar();
        else
        {
            switch (sti)
            {
                case 'N':
                    current = current->stimulaN();
                    break;
                case 0:
                    current = current->stimulaEndSeq();
                    break;
                default:
                    current = current->stimulaNotN(sti);
            }
        }
    }

    std::string getEscapeSequence() const
    {
        return outSeq;
    }
};

CompressingFSM::State* CompressingFSM::StateInitial::stimulaNotN(char c) const
{
    fsm->outSeq += c;
    return fsm->stateNotN;
}

CompressingFSM::State* CompressingFSM::StateInitial::stimulaN() const
{
    fsm->ns = 1;
    return fsm->stateN;
}


CompressingFSM::State* CompressingFSM::StateInitial::stimulaEndSeq() const
{
    return NULL;
}

CompressingFSM::State* CompressingFSM::StateInitial::stimulaRareSeqChar() const
{
    fsm->stimuliOrder = 1;
    fsm->ns = 0;
    return fsm->stateRareSequenceChar;
}

CompressingFSM::State* CompressingFSM::StateNotN::stimulaNotN(char c) const
{
    fsm->outSeq += c;
    return fsm->stateNotN;
}

CompressingFSM::State* CompressingFSM::StateNotN::stimulaN() const
{
    fsm->ns = 1;
    return fsm->stateN;
}

CompressingFSM::State* CompressingFSM::StateNotN::stimulaEndSeq() const
{
    return NULL;
}

CompressingFSM::State* CompressingFSM::StateNotN::stimulaRareSeqChar() const
{
    fsm->stimuliOrder = 1;
    fsm->ns = 0;
    return fsm->stateRareSequenceChar;

}

CompressingFSM::State* CompressingFSM::StateN::stimulaNotN(char c) const
{
    fsm->makeEscapeSequence();
    fsm->ns = 0;
    fsm->outSeq += c;
    return fsm->stateNotN;
}


CompressingFSM::State* CompressingFSM::StateN::stimulaN() const
{
    if (fsm->ns < 255)
        fsm->ns++;

    else
    {
        fsm->makeEscapeSequence();
        fsm->ns = 1;
    }

    return fsm->stateN;
}

CompressingFSM::State* CompressingFSM::StateN::stimulaEndSeq() const
{
    fsm->makeEscapeSequence();
    return NULL;
}

CompressingFSM::State* CompressingFSM::StateN::stimulaRareSeqChar() const
{
    fsm->stimuliOrder = 1;
    fsm->makeEscapeSequence();
    fsm->ns = 0;
    return fsm->stateRareSequenceChar;
}

CompressingFSM::State* CompressingFSM::StateRareSequenceChar::stimulaNotN(char c) const
{
    fsm->addMissingNuc();
    fsm->outSeq += c;
    return fsm->stateNotN;
}

CompressingFSM::State* CompressingFSM::StateRareSequenceChar::stimulaN() const
{
    fsm->addMissingNuc();
    fsm->ns = 1;
    return fsm->stateN;
}


CompressingFSM::State* CompressingFSM::StateRareSequenceChar::stimulaEndSeq() const
{
    fsm->addMissingNuc();
    return NULL;
}

CompressingFSM::State* CompressingFSM::StateRareSequenceChar::stimulaRareSeqChar() const
{
    State* state;

    if (fsm->stimuliOrder < fsm->rareSeq.size() - 1)
    {
        fsm->stimuliOrder++;
        state = fsm->stateRareSequenceChar;
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
