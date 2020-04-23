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
    float coef; //数据域，多项式的coef
    int expn; //数据域，多项式的expn
    struct poly_Node* next; //指针域，指向下一个结点的*next
}poly_Node, *poly_LinkList; //定义链表结构，并设置其的指针

typedef int status; //定义status为int型，用于设置返回OK或ERROR

poly_LinkList poly_Init() {
    poly_LinkList L = (poly_LinkList)malloc(sizeof(poly_Node)); //给头结点分配空间
    if (!L) { printf("overflow!\n"); /*return ERROR;*/ exit(OVERFLOW); } //空间开辟失败
    L->expn = 0; //头结点的expn位用于记录多项式项数，初始化为0
    L->next = NULL; //指针域指向NULL
    //printf("successfully initialize a polynominal LinkList!\n");
    return L;
} //链表初始化的子函数

void poly_readfile(char file_name[], poly_LinkList L) {
    int i = 0, j = 0, line = 0;
    struct poly_Node* pre, * pn;
    FILE *input_file;
    input_file = fopen(file_name, "r"); //打开只读文件
    if (input_file == NULL) { printf("cann't open %s!\n", file_name); return; } //文件打开失败
    while (!feof(input_file)) {
        ++line; //输入文件行计数（总输入）
        pre = L; //初始化pre等于头结点
        pn = (poly_LinkList)malloc(sizeof(poly_Node)); //开辟新的内存空间pn
        if (!pn) { printf("overflow!\n"); return; /*exit(OVERFLOW);*/ } //空间开辟失败
        fscanf_s(input_file, "%f,%d", &pn->coef, &pn->expn); //存储文件内每行格式为：%f,%d（分别表示多项式的系数和指数）,给p的数据域插入多项式系数和指数
        if (pn->coef == 0) {
            printf("input at line %d:coef is 0\n", line);
            free(pn);
            ++j; //无效输入计数
            continue; //终止此项插入链表，进入下一个循环
        } //如果系数为0，则不插入此项，释放pn
        while (pre->next != NULL && pn->expn < pre->next->expn) pre = pre->next; //找到本元素按降幂排列应插入的位置
        if (pre->next != NULL && pn->expn == pre->next->expn) {
            printf("input at line %d:(%.1f,%d) is combined to a term with same expn (%.1f,%d) that forming (%.1f,%d)\n", line, pn->coef, pn->expn, pre->next->coef, pre->next->expn, (double)pn->coef + (double)pre->next->coef, pn->expn);
            pre->next->coef = pre->next->coef + pn->coef; //将相同指数的项合并
            free(pn);
            ++i;  //被合并的多项式计数
        } //幂相等，则系数相加合并两元素到pre->next->coef内，释放pn
        else {
            pn->next = pre->next;
            pre->next = pn; //插入元素到降幂排列的位置
        } //幂不相等，则降幂排列插入新元素
    } //依次按行读取文件内容（遍历文件）
    L->expn = line - i - j; //在头结点的指数存储位记录多项式项数，该数值去除了无效输入（系数为0）的项数和系数相同合并的项数
    printf("successfully struct the polynominal LinkList from %s\noriginal input:%d\ncombined valid input:%d\n", file_name, line, L->expn);
    fclose(input_file);
    return;
} //给已初始化的链表添加多项式元素

void poly_writefile(char file_name[], poly_LinkList L) {
    struct poly_Node *p;
    FILE *out;
    int i = 0; //用于多项式计数并判断是否是首位
    out = fopen(file_name, "w"); //打开只写文件
    printf("there are %d poly in this polynominal.\noutput in the form of mathematics:\n", L->expn); //查看该多项式项数
    p = L->next; //初始化p到链表首位元素
    while (p) {
        if (p->coef > 0 && i >= 1) {
            printf("+");
            fprintf(out, "+");
        } //判断是否非首位且系数为正，非首位且是正则添加加号
        if (p->expn == 1) {
            printf("%.1fx", p->coef);
            fprintf(out, "%.1fx", p->coef);
        } //指数为1，不显示指数，仅有系数和x
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
        } //指数大于1，正常显示指数和系数，其中系数为1或-1时省略其中的1
        else if (p->expn == 0) {
            printf("%.1f", p->coef);
            fprintf(out, "%.1f", p->coef);
        } //指数为0，输出结果为系数本身，不带x
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
        } //指数小于0，输出的指数带括号，其中系数为1或-1时省略其中的1
        ++i; //多项式计数
        p = p->next; //下移
    } //第一次遍历p，以数学的格式显示多项式
    if (i == 0) {
        printf("0.0");
        fprintf(out, "0.0");
    } //如果没有元素，则i（多项式数量）为0，输出结果0.0
    printf("\noutput in the form of input dataset:\n");
    p = L->next; /*回到链表开头*/
    while (p) {
        printf("%.1f,%d\n", p->coef, p->expn);
        fprintf(out, "\n%.1f,%d", p->coef, p->expn);
        p = p->next;
    } //第二次遍历p，以输入的格式显示多项式系数和指数
    fclose(out);
    return;
} //从多项式单链表中读取元素并以特定文件名保存到文件中，不考虑系数为负的情况

