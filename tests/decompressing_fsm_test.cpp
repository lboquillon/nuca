#include <gtest/gtest.h>
#include "../decompressing_fsm.h"

TEST(Decompressing, equalStr)
{
    std::string out;
    const std::string sequence = "CTGATTTGGGGAATTTAAAA";
    DecompressingFSM fsm(out);

    for (std::string::const_iterator it = sequence.begin(); it < sequence.end(); ++it)
        fsm.stimulate(*it);

    fsm.stimulate(0);

    ASSERT_EQ(out, "CTGANNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNAATTT");

}
