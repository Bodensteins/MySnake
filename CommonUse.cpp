#include"Snake2.h"

//外部变量声明
extern Snake player;
extern Foods nof, pof, mif, door1, door2, inside_wall, acf;
extern IMAGE fill;
extern M mon;
extern Bullet bullet;
extern int now_time, t1, t2, begin;
extern unsigned poi_times, loop;
extern FILE* load_fp1, * load_fp2, * load_fp3;
extern LOGFONT lg;
extern MenuState ms;
extern Gun gun[4];
extern bool gun_exist;

void load1();
void load2();
void load3();

void rand_xy(int* x, int* y, int r) {//在游戏边界内生成一对随机数
	*x = rand() % (EDGE_WALL_X - 2 * WALL_B - 2 * r - 3) + WALL_B + r + 2;
	*y = rand() % (EDGE_WALL_Y - 2 * WALL_B - 2 * r - 3) + WALL_B + r + 2;
}

inline int sq_distance(int x1, int y1, int x2, int y2) {//计算两点间距离的平方
	return ((x1 - x2) * (x1 - x2) + (y1 - y2) * (y1 - y2));
}

inline double time() {//返回程序执行的时间
	return ((double)clock() / CLOCKS_PER_SEC);
}

void time_left() {//计算游戏剩余时间
	t1 = time() - begin;
	if (t1 - t2 >= 1) {
		now_time--;
		t2 = t1;
	}
}

void shining(Foods food, int color, int loop) {//使某种食物闪烁
	if (food.head == NULL)return;
	FN* f = food.head;
	if (loop % 5 == 0 || loop % 5 == 1) {
		while (f != NULL && !f->is_empty) {
			clearcircle(f->x, f->y, food.r);
			f = f->next;
		}
	}
	else {
		while (f != NULL && !f->is_empty) {
			setfillcolor(color);
			fillcircle(f->x, f->y, food.r);
			f = f->next;
		}
	}
}

void clearfoods(Foods* f) {//清除某种食物
	if (!f->state)return;
	f->state = EMPTY;
	FN* cur = f->head, * pre = f->head;
	while (cur->next != NULL) {
		cur = cur->next;
		clearcircle(pre->x, pre->y, f->r);
		free(pre);
		pre = cur;
	}
	clearcircle(cur->x, cur->y, f->r);
	free(cur);
	f->head = NULL;
}

void redraw(Foods food, int color) {//重画某种食物
	if (!food.state)return;
	FN* f = food.head;
	while (f != NULL) {
		if (!f->is_empty) {
			setfillcolor(color);
			fillcircle(f->x, f->y, food.r);
		}
		f = f->next;
	}
}

void redrawdoor(Foods door, int color) {//重画传送门
	if (!door.state)return;
	setfillcolor(color);
	fillrectangle(door.head->x - door.r, door.head->y - door.r, door.head->x + door.r, door.head->y + door.r);
	fillrectangle(door.head->next->x - door.r, door.head->next->y - door.r, door.head->next->x + door.r, door.head->next->y + door.r);
	setfillcolor(BK_COLOR);
	fillrectangle(door.head->x - door.r + DOOR_B, door.head->y - door.r + DOOR_B, door.head->x + door.r - DOOR_B, door.head->y + door.r - DOOR_B);
	fillrectangle(door.head->next->x - door.r + DOOR_B, door.head->next->y - door.r + DOOR_B, door.head->next->x + door.r - DOOR_B, door.head->next->y + door.r - DOOR_B);
}

void redraw_wall(Foods inside_wall) {//重画墙
	FN* cur = inside_wall.head;
	setfillcolor(WALL_COLOR);
	while (cur != NULL) {
		fillrectangle(cur->x - inside_wall.r, cur->y - inside_wall.r, cur->x + inside_wall.r, cur->y + inside_wall.r);
		cur = cur->next;
	}
}

