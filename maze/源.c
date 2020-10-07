#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define SQUARE 100 //迷宫最大行列数
#define FILENAME_MAX_LENGTH 64
#define ERROR 0
#define OK 1

struct pos { 
	int x, y;
}; //坐标

struct node {
	struct node* pre; //上一个元素
	struct pos p; //坐标信息
	int dir;  //方向，1右,2下,3左,4上
}; //迷宫路径记录结点

struct maze {
	int r, c;
	char m[SQUARE + 2][SQUARE + 2]; //0表示通路,1表示墙壁,2表示已走过的路径
	struct pos s; //起点位置信息
	struct pos e; //终点位置信息
}; //迷宫结构

struct stack {
	struct node* top; //栈顶指针
	int size; //栈中元素数
}; //栈结构

int stack_init(struct stack* S) {
	S->top = NULL;
	S->size = 0;
	return OK;
} //栈初始化

int stack_push(struct stack* S, struct node e) {
	struct node* p = (struct node*)malloc(sizeof(struct node));
	if (!p) { printf("overflow during stack_push!\n"); return ERROR; /*exit(0);*/ }
	p->dir = e.dir;
	p->p = e.p;
	p->pre = S->top; //新增元素的上一位是原先的top
	S->top = p; //新增元素到栈顶
	++S->size;
	return OK;
} //指定元素e（element）入栈S

struct node stack_pop(struct stack* S) {
	struct node popnode; //此非指针，而是node结构，记录出栈元素信息
	struct node* p; //临时指针，指向出栈前的top
	if (S->size == 0) {
		printf("this maze have no path！\n");
		popnode.dir = 5; //设方向为5（“不可能方向”）
		return popnode;
	} //栈无元素，迷宫无法走通
	popnode.dir = S->top->dir;
	popnode.p = S->top->p;
	popnode.pre = NULL;
	p = S->top;
	S->top = S->top->pre; //栈回溯
	free(p); //释放原top位置
	S->size--; //栈元素减一
	return popnode;
} //指定栈顶元素e出栈S

void stack_destory(struct stack* S) {
	while (S->size) stack_pop(S); //忽视stack_pop的返回值，相当于直接清除
	return;
} //栈删除

void stack_top(struct stack* S) {
	char c[3] = { 0 };
	struct node* p = S->top;
	if (p->dir == 1) strcpy(c, "→");
	else if (p->dir == 2) strcpy(c, "↓");
	else if (p->dir == 3) strcpy(c, "←");
	else if (p->dir == 4) strcpy(c, "↑");
	printf("%d\t%d\t%d\t%s\n", S->size, p->p.x, p->p.y, c);
	return;
} //取栈顶元素（同时输出当前栈大小），测试路径点用

void stack_show(struct stack* S) {
	int i = S->size, endx = 0, endy = 0;
	char c[3];
	struct node* p = S->top;
	if (p->dir == 1) { endx = p->p.x; endy = p->p.y + 1; } //1右,2下,3左,4上，求迷宫出口坐标（或当前抵达位置）
	else if (p->dir == 2) { endx = p->p.x + 1; endy = p->p.y; }
	else if (p->dir == 3) { endx = p->p.x; endy = p->p.y - 1; }
	else if (p->dir == 4) { endx = p->p.x - 1; endy = p->p.y; }
	printf("step\tline\trow\tdirection\nend\t%d\t%d\t⊙\n", endx, endy);
	while (p) {
		memset(c, 0, 3);
		if (p->dir == 1) strcpy(c, "→");
		else if (p->dir == 2) strcpy(c, "↓");
		else if (p->dir == 3) strcpy(c, "←");
		else if (p->dir == 4) strcpy(c, "↑");
		i--;
		if (i > 0) printf("%d\t%d\t%d\t%s\n", i, p->p.x, p->p.y, c);
		else printf("start\t%d\t%d\t%s\n", p->p.x, p->p.y, c);
		p = p->pre;
	} //遍历栈元素（向栈顶向前）
} //栈元素显示

