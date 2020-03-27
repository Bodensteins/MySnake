#include "Snake2.h"

//���ֱ�������
Snake player;//���
M mon;//����
Foods nof, pof, mif, door1, door2, inside_wall, acf;//����ʳ��
IMAGE fill(FILLIMG_X, FILLIMG_Y);//��Ϸ����
Bullet bullet;//�ӵ�����
long long grade;//�ɼ�
int t1, t2, begin, now_time;//ʱ���¼
unsigned poi_times, loop;//��ǰ������������Ϸѭ������
bool gun_exist;//����ǹ

MenuState ms;//�˵�״̬

MOUSEMSG menu_mouse;//�����Ϣ״̬
LOGFONT lg;//����״̬
//�����ļ�ָ��
FILE* load_fp1, * load_fp2, * load_fp3;
FILE* grade_fp1, * grade_fp2, * grade_fp3;

//��������
void ini_menu();
void menu_choice();
void menu_output();
void redraw_menu();

void load_menu();
void load_menu_output();
void load_menu_choice();
void redraw_load_menu();

void rank_menu();
void rank_menu_output();
void rank_menu_choice();
void redraw_rank_menu();

void game_menu();
void game_menu_output();
void game_menu_choice();
void redraw_game_menu();

void game1();
void game2();
void game3();
void load_game1();
void load_game2();
void load_game3();

void lv1_rank();
void lv2_rank();
void lv3_rank();

void game_exit();

int main() {//������
	ini_menu();
	while (!ms.is_exit) {
		menu_mouse = GetMouseMsg();
		menu_output();
		menu_choice();
	}
	return 0;
}

void ini_menu() {//��ʼ�����˵�
	initgraph(700, 500);
	setbkcolor(BK_COLOR);
	cleardevice();
	_tcscpy_s(lg.lfFaceName, _T("����"));
	lg.lfHeight = 140;
	lg.lfWidth = 80;
	lg.lfEscapement = 0;
	lg.lfItalic = true;
	lg.lfOrientation = 0;
	lg.lfStrikeOut = false;
	lg.lfUnderline = false;
	settextcolor(BLUE);
	settextstyle(&lg);
	outtextxy(130, 10, _T("SNAKE"));
	ms.is_exit = 0;
	FlushMouseMsgBuffer();
}

void menu_output() {//��ʾ���˵�
	if (menu_mouse.x > 260 && menu_mouse.x < 440 && menu_mouse.y>150 && menu_mouse.y < 210) {
		lg.lfHeight = 60;
		lg.lfWidth = 30;
		settextcolor(LIGHTBLUE);
		settextstyle(&lg);
		outtextxy(260, 150, _T("Start"));
	}
	else {
		lg.lfHeight = 60;
		lg.lfWidth = 30;
		settextcolor(RED);
		settextstyle(&lg);
		outtextxy(260, 150, _T("Start"));
	}

	if (menu_mouse.x > 260 && menu_mouse.x < 440 && menu_mouse.y>230 && menu_mouse.y < 280) {
		lg.lfHeight = 60;
		lg.lfWidth = 30;
		settextcolor(LIGHTBLUE);
		settextstyle(&lg);
		outtextxy(260, 230, _T("Load"));
	}
	else {
		lg.lfHeight = 60;
		lg.lfWidth = 30;
		settextcolor(RED);
		settextstyle(&lg);
		outtextxy(260, 230, _T("Load"));
	}

	if (menu_mouse.x > 260 && menu_mouse.x < 440 && menu_mouse.y>310 && menu_mouse.y < 360) {
		lg.lfHeight = 60;
		lg.lfWidth = 30;
		settextcolor(LIGHTBLUE);
		settextstyle(&lg);
		outtextxy(260, 310, _T("Rank"));
	}
	else {
		lg.lfHeight = 60;
		lg.lfWidth = 30;
		settextcolor(RED);
		settextstyle(&lg);
		outtextxy(260, 310, _T("Rank"));
	}

	if (menu_mouse.x > 260 && menu_mouse.x < 440 && menu_mouse.y>390 && menu_mouse.y < 440) {
		lg.lfHeight = 60;
		lg.lfWidth = 30;
		settextcolor(LIGHTBLUE);
		settextstyle(&lg);
		outtextxy(260, 390, _T("Exit"));
	}
	else {
		lg.lfHeight = 60;
		lg.lfWidth = 30;
		settextcolor(RED);
		settextstyle(&lg);
		outtextxy(260, 390, _T("Exit"));
	}
}

