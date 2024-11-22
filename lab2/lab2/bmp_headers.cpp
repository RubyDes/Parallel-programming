#include "bmp_headers.h"

BMPData readHeaders(std::ifstream& file) {
    BMPData headers;
    file.read(reinterpret_cast<char*>(&headers.fileHeader), sizeof(BMPHeader));
    file.read(reinterpret_cast<char*>(&headers.infoHeader), sizeof(BMPInfo));
    return headers;
}

void writeHeaders(const BMPData& headers, std::ofstream& file) {
    file.write(reinterpret_cast<const char*>(&headers.fileHeader), sizeof(BMPHeader));
    file.write(reinterpret_cast<const char*>(&headers.infoHeader), sizeof(BMPInfo));
}