void poly_destory(poly_LinkList L) {
    poly_Node *pre, *p;
    pre = L->next; //初始化pre到链表首位元素
    L->next = NULL; //原头结点指向置空
    p = pre;
    while (pre) { pre = pre->next; free(p); p = pre; } //遍历链表元素并释放空间
    free(L); //释放头结点空间
    //printf("successfully destory a polynominal LinkList.\n");
    return;
} //删除链表，释放内存

poly_LinkList poly_plus(poly_LinkList A, poly_LinkList B) {
    if (A == NULL) return B;
    if (B == NULL) return A;
    poly_LinkList C = poly_Init(); //创建并初始化链表C
    struct poly_Node *pa, *pb, *pc, *prec;
    pa = A->next; pb = B->next;
    int i = 0, expn;
    float coef;
    /*
    //仅合并两个链表并排序，如下
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
        C->next = pc1; //在链表首位插入新节点
        pa = pa->next;
    } //在C中插入A
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
    } //在C中插入B
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
    } //给合并后的C表排序
    */
    while (pa != NULL && pb != NULL) {
        if (pa->expn == pb->expn) {
            coef = pa->coef + pb->coef;
            expn = pa->expn;
            pa = pa->next;
            pb = pb->next;
            ++i;  if (coef == 0) ++i; //A、B中因相加而合并的（计1）和相加后被消项的（计2）项计数
        } //A中元素指数和B相同时，此位加和后共同跳过
        else if (pa->expn > pb->expn) {
            coef = pa->coef;
            expn = pa->expn;
            pa = pa->next;
        } //A中元素指数比B大时，放入A中元素
        else {
            coef = pb->coef;
            expn = pb->expn;
            pb = pb->next;
        } //B中元素指数比A大时，放入B中元素
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
            pc->next = prec->next; //插入C的头部
            prec->next = pc;
            */
            pc->next = NULL;
            while (prec->next != NULL) prec = prec->next;
            prec->next = pc; //插入C的尾部
        } //若求和后或现有的系数非0，则将元素纳入链表C
    } //A、B均有元素时，元素按从大到小逐渐插入链表C末尾处
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
            prec->next = pc; //插入C的头部
            */
            pc->next = NULL;
            while (prec->next != NULL) prec = prec->next;
            prec->next = pc; //插入C的尾部
        }
    } //经过第一个循环后若剩下A，则继续添加A的元素
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
            prec->next = pc; //插入C的头部
            */
            pc->next = NULL;
            while (prec->next != NULL) prec = prec->next;
            prec->next = pc; //插入C的尾部
        }
    } //经过第一个循环后若剩下B，则继续添加B的元素
    C->expn = A->expn + B->expn - i;
    /*
    //下方不可用，但暂留
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
            } //检查插入元素相对既有元素在降幂排列上是否和前项相同幂，相同则合并（相加）
            else {
                pc1->next = prec->next;
                prec->next = pc1;
            }
            pb = pb->next;
        } //A遍历完，遍历检查插入B的元素
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
        } //B遍历完，遍历检查、插入A的元素
        else if (pa != NULL && pb != NULL) {
            pc2 = (poly_LinkList)malloc(sizeof(poly_Node));
            if (!pc2) {
                printf("overflow!\n");
                //return; 
                exit(OVERFLOW);
            }
            pc1 = pa; pc2 = pb; //一次赋值两个
            while (prec->next != NULL && (pc1->expn < prec->next->expn || pc2->expn < prec->next->expn)) {
                if (pc1->expn < prec->next->expn) {
                    if (pc1->expn == prec->next->expn) {
                        prec->next->coef = prec->next->coef + pc1->coef; //将相同指数的项相加
                        free(pc1);
                        ++i;  //相加的多项式计数
                    }
                    else {
                        pc1->next = prec->next;
                        prec->next = pc1;
                    }
                } //插入pc1
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
                } //插入pc2
                prec = prec->next;
            } //找到这两个元素按降幂排列应插入的位置
            pa = pa->next;
            pb = pb->next;
        } //A和B均未遍历完，同时遍历检查、插入A和B两个链表的元素
    }
    */
    return C; //返回相加所得的链表C
}

