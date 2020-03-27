#include"Snake2.h"

#define NORMAL_NUM 4
#define POI_NUM 5
#define MINE_NUM 7
#define SMART_NUM 1
#define ACC_NUM 3
#define POI_VANISH 60
#define MON_X 800
#define MON_Y 400
#define MON_SPEED 5
#define MON_ANGRY_SPEED 8
#define EYE_D 16
#define EYE_A (PI/3)
#define TIME_LIMIT 640
#define INI_MON_HP 20

extern Snake player;
extern Foods nof, pof, mif, door1, door2, inside_wall, acf;
extern IMAGE fill;
extern M mon;
extern Bullet bullet;
extern long long grade;
extern int t1, t2, begin, now_time;
extern unsigned poi_times, loop;
extern FILE* load_fp2, * grade_fp2;
extern MenuState ms;
extern bool gun_exist;

bool is_mon_foods(Foods);
void drawmonster(M, int, int);
void creatmonster();

void checkmonster();
void movemonster();
void initial2();
void load2();
void record2();
void game2();
void reli();
void save_grade2();

void game2(){//生成第二关新游戏
	initial2();
	while (player.state.is_live && !player.state.is_pass) {
		loop++;
		if (player.high_step)player.high_step--;
		if (pof.state && (loop > POI_VANISH) && (loop / POI_VANISH) % 3 == 0)clearfoods(&pof);
		else if (!pof.state && (loop / POI_VANISH) % 3 != 0) creatfs(&pof, POI, POI_R, POI_COLOR, POI_NUM);
		time_left();
		if (now_time <= 0) break;
		record2();
		getmsg(player.head->x, player.head->y, 2);
		if (ms.is_game2_exit)return;
		movebullet();
		redraw(mif, MINE_COLOR);
		redraw(nof, NORMAL_COLOR);
		redraw(acf, ACC_COLOR);
		redrawdoor(door1, DOOR1_COLOR);
		redrawdoor(door2, DOOR2_COLOR);
		creatwall();
		movemonster();
		move();
		deal();
		shining(pof, POI_COLOR, loop);
		Sleep(SLEEP_TIME);
	}
	grade = (long long)player.score * (long long)player.hp;
	save_grade2();
	clearcircle(mon.x, mon.y, MON_R);
	if (mon.hp > 5)drawmonster(mon, MON_COLOR, MON_EYE_COLOR);
	else drawmonster(mon, MON_COLOR, MON_ANGRY_EYE_COLOR);
	record2();
	redraw(pof, POI_COLOR);
	reli();
	_getch();
	closegraph();
	end();
}

void load_game2(){//加载第二关的游戏
	load2();
	while (player.state.is_live && !player.state.is_pass) {
		loop++;
		if (player.high_step)player.high_step--;
		if (pof.state && (loop > POI_VANISH) && (loop / POI_VANISH) % 3 == 0)clearfoods(&pof);
		else if (!pof.state && (loop / POI_VANISH) % 3 != 0) creatfs(&pof, POI, POI_R, POI_COLOR, POI_NUM);
		time_left();
		if (now_time <= 0) break;
		record2();
		getmsg(player.head->x, player.head->y, 2);
		if (ms.is_game2_exit)return;
		movebullet();
		redraw(mif, MINE_COLOR);
		redraw(nof, NORMAL_COLOR);
		redraw(acf, ACC_COLOR);
		redrawdoor(door1, DOOR1_COLOR);
		redrawdoor(door2, DOOR2_COLOR);
		creatwall();
		movemonster();
		move();
		deal();
		shining(pof, POI_COLOR, loop);
		Sleep(SLEEP_TIME);
	}
	grade = (long long)player.score * (long long)player.hp;
	save_grade2();
	clearcircle(mon.x, mon.y, MON_R);
	if (mon.hp > 5)drawmonster(mon, MON_COLOR, MON_EYE_COLOR);
	else drawmonster(mon, MON_COLOR, MON_ANGRY_EYE_COLOR);
	record2();
	redraw(pof, POI_COLOR);
	reli();
	_getch();
	closegraph();
	end();
}