void dispmaze(struct maze* a_maze) {
	int i, j;
	printf("□route  █wall  ╋ start  ⊙end  ╳ can't through\n");
	for (i = 0; i <= a_maze->r + 1; i++) {
		for (j = 0; j <= a_maze->c + 1; j++) {
			if (i == a_maze->s.x && j == a_maze->s.y) printf("╋ "); //起点
			else if (i == a_maze->e.x && j == a_maze->e.y) printf("⊙"); //终点
			else {
				if (a_maze->m[i][j] == '0') printf("□");
				if (a_maze->m[i][j] == '1') printf("█");
				if (a_maze->m[i][j] == '2') printf("╳ ");
				if (a_maze->m[i][j] == 'A') printf("→");
				if (a_maze->m[i][j] == 'B') printf("↓");
				if (a_maze->m[i][j] == 'C') printf("←");
				if (a_maze->m[i][j] == 'D') printf("↑");
				if (a_maze->m[i][j] == 'I') printf("┌ "); //┏┌改为◤则是全角，或加空格
				if (a_maze->m[i][j] == 'F') printf("┐ "); //┓┐改为◥则是全角，或加空格
				if (a_maze->m[i][j] == 'G') printf("┘ "); //┛┘改为◢则是全角，或加空格
				if (a_maze->m[i][j] == 'H') printf("└ "); //┗└改为◣则是全角，或加空格
			}
		}
		printf("\n");
	}
	return;
}

int savemaze(struct maze* a_maze) {
	char y_or_n[2] = { 0 };
	printf("save this maze image?(y/n):");
	scanf_s("%s", &y_or_n, 2);
	if (y_or_n[0] != 89 && y_or_n[0] != 121) return ERROR;
	char outname[FILENAME_MAX_LENGTH] = { 0 };
	int i, j;
	FILE* fp;
	printf("save as (file name):");
	scanf_s("%s", &outname, FILENAME_MAX_LENGTH);
	fopen_s(&fp, outname, "w");
	if (!fp) { printf("can't open %s\n", outname); return ERROR; }
	fprintf(fp, "□route  █wall  ╋ start  ⊙end  ╳ can't through\n");
	for (i = 0; i <= a_maze->r + 1; i++) {
		for (j = 0; j <= a_maze->c + 1; j++) {
			if (i == a_maze->s.x && j == a_maze->s.y) fprintf(fp, "╋"); //起点
			else if (i == a_maze->e.x && j == a_maze->e.y) fprintf(fp, "⊙"); //终点
			else {
				if (a_maze->m[i][j] == '0') fprintf(fp, "□");
				if (a_maze->m[i][j] == '1') fprintf(fp, "█");
				if (a_maze->m[i][j] == '2') fprintf(fp, "╳");
				if (a_maze->m[i][j] == 'A') fprintf(fp, "→");
				if (a_maze->m[i][j] == 'B') fprintf(fp, "↓");
				if (a_maze->m[i][j] == 'C') fprintf(fp, "←");
				if (a_maze->m[i][j] == 'D') fprintf(fp, "↑");
				if (a_maze->m[i][j] == 'I') fprintf(fp, "┌"); //┏┌改为◤则是全角，或加空格
				if (a_maze->m[i][j] == 'F') fprintf(fp, "┐"); //┓┐改为◥则是全角，或加空格
				if (a_maze->m[i][j] == 'G') fprintf(fp, "┘"); //┛┘改为◢则是全角，或加空格
				if (a_maze->m[i][j] == 'H') fprintf(fp, "└"); //┗└改为◣则是全角，或加空格
			}
		}
		fprintf(fp, "\n");
	}
	return;
}

