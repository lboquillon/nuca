/*
decompdna.cpp: Nucleotides Compression Algorithms
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
            NucSequence seq;
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
