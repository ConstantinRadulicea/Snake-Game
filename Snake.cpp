#include "Snake.h"

int windowWidth = WIDTH;
int windowHeight = HEIGHT;

SnakeGame::SnakeGame() : dir(RIGHT), gameOver(false), gameScore(0), highScore(0), isPaused(false), numHearts(1), normalSpeed(100)
{
    snake.push_back(SnakePoint(windowWidth / 2, windowHeight / 2));
    srand((unsigned)time(0));
    placeApple();
    specialApple = SnakePoint(-1, -1);
    pinkApple = SnakePoint(-1, -1);
    loadHighScore();
}

SnakeGame::~SnakeGame() {}

void SnakeGame::update(int& snakeSpeed) {
    if (gameOver) return;

    if (cv::getTickCount() > invincibilityEndTime) {
        snakeSpeed = this->normalSpeed; // Reset speed back to normal after superpower ends
    }

    if (isInvincible && cv::getTickCount() > invincibilityEndTime) {
        isInvincible = false;
    }

    // The next head position (direction) based on the pressed key
    SnakePoint head = snake.front();
    switch (dir) {
    case UP: head.y -= 1; break;
    case DOWN: head.y += 1; break;
    case LEFT: head.x -= 1; break;
    case RIGHT: head.x += 1; break;
    }

    if (isInvincible) {
        if (head.x < 0) {
            head.x = MAP_WIDTH - 1; // Teleport to the right
        }
        else if (head.x >= MAP_WIDTH) {
            head.x = 0; // Teleport to the left
        }

        if (head.y < 0) {
            head.y = MAP_HEIGHT - 1; // Teleport to the bottom
        }
        else if (head.y >= MAP_HEIGHT) {
            head.y = 0; // Teleport to the top
        }
    }
    else if (isCollision(head)) {
        loseHeart();
        return;
    }

    snake.push_front(head);

    if (head.x == apple.x && head.y == apple.y) {
        placeApple();
        //gameScore++;

        if (specialApple.x == -1 && specialApple.y == -1) {
            placeSpecialApple();
        }
        if (pinkApple.x == -1 && pinkApple.y == -1) {
            placePinkApple();
        }
    }
    else if (head.x == specialApple.x && head.y == specialApple.y) {
        //gameScore++;
        isInvincible = true;
        invincibilityEndTime = cv::getTickCount() + INVINCIBILITY_DURATION * cv::getTickFrequency();
        placeSpecialApple();
    }
    else if (head.x == pinkApple.x && head.y == pinkApple.y) {
        //gameScore++;
        if (numHearts < MAX_HARTS) {
            numHearts++;
        }
        placePinkApple();
    }
    else {
        snake.pop_back();
    }

    gameScore = (int)snake.size() - (int)1;
    if (gameScore > highScore) {
        highScore = gameScore;
        saveHighScore();
    }

}

void SnakeGame::changeDirection(int key) {
    switch (tolower(key)) {
    case 'w': if (dir != DOWN) dir = UP; break;
    case 'a': if (dir != RIGHT) dir = LEFT; break;
    case 's': if (dir != UP) dir = DOWN; break;
    case 'd': if (dir != LEFT) dir = RIGHT; break;
    }
}

void SnakeGame::drawCell(cv::Mat& frame, int x, int y, cv::Scalar color) {
    rectangle(frame, cv::Rect(x * CELL_SIZE, y * CELL_SIZE, CELL_SIZE, CELL_SIZE), color, cv::FILLED);
}

void SnakeGame::render(cv::Mat& frame) {
    frame = cv::Scalar(0, 0, 0);
    cv::Scalar snakeColor = isInvincible ? cv::Scalar(0, 255, 255) : cv::Scalar(0, 255, 0);

    if (gameOver) {
        putText(frame, "Game Over", cv::Point(windowWidth / 3, windowHeight / 2), cv::FONT_HERSHEY_SIMPLEX, 1, cv::Scalar(0, 0, 255), 2);
        return;
    }

    // "Drawing" the snake body
    for (auto& segment : snake) {
        this->drawCell(frame, segment.x, segment.y, snakeColor);
        //rectangle(frame, cv::Rect(segment.x, segment.y, CELL_SIZE, CELL_SIZE), snakeColor, cv::FILLED);
    }

    // "Drawing" the red apple
    this->drawCell(frame, apple.x, apple.y, cv::Scalar(0, 0, 255));
    //rectangle(frame, cv::Rect(apple.x, apple.y, CELL_SIZE, CELL_SIZE), cv::Scalar(0, 0, 255), cv::FILLED);

    // "Drawing" the golden apple
    if (specialApple.x != -1 && specialApple.y != -1) {
        this->drawCell(frame, specialApple.x, specialApple.y, cv::Scalar(0, 255, 255));
        //rectangle(frame, cv::Rect(specialApple.x, specialApple.y, CELL_SIZE, CELL_SIZE), cv::Scalar(0, 255, 255), cv::FILLED);
    }

    // "Drawing" the pink apple
    if (pinkApple.x != -1 && pinkApple.y != -1) {
        this->drawCell(frame, pinkApple.x, pinkApple.y, cv::Scalar(255, 105, 180));
        //rectangle(frame, cv::Rect(pinkApple.x, pinkApple.y, CELL_SIZE, CELL_SIZE), cv::Scalar(255, 105, 180), cv::FILLED);
    }

    // Drawing hearts
    for (int i = 0; i < numHearts; i++) {
        cv::Point heartPos(10 + i * 30, 50);
        drawHeart(frame, heartPos);
    }

    putText(frame, ("Score: " + std::to_string(gameScore)), cv::Point(10, 30), cv::FONT_HERSHEY_SIMPLEX, 0.7, cv::Scalar(255, 255, 255), 2);
    putText(frame, ("HighScore: " + std::to_string(highScore)), cv::Point(windowWidth - 160, 30), cv::FONT_HERSHEY_SIMPLEX, 0.7, cv::Scalar(0, 255, 255, 255), 2);

    if (isInvincible) {
        int remainingTime = static_cast<int>((invincibilityEndTime - cv::getTickCount()) / cv::getTickFrequency());
        if (remainingTime > 0) {
            putText(frame, "Invincible: " + std::to_string(remainingTime) + "s",
                cv::Point(10, windowHeight - 30), cv::FONT_HERSHEY_SIMPLEX, 0.7, cv::Scalar(0, 255, 255), 2);
        }
        else {
            isInvincible = false; // Deactivate the shown time
        }
    }

}

