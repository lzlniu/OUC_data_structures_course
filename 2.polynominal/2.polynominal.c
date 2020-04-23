#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <malloc.h>
#include <math.h>
#include <string.h>
#define OK 1
#define ERROR 0
#define AUTOLENGTH 16

typedef struct poly_Node {
    float coef; //�����򣬶���ʽ��coef
    int expn; //�����򣬶���ʽ��expn
    struct poly_Node* next; //ָ����ָ����һ������*next
}poly_Node, *poly_LinkList; //��������ṹ�����������ָ��

typedef int status; //����statusΪint�ͣ��������÷���OK��ERROR

poly_LinkList poly_Init() {
    poly_LinkList L = (poly_LinkList)malloc(sizeof(poly_Node)); //��ͷ������ռ�
    if (!L) { printf("overflow!\n"); /*return ERROR;*/ exit(OVERFLOW); } //�ռ俪��ʧ��
    L->expn = 0; //ͷ����expnλ���ڼ�¼����ʽ��������ʼ��Ϊ0
    L->next = NULL; //ָ����ָ��NULL
    //printf("successfully initialize a polynominal LinkList!\n");
    return L;
} //�����ʼ�����Ӻ���

void poly_readfile(char file_name[], poly_LinkList L) {
    int i = 0, j = 0, line = 0;
    struct poly_Node* pre, * pn;
    FILE *input_file;
    input_file = fopen(file_name, "r"); //��ֻ���ļ�
    if (input_file == NULL) { printf("cann't open %s!\n", file_name); return; } //�ļ���ʧ��
    while (!feof(input_file)) {
        ++line; //�����ļ��м����������룩
        pre = L; //��ʼ��pre����ͷ���
        pn = (poly_LinkList)malloc(sizeof(poly_Node)); //�����µ��ڴ�ռ�pn
        if (!pn) { printf("overflow!\n"); return; /*exit(OVERFLOW);*/ } //�ռ俪��ʧ��
        fscanf_s(input_file, "%f,%d", &pn->coef, &pn->expn); //�洢�ļ���ÿ�и�ʽΪ��%f,%d���ֱ��ʾ����ʽ��ϵ����ָ����,��p��������������ʽϵ����ָ��
        if (pn->coef == 0) {
            printf("input at line %d:coef is 0\n", line);
            free(pn);
            ++j; //��Ч�������
            continue; //��ֹ�����������������һ��ѭ��
        } //���ϵ��Ϊ0���򲻲������ͷ�pn
        while (pre->next != NULL && pn->expn < pre->next->expn) pre = pre->next; //�ҵ���Ԫ�ذ���������Ӧ�����λ��
        if (pre->next != NULL && pn->expn == pre->next->expn) {
            printf("input at line %d:(%.1f,%d) is combined to a term with same expn (%.1f,%d) that forming (%.1f,%d)\n", line, pn->coef, pn->expn, pre->next->coef, pre->next->expn, (double)pn->coef + (double)pre->next->coef, pn->expn);
            pre->next->coef = pre->next->coef + pn->coef; //����ָͬ������ϲ�
            free(pn);
            ++i;  //���ϲ��Ķ���ʽ����
        } //����ȣ���ϵ����Ӻϲ���Ԫ�ص�pre->next->coef�ڣ��ͷ�pn
        else {
            pn->next = pre->next;
            pre->next = pn; //����Ԫ�ص��������е�λ��
        } //�ݲ���ȣ��������в�����Ԫ��
    } //���ΰ��ж�ȡ�ļ����ݣ������ļ���
    L->expn = line - i - j; //��ͷ����ָ���洢λ��¼����ʽ����������ֵȥ������Ч���루ϵ��Ϊ0����������ϵ����ͬ�ϲ�������
    printf("successfully struct the polynominal LinkList from %s\noriginal input:%d\ncombined valid input:%d\n", file_name, line, L->expn);
    fclose(input_file);
    return;
} //���ѳ�ʼ����������Ӷ���ʽԪ��

