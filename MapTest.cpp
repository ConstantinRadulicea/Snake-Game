#include <opencv2/opencv.hpp>
#include "Map.h"
#include "MapEditor.h"

//const int cellSize = 20;   // Size of each cell in pixels
//
//// Mouse callback wrapper for MapEditor
//void mouseCallback(int event, int x, int y, int flags, void* userdata) {
//    auto editor = reinterpret_cast<MapEditor*>(userdata);
//    editor->handleMouse(event, x, y);
//}
//
//int main222() {
//    const int rows = 20;       // Map rows
//    const int cols = 30;       // Map columns
//    const std::string mapFile = "map.txt";
//
//    // Initialize Map
//    Map mapHandler(rows, cols, mapFile);
//
//    // Load the map (or create a new one if not found)
//    mapHandler.load();
//
//    // Initialize MapEditor
//    MapEditor editor(mapHandler, cellSize);
//
//    // Canvas for displaying the map
//    cv::Mat canvas(rows * cellSize, cols * cellSize, CV_8UC3);
//
//    // Create window and set mouse callback
//    cv::namedWindow("Snake Game Map Editor");
//    cv::setMouseCallback("Snake Game Map Editor", mouseCallback, &editor);
//
//    while (true) {
//        // Render the map using the MapEditor
//        editor.render(canvas);
//        cv::imshow("Snake Game Map Editor", canvas);
//
//        char key = cv::waitKey(10);
//        if (key == 's') {
//            mapHandler.save();
//        }
//        else if (key == 27) { // ESC to exit
//            break;
//        }
//    }
//
//    return 0;
//}
