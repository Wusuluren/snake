/*
************************************
* wsad控制上下左右，q退出，空格暂停
* 编译命令 gcc snake.c -lcurses
* 可能需要下载ncurses库，具体方法请百度
* 发现bug请在贴吧@微笑一日
*************************************
*/
/*
*************************************
*how to play: w:up, s:down, a:left, d:right, q:quit, space:pause
*compile : gcc snake.c -lcurses
*you may need curses library,please google it
*if you find bugs,mail me at weixiaoyiri@gmali.com
*************************************
*/


#include <unistd.h>
#include <signal.h>
#include <stdio.h>
#include <curses.h>
#include <stdlib.h>
#include <sys/time.h>

typedef struct snake {
	int x, y;
	struct snake *next, *prev;
}SNAKE, *Snake;

typedef struct food {
	int x, y;
}FOOD, *Food;

int a;
int dir_x, dir_y;
Snake head, tail;
FOOD food;

int set_ticker(int n_msec)
{
	struct itimerval timeset;
	long n_sec, n_usec;

	n_sec = n_msec / 1000;
	n_usec = (n_msec % 1000) * 1000L;

	timeset.it_interval.tv_sec = n_sec;
	timeset.it_interval.tv_usec = n_usec;

	timeset.it_value.tv_sec = n_sec;
	timeset.it_value.tv_usec = n_usec;

	return setitimer(ITIMER_REAL, &timeset, NULL);
}

void Snake_Move()
{
	Snake p, tmp;

	for(p = tail; p != head; p = p->prev) {
		p->x = p->prev->x;
		p->y = p->prev->y;
	}
	p->x += dir_x;
	p->y += dir_y;

	if(head->x > 79)
		head->x = 0;
	if(head->x < 0)
		head->x = 79;
	if(head->y > 23)
		head->y = 0;
	if(head->y < 0)
		head->y = 23;

	move(head->y, head->x);
	if((char)inch() == '*') { //eat self
		Game_Over();
	}
	if((char)inch() == 'o') { //eat food
		move(head->y, head->x);
		addch('*');
		refresh();
		
		tmp = (Snake)malloc(sizeof(SNAKE));
		tmp->x = head->x + dir_x;
		tmp->y = head->y + dir_y;
		tmp->next = head;
		head->prev = tmp;
		head = tmp;

		do {
			food.x = rand() % 80;
			food.y = rand() % 24;
			move(food.y, food.x);
		}while(((char)inch()) == '*');
		move(food.y, food.x);
		addch('o');
		refresh();
	}

	move(head->y, head->x);
	addch('*');
	refresh();
	move(tail->y, tail->x);
	addch(' ');
	refresh();
}

int Game_Over()
{
	sleep(1);
	endwin();
	exit(0);
}

void key_ctl()
{
	int ch = getch();
	switch(ch) {
		case 'W':
		case 'w':
			if(dir_x == 0)
				break;
			dir_x = 0;
			dir_y = -1;
			break;
		case 'S':
		case 's':
			if(dir_x == 0)
				break;
			dir_x = 0;
			dir_y = 1;
			break;
		case 'A':
		case 'a':
			if(dir_y == 0)
				break;
			dir_y = 0;
			dir_x = -1;
			break;
		case 'D':
		case 'd':
			if(dir_y == 0)
				break;
			dir_y = 0;
			dir_x = 1;
			break;
		case ' ':
			set_ticker(0);
			do {
				ch = getch();
			}while(ch != ' ');
			set_ticker(500);
			break;
		case 'Q':
		case 'q':
			Game_Over();
			break;
		default:break;
	}
}

void Init()
{
	initscr();
	cbreak();
	noecho();
	curs_set(0);
	srand(time(0));
	
	dir_x = 1;
	dir_y = 0;

	head = (Snake)malloc(sizeof(SNAKE));
	head->x = rand() % 80;
	head->y = rand() % 24;
	head->next = (Snake)malloc(sizeof(SNAKE));
	tail = head->next;
	tail->prev = head;
	tail->x = head->x - dir_x;
	tail->y = head->y - dir_y;

	do {
		food.x = rand() % 80;
		food.y = rand() % 24;
		move(food.y, food.x);
	}while((char)inch() == '*');

	move(head->y, head->x);
	addch('*');
	move(food.y, food.x);
	addch('o');
	refresh();
}

void sig_alrm(int singo)
{
	set_ticker(500);
	Snake_Move();
}

int main()
{
	char ch;

	Init();
	signal(SIGALRM, sig_alrm);
	set_ticker(500);

	while(1) {
	key_ctl();
	}
	endwin();
	return 0;
}