void load2() {//第二关读档
	initgraph(IMG_X, IMG_Y);
	setbkcolor(BK_COLOR);
	cleardevice();
	fopen_s(&load_fp2, "snake2_save2.bin", "rb");
	JUDGE(load_fp2);

	fread(&now_time, sizeof(int), 1, load_fp2);
	fread(&loop, sizeof(unsigned), 1, load_fp2);
	fread(&poi_times, sizeof(unsigned), 1, load_fp2);

	creatwall();

	fread(&mon, sizeof(M), 1, load_fp2);
	if (mon.hp > 5)drawmonster(mon, MON_COLOR, MON_EYE_COLOR);
	else drawmonster(mon, MON_COLOR, MON_ANGRY_EYE_COLOR);

	fread(&bullet, sizeof(Bullet), 1, load_fp2);
	reload_bullet(load_fp2, bullet.num);

	reload_player(load_fp2);
	reload_foods(load_fp2, &nof, NORMAL_NUM);
	redraw(nof, NORMAL_COLOR);
	reload_foods(load_fp2, &mif, MINE_NUM);
	redraw(mif, MINE_COLOR);
	reload_foods(load_fp2, &acf, ACC_NUM);
	redraw(acf, ACC_COLOR);
	reload_foods(load_fp2, &door1, 2);
	redrawdoor(door1, DOOR1_COLOR);
	reload_foods(load_fp2, &door2, 2);
	redrawdoor(door2, DOOR2_COLOR);
	reload_foods(load_fp2, &pof, (POI_NUM + poi_times));
	redraw(pof, POI_COLOR);
	
	fclose(load_fp2);

	settextcolor(RED);
	outtextxy(EDGE_WALL_X + WALL_B + 2, 0, _T("Press any key to start"));
	_getch();
	setfillcolor(WHITE);
	solidrectangle(EDGE_WALL_X + WALL_B + 2, 0, IMG_X, 20);
	gun_exist = false;
	t2 = 0;
	t1 = begin = time();
}