void menu_choice() {//�����˵�ʱ�ж��Ƿ�����Ϣ
	if (menu_mouse.mkLButton) {
		if (menu_mouse.x > 260 && menu_mouse.x < 440 && menu_mouse.y>150 && menu_mouse.y < 210) game_menu();
		else if (menu_mouse.x > 260 && menu_mouse.x < 440 && menu_mouse.y>230 && menu_mouse.y < 280)load_menu();
		else if (menu_mouse.x > 260 && menu_mouse.x < 440 && menu_mouse.y>310 && menu_mouse.y < 360)rank_menu();
		else if (menu_mouse.x > 260 && menu_mouse.x < 440 && menu_mouse.y>390 && menu_mouse.y < 440)game_exit();
	}
}

void game_menu() {//����Ϸ�˵�
	FlushMouseMsgBuffer();
	cleardevice();
	lg.lfHeight = 70;
	lg.lfWidth = 35;
	settextstyle(&lg);
	settextcolor(BLUE);
	outtextxy(180, 10, _T("GAME LEVEL"));
	ms.is_game_menu_exit = 0;
	while (!ms.is_game_menu_exit) {
		menu_mouse = GetMouseMsg();
		game_menu_output();
		game_menu_choice();
	}
	redraw_menu();
}

void redraw_menu() {//�ػ����˵�����
	cleardevice();
	lg.lfHeight = 140;
	lg.lfWidth = 80;
	lg.lfEscapement = 0;
	lg.lfItalic = true;
	lg.lfOrientation = 0;
	lg.lfStrikeOut = false;
	lg.lfUnderline = false;
	settextcolor(BLUE);
	settextstyle(&lg);
	outtextxy(130, 10, _T("SNAKE"));
}

void game_menu_output() {//��ʾ����Ϸ�˵�
	if (menu_mouse.x > 230 && menu_mouse.x < 410 && menu_mouse.y>110 && menu_mouse.y < 170) {
		lg.lfHeight = 60;
		lg.lfWidth = 30;
		settextcolor(LIGHTBLUE);
		settextstyle(&lg);
		outtextxy(230, 110, _T("Level 1"));
	}
	else {
		lg.lfHeight = 60;
		lg.lfWidth = 30;
		settextcolor(RED);
		settextstyle(&lg);
		outtextxy(230, 110, _T("Level 1"));
	}

	if (menu_mouse.x > 230 && menu_mouse.x < 410 && menu_mouse.y>190 && menu_mouse.y < 240) {
		lg.lfHeight = 60;
		lg.lfWidth = 30;
		settextcolor(LIGHTBLUE);
		settextstyle(&lg);
		outtextxy(230, 190, _T("Level 2"));
	}
	else {
		lg.lfHeight = 60;
		lg.lfWidth = 30;
		settextcolor(RED);
		settextstyle(&lg);
		outtextxy(230, 190, _T("Level 2"));
	}

	if (menu_mouse.x > 230 && menu_mouse.x < 410 && menu_mouse.y>270 && menu_mouse.y < 320) {
		lg.lfHeight = 60;
		lg.lfWidth = 30;
		settextcolor(LIGHTBLUE);
		settextstyle(&lg);
		outtextxy(230, 270, _T("Level 3"));
	}
	else {
		lg.lfHeight = 60;
		lg.lfWidth = 30;
		settextcolor(RED);
		settextstyle(&lg);
		outtextxy(230, 270, _T("Level 3"));
	}

	if (menu_mouse.x > 230 && menu_mouse.x < 410 && menu_mouse.y>350 && menu_mouse.y < 400) {
		lg.lfHeight = 60;
		lg.lfWidth = 30;
		settextcolor(LIGHTBLUE);
		settextstyle(&lg);
		outtextxy(230, 350, _T("Back"));
	}
	else {
		lg.lfHeight = 60;
		lg.lfWidth = 30;
		settextcolor(RED);
		settextstyle(&lg);
		outtextxy(230, 350, _T("Back"));
	}
}