poly_LinkList poly_minus(poly_LinkList A, poly_LinkList B) {
    if (A == NULL) return B;
    if (B == NULL) return A;
    poly_LinkList C = poly_Init(); //创建并初始化链表C
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
            ++i;  if (coef == 0) ++i; //A、B中因相加而合并的（计1）和相减后被消项的（计2）项计数
        } //A中元素指数和B相同时，此位相减后共同跳过
        else if (pa->expn > pb->expn) {
            coef = pa->coef;
            expn = pa->expn;
            pa = pa->next;
        } //A中元素指数比B大时，放入A中元素
        else {
            coef = - pb->coef;
            expn = pb->expn;
            pb = pb->next;
        } //B中元素指数比A大时，放入B中元素
        prec = C;
        if (coef != 0) {
            pc = (poly_LinkList)malloc(sizeof(poly_Node));
            if (!pc) { printf("overflow!\n"); /*return;*/ exit(OVERFLOW); }
            pc->coef = coef;
            pc->expn = expn;
            pc->next = NULL;
            while (prec->next != NULL) prec = prec->next;
            prec->next = pc;
        } //若相减后或现有的系数非0，则将元素纳入链表C
    } //A、B均有元素时
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
    } //经过第一个循环后若剩下A，则继续添加A的元素
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
    } //经过第一个循环后若剩下B，则继续添加B的元素
    C->expn = A->expn + B->expn - i;
    return C;
} //和加法子程序唯一区别在于所有B链表的系数增加负号

void poly_select(poly_LinkList L, int i) {
    struct poly_Node *p;
    p = L->next;
    if (i <= L->expn && i > 0) {
        for (int j = 1; j < i; j++) p = p->next;
        printf("the %d of the polynominal is (%.1f,%d),its mathematical form is:", i, p->coef, p->expn);
        if (p->expn == 1) printf("%.1fx", p->coef); //指数为1，不显示指数，仅有系数和x
        else if (p->expn > 1) {
            if (p->coef == 1) printf("x^%d", p->expn);
            else if (p->coef == -1) printf("-x^%d", p->expn);
            else printf("%.1fx^%d", p->coef, p->expn);
        } //指数大于1，正常显示指数和系数，其中系数为1或-1时省略其中的1
        else if (p->expn == 0) printf("%.1f", p->coef); //指数为0，输出结果为系数本身，不带x
        else {
            if (p->coef == 1) printf("x^(%d)", p->expn);
            else if (p->coef == -1) printf("-x^(%d)", p->expn);
            else printf("%.1fx^(%d)", p->coef, p->expn);
        } //指数小于0，系数同大于1时一样也分三种情况（-1,1，及0以外的其他）
        printf("\n");
    }
    else printf("input number %d is out of polynominal range.\n", i);
    return;
} //从链表中取值并展示子程序

void poly_diff(poly_LinkList L) {

}

void main() {
    poly_LinkList ListA, ListB, AplusB, AminusB/*, BminusA*/;
    ListA = poly_Init(); ListB = poly_Init(); //初始化链表
    int i=1;
    char in_name[AUTOLENGTH], out_name[AUTOLENGTH];
    printf("input 1st file:");
    scanf_s("%s", &in_name, AUTOLENGTH);
    poly_readfile(in_name, ListA); //指定文件读取其内容并产生链表A

    printf("\ninput 2nd file:");
    scanf_s("%s", &in_name, AUTOLENGTH);
    poly_readfile(in_name, ListB);

    printf("\noutput 1st file:");
    scanf_s("%s", &out_name, AUTOLENGTH);
    poly_writefile(out_name, ListA); //输出链表A内容到指定文件

    printf("\noutput 2nd file:");
    scanf_s("%s", &out_name, AUTOLENGTH);
    poly_writefile(out_name, ListB);

    printf("\noutput 1st+2nd file:");
    scanf_s("%s", &out_name, AUTOLENGTH);
    AplusB = poly_plus(ListA, ListB); //将链表A、B多项式相加得到链表AplusB
    poly_writefile(out_name, AplusB);

    printf("\noutput 1st-2nd file:");
    scanf_s("%s", &out_name, AUTOLENGTH);
    AminusB = poly_minus(ListA, ListB); //将链表A多项式减去链表B多项式得到链表AminusB
    poly_writefile(out_name, AminusB);
    
    /*
    printf("\noutput 2nd-1st file:");
    scanf_s("%s", &out_name, AUTOLENGTH);
    BminusA = poly_minus(ListB, ListA); //将链表B多项式减去链表A多项式得到链表BminusA
    poly_writefile(out_name, BminusA);
    */

    printf("\nat 1st poly LinkList:\n");
    for (int i = 1; i <= ListA->expn; i++) poly_select(ListA, i); //展示链表A所有元素
    poly_select(ListA, 1);
    poly_select(ListA, 0);
    poly_select(ListA, 2);
    poly_select(ListA, -2);
    poly_select(ListA, 8);
    poly_select(ListA, 7); //展示链表A特定位置元素

    printf("\nat 1st+2nd poly LinkList:\n");
    for (int i = 1; i <= AplusB->expn; i++) poly_select(AplusB, i); //展示链表A+B后的所有元素
    poly_select(AplusB, -5);
    poly_select(AplusB, 7);
    poly_select(AplusB, 3); //展示链表A+B后特定位置元素

    poly_destory(ListA); poly_destory(ListB);
    poly_destory(AplusB);
    poly_destory(AminusB);
    //poly_destory(BminusA); //删除链表
    return;
} //主函数