bool is_f_overlap(FN* node, int r, unsigned type) {//判断某种食物生成时是否与其他游戏元素重合
	int rd1, rd2, i;
	SN* snake = player.head;
	FN* behind = node->next, * forward = node->next, * p;
	rd2 = (r + HEAD_R) * (r + HEAD_R);
	while (snake != NULL) {
		rd1 = sq_distance(snake->x, snake->y, node->x, node->y);
		if (rd1 <= rd2)return true;
		snake = snake->next;
	}
	if (mon.exist) {
		rd1 = sq_distance(mon.x, mon.y, node->x, node->y);
		rd2 = (r + MON_R) * (r + MON_R);
		if (rd1 <= rd2) return true;
	}
	if (gun_exist) {
		rd2 = (r + GUN_R) * (r + GUN_R);
		for (i = 0; i < 4; i++) {
			rd1 = sq_distance((gun + i)->x, (gun + i)->y, node->x, node->y);
			if (rd1 <= rd2)return true;
		}
	}
	rd2 = 8 * r * r;
	while (forward != NULL) {
		rd1 = sq_distance(forward->x, forward->y, node->x, node->y);
		if (rd1 <= rd2 && !forward->is_empty)return true;
		forward = forward->prev;
	}
	while (behind != NULL) {
		rd1 = sq_distance(behind->x, behind->y, node->x, node->y);
		if (rd1 <= rd2 && !behind->is_empty)return true;
		behind = behind->next;
	}
	if (nof.state && type != NORMAL) {
		rd2 = (r + NORMAL_R) * (r + NORMAL_R);
		p = nof.head;
		while (p != NULL) {
			rd1 = sq_distance(p->x, p->y, node->x, node->y);
			if (rd1 <= rd2 && !p->is_empty)return true;
			p = p->next;
		}
	}
	if (pof.state && type != POI) {
		rd2 = (r + POI_R) * (r + POI_R);
		p = pof.head;
		while (p != NULL) {
			rd1 = sq_distance(p->x, p->y, node->x, node->y);
			if (rd1 <= rd2 && !p->is_empty)return true;
			p = p->next;
		}
	}
	if (mif.state && type != MINE) {
		rd2 = (r + MINE_R) * (r + MINE_R);
		p = mif.head;
		while (p != NULL) {
			rd1 = sq_distance(p->x, p->y, node->x, node->y);
			if (rd1 <= rd2 && !p->is_empty)return true;
			p = p->next;
		}
	}
	if (door1.state && type != DOOR1) {
		rd2 = (r + DOOR_R) * (r + DOOR_R) * 2;
		p = door1.head;
		while (p != NULL) {
			rd1 = sq_distance(p->x, p->y, node->x, node->y);
			if (rd1 <= rd2 && !p->is_empty)return true;
			p = p->next;
		}
	}
	if (door2.state && type != DOOR2) {
		rd2 = (r + DOOR_R) * (r + DOOR_R) * 2;
		p = door2.head;
		while (p != NULL) {
			rd1 = sq_distance(p->x, p->y, node->x, node->y);
			if (rd1 <= rd2 && !p->is_empty)return true;
			p = p->next;
		}
	}
	if (inside_wall.state && type != WALL) {
		rd2 = (r + INSIDE_WALL_R + 3) * (r + INSIDE_WALL_R + 3);
		p = inside_wall.head;
		while (p != NULL) {
			rd1 = sq_distance(p->x, p->y, node->x, node->y);
			if (rd1 <= rd2 && !p->is_empty)return true;
			p = p->next;
		}
	}
	if (acf.state && type != ACC) {
		rd2 = (r + ACC_R) * (r + ACC_R);
		p = acf.head;
		while (p != NULL) {
			rd1 = sq_distance(p->x, p->y, node->x, node->y);
			if (rd1 <= rd2 && !p->is_empty)return true;
			p = p->next;
		}
	}
	if (bullet.exist) {
		BN* b = bullet.head->next;
		while (b != NULL) {
			rd1 = sq_distance(b->x, b->y, node->x, node->y);
			rd2 = r * r;
			if (rd1 <= rd2)return true;
			b = b->next;
		}
	}
	return false;
}

bool is_phead_foods(Foods food) {//判断蛇头是否吃到某种食物
	if (!food.state)return false;
	FN* p = food.head;
	int rd1, rd2;
	rd2 = (food.r + HEAD_R) * (food.r + HEAD_R);
	while (p != NULL) {
		rd1 = sq_distance(p->x, p->y, player.head->x, player.head->y);
		//if (food.type == WALL)rd1 -= 2; 
		if (rd1 <= rd2 && !p->is_empty) {
			p->is_empty = true;
			if (food.type != WALL) {
				clearcircle(p->x, p->y, food.r);
				fillcircle(player.head->x, player.head->y, HEAD_R);
			}
			else player.hp = 0;
			return true;
		}
		p = p->next;
	}
	return false;
}

void getmsg(int xx, int yy, int level) {//获取鼠标和键盘的信息
	double d;
	MOUSEMSG msg;
	while (MouseHit()) {
		msg = GetMouseMsg();
		if (msg.mkLButton) {
			if (msg.x > EDGE_WALL_X + WALL_B)return;
			d = (double)sq_distance(msg.x, msg.y, xx, yy);
			d = sqrt(d);
			player.xd = ((double)msg.x - (double)xx) / d;
			player.yd = ((double)msg.y - (double)yy) / d;
		}
	}
	while (_kbhit()) {
		switch (_getch()) {
		case 'D':
		case 'd':
			player.state.accel = 1;
			break;
		case 'A':
		case 'a':
			player.state.accel = 2;
			break;
		case 'E':
		case'e':
			switch (level) {
			case 1: save1(); break;
			case 2: save2(); break;
			case 3: save3(); break;
			}
			reli();
			closegraph();
			game_exit_menu(level);
			break;
		case 'Q':
		case 'q':
			redraw(pof, POI_COLOR);
			setfillcolor(WHITE);
			solidrectangle(EDGE_WALL_X + WALL_B + 2, 0, IMG_X, 220);
			settextcolor(RED);
			outtextxy(EDGE_WALL_X + WALL_B + 2, 0, _T("Game pauses"));
			outtextxy(EDGE_WALL_X + WALL_B + 2, 25, _T("Press any key to start"));
			system("pause");
			setfillcolor(WHITE);
			solidrectangle(EDGE_WALL_X + WALL_B + 2, 0, IMG_X, 220);
			break;
		case'W':
		case'w':
			if (player._bullet && player.hp > 1)player.state.is_shoot = 1;
			break;
		}
	}
}