void save2() {//第二关存档
	fopen_s(&load_fp2, "snake2_save2.bin", "wb");
	JUDGE(load_fp2);

	fwrite(&now_time, sizeof(int), 1, load_fp2);
	fwrite(&loop, sizeof(unsigned), 1, load_fp2);
	fwrite(&poi_times, sizeof(unsigned), 1, load_fp2);

	fwrite(&mon, sizeof(M), 1, load_fp2);
	
	fwrite(&bullet, sizeof(Bullet), 1, load_fp2);
	BN* b = bullet.head->next;
	while (b != NULL) {
		fwrite(&b->x, sizeof(int), 1, load_fp2);
		fwrite(&b->y, sizeof(int), 1, load_fp2);
		fwrite(&b->xd, sizeof(double), 1, load_fp2);
		fwrite(&b->yd, sizeof(double), 1, load_fp2);
		fwrite(&b->is_hit, sizeof(bool), 1, load_fp2);
		b = b->next;
	}

	fwrite(&player, sizeof(Snake), 1, load_fp2);
	SN* p = player.head;
	while (p != NULL) {
		fwrite(&p->x, sizeof(int), 1, load_fp2);
		fwrite(&p->y, sizeof(int), 1, load_fp2);
		p = p->next;
	}

	FN* f;

	fwrite(&nof, sizeof(Foods), 1, load_fp2);
	f = nof.head;
	while (f != NULL) {
		fwrite(&f->x, sizeof(int), 1, load_fp2);
		fwrite(&f->y, sizeof(int), 1, load_fp2);
		fwrite(&f->is_empty, sizeof(bool), 1, load_fp2);
		f = f->next;
	}

	fwrite(&mif, sizeof(Foods), 1, load_fp2);
	f = mif.head;
	while (f != NULL) {
		fwrite(&f->x, sizeof(int), 1, load_fp2);
		fwrite(&f->y, sizeof(int), 1, load_fp2);
		fwrite(&f->is_empty, sizeof(bool), 1, load_fp2);
		f = f->next;
	}

	fwrite(&acf, sizeof(Foods), 1, load_fp2);
	f = acf.head;
	while (f != NULL) {
		fwrite(&f->x, sizeof(int), 1, load_fp2);
		fwrite(&f->y, sizeof(int), 1, load_fp2);
		fwrite(&f->is_empty, sizeof(bool), 1, load_fp2);
		f = f->next;
	}

	fwrite(&door1, sizeof(Foods), 1, load_fp2);
	f = door1.head;
	while (f != NULL) {
		fwrite(&f->x, sizeof(int), 1, load_fp2);
		fwrite(&f->y, sizeof(int), 1, load_fp2);
		fwrite(&f->is_empty, sizeof(bool), 1, load_fp2);
		f = f->next;
	}

	fwrite(&door2, sizeof(Foods), 1, load_fp2);
	f = door2.head;
	while (f != NULL) {
		fwrite(&f->x, sizeof(int), 1, load_fp2);
		fwrite(&f->y, sizeof(int), 1, load_fp2);
		fwrite(&f->is_empty, sizeof(bool), 1, load_fp2);
		f = f->next;
	}

	fwrite(&pof, sizeof(Foods), 1, load_fp2);
	f = pof.head;
	while (f != NULL) {
		fwrite(&f->x, sizeof(int), 1, load_fp2);
		fwrite(&f->y, sizeof(int), 1, load_fp2);
		fwrite(&f->is_empty, sizeof(bool), 1, load_fp2);
		f = f->next;
	}
	

	fclose(load_fp2);
}

void initial2() {//初始化第二关
	initgraph(IMG_X, IMG_Y);
	setbkcolor(BK_COLOR);
	cleardevice();
	loop = 0;
	player._bullet = true;
	bullet.head = bullet.tail = (BN*)malloc(sizeof(BN));
	JUDGE(bullet.head);
	bullet.head->next = NULL;
	bullet.head->prev = NULL;
	bullet.exist = false;
	bullet.speed = BULLET_SPEED;
	bullet.num = 0;
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
	creatmonster();
	JUDGE(player.head);
	srand((unsigned)time(NULL));
	creatfs(&nof, NORMAL, NORMAL_R, NORMAL_COLOR, NORMAL_NUM);
	creatfs(&mif, MINE, MINE_R, MINE_COLOR, MINE_NUM);
	creatdoor(&door1, DOOR1_COLOR, DOOR1);
	creatfs(&acf, ACC, ACC_R, ACC_COLOR, ACC_NUM);
	settextcolor(RED);
	outtextxy(EDGE_WALL_X + WALL_B + 2, 0, _T("Press any key to start"));
	_getch();
	setfillcolor(WHITE);
	solidrectangle(EDGE_WALL_X + WALL_B + 2, 0, IMG_X, 20);
	gun_exist = false;
	poi_times = 0;
	t1 = t2 = 0;
	begin = time();
	now_time = TIME_LIMIT;
}

