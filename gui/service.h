#ifndef SERVICE_H
#define SERVICE_H

#include <iostream>
#include <fstream>
#include "nuca.h"

class Service
{
public:
    Service();
    static void compression(const std::string& fileIn, const std::string& fileOut);
    static void decompression(const std::string& fileIn, const std::string& fileOut);

};

#endif
