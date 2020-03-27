#include"Snake2.h"

//第一关的宏
#define NORMAL_NUM 3
#define POI_NUM 3
#define MINE_NUM 4
#define SMART_NUM 1
#define ACC_NUM 1

#define TIME_LIMIT 240

#define POI_VANISH 50

//外部变量声明
extern Snake player;
extern Foods nof, pof, mif, door1, door2, inside_wall, acf;
extern IMAGE fill;
extern M mon;
extern long long grade;
extern int t1, t2, begin, now_time;
extern unsigned poi_times, loop;
extern FILE* load_fp1, * grade_fp1;
extern MenuState ms;
extern bool gun_exist, break_record;

void initial1();
void load1();

void create_ini_wall1();
void record1();
inline void pass_check();
void save_grade1();

void game1() {//生成第一关的新游戏
	initial1();
	while (player.state.is_live&&!player.state.is_pass) {
		loop++;
		if (player.speed == HIGH_SPEED)player.high_step--;
		if (pof.state && (loop > POI_VANISH) && (loop / POI_VANISH) % 3 == 0)clearfoods(&pof);
		else if (!pof.state && (loop / POI_VANISH) % 3 != 0) creatfs(&pof, POI, POI_R, POI_COLOR, POI_NUM);
		time_left();
		if (now_time <= 0) break;
		record1();
		getmsg(player.head->x, player.head->y, 1);
		if (ms.is_game1_exit)return;
		move();
		deal();
		pass_check();
		shining(pof, POI_COLOR, loop);
		Sleep(SLEEP_TIME);
	}
	grade =(long long)player.score * (long long)player.hp;
	save_grade1();
	record1();
	redraw(pof, POI_COLOR);
	reli();
	_getch();
	closegraph();
	end();
}

void load_game1() {//加载第一关
	load1();
	while (player.state.is_live && !player.state.is_pass) {
		loop++;
		if (player.speed == HIGH_SPEED)player.high_step--;
		if (pof.state && (loop > POI_VANISH) && (loop / POI_VANISH) % 3 == 0)clearfoods(&pof);
		else if (!pof.state && (loop / POI_VANISH) % 3 != 0) creatfs(&pof, POI, POI_R, POI_COLOR, POI_NUM);
		time_left();
		if (now_time <= 0) break;
		record1();
		getmsg(player.head->x, player.head->y, 1);
		if (ms.is_game1_exit)return;
		move();
		deal();
		pass_check();
		shining(pof, POI_COLOR, loop);
		Sleep(SLEEP_TIME);
	}
	grade = (long long)player.score * (long long)player.hp;
	save_grade1();
	record1();
	redraw(pof, POI_COLOR);
	reli();
	_getch();
	closegraph();
	end();
}

void initial1() {//初始化第一关
	initgraph(IMG_X, IMG_Y);
	setbkcolor(BK_COLOR);
	cleardevice();
	player._bullet = false;
	loop = 0;
	mon.exist = false;
	nof.state = EMPTY;
	pof.state = EMPTY;
	mif.state = EMPTY;
	door1.state = EMPTY;
	door2.state = EMPTY;
	inside_wall.state = EMPTY;
	acf.state = EMPTY;
	nof.head = NULL;
	pof.head = NULL;
	mif.head = NULL;
	door1.head = NULL;
	door2.head = NULL;
	inside_wall.head = NULL;
	acf.head = NULL;
	creatwall();
	player.head = creatplayer();
	JUDGE(player.head);
	create_ini_wall1();
	srand((unsigned)time(NULL));
	creatfs(&nof, NORMAL, NORMAL_R, NORMAL_COLOR, NORMAL_NUM);
	creatfs(&mif, MINE, MINE_R, MINE_COLOR, MINE_NUM);
	creatdoor(&door1, DOOR1_COLOR, DOOR1);
	creatdoor(&door2, DOOR2_COLOR, DOOR2);
	creatfs(&acf, ACC, ACC_R, ACC_COLOR, ACC_NUM);
	settextcolor(RED);
	gun_exist = false;
	outtextxy(EDGE_WALL_X + WALL_B + 2, 0, _T("Press any key to start"));
	_getch();
	setfillcolor(WHITE);
	solidrectangle(EDGE_WALL_X + WALL_B + 2, 0, IMG_X, 20);
	now_time = TIME_LIMIT;
	poi_times = 0;
	t2 = 0;
	t1 = begin = time();
}

