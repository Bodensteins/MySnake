#include"Snake2.h"

#define NORMAL_NUM 4
#define POI_NUM 5
#define MINE_NUM 6
#define SMART_NUM 1
#define ACC_NUM 2

#define TIME_LIMIT 360
#define GUN_SHOOT_LOOP 40
#define POI_VANISH 50
#define INI_GUN_HP 7

extern Snake player;
extern Foods nof, pof, mif, door1, door2, inside_wall, acf;
extern IMAGE fill;
extern Bullet bullet;
extern M mon;
extern long long grade;
extern int t1, t2, begin, now_time;
extern unsigned poi_times, loop;
extern FILE* load_fp3, * grade_fp3;
extern MenuState ms;
extern bool gun_exist;
Gun gun[4];

void creat_ini_wall3();
void creat_gun();
void move_gun(Gun*);
void check_gun(Gun*);
void initial3();
void record3();
void load3();
void draw_gun(Gun*, int);
bool check_pass();
void save_grade3();

void game3() {//第三关新游戏
	int i;
	initial3();
	while (player.state.is_live && !player.state.is_pass) {
		loop++;
		if (player.speed == HIGH_SPEED)player.high_step--;
		if (pof.state && (loop > POI_VANISH) && (loop / POI_VANISH) % 3 == 0)clearfoods(&pof);
		else if (!pof.state && (loop / POI_VANISH) % 3 != 0) creatfs(&pof, POI, POI_R, POI_COLOR, POI_NUM);
		time_left();
		if (now_time <= 0) break;
		record3();
		getmsg(player.head->x, player.head->y, 3);
		if (ms.is_game3_exit)return;
		movebullet();
		redraw(mif, MINE_COLOR);
		redraw(nof, NORMAL_COLOR);
		redraw(acf, ACC_COLOR);
		redrawdoor(door1, DOOR1_COLOR);
		redrawdoor(door2, DOOR2_COLOR);
		redraw_wall(inside_wall);
		creatwall();
		for (i = 0; i < 4; i++) {
			move_gun(gun + i);
			if ((gun + i)->is_live && loop % GUN_SHOOT_LOOP == 10 * i)(gun + i)->is_shoot = true;
			check_gun(gun + i);
		}
		move();
		deal();
		if (check_pass())player.state.is_pass = 1;
		shining(pof, POI_COLOR, loop);
		Sleep(SLEEP_TIME - 10);
	}
	grade = (long long)player.score * (long long)player.hp;
	save_grade3();
	record3();
	redraw(pof, POI_COLOR);
	reli();
	
	_getch();
	closegraph();
	end();
}

void load_game3() {//加载第三关的存档游戏
	int i;
	load3();
	while (player.state.is_live && !player.state.is_pass) {
		loop++;
		if (player.speed == HIGH_SPEED)player.high_step--;
		if (pof.state && (loop > POI_VANISH) && (loop / POI_VANISH) % 3 == 0)clearfoods(&pof);
		else if (!pof.state && (loop / POI_VANISH) % 3 != 0) creatfs(&pof, POI, POI_R, POI_COLOR, POI_NUM);
		time_left();
		if (now_time <= 0) break;
		record3();
		getmsg(player.head->x, player.head->y, 3);
		if (ms.is_game3_exit)return;
		movebullet();
		redraw(mif, MINE_COLOR);
		redraw(nof, NORMAL_COLOR);
		redraw(acf, ACC_COLOR);
		redrawdoor(door1, DOOR1_COLOR);
		redrawdoor(door2, DOOR2_COLOR);
		redraw_wall(inside_wall);
		creatwall();
		for (i = 0; i < 4; i++) {
			move_gun(&gun[i]);
			if (gun[i].is_live && loop % GUN_SHOOT_LOOP == 10 * i)gun[i].is_shoot = true;
			check_gun(&gun[i]);
		}
		move();
		deal();
		shining(pof, POI_COLOR, loop);
		Sleep(SLEEP_TIME - 10);
	}
	grade = (long long)player.score * (long long)player.hp;
	save_grade3();
	record3();
	redraw(pof, POI_COLOR);
	reli();
	_getch();
	closegraph();
	end();
}