void game_exit_menu(int level) {//游戏中途退出菜单
	MOUSEMSG quit_msg;
	bool choice;
	initgraph(600, 400);
	setbkcolor(BK_COLOR);
	cleardevice();
	lg.lfHeight = 40;
	lg.lfWidth = 20;
	lg.lfEscapement = 0;
	lg.lfItalic = true;
	lg.lfOrientation = 0;
	lg.lfStrikeOut = false;
	lg.lfUnderline = false;
	settextcolor(BLUE);
	settextstyle(&lg);
	outtextxy(90, 10, _T("Are you sure to quit?"));
	lg.lfHeight = 24;
	lg.lfWidth = 12;
	settextstyle(&lg);
	outtextxy(140, 60, _T("Your process will be saved"));
	lg.lfHeight = 40;
	lg.lfWidth = 20;
	settextstyle(&lg);
	while (true) {
		quit_msg = GetMouseMsg();
		if (quit_msg.x > 120 && quit_msg.x < 480 && quit_msg.y>140 && quit_msg.y < 180) {
			settextcolor(LIGHTBLUE);
			outtextxy(120, 140, _T("Yes, save and quit"));
			if (quit_msg.mkLButton) {
				choice = true;
				break;
			}
		}
		else {
			settextcolor(RED);
			outtextxy(120, 140, _T("Yes, save and quit"));
		}
		if (quit_msg.x > 100 && quit_msg.x < 500 && quit_msg.y>250 && quit_msg.y < 290) {
			settextcolor(LIGHTBLUE);
			outtextxy(100, 250, _T("NO, back to the game"));
			if (quit_msg.mkLButton) {
			    choice = false;
				break;
			}
		}
		else {
			settextcolor(RED);
			outtextxy(100, 250, _T("NO, back to the game"));
		}
	}
	switch (level) {
	case 1:
		if (!choice)load1();
		else ms.is_game1_exit = 1;
		break;
	case 2:
		if (!choice)load2();
		else ms.is_game2_exit = 1;
		break;
	case 3:
		if (!choice)load3();
		else ms.is_game3_exit = 1;
		break;
	}

}

void creatwall() {//生成边界墙
	SetWorkingImage(&fill);
	setbkcolor(BLACK);
	cleardevice();
	solidrectangle(1, 1, WALL_B - 1, WALL_B - 1);
	SetWorkingImage(NULL);
	setfillstyle(BS_PATTERN, NULL, &fill);
	settextcolor(WALL_COLOR);
	solidrectangle(0, 0, WALL_B, EDGE_WALL_Y);
	solidrectangle(0, 0, EDGE_WALL_X, WALL_B);
	solidrectangle(0, EDGE_WALL_Y - WALL_B, EDGE_WALL_X, EDGE_WALL_Y);
	solidrectangle(EDGE_WALL_X - WALL_B, 0, EDGE_WALL_X, EDGE_WALL_Y);
	setfillstyle(NULL);
}

SN* creatplayer() {//生成玩家的蛇
	SN* head, * pre, * cur;
	setfillstyle(NULL);
	setfillcolor(PHEAD_COLOR);
	head = pre = cur = (SN*)malloc(sizeof(SN));
	JUDGE(head);
	head->x = INI_PX;
	head->y = INI_PY;
	head->prev = NULL;
	fillcircle(head->x, head->y, HEAD_R);
	setfillcolor(PBODY_COLOR);
	int i;
	for (i = 1; i < INI_PLEN; i++) {
		cur = (SN*)malloc(sizeof(SN));
		JUDGE(cur);
		pre->next = cur;
		cur->prev = pre;
		player.tail = pre = cur;
		cur->x = INI_PX - 2 * i * HEAD_R;
		cur->y = INI_PY;
		cur->next = NULL;
		fillcircle(cur->x, cur->y, BODY_R);
	}
	player.score = 0;
	player.hp = INI_HP;
	player.speed = LOW_SPEED;
	player.high_step = LIMIT_STEP;
	player.accel_times = INI_ACCEL_TIMES;
	player.xd = 1;
	player.yd = 0;
	player.state = { 0,1,0,0,0,0,0,0 };
	return head;
}