int inputmaze_data(struct maze* a_maze) {
	char inname[FILENAME_MAX_LENGTH] = { 0 };
	int i, j;
	char row[SQUARE + 10];
	FILE* fp;
	int s, e;//有无起点和终点
	s = 0, e = 0;
	printf("input maze data from (file name):");
	scanf_s("%s", &inname, FILENAME_MAX_LENGTH);
	fopen_s(&fp, inname, "r");
	if (!fp) { printf("can't open %s\n", inname); return ERROR; }
	fscanf_s(fp, "%d,%d", &(a_maze->r), &(a_maze->c)); //迷宫限定长宽储存于r、c，此信息在迷宫输入中位于第一行以%d,%d形式写
	fgetc(fp); //读取换行符
	for (i = 0; i < a_maze->r + 2; i++)
		for (j = 0; j < a_maze->c + 2; j++)
			a_maze->m[i][j] = '1';//先全部填充为1（1代表墙壁）
	for (i = 1; i <= a_maze->r; i++) { //跳过迷宫输入第一行的迷宫边界
		fgets(row, SQUARE + 10, fp); //从fp中整行读取
		for (j = 1; j <= a_maze->c; j++) { //跳过第一列的迷宫边界
			a_maze->m[i][j] = row[j - 1]; //逐列录入该行迷宫位点情况（墙或路或起点或终点）
			if (a_maze->m[i][j] == 'S') {
				a_maze->s.x = i;
				a_maze->s.y = j;
				s = 1;
			} //起点被标记为S，录入其位置到s
			else if (a_maze->m[i][j] == 'E') {
				a_maze->e.x = i;
				a_maze->e.y = j;
				e = 1;
			} //终点被标记为E，录入其位置到e
		}
	}
	fclose(fp);
	if (s + e < 2) {
		printf("can't find start or end position of this maze %s\n", inname);
		return ERROR;
	}
	return OK;
} //输入迷宫

int outputmaze_data(struct maze* a_maze) {
	char y_or_n[2] = { 0 };
	printf("output maze data?(y/n):");
	scanf_s("%s", &y_or_n, 2);
	if (y_or_n[0] != 89 && y_or_n[0] != 121) return ERROR;
	char outname[FILENAME_MAX_LENGTH] = { 0 };
	int i, j;
	FILE* fp;
	printf("output maze data to (file name):");
	scanf_s("%s", &outname, FILENAME_MAX_LENGTH);
	fopen_s(&fp, outname, "w");
	if (!fp) { printf("can't open %s\n", outname); return ERROR; }
	fprintf(fp, "%d,%d\n", a_maze->r, a_maze->c);
	for (i = 1; i <= a_maze->r; i++) {
		for (j = 1; j <= a_maze->c; j++) {
			if (i == a_maze->s.x && j == a_maze->s.y) fprintf(fp, "S");
			else if (i == a_maze->e.x && j == a_maze->e.y) fprintf(fp, "E");
			else fprintf(fp, "%c", a_maze->m[i][j]);
		}
		fprintf(fp, "\n");
	} //不输出边界
	fprintf(fp, "\n\n\n\n0路  1墙  S起点  E终点  2不通路  A右   B下  C左  D上  F右下/上左  G右上/下左  H左上/下右  I左下/上右\n");
	fclose(fp);
	return OK;
} //输出迷宫

