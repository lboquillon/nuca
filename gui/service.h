#ifndef SERVICE_H
#define SERVICE_H

#include <string>

class Service
{

public:
    void compression(const std::string& fileIn, const std::string& fileOut);
    void decompression(const std::string& fileIn, const std::string& fileOut);

private:
    std::string sequenceName;
};


#endif
