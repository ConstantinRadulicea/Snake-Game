#pragma once
#include <opencv2/opencv.hpp>
#include <iostream>
#include <fstream>

class Map {
private:
    cv::Mat map; // Map matrix
    std::string mapFile; // Path to the map file

public:
    // Constructor
    Map(int rows, int cols, const std::string& mapFile)
        : mapFile(mapFile) {
        map = cv::Mat(rows, cols, CV_8UC1, cv::Scalar(0)); // Initialize with free space (0)
    }




    // Load map from file
    bool load() {
        std::ifstream file(mapFile);
        if (!file.is_open()) {
            std::cerr << "Map file not found! Creating a new empty map." << std::endl;
            return false;
        }
        for (int i = 0; i < map.rows; ++i) {
            for (int j = 0; j < map.cols; ++j) {
                int value;
                file >> value;
                map.at<uchar>(i, j) = static_cast<uchar>(value);
            }
        }
        file.close();
        return true;
    }

    // Save map to file
    void save() const {
        std::ofstream file(mapFile);
        if (!file.is_open()) {
            std::cerr << "Failed to save the map!" << std::endl;
            return;
        }
        for (int i = 0; i < map.rows; ++i) {
            for (int j = 0; j < map.cols; ++j) {
                file << static_cast<int>(map.at<uchar>(i, j)) << " ";
            }
            file << "\n";
        }
        file.close();
        std::cout << "Map saved to " << mapFile << std::endl;
    }

    bool isObstacle(int x, int y) const {
        // Calculate the row and column indices based on x and y
        int i = y;
        int j = x;

        // Check bounds to ensure valid access
        if (i >= 0 && i < map.rows && j >= 0 && j < map.cols) {
            return map.at<uchar>(i, j) == 1; // Return true if the cell is an obstacle
        }

        return false; // Out of bounds, treat as no obstacle
    }

    void setObstacle(int x, int y) {
        int i = y;
        int j = x;

        if (i >= 0 && i < map.rows && j >= 0 && j < map.cols) {
            map.at<uchar>(i, j) = 1;
        }
    }

    void unsetObstacle(int x, int y) {
        int i = y;
        int j = x;

        if (i >= 0 && i < map.rows && j >= 0 && j < map.cols) {
            map.at<uchar>(i, j) = 0;
        }
    }

    // Getters
    int getRows() const { return map.rows; }
    int getCols() const { return map.cols; }
    const cv::Mat& getMap() const { return map; }
};
