// Copyright (C) FuDePAN 2011

#include <stdlib.h>
#include "comp.h"

int main(int argc, char* argv[])
{
    int ret = EXIT_FAILURE;
    // compress!
    if (argc != 2)
        cerr << "Invalid arguments. Use " << argv[0] << " input " << endl;
    else
    {
        try
        {
            Sequence seq;
            Loader loader(argv[1]);
            Retriever<Loader> retriever(loader);
            loadSeq(retriever, seq);
            cout << seq << endl;
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