void load1() {//第一关读档
	initgraph(IMG_X, IMG_Y);
	setbkcolor(BK_COLOR);
	cleardevice();
	fopen_s(&load_fp1, "snake2_save1.bin", "rb");
	JUDGE(load_fp1);

	fread(&now_time, sizeof(int), 1, load_fp1);
	fread(&loop, sizeof(unsigned), 1, load_fp1);
	fread(&poi_times, sizeof(unsigned), 1, load_fp1);

	creatwall();
	create_ini_wall1();

	reload_player(load_fp1);
	reload_foods(load_fp1, &nof, NORMAL_NUM);
	redraw(nof, NORMAL_COLOR);
	reload_foods(load_fp1, &mif, MINE_NUM);
	redraw(mif, MINE_COLOR);
	reload_foods(load_fp1, &acf, ACC_NUM);
	redraw(acf, ACC_COLOR);
	reload_foods(load_fp1, &door1, 2);
	redrawdoor(door1, DOOR1_COLOR);
	reload_foods(load_fp1, &door2, 2);
	redrawdoor(door2, DOOR2_COLOR);
	reload_foods(load_fp1, &pof, (POI_NUM + poi_times));
	redraw(pof, POI_COLOR);
	
	fclose(load_fp1);
	
	settextcolor(RED);
	outtextxy(EDGE_WALL_X + WALL_B + 2, 0, _T("Press any key to start"));
	_getch();
	setfillcolor(WHITE);
	solidrectangle(EDGE_WALL_X + WALL_B + 2, 0, IMG_X, 20);
	gun_exist = false;
	mon.exist = false;
	t2 = 0;
	t1 = begin = time();
}

void save1() {//第一关存档
	fopen_s(&load_fp1, "snake2_save1.bin", "wb");
	JUDGE(load_fp1);
	
	fwrite(&now_time, sizeof(int), 1, load_fp1);
	fwrite(&loop, sizeof(unsigned), 1, load_fp1);
	fwrite(&poi_times, sizeof(unsigned), 1, load_fp1);
	
	fwrite(&player, sizeof(Snake), 1, load_fp1);
	SN* p = player.head;
	while (p != NULL) {
		fwrite(&p->x, sizeof(int), 1, load_fp1);
		fwrite(&p->y, sizeof(int), 1, load_fp1);
		p = p->next;
	}
	
	FN* f;
	
	fwrite(&nof, sizeof(Foods), 1, load_fp1);
	f = nof.head;
	while (f != NULL) {
		fwrite(&f->x, sizeof(int), 1, load_fp1);
		fwrite(&f->y, sizeof(int), 1, load_fp1);
		fwrite(&f->is_empty, sizeof(bool), 1, load_fp1);
		f = f->next;
	}

	fwrite(&mif, sizeof(Foods), 1, load_fp1);
	f = mif.head;
	while (f != NULL) {
		fwrite(&f->x, sizeof(int), 1, load_fp1);
		fwrite(&f->y, sizeof(int), 1, load_fp1);
		fwrite(&f->is_empty, sizeof(bool), 1, load_fp1);
		f = f->next;
	}

	fwrite(&acf, sizeof(Foods), 1, load_fp1);
	f = acf.head;
	while (f != NULL) {
		fwrite(&f->x, sizeof(int), 1, load_fp1);
		fwrite(&f->y, sizeof(int), 1, load_fp1);
		fwrite(&f->is_empty, sizeof(bool), 1, load_fp1);
		f = f->next;
	}

	fwrite(&door1, sizeof(Foods), 1, load_fp1);
	f = door1.head;
	while (f != NULL) {
		fwrite(&f->x, sizeof(int), 1, load_fp1);
		fwrite(&f->y, sizeof(int), 1, load_fp1);
		fwrite(&f->is_empty, sizeof(bool), 1, load_fp1);
		f = f->next;
	}

	fwrite(&door2, sizeof(Foods), 1, load_fp1);
	f = door2.head;
	while (f != NULL) {
		fwrite(&f->x, sizeof(int), 1, load_fp1);
		fwrite(&f->y, sizeof(int), 1, load_fp1);
		fwrite(&f->is_empty, sizeof(bool), 1, load_fp1);
		f = f->next;
	}

	fwrite(&pof, sizeof(Foods), 1, load_fp1);
	f = pof.head;
	while (f != NULL) {
		fwrite(&f->x, sizeof(int), 1, load_fp1);
		fwrite(&f->y, sizeof(int), 1, load_fp1);
		fwrite(&f->is_empty, sizeof(bool), 1, load_fp1);
		f = f->next;
	}

	fclose(load_fp1);
}

