// Copyright (C) FuDePAN 2011

#include <stdlib.h>
#include "comp.h"

static void loadUncompressed(const char* fname, NucSequence& seq);

int main(int argc, char* argv[])
{
    int ret = EXIT_FAILURE;
    // compress!
    if (argc != 3)
        cerr << "Invalid arguments. Use " << argv[0] << " input output" << endl;
    else
    {
        try
        {
            NucSequence seq;
            loadUncompressed(argv[1], seq);
            Saver saver(argv[2]);
            Storer<Saver> storer(saver, seq.size());
            storeSeq(storer, seq);
            ret = EXIT_SUCCESS;
        }
        catch (const string& msg)
        {
            cerr << "Error: " << msg << endl;
        }
        catch (const char* msg)
        {
            cerr << "Error: " << msg << endl;
        }
    }
    return ret;
}

void loadUncompressed(const char* fname, NucSequence& seq)
{
    ifstream input(fname);
    if (!input)
        throw "Input file not found";
    getline(input, seq);
    cerr << "Sequence loaded: " << seq << endl;
}

