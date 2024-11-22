#include "image_processing.h"

std::vector<std::vector<cv::Vec3b>> createImageFromData(int width, int height, const std::vector<uint8_t>& data) {
    int byteIndex = 0;
    std::vector<std::vector<cv::Vec3b>> image(height, std::vector<cv::Vec3b>(width));
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            image[y][x] = cv::Vec3b(data[byteIndex], data[byteIndex + 1], data[byteIndex + 2]);
            byteIndex += 4;
        }
    }
    return image;
}

void writeImageToData(const std::vector<std::vector<cv::Vec3b>>& image, std::vector<uint8_t>& data) {
    int byteIndex = 0;
    for (const auto& row : image) {
        for (const auto& pixel : row) {
            data[byteIndex] = pixel[0];
            data[byteIndex + 1] = pixel[1];
            data[byteIndex + 2] = pixel[2];
            data[byteIndex + 3] = 0xFF; // Alpha (для 32-битного)
            byteIndex += 4;
        }
    }
}