#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <conio.h>
#include <windows.h>
#include <wincon.h>
#include <time.h>
#include <stdlib.h>

WCHAR window_name[] = L"Snake by BadBoy";
/*
Black        |   0
Blue         |   1
Green        |   2
Cyan         |   3
Red          |   4
Magenta      |   5
Brown        |   6
Light Gray   |   7
Dark Gray    |   8
Light Blue   |   9
Light Green  |   10
Light Cyan   |   11
Light Red    |   12
Light Magenta|   13
Yellow       |   14
White        |   15
*/
#define UP 87        ///W
#define DOWN 83      ///S
#define RIGHT 68     ///D
#define LEFT 65      ///A

#define UP_2 119       ///w
#define DOWN_2 115     ///s
#define RIGHT_2 100    ///d
#define LEFT_2 97      ///a

#define MAX_FOOD 10
/*
#define UP 72
#define DOWN 80
#define RIGHT 77
#define LEFT 75
*/
int head_x, head_y;
int sleep_x = 150, sleep_y = 150, sleep_global = 100;
unsigned int max_x = 119, max_y = 30, min_y = 3;
int last_x = 0, last_y = 0;
int x = 2, y = 0;
unsigned int head_x_1 = 61, head_x_2 = 60, head_y_1 = 15, head_y_2 = 15;
char food_token = ' ';
char head_token_U = ' ', head_token_D = ' ', head_token_R = ' ', head_token_L = ' ', head_token = ' ', chage_token = ' ';
int head_color = 16, tail_color = 32, food_color = 581, chage_color = 8177, eaten_food_color = 12, background_color_def = 0;//8177
char tail_token = ' ';
int new_x = 0, new_y = 0;
int blocks_pos[1000][4];
int total_blocks = 1;
int food_pos[MAX_FOOD][4];
int total_food = 0;
bool continue_program = 1;

/// -32 72 UP
/// -32 80 DOWN
/// -32 77 RIGHT
/// -32 75 LEFT
/********************************************************************************/
void super_user_menu();
void change_speed_menu();
void change_window_res_menu();
void print_window_res_menu();
void change_window_res(int width, int hight);
void restart_game();
void print_score();
void re_draw_screen();
void print_settings_menu();
void settings_menu();
void game_lost();
void collision();
void refresh_snake();
int eat_food();
void draw_grid();
void spawn_food(int index);
void move_snake();
void get_input();
void get_window_res();
void window_title(WCHAR title[]);
void SetConsoleColour(DWORD Colour);
void SetColor(int ForgC);
void gotoxy(int x, int y);
void clrscr();