void creatfs(Foods* f, unsigned type, int r, int color, int num) {//生成某种事物
	if (!num)return;
	unsigned add_num;
	if (type == POI) {
		add_num = num + poi_times;
		poi_times++;
	}
	else add_num = num;
	f->type = type;
	f->r = r;
	FN* cur, * pre;
	SN* p = player.head;
	int rd1, rd2, i;
	bool overlap = false, temp_overlap = false;
	setfillcolor(color);
	f->head = cur = pre = (FN*)malloc(sizeof(FN));
	JUDGE(cur);
	cur->prev = NULL;
	cur->next = NULL;
	rand_xy(&cur->x, &cur->y, r);
	temp_overlap = is_f_overlap(cur, r, type);
	if (temp_overlap) {
		cur->is_empty = true;
		overlap = true;
		temp_overlap = false;
	}
	else {
		cur->is_empty = false;
		fillcircle(cur->x, cur->y, r);
	}
	for (i = 1; i < add_num; i++) {
		p = player.head;
		cur = (FN*)malloc(sizeof(FN));
		JUDGE(cur);
		pre->next = cur;
		cur->prev = pre;
		pre = cur;
		cur->next = NULL;
		rand_xy(&cur->x, &cur->y, r);
		temp_overlap = is_f_overlap(cur, r, type);
		if (temp_overlap) {
			cur->is_empty = true;
			overlap = true;
			temp_overlap = false;
		}
		else {
			cur->is_empty = false;
			fillcircle(cur->x, cur->y, r);
		}
	}


	if (overlap)f->state = VACA;
	else f->state = FULL;
}

void creatdoor(Foods*door,int color,int type) {//生成传送门
	door->type = type;
	door->r = DOOR_R;
	door->state = EMPTY;
	FN* cur, * pre;
	int rd1, rd2;
	bool overlap = false;
	door->head = cur = pre = (FN*)malloc(sizeof(FN));
	JUDGE(cur);
	cur->next = NULL;
	cur->prev = NULL;
	rand_xy(&cur->x, &cur->y, DOOR_R);
	overlap = is_f_overlap(cur, DOOR_R, type);
	if (overlap) {
		free(cur);
		door->head = NULL;
		return;
	}
	else {
		cur = (FN*)malloc(sizeof(FN));
		JUDGE(cur);
		rand_xy(&cur->x, &cur->y, DOOR_R);
		cur->prev = pre;
		pre->next = cur;
		cur->next = NULL;
		overlap = is_f_overlap(cur, DOOR_R, type);
		if (overlap) {
			free(cur);
			free(pre);
			door->head = NULL;
			return;
		}
	}
	if (overlap) door->state = EMPTY;
	else {
		door->head->is_empty = false;
		JUDGE(door->head->next);
		door->head->next->is_empty = false;
		door->state = FULL;
		setfillcolor(color);
		fillrectangle(door->head->x - door->r, door->head->y - door->r, door->head->x + door->r, door->head->y + door->r);
		fillrectangle(door->head->next->x - door->r, door->head->next->y - door->r, door->head->next->x + door->r, door->head->next->y + door->r);
		setfillcolor(BK_COLOR);
		fillrectangle(door->head->x - door->r + DOOR_B, door->head->y - door->r + DOOR_B, door->head->x + door->r - DOOR_B, door->head->y + door->r - DOOR_B);
		fillrectangle(door->head->next->x - door->r + DOOR_B, door->head->next->y - door->r + DOOR_B, door->head->next->x + door->r - DOOR_B, door->head->next->y + door->r - DOOR_B);
	}
}

inline void drawbullet(int x,int y) {//画出子弹
	settextcolor(BULLET_COLOR);
	outtextxy(x, y, '*');
}

void creatbullet(int x,int y) {//生成一个玩家发射的子弹
	BN* b;
	b = (BN*)malloc(sizeof(BN));
	JUDGE(b);
	b->x = x;
	b->y = y;
	b->xd = player.xd;
	b->yd = player.yd;
	b->is_hit = false;
	b->next = NULL;
	b->prev = bullet.tail;
	bullet.tail->next = b;
	bullet.tail = b;
	drawbullet(x, y);
	if (!bullet.exist)bullet.exist = true;
	bullet.num++;
}

void creatbullet(int x, int y, Gun* g) {//生成一个枪发射的子弹
	BN* b;
	b = (BN*)malloc(sizeof(BN));
	JUDGE(b);
	b->x = x;
	b->y = y;
	b->xd = g->xd;
	b->yd = g->yd;
	b->is_hit = false;
	b->next = NULL;
	b->prev = bullet.tail;
	bullet.tail->next = b;
	bullet.tail = b;
	drawbullet(x, y);
	if (!bullet.exist)bullet.exist = true;
	bullet.num++;
}

void movebullet() {//子弹移动函数
	if (!bullet.exist)return;
	BN* b = bullet.head->next, * del_temp = NULL;
	int i;
	while (b != NULL) {
		for (i = BULLET_BASIC_SPEED; i <= BULLET_SPEED; i += BULLET_BASIC_SPEED) {
			settextcolor(WHITE);
			outtextxy(b->x, b->y, '*');
			b->x += b->xd * BULLET_BASIC_SPEED;
			b->y += b->yd * BULLET_BASIC_SPEED;
			b->is_hit = checkbullet(b);
			if (!b->is_hit) continue;
			else {
				del_temp = b->next;
				delbullet(b);
				break;
			}

		}
		if (!b->is_hit) {
			drawbullet(b->x, b->y);
			b = b->next;
		}
		else b = del_temp;
	}
}

