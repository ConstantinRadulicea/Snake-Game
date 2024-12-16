
#include <iostream>
#include <ctime>
#include <deque>
#include <cstdlib>
#include <fstream>
#include "Snake.h"
#include "Menu.h"
#include "Glob.h"
#include "MapEditor.h"


// Globals to track the window size
//int g_window_width = 0, g_window_height = 0;

int buttonPressed;


// Mouse callback wrapper for MapEditor
void mouseCallback(int event, int x, int y, int flags, void* userdata) {
    auto editor = reinterpret_cast<MapEditor*>(userdata);
    editor->handleMouse(event, x, y);
}

int map_editor_routine() {
    const int rows = MAP_HEIGHT;       // Map rows
    const int cols = MAP_WIDTH;       // Map columns
    const std::string mapFile = "map.txt";

    // Initialize Map
    Map mapHandler(rows, cols, mapFile);

    // Load the map (or create a new one if not found)
    mapHandler.load();

    // Initialize MapEditor
    MapEditor editor(mapHandler, CELL_SIZE);

    // Canvas for displaying the map
    cv::Mat canvas(rows * CELL_SIZE, cols * CELL_SIZE, CV_8UC3);

    // Create window and set mouse callback
    cv::namedWindow("Snake Game Map Editor");
    cv::setMouseCallback("Snake Game Map Editor", mouseCallback, &editor);

    while (true) {
        // Render the map using the MapEditor
        editor.render(canvas);
        cv::imshow("Snake Game Map Editor", canvas);

        char key = cv::waitKey(10);
        if (key == 's') {
            mapHandler.save();
        }
        else if (key == 27) { // ESC to exit
            break;
        }
    }

    return 0;
}



int main() {
    SnakeGame game;
    windowWidth = game.getWindowWidth();
    windowHeight = game.getWindowHeigth();
    cv::Mat frame(windowHeight, windowWidth, CV_8UC3);
    cv::namedWindow("Snake Game", cv::WINDOW_NORMAL);

    GameStates currentState = MENU;
    int selectedOption = 0;
    int snakeSpeed = 100;
    bool soundEnable = true;

    int64_t gameOverTimeStamp = 0;

    while (currentState != EXIT) 
    {
        int key = cv::waitKey(snakeSpeed);

        if (key == 27 && currentState == PLAYING) { // SPACE for pause
            game.togglePause();
        }

        if (game.isGamePaused()) {
            frame = cv::Scalar(0, 0, 0);
            putText(frame, "Pause", cv::Point(windowWidth / 3 + 20, windowHeight / 2), cv::FONT_HERSHEY_SIMPLEX, 1, cv::Scalar(255, 255, 255), 2);
            putText(frame, "Press ESC to Resume", cv::Point(windowWidth / 3 - 110, windowHeight / 2 + 50), cv::FONT_HERSHEY_SIMPLEX, 0.9, cv::Scalar(255, 255, 255), 1.5);
            //putText(frame, "Press 1 to Buy a Life (5 points)", cv::Point(windowWidth / 3 - 110, windowHeight / 2 + 150), cv::FONT_HERSHEY_SIMPLEX, 0.9, cv::Scalar(255, 255, 255), 1.5);
            //putText(frame, "Press 2 to Buy a Super Power (15 points)", cv::Point(windowWidth / 3 - 110, windowHeight / 2 + 100), cv::FONT_HERSHEY_SIMPLEX, 0.9, cv::Scalar(255, 255, 255), 1.5);
            putText(frame, "Press 1 to enter map editor", cv::Point(windowWidth / 3 - 110, windowHeight / 2 + 150), cv::FONT_HERSHEY_SIMPLEX, 0.9, cv::Scalar(255, 255, 255), 1.5);

            //if (key == '1') {  // Check for key '1'
            //    game.buyLife();  // Call buyLife() function
            //}
            //if (key == '2') {  // Check for key '2'
            //    game.buySuperPower(snakeSpeed);  // Call buySuperPower() function
            //}
            if (key == '1') // map editor
            {
                map_editor_routine();
                game.resetGame();
            }

            imshow("Snake Game", frame); 
            continue;
        }

        switch (currentState)
        {
        case MENU:
            showMenu(frame, selectedOption);
            handleMenuInput(key, selectedOption, currentState, game);
            break;

        case PLAYING:
            if (key != -1) game.changeDirection(key);
            game.update(snakeSpeed);
            game.render(frame);

            if (game.isGameOver()) {
                currentState = GAME_OVER;
                gameOverTimeStamp = cv::getTickCount();
                selectedOption = 0;
            }
            break;

        case OPTIONS:
            showOptionsMenu(frame, selectedOption, snakeSpeed, soundEnable, windowWidth, windowHeight);
            handleOptionsMenuInput(key, selectedOption, currentState, snakeSpeed, soundEnable, windowWidth, windowHeight, game, frame);
            break;

        case EXIT:
            break;
        
        case GAME_OVER:
            game.render(frame);
            if ((cv::getTickCount() - gameOverTimeStamp) / cv::getTickFrequency() >= 3) {
                showGameOverMenu(frame, selectedOption);
                handleGameOverMenuInput(key, selectedOption, currentState, game);
            }
            break;
        
        }

        imshow("Snake Game", frame);
    }

    return 0;
}


