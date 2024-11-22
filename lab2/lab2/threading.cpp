#include "threading.h"

DWORD WINAPI blurThread(const LPVOID params) {
    ThreadInfo* info = (ThreadInfo*)params;
    int height = info->image->size();
    int width = info->image->at(0).size();

    for (int iteration = 0; iteration < 7; iteration++) {
        for (int y = 0; y < height; y++) {
            for (int x = info->startColumn; x <= info->endColumn; x++) {
                int sumRed = 0;
                int sumGreen = 0;
                int sumBlue = 0;
                int count = 0;

                for (int dy = y - 1; dy <= y + 1; dy++) {
                    for (int dx = x - 1; dx <= x + 1; dx++) {
                        if (dy >= 0 && dy < height && dx >= 0 && dx < width) {
                            sumRed += info->image->at(dy)[dx][2];
                            sumGreen += info->image->at(dy)[dx][1];
                            sumBlue += info->image->at(dy)[dx][0];
                            count++;
                        }
                    }
                }
                if (count > 0) {
                    info->image->at(y)[x][2] = sumRed / count;
                    info->image->at(y)[x][1] = sumGreen / count;
                    info->image->at(y)[x][0] = sumBlue / count;
                }
            }
        }
    }
    ExitThread(0);
}

void blurImageInThreads(int threadCount, std::vector<std::vector<cv::Vec3b>>& image) {
    int height = image.size();
    int width = image[0].size();
    int columnCount = width / threadCount;
    int remainderColumns = width % threadCount;
    HANDLE* threadHandles = new HANDLE[threadCount];
    ThreadInfo* threadInfos = new ThreadInfo[threadCount];
    int lastEnd = 0;
    for (int i = 0; i < threadCount; i++) {
        ThreadInfo info;
        info.threadCount = threadCount;
        info.image = &image;
        info.startColumn = lastEnd;
        lastEnd += columnCount;
        info.endColumn = lastEnd - 1;
        if (i < remainderColumns) {
            lastEnd++;
        }
        threadInfos[i] = info;
        threadHandles[i] = CreateThread(NULL, 0, &blurThread, &threadInfos[i], CREATE_SUSPENDED, NULL);
    }
    for (int i = 0; i < threadCount; i++) {
        ResumeThread(threadHandles[i]);
    }
    WaitForMultipleObjects(threadCount, threadHandles, true, INFINITE);
}