/********************************************************************************/
int main() {
    get_window_res();
    SetConsoleColour(background_color_def);
    clrscr();
    /*for(int x=0; x<10000; x++){
        gotoxy(0,x);
        printf("%d",x);
        SetConsoleColour(x);
        gotoxy(10,x);
        printf(" ");
    }*/

    if ((max_x / 2) % 2) head_x_1 = max_x / 2 + 1;
    else head_x_1 = max_x / 2;

    head_y_1 = (unsigned int)max_y / 2;
    head_x_2 = head_x_1 - 1;
    head_y_2 = head_y_1;
    window_title(window_name);
    blocks_pos[0][0] = head_x_1;
    blocks_pos[0][1] = head_y_1;
    blocks_pos[0][2] = head_x_2;
    blocks_pos[0][3] = head_y_2;
    gotoxy(5, 1);
    SetColor(255);
    printf("Eaten Food: ");
    print_score();
    gotoxy(head_x_1, head_y_1);
    SetColor(head_color);
    SetConsoleColour(head_color);
    printf("%c", head_token);
    gotoxy(head_x_2, head_y_2);
    printf("%c", head_token);
    draw_grid();
    srand((unsigned int)time(0));
    int start_clock, end_clock;
    int timer = 0;
    while (continue_program) {
        start_clock = clock();
        if (total_food < MAX_FOOD) {
            spawn_food(total_food);
            total_food++;
        }
        get_input();
        move_snake();
        collision();
        end_clock = clock();
        if ((timer = sleep_global + ((start_clock - end_clock) / CLOCKS_PER_SEC) * 1000) >= 0) Sleep(timer);
    }
}
/********************************************************************************/
void super_user_menu() {
    clrscr();
    gotoxy(0, 0);
    printf("SuperUser menu\n\n");
    printf("Starting tail length: %d\n\n", total_blocks);
    printf("1) Change starting tail length\n\n");
    printf("2) Return to settings\n");
    char input_c[4] = {};
    while (1) {
        Sleep(20);
        if (_kbhit()) {
            switch (_getch()) {
            case 49:
                gotoxy(30, 2);
                printf("New tail length: ");
                scanf("%s", input_c);
                break;
            case 50:
                if (strlen(input_c)) total_blocks = atoi(input_c);
                print_settings_menu();
                return;
                break;
            case 27:
                if (strlen(input_c)) total_blocks = atoi(input_c);
                print_settings_menu();
                return;
                break;
            default:
                break;

            }
        }
    }
}
/********************************************************************************/
void change_speed_menu() {
    clrscr();
    gotoxy(0, 0);
    printf("Change Speed\n\n");
    printf("Time sleep x  ms: %d\n", sleep_x);
    printf("Time sleep y  ms: %d\n\n", sleep_y);
    printf("1)Change sleep x\n");
    printf("2)Change sleep y\n\n");
    printf("3)Return to settings\n");
    char speed_x_s[4] = {};
    char speed_y_s[4] = {};
    while (1) {
        Sleep(20);
        if (_kbhit()) {
            switch (_getch()) {
            case 49:
                gotoxy(25, 2);
                printf("New sleep x : ");
                scanf("%s", speed_x_s);
                break;
            case 50:
                gotoxy(25, 3);
                printf("New sleep y : ");
                scanf("%s", speed_y_s);
                break;
            case 51:
                if (strlen(speed_x_s)) sleep_x = atoi(speed_x_s);
                if (strlen(speed_y_s)) sleep_y = atoi(speed_y_s);
                print_settings_menu();
                return;
            case 27:
                if (strlen(speed_x_s)) sleep_x = atoi(speed_x_s);
                if (strlen(speed_y_s)) sleep_y = atoi(speed_y_s);
                print_settings_menu();
                return;
            default:
                break;
            }
        }
    }
}
/********************************************************************************/
void print_window_res_menu() {
    clrscr();
    gotoxy(0, 0);
    printf("Change window resolution\n\n");
    printf("Screen width: %d\n", max_x);
    printf("Screen hight: %d\n\n", max_y);
    printf("1)Change width\n");
    printf("2)Change hight\n\n");
    printf("3)Change resolution to window resolution\n\n");
    printf("4)Return to settings\n");
}
/********************************************************************************/
void change_window_res_menu() {
    print_window_res_menu();
    char width_s[4] = {};
    char hight_s[4] = {};
    int width_i = 0, hight_i = 0;
    while (1) {
        Sleep(20);
        if (_kbhit()) {
            switch (_getch()) {
            case 49:
                gotoxy(20, 2);
                printf("New width : ");
                scanf("%s", width_s);
                break;
            case 50:
                gotoxy(20, 3);
                printf("New hight : ");
                scanf("%s", hight_s);
                break;
            case 51:
                get_window_res();
                print_window_res_menu();
                break;
            case 52:
                width_i = atoi(width_s);
                hight_i = atoi(hight_s);
                change_window_res(width_i, hight_i);
                if (width_i > 0) max_x = width_i;
                if (hight_i > 0) max_y = hight_i;
                print_settings_menu();
                return;
            case 27:
                width_i = atoi(width_s);
                hight_i = atoi(hight_s);
                change_window_res(width_i, hight_i);
                if (width_i > 0) max_x = width_i;
                if (hight_i > 0) max_y = hight_i;
                print_settings_menu();
                return;
            default:
                break;
            }
        }
    }
}
/********************************************************************************/
void change_window_res(int width, int hight) {
    COORD coord;
    if (width > 0) coord.X = width; else coord.X = max_x;
    if (hight > 0) coord.Y = hight; else coord.Y = max_y;


    SMALL_RECT Rect;
    Rect.Top = 0;
    Rect.Left = 0;
    Rect.Bottom = hight - 1;
    Rect.Right = width - 1;

    HANDLE Handle = GetStdHandle(STD_OUTPUT_HANDLE);      // Get Handle
    SetConsoleScreenBufferSize(Handle, coord);            // Set Buffer Size
    SetConsoleWindowInfo(Handle, TRUE, &Rect);            // Set Window Size
}
/********************************************************************************/
void restart_game() {
    x = 2;
    y = 0;
    last_x = 0;
    last_y = 0;
    total_blocks = 1;
    total_food = 0;
    SetConsoleColour(background_color_def);
    clrscr();

    if ((max_x / 2) % 2) head_x_1 = max_x / 2 + 1;
    else head_x_1 = max_x / 2;

    head_y_1 = (unsigned int)max_y / 2;
    head_x_2 = head_x_1 - 1;
    head_y_2 = head_y_1;
    blocks_pos[0][0] = head_x_1;
    blocks_pos[0][1] = head_y_1;
    blocks_pos[0][2] = head_x_2;
    blocks_pos[0][3] = head_y_2;
    gotoxy(5, 1);
    SetColor(255);
    printf("Eaten Food: ");
    print_score();
    gotoxy(head_x_1, head_y_1);
    SetColor(head_color);
    SetConsoleColour(head_color);
    printf("%c", head_token);
    gotoxy(head_x_2, head_y_2);
    printf("%c", head_token);
    draw_grid();
}
/********************************************************************************/
void print_score() {
    gotoxy(5 + 12, 1);
    SetConsoleColour(eaten_food_color);
    printf("%d", total_blocks - 1);
}
/********************************************************************************/
void re_draw_screen() {
    SetConsoleColour(background_color_def);
    clrscr();
    blocks_pos[0][0] = head_x_1;
    blocks_pos[0][1] = head_y_1;
    blocks_pos[0][2] = head_x_2;
    blocks_pos[0][3] = head_y_2;
    gotoxy(5, 1);
    SetColor(255);
    printf("Eaten Food: ");
    print_score();
    gotoxy(head_x_1, head_y_1);
    SetConsoleColour(head_color);
    printf("%c", head_token);
    gotoxy(head_x_2, head_y_2);
    printf("%c", head_token);
    draw_grid();
    SetConsoleColour(tail_color);
    for (int j = 1; j < total_blocks; j++) {
        gotoxy(blocks_pos[j][0], blocks_pos[j][1]);
        printf("%c", tail_token);
        gotoxy(blocks_pos[j][2], blocks_pos[j][3]);
        printf("%c", tail_token);
    }
    SetConsoleColour(food_color);
    for (int j = 0; j < total_food; j++) {
        gotoxy(food_pos[j][0], food_pos[j][1]);
        printf("%c", food_token);
        gotoxy(food_pos[j][2], food_pos[j][3]);
        printf("%c", food_token);
    }
    gotoxy(0, 0);
}
/********************************************************************************/
void print_settings_menu() {
    SetConsoleColour(10);
    clrscr();
    gotoxy(0, 0);
    SetColor(255);
    printf("1) Resume\n");
    printf("2) Change Speed\n");
    printf("3) Change window resolution\n");
    printf("4) SuperUser menu\n");
    printf("5) Restart\n");
    printf("\n6) Quit game\n");
}
/********************************************************************************/
void settings_menu() {
    print_settings_menu();
    while (1) {
        Sleep(20);
        if (_kbhit()) {
            switch (_getch()) {
            case 49:
                continue_program = 1;
                re_draw_screen();
                return;
            case 50:
                change_speed_menu();
                break;
            case 53:
                continue_program = 1;
                restart_game();
                return;
            case 51:
                change_window_res_menu();
                break;
            case 52:
                super_user_menu();
                print_settings_menu();
                break;
            case 54:
                continue_program = 0;
                exit(0);
                return;
            case 27:
                continue_program = 1;
                re_draw_screen();
                return;
            default:
                break;
            }
        }
    }
}
/********************************************************************************/
void game_lost() {
    SetConsoleColour(chage_color);
    gotoxy(0, max_y / 2 - 3);
    for (int j = 0; j < max_x; j++) {
        printf("%c", chage_token);
    }
    gotoxy(0, max_y / 2 + 3);
    for (int j = 0; j < max_x; j++) {
        printf("%c", chage_token);
    }
    char game_lost_text[] = "GAME OVER";
    gotoxy(max_x / 2 - strlen(game_lost_text) / 2, max_y / 2);
    SetConsoleColour(65);
    printf("%s", game_lost_text);
    gotoxy(max_x / 2 - strlen("Press any key") / 2, max_y / 2 + 6);
    printf("Press any key");
    x = 0;
    y = 0;
    last_x = 0;
    last_y = 0;
    _getch();
}
/********************************************************************************/
void collision() {
    if (head_x_1 <= 0 || head_x_2 <= 0 || head_x_1 >= max_x || head_x_2 >= max_x || head_y_1 <= min_y || head_y_2 <= min_y || head_y_1 >= max_y - 1 || head_y_2 >= max_y - 1) {
        game_lost();
        continue_program = 0;
        _getch();
        settings_menu();
        return;
    }
    for (int x = 1; x < total_blocks; x++) {
        if (head_x_1 == blocks_pos[x][0] && head_y_1 == blocks_pos[x][1] && head_x_2 == blocks_pos[x][2]) {
            SetConsoleColour(food_color);
            /*
            gotoxy(20,0);
            printf("(%d : %d) (%d : %d)   block: %d", head_x_2, head_y_2, head_x_1, head_y_1, x);
            gotoxy(20,1);
            printf("(%d : %d) (%d : %d)", blocks_pos[x][2], blocks_pos[x][3], blocks_pos[x][0], blocks_pos[x][1]);
            */
            game_lost();
            continue_program = 0;
            _getch();
            settings_menu();
        }
    }
}
/********************************************************************************/
void refresh_snake() {
    SetConsoleColour(background_color_def);
    if (!eat_food()) {
        gotoxy(blocks_pos[total_blocks - 1][0], blocks_pos[total_blocks - 1][1]);
        printf(" ");
        gotoxy(blocks_pos[total_blocks - 1][2], blocks_pos[total_blocks - 1][3]);
        printf(" ");
    }
    if (total_blocks > 1) {
        SetConsoleColour(tail_color);
        gotoxy(blocks_pos[0][0], blocks_pos[0][1]);
        //SetColor(tail_color);
        printf("%c", tail_token);
        gotoxy(blocks_pos[0][2], blocks_pos[0][3]);
        printf("%c", tail_token);
    }
    //else{return;}

    for (int x = total_blocks - 1; x > 0; x--) {
        blocks_pos[x][0] = blocks_pos[x - 1][0];
        blocks_pos[x][1] = blocks_pos[x - 1][1];
        blocks_pos[x][2] = blocks_pos[x - 1][2];
        blocks_pos[x][3] = blocks_pos[x - 1][3];
    }
}
/********************************************************************************/
int eat_food() {
    for (int x = 0; x < total_food; x++) {
        if ((head_x_1 == food_pos[x][0] && head_y_1 == food_pos[x][1]) || (blocks_pos[0][2] == food_pos[x][2] && blocks_pos[0][3] == food_pos[x][3])) {
            blocks_pos[total_blocks][0] = blocks_pos[total_blocks - 1][0];
            blocks_pos[total_blocks][1] = blocks_pos[total_blocks - 1][1];
            blocks_pos[total_blocks][2] = blocks_pos[total_blocks - 1][2];
            blocks_pos[total_blocks][3] = blocks_pos[total_blocks - 1][3];
            total_blocks++;
            print_score();
            spawn_food(x);
            return 1;
        }
    }
    return 0;
}
/********************************************************************************/
void draw_grid() {
    SetConsoleColour(chage_color);
    gotoxy(0, min_y);
    for (int x = 0; x < max_x; x++) {
        printf("%c", chage_token);
    }
    gotoxy(0, max_y - 1);
    for (int x = 0; x < max_x; x++) {
        printf("%c", chage_token);
    }
    for (int x = min_y; x < max_y; x++) {
        gotoxy(0, x);
        printf("%c", chage_token);
        gotoxy(max_x, x);
        printf("%c", chage_token);
    }
    gotoxy(0, 0);
}
/********************************************************************************/
void spawn_food(int index) {
    int x_1, y_1, x_2, y_2;
    time_t seconds = time(0);
    while ((x_1 = (rand() % (max_x - 2)) + 2) % 2) {
        seconds++;
        srand(seconds);
    }
    y_1 = (rand() % (max_y - 2 - min_y)) + 1 + min_y;
    x_2 = x_1 - 1;
    y_2 = y_1;
    int j = 0;
    bool blocks_check = 0, food_check = 0;
    while (1) {
        for (j = 0; j < total_blocks; j++) {
            if (x_1 == blocks_pos[j][0] && y_1 == blocks_pos[j][1]) {
                seconds++;
                srand(seconds);
                while ((x_1 = (rand() % (max_x - 2)) + 2) % 2) {
                    seconds++;
                    srand(seconds);
                }
                y_1 = (rand() % (max_y - 2 - min_y)) + 1 + min_y;
                x_2 = x_1 - 1;
                y_2 = y_1;
                food_check = 0;
                j = 0;
            }
        }
        blocks_check = 1;
        for (j = 0; j < total_food; j++) {
            if (x_1 == food_pos[j][0] && y_1 == food_pos[j][1]) {
                seconds++;
                srand(seconds);
                while ((x_1 = (rand() % (max_x - 2)) + 2) % 2) {
                    seconds++;
                    srand(seconds);
                }
                y_1 = (rand() % (max_y - 2 - min_y)) + 1 + min_y;
                x_2 = x_1 - 1;
                y_2 = y_1;
                j = 0;
                blocks_check = 0;
            }
        }
        food_check = 1;
        if (blocks_check && food_check) {
            food_pos[index][0] = x_1;
            food_pos[index][1] = y_1;
            food_pos[index][2] = x_2;
            food_pos[index][3] = y_2;
            SetConsoleColour(food_color);
            gotoxy(food_pos[index][0], food_pos[index][1]);
            printf("%c", food_token);
            gotoxy(food_pos[index][2], food_pos[index][3]);
            printf("%c", food_token);
            return;
        }
    }
}
/********************************************************************************/
void move_snake() {
    refresh_snake();
    head_x_1 += x;
    head_y_1 += y;
    head_x_2 += x;
    head_y_2 += y;
    SetConsoleColour(head_color);
    gotoxy(head_x_1, head_y_1);
    SetColor(head_color);
    printf("%c", head_token);
    gotoxy(head_x_2, head_y_2);
    printf("%c", head_token);
    blocks_pos[0][0] = head_x_1;
    blocks_pos[0][1] = head_y_1;
    blocks_pos[0][2] = head_x_2;
    blocks_pos[0][3] = head_y_2;

}
/********************************************************************************/
void get_input() {
    if (_kbhit()) {
        last_x = x;
        last_y = y;
        short input_c;
        input_c = _getch();
        switch (input_c) {
        case UP:
            y = -1;
            x = 0;
            sleep_global = sleep_y;
            head_token = head_token_U;
            break;
        case UP_2:
            y = -1;
            x = 0;
            sleep_global = sleep_y;
            head_token = head_token_U;
            break;

        case DOWN:
            y = 1;
            x = 0;
            sleep_global = sleep_y;
            head_token = head_token_D;
            break;
        case DOWN_2:
            y = 1;
            x = 0;
            sleep_global = sleep_y;
            head_token = head_token_D;
            break;

        case RIGHT:
            x = 2;
            y = 0;
            sleep_global = sleep_x;
            head_token = head_token_R;
            break;
        case RIGHT_2:
            x = 2;
            y = 0;
            sleep_global = sleep_x;
            head_token = head_token_R;
            break;

        case LEFT:
            x = -2;
            y = 0;
            sleep_global = sleep_x;
            head_token = head_token_L;
            break;
        case LEFT_2:
            x = -2;
            y = 0;
            sleep_global = sleep_x;
            head_token = head_token_L;
            break;
        case 27:
            settings_menu();
            break;
        default:
            break;
        }
        if (x && last_x && last_x != x) x = last_x;
        else if (y && last_y && last_y != y) y = last_y;
    }
}
/********************************************************************************/
void get_window_res() {
    HANDLE hStdOut = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_SCREEN_BUFFER_INFO csbi;

    //We use csbi for the wAttributes word.
    if (GetConsoleScreenBufferInfo(hStdOut, &csbi)) {
        max_x = csbi.srWindow.Right - csbi.srWindow.Left;
        max_y = csbi.srWindow.Bottom - csbi.srWindow.Top + 1;
    }
    return;
}

