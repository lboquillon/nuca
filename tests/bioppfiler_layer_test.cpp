#include <gtest/gtest.h>
#include <string>

#include "nuca/nuca.h"
#include "biopp/biopp.h"
#include "biopp-filer/bioppFiler.h"

typedef FastaLoader<RemoveNs<CompressingBitHandler<DecompressingBitHandler<AddNs<StringTestLayer<EndLayer> > > > > > Loader;

typedef FastaLoader<RemoveNs<CompressingBitHandler<DecompressingBitHandler<AddNs<FastaWriter<StringTestLayer<EndLayer> > > > > > > LoaderWriter;

#define CREATE_FASTA_FILE(fileName, name, seq) \
    bioppFiler::FastaSaver<biopp::PseudoNucSequence> fs(fileName); \
    fs.saveNextSequence(name, biopp::PseudoNucSequence(seq))

#define GET_SEQUENCE_FROM_FILE(fileName, name, seq) \
    bioppFiler::FastaParser<biopp::PseudoNucSequence> fp(fileName); \
    fp.getNextSequence(name, seq)

TEST(BiopppFilerLayerTest, Load)
{
    CREATE_FASTA_FILE("file1_test", "seq1", "ACUGNNCATT");

    bool multiple;
    std::string out;
    std::string nameOut;
    Loader loader;

    loader.setFastaFile("file1_test");
    loader.setOutputString(out);
    loader.setSeqNameString(nameOut);
    loader.run(multiple);

    ASSERT_EQ(out, "ACUGNNCAUU");
    ASSERT_EQ(nameOut, "seq1");
}

TEST(BioppFilerLayerTest, LoadWrite)
{
    const std::string seq = "ACNNNUGGNUUACGGGNNNNNNNNNNNNNNUNN";
    std::string name;
    std::string nameEx;
    std::string testStr;
    bool multiple;
    biopp::PseudoNucSequence outFromFile;

    CREATE_FASTA_FILE("file_in_test", "seq2", seq);

    LoaderWriter loaderWriter;
    loaderWriter.setOutputString(testStr);

    //for loader layer
    loaderWriter.setFastaFile("file_in_test");
    loaderWriter.setSeqNameString(name);

    //for writer layer
    loaderWriter.setFastaFileOut("file_out_test");
    loaderWriter.setSeqName(name);

    loaderWriter.run(multiple);

    GET_SEQUENCE_FROM_FILE("file_out_test", nameEx, outFromFile);

    ASSERT_EQ(testStr, seq);
    ASSERT_EQ(outFromFile, biopp::PseudoNucSequence(seq));
}
