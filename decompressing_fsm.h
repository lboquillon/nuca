#include <iostream>
#include <string>

#ifndef DECOMPRESSING_FSM
#define DECOMPRESSING_FSM

class DecompressingFSM
{
private:
    class State
    {
    protected:
        DecompressingFSM* const fsm;
    public:
        State(DecompressingFSM* dcfsm) : fsm(dcfsm) { }
        virtual ~State() { }

        virtual const State* stimulusNuc(char) const = 0;
        virtual const State* stimulusEscSeqChar() const = 0;
        virtual const State* stimulusEndSeq() const = 0;
    };

    class StateInitial : public State
    {
    private:
        const State* stimulusNuc(char) const;
        const State* stimulusEscSeqChar() const;
        const State* stimulusEndSeq() const;
    public:
        StateInitial(DecompressingFSM* dcfsm) : State(dcfsm) { }
    };

    class StateNuc : public State
    {
    private:
        const State* stimulusNuc(char) const;
        const State* stimulusEscSeqChar() const;
        const State* stimulusEndSeq() const;
    public:
        StateNuc(DecompressingFSM* dcfsm): State(dcfsm) { }
    };

    class StateEscapeSequenceChar : public State
    {
    private:
        const State* stimulusNuc(char) const;
        const State* stimulusEscSeqChar() const;
        const State* stimulusEndSeq() const;
    public:
        StateEscapeSequenceChar(DecompressingFSM* dcfsm) : State(dcfsm) { }
    };

    class StateReadingNCount : public State
    {
    private:
        const State* stimulusNuc(char) const;
        const State* stimulusEscSeqChar() const;
        const State* stimulusEndSeq() const;
    public:
        StateReadingNCount(DecompressingFSM* dcfsm) : State(dcfsm) { }
    };

    const State* stateInitial;
    const State* stateNuc;
    const State* stateEscapeSequenceChar;
    const State* stateReadBit;
    const State* current;
    std::string& outSeq;
    std::string rareSeq;
    size_t nc;
    size_t nCounter;
    size_t bc;

    void addMissingNuc()
    {
        outSeq += rareSeq.substr(0, nc);
    }

    void addNs()
    {
        if (0 == nCounter)
            outSeq += rareSeq;

        else
            outSeq += std::string(nCounter, 'N');
    }

public:
    DecompressingFSM(std::string& out)
        : stateInitial(new StateInitial(this))
        , stateNuc(new StateNuc(this))
        , stateEscapeSequenceChar(new StateEscapeSequenceChar(this))
        , stateReadBit(new StateReadingNCount(this))
        , current(stateInitial)
        , outSeq(out)
        , rareSeq("TTT")
        , nc(0)
        , nCounter(0)
        , bc(0)
    { }

    ~DecompressingFSM()
    {
        delete stateInitial;
        delete stateNuc;
        delete stateEscapeSequenceChar;
        delete stateReadBit;
    }

    void stimulate(char sti)
    {
        if (current == NULL)
            throw "Invalid State";

        if (sti == rareSeq[nc])
            current = current->stimulusEscSeqChar();
        else
        {
            switch (sti)
            {
                case 0:
                    current = current->stimulusEndSeq();
                    break;

                default:
                    current = current->stimulusNuc(sti);
            }
        }
    }
};

const DecompressingFSM::State* DecompressingFSM::StateInitial::stimulusNuc(char n) const
{
    fsm->outSeq += n;
    return fsm->stateNuc;
}

const DecompressingFSM::State* DecompressingFSM::StateInitial::stimulusEscSeqChar() const
{
    fsm->nc = 1;
    return fsm->stateEscapeSequenceChar;
}

const DecompressingFSM::State* DecompressingFSM::StateInitial::stimulusEndSeq() const
{
    return NULL;
}

const DecompressingFSM::State* DecompressingFSM::StateNuc::stimulusNuc(char n) const
{
    fsm->outSeq += n;
    return this;
}

const DecompressingFSM::State* DecompressingFSM::StateNuc::stimulusEscSeqChar() const
{
    fsm->nc = 1;
    return fsm->stateEscapeSequenceChar;
}

const DecompressingFSM::State* DecompressingFSM::StateNuc::stimulusEndSeq() const
{
    return NULL;
}

const DecompressingFSM::State* DecompressingFSM::StateEscapeSequenceChar::stimulusNuc(char n) const
{
    fsm->addMissingNuc();
    fsm->outSeq += n;
    fsm->nc = 0;
    return fsm->stateNuc;
}

const DecompressingFSM::State* DecompressingFSM::StateEscapeSequenceChar::stimulusEscSeqChar() const
{
    const State* state;

    if (fsm->nc < fsm->rareSeq.size() - 1)
    {
        fsm->nc++;
        state = this;
    }

    else
    {
        fsm->bc = 1;
        state = fsm->stateReadBit;
    }

    return state;

}

const DecompressingFSM::State* DecompressingFSM::StateEscapeSequenceChar::stimulusEndSeq() const
{
    fsm->addMissingNuc();
    return NULL;
}

const DecompressingFSM::State* DecompressingFSM::StateReadingNCount::stimulusNuc(char n) const
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

    if (fsm->bc < 4)
    {
        ++fsm->bc;
        state = this;
    }

    else
    {
        fsm->addNs();
        fsm->bc = 0;
        fsm->nCounter = 0;
        state = fsm->stateInitial;
    }

    return state;
}

const DecompressingFSM::State* DecompressingFSM::StateReadingNCount::stimulusEscSeqChar() const
{
    return NULL;
}

const DecompressingFSM::State* DecompressingFSM::StateReadingNCount::stimulusEndSeq() const
{
    return NULL;
}

#endif