void drawpath(struct maze* a_maze) {
	struct stack stackstruct, * s = &stackstruct; //s为指向stackstruct（stack类型）的指针
	int x, lastx, y, lasty, dir, lastdir; //记录指向当前行、列、方向的值及它们三者在上一位的值
	struct node n;
	x = a_maze->s.x;
	y = a_maze->s.y;
	dir = 1; //初始方向为1
	stack_init(s);
	printf("\nstack status:\n");
	while (a_maze->m[x][y] != 'E') { //x为行，y为列，1右,2下,3左,4上
		if (x < a_maze->e.x && y < a_maze->e.y) {
			if (a_maze->m[x][y + 1] == '0' || a_maze->m[x][y + 1] == 'E') {
				if (a_maze->m[x][y + 1] == '0') a_maze->m[x][y + 1] = '2';
				n.dir = 1; n.p.x = x; n.p.y = y; y += 1; stack_push(s, n);
			}
			else if (a_maze->m[x + 1][y] == '0' || a_maze->m[x + 1][y] == 'E') {
				if (a_maze->m[x + 1][y] == '0') a_maze->m[x + 1][y] = '2';
				n.dir = 2; n.p.x = x; n.p.y = y; x += 1; stack_push(s, n);
			}
			else if (a_maze->m[x - 1][y] == '0' || a_maze->m[x - 1][y] == 'E') {
				if (a_maze->m[x - 1][y] == '0') a_maze->m[x - 1][y] = '2';
				n.dir = 4; n.p.x = x; n.p.y = y; x -= 1; stack_push(s, n);
			}
			else if (a_maze->m[x][y - 1] == '0' || a_maze->m[x][y - 1] == 'E') {
				if (a_maze->m[x][y - 1] == '0') a_maze->m[x][y - 1] = '2';
				n.dir = 3; n.p.x = x; n.p.y = y; y -= 1; stack_push(s, n);
			}
			else {
				n = stack_pop(s);
				if (n.dir == 5) return; //“不可能方向”5表示迷宫无解，退出
				x = n.p.x; y = n.p.y;
			}
		} //终点在右下方，优先顺序右下上左1243
		else if (x == a_maze->e.x && y < a_maze->e.y) {
			if (a_maze->m[x][y + 1] == '0' || a_maze->m[x][y + 1] == 'E') {
				if (a_maze->m[x][y + 1] == '0') a_maze->m[x][y + 1] = '2';
				n.dir = 1; n.p.x = x; n.p.y = y; y += 1; stack_push(s, n);
			}
			else if (a_maze->m[x - 1][y] == '0' || a_maze->m[x - 1][y] == 'E') {
				if (a_maze->m[x - 1][y] == '0') a_maze->m[x - 1][y] = '2';
				n.dir = 4; n.p.x = x; n.p.y = y; x -= 1; stack_push(s, n);
			}
			else if (a_maze->m[x + 1][y] == '0' || a_maze->m[x + 1][y] == 'E') {
				if (a_maze->m[x + 1][y] == '0') a_maze->m[x + 1][y] = '2';
				n.dir = 2; n.p.x = x; n.p.y = y; x += 1; stack_push(s, n);
			}
			else if (a_maze->m[x][y - 1] == '0' || a_maze->m[x][y - 1] == 'E') {
				if (a_maze->m[x][y - 1] == '0') a_maze->m[x][y - 1] = '2';
				n.dir = 3; n.p.x = x; n.p.y = y; y -= 1; stack_push(s, n);
			}
			else {
				n = stack_pop(s);
				if (n.dir == 5) return; //“不可能方向”5表示迷宫无解，退出
				x = n.p.x; y = n.p.y;
			}
		} //终点在正右方，优先顺序右上下左1423
		else if (x > a_maze->e.x && y < a_maze->e.y) {
			if (a_maze->m[x - 1][y] == '0' || a_maze->m[x - 1][y] == 'E') {
				if (a_maze->m[x - 1][y] == '0') a_maze->m[x - 1][y] = '2';
				n.dir = 4; n.p.x = x; n.p.y = y; x -= 1; stack_push(s, n);
			}
			else if (a_maze->m[x][y + 1] == '0' || a_maze->m[x][y + 1] == 'E') {
				if (a_maze->m[x][y + 1] == '0') a_maze->m[x][y + 1] = '2';
				n.dir = 1; n.p.x = x; n.p.y = y; y += 1; stack_push(s, n);
			}
			else if (a_maze->m[x + 1][y] == '0' || a_maze->m[x + 1][y] == 'E') {
				if (a_maze->m[x + 1][y] == '0') a_maze->m[x + 1][y] = '2';
				n.dir = 2; n.p.x = x; n.p.y = y; x += 1; stack_push(s, n);
			}
			else if (a_maze->m[x][y - 1] == '0' || a_maze->m[x][y - 1] == 'E') {
				if (a_maze->m[x][y - 1] == '0') a_maze->m[x][y - 1] = '2';
				n.dir = 3; n.p.x = x; n.p.y = y; y -= 1; stack_push(s, n);
			}
			else {
				n = stack_pop(s);
				if (n.dir == 5) return; //“不可能方向”5表示迷宫无解，退出
				x = n.p.x; y = n.p.y;
			}
		} //终点在右上方，优先顺序上右下左4123
		else if (x < a_maze->e.x && y > a_maze->e.y) {
			if (a_maze->m[x + 1][y] == '0' || a_maze->m[x + 1][y] == 'E') {
				if (a_maze->m[x + 1][y] == '0') a_maze->m[x + 1][y] = '2';
				n.dir = 2; n.p.x = x; n.p.y = y; x += 1; stack_push(s, n);
			}
			else if (a_maze->m[x][y - 1] == '0' || a_maze->m[x][y - 1] == 'E') {
				if (a_maze->m[x][y - 1] == '0') a_maze->m[x][y - 1] = '2';
				n.dir = 3; n.p.x = x; n.p.y = y; y -= 1; stack_push(s, n);
			}
			else if (a_maze->m[x - 1][y] == '0' || a_maze->m[x - 1][y] == 'E') {
				if (a_maze->m[x - 1][y] == '0') a_maze->m[x - 1][y] = '2';
				n.dir = 4; n.p.x = x; n.p.y = y; x -= 1; stack_push(s, n);
			}
			else if (a_maze->m[x][y + 1] == '0' || a_maze->m[x][y + 1] == 'E') {
				if (a_maze->m[x][y + 1] == '0') a_maze->m[x][y + 1] = '2';
				n.dir = 1; n.p.x = x; n.p.y = y; y += 1; stack_push(s, n);
			}
			else {
				n = stack_pop(s);
				if (n.dir == 5) return; //“不可能方向”5表示迷宫无解，退出
				x = n.p.x; y = n.p.y;
			}
		} //终点在左下方，优先顺序下左上右2341
		else if (x == a_maze->e.x && y > a_maze->e.y) {
			if (a_maze->m[x][y - 1] == '0' || a_maze->m[x][y - 1] == 'E') {
				if (a_maze->m[x][y - 1] == '0') a_maze->m[x][y - 1] = '2';
				n.dir = 3; n.p.x = x; n.p.y = y; y -= 1; stack_push(s, n);
			}
			else if (a_maze->m[x + 1][y] == '0' || a_maze->m[x + 1][y] == 'E') {
				if (a_maze->m[x + 1][y] == '0') a_maze->m[x + 1][y] = '2';
				n.dir = 2; n.p.x = x; n.p.y = y; x += 1; stack_push(s, n);
			}
			else if (a_maze->m[x - 1][y] == '0' || a_maze->m[x - 1][y] == 'E') {
				if (a_maze->m[x - 1][y] == '0') a_maze->m[x - 1][y] = '2';
				n.dir = 4; n.p.x = x; n.p.y = y; x -= 1; stack_push(s, n);
			}
			else if (a_maze->m[x][y + 1] == '0' || a_maze->m[x][y + 1] == 'E') {
				if (a_maze->m[x][y + 1] == '0') a_maze->m[x][y + 1] = '2';
				n.dir = 1; n.p.x = x; n.p.y = y; y += 1; stack_push(s, n);
			}
			else {
				n = stack_pop(s);
				if (n.dir == 5) return; //“不可能方向”5表示迷宫无解，退出
				x = n.p.x; y = n.p.y;
			}
		} //终点在正左方，优先顺序左下上右3241
		else if (x > a_maze->e.x && y > a_maze->e.y) {
			if (a_maze->m[x][y - 1] == '0' || a_maze->m[x][y - 1] == 'E') {
				if (a_maze->m[x][y - 1] == '0') a_maze->m[x][y - 1] = '2';
				n.dir = 3; n.p.x = x; n.p.y = y; y -= 1; stack_push(s, n);
			}
			else if (a_maze->m[x - 1][y] == '0' || a_maze->m[x - 1][y] == 'E') {
				if (a_maze->m[x - 1][y] == '0') a_maze->m[x - 1][y] = '2';
				n.dir = 4; n.p.x = x; n.p.y = y; x -= 1; stack_push(s, n);
			}
			else if (a_maze->m[x + 1][y] == '0' || a_maze->m[x + 1][y] == 'E') {
				if (a_maze->m[x + 1][y] == '0') a_maze->m[x + 1][y] = '2';
				n.dir = 2; n.p.x = x; n.p.y = y; x += 1; stack_push(s, n);
			}
			else if (a_maze->m[x][y + 1] == '0' || a_maze->m[x][y + 1] == 'E') {
				if (a_maze->m[x][y + 1] == '0') a_maze->m[x][y + 1] = '2';
				n.dir = 1; n.p.x = x; n.p.y = y; y += 1; stack_push(s, n);
			}
			else {
				n = stack_pop(s);
				if (n.dir == 5) return; //“不可能方向”5表示迷宫无解，退出
				x = n.p.x; y = n.p.y;
			}
		} //终点在左上方，优先顺序左上下右3421
		else if (x < a_maze->e.x && y == a_maze->e.y) {
			if (a_maze->m[x + 1][y] == '0' || a_maze->m[x + 1][y] == 'E') {
				if (a_maze->m[x + 1][y] == '0') a_maze->m[x + 1][y] = '2';
				n.dir = 2; n.p.x = x; n.p.y = y; x += 1; stack_push(s, n);
			}
			else if (a_maze->m[x][y - 1] == '0' || a_maze->m[x][y - 1] == 'E') {
				if (a_maze->m[x][y - 1] == '0') a_maze->m[x][y - 1] = '2';
				n.dir = 3; n.p.x = x; n.p.y = y; y -= 1; stack_push(s, n);
			}
			else if (a_maze->m[x][y + 1] == '0' || a_maze->m[x][y + 1] == 'E') {
				if (a_maze->m[x][y + 1] == '0') a_maze->m[x][y + 1] = '2';
				n.dir = 1; n.p.x = x; n.p.y = y; y += 1; stack_push(s, n);
			}
			else if (a_maze->m[x - 1][y] == '0' || a_maze->m[x - 1][y] == 'E') {
				if (a_maze->m[x - 1][y] == '0') a_maze->m[x - 1][y] = '2';
				n.dir = 4; n.p.x = x; n.p.y = y; x -= 1; stack_push(s, n);
			}
			else {
				n = stack_pop(s);
				if (n.dir == 5) return; //“不可能方向”5表示迷宫无解，退出
				x = n.p.x; y = n.p.y;
			}
		} //终点在正下方，优先顺序下左右上2314
		else if (x > a_maze->e.x && y == a_maze->e.y) {
			if (a_maze->m[x - 1][y] == '0' || a_maze->m[x - 1][y] == 'E') {
				if (a_maze->m[x - 1][y] == '0') a_maze->m[x - 1][y] = '2';
				n.dir = 4; n.p.x = x; n.p.y = y; x -= 1; stack_push(s, n); 
			}
			else if (a_maze->m[x][y + 1] == '0' || a_maze->m[x][y + 1] == 'E') {
				if (a_maze->m[x][y + 1] == '0') a_maze->m[x][y + 1] = '2';
				n.dir = 1; n.p.x = x; n.p.y = y; y += 1; stack_push(s, n);
			}
			else if (a_maze->m[x][y - 1] == '0' || a_maze->m[x][y - 1] == 'E') {
				if (a_maze->m[x][y - 1] == '0') a_maze->m[x][y - 1] = '2';
				n.dir = 3; n.p.x = x; n.p.y = y; y -= 1; stack_push(s, n);
			}
			else if (a_maze->m[x + 1][y] == '0' || a_maze->m[x + 1][y] == 'E') {
				if (a_maze->m[x + 1][y] == '0') a_maze->m[x + 1][y] = '2';
				n.dir = 2; n.p.x = x; n.p.y = y; x += 1; stack_push(s, n);
			}
			else {
				n = stack_pop(s);
				if (n.dir == 5) return; //“不可能方向”5表示迷宫无解，退出
				x = n.p.x; y = n.p.y;
			}
		} //终点在正上方，优先顺序上右左下4132
		stack_top(s);
		/*
		if (a_maze->m[x][y + 1] == '0' || a_maze->m[x][y + 1] == 'E') {
			if (a_maze->m[x][y + 1] == '0') a_maze->m[x][y + 1] = '2';
			n.dir = 1; n.p.x = x; n.p.y = y; y += 1; stack_push(s, n);
		}
		else if (a_maze->m[x + 1][y] == '0' || a_maze->m[x + 1][y] == 'E') {
			if (a_maze->m[x + 1][y] == '0') a_maze->m[x + 1][y] = '2';
			n.dir = 2; n.p.x = x; n.p.y = y; x += 1; stack_push(s, n);
		}
		else if (a_maze->m[x][y - 1] == '0' || a_maze->m[x][y - 1] == 'E') {
			if (a_maze->m[x][y - 1] == '0') a_maze->m[x][y - 1] = '2';
			n.dir = 3; n.p.x = x; n.p.y = y; y -= 1; stack_push(s, n);
		}
		else if (a_maze->m[x - 1][y] == '0' || a_maze->m[x - 1][y] == 'E') {
			if (a_maze->m[x - 1][y] == '0') a_maze->m[x - 1][y] = '2';
			n.dir = 4; n.p.x = x; n.p.y = y; x -= 1; stack_push(s, n);
		}
		else {
			n = stack_pop(s);
			if (n.dir == 5) return; //“不可能方向”5表示迷宫无解，退出
			x = n.p.x; y = n.p.y;
		}
		*/
	}
	printf("\nmaze path:\n");
	stack_show(s);
	printf("\n");
	n = stack_pop(s); //此时栈顶为终点，终点出栈
	x = n.p.x;
	y = n.p.y;
	dir = n.dir;
	while (s->size != 0) { //方向（dir），1右,2下,3左,4上
		n = stack_pop(s); //路径点逐个出栈，将最终栈的路径画下（取代原有的2（已走过路径））
		lastx = n.p.x;
		lasty = n.p.y;
		lastdir = n.dir;
		if (lastdir == 1 && dir == 1) a_maze->m[x][y] = 'A';      //上个方向向右，本方向向右，为→
		else if (lastdir == 1 && dir == 2) a_maze->m[x][y] = 'F'; //上个方向向右，本方向向下，为┐
		else if (lastdir == 1 && dir == 4) a_maze->m[x][y] = 'G'; //上个方向向右，本方向向上，为┘
		else if (lastdir == 2 && dir == 1) a_maze->m[x][y] = 'H'; //上个方向向下，本方向向右，为└
		else if (lastdir == 2 && dir == 2) a_maze->m[x][y] = 'B'; //上个方向向下，本方向向下，为↓
		else if (lastdir == 2 && dir == 3) a_maze->m[x][y] = 'G'; //上个方向向下，本方向向左，为┘
		else if (lastdir == 3 && dir == 2) a_maze->m[x][y] = 'I'; //上个方向向左，本方向向下，为┌
		else if (lastdir == 3 && dir == 3) a_maze->m[x][y] = 'C'; //上个方向向左，本方向向左，为←
		else if (lastdir == 3 && dir == 4) a_maze->m[x][y] = 'H'; //上个方向向左，本方向向上，为└
		else if (lastdir == 4 && dir == 1) a_maze->m[x][y] = 'I'; //上个方向向上，本方向向右，为┌
		else if (lastdir == 4 && dir == 3) a_maze->m[x][y] = 'F'; //上个方向向上，本方向向左，为┐
		else if (lastdir == 4 && dir == 4) a_maze->m[x][y] = 'D'; //上个方向向上，本方向向上，为↑
		x = lastx;
		y = lasty;
		dir = lastdir;
	}
	a_maze->m[a_maze->s.x][a_maze->s.y] = 'S';
	a_maze->m[a_maze->e.x][a_maze->e.y] = 'E';
	stack_destory(s);
	return;
} //求迷宫路径

void main() {
	struct maze* maze1 = (struct maze*)malloc(sizeof(struct maze)); //迷宫初始化
	if (inputmaze_data(maze1) == OK) {
		if (maze1 != NULL) printf("successfully input maze data from file\nmaze size:%d×%d\n", maze1->r, maze1->c);
		dispmaze(maze1);
		if (savemaze(maze1) == OK) printf("successfully save the original maze image\n");
		drawpath(maze1);
		dispmaze(maze1);
		if (savemaze(maze1) == OK) printf("successfully save the maze path image\n");
		if (outputmaze_data(maze1) == OK) printf("successfully output maze path data to file\n");
	}
	free(maze1);
	return;
}
