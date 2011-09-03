/*
comp.h: Nucleotides Compression Algorithms
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

#include <fstream>
#include <string>
#include <iostream>
#include <queue>
#include <biopp/bio_molecular/bio_molecular.h>

using namespace std;

typedef unsigned char BufferType;

// Loader part: =======================

template<class User>
class Retriever
{
    User& u;
    bool aux_ret;
    size_t current_bit;
    BufferType buffer;
    typedef unsigned int Bits;
    static const Bits DataSize = 2;
    static const Bits TotalUsedBits = sizeof(buffer) << 3;
    std::queue<BufferType> pending;
    size_t size;

    void reload()
    {
        buffer = pending.front();
        pending.pop();

        char tmp;
        aux_ret = u.load(tmp);

        if (aux_ret)
        {
            pending.push(tmp);
        }
        else
        {
            size = size_t (pending.front());
            pending.pop();
        }

        current_bit = 0;
    }
public:

    typedef unsigned int Data;

    Retriever(User& u)
        : u(u),
          aux_ret(true),
          current_bit(8)
    {

        char re;
        u.load(re);
        pending.push(re);
        u.load(re);
        pending.push(re);

    }

    bool get(Data& data)
    {
        bool ret;
        if (aux_ret || size >= 1)
        {
            if (current_bit == 8)
                reload();
            else
                buffer >>= DataSize;

            data = buffer & 0x3;
            current_bit += DataSize;

            if (!aux_ret)
                --size;

            ret = true;
        }
        else
        {
            ret = false;
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

    bool load(char& buffer)
    {
        bool ret;
        char tmp;

        if (f.get(tmp))
        {
            buffer = tmp;
            ret = true;
        }
        else
        {
            ret = false;
        }

        cerr << std::hex << (buffer & 0xff) << endl;
//      buffer >>= 8;
        return ret;
//    }
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

    Storer(User& u)
        : u(u),
          free_bits(TotalFreeBits),
          buffer(0)
    {
    }

    ~Storer()
    {
        flush();
    }

    void add(Data data)
    {
        if (free_bits < DataSize)
            flush();

        buffer |= (data << (TotalFreeBits - free_bits));     // optimize out!
        free_bits -= DataSize;
    }

    void end()
    {
        flush();
    }

    void close()
    {
        flush();
        u.close();
    }

    void set_size(Data size)
    {
        size_t n = size % 4;
        // should be save as char
        // to not add extra bits
        // e.g (2 0 0 0)
        u.save(char(n == 0 ? 4 : n));
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

    void close()
    {
        of.close();
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

    // should be called to flush time before end the
    // scope and not depend of dtor..  to later save the size
    st.end();
    st.set_size(sequ.size());

    st.close();
}

template <class User>
inline void loadSeq(Retriever<User>& rt, NucSequence& seq)
{
    typename Retriever<User>::Data data;
    while (rt.get(data))
        seq += Nucleotide(data);
}

