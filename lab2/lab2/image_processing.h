#pragma once
#ifndef IMAGE_PROCESSING_H
#define IMAGE_PROCESSING_H

#include <vector>
#include <opencv2/opencv.hpp>

std::vector<std::vector<cv::Vec3b>> createImageFromData(int width, int height, const std::vector<uint8_t>& data);
void writeImageToData(const std::vector<std::vector<cv::Vec3b>>& image, std::vector<uint8_t>& data);

#endif // IMAGE_PROCESSING_H