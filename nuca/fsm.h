#ifndef FSM_H
#define FSM_H

#include <string>

enum EndSeqStimulus
{
    EndSeq
};

class Fsm
{
protected:
    std::string rareSeq;
    std::string& outSeq;

    void addMissingNuc(size_t n)
    {
        outSeq += rareSeq.substr(0, n);
    }

public:
    Fsm(std::string& out)
        : rareSeq("TTT")
        , outSeq(out)
    {
    }

};

#endif
