#include <iostream>
#include <fstream>
#include <vector>
#include <windows.h>
#include <chrono>
#include <opencv2/opencv.hpp>
#include "bmp_headers.h"
#include "image_processing.h"
#include "threading.h"

void processImage(int threadCount, const std::string& inputFileName, const std::string& outputFileName) {
    std::ifstream inputFile(inputFileName, std::ios::binary);
    if (!inputFile.is_open()) {
        throw std::invalid_argument("Не удалось открыть входной файл");
    }
    auto headers = readHeaders(inputFile);
    if (headers.fileHeader.type != 'MB') {
        throw std::invalid_argument("Входной файл не является .bmp");
    }
    if (headers.infoHeader.bitDepth != 32) {
        throw std::invalid_argument("Количество бит должно быть 32");
    }
    std::vector<uint8_t> data(headers.infoHeader.width * headers.infoHeader.height * 4);
    inputFile.read(reinterpret_cast<char*>(data.data()), data.size());
    inputFile.close();
    auto image = createImageFromData(headers.infoHeader.width, headers.infoHeader.height, data);
    blurImageInThreads(threadCount, image);
    writeImageToData(image, data);
    std::ofstream outputFile(outputFileName, std::ios::binary);
    writeHeaders(headers, outputFile);
    outputFile.write(reinterpret_cast<char*>(data.data()), data.size());
    outputFile.close();
}

int main() {
    setlocale(LC_ALL, "Russian");

    while (true) {
        std::string inputFileName = "input.bmp";
        std::string outputFileName = "output.bmp";
        int threadCount;
        int processorCount;

        std::cout << "Введите количество потоков (максимум 16): ";
        while (true) {
            std::cin >> threadCount;
            if (std::cin.fail() || threadCount <= 0 || threadCount > 16) {
                std::cin.clear();
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                std::cout << "Неверный ввод. Пожалуйста, введите целое число от 1 до 16: ";
            }
            else {
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                break;
            }
        }

        std::cout << "Введите количество процессоров: ";
        while (true) {
            std::cin >> processorCount;
            if (std::cin.fail() || processorCount <= 0 || processorCount > 4) {
                std::cin.clear();
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                std::cout << "Неверный ввод. Пожалуйста, введите целое число от 1 до 4: ";
            }
            else {
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                break;
            }
        }

        auto startTime = std::chrono::high_resolution_clock::now();

        try {
            auto processStartTime = std::chrono::high_resolution_clock::now();

            // Обработка изображения
            processImage(threadCount, inputFileName, outputFileName);

            auto processEndTime = std::chrono::high_resolution_clock::now();
            std::chrono::duration<double> processDuration = processEndTime - processStartTime;

            // Повторить размытие, если время размытия меньше 0.5 секунд
            while (processDuration.count() < 0.5) {
                processImage(threadCount, inputFileName, outputFileName);
                processEndTime = std::chrono::high_resolution_clock::now();
                processDuration = processEndTime - processStartTime;
            }
        }
        catch (const std::exception& e) {
            std::cout << e.what() << std::endl;
        }

        auto endTime = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double> duration = endTime - startTime;

        std::cout << "Время выполнения: " << duration.count() << " секунд" << std::endl;

        std::cout << "Хотите повторить выполнение программы? (yes/no): ";
        std::string response;
        std::cin >> response;

        if (response != "да" && response != "yes") {
            break;
        }
    }

    return 0;
}