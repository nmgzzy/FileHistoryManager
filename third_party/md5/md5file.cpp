#include "md5file.h"
#include "md5global.h"
#include "md5.h"

#include <fstream>
#include <sstream>
#include <memory>
#include <iomanip>
#include <exception>

std::string getFileMD5(const std::string &filename)
{
    std::ifstream fin(filename.c_str(), std::ifstream::in | std::ifstream::binary);
    if (fin)
    {
        MD5_CTX context;
        MD5Init(&context);

        fin.seekg(0, fin.end);
        const auto fileLength = fin.tellg();
        fin.seekg(0, fin.beg);

        const int bufferLen = 8192;
        std::unique_ptr<unsigned char[]> buffer{new unsigned char[bufferLen]{}};
        unsigned long long totalReadCount = 0;
        decltype(fin.gcount()) readCount = 0;

        while (fin.read(reinterpret_cast<char *>(buffer.get()), bufferLen))
        {
            readCount = fin.gcount();
            totalReadCount += readCount;
            MD5Update(&context, buffer.get(), static_cast<unsigned int>(readCount));
        }

        readCount = fin.gcount();
        if (readCount > 0)
        {
            totalReadCount += readCount;
            MD5Update(&context, buffer.get(), static_cast<unsigned int>(readCount));
        }
        fin.close();

        if (totalReadCount != fileLength)
        {
            std::ostringstream oss;
            oss << "FATAL ERROR: read " << filename << " failed!" << std::ends;
            throw std::runtime_error(oss.str());
        }

        unsigned char digest[16];
        MD5Final(digest, &context);

        std::ostringstream oss;
        for (int i = 0; i < 16; ++i)
        {
            oss << std::hex << std::setw(2) << std::setfill('0') << static_cast<unsigned int>(digest[i]);
        }
        oss << std::ends;

        return std::move(oss.str());
    }
    else
    {
        std::ostringstream oss;
        oss << "FATAL ERROR: " << filename << " can't be opened" << std::ends;
        throw std::runtime_error(oss.str());
    }
}