void SnakeGame::resetGame() {
    snake.clear();
    snake.push_back(SnakePoint(windowWidth / 2, windowHeight / 2));
    gameOver = false;
    gameScore = 0;
    dir = RIGHT;
    numHearts = MAX_HARTS;
    isInvincible = false;

    placeApple();
}

void SnakeGame::loadHighScore() {
    std::ifstream file(HIGH_SCORE_FILE);
    if (file.is_open()) {
        if (!(file >> highScore)) { // load the highscore value in the program
            highScore = 0;
        }
        else {
            std::cout << "Error HIGH_SCORE_FILE" << std::endl;
        }
    }

}

void SnakeGame::saveHighScore() {
    std::ofstream file(HIGH_SCORE_FILE);

    if (file.is_open()) {
        file << highScore; // save the highscore in the text file
        file.close();
    }
    else {
        std::cout << "\nNu s-a putut deschide fisierulul pentru SALVARE!" << std::endl;
    }
}

void SnakeGame::loseHeart() {
    if (isInvincible) {
        return; // not losing hearts
    }

    numHearts--;
    if (numHearts <= 0) {
        gameOver = true;
    }
    else {
        isInvincible = true;
        invincibilityEndTime = cv::getTickCount() + INVINCIBILITY_DURATION * cv::getTickFrequency();
    }
}

void SnakeGame::drawHeart(cv::Mat& frame, cv::Point position) {
    std::vector<cv::Point> heart;
    heart.push_back(cv::Point(position.x, position.y));
    heart.push_back(cv::Point(position.x + 20, position.y));
    heart.push_back(cv::Point(position.x + 10, position.y + 20));
    polylines(frame, heart, true, cv::Scalar(255, 105, 180), 2);
}

bool SnakeGame::isAppleOnSnake(int x, int y) {
    for (auto segment : snake) {
        if (segment.x == x && segment.y == y) {
            return true;
        }
    }
    return false;
}

void SnakeGame::buyLife() {
    if (highScore >= 5 && numHearts < MAX_HARTS) {
        numHearts++;
        highScore -= 5;
    }
}

void SnakeGame::buySuperPower(int& snakeSpeed) {
    if (numHearts > SUPERPOWER_HARTS_PRICE) {
        snakeSpeed = snakeSpeed - (int)((float)snakeSpeed * 0.3f);
        isInvincible = true;
        invincibilityEndTime = cv::getTickCount() + SUPERPOWER_DURATION * cv::getTickFrequency();
        numHearts -= SUPERPOWER_HARTS_PRICE;
    }
}




void SnakeGame::placeApple() {
    srand(time(0));
    if (windowWidth > 0 && windowHeight > 0) {
        apple.x = (rand() % MAP_WIDTH);
        apple.y = (rand() % MAP_HEIGHT);
    }
    else {
        apple.x = MAP_WIDTH / 2;
        apple.y = MAP_HEIGHT / 2;
    }
}

void SnakeGame::placeSpecialApple() {
    if (rand() % 100 >= 50) { // 50% to respawn
        specialApple.x = -1;
        specialApple.y = -1;
        return;
    }

    int x, y;
    do {
        x = (rand() % MAP_WIDTH);
        y = (rand() % MAP_HEIGHT);
    } while (isAppleOnSnake(x, y) || (x == apple.x && y == apple.y) || (x == pinkApple.x && y == pinkApple.y));
    specialApple.x = x;
    specialApple.y = y;
}

void SnakeGame::placePinkApple() {
    if (rand() % 100 >= 20) { // 20% to respawn
        pinkApple.x = -1;
        pinkApple.y = -1;
        return;
    }

    int x, y;
    do {
        x = (rand() % MAP_WIDTH);
        y = (rand() % MAP_HEIGHT);
    } while (isAppleOnSnake(x, y) || (x == apple.x && y == apple.y) || (x == specialApple.x && y == specialApple.y));

    pinkApple.x = x;
    pinkApple.y = y;
}

bool SnakeGame::isCollision(SnakePoint pt) {
    if (pt.x < 0 || pt.x >= MAP_WIDTH || pt.y < 0 || pt.y >= MAP_HEIGHT)
        return true;
    for (auto& segment : snake) {
        if (segment.x == pt.x && segment.y == pt.y)
            return true;
    }
    return false;
}

