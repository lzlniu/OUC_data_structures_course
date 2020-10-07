#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define SQUARE 100 //�Թ����������
#define FILENAME_MAX_LENGTH 64
#define ERROR 0
#define OK 1

struct pos { 
	int x, y;
}; //����

struct node {
	struct node* pre; //��һ��Ԫ��
	struct pos p; //������Ϣ
	int dir;  //����1��,2��,3��,4��
}; //�Թ�·����¼���

struct maze {
	int r, c;
	char m[SQUARE + 2][SQUARE + 2]; //0��ʾͨ·,1��ʾǽ��,2��ʾ���߹���·��
	struct pos s; //���λ����Ϣ
	struct pos e; //�յ�λ����Ϣ
}; //�Թ��ṹ

struct stack {
	struct node* top; //ջ��ָ��
	int size; //ջ��Ԫ����
}; //ջ�ṹ

int stack_init(struct stack* S) {
	S->top = NULL;
	S->size = 0;
	return OK;
} //ջ��ʼ��

int stack_push(struct stack* S, struct node e) {
	struct node* p = (struct node*)malloc(sizeof(struct node));
	if (!p) { printf("overflow during stack_push!\n"); return ERROR; /*exit(0);*/ }
	p->dir = e.dir;
	p->p = e.p;
	p->pre = S->top; //����Ԫ�ص���һλ��ԭ�ȵ�top
	S->top = p; //����Ԫ�ص�ջ��
	++S->size;
	return OK;
} //ָ��Ԫ��e��element����ջS

struct node stack_pop(struct stack* S) {
	struct node popnode; //�˷�ָ�룬����node�ṹ����¼��ջԪ����Ϣ
	struct node* p; //��ʱָ�룬ָ���ջǰ��top
	if (S->size == 0) {
		printf("this maze have no path��\n");
		popnode.dir = 5; //�跽��Ϊ5���������ܷ��򡱣�
		return popnode;
	} //ջ��Ԫ�أ��Թ��޷���ͨ
	popnode.dir = S->top->dir;
	popnode.p = S->top->p;
	popnode.pre = NULL;
	p = S->top;
	S->top = S->top->pre; //ջ����
	free(p); //�ͷ�ԭtopλ��
	S->size--; //ջԪ�ؼ�һ
	return popnode;
} //ָ��ջ��Ԫ��e��ջS

void stack_destory(struct stack* S) {
	while (S->size) stack_pop(S); //����stack_pop�ķ���ֵ���൱��ֱ�����
	return;
} //ջɾ��

void stack_top(struct stack* S) {
	char c[3] = { 0 };
	struct node* p = S->top;
	if (p->dir == 1) strcpy(c, "��");
	else if (p->dir == 2) strcpy(c, "��");
	else if (p->dir == 3) strcpy(c, "��");
	else if (p->dir == 4) strcpy(c, "��");
	printf("%d\t%d\t%d\t%s\n", S->size, p->p.x, p->p.y, c);
	return;
} //ȡջ��Ԫ�أ�ͬʱ�����ǰջ��С��������·������

void stack_show(struct stack* S) {
	int i = S->size, endx = 0, endy = 0;
	char c[3];
	struct node* p = S->top;
	if (p->dir == 1) { endx = p->p.x; endy = p->p.y + 1; } //1��,2��,3��,4�ϣ����Թ��������꣨��ǰ�ִ�λ�ã�
	else if (p->dir == 2) { endx = p->p.x + 1; endy = p->p.y; }
	else if (p->dir == 3) { endx = p->p.x; endy = p->p.y - 1; }
	else if (p->dir == 4) { endx = p->p.x - 1; endy = p->p.y; }
	printf("step\tline\trow\tdirection\nend\t%d\t%d\t��\n", endx, endy);
	while (p) {
		memset(c, 0, 3);
		if (p->dir == 1) strcpy(c, "��");
		else if (p->dir == 2) strcpy(c, "��");
		else if (p->dir == 3) strcpy(c, "��");
		else if (p->dir == 4) strcpy(c, "��");
		i--;
		if (i > 0) printf("%d\t%d\t%d\t%s\n", i, p->p.x, p->p.y, c);
		else printf("start\t%d\t%d\t%s\n", p->p.x, p->p.y, c);
		p = p->pre;
	} //����ջԪ�أ���ջ����ǰ��
} //ջԪ����ʾ