void poly_writefile(char file_name[], poly_LinkList L) {
    struct poly_Node *p;
    FILE *out;
    int i = 0; //���ڶ���ʽ�������ж��Ƿ�����λ
    out = fopen(file_name, "w"); //��ֻд�ļ�
    printf("there are %d poly in this polynominal.\noutput in the form of mathematics:\n", L->expn); //�鿴�ö���ʽ����
    p = L->next; //��ʼ��p��������λԪ��
    while (p) {
        if (p->coef > 0 && i >= 1) {
            printf("+");
            fprintf(out, "+");
        } //�ж��Ƿ����λ��ϵ��Ϊ��������λ����������ӼӺ�
        if (p->expn == 1) {
            printf("%.1fx", p->coef);
            fprintf(out, "%.1fx", p->coef);
        } //ָ��Ϊ1������ʾָ��������ϵ����x
        else if (p->expn > 1) {
            if (p->coef == 1) {
                printf("x^%d", p->expn);
                fprintf(out, "x^%d", p->expn);
            }
            else if (p->coef == -1) {
                printf("-x^%d", p->expn);
                fprintf(out, "-x^%d", p->expn);
            }
            else {
                printf("%.1fx^%d", p->coef, p->expn);
                fprintf(out, "%.1fx^%d", p->coef, p->expn);
            }
        } //ָ������1��������ʾָ����ϵ��������ϵ��Ϊ1��-1ʱʡ�����е�1
        else if (p->expn == 0) {
            printf("%.1f", p->coef);
            fprintf(out, "%.1f", p->coef);
        } //ָ��Ϊ0��������Ϊϵ����������x
        else {
            if (p->coef == 1) {
                printf("x^(%d)", p->expn);
                fprintf(out, "x^(%d)", p->expn);
            }
            else if (p->coef == -1) {
                printf("-x^(%d)", p->expn);
                fprintf(out, "-x^(%d)", p->expn);
            }
            else {
                printf("%.1fx^(%d)", p->coef, p->expn);
                fprintf(out, "%.1fx^(%d)", p->coef, p->expn);
            }
        } //ָ��С��0�������ָ�������ţ�����ϵ��Ϊ1��-1ʱʡ�����е�1
        ++i; //����ʽ����
        p = p->next; //����
    } //��һ�α���p������ѧ�ĸ�ʽ��ʾ����ʽ
    if (i == 0) {
        printf("0.0");
        fprintf(out, "0.0");
    } //���û��Ԫ�أ���i������ʽ������Ϊ0��������0.0
    printf("\noutput in the form of input dataset:\n");
    p = L->next; /*�ص�����ͷ*/
    while (p) {
        printf("%.1f,%d\n", p->coef, p->expn);
        fprintf(out, "\n%.1f,%d", p->coef, p->expn);
        p = p->next;
    } //�ڶ��α���p��������ĸ�ʽ��ʾ����ʽϵ����ָ��
    fclose(out);
    return;
} //�Ӷ���ʽ�������ж�ȡԪ�ز����ض��ļ������浽�ļ��У�������ϵ��Ϊ�������

void poly_destory(poly_LinkList L) {
    poly_Node *pre, *p;
    pre = L->next; //��ʼ��pre��������λԪ��
    L->next = NULL; //ԭͷ���ָ���ÿ�
    p = pre;
    while (pre) { pre = pre->next; free(p); p = pre; } //��������Ԫ�ز��ͷſռ�
    free(L); //�ͷ�ͷ���ռ�
    //printf("successfully destory a polynominal LinkList.\n");
    return;
} //ɾ�������ͷ��ڴ�

poly_LinkList poly_plus(poly_LinkList A, poly_LinkList B) {
    if (A == NULL) return B;
    if (B == NULL) return A;
    poly_LinkList C = poly_Init(); //��������ʼ������C
    struct poly_Node *pa, *pb, *pc, *prec;
    pa = A->next; pb = B->next;
    int i = 0, expn;
    float coef;
    /*
    //���ϲ�����������������
    float swap_coef; int swap_expn;
    pa = A->next; pb = B->next;
    while(pa) {
        pc1 = (poly_LinkList)malloc(sizeof(poly_Node));
        if (!pc1) {
            printf("overflow!\n");
            //return;
            exit(OVERFLOW);
        }
        pc1->coef = pa->coef;
        pc1->expn = pa->expn;
        pc1->next = C->next;
        C->next = pc1; //��������λ�����½ڵ�
        pa = pa->next;
    } //��C�в���A
    while (pb) {
        pc1 = (poly_LinkList)malloc(sizeof(poly_Node));
        if (!pc1) {
            printf("overflow!\n");
            //return;
            exit(OVERFLOW);
        }
        pc1->coef = pb->coef;
        pc1->expn = pb->expn;
        pc1->next = C->next;
        C->next = pc1;
        pb = pb->next;
    } //��C�в���B
    for (pre = C->next; pre != NULL; pre = pre->next) {
        for (pn = C->next; pn->next != NULL; pn = pn->next) {
            if (pn->expn < pn->next->expn) {
                swap_coef = pn->coef;
                swap_expn = pn->expn;
                pn->coef = pn->next->coef;
                pn->expn = pn->next->expn;
                pn->next->coef = swap_coef;
                pn->next->expn = swap_expn;
            }
        }
    } //���ϲ����C������
    */
    while (pa != NULL && pb != NULL) {
        if (pa->expn == pb->expn) {
            coef = pa->coef + pb->coef;
            expn = pa->expn;
            pa = pa->next;
            pb = pb->next;
            ++i;  if (coef == 0) ++i; //A��B������Ӷ��ϲ��ģ���1������Ӻ�����ģ���2�������
        } //A��Ԫ��ָ����B��ͬʱ����λ�Ӻͺ�ͬ����
        else if (pa->expn > pb->expn) {
            coef = pa->coef;
            expn = pa->expn;
            pa = pa->next;
        } //A��Ԫ��ָ����B��ʱ������A��Ԫ��
        else {
            coef = pb->coef;
            expn = pb->expn;
            pb = pb->next;
        } //B��Ԫ��ָ����A��ʱ������B��Ԫ��
        prec = C;
        if (coef != 0) {
            pc = (poly_LinkList)malloc(sizeof(poly_Node));
            if (!pc) {
                printf("overflow!\n");
                //return;
                exit(OVERFLOW);
            }
            pc->coef = coef;
            pc->expn = expn;
            /*
            pc->next = prec->next; //����C��ͷ��
            prec->next = pc;
            */
            pc->next = NULL;
            while (prec->next != NULL) prec = prec->next;
            prec->next = pc; //����C��β��
        } //����ͺ�����е�ϵ����0����Ԫ����������C
    } //A��B����Ԫ��ʱ��Ԫ�ذ��Ӵ�С�𽥲�������Cĩβ��
    while (pa != NULL) {
        coef = pa->coef;
        expn = pa->expn;
        pa = pa->next;
        prec = C;
        if (coef != 0) {
            pc = (poly_LinkList)malloc(sizeof(poly_Node));
            if (!pc) {
                printf("overflow!\n");
                //return;
                exit(OVERFLOW);
            }
            pc->coef = coef;
            pc->expn = expn;
            /*
            pc->next = prec->next;
            prec->next = pc; //����C��ͷ��
            */
            pc->next = NULL;
            while (prec->next != NULL) prec = prec->next;
            prec->next = pc; //����C��β��
        }
    } //������һ��ѭ������ʣ��A����������A��Ԫ��
    while (pb != NULL) {
        coef = pb->coef;
        expn = pb->expn;
        pb = pb->next;
        prec = C;
        if (coef != 0) {
            pc = (poly_LinkList)malloc(sizeof(poly_Node));
            if (!pc) {
                printf("overflow!\n");
                //return;
                exit(OVERFLOW);
            }
            pc->coef = coef;
            pc->expn = expn;
            /*
            pc->next = prec->next;
            prec->next = pc; //����C��ͷ��
            */
            pc->next = NULL;
            while (prec->next != NULL) prec = prec->next;
            prec->next = pc; //����C��β��
        }
    } //������һ��ѭ������ʣ��B����������B��Ԫ��
    C->expn = A->expn + B->expn - i;
    /*
    //�·������ã�������
    while (pa != NULL || pb != NULL) {
        prec = C;
        pc1 = (poly_LinkList)malloc(sizeof(poly_Node));
        if (!pc1) { 
            printf("overflow!\n");
            //return;
            exit(OVERFLOW);
        }
        if (pa == NULL && pb != NULL) {
            pc1 = pb;
            while (prec->next != NULL && pc1->expn < prec->next->expn) prec = prec->next;
            if (prec->next != NULL && pc1->expn == prec->next->expn) {
                prec->next->coef = prec->next->coef + pc1->coef;
                free(pc1);
                ++i;
            } //������Ԫ����Լ���Ԫ���ڽ����������Ƿ��ǰ����ͬ�ݣ���ͬ��ϲ�����ӣ�
            else {
                pc1->next = prec->next;
                prec->next = pc1;
            }
            pb = pb->next;
        } //A�����꣬����������B��Ԫ��
        else if (pb == NULL && pa != NULL) {
            pc1 = pa;
            while (prec->next != NULL && pc1->expn < prec->next->expn) prec = prec->next;
            if (prec->next != NULL && pc1->expn == prec->next->expn) {
                prec->next->coef = prec->next->coef + pc1->coef;
                free(pc1);
                ++i;
            }
            else {
                pc1->next = prec->next;
                prec->next = pc1;
            }
            pa = pa->next;
        } //B�����꣬������顢����A��Ԫ��
        else if (pa != NULL && pb != NULL) {
            pc2 = (poly_LinkList)malloc(sizeof(poly_Node));
            if (!pc2) {
                printf("overflow!\n");
                //return; 
                exit(OVERFLOW);
            }
            pc1 = pa; pc2 = pb; //һ�θ�ֵ����
            while (prec->next != NULL && (pc1->expn < prec->next->expn || pc2->expn < prec->next->expn)) {
                if (pc1->expn < prec->next->expn) {
                    if (pc1->expn == prec->next->expn) {
                        prec->next->coef = prec->next->coef + pc1->coef; //����ָͬ���������
                        free(pc1);
                        ++i;  //��ӵĶ���ʽ����
                    }
                    else {
                        pc1->next = prec->next;
                        prec->next = pc1;
                    }
                } //����pc1
                else if (pc2->expn < prec->next->expn) {
                    if (pc2->expn == prec->next->expn) {
                        prec->next->coef = prec->next->coef + pc2->coef;
                        free(pc2);
                        ++i;
                    }
                    else {
                        pc2->next = prec->next;
                        prec->next = pc2;
                    }
                } //����pc2
                prec = prec->next;
            } //�ҵ�������Ԫ�ذ���������Ӧ�����λ��
            pa = pa->next;
            pb = pb->next;
        } //A��B��δ�����꣬ͬʱ������顢����A��B���������Ԫ��
    }
    */
    return C; //����������õ�����C
}

