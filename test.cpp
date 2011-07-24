// Copyright (C) FuDePAN 2011

#include "comp.h"

static void showSeq(const NucSequence& seq)
{
    cout << seq.getString() << " " << "(" << seq.length() << ")" << endl;
}

static void testSave(const NucSequence& seq)
{
    showSeq(seq);
    Saver saver("archivo.out");
    Storer<Saver> storer(saver, seq.length());
    storeSeq(storer, seq);
}

static void testLoad(const NucSequence& orig)
{
    NucSequence seq;
    Loader loader("archivo.out");
    Retriever<Loader> retriever(loader);
    loadSeq(retriever, seq);
    showSeq(seq);
    cout << boolalpha << (orig.getString() == seq.getString()) << endl;
}

int main()
{
    try
    {
    	const std::string str = "ATTCCTTGTGGTTTCCAAGGTTCTCTCTAGTCCTGAC";
        NucSequence seq = str;
        testSave(seq);
        cerr << "----------" << endl;
        testLoad(seq);
    }
    catch (const string& msg)
    {
        cerr << "Error: " << msg << endl;
    }
    catch (const char* msg)
    {
        cerr << "Error: " << msg << endl;
    }
    return 0;
}

