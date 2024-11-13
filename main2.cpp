#include <opencv2/opencv.hpp>
#include <iostream>
#include <ctime>
#include <deque>
#include <cstdlib>

using namespace cv;
using namespace std;

const int WIDTH = 600;
const int HEIGHT = 400;
const int CELL_SIZE = 20;

enum Direction { UP, DOWN, LEFT, RIGHT };

enum GameStates { MENU, PLAYING, OPTIONS, EXIT, GAME_OVER };

struct SnakePoint{
    int x, y;
    SnakePoint(int x = 0, int y = 0) : x(x), y(y) {}
};

class SnakeGame
{
private:
    deque<SnakePoint> snake;
    SnakePoint apple;
    Direction dir;
    bool gameOver;
    size_t gameScore;  

    void placeApple() {
        apple.x = (rand() % (WIDTH / CELL_SIZE)) * CELL_SIZE;
        apple.y = (rand() % (HEIGHT / CELL_SIZE)) * CELL_SIZE;
    }

    bool isCollision(SnakePoint pt) {
        if (pt.x < 0 || pt.x >= WIDTH || pt.y < 0 || pt.y >= HEIGHT)
            return true;
        for (auto& segment : snake) {
            if (segment.x == pt.x && segment.y == pt.y)
                return true;
        }
        return false;
    }

public:
	SnakeGame();
	~SnakeGame();
    void update();
    void changeDirection(int key);
    void render(Mat& frame);
    bool isGameOver() const { return gameOver; }
    void resetGame();
};

SnakeGame::SnakeGame() : dir(RIGHT), gameOver(false), gameScore(0)
{
    snake.push_back(SnakePoint(WIDTH / 2, HEIGHT / 2));
    srand((unsigned)time(0));
    placeApple();
}

SnakeGame::~SnakeGame() {}

void SnakeGame::update() {
    if (gameOver) return;

    SnakePoint head = snake.front();
    switch (dir) {
    case UP: head.y -= CELL_SIZE; break;
    case DOWN: head.y += CELL_SIZE; break;
    case LEFT: head.x -= CELL_SIZE; break;
    case RIGHT: head.x += CELL_SIZE; break;
    }

    if (isCollision(head)) {
        gameOver = true;
        return;
    }

    snake.push_front(head);

    if (head.x == apple.x && head.y == apple.y) {
        placeApple();
        gameScore++;
    }
    else {
        snake.pop_back();
    }
}

void SnakeGame::changeDirection(int key) {
    switch (key) {
    case 'w': if (dir != DOWN) dir = UP; break;
    case 'a': if (dir != RIGHT) dir = LEFT; break;
    case 's': if (dir != UP) dir = DOWN; break;
    case 'd': if (dir != LEFT) dir = RIGHT; break;
    }
}

void SnakeGame::render(Mat &frame) {
    frame = Scalar(0, 0, 0);

    if (gameOver) {
        putText(frame, "Game Over", Point(WIDTH / 3, HEIGHT / 2), FONT_HERSHEY_SIMPLEX, 1, Scalar(0, 0, 255), 2);
        return;
    }

    for (auto& segment : snake) {
        rectangle(frame, Rect(segment.x, segment.y, CELL_SIZE, CELL_SIZE), Scalar(0, 255, 0), FILLED);
    }

    rectangle(frame, Rect(apple.x, apple.y, CELL_SIZE, CELL_SIZE), Scalar(0, 0, 255), FILLED);

    putText(frame, ("Score: " + to_string(gameScore)), Point(10, 30), FONT_HERSHEY_SIMPLEX, 0.7, Scalar(255, 255, 255, 1));
}

void SnakeGame::resetGame() {
    snake.clear();
    snake.push_back(SnakePoint(WIDTH / 2, HEIGHT / 2));
    gameOver = false;
    dir = RIGHT;
    placeApple();
}



void showMenu(Mat& frame, int& selectedOption);
void handleMenuInput(int key, int &selectedOption, GameStates &currentState, SnakeGame& game);

void showGameOverMenu(Mat& frame, int& selectedOption);
void handleGameOverMenu(int key, int& selectedOption, GameStates& currentState, SnakeGame& game);



int main() {
    SnakeGame game;
    Mat frame(HEIGHT, WIDTH, CV_8UC3);
    namedWindow("Snake Game", WINDOW_AUTOSIZE);

    GameStates currentState = MENU;
    int selectedOption = 0;
    
    int64_t gameOverTimeStamp = 0;

    while (currentState != EXIT) 
    {
        int key = waitKey(100);

        switch (currentState)
        {
        case MENU:
            showMenu(frame, selectedOption);
            handleMenuInput(key, selectedOption, currentState, game);
            break;

        case PLAYING:
            if (key != -1) game.changeDirection(key);
            game.update();
            game.render(frame);

            if (game.isGameOver()) {
                currentState = GAME_OVER;
                gameOverTimeStamp = getTickCount();
                selectedOption = 0;
            }
            break;

        case OPTIONS:
            break;

        case EXIT:
            break;
        
        case GAME_OVER:
            game.render(frame);
            if ((getTickCount() - gameOverTimeStamp) / getTickFrequency() >= 3) {
                showGameOverMenu(frame, selectedOption);
                handleGameOverMenu(key, selectedOption, currentState, game);
            }
            break;
        
        }

        imshow("Snake Game", frame);
    }

    return 0;
}



void showMenu(Mat &frame, int &selectedOption) {
    vector<string> menuOptions = { "Start the Game", "Options", "Exit" };
    frame = Scalar(0, 0, 0);

    for (size_t i = 0; i < menuOptions.size(); i++) {
        Scalar color = (i == selectedOption) ? Scalar(0, 255, 0) : Scalar(255, 255, 255);
        putText(frame, menuOptions[i], Point(WIDTH / 3, 100 + i * 40), FONT_HERSHEY_SIMPLEX, 1, color, 2);
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

void showGameOverMenu(Mat& frame, int& selectedOption) {
    vector<string> gameOverMenuOptions = { "Retry", "Back to Menu" };
    frame = Scalar(0, 0, 0);

    for (size_t i = 0; i < gameOverMenuOptions.size(); i++) {
        Scalar color = (i == selectedOption) ? Scalar(0, 255, 0) : Scalar(255, 255, 255);
        putText(frame, gameOverMenuOptions[i], Point(WIDTH / 3, HEIGHT / 2 + i * 40), FONT_HERSHEY_SIMPLEX, 1, color, 2);
    }

}

void handleGameOverMenu(int key, int& selectedOption, GameStates& currentState, SnakeGame& game) {

    if (key == 'w' && selectedOption > 0) selectedOption--;
    if (key == 's' && selectedOption < 1) selectedOption ++;

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