void save3() {//第三关存档
	fopen_s(&load_fp3, "snake2_save3.bin", "wb");
	JUDGE(load_fp3);

	fwrite(&now_time, sizeof(int), 1, load_fp3);
	fwrite(&loop, sizeof(unsigned), 1, load_fp3);
	fwrite(&poi_times, sizeof(unsigned), 1, load_fp3);
	
	fwrite(&bullet, sizeof(Bullet), 1, load_fp3);
	BN* b = bullet.head->next;
	while (b != NULL) {
		fwrite(&b->x, sizeof(int), 1, load_fp3);
		fwrite(&b->y, sizeof(int), 1, load_fp3);
		fwrite(&b->xd, sizeof(double), 1, load_fp3);
		fwrite(&b->yd, sizeof(double), 1, load_fp3);
		fwrite(&b->is_hit, sizeof(bool), 1, load_fp3);
		b = b->next;
	}

	fwrite(gun, sizeof(Gun), 4, load_fp3);

	fwrite(&player, sizeof(Snake), 1, load_fp3);
	SN* p = player.head;
	while (p != NULL) {
		fwrite(&p->x, sizeof(int), 1, load_fp3);
		fwrite(&p->y, sizeof(int), 1, load_fp3);
		p = p->next;
	}

	FN* f;

	fwrite(&nof, sizeof(Foods), 1, load_fp3);
	f = nof.head;
	while (f != NULL) {
		fwrite(&f->x, sizeof(int), 1, load_fp3);
		fwrite(&f->y, sizeof(int), 1, load_fp3);
		fwrite(&f->is_empty, sizeof(bool), 1, load_fp3);
		f = f->next;
	}

	fwrite(&mif, sizeof(Foods), 1, load_fp3);
	f = mif.head;
	while (f != NULL) {
		fwrite(&f->x, sizeof(int), 1, load_fp3);
		fwrite(&f->y, sizeof(int), 1, load_fp3);
		fwrite(&f->is_empty, sizeof(bool), 1, load_fp3);
		f = f->next;
	}

	fwrite(&acf, sizeof(Foods), 1, load_fp3);
	f = acf.head;
	while (f != NULL) {
		fwrite(&f->x, sizeof(int), 1, load_fp3);
		fwrite(&f->y, sizeof(int), 1, load_fp3);
		fwrite(&f->is_empty, sizeof(bool), 1, load_fp3);
		f = f->next;
	}

	fwrite(&door1, sizeof(Foods), 1, load_fp3);
	f = door1.head;
	while (f != NULL) {
		fwrite(&f->x, sizeof(int), 1, load_fp3);
		fwrite(&f->y, sizeof(int), 1, load_fp3);
		fwrite(&f->is_empty, sizeof(bool), 1, load_fp3);
		f = f->next;
	}

	fwrite(&door2, sizeof(Foods), 1, load_fp3);
	f = door2.head;
	while (f != NULL) {
		fwrite(&f->x, sizeof(int), 1, load_fp3);
		fwrite(&f->y, sizeof(int), 1, load_fp3);
		fwrite(&f->is_empty, sizeof(bool), 1, load_fp3);
		f = f->next;
	}

	fwrite(&pof, sizeof(Foods), 1, load_fp3);
	f = pof.head;
	while (f != NULL) {
		fwrite(&f->x, sizeof(int), 1, load_fp3);
		fwrite(&f->y, sizeof(int), 1, load_fp3);
		fwrite(&f->is_empty, sizeof(bool), 1, load_fp3);
		f = f->next;
	}

	fclose(load_fp3);
}

void initial3() {//初始化第三关
	initgraph(IMG_X, IMG_Y);
	setbkcolor(BK_COLOR);
	cleardevice();
	player._bullet = true;
	gun_exist = true;
	mon.exist = false;
	loop = 0;
	bullet.head = bullet.tail = (BN*)malloc(sizeof(BN));
	JUDGE(bullet.head);
	bullet.head->next = NULL;
	bullet.head->prev = NULL;
	bullet.exist = false;
	bullet.speed = BULLET_SPEED;
	bullet.num = 0;
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
	creat_gun();
	creat_ini_wall3();
	srand((unsigned)time(NULL));
	creatfs(&nof, NORMAL, NORMAL_R, NORMAL_COLOR, NORMAL_NUM);
	creatfs(&mif, MINE, MINE_R, MINE_COLOR, MINE_NUM);
	creatdoor(&door1, DOOR1_COLOR, DOOR1);
	creatdoor(&door2, DOOR2_COLOR, DOOR2);
	creatfs(&acf, ACC, ACC_R, ACC_COLOR, ACC_NUM);
	settextcolor(RED);
	outtextxy(EDGE_WALL_X + WALL_B + 2, 0, _T("Press any key to start"));
	_getch();
	setfillcolor(WHITE);
	solidrectangle(EDGE_WALL_X + WALL_B + 2, 0, IMG_X, 20);
	now_time = TIME_LIMIT;
	poi_times = 0;
	t2 = 0;
	t1 = begin = (int)time();
}

