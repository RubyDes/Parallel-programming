#pragma once
#ifndef BMP_HEADERS_H
#define BMP_HEADERS_H

#include <fstream>
#include <cstdint>

#pragma pack(push, 1)

struct BMPHeader {
    uint16_t type;
    uint32_t size;
    uint16_t reserved1;
    uint16_t reserved2;
    uint32_t offset;
};

struct BMPInfo {
    uint32_t headerSize;
    int32_t width;
    int32_t height;
    uint16_t planes;
    uint16_t bitDepth;
    uint32_t compression;
    uint32_t imageSize;
    int32_t xResolution;
    int32_t yResolution;
    uint32_t colorsUsed;
    uint32_t importantColors;
};

#pragma pack(pop)

struct BMPData {
    BMPHeader fileHeader;
    BMPInfo infoHeader;
};

BMPData readHeaders(std::ifstream& file);
void writeHeaders(const BMPData& headers, std::ofstream& file);

#endif // BMP_HEADERS_H