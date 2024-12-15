#pragma once

#include <opencv2/opencv.hpp>
#include <deque>
#include <string>
#include <fstream>
#include "Glob.h"

const std::string HIGH_SCORE_FILE = "highscore.txt";

#define MAX_HARTS 3
#define SUPERPOWER_HARTS_PRICE 1

enum Direction { UP, DOWN, LEFT, RIGHT };

enum GameStates { MENU, PLAYING, OPTIONS, EXIT, GAME_OVER };

struct SnakePoint {
    int x, y;
    SnakePoint(int x = 0, int y = 0) : x(x), y(y) {}
};

class SnakeGame
{
private:
    std::deque<SnakePoint> snake;
    SnakePoint apple;
    SnakePoint specialApple;
    SnakePoint pinkApple;
    int numHearts;
    Direction dir;
    bool gameOver;
    size_t gameScore;
    size_t highScore;
    bool isPaused;
    bool isInvincible;
    int64_t invincibilityEndTime;
    const int INVINCIBILITY_DURATION = 10;
    const int SUPERPOWER_DURATION = 20;
    int normalSpeed;


    void placeApple();
    void placeSpecialApple();
    void placePinkApple();
    bool isCollision(SnakePoint pt);


public:
    int MAP_WIDTH = 30;
    int MAP_HEIGHT = 20;
    int CELL_SIZE = 20;

    SnakeGame();
    ~SnakeGame();
    void update(int& snakeSpeed);
    void changeDirection(int key);
    void render(cv::Mat& frame);
    bool isGameOver() const { return gameOver; }
    void resetGame();
    void loadHighScore();
    void saveHighScore();
    void loseHeart();
    void drawHeart(cv::Mat& frame, cv::Point position);
    void drawCell(cv::Mat& frame, int x, int y, cv::Scalar color);

    bool isGamePaused() const { return isPaused; }
    void togglePause() { isPaused = !isPaused; }
    bool isAppleOnSnake(int x, int y);
    void buyLife();
    void buySuperPower(int& snakeSpeed);
    int getWindowWidth();
    int getWindowHeigth();
    
};
