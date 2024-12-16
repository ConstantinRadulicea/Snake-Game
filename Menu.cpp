#include "Menu.h"



void showMenu(cv::Mat& frame, int& selectedOption) {
    std::vector<std::string> menuOptions = { "Start the Game", "Options", "Exit" };
    frame = cv::Scalar(0, 0, 0);

    for (size_t i = 0; i < menuOptions.size(); i++) {
        cv::Scalar color = (i == selectedOption) ? cv::Scalar(0, 255, 0) : cv::Scalar(255, 255, 255);
        putText(frame, menuOptions[i], cv::Point(windowWidth / 3, 100 + i * 40), cv::FONT_HERSHEY_SIMPLEX, 1, color, 2);
    }
}

void handleMenuInput(int key, int& selectedOption, GameStates& currentState, SnakeGame& game) {
    if (key == 'w' && selectedOption > 0) {
        selectedOption--;
    }
    if (key == 's' && selectedOption < 2) {
        selectedOption++;
    }

    if (key == 13) { // ASCI code for enter button
        switch (selectedOption) {
        case 0:
            currentState = PLAYING;
            game.resetGame();
            break;
        case 1: currentState = OPTIONS; break;
        case 2: currentState = EXIT; break;
        }
    }
}

void showGameOverMenu(cv::Mat& frame, int& selectedOption) {
    std::vector<std::string> gameOverMenuOptions = { "Retry", "Back to Menu" };
    frame = cv::Scalar(0, 0, 0);
    
    for (size_t i = 0; i < gameOverMenuOptions.size(); i++) {
        cv::Scalar color = (i == selectedOption) ? cv::Scalar(0, 255, 0) : cv::Scalar(255, 255, 255);
        putText(frame, gameOverMenuOptions[i], cv::Point(windowWidth / 3, windowHeight / 2 + i * 40), cv::FONT_HERSHEY_SIMPLEX, 1, color, 2);
    }

}

void handleGameOverMenuInput(int key, int& selectedOption, GameStates& currentState, SnakeGame& game) {

    if (key == 'w' && selectedOption > 0) selectedOption--;
    if (key == 's' && selectedOption < 1) selectedOption++;

    if (key == 13) { // ASCI code for enter button
        switch (selectedOption) {
        case 0:
            game.resetGame();
            currentState = PLAYING;
            break;
        case 1:
            currentState = MENU;
            break;
        }
    }

}

void showOptionsMenu(cv::Mat& frame, int& selectedOption, int snakeSpeed, bool soundEnable, int& windowWidth, int& windowHeight) {
    std::vector<std::string> optionsMenu = {
        "1. Snake Speed:",
        "2. Sound:",
        "3. Window Size: 800 x 600",
        "4. Full-screen: 1400 x 760",
        "5. Back"
    };
    frame = cv::Scalar(0, 0, 0);

    int textYPosition = 80;
    int lineSpacing = 50;

    for (size_t i = 0; i < optionsMenu.size(); i++) {
        cv::Scalar color = (i == selectedOption) ? cv::Scalar(0, 255, 0) : cv::Scalar(255, 255, 255);
        putText(frame, optionsMenu[i], cv::Point(50, textYPosition + i * lineSpacing), cv::FONT_HERSHEY_SIMPLEX, 0.8, color, 2);

        if (i == 0) {
            int baseX = 165;
            cv::Scalar colorLent = cv::Scalar(255, 255, 255);
            cv::Scalar colorNormal = cv::Scalar(255, 255, 255);
            cv::Scalar colorRapid = cv::Scalar(255, 255, 255);

            if (snakeSpeed == 200) {
                colorLent = cv::Scalar(0, 255, 0);
            }
            else if (snakeSpeed == 100) {
                colorNormal = cv::Scalar(0, 255, 0);
            }
            else if (snakeSpeed == 50) {
                colorRapid = cv::Scalar(0, 255, 0);
            }

            putText(frame, "Slow", cv::Point(baseX + 100, textYPosition), cv::FONT_HERSHEY_SIMPLEX, 0.8, colorLent, 2);
            putText(frame, "|", cv::Point(baseX + 160, textYPosition), cv::FONT_HERSHEY_SIMPLEX, 0.8, cv::Scalar(255, 255, 255), 2);
            putText(frame, "Normal", cv::Point(baseX + 170, textYPosition), cv::FONT_HERSHEY_SIMPLEX, 0.8, colorNormal, 2);
            putText(frame, "|", cv::Point(baseX + 270, textYPosition), cv::FONT_HERSHEY_SIMPLEX, 0.8, cv::Scalar(255, 255, 255), 2);
            putText(frame, "Fast", cv::Point(baseX + 280, textYPosition), cv::FONT_HERSHEY_SIMPLEX, 0.8, colorRapid, 2);
        }

        if (i == 1) {
            int baseX = 200;
            cv::Scalar colorOn = cv::Scalar(255, 255, 255);
            cv::Scalar colorOff = cv::Scalar(255, 255, 255);

            if (soundEnable) {
                colorOn = cv::Scalar(0, 255, 0);
            }
            else
            {
                colorOff = cv::Scalar(0, 255, 0);
            }


            putText(frame, "On", cv::Point(baseX, textYPosition + 53), cv::FONT_HERSHEY_SIMPLEX, 0.8, colorOn, 2);
            putText(frame, "|", cv::Point(baseX + 40, textYPosition + 53), cv::FONT_HERSHEY_SIMPLEX, 0.8, cv::Scalar(255, 255, 255), 2);
            putText(frame, "Off", cv::Point(baseX + 60, textYPosition + 53), cv::FONT_HERSHEY_SIMPLEX, 0.8, colorOff, 2);

        }
    }
}

void handleOptionsMenuInput(int key, int& selectedOption, GameStates& currentState, int& snakeSpeed, bool& soundEnable, int& windowWidth, int& windowHeight, SnakeGame& game, cv::Mat& frame) {
    if (key == 'w' && selectedOption > 0) selectedOption--;
    if (key == 's' && selectedOption < 4) selectedOption++;

    if (key == 13) {
        switch (selectedOption) {
        case 0:
            if (snakeSpeed == 100) snakeSpeed = 200;
            else if (snakeSpeed == 200) snakeSpeed = 50;
            else snakeSpeed = 100;
            break;

        case 1:
            soundEnable = !soundEnable;
            break;

        case 2:
            windowWidth = (windowWidth == WIDTH) ? 800 : WIDTH;
            windowHeight = (windowHeight == HEIGHT) ? 600 : HEIGHT;
            cv::resizeWindow("Snake Game", windowWidth, windowHeight);
            frame = cv::Mat(windowHeight, windowWidth, CV_8UC3);
            game.resetGame();
            break;

        case 3:
            windowWidth = 1400;
            windowHeight = 760;
            setWindowProperty("Snake Game", cv::WND_PROP_FULLSCREEN, cv::WINDOW_FULLSCREEN);
            frame = cv::Mat(windowHeight, windowWidth, CV_8UC3);
            game.resetGame();
            break;

        case 4:
            currentState = MENU;
            selectedOption = 0;
            break;
        }
    }

}

// Callback function to handle window size
//void checkWindowSize(const std::string& windowName) {
//    cv::Rect rect = cv::getWindowImageRect(windowName); // Get window dimensions
//    if (rect.width != g_window_width || rect.height != g_window_height) {
//        g_window_height = rect.height;
//        g_window_height = g_window_height >> 2;
//        g_window_height = g_window_height << 2;
//        g_window_height = g_window_height - (g_window_height % CELL_SIZE);
//        //g_window_width = rect.width;
//        g_window_width = (int)((float)g_window_height * 1.5f);
//        g_window_width = g_window_width - (g_window_width % CELL_SIZE);
//        std::cout << "Window resized to: " << g_window_width << "x" << g_window_height << std::endl;
//    }
//}

