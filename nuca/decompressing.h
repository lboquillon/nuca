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