void game_menu_choice() {//������Ϸ�˵�ʱ�ж��Ƿ��������Ϣ
	if (menu_mouse.mkLButton) {
		if (menu_mouse.x > 230 && menu_mouse.x < 410 && menu_mouse.y>110 && menu_mouse.y < 170) {
			game1();
			redraw_game_menu();
			FlushMouseMsgBuffer();
			ms.is_game1_exit = 0;
		}
		else if (menu_mouse.x > 230 && menu_mouse.x < 410 && menu_mouse.y>190 && menu_mouse.y < 240) {
			game2();
			redraw_game_menu();
			FlushMouseMsgBuffer();
			ms.is_game2_exit = 0;
		}
		else if (menu_mouse.x > 230 && menu_mouse.x < 410 && menu_mouse.y>270 && menu_mouse.y < 320) {
			game3();
			redraw_game_menu();
			FlushMouseMsgBuffer();
			ms.is_game3_exit = 0;
		}
		else if (menu_mouse.x > 230 && menu_mouse.x < 410 && menu_mouse.y>350 && menu_mouse.y < 400)ms.is_game_menu_exit = 1;
	}
}

void redraw_game_menu() {//�ػ�����Ϸ�˵�����
	initgraph(700, 500);
	setbkcolor(BK_COLOR);
	cleardevice();
	lg.lfHeight = 70;
	lg.lfWidth = 35;
	settextstyle(&lg);
	settextcolor(BLUE);
	outtextxy(180, 10, _T("GAME LEVEL"));
}

void rank_menu() {//�ɼ������˵�
	cleardevice();
	FlushMouseMsgBuffer();
	lg.lfHeight = 60;
	lg.lfWidth = 30;
	settextstyle(&lg);
	settextcolor(BLUE);
	outtextxy(120, 10, _T("HISTORICAL GRADE"));
	ms.is_rank_menu_exit = 0;
	while (!ms.is_rank_menu_exit) {
		menu_mouse = GetMouseMsg();
		rank_menu_output();
		rank_menu_choice();
	}
	redraw_menu();
	ms.is_rank_menu_exit = 0;
}

void rank_menu_output() {//��ʾ�ɼ������˵�
	if (menu_mouse.x > 230 && menu_mouse.x < 410 && menu_mouse.y>110 && menu_mouse.y < 170) {
		lg.lfHeight = 60;
		lg.lfWidth = 30;
		settextcolor(LIGHTBLUE);
		settextstyle(&lg);
		outtextxy(230, 110, _T("Level 1"));
	}
	else {
		lg.lfHeight = 60;
		lg.lfWidth = 30;
		settextcolor(RED);
		settextstyle(&lg);
		outtextxy(230, 110, _T("Level 1"));
	}

	if (menu_mouse.x > 230 && menu_mouse.x < 410 && menu_mouse.y>190 && menu_mouse.y < 240) {
		lg.lfHeight = 60;
		lg.lfWidth = 30;
		settextcolor(LIGHTBLUE);
		settextstyle(&lg);
		outtextxy(230, 190, _T("Level 2"));
	}
	else {
		lg.lfHeight = 60;
		lg.lfWidth = 30;
		settextcolor(RED);
		settextstyle(&lg);
		outtextxy(230, 190, _T("Level 2"));
	}

	if (menu_mouse.x > 230 && menu_mouse.x < 410 && menu_mouse.y>270 && menu_mouse.y < 320) {
		lg.lfHeight = 60;
		lg.lfWidth = 30;
		settextcolor(LIGHTBLUE);
		settextstyle(&lg);
		outtextxy(230, 270, _T("Level 3"));
	}
	else {
		lg.lfHeight = 60;
		lg.lfWidth = 30;
		settextcolor(RED);
		settextstyle(&lg);
		outtextxy(230, 270, _T("Level 3"));
	}

	if (menu_mouse.x > 230 && menu_mouse.x < 410 && menu_mouse.y>350 && menu_mouse.y < 410) {
		lg.lfHeight = 60;
		lg.lfWidth = 30;
		settextcolor(LIGHTBLUE);
		settextstyle(&lg);
		outtextxy(230, 350, _T("Back"));
	}
	else {
		lg.lfHeight = 60;
		lg.lfWidth = 30;
		settextcolor(RED);
		settextstyle(&lg);
		outtextxy(230, 350, _T("Back"));
	}
}

