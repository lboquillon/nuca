/*
comp.h: Nucleotides Compression Algorithms
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

#include <fstream>
#include <string>
#include <iostream>
#include <biopp/bio_molecular/bio_molecular.h>

using namespace std;

typedef unsigned char BufferType;

// Loader part: =======================

template<class User>
class Retriever
{
    User& u;
    size_t current_bit;
    size_t size;
    BufferType buffer;
    typedef unsigned int Bits;
    static const Bits DataSize = 2;
    static const Bits TotalUsedBits = sizeof(buffer) << 3;

    void reload()
    {
        char tmp;
        u.load(tmp);
        buffer = tmp;
        current_bit = 0;
    }
public:
    typedef unsigned int Data;

    Retriever(User& u) : u(u), current_bit(0)
    {
        // the first data is the size
        u.load(size);
        current_bit = 8;
    }

    bool get(Data& data)
    {
        const bool ret = (size > 0);
        if (ret)
        {
            if (current_bit == 8)
                reload();
            else
                buffer >>= DataSize;

            data = buffer & 0x3;
            current_bit += DataSize;

            size--;
        }
        return ret;
    }
};

class Loader
{
    ifstream f;
public:
    Loader(const char* fname)
        : f(fname, ios_base::binary | ios_base::in)
    {
        if (!f)
            throw string(fname) + " not found";
    }

    void load(char& buffer)
    {
//    for( size_t byte = 0; byte < sizeof(buffer); ++byte )
//    {
        char tmp;
        if (!(f.get(tmp)))
            throw "Cannot read";

        buffer = tmp;
        cerr << std::hex << (buffer & 0xff) << endl;
//      buffer >>= 8;
//    }
    }

    void load(size_t& number)
    {
        char* const number_chr = reinterpret_cast<char*>(&number);
        for (size_t byte = 0; byte < sizeof(number); ++byte)
            load(number_chr[byte]);
    }
};


// Saver part: ==============================================================
template<class User>
class Storer
{
    User& u;
    size_t free_bits;
    BufferType buffer;
    typedef unsigned int Bits;
    static const Bits DataSize = 2;
    static const Bits TotalFreeBits = sizeof(buffer) << 3;

    void flush()
    {
        if (free_bits < TotalFreeBits)
        {
            u.save(char(buffer));
            free_bits = TotalFreeBits;
        }
        buffer = 0;
    }
public:
    typedef unsigned int Data;

    Storer(User& u, size_t size) : u(u)
    {
        // the first data is the size
//      const Bits size_in_bits = size % 8;
//    buffer = size_in_bits;
        //buffer = size;
        buffer = 0;
        free_bits = TotalFreeBits;
        u.save(size);
        //flush();
    }

    ~Storer()
    {
        flush();
    }

    void add(Data data)
    {
        if (free_bits < DataSize)
            flush();

        buffer |= (data << (TotalFreeBits - free_bits));  // optimize out!
        free_bits -= DataSize;
    }

    void close()
    {
        flush();
    }
};

class Saver
{
    ofstream of;
public:
    Saver(const char* fname)
        : of(fname, ios_base::binary)
    {}

    void save(char buffer)
    {
        for (size_t byte = 0; byte < sizeof(buffer); ++byte)
        {
            of.put(static_cast<char>(buffer & 0xff));
            cerr << std::hex << (buffer & 0xff) << endl;
            buffer >>= 8;
        }
    }

    void save(size_t number)
    {
        const char* number_chr = reinterpret_cast<const char*>(&number);
        for (size_t byte = 0; byte < sizeof(number); ++byte)
            save(number_chr[byte]);
    }
};


//static void loadSeq(const char* fname, NucSequence& seq);
template <class User>
static void storeSeq(Storer<User>& st, const NucSequence& seq);

// Utility ------------------------------

template <class User>
inline void storeSeq(Storer<User>& st, const NucSequence& seq)
{
    const std::string sequ = seq.getString();

    for (std::string::const_iterator it = sequ.begin(); it != sequ.end(); ++it)
    {
        st.add(BufferType(to_nuc(*it)));
    }

    st.close();
}

template <class User>
inline void loadSeq(Retriever<User>& rt, NucSequence& seq)
{
    string sequence = "";

    typename Retriever<User>::Data data;
    while (rt.get(data))
        sequence += to_str(Nucleotide(data));

    seq = sequence;
}