void record1() {//记录并显示第一关的各种数据
	settextcolor(RED);
	TCHAR si[8];
	char s[8];
	int i;
	sprintf_s(s, "%d", player.hp);
	for (i = 0; *(s + i); i++)si[i] = s[i];
	si[i] = 0;
	outtextxy(EDGE_WALL_X + WALL_B + 2, 0, _T("Hp:"));
	outtextxy(EDGE_WALL_X + WALL_B + 42, 0, _T("      "));
	outtextxy(EDGE_WALL_X + WALL_B + 42, 0, si);
	sprintf_s(s, "%d", player.score);
	for (i = 0; *(s + i); i++)si[i] = s[i];
	si[i] = 0;
	outtextxy(EDGE_WALL_X + WALL_B + 2, 25, _T("Score:"));
	outtextxy(EDGE_WALL_X + WALL_B + 82, 25, _T("      "));
	outtextxy(EDGE_WALL_X + WALL_B + 82, 25, si);
	sprintf_s(s, "%d", player.accel_times);
	for (i = 0; *(s + i); i++)si[i] = s[i];
	si[i] = 0;
	outtextxy(EDGE_WALL_X + WALL_B + 2, 50, _T("Speed up times:"));
	outtextxy(EDGE_WALL_X + WALL_B + 122, 50, _T("         "));
	outtextxy(EDGE_WALL_X + WALL_B + 122, 50, si);
	sprintf_s(s, "%d", now_time);
	for (i = 0; *(s + i); i++)si[i] = s[i];
	si[i] = 's';
	si[i + 1] = 0;
	outtextxy(EDGE_WALL_X + WALL_B + 2, 75, _T("Time left:"));
	outtextxy(EDGE_WALL_X + WALL_B + 82, 75, _T("         "));
	outtextxy(EDGE_WALL_X + WALL_B + 82, 75, si);


	if (!player.state.is_live) {
		settextcolor(BLUE);
		outtextxy(EDGE_WALL_X + WALL_B + 2, 225, _T("Game over!"));
		outtextxy(EDGE_WALL_X + WALL_B + 2, 250, _T("You have died!"));
		outtextxy(EDGE_WALL_X + WALL_B + 2, 275, _T("Press any key to exit"));
	}
	else if (now_time <= 0) {
		settextcolor(BLUE);
		outtextxy(EDGE_WALL_X + WALL_B + 2, 225, _T("Game over!"));
		outtextxy(EDGE_WALL_X + WALL_B + 2, 250, _T("Time is out"));
		outtextxy(EDGE_WALL_X + WALL_B + 2, 275, _T("Press any key to exit"));
	}
	else if (player.state.is_pass) {
		settextcolor(BLUE);
		outtextxy(EDGE_WALL_X + WALL_B + 2, 225, _T("Congratulations!"));
		outtextxy(EDGE_WALL_X + WALL_B + 2, 250, _T("You win the game!"));
		outtextxy(EDGE_WALL_X + WALL_B + 2, 275, _T("Press any key to exit"));
	}
	
}