void rank_menu_choice() {//�������˵�ʱ�ж��Ƿ��������Ϣ
	if (menu_mouse.mkLButton) {
		if (menu_mouse.x > 230 && menu_mouse.x < 410 && menu_mouse.y>110 && menu_mouse.y < 170) {
			lv1_rank();
			redraw_rank_menu();
		}
		else if (menu_mouse.x > 230 && menu_mouse.x < 410 && menu_mouse.y>190 && menu_mouse.y < 240) {
			lv2_rank();
			redraw_rank_menu();
		}
		else if (menu_mouse.x > 230 && menu_mouse.x < 410 && menu_mouse.y>270 && menu_mouse.y < 320) {
			lv3_rank();
			redraw_rank_menu();
		}
		else if (menu_mouse.x > 230 && menu_mouse.x < 410 && menu_mouse.y>350 && menu_mouse.y < 410)ms.is_rank_menu_exit = 1;
	}
}

void lv1_rank() {//��ʾ��һ������
	cleardevice();
	settextstyle(&lg);
	settextcolor(BLUE);
	outtextxy(140, 10, _T("LV1 GRADE RANK"));
	long long g[5];
	TCHAR si[8];
	char s[8];
	fopen_s(&grade_fp1, "snake2_grade1.bin", "rb");
	JUDGE(grade_fp1);
	fread(g, sizeof(long long), 5, grade_fp1);
	printf("%lld %lld %lld %lld %lld", g[0], g[1], g[2], g[3], g[4]);
	fclose(grade_fp1);
	
	int i, j;
	settextcolor(DARKGRAY);
	for (i = 0; i < 5; i++) {
		lg.lfHeight = 40;
		lg.lfWidth = 20;
		settextstyle(&lg);
		sprintf_s(s, "%lld", g[i]);
		for (j = 0; *(s + j); j++)si[j] = s[j];
		si[j] = 0;
		switch (i) {
		case 0:
			outtextxy(150, 120, _T("No."));
			outtextxy(212, 120, _T("1 :"));
			outtextxy(304, 120, si);
			break;
		case 1:
			outtextxy(150, 170, _T("No."));
			outtextxy(212, 170, _T("2 :"));
			outtextxy(304, 170, si);
			break;
		case 2:
			outtextxy(150, 220, _T("No."));
			outtextxy(212, 220, _T("3 :"));
			outtextxy(304, 220, si);
			break;
		case 3:
			outtextxy(150, 270, _T("No."));
			outtextxy(212, 270, _T("4 :"));
			outtextxy(304, 270, si);
			break;
		case 4:
			outtextxy(150, 320, _T("No."));
			outtextxy(212, 320, _T("5 :"));
			outtextxy(304, 320, si);
			break;
		}

	}
	lg.lfHeight = 20;
	lg.lfWidth = 10;
	settextcolor(RED);
	settextstyle(&lg);
	outtextxy(260, 85, _T("Grade = Score * Hp"));
	lg.lfHeight = 30;
	lg.lfWidth = 15;
	settextstyle(&lg);
	outtextxy(160, 400, _T("Press any key to get back"));
	
	_getch();
	FlushMouseMsgBuffer();
	closegraph();
	
}

void lv2_rank() {//��ʾ�ڶ�������
	cleardevice();
	settextstyle(&lg);
	settextcolor(BLUE);
	outtextxy(140, 10, _T("LV2 GRADE RANK"));
	long long g[5];
	TCHAR si[8];
	char s[8];
	fopen_s(&grade_fp2, "snake2_grade2.bin", "rb");
	JUDGE(grade_fp2);
	fread(g, sizeof(long long), 5, grade_fp2);
	fclose(grade_fp2);
	int i, j;
	settextcolor(DARKGRAY);
	for (i = 0; i < 5; i++) {
		lg.lfHeight = 40;
		lg.lfWidth = 20;
		settextstyle(&lg);
		sprintf_s(s, "%lld", g[i]);
		for (j = 0; *(s + j); j++)si[j] = s[j];
		si[j] = 0;
		switch (i) {
		case 0:
			outtextxy(150, 120, _T("No."));
			outtextxy(212, 120, _T("1 :"));
			outtextxy(304, 120, si);
			break;
		case 1:
			outtextxy(150, 170, _T("No."));
			outtextxy(212, 170, _T("2 :"));
			outtextxy(304, 170, si);
			break;
		case 2:
			outtextxy(150, 220, _T("No."));
			outtextxy(212, 220, _T("3 :"));
			outtextxy(304, 220, si);
			break;
		case 3:
			outtextxy(150, 270, _T("No."));
			outtextxy(212, 270, _T("4 :"));
			outtextxy(304, 270, si);
			break;
		case 4:
			outtextxy(150, 320, _T("No."));
			outtextxy(212, 320, _T("5 :"));
			outtextxy(304, 320, si);
			break;
		}

	}
	lg.lfHeight = 20;
	lg.lfWidth = 10;
	settextcolor(RED);
	settextstyle(&lg);
	outtextxy(260, 85, _T("Grade = Score * Hp"));
	lg.lfHeight = 30;
	lg.lfWidth = 15;
	settextcolor(RED);
	settextstyle(&lg);
	outtextxy(160, 400, _T("Press any key to get back"));
	_getch();
	FlushMouseMsgBuffer();
	closegraph();
}