void dispmaze(struct maze* a_maze) {
	int i, j;
	printf("��route  ��wall  �� start  ��end  �w can't through\n");
	for (i = 0; i <= a_maze->r + 1; i++) {
		for (j = 0; j <= a_maze->c + 1; j++) {
			if (i == a_maze->s.x && j == a_maze->s.y) printf("�� "); //���
			else if (i == a_maze->e.x && j == a_maze->e.y) printf("��"); //�յ�
			else {
				if (a_maze->m[i][j] == '0') printf("��");
				if (a_maze->m[i][j] == '1') printf("��");
				if (a_maze->m[i][j] == '2') printf("�w ");
				if (a_maze->m[i][j] == 'A') printf("��");
				if (a_maze->m[i][j] == 'B') printf("��");
				if (a_maze->m[i][j] == 'C') printf("��");
				if (a_maze->m[i][j] == 'D') printf("��");
				if (a_maze->m[i][j] == 'I') printf("�� "); //������Ϊ������ȫ�ǣ���ӿո�
				if (a_maze->m[i][j] == 'F') printf("�� "); //������Ϊ������ȫ�ǣ���ӿո�
				if (a_maze->m[i][j] == 'G') printf("�� "); //������Ϊ������ȫ�ǣ���ӿո�
				if (a_maze->m[i][j] == 'H') printf("�� "); //������Ϊ������ȫ�ǣ���ӿո�
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
	fprintf(fp, "��route  ��wall  �� start  ��end  �w can't through\n");
	for (i = 0; i <= a_maze->r + 1; i++) {
		for (j = 0; j <= a_maze->c + 1; j++) {
			if (i == a_maze->s.x && j == a_maze->s.y) fprintf(fp, "��"); //���
			else if (i == a_maze->e.x && j == a_maze->e.y) fprintf(fp, "��"); //�յ�
			else {
				if (a_maze->m[i][j] == '0') fprintf(fp, "��");
				if (a_maze->m[i][j] == '1') fprintf(fp, "��");
				if (a_maze->m[i][j] == '2') fprintf(fp, "�w");
				if (a_maze->m[i][j] == 'A') fprintf(fp, "��");
				if (a_maze->m[i][j] == 'B') fprintf(fp, "��");
				if (a_maze->m[i][j] == 'C') fprintf(fp, "��");
				if (a_maze->m[i][j] == 'D') fprintf(fp, "��");
				if (a_maze->m[i][j] == 'I') fprintf(fp, "��"); //������Ϊ������ȫ�ǣ���ӿո�
				if (a_maze->m[i][j] == 'F') fprintf(fp, "��"); //������Ϊ������ȫ�ǣ���ӿո�
				if (a_maze->m[i][j] == 'G') fprintf(fp, "��"); //������Ϊ������ȫ�ǣ���ӿո�
				if (a_maze->m[i][j] == 'H') fprintf(fp, "��"); //������Ϊ������ȫ�ǣ���ӿո�
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
	int s, e;//���������յ�
	s = 0, e = 0;
	printf("input maze data from (file name):");
	scanf_s("%s", &inname, FILENAME_MAX_LENGTH);
	fopen_s(&fp, inname, "r");
	if (!fp) { printf("can't open %s\n", inname); return ERROR; }
	fscanf_s(fp, "%d,%d", &(a_maze->r), &(a_maze->c)); //�Թ��޶���������r��c������Ϣ���Թ�������λ�ڵ�һ����%d,%d��ʽд
	fgetc(fp); //��ȡ���з�
	for (i = 0; i < a_maze->r + 2; i++)
		for (j = 0; j < a_maze->c + 2; j++)
			a_maze->m[i][j] = '1';//��ȫ�����Ϊ1��1����ǽ�ڣ�
	for (i = 1; i <= a_maze->r; i++) { //�����Թ������һ�е��Թ��߽�
		fgets(row, SQUARE + 10, fp); //��fp�����ж�ȡ
		for (j = 1; j <= a_maze->c; j++) { //������һ�е��Թ��߽�
			a_maze->m[i][j] = row[j - 1]; //����¼������Թ�λ�������ǽ��·�������յ㣩
			if (a_maze->m[i][j] == 'S') {
				a_maze->s.x = i;
				a_maze->s.y = j;
				s = 1;
			} //��㱻���ΪS��¼����λ�õ�s
			else if (a_maze->m[i][j] == 'E') {
				a_maze->e.x = i;
				a_maze->e.y = j;
				e = 1;
			} //�յ㱻���ΪE��¼����λ�õ�e
		}
	}
	fclose(fp);
	if (s + e < 2) {
		printf("can't find start or end position of this maze %s\n", inname);
		return ERROR;
	}
	return OK;
} //�����Թ�

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
	} //������߽�
	fprintf(fp, "\n\n\n\n0·  1ǽ  S���  E�յ�  2��ͨ·  A��   B��  C��  D��  F����/����  G����/����  H����/����  I����/����\n");
	fclose(fp);
	return OK;
} //����Թ�

void drawpath(struct maze* a_maze) {
	struct stack stackstruct, * s = &stackstruct; //sΪָ��stackstruct��stack���ͣ���ָ��
	int x, lastx, y, lasty, dir, lastdir; //��¼ָ��ǰ�С��С������ֵ��������������һλ��ֵ
	struct node n;
	x = a_maze->s.x;
	y = a_maze->s.y;
	dir = 1; //��ʼ����Ϊ1
	stack_init(s);
	printf("\nstack status:\n");
	while (a_maze->m[x][y] != 'E') { //xΪ�У�yΪ�У�1��,2��,3��,4��
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
				if (n.dir == 5) return; //�������ܷ���5��ʾ�Թ��޽⣬�˳�
				x = n.p.x; y = n.p.y;
			}
		} //�յ������·�������˳����������1243
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
				if (n.dir == 5) return; //�������ܷ���5��ʾ�Թ��޽⣬�˳�
				x = n.p.x; y = n.p.y;
			}
		} //�յ������ҷ�������˳����������1423
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
				if (n.dir == 5) return; //�������ܷ���5��ʾ�Թ��޽⣬�˳�
				x = n.p.x; y = n.p.y;
			}
		} //�յ������Ϸ�������˳����������4123
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
				if (n.dir == 5) return; //�������ܷ���5��ʾ�Թ��޽⣬�˳�
				x = n.p.x; y = n.p.y;
			}
		} //�յ������·�������˳����������2341
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
				if (n.dir == 5) return; //�������ܷ���5��ʾ�Թ��޽⣬�˳�
				x = n.p.x; y = n.p.y;
			}
		} //�յ������󷽣�����˳����������3241
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
				if (n.dir == 5) return; //�������ܷ���5��ʾ�Թ��޽⣬�˳�
				x = n.p.x; y = n.p.y;
			}
		} //�յ������Ϸ�������˳����������3421
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
				if (n.dir == 5) return; //�������ܷ���5��ʾ�Թ��޽⣬�˳�
				x = n.p.x; y = n.p.y;
			}
		} //�յ������·�������˳����������2314
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
				if (n.dir == 5) return; //�������ܷ���5��ʾ�Թ��޽⣬�˳�
				x = n.p.x; y = n.p.y;
			}
		} //�յ������Ϸ�������˳����������4132
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
			if (n.dir == 5) return; //�������ܷ���5��ʾ�Թ��޽⣬�˳�
			x = n.p.x; y = n.p.y;
		}
		*/
	}
	printf("\nmaze path:\n");
	stack_show(s);
	printf("\n");
	n = stack_pop(s); //��ʱջ��Ϊ�յ㣬�յ��ջ
	x = n.p.x;
	y = n.p.y;
	dir = n.dir;
	while (s->size != 0) { //����dir����1��,2��,3��,4��
		n = stack_pop(s); //·���������ջ��������ջ��·�����£�ȡ��ԭ�е�2�����߹�·������
		lastx = n.p.x;
		lasty = n.p.y;
		lastdir = n.dir;
		if (lastdir == 1 && dir == 1) a_maze->m[x][y] = 'A';      //�ϸ��������ң����������ң�Ϊ��
		else if (lastdir == 1 && dir == 2) a_maze->m[x][y] = 'F'; //�ϸ��������ң����������£�Ϊ��
		else if (lastdir == 1 && dir == 4) a_maze->m[x][y] = 'G'; //�ϸ��������ң����������ϣ�Ϊ��
		else if (lastdir == 2 && dir == 1) a_maze->m[x][y] = 'H'; //�ϸ��������£����������ң�Ϊ��
		else if (lastdir == 2 && dir == 2) a_maze->m[x][y] = 'B'; //�ϸ��������£����������£�Ϊ��
		else if (lastdir == 2 && dir == 3) a_maze->m[x][y] = 'G'; //�ϸ��������£�����������Ϊ��
		else if (lastdir == 3 && dir == 2) a_maze->m[x][y] = 'I'; //�ϸ��������󣬱��������£�Ϊ��
		else if (lastdir == 3 && dir == 3) a_maze->m[x][y] = 'C'; //�ϸ��������󣬱���������Ϊ��
		else if (lastdir == 3 && dir == 4) a_maze->m[x][y] = 'H'; //�ϸ��������󣬱��������ϣ�Ϊ��
		else if (lastdir == 4 && dir == 1) a_maze->m[x][y] = 'I'; //�ϸ��������ϣ����������ң�Ϊ��
		else if (lastdir == 4 && dir == 3) a_maze->m[x][y] = 'F'; //�ϸ��������ϣ�����������Ϊ��
		else if (lastdir == 4 && dir == 4) a_maze->m[x][y] = 'D'; //�ϸ��������ϣ����������ϣ�Ϊ��
		x = lastx;
		y = lasty;
		dir = lastdir;
	}
	a_maze->m[a_maze->s.x][a_maze->s.y] = 'S';
	a_maze->m[a_maze->e.x][a_maze->e.y] = 'E';
	stack_destory(s);
	return;
} //���Թ�·��

void main() {
	struct maze* maze1 = (struct maze*)malloc(sizeof(struct maze)); //�Թ���ʼ��
	if (inputmaze_data(maze1) == OK) {
		if (maze1 != NULL) printf("successfully input maze data from file\nmaze size:%d��%d\n", maze1->r, maze1->c);
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
