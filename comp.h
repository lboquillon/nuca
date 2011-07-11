// Copyright (C) FuDePAN 2011

#include <fstream>
#include <string>
#include <iostream>

using namespace std;

typedef string Sequence;
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


//static void loadSeq(const char* fname, Sequence& seq);
template <class User>
static void storeSeq(Storer<User>& st, const Sequence& seq);

enum Nuc { A, T, C, G };

inline BufferType nuc2data(char nuc)
{
    switch (nuc)
    {
        case 'A':
        case 'a':
            return A;
        case 'T':
        case 't':
        case 'U':
        case 'u':
            return T;
        case 'C':
        case 'c':
            return C;
        case 'G':
        case 'g':
            return G;
        default:
            throw string("Invalid nucleotide ") + nuc;
    }
}

inline char data2nuc(BufferType b)
{
    static const char nucs[] = "ATCG";
    if (b <= G)
        return nucs[unsigned(b)];
    else
        throw string("Invalid byte");
}


// Utility ------------------------------

template <class User>
inline void storeSeq(Storer<User>& st, const Sequence& seq)
{
    for (Sequence::const_iterator it = seq.begin(); it != seq.end(); ++it)
    {
        st.add(nuc2data(*it));
    }

    st.close();
}

template <class User>
inline void loadSeq(Retriever<User>& rt, Sequence& seq)
{
    typename Retriever<User>::Data data;
    while (rt.get(data))
        seq += data2nuc(data);
}