void lv3_rank() {//��ʾ����������
	cleardevice();
	settextstyle(&lg);
	settextcolor(BLUE);
	outtextxy(140, 10, _T("LV3 GRADE RANK"));
	long long g[5];
	TCHAR si[8];
	char s[8];
	fopen_s(&grade_fp3, "snake2_grade3.bin", "rb");
	JUDGE(grade_fp3);
	fread(g, sizeof(long long), 5, grade_fp3);
	fclose(grade_fp3);
	int i, j;
	settextcolor(DARKGRAY);
	for (i = 0; i < 5; i++) {
		lg.lfHeight = 40;
		lg.lfWidth = 20;
		settextstyle(&lg);
		sprintf_s(s, "%lld", g[i]);
		for (j = 0; *(s + j); j++)si[j] = s[j];
		si[j] = 0;
		switch (i) {
		case 0:
			outtextxy(150, 120, _T("No."));
			outtextxy(212, 120, _T("1 :"));
			outtextxy(304, 120, si);
			break;
		case 1:
			outtextxy(150, 170, _T("No."));
			outtextxy(212, 170, _T("2 :"));
			outtextxy(304, 170, si);
			break;
		case 2:
			outtextxy(150, 220, _T("No."));
			outtextxy(212, 220, _T("3 :"));
			outtextxy(304, 220, si);
			break;
		case 3:
			outtextxy(150, 270, _T("No."));
			outtextxy(212, 270, _T("4 :"));
			outtextxy(304, 270, si);
			break;
		case 4:
			outtextxy(150, 320, _T("No."));
			outtextxy(212, 320, _T("5 :"));
			outtextxy(304, 320, si);
			break;
		}

	}
	lg.lfHeight = 20;
	lg.lfWidth = 10;
	settextcolor(RED);
	settextstyle(&lg);
	outtextxy(260, 85, _T("Grade = Score * Hp"));
	lg.lfHeight = 30;
	lg.lfWidth = 15;
	settextcolor(RED);
	settextstyle(&lg);
	outtextxy(160, 400, _T("Press any key to get back"));
	_getch();
	FlushMouseMsgBuffer();
	closegraph();
}

void redraw_rank_menu() {//�ػ��ɼ������˵�����
	initgraph(700, 500);
	setbkcolor(BK_COLOR);
	cleardevice();
	lg.lfHeight = 60;
	lg.lfWidth = 30;
	settextstyle(&lg);
	settextcolor(BLUE);
	outtextxy(120, 10, _T("HISTORICAL GRADE"));
}

void load_menu() {//��Ϸ�����˵�
	FlushMouseMsgBuffer();
	cleardevice();
	lg.lfHeight = 60;
	lg.lfWidth = 30;
	settextstyle(&lg);
	settextcolor(BLUE);
	outtextxy(210, 10, _T("LOAD GAME"));
	ms.is_load_menu_exit = 0;
	while (!ms.is_load_menu_exit) {
		menu_mouse = GetMouseMsg();
		load_menu_output();
		load_menu_choice();
	}
	redraw_menu();
	ms.is_load_menu_exit = 0;
}

