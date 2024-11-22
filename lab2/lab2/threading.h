#pragma once
#ifndef THREADING_H
#define THREADING_H

#include <vector>
#include <windows.h>
#include <opencv2/opencv.hpp>

struct ThreadInfo {
    int threadCount;
    int startColumn;
    int endColumn;
    std::vector<std::vector<cv::Vec3b>>* image;
};

DWORD WINAPI blurThread(const LPVOID params);
void blurImageInThreads(int threadCount, std::vector<std::vector<cv::Vec3b>>& image);

#endif // THREADING_H