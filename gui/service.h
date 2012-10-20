#ifndef SERVICE_H
#define SERVICE_H

#include <string>

class Service
{

public:
    static void compression(const std::string& fileIn, const std::string& fileOut, bool& multipleSequence);
    static void decompression(const std::string& fileIn, const std::string& fileOut);
};


#endif
