#pragma once
#pragma once
#include <opencv2/opencv.hpp>
#include "Map.h"

class MapEditor {
private:
    Map& map;             // Reference to the map being edited
    int cellSize;         // Size of each grid cell in pixels
    bool isDrawing;       // Tracks left mouse button state
    bool isErasing;       // Tracks right mouse button state

public:
    // Constructor
    MapEditor(Map& map, int cellSize)
        : map(map), cellSize(cellSize), isDrawing(false), isErasing(false) {}

    // Mouse callback for editing the map
    void handleMouse(int event, int x, int y) {
        // Calculate cell indices from pixel coordinates
        int col = x / cellSize;
        int row = y / cellSize;

        if (event == cv::EVENT_LBUTTONDOWN) {
            isDrawing = true;  // Start drawing
            map.setObstacle(col, row);
        }
        else if (event == cv::EVENT_RBUTTONDOWN) {
            isErasing = true;  // Start erasing
            map.unsetObstacle(col, row);
        }
        else if (event == cv::EVENT_MOUSEMOVE) {
            if (isDrawing) {
                map.setObstacle(col, row);
            }
            else if (isErasing) {
                map.unsetObstacle(col, row);
            }
        }
        else if (event == cv::EVENT_LBUTTONUP) {
            isDrawing = false; // Stop drawing
        }
        else if (event == cv::EVENT_RBUTTONUP) {
            isErasing = false; // Stop erasing
        }
    }

    // Render the map onto a canvas
    void render(cv::Mat& canvas) const {
        canvas.setTo(cv::Scalar(255, 255, 255)); // Clear to white background
        const cv::Mat& mapMatrix = map.getMap();

        for (int i = 0; i < mapMatrix.rows; ++i) {
            for (int j = 0; j < mapMatrix.cols; ++j) {
                if (mapMatrix.at<uchar>(i, j) == 1) { // Obstacle
                    cv::rectangle(canvas,
                        cv::Point(j * cellSize, i * cellSize),
                        cv::Point((j + 1) * cellSize, (i + 1) * cellSize),
                        cv::Scalar(0, 0, 255), cv::FILLED);
                }
            }
        }
    }
};