void record2() {//记录并显示第二关的各种数据
	settextcolor(RED);
	TCHAR si[7];
	char s[7];
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
	sprintf_s(s, "%d", mon.hp);
	for (i = 0; *(s + i); i++)si[i] = s[i];
	si[i] = 0;
	outtextxy(EDGE_WALL_X + WALL_B + 2, 75, _T("Monster's hp:"));
	outtextxy(EDGE_WALL_X + WALL_B + 122, 75, _T("         "));
	outtextxy(EDGE_WALL_X + WALL_B + 122, 75, si);
	sprintf_s(s, "%d", now_time);
	for (i = 0; *(s + i); i++)si[i] = s[i];
	si[i] = 's';
	si[i + 1] = 0;
	outtextxy(EDGE_WALL_X + WALL_B + 2, 100, _T("Time left:"));
	outtextxy(EDGE_WALL_X + WALL_B + 82, 100, _T("         "));
	outtextxy(EDGE_WALL_X + WALL_B + 82, 100, si);

	if (!player.hp) {
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
		outtextxy(EDGE_WALL_X + WALL_B + 2, 225, _T("The monster is defeated!"));
		outtextxy(EDGE_WALL_X + WALL_B + 2, 250, _T("Congratulations!"));
		outtextxy(EDGE_WALL_X + WALL_B + 2, 275, _T("Press any key to exit"));
	}
}

void creatmonster() {//生成怪物
	mon.exist = true;
	mon.hp = INI_MON_HP;
	mon.speed = MON_SPEED;
	mon.xd = -1;
	mon.yd = 0;
	mon.x = MON_X;
	mon.y = MON_Y;
	mon.state = { 0,1,0 };
	drawmonster(mon, MON_COLOR, MON_EYE_COLOR);
}

void drawmonster(M mon,int color,int eye_color) {//画怪物
	setfillcolor(color);
	int temp;
	if (mon.yd > 0)temp = -1;
	else temp = 1;
	solidpie(mon.x - MON_R, mon.y - MON_R, mon.x + MON_R, mon.y + MON_R, temp * acos(mon.xd) + ANGLE, temp * acos(mon.xd) - ANGLE);
	double s, c;
	if (mon.xd >= 0 && mon.yd >= 0) {
		c = acos(mon.xd);
		s = asin(mon.yd);
	}
	else if (mon.xd < 0 && mon.yd >= 0) {
		c = acos(mon.xd);
		s = PI - asin(mon.yd);
	}
	else if (mon.xd < 0 && mon.yd < 0) {
		c = 2 * PI - acos(mon.xd);
		s = PI - asin(mon.yd);
	}
	else {
		c = 2 * PI - acos(mon.xd);
		s = 2 * PI + asin(mon.yd);
	}
	if (mon.state.is_live) {
		setfillcolor(eye_color);
		solidcircle(mon.x + EYE_D * cos(c + EYE_A), mon.y + EYE_D * sin(s + EYE_A), 3);
		setfillcolor(MON_R);
	}
	else {
		setlinecolor(eye_color);
		moveto(mon.x + EYE_D * cos(c + EYE_A), mon.y + EYE_D * sin(s + EYE_A));
		linerel(3, 3);
		moveto(mon.x + EYE_D * cos(c + EYE_A), mon.y + EYE_D * sin(s + EYE_A));
		linerel(-3, -3);
		moveto(mon.x + EYE_D * cos(c + EYE_A), mon.y + EYE_D * sin(s + EYE_A));
		linerel(3, -3);
		moveto(mon.x + EYE_D * cos(c + EYE_A), mon.y + EYE_D * sin(s + EYE_A));
		linerel(-3, 3);
		setlinecolor(WHITE);
	}
}

void movemonster() {//移动怪物
	double d;
	int eye;
	if (mon.hp > 5) {
		eye = MON_EYE_COLOR;
		mon.speed = MON_SPEED;
	}
	else {
		eye = MON_ANGRY_EYE_COLOR;
		mon.speed = MON_ANGRY_SPEED;
	}
	d = (double)sq_distance(mon.x, mon.y, player.head->x, player.head->y);
	d = sqrt(d);
	mon.xd = ((double)player.head->x - (double)mon.x) / d;
	mon.yd = ((double)player.head->y - (double)mon.y) / d;
	clearcircle(mon.x, mon.y, MON_R);
	mon.x += mon.xd * mon.speed;
	mon.y += mon.yd * mon.speed;
	drawmonster(mon, MON_COLOR, eye);
	checkmonster();
}

