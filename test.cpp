// Copyright (C) FuDePAN 2011

#include "comp.h"

static void showSeq(const Sequence& seq)
{
    cout << seq << " " << "(" << seq.size() << ")" << endl;
}

static void testSave(const Sequence& seq)
{
    showSeq(seq);
    Saver saver("archivo.out");
    Storer<Saver> storer(saver, seq.size());
    storeSeq(storer, seq);
}

static void testLoad(const Sequence& orig)
{
    Sequence seq;
    Loader loader("archivo.out");
    Retriever<Loader> retriever(loader);
    loadSeq(retriever, seq);
    showSeq(seq);
    cout << boolalpha << (orig == seq) << endl;
}

int main()
{
    try
    {
        const Sequence seq = "ATTCCTTGTGGTTTCCAAGGTTCTCTCTAGTCCTGAC";
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