void create_ini_wall1() {//生成第一关内的墙
	FN* cur, * pre;
	inside_wall.type = WALL;
	inside_wall.r = INSIDE_WALL_R;
	inside_wall.state = FULL;
	inside_wall.head = cur = pre = (FN*)malloc(sizeof(FN));
	JUDGE(cur);
	cur->x = 200;
	cur->y = 100;
	cur->is_empty = false;
	cur->prev = cur->next = NULL;
	setfillcolor(WALL_COLOR);
	fillrectangle(cur->x - inside_wall.r, cur->y - inside_wall.r, cur->x + inside_wall.r, cur->y + inside_wall.r);
	int i;
	for (i = inside_wall.r * 2; i <= 80 * INSIDE_WALL_R; i += 2 * INSIDE_WALL_R) {
		cur = (FN*)malloc(sizeof(FN));
		JUDGE(cur);
		cur->prev = pre;
		pre->next = cur;
		cur->next = NULL;
		cur->is_empty = false;
		cur->x = 200;
		cur->y = 100 + i;
		fillrectangle(cur->x - inside_wall.r, cur->y - inside_wall.r, cur->x + inside_wall.r, cur->y + inside_wall.r);
		pre = cur;
	}
	for (i = inside_wall.r * 2; i <= 80 * INSIDE_WALL_R; i += 2 * INSIDE_WALL_R) {
		cur = (FN*)malloc(sizeof(FN));
		JUDGE(cur);
		cur->prev = pre;
		pre->next = cur;
		cur->next = NULL;
		cur->is_empty = false;
		cur->x = 800;
		cur->y = 100 + i;
		fillrectangle(cur->x - inside_wall.r, cur->y - inside_wall.r, cur->x + inside_wall.r, cur->y + inside_wall.r);
		pre = cur;
	}
	for (i = inside_wall.r * 2; i <= 60 * INSIDE_WALL_R; i += 2 * INSIDE_WALL_R) {
		cur = (FN*)malloc(sizeof(FN));
		JUDGE(cur);
		cur->prev = pre;
		pre->next = cur;
		cur->next = NULL;
		cur->is_empty = false;
		cur->x = 350 + i;
		cur->y = 150;
		fillrectangle(cur->x - inside_wall.r, cur->y - inside_wall.r, cur->x + inside_wall.r, cur->y + inside_wall.r);
		pre = cur;
	}
	for (i = inside_wall.r * 2; i <= 60 * INSIDE_WALL_R; i += 2 * INSIDE_WALL_R) {
		cur = (FN*)malloc(sizeof(FN));
		JUDGE(cur);
		cur->prev = pre;
		pre->next = cur;
		cur->next = NULL;
		cur->is_empty = false;
		cur->x = 350 + i;
		cur->y = 450;
		fillrectangle(cur->x - inside_wall.r, cur->y - inside_wall.r, cur->x + inside_wall.r, cur->y + inside_wall.r);
		pre = cur;
	}
	for (i = inside_wall.r * 2; i <= 20 * INSIDE_WALL_R; i += 2 * INSIDE_WALL_R) {
		cur = (FN*)malloc(sizeof(FN));
		JUDGE(cur);
		cur->prev = pre;
		pre->next = cur;
		cur->next = NULL;
		cur->is_empty = false;
		cur->x = 500;
		cur->y = 250 + i;
		fillrectangle(cur->x - inside_wall.r, cur->y - inside_wall.r, cur->x + inside_wall.r, cur->y + inside_wall.r);
		pre = cur;
	}
	for (i = inside_wall.r * 2; i <= 10 * INSIDE_WALL_R; i += 2 * INSIDE_WALL_R) {
		cur = (FN*)malloc(sizeof(FN));
		JUDGE(cur);
		cur->prev = pre;
		pre->next = cur;
		cur->next = NULL;
		cur->is_empty = false;
		cur->x = 350;
		cur->y = 285 + i;
		fillrectangle(cur->x - inside_wall.r, cur->y - inside_wall.r, cur->x + inside_wall.r, cur->y + inside_wall.r);
		pre = cur;
	}
	for (i = inside_wall.r * 2; i <= 10 * INSIDE_WALL_R; i += 2 * INSIDE_WALL_R) {
		cur = (FN*)malloc(sizeof(FN));
		JUDGE(cur);
		cur->prev = pre;
		pre->next = cur;
		cur->next = NULL;
		cur->is_empty = false;
		cur->x = 650;
		cur->y = 285 + i;
		fillrectangle(cur->x - inside_wall.r, cur->y - inside_wall.r, cur->x + inside_wall.r, cur->y + inside_wall.r);
		pre = cur;
	}
}

inline void pass_check() {//检查是否过关
	if (player.hp == 30)player.state.is_pass = 1;
}

void save_grade1() {//保存游戏成绩（不是分数）
	long long history[5];
	fopen_s(&grade_fp1, "snake2_grade1.bin", "rb");
	JUDGE(grade_fp1);
	fread(history, sizeof(long long), 5, grade_fp1);
	fclose(grade_fp1);
	long long temp1 = grade, temp2;
	int i;
	for (i = 0; i < 5; i++) {
		if (*(history + i) < temp1) {
			temp2 = temp1;
			temp1 = *(history + i);
			*(history + i) = temp2;
		}
	}
	fopen_s(&grade_fp1, "C:\\Users\\15102\\source\\repos\\贪吃蛇2\\贪吃蛇2\\snake2_grade1.bin", "wb");
	JUDGE(grade_fp1);
	fwrite(history, sizeof(long long), 5, grade_fp1);
	fclose(grade_fp1);
}