void load_menu_output() {//��ʾ��Ϸ�����˵�
	if (menu_mouse.x > 230 && menu_mouse.x < 410 && menu_mouse.y>110 && menu_mouse.y < 170) {
		lg.lfHeight = 60;
		lg.lfWidth = 30;
		settextcolor(LIGHTBLUE);
		settextstyle(&lg);
		outtextxy(230, 110, _T("Level 1"));
	}
	else {
		lg.lfHeight = 60;
		lg.lfWidth = 30;
		settextcolor(RED);
		settextstyle(&lg);
		outtextxy(230, 110, _T("Level 1"));
	}

	if (menu_mouse.x > 230 && menu_mouse.x < 410 && menu_mouse.y>190 && menu_mouse.y < 240) {
		lg.lfHeight = 60;
		lg.lfWidth = 30;
		settextcolor(LIGHTBLUE);
		settextstyle(&lg);
		outtextxy(230, 190, _T("Level 2"));
	}
	else {
		lg.lfHeight = 60;
		lg.lfWidth = 30;
		settextcolor(RED);
		settextstyle(&lg);
		outtextxy(230, 190, _T("Level 2"));
	}

	if (menu_mouse.x > 230 && menu_mouse.x < 410 && menu_mouse.y>270 && menu_mouse.y < 320) {
		lg.lfHeight = 60;
		lg.lfWidth = 30;
		settextcolor(LIGHTBLUE);
		settextstyle(&lg);
		outtextxy(230, 270, _T("Level 3"));
	}
	else {
		lg.lfHeight = 60;
		lg.lfWidth = 30;
		settextcolor(RED);
		settextstyle(&lg);
		outtextxy(230, 270, _T("Level 3"));
	}

	if (menu_mouse.x > 230 && menu_mouse.x < 410 && menu_mouse.y>350 && menu_mouse.y < 410) {
		lg.lfHeight = 60;
		lg.lfWidth = 30;
		settextcolor(LIGHTBLUE);
		settextstyle(&lg);
		outtextxy(230, 350, _T("Back"));
	}
	else {
		lg.lfHeight = 60;
		lg.lfWidth = 30;
		settextcolor(RED);
		settextstyle(&lg);
		outtextxy(230, 350, _T("Back"));
	}
}

void load_menu_choice(){//����Ϸ�����˵�ʱ�ж��Ƿ��������Ϣ
	if (menu_mouse.mkLButton) {
		if (menu_mouse.x > 230 && menu_mouse.x < 410 && menu_mouse.y>110 && menu_mouse.y < 170) {
			load_game1();
			redraw_load_menu();
			FlushMouseMsgBuffer();
			ms.is_game1_exit = 0;
		}
		else if (menu_mouse.x > 230 && menu_mouse.x < 410 && menu_mouse.y>190 && menu_mouse.y < 240) {
			load_game2();
			redraw_load_menu();
			FlushMouseMsgBuffer();
			ms.is_game2_exit = 0;
		}
		else if (menu_mouse.x > 230 && menu_mouse.x < 410 && menu_mouse.y>270 && menu_mouse.y < 320) {
			load_game3();
			redraw_load_menu();
			FlushMouseMsgBuffer();
			ms.is_game3_exit = 0;
		}
		else if (menu_mouse.x > 230 && menu_mouse.x < 410 && menu_mouse.y>350 && menu_mouse.y < 400)ms.is_load_menu_exit = 1;
	}
}

void redraw_load_menu() {//�ػ���Ϸ�����˵�����
	initgraph(700, 500);
	setbkcolor(BK_COLOR);
	cleardevice();
	lg.lfHeight = 60;
	lg.lfWidth = 30;
	settextstyle(&lg);
	settextcolor(BLUE);
	outtextxy(210, 10, _T("LOAD GAME"));
}

void game_exit() {//���˵��˳�
	ms.is_exit = true;
}

void end() {//��Ϸ����
	initgraph(700, 500);
	setbkcolor(WHITE);
	cleardevice();
	LOGFONT f;
	gettextstyle(&f);
	f.lfHeight = 30;
	f.lfWidth = 15;
	_tcscpy_s(f.lfFaceName, _T("����"));
	settextstyle(&f);
	settextcolor(RED);
	TCHAR si[20];
	char s[20];
	int i;
	sprintf_s(s, "%d", player.score);
	for (i = 0; *(s + i); i++)si[i] = s[i];
	si[i] = 0;
	outtextxy(121, 46, _T("Your final score: "));
	outtextxy(383, 46, si);
	sprintf_s(s, "%lld", grade);
	for (i = 0; *(s + i); i++)si[i] = s[i];
	si[i] = 0;
	outtextxy(121, 156, _T("Your final grade: "));
	outtextxy(383, 156, si);
	settextcolor(BLUE);
	_tcscpy_s(f.lfFaceName, _T("����"));
	settextstyle(&f);
	outtextxy(121, 300, _T("Press any key to get back "));
	_getch();
	closegraph();
}