const wchar_t* GetWC(const char* c)
{
    const size_t cSize = strlen(c) + 1;
    wchar_t* wc = new wchar_t[cSize];
    mbstowcs(wc, c, cSize);

    return wc;
}
/********************************************************************************/
void window_title(WCHAR title[]) {
    HANDLE hStdOut = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    //We use csbi for the wAttributes word.
    if (GetConsoleScreenBufferInfo(hStdOut, &csbi)) {
        SetConsoleTitle(title);
        max_x = csbi.srWindow.Right - csbi.srWindow.Left;
        max_y = csbi.srWindow.Bottom - csbi.srWindow.Top + 1;
    }
    return;
}
/********************************************************************************/
void clrscr()
{
    system("@cls||clear");
}
/********************************************************************************/
void ResetConsoleColour() {
    WORD Attributes = 0;
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), Attributes);
}
/********************************************************************************/
void SetConsoleColour(DWORD Colour) {
    CONSOLE_SCREEN_BUFFER_INFO Info;
    HANDLE hStdout = GetStdHandle(STD_OUTPUT_HANDLE);
    GetConsoleScreenBufferInfo(hStdout, &Info);
    SetConsoleTextAttribute(hStdout, Colour);
}
/********************************************************************************/
void SetColor(int ForgC) {
    WORD wColor;

    HANDLE hStdOut = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_SCREEN_BUFFER_INFO csbi;

    //We use csbi for the wAttributes word.
    if (GetConsoleScreenBufferInfo(hStdOut, &csbi)) {
        //Mask out all but the background attribute, and add in the forgournd     color
        wColor = (csbi.wAttributes & 0xF0) + (ForgC & 0x0F);
        SetConsoleTextAttribute(hStdOut, wColor);
    }
    return;
}
/********************************************************************************/
void gotoxy(int x, int y) {
    COORD coord;
    coord.X = x;
    coord.Y = y;
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
}
/********************************************************************************/