bool checkbullet(BN*b) {//判断子弹是否击中某物
	int rd1, rd2, i;
	SN* p1 = player.head, * p;
	rd2 = HEAD_R * HEAD_R;
	rd1 = sq_distance(b->x, b->y, p1->x, p1->y);
	if (rd1 <= rd2) {
		player.hp--;
		p = player.tail;
		player.tail = p->prev;
		player.tail->next = NULL;
		clearcircle(p->x, p->y, BODY_R);
		free(p);
		if (player.hp == 0)player.state.is_live = 0;
		return true;
	}
	p1 = player.head->next;
	rd2 = BODY_R * BODY_R;
	while (p1 != NULL) {
		rd1 = sq_distance(b->x, b->y, p1->x, p1->y);
		if (rd1 <= rd2 && !b->is_hit) {
			player.hp--;
			p = player.tail;
			player.tail = p->prev;
			player.tail->next = NULL;
			clearcircle(p->x, p->y, BODY_R);
			free(p);
			if (player.hp == 0)player.state.is_live = 0;
			return true;
		}
		p1 = p1->next;
	}
	
	if (mon.exist) {
		rd2 = MON_R * MON_R;
		rd1 = sq_distance(b->x, b->y, mon.x, mon.y);
		if (rd1 <= rd2) {
			mon.state.is_shoten = 1;
			return true;
		}
	}

	if (gun_exist) {
		rd2 = GUN_R * GUN_R + 10;
		for (i = 0; i < 4; i++) {
			rd1 = sq_distance(gun[i].x, gun[i].y, b->x, b->y);
			if (rd1 <= rd2) {
				gun[i].is_shoten = true;
				return true;
			}
		}
	}

	if (is_bullet_foods(nof, b)) {
		nof.state = VACA;
		return true;
	}
	if (is_bullet_foods(pof, b)) {
		pof.state = VACA;
		return true;
	}
	if (is_bullet_foods(mif, b)) {
		mif.state = VACA;
		return true;
	}
	if (is_bullet_foods(acf, b)) {
		acf.state = VACA;
		return true;
	}
	if (is_bullet_foods(door1, b)) {
		return true;
	}
	if (is_bullet_foods(door2, b)) {
		return true;
	}
	if (is_bullet_foods(inside_wall, b))return true;
	if (b->x <= WALL_B || b->x >= EDGE_WALL_X - WALL_B - 3) {
		return true;
	}
	if (b->y <= WALL_B || b->y >= EDGE_WALL_Y - WALL_B - 3) {
		return true;
	}
	return false;
}

bool is_bullet_foods(Foods food, BN* b) {//子弹是否击中食物
	if (food.state == EMPTY)return false;
	FN* f = food.head;
	int rd1, rd2;
	rd2 = 2 * food.r * food.r;
	while (f != NULL) {
		rd1 = sq_distance(f->x, f->y, b->x, b->y);
		if (rd1 <= rd2) {
			if (food.type != WALL && food.type != DOOR1 && food.type != DOOR2) {
				clearcircle(f->x, f->y, food.r);
				f->is_empty = true;
			}
			return true;
		}
		f = f->next;
	}
	return false;
}

void delbullet(BN* b) {//删除一个子弹节点
	if (b == bullet.tail) {
		bullet.tail = b->prev;
		bullet.tail->next = NULL;
		free(b);
	}
	else {
		b->prev->next = b->next;
		b->next->prev = b->prev; 
		free(b);
	}
	if (bullet.head->next == NULL)bullet.exist = false;
	bullet.num--;
}

void move() {//蛇移动函数
	if (!player.state.is_live)return;
	if (!player.hp) {
		player.state.is_live = 0;
		return;
	}
	SN* p1;
	int i;
	for (i = 0; i < player.speed; i += LOW_SPEED) {
		p1 = (SN*)malloc(sizeof(SN));
		JUDGE(p1);
		p1->next = player.head;
		player.head->prev = p1;
		p1->prev = NULL;
		p1->x = player.head->x + LOW_SPEED * player.xd;
		p1->y = player.head->y + LOW_SPEED * player.yd;
		clearcircle(player.head->x, player.head->y, HEAD_R);
		setfillcolor(PBODY_COLOR);
		fillcircle(player.head->x, player.head->y, BODY_R);
		setfillcolor(PHEAD_COLOR);
		fillcircle(p1->x, p1->y, HEAD_R);
		player.head = p1;
		checkplayer();
	}
	player.head->prev = NULL;
}