void load3() {//第三关读档
	fopen_s(&load_fp3, "snake2_save3.bin", "rb");
	JUDGE(load_fp3);
	
	initgraph(IMG_X, IMG_Y);
	setbkcolor(BK_COLOR);
	cleardevice();
	creatwall();
	creat_ini_wall3();


	fread(&now_time, sizeof(int), 1, load_fp3);
	fread(&loop, sizeof(unsigned), 1, load_fp3);
	fread(&poi_times, sizeof(unsigned), 1, load_fp3);


	fread(&bullet, sizeof(Bullet), 1, load_fp3);
	reload_bullet(load_fp3, bullet.num);

	int i;
	fread(gun, sizeof(Gun), 4, load_fp3);
	for (i = 0; i < 4; i++) {
		if ((gun + i)->is_live)draw_gun(gun + i, GUN_COLOR);
		else draw_gun(gun + i, DEAD_GUN_COLOR);
	}

	reload_player(load_fp3);
	reload_foods(load_fp3, &nof, NORMAL_NUM);
	redraw(nof, NORMAL_COLOR);
	reload_foods(load_fp3, &mif, MINE_NUM);
	redraw(mif, MINE_COLOR);
	reload_foods(load_fp3, &acf, ACC_NUM);
	redraw(acf, ACC_COLOR);
	reload_foods(load_fp3, &door1, 2);
	redrawdoor(door1, DOOR1_COLOR);
	reload_foods(load_fp3, &door2, 2);
	redrawdoor(door2, DOOR2_COLOR);
	reload_foods(load_fp3, &pof, (POI_NUM + poi_times));
	redraw(pof, POI_COLOR);

	fclose(load_fp3);

	settextcolor(RED);
	outtextxy(EDGE_WALL_X + WALL_B + 2, 0, _T("Press any key to start"));
	_getch();
	setfillcolor(WHITE);
	solidrectangle(EDGE_WALL_X + WALL_B + 2, 0, IMG_X, 20);
	mon.exist = false;
	gun_exist = true;
	t2 = 0;
	t1 = begin = time();
}

void record3() {//记录并显示第三关的数据
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
	sprintf_s(s, "%d", now_time);
	for (i = 0; *(s + i); i++)si[i] = s[i];
	si[i] = 's';
	si[i + 1] = 0;
	outtextxy(EDGE_WALL_X + WALL_B + 2, 75, _T("Time left:"));
	outtextxy(EDGE_WALL_X + WALL_B + 82, 75, _T("         "));
	outtextxy(EDGE_WALL_X + WALL_B + 82, 75, si);
	sprintf_s(s, "%d", gun[0].hp);
	for (i = 0; *(s + i); i++)si[i] = s[i];
	si[i] = 0;
	outtextxy(EDGE_WALL_X + WALL_B + 2, 100, _T("Gun1's hp:"));
	outtextxy(EDGE_WALL_X + WALL_B + 122, 100, _T("         "));
	outtextxy(EDGE_WALL_X + WALL_B + 122, 100, si);
	sprintf_s(s, "%d", gun[1].hp);
	for (i = 0; *(s + i); i++)si[i] = s[i];
	si[i] = 0;
	outtextxy(EDGE_WALL_X + WALL_B + 2, 125, _T("Gun2's hp:"));
	outtextxy(EDGE_WALL_X + WALL_B + 122, 125, _T("         "));
	outtextxy(EDGE_WALL_X + WALL_B + 122, 125, si);
	sprintf_s(s, "%d", gun[2].hp);
	for (i = 0; *(s + i); i++)si[i] = s[i];
	si[i] = 0;
	outtextxy(EDGE_WALL_X + WALL_B + 2, 150, _T("Gun3's hp:"));
	outtextxy(EDGE_WALL_X + WALL_B + 122, 150, _T("         "));
	outtextxy(EDGE_WALL_X + WALL_B + 122, 150, si);
	sprintf_s(s, "%d", gun[3].hp);
	for (i = 0; *(s + i); i++)si[i] = s[i];
	si[i] = 0;
	outtextxy(EDGE_WALL_X + WALL_B + 2, 175, _T("Gun4's hp:"));
	outtextxy(EDGE_WALL_X + WALL_B + 122, 175, _T("         "));
	outtextxy(EDGE_WALL_X + WALL_B + 122, 175, si);

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
		outtextxy(EDGE_WALL_X + WALL_B + 2, 225, _T("You have won the game!"));
		outtextxy(EDGE_WALL_X + WALL_B + 2, 250, _T("Congratulations!"));
		outtextxy(EDGE_WALL_X + WALL_B + 2, 275, _T("Press any key to exit"));
	}
}

void creat_ini_wall3() {//生成第三关的内墙
	FN* cur, * pre;
	inside_wall.type = WALL;
	inside_wall.r = INSIDE_WALL_R;
	inside_wall.state = FULL;
	inside_wall.head = cur = pre = (FN*)malloc(sizeof(FN));
	JUDGE(cur);
	cur->x = 500;
	cur->y = 300;
	cur->is_empty = false;
	cur->prev = cur->next = NULL;
	setfillcolor(WALL_COLOR);
	fillrectangle(cur->x - inside_wall.r, cur->y - inside_wall.r, cur->x + inside_wall.r, cur->y + inside_wall.r);
	int i;
	for (i = inside_wall.r * 2; i <= 16 * INSIDE_WALL_R; i += 2 * INSIDE_WALL_R) {
		cur = (FN*)malloc(sizeof(FN));
		JUDGE(cur);
		cur->prev = pre;
		pre->next = cur;
		cur->next = NULL;
		cur->is_empty = false;
		cur->x = 500;
		cur->y = 300 + i;
		fillrectangle(cur->x - inside_wall.r, cur->y - inside_wall.r, cur->x + inside_wall.r, cur->y + inside_wall.r);
		pre = cur;
	}
	for (i = inside_wall.r * 2; i <= 16 * INSIDE_WALL_R; i += 2 * INSIDE_WALL_R) {
		cur = (FN*)malloc(sizeof(FN));
		JUDGE(cur);
		cur->prev = pre;
		pre->next = cur;
		cur->next = NULL;
		cur->is_empty = false;
		cur->x = 500;
		cur->y = 300 - i;
		fillrectangle(cur->x - inside_wall.r, cur->y - inside_wall.r, cur->x + inside_wall.r, cur->y + inside_wall.r);
		pre = cur;
	}
	for (i = inside_wall.r * 2; i <= 16 * INSIDE_WALL_R; i += 2 * INSIDE_WALL_R) {
		cur = (FN*)malloc(sizeof(FN));
		JUDGE(cur);
		cur->prev = pre;
		pre->next = cur;
		cur->next = NULL;
		cur->is_empty = false;
		cur->x = 500 - i;
		cur->y = 300;
		fillrectangle(cur->x - inside_wall.r, cur->y - inside_wall.r, cur->x + inside_wall.r, cur->y + inside_wall.r);
		pre = cur;
	}
	for (i = inside_wall.r * 2; i <= 16 * INSIDE_WALL_R; i += 2 * INSIDE_WALL_R) {
		cur = (FN*)malloc(sizeof(FN));
		JUDGE(cur);
		cur->prev = pre;
		pre->next = cur;
		cur->next = NULL;
		cur->is_empty = false;
		cur->x = 500 + i;
		cur->y = 300;
		fillrectangle(cur->x - inside_wall.r, cur->y - inside_wall.r, cur->x + inside_wall.r, cur->y + inside_wall.r);
		pre = cur;
	}
	for (i = inside_wall.r * 2; i <= 16 * INSIDE_WALL_R; i += 2 * INSIDE_WALL_R) {
		cur = (FN*)malloc(sizeof(FN));
		JUDGE(cur);
		cur->prev = pre;
		pre->next = cur;
		cur->next = NULL;
		cur->is_empty = false;
		cur->x = 150;
		cur->y = 80 + i;
		fillrectangle(cur->x - inside_wall.r, cur->y - inside_wall.r, cur->x + inside_wall.r, cur->y + inside_wall.r);
		pre = cur;
	}
	for (i = inside_wall.r * 2; i <= 16 * INSIDE_WALL_R; i += 2 * INSIDE_WALL_R) {
		cur = (FN*)malloc(sizeof(FN));
		JUDGE(cur);
		cur->prev = pre;
		pre->next = cur;
		cur->next = NULL;
		cur->is_empty = false;
		cur->x = 150 + i;
		cur->y = 80;
		fillrectangle(cur->x - inside_wall.r, cur->y - inside_wall.r, cur->x + inside_wall.r, cur->y + inside_wall.r);
		pre = cur;
	}
	for (i = inside_wall.r * 2; i <= 16 * INSIDE_WALL_R; i += 2 * INSIDE_WALL_R) {
		cur = (FN*)malloc(sizeof(FN));
		JUDGE(cur);
		cur->prev = pre;
		pre->next = cur;
		cur->next = NULL;
		cur->is_empty = false;
		cur->x = 150;
		cur->y = 520 - i;
		fillrectangle(cur->x - inside_wall.r, cur->y - inside_wall.r, cur->x + inside_wall.r, cur->y + inside_wall.r);
		pre = cur;
	}
	for (i = inside_wall.r * 2; i <= 16 * INSIDE_WALL_R; i += 2 * INSIDE_WALL_R) {
		cur = (FN*)malloc(sizeof(FN));
		JUDGE(cur);
		cur->prev = pre;
		pre->next = cur;
		cur->next = NULL;
		cur->is_empty = false;
		cur->x = 150 + i;
		cur->y = 520;
		fillrectangle(cur->x - inside_wall.r, cur->y - inside_wall.r, cur->x + inside_wall.r, cur->y + inside_wall.r);
		pre = cur;
	}
	for (i = inside_wall.r * 2; i <= 16 * INSIDE_WALL_R; i += 2 * INSIDE_WALL_R) {
		cur = (FN*)malloc(sizeof(FN));
		JUDGE(cur);
		cur->prev = pre;
		pre->next = cur;
		cur->next = NULL;
		cur->is_empty = false;
		cur->x = 850 - i;
		cur->y = 80;
		fillrectangle(cur->x - inside_wall.r, cur->y - inside_wall.r, cur->x + inside_wall.r, cur->y + inside_wall.r);
		pre = cur;
	}
	for (i = inside_wall.r * 2; i <= 16 * INSIDE_WALL_R; i += 2 * INSIDE_WALL_R) {
		cur = (FN*)malloc(sizeof(FN));
		JUDGE(cur);
		cur->prev = pre;
		pre->next = cur;
		cur->next = NULL;
		cur->is_empty = false;
		cur->x = 850;
		cur->y = 80 + i;
		fillrectangle(cur->x - inside_wall.r, cur->y - inside_wall.r, cur->x + inside_wall.r, cur->y + inside_wall.r);
		pre = cur;
	}
	for (i = inside_wall.r * 2; i <= 16 * INSIDE_WALL_R; i += 2 * INSIDE_WALL_R) {
		cur = (FN*)malloc(sizeof(FN));
		JUDGE(cur);
		cur->prev = pre;
		pre->next = cur;
		cur->next = NULL;
		cur->is_empty = false;
		cur->x = 850;
		cur->y = 520 - i;
		fillrectangle(cur->x - inside_wall.r, cur->y - inside_wall.r, cur->x + inside_wall.r, cur->y + inside_wall.r);
		pre = cur;
	}
	for (i = inside_wall.r * 2; i <= 16 * INSIDE_WALL_R; i += 2 * INSIDE_WALL_R) {
		cur = (FN*)malloc(sizeof(FN));
		JUDGE(cur);
		cur->prev = pre;
		pre->next = cur;
		cur->next = NULL;
		cur->is_empty = false;
		cur->x = 850 - i;
		cur->y = 520;
		fillrectangle(cur->x - inside_wall.r, cur->y - inside_wall.r, cur->x + inside_wall.r, cur->y + inside_wall.r);
		pre = cur;
	}
}

void creat_gun() {//生成四把枪
	gun->hp = INI_GUN_HP;
	gun->is_shoot = false;
	gun->x = 250;
	gun->y = 150;
	gun->xd = 1;
	gun->yd = 0;
	gun->is_shoten = false;
	gun->is_live = true;
	gun->is_eaten = false;
	draw_gun(gun, GUN_COLOR);
	(gun + 1)->hp = INI_GUN_HP;
	(gun + 1)->is_shoot = false;
	(gun + 1)->x = 250;
	(gun + 1)->y = 450;
	(gun + 1)->xd = 1;
	(gun + 1)->yd = 0;
	(gun + 1)->is_shoten = false;
	(gun + 1)->is_live = true;
	(gun + 1)->is_eaten = false;
	draw_gun(gun + 1, GUN_COLOR);
	(gun + 2)->hp = INI_GUN_HP;
	(gun + 2)->is_shoot = false;
	(gun + 2)->x = 750;
	(gun + 2)->y = 150;
	(gun + 2)->xd = -1;
	(gun + 2)->yd = 0;
	(gun + 2)->is_shoten = false;
	(gun + 2)->is_live = true;
	(gun + 2)->is_eaten = false;
	draw_gun(gun + 2, GUN_COLOR);
	(gun + 3)->hp = INI_GUN_HP;
	(gun + 3)->is_shoot = false;
	(gun + 3)->x = 750;
	(gun + 3)->y = 450;
	(gun + 3)->xd = -1;
	(gun + 3)->yd = 0;
	(gun + 3)->is_shoten = false;
	(gun + 3)->is_live = true;
	(gun + 3)->is_eaten = false;
	draw_gun(gun + 3, GUN_COLOR);
}