void checkmonster() {//检查并处理怪物的各种状态
	if (mon.state.is_shoten) {
		int eye;
		mon.hp--;
		player.score++;
		if (mon.hp > 5)	eye = MON_COLOR;
		else eye = MON_ANGRY_EYE_COLOR;
		drawmonster(mon, LIGHTRED, eye);
		mon.state.is_shoten = 0;
	}

	if (is_mon_foods(nof)) {
		nof.state = VACA;
		mon.hp++;
		if(player.score>0)player.score--;
		setfillcolor(MON_COLOR);
		fillcircle(mon.x, mon.y, MON_R);
	}
	if (is_mon_foods(pof)) {
		setfillcolor(MON_COLOR);
		fillcircle(mon.x, mon.y, MON_R);
		pof.state = VACA;
	}
	if (is_mon_foods(mif)) {
		mif.state = VACA;
		setfillcolor(MON_COLOR);
		fillcircle(mon.x, mon.y, MON_R);
	}
	if (is_mon_foods(door1)) {
		door1.state = EMPTY;
		clearrectangle(door1.head->x - door1.r, door1.head->y - door1.r, door1.head->x + door1.r, door1.head->y + door1.r);
		clearrectangle(door1.head->next->x - door1.r, door1.head->next->y - door1.r, door1.head->next->x + door1.r, door1.head->next->y + door1.r);
		free(door1.head->next);
		free(door1.head);
		door1.head = NULL;
		setfillcolor(MON_COLOR);
		fillcircle(mon.x, mon.y, MON_R);
	}
	if (is_mon_foods(door2)) {
		door2.state = EMPTY;
		clearrectangle(door2.head->x - door2.r, door2.head->y - door2.r, door2.head->x + door2.r, door2.head->y + door2.r);
		clearrectangle(door2.head->next->x - door2.r, door2.head->next->y - door2.r, door2.head->next->x + door2.r, door2.head->next->y + door2.r);
		free(door2.head->next);
		free(door2.head);
		door2.head = NULL;
		setfillcolor(MON_COLOR);
		fillcircle(mon.x, mon.y, MON_R);
	}
	if (is_mon_foods(acf)){
		if(player.score>0)player.score--;
		acf.state = VACA;
		setfillcolor(MON_COLOR);
		fillcircle(mon.x, mon.y, MON_R);
    }
	if (!mon.hp) {
		mon.state.is_live = 0;
		player.score += 60;
		player.state.is_pass = 1;
	}
}

bool is_mon_foods(Foods food) {//判断怪物是否吃到食物
	if (!food.state)return false;
	FN* p = food.head;
	int rd1, rd2;
	rd2 = (food.r + MON_R) * (food.r + MON_R);
	while (p != NULL) {
		rd1 = sq_distance(p->x, p->y, mon.x, mon.y);
		if (rd1 <= rd2 && !p->is_empty) {
			p->is_empty = true;
			if (food.type != WALL) {
				clearcircle(p->x, p->y, food.r);
				fillcircle(mon.x, mon.y, MON_R);
			}
			return true;
		}
		p = p->next;
	}
	return false;
}

void save_grade2() {//保存第二关成绩（不是分数）
	long long history[5];
	fopen_s(&grade_fp2, "snake2_grade2.bin", "rb");
	JUDGE(grade_fp2);
	fread(history, sizeof(long long), 5, grade_fp2);
	fclose(grade_fp2);
	long long temp1 = grade, temp2;
	int i;
	for (i = 0; i < 5; i++) {
		if (*(history + i) < temp1) {
			temp2 = temp1;
			temp1 = *(history + i);
			*(history + i) = temp2;
		}
	}
	fopen_s(&grade_fp2, "C:\\Users\\15102\\source\\repos\\贪吃蛇2\\贪吃蛇2\\snake2_grade2.bin", "wb");
	JUDGE(grade_fp2);
	fwrite(history, sizeof(long long), 5, grade_fp2);
	fclose(grade_fp2);
}