poly_LinkList poly_minus(poly_LinkList A, poly_LinkList B) {
    if (A == NULL) return B;
    if (B == NULL) return A;
    poly_LinkList C = poly_Init(); //��������ʼ������C
    struct poly_Node *pa, *pb, *pc, *prec;
    pa = A->next; pb = B->next;
    int i = 0, expn;
    float coef;
    while (pa != NULL && pb != NULL) {
        if (pa->expn == pb->expn) {
            coef = pa->coef - pb->coef;
            expn = pa->expn;
            pa = pa->next;
            pb = pb->next;
            ++i;  if (coef == 0) ++i; //A��B������Ӷ��ϲ��ģ���1�������������ģ���2�������
        } //A��Ԫ��ָ����B��ͬʱ����λ�����ͬ����
        else if (pa->expn > pb->expn) {
            coef = pa->coef;
            expn = pa->expn;
            pa = pa->next;
        } //A��Ԫ��ָ����B��ʱ������A��Ԫ��
        else {
            coef = - pb->coef;
            expn = pb->expn;
            pb = pb->next;
        } //B��Ԫ��ָ����A��ʱ������B��Ԫ��
        prec = C;
        if (coef != 0) {
            pc = (poly_LinkList)malloc(sizeof(poly_Node));
            if (!pc) { printf("overflow!\n"); /*return;*/ exit(OVERFLOW); }
            pc->coef = coef;
            pc->expn = expn;
            pc->next = NULL;
            while (prec->next != NULL) prec = prec->next;
            prec->next = pc;
        } //�����������е�ϵ����0����Ԫ����������C
    } //A��B����Ԫ��ʱ
    while (pa != NULL) {
        coef = pa->coef;
        expn = pa->expn;
        pa = pa->next;
        prec = C;
        if (coef != 0) {
            pc = (poly_LinkList)malloc(sizeof(poly_Node));
            if (!pc) { printf("overflow!\n"); /*return;*/ exit(OVERFLOW); }
            pc->coef = coef;
            pc->expn = expn;
            pc->next = NULL;
            while (prec->next != NULL) prec = prec->next;
            prec->next = pc;
        }
    } //������һ��ѭ������ʣ��A����������A��Ԫ��
    while (pb != NULL) {
        coef = - pb->coef;
        expn = pb->expn;
        pb = pb->next;
        prec = C;
        if (coef != 0) {
            pc = (poly_LinkList)malloc(sizeof(poly_Node));
            if (!pc) { printf("overflow!\n"); /*return;*/ exit(OVERFLOW); }
            pc->coef = coef;
            pc->expn = expn;
            pc->next = NULL;
            while (prec->next != NULL) prec = prec->next;
            prec->next = pc;
        }
    } //������һ��ѭ������ʣ��B����������B��Ԫ��
    C->expn = A->expn + B->expn - i;
    return C;
} //�ͼӷ��ӳ���Ψһ������������B�����ϵ�����Ӹ���

void poly_select(poly_LinkList L, int i) {
    struct poly_Node *p;
    p = L->next;
    if (i <= L->expn && i > 0) {
        for (int j = 1; j < i; j++) p = p->next;
        printf("the %d of the polynominal is (%.1f,%d),its mathematical form is:", i, p->coef, p->expn);
        if (p->expn == 1) printf("%.1fx", p->coef); //ָ��Ϊ1������ʾָ��������ϵ����x
        else if (p->expn > 1) {
            if (p->coef == 1) printf("x^%d", p->expn);
            else if (p->coef == -1) printf("-x^%d", p->expn);
            else printf("%.1fx^%d", p->coef, p->expn);
        } //ָ������1��������ʾָ����ϵ��������ϵ��Ϊ1��-1ʱʡ�����е�1
        else if (p->expn == 0) printf("%.1f", p->coef); //ָ��Ϊ0��������Ϊϵ����������x
        else {
            if (p->coef == 1) printf("x^(%d)", p->expn);
            else if (p->coef == -1) printf("-x^(%d)", p->expn);
            else printf("%.1fx^(%d)", p->coef, p->expn);
        } //ָ��С��0��ϵ��ͬ����1ʱһ��Ҳ�����������-1,1����0�����������
        printf("\n");
    }
    else printf("input number %d is out of polynominal range.\n", i);
    return;
} //��������ȡֵ��չʾ�ӳ���

void poly_diff(poly_LinkList L) {

}

void main() {
    poly_LinkList ListA, ListB, AplusB, AminusB/*, BminusA*/;
    ListA = poly_Init(); ListB = poly_Init(); //��ʼ������
    int i=1;
    char in_name[AUTOLENGTH], out_name[AUTOLENGTH];
    printf("input 1st file:");
    scanf_s("%s", &in_name, AUTOLENGTH);
    poly_readfile(in_name, ListA); //ָ���ļ���ȡ�����ݲ���������A

    printf("\ninput 2nd file:");
    scanf_s("%s", &in_name, AUTOLENGTH);
    poly_readfile(in_name, ListB);

    printf("\noutput 1st file:");
    scanf_s("%s", &out_name, AUTOLENGTH);
    poly_writefile(out_name, ListA); //�������A���ݵ�ָ���ļ�

    printf("\noutput 2nd file:");
    scanf_s("%s", &out_name, AUTOLENGTH);
    poly_writefile(out_name, ListB);

    printf("\noutput 1st+2nd file:");
    scanf_s("%s", &out_name, AUTOLENGTH);
    AplusB = poly_plus(ListA, ListB); //������A��B����ʽ��ӵõ�����AplusB
    poly_writefile(out_name, AplusB);

    printf("\noutput 1st-2nd file:");
    scanf_s("%s", &out_name, AUTOLENGTH);
    AminusB = poly_minus(ListA, ListB); //������A����ʽ��ȥ����B����ʽ�õ�����AminusB
    poly_writefile(out_name, AminusB);
    
    /*
    printf("\noutput 2nd-1st file:");
    scanf_s("%s", &out_name, AUTOLENGTH);
    BminusA = poly_minus(ListB, ListA); //������B����ʽ��ȥ����A����ʽ�õ�����BminusA
    poly_writefile(out_name, BminusA);
    */

    printf("\nat 1st poly LinkList:\n");
    for (int i = 1; i <= ListA->expn; i++) poly_select(ListA, i); //չʾ����A����Ԫ��
    poly_select(ListA, 1);
    poly_select(ListA, 0);
    poly_select(ListA, 2);
    poly_select(ListA, -2);
    poly_select(ListA, 8);
    poly_select(ListA, 7); //չʾ����A�ض�λ��Ԫ��

    printf("\nat 1st+2nd poly LinkList:\n");
    for (int i = 1; i <= AplusB->expn; i++) poly_select(AplusB, i); //չʾ����A+B�������Ԫ��
    poly_select(AplusB, -5);
    poly_select(AplusB, 7);
    poly_select(AplusB, 3); //չʾ����A+B���ض�λ��Ԫ��

    poly_destory(ListA); poly_destory(ListB);
    poly_destory(AplusB);
    poly_destory(AminusB);
    //poly_destory(BminusA); //ɾ������
    return;
} //������
