#pragma once
#include <opencv2/opencv.hpp>
#include <vector>
#include <string>
#include "Snake.h"


void showMenu(cv::Mat& frame, int& selectedOption);
void handleMenuInput(int key, int& selectedOption, GameStates& currentState, SnakeGame& game);

void showGameOverMenu(cv::Mat& frame, int& selectedOption);
void handleGameOverMenuInput(int key, int& selectedOption, GameStates& currentState, SnakeGame& game);

void showOptionsMenu(cv::Mat& frame, int& selectedOption, int snakeSpeed, bool soundEnable, int& windowWidth, int& windowHeight);
void handleOptionsMenuInput(int key, int& selectedOption, GameStates& currentState, int& snakeSpeed, bool& soundEnable, int& windowWidth, int& windowHeight, SnakeGame& game, cv::Mat& frame);
//void checkWindowSize(const std::string& windowName);