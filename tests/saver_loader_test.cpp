#include <gtest/gtest.h>
#include <biopp/bio_molecular/bio_molecular.h>
#include "nuca.h"

TEST(saver_loader, equalStr1)
{
    const NucSequence in("ATTCCTTGTGGTTTCCAAGGTTCTCTCTAGTCCTGAC");
    NucSequence out;
    Saver saver("archivo.out");
    Storer<Saver> storer(saver, in.length());
    storeSeq(storer, in);

    Loader loader("archivo.out");
    Retriever<Loader> retriever(loader);
    loadSeq(retriever, out);

    ASSERT_EQ(in, out);
}

TEST(saver_loader, equalStr2)
{
    const NucSequence in("CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAACCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTGGGGGGGGGGGGGGGGGGGGGGGGGGGACTGAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAACCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGTCAGTCA");
    NucSequence out;
    Saver saver("archivo.out");
    Storer<Saver> storer(saver, in.length());
    storeSeq(storer, in);

    Loader loader("archivo.out");
    Retriever<Loader> retriever(loader);
    loadSeq(retriever, out);

    ASSERT_EQ(in, out);
}