void draw_gun(Gun* g, int color) {//画枪
	if (g->is_eaten)return;
	clearcircle(g->x, g->y, GUN_R);
	setfillcolor(color);
	int temp;
	if (g->yd > 0)temp = -1;
	else temp = 1;
	solidpie(g->x - GUN_R, g->y - GUN_R, g->x + GUN_R, g->y + GUN_R, temp * acos(g->xd) + ANGLE, temp * acos(g->xd) - ANGLE);
}

void move_gun(Gun* g) {//移动枪的方向
	if (!g->is_live) {
		draw_gun(g, DEAD_GUN_COLOR);
		return;
	}
	double d;
	d = (double)sq_distance(g->x, g->y, player.head->x, player.head->y);
	d = sqrt(d);
	g->xd = ((double)player.head->x - (double)g->x) / d;
	g->yd = ((double)player.head->y - (double)g->y) / d;
	draw_gun(g, GUN_COLOR);
}

void check_gun(Gun* g) {//检查并处理枪的各种状态
	if (!g->is_live)return;
	if (g->is_shoot) {
		g->is_shoot = false;
		creatbullet(g->x + (int)(g->xd * bullet.speed), (int)(g->y + g->yd * bullet.speed), g);
	}
	if (g->is_shoten) {
		g->is_shoten = false;
		g->hp--;
	}
	if (g->hp <= 0) {
		g->is_live = false;
		draw_gun(g, DEAD_GUN_COLOR);
	}
}

inline bool check_pass() {//检查是否过关
	int i;
	for (i = 0; i < 4; i++) {
		if (!((gun + i)->is_eaten))return false;
	}
	return true;
}

void save_grade3() {//保存游戏成绩（不是分数）
	long long history[5];
	fopen_s(&grade_fp3, "snake2_grade3.bin", "rb");
	JUDGE(grade_fp3);
	fread(history, sizeof(long long), 5, grade_fp3);
	fclose(grade_fp3);
	long long temp1 = grade, temp2;
	int i;
	for (i = 0; i < 5; i++) {
		if (*(history + i) < temp1) {
			temp2 = temp1;
			temp1 = *(history + i);
			*(history + i) = temp2;
		}
	}
	fopen_s(&grade_fp3, "snake2_grade3.bin", "wb");
	JUDGE(grade_fp3);
	fwrite(history, sizeof(long long), 5, grade_fp3);
	fclose(grade_fp3);
}