void checkplayer() {//检查蛇是否碰到某物
	int rd1, rd2;
	SN* p1;
	if (player.head->x <= WALL_B + HEAD_R || player.head->x >= EDGE_WALL_X - WALL_B - HEAD_R) {
		player.state.is_live = 0;
		player.hp = 0;
		return;
	}
	else if (player.head->y <= WALL_B + HEAD_R || player.head->y >= EDGE_WALL_Y - WALL_B - HEAD_R) {
		player.state.is_live = 0;
		player.hp = 0;
		return;
	}
	else {

		int i;
		if (gun_exist) {
			rd2 = (HEAD_R + GUN_R) * (HEAD_R + GUN_R);
			for (i = 0; i < 4; i++) {
				rd1 = sq_distance((gun + i)->x, (gun + i)->y, player.head->x, player.head->y);
				if (rd1 <= rd2) {
					if ((gun + i)->is_live) {
						player.state.is_live = 0;
						player.hp = 0;
						return;
					}
					else if (!(gun + i)->is_eaten) {
						(gun + i)->is_eaten = true;
						player.score += 30;
						clearcircle((gun + i)->x, (gun + i)->y, GUN_R + 1);
						setfillcolor(PHEAD_COLOR);
						fillcircle(player.head->x, player.head->y, HEAD_R);
					}
				}
			}
		}


		for (i = 0, p1 = player.head; i < 3 && p1 != NULL; i++, p1 = p1->next);
		rd2 = (HEAD_R + BODY_R) * (HEAD_R + BODY_R);
		while (p1 != NULL) {
			rd1 = sq_distance(p1->x, p1->y, player.head->x, player.head->y);
			if (rd1 < rd2) {
				player.hp--;
				SN* p = player.tail;
				player.tail = p->prev;
				player.tail->next = NULL;
				clearcircle(p->x, p->y, BODY_R);
				free(p);
				if (player.hp == 0)player.state.is_live = 0;
				break;
			}
			p1 = p1->next;
		}
	
		
		if (player.state.accel == 1 && player.speed == LOW_SPEED && player.accel_times > 0) {
			player.speed = HIGH_SPEED;
			player.accel_times--;
			player.state.accel = 0;
		}
		else if ((player.state.accel == 2 && player.speed == HIGH_SPEED) || player.high_step == 0) {
			player.speed = LOW_SPEED;
			player.state.accel = 0;
			player.high_step = LIMIT_STEP;
		}
		
	}
	if (is_phead_foods(inside_wall))player.state.is_live = 0; 
	if (is_phead_foods(nof))player.state.is_nfood = 1; 
	if (is_phead_foods(pof))player.state.is_poi = 1; 
	if (is_phead_foods(mif))player.state.is_mine = 1;
	if (is_phead_foods(door1))player.state.is_door1 = 1; 
	if (is_phead_foods(door2))player.state.is_door2 = 1;
	if (is_phead_foods(acf))player.state.is_acf = 1;
	
	
}

void deal() {//处理蛇和食物的各种状态
	if (!player.state.is_live)return;
	int i = 0, rd1, rd2;
	SN* p1;
	if (player.state.is_nfood) {
		player.score++;
		player.hp++;
		player.state.is_nfood = 0;
		nof.state = VACA;
		i += LOW_SPEED;
	}
	SN* p;
	for (; i < player.speed; i += LOW_SPEED) {
		p = player.tail;
		player.tail = p->prev;
		player.tail->next = NULL;
		clearcircle(p->x, p->y, BODY_R);
		free(p);
	}
	
	if (player.state.is_poi) {
		player.state.is_poi = 0;
		player.score += 2;
		player.hp--;
		pof.state = VACA;
		p = player.tail;
		player.tail = p->prev;
		player.tail->next = NULL;
		clearcircle(p->x, p->y, BODY_R);
		free(p);
		if (player.hp == 0) {
			player.state.is_live = 0;
			return;
		}
	}

	if (player.state.is_mine) {
		player.state.is_mine = 0;
		player.score += 5;
		mif.state = VACA;
		SN* p1 = player.head, * p2 = player.tail;
		while (p1 != p2 && p1->next != p2) {
			p1 = p1->next;
			player.tail = p2->prev;
			player.tail->next = NULL;
			clearcircle(p2->x, p2->y, BODY_R);
			free(p2);
			p2 = player.tail;
			player.hp--;
			if (player.hp == 0) {
				player.state.is_live = 0;
				return;
			}
		}
		if (p1->next == p2) {
			player.tail = p1;
			p1->next = NULL;
			clearcircle(p2->x, p2->y, BODY_R);
			free(p2);
			player.hp--;
			if (player.hp == 0) {
				player.state.is_live = 0;
				return;
			}
		}
	}

	if (player.state.is_door1) {
		player.state.is_door1 = 0;
		door1.state = EMPTY;
		int rd1, rd2;
		rd1 = sq_distance(door1.head->x, door1.head->y, player.head->x, player.head->y);
		rd2 = (HEAD_R + door1.r) * (HEAD_R + door1.r);
		if (rd1 <= rd2) {
			clearcircle(player.head->x, player.head->y, HEAD_R);
			clearrectangle(door1.head->x - door1.r, door1.head->y - door1.r, door1.head->x + door1.r, door1.head->y + door1.r);
			player.head->x = door1.head->next->x;
			player.head->y = door1.head->next->y;
			clearrectangle(door1.head->next->x - door1.r, door1.head->next->y - door1.r, door1.head->next->x + door1.r, door1.head->next->y + door1.r);
			setfillcolor(RED);
			fillcircle(player.head->x, player.head->y, HEAD_R);
		}
		else {
			clearcircle(player.head->x, player.head->y, HEAD_R);
			clearrectangle(door1.head->next->x - door1.r, door1.head->next->y - door1.r, door1.head->next->x + door1.r, door1.head->next->y + door1.r);
			player.head->x = door1.head->x;
			player.head->y = door1.head->y;
			clearrectangle(door1.head->x - door1.r, door1.head->y - door1.r, door1.head->x + door1.r, door1.head->y + door1.r);
			setfillcolor(RED);
			fillcircle(player.head->x, player.head->y, HEAD_R);
		}
		free(door1.head->next);
		free(door1.head);
		door1.head = NULL;
	}
	if (player.state.is_door2) {
		player.state.is_door2 = 0;
		door2.state = EMPTY;
		int rd1, rd2;
		rd1 = sq_distance(door2.head->x, door2.head->y, player.head->x, player.head->y);
		rd2 = (HEAD_R + door2.r) * (HEAD_R + door2.r);
		if (rd1 <= rd2) {
			clearcircle(player.head->x, player.head->y, HEAD_R);
			clearrectangle(door2.head->x - door2.r, door2.head->y - door2.r, door2.head->x + door2.r, door2.head->y + door2.r);
			player.head->x = door2.head->next->x;
			player.head->y = door2.head->next->y;
			clearrectangle(door2.head->next->x - door2.r, door2.head->next->y - door2.r, door2.head->next->x + door2.r, door2.head->next->y + door2.r);
			setfillcolor(RED);
			fillcircle(player.head->x, player.head->y, HEAD_R);
		}
		else {
			clearcircle(player.head->x, player.head->y, HEAD_R);
			clearrectangle(door2.head->next->x - door2.r, door2.head->next->y - door2.r, door2.head->next->x + door2.r, door2.head->next->y + door2.r);
			player.head->x = door2.head->x;
			player.head->y = door2.head->y;
			clearrectangle(door2.head->x - door2.r, door2.head->y - door2.r, door2.head->x + door2.r, door2.head->y + door2.r);
			setfillcolor(RED);
			fillcircle(player.head->x, player.head->y, HEAD_R);
		}
		free(door2.head->next);
		free(door2.head);
		door2.head = NULL;
	}
	if (player.state.is_acf) {
		player.state.is_acf = 0;
		player.accel_times++;
		acf.state = VACA;
	}
	if (player.state.is_shoot) {
		player.state.is_shoot = 0;
		player.hp--;
		SN* p = player.tail;
		player.tail = p->prev;
		player.tail->next = NULL;
		clearcircle(p->x, p->y, BODY_R);
		free(p);
		creatbullet(player.head->x + player.xd * player.speed, player.head->y + player.yd * player.speed);
	}

	rd2 = (HEAD_R + MON_R) * (HEAD_R + MON_R);
	p1 = player.head;
	JUDGE(p1);
	
	rd1 = sq_distance(mon.x, mon.y, p1->x, p1->y);
	if (rd1 <= rd2) {
		player.state.is_live = 0;
		player.hp = 0;
		return;
	}
	
	p1 = player.head->next;
	while (p1 != NULL) {
		rd1 = sq_distance(mon.x, mon.y, p1->x, p1->y);
		if (rd1 <= rd2) {
			SN* p2;
			player.tail = p1->prev;
			p1->prev->next = NULL;
			for (p2 = p1; p2 != NULL; p2 = p2->next) {
				clearcircle(p2->x, p2->y, BODY_R);
				player.hp--;
			}
			p2 = p1;
			reli_snake(p2);
			setfillcolor(MON_COLOR);
			fillcircle(mon.x, mon.y, MON_R);
			break;
		}
		p1 = p1->next;
	}

	if (player.hp == 0) {
		player.state.is_live = 0;
		return;
	}

	if (nof.state == VACA) ment_fs(nof);
	if (pof.state == VACA)ment_fs(pof);
	if (mif.state == VACA)ment_fs(mif);
	if (door1.state == EMPTY)creatdoor(&door1, DOOR1_COLOR, DOOR1);
	if (door2.state == EMPTY)creatdoor(&door2, DOOR2_COLOR, DOOR2);
	if (acf.state == VACA)ment_fs(acf);
}

void ment_fs(Foods food) {//补充食物
	FN* p = food.head;
	bool temp_overlap = false, overlap = false;
	while (p != NULL) {
		if (p->is_empty) {
			rand_xy(&p->x, &p->y, food.r);
			temp_overlap = is_f_overlap(p, food.r, food.type);
			if (temp_overlap) {
				overlap = true;
				temp_overlap = false;
			}
			else {
				p->is_empty = false;
				switch (food.type) {
				case NORMAL:
					setfillcolor(NORMAL_COLOR);
					fillcircle(p->x, p->y, NORMAL_R);
					break;
				case POI:
					setfillcolor(POI_COLOR);
					fillcircle(p->x, p->y, POI_R);
					break;
				case MINE:
					setfillcolor(MINE_COLOR);
					fillcircle(p->x, p->y, MINE_R);
					break;
				case ACC:
					setfillcolor(ACC_COLOR);
					fillcircle(p->x, p->y, ACC_R);
				}
			}
		}
		p = p->next;
	}
	if (!overlap) {
		switch (food.type) {
		case NORMAL:nof.state = FULL; break;
		case POI:pof.state = FULL; break;
		case MINE:mif.state = FULL; break;
		case ACC:acf.state = FULL; break;
		}
	}
}

void reload_player(FILE* load_fp) {//重新加载蛇
	int i;
	fread(&player, sizeof(Snake), 1, load_fp);
	SN* pre, * cur;
	player.head = pre = cur = (SN*)malloc(sizeof(SN));
	JUDGE(cur);
	fread(&cur->x, sizeof(int), 1, load_fp);
	fread(&cur->y, sizeof(int), 1, load_fp);
	cur->prev = NULL;
	setfillcolor(PHEAD_COLOR);
	fillcircle(cur->x, cur->y, HEAD_R);
	for (i = 0; i < player.hp; i++) {
		cur = (SN*)malloc(sizeof(SN));
		JUDGE(cur);
		fread(&cur->x, sizeof(int), 1, load_fp);
		fread(&cur->y, sizeof(int), 1, load_fp);
		cur->prev = pre;
		pre->next = cur;
		pre = cur;
		setfillcolor(PBODY_COLOR);
		fillcircle(cur->x, cur->y, BODY_R);
	}
	cur->next = NULL;
	player.tail = cur;
}

void reload_foods(FILE* fp, Foods* food, int num) {//重新加载某种食物
	int i;
	fread(food, sizeof(Foods), 1, fp);
	FN* cur, * pre;
	food->head = cur = pre = (FN*)malloc(sizeof(FN));
	JUDGE(cur);
	fread(&cur->x, sizeof(int), 1, fp);
	fread(&cur->y, sizeof(int), 1, fp);
	fread(&cur->is_empty, sizeof(bool), 1, fp);
	cur->prev = NULL;
	for (i = 1; i < num; i++) {
		cur = (FN*)malloc(sizeof(FN));
		JUDGE(cur);
		fread(&cur->x, sizeof(int), 1, fp);
		fread(&cur->y, sizeof(int), 1, fp);
		fread(&cur->is_empty, sizeof(bool), 1, fp);
		cur->prev = pre;
		pre->next = cur;
		pre = cur;
	}
	cur->next = NULL;
}

void reload_bullet(FILE* fp, int num) {//重新加载子弹
	bullet.head = bullet.tail = (BN*)malloc(sizeof(BN));
	JUDGE(bullet.head);
	if (!bullet.exist || !num) {
		bullet.head->next = NULL;
		bullet.exist = false;
		return;
	}
	int i;
	BN* cur, * pre;
	bullet.head->next = cur = pre = (BN*)malloc(sizeof(BN));
	JUDGE(cur);
	fread(&cur->x, sizeof(int), 1, fp);
	fread(&cur->y, sizeof(int), 1, fp);
	fread(&cur->xd, sizeof(double), 1, fp);
	fread(&cur->yd, sizeof(double), 1, fp);
	fread(&cur->is_hit, sizeof(bool), 1, fp);
	cur->prev = bullet.head;
	drawbullet(cur->x, cur->y);
	for (i = 1; i < num; i++) {
		cur = (BN*)malloc(sizeof(BN));
		JUDGE(cur);
		cur->prev = pre;
		pre->next = cur;
		pre = cur;
		fread(&cur->x, sizeof(int), 1, fp);
		fread(&cur->y, sizeof(int), 1, fp);
		fread(&cur->xd, sizeof(double), 1, fp);
		fread(&cur->yd, sizeof(double), 1, fp);
		fread(&cur->is_hit, sizeof(bool), 1, fp);
		drawbullet(cur->x, cur->y);
	}
	cur->next = NULL;
	bullet.tail = cur;
}

void reli_snake(SN* head) {//释放蛇节点
	SN* cur = head, * pre = head;
	while (cur->next != NULL) {
		cur = cur->next;
		free(pre);
		pre = cur;
	}
	free(cur);
}

void reli_foods(FN* head) {//释放某种食物节点
	if (head == NULL)return;
	FN* cur = head, * pre = head;
	while (cur->next != NULL) {
		cur = cur->next;
		free(pre);
		pre = cur;
	}
	free(cur);
}

void reli_bullet() {//释放子弹节点
	if (!bullet.exist || !player._bullet)return;
	BN* cur = bullet.head->next, * pre = bullet.head->next;
	while (cur->next != NULL) {
		cur = cur->next;
		free(pre);
		pre = cur;
	}
	free(cur);
	free(bullet.head);
	bullet.exist = false;
}

void reli() { //各种释放链表
	reli_snake(player.head);
	reli_foods(nof.head);
	reli_foods(pof.head);
	reli_foods(mif.head);
	reli_foods(acf.head);
	reli_foods(door1.head);
	reli_foods(door2.head);
	reli_foods(inside_wall.head);
	reli_bullet();
	nof.head = NULL;
	pof.head = NULL;
	mif.head = NULL;
	door1.head = NULL;
	door2.head = NULL;
	inside_wall.head = NULL;
	mon.exist = false;
}
