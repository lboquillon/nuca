/*
test.cpp: Nucleotides Compression Algorithms
    Copyright (C) 2011 Daniel Gutson and Leonardo Boquillon, FuDePAN

    This file is part of Nuca.

    Aso is free software: you can redistribute it and/or modify
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

#include "comp.h"

static void showSeq(const NucSequence& seq)
{
    cout << seq << " " << "(" << seq.length() << ")" << endl;
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
    cout << boolalpha << (orig == seq) << endl;
}

int main()
{
    try
    {
        const NucSequence seq("ATTCCTTGTGGTTTCCAAGGTTCTCTCTAGTCCTGAC");
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

