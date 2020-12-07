#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <curses.h>
#include <unistd.h>
struct point {
  int x;
  int y;
} snake[256];
typedef struct point point;
const point dirUp = {0, -1};
const point dirDown = {0, 1};
const point dirLeft = {-1, 0};
const point dirRight = {1, 0};
point apple;
int length = 3;
int width = 78;
int height = 22;
point direction;
point newDir;
int run = 1;
int ch;

void init(void);
void moveSnake(void);
void moveApple(void);
int checkLoss(void);
int touchingSnake(int, int);
void draw(void);
void update(void);
int directionOpposite(point, point);

int main() {
  srand(time(0));
  //curses initialization
  initscr();
  cbreak();
  noecho();
  keypad(stdscr, TRUE);
  nodelay(stdscr, TRUE);
  curs_set(0);

  init();
  while(run) {
    update();
    usleep(100000);
  }
  clear();
  refresh();
  endwin();
  return 0;
}

void init() {
  direction = dirRight;
  for (int i = 0; i < length; i++) {
    snake[i].x = width/2-i;
    snake[i].y = height/2;
  }
  moveApple();
}

void moveSnake() {
  for (int i = length; i > 0; i--) {
    snake[i] = snake[i-1];
  }
  snake[0].x += direction.x;
  snake[0].y += direction.y;
}

void moveApple() {
  do {
    apple.x = rand() % (width - 2) + 1;
    apple.y = rand() % (height - 2) + 1;
  } while (touchingSnake(apple.x, apple.y) || apple.x < 0 || apple.x >= width || apple.y < 0 || apple.y >= height);
}

void update() {
  if((ch = getch()) != ERR) {
    switch (ch) {
      case KEY_UP:
        newDir = dirUp;
        break;
      case KEY_DOWN:
        newDir = dirDown;
        break;
      case KEY_LEFT:
        newDir  = dirLeft;
        break;
      case KEY_RIGHT:
        newDir = dirRight;
        break;
      case 'Q':
        run = 0;
        break;
    }
    if(!directionOpposite(direction, newDir)) {
      direction = newDir;
    }
  }
  moveSnake();
  draw();
  if(snake[0].x == apple.x && snake[0].y == apple.y) {
    length++;
    moveApple();
  }
  if(checkLoss()) {
    run = 0;
  }
}

int checkLoss() {
  if (snake[0].x >= width || snake[0].x < 0 || snake[0].y >= height || snake[0].y < 0) {
    return 1;
  }
  for (int i = 1; i < length; i++) {
    if (snake[i].x == snake[0].x && snake[i].y == snake[0].y) {
      return 1;
    }
  }
  return 0;
}

void draw() {
  clear();
  //draw board
  for(int i = 0; i <= width+1; i++) {
    mvaddch(0, i, '#');
    mvaddch(height+1, i, '#');
  }
  for(int i = 1; i <= height; i++) {
    mvaddch(i, 0, '#');
    mvaddch(i, width+1, '#');
  }
  //draw apple
  mvaddch(apple.y+1, apple.x+1, '@');
  //draw snake
  for(int i = 0; i < length; i++) {
    mvaddch(snake[i].y+1, snake[i].x+1, '$');
  }
}

int touchingSnake(int x, int y) {
  for(int i = 0; i < length; i++) {
    if (snake[i].x == x && snake[i].y == y) {
      return 1;
    }
  }
  return 0;
}

int directionOpposite(point a, point b) {
  if(a.x == -b.x && a.y == -b.y) {
    return 1;
  }
  return 0;
}
