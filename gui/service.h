#ifndef SERVICE_H
#define SERVICE_H

#include <string>
#include "nuca/nuca.h"

void compression(const std::string& fileIn, const std::string& fileOut);

void decompression(const std::string& fileIn, const std::string& fileOut);

#endif
