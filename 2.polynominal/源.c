#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <malloc.h>
#include <math.h>
#include <string.h>
#define OK 1
#define ERROR 0
#define AUTOLENGTH 64 //用于文件名字符串长度限定

typedef struct poly_Node {
    float coef; //数据域，多项式的coef
    int expn; //数据域，多项式的expn
    struct poly_Node* next; //指针域，指向下一个结点的*next
}poly_Node, * poly_LinkList; //定义链表结构，并设置其的指针

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
    FILE* input_file;
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
    struct poly_Node* p;
    FILE* out;
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
            if (p->coef == 1) {
                printf("x");
                fprintf(out, "x");
            }
            else if (p->coef == -1) {
                printf("-x");
                fprintf(out, "-x");
            }
            else {
                printf("%.1fx", p->coef);
                fprintf(out, "%.1fx", p->coef);
            }
        } //指数为1，不显示指数，仅有系数和x，其中系数为1或-1时省略其中的1
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
} //从多项式单链表中读取元素并以特定文件名保存到文件中

void poly_destory(poly_LinkList L) {
    poly_Node* pre, * p;
    if (L->next != NULL) {
        pre = L->next; //初始化pre到链表首位元素
        L->next = NULL; //原头结点指向置空
        if (L->expn > 0) {
            for (int i = 1; i <= L->expn; i++) {
                p = pre->next;
                free(pre);
                pre = p;
            } //遍历链表元素并释放空间
        } //双保险，再次确认链表内有元素
    } //查看单链表有无元素，若有，则删除，若无，则只需删除头结点
    free(L); //释放头结点空间
    //printf("successfully destory a polynominal LinkList.\n");
    return;
} //删除链表，释放内存

poly_LinkList poly_plus(poly_LinkList A, poly_LinkList B) {
    if (A->next == NULL) return B;
    if (B->next == NULL) return A; //一多项式链表无内容则返回另一多项式链表
    poly_LinkList C = poly_Init(); //创建并初始化链表C
    struct poly_Node* pa, * pb, * pc, * prec;
    pa = A->next; pb = B->next;
    int i = 0, expn;
    float coef;
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
            if (!pc) { printf("overflow!\n"); /*return;*/ exit(OVERFLOW); }
            pc->coef = coef;
            pc->expn = expn;
            /*
            pc->next = prec->next; //插入C的头部
            prec->next = pc;
            */
            /*
            pc->next = NULL;
            while (prec->next != NULL) prec = prec->next; //插入C的尾部
            prec->next = pc;
            */
            while (prec->next != NULL && pc->expn < prec->next->expn) prec = prec->next;
            pc->next = prec->next;
            prec->next = pc; //降幂排列插入C
        } //若求和后或现有的系数非0，则将元素纳入链表C
    } //A、B均有元素时，元素按从大到小逐渐插入链表C末尾处
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
            /*
            pc->next = prec->next;
            prec->next = pc; //插入C的头部
            */
            /*
            pc->next = NULL;
            while (prec->next != NULL) prec = prec->next; //插入C的尾部
            prec->next = pc;
            */
            while (prec->next != NULL && pc->expn < prec->next->expn) prec = prec->next;
            pc->next = prec->next;
            prec->next = pc; //降幂排列插入C
        }
    } //经过第一个循环后若剩下A，则继续添加A的元素
    while (pb != NULL) {
        coef = pb->coef;
        expn = pb->expn;
        pb = pb->next;
        prec = C;
        if (coef != 0) {
            pc = (poly_LinkList)malloc(sizeof(poly_Node));
            if (!pc) { printf("overflow!\n"); /*return;*/ exit(OVERFLOW); }
            pc->coef = coef;
            pc->expn = expn;
            /*
            pc->next = prec->next;
            prec->next = pc; //插入C的头部
            */
            /*
            pc->next = NULL;
            while (prec->next != NULL) prec = prec->next; //插入C的尾部
            prec->next = pc;
            */
            while (prec->next != NULL && pc->expn < prec->next->expn) prec = prec->next;
            pc->next = prec->next;
            prec->next = pc; //降幂排列插入C
        }
    } //经过第一个循环后若剩下B，则继续添加B的元素
    C->expn = A->expn + B->expn - i;
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
    return C; //返回相加所得的链表C
}

poly_LinkList poly_minus(poly_LinkList A, poly_LinkList B) {
    if (A->next == NULL) return B;
    if (B->next == NULL) return A;
    poly_LinkList C = poly_Init(); //创建并初始化链表C
    struct poly_Node* pa, * pb, * pc, * prec;
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
            coef = -pb->coef;
            expn = pb->expn;
            pb = pb->next;
        } //B中元素指数比A大时，放入B中元素
        prec = C;
        if (coef != 0) {
            pc = (poly_LinkList)malloc(sizeof(poly_Node));
            if (!pc) { printf("overflow!\n"); /*return;*/ exit(OVERFLOW); }
            pc->coef = coef;
            pc->expn = expn;
            while (prec->next != NULL && pc->expn < prec->next->expn) prec = prec->next;
            pc->next = prec->next;
            prec->next = pc; //降幂排列插入C
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
            while (prec->next != NULL && pc->expn < prec->next->expn) prec = prec->next;
            pc->next = prec->next;
            prec->next = pc; //降幂排列插入C
        }
    } //经过第一个循环后若剩下A，则继续添加A的元素
    while (pb != NULL) {
        coef = -pb->coef;
        expn = pb->expn;
        pb = pb->next;
        prec = C;
        if (coef != 0) {
            pc = (poly_LinkList)malloc(sizeof(poly_Node));
            if (!pc) { printf("overflow!\n"); /*return;*/ exit(OVERFLOW); }
            pc->coef = coef;
            pc->expn = expn;
            while (prec->next != NULL && pc->expn < prec->next->expn) prec = prec->next;
            pc->next = prec->next;
            prec->next = pc; //降幂排列插入C
        }
    } //经过第一个循环后若剩下B，则继续添加B的元素
    C->expn = A->expn + B->expn - i;
    return C;
} //和加法子程序唯一区别在于所有B链表的系数增加负号

void poly_select(poly_LinkList L, int i) {
    struct poly_Node* p;
    p = L->next;
    if (i <= L->expn && i > 0) {
        for (int j = 1; j < i; j++) p = p->next;
        printf("the %d of the polynominal is (%.1f,%d),its mathematical form is:", i, p->coef, p->expn);
        if (p->expn == 1) {
            if (p->coef == 1) printf("x");
            else if (p->coef == -1) printf("-x");
            else printf("%.1fx", p->coef);
        } //指数为1，不显示指数，仅有系数和x，其中系数为1或-1时省略其中的1
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
} //从链表中取值并展示子程序，需输入取多项式链表中的第几位元素，如果位置无元素则显示超出范围。

poly_LinkList poly_diff(poly_LinkList L) {
    struct poly_Node* p, * pre, * pdiff, * prediff;
    poly_LinkList diffL = poly_Init(); diffL->expn = 0;
    pre = L;
    while (pre->next) {
        prediff = diffL;
        p = pre->next; //让p为pre的下一位
        if (p->expn != 0) {
            pdiff = (poly_LinkList)malloc(sizeof(poly_Node));
            if (!pdiff) { printf("overflow!\n"); /*return;*/ exit(OVERFLOW); }
            pdiff->coef = p->expn * p->coef;
            pdiff->expn = p->expn - 1;
            while (prediff->next != NULL && pdiff->expn < prediff->next->expn) prediff = prediff->next;
            pdiff->next = prediff->next;
            prediff->next = pdiff; //降幂排列插入C
            diffL->expn = diffL->expn + 1;
        } //指数不为0正常求导，新系数=指数*老系数，新指数=指数-1
        //else { pre->next = p->next; free(p); } //指数为0项则不加入新多项式（因为其为常数，求导后是0）
        pre = pre->next; //下移
    } //遍历链表
    return diffL;
} //多项式求导子函数，输入参数为要求导的多项式链表，输出为求导所得新多项式链表

poly_LinkList poly_multi(poly_LinkList A, poly_LinkList B) {
    struct poly_Node* pa, * pb, * pc, * prec;
    poly_LinkList C = poly_Init(); C->expn = 0;
    if (A->next == NULL || B->next == NULL) return C; //如果AB任一无元素，为0，则立即返回C，此时C无值（看作乘积为0）
    else {
        pa = A->next; //初始化到链表A首位
        while (pa != NULL) {
            pb = B->next; //每次循环pa时B回到首位
            while (pb != NULL) {
                prec = C;
                pc = (poly_LinkList)malloc(sizeof(poly_Node));
                if (!pc) { printf("overflow!\n"); /*return;*/ exit(OVERFLOW); }
                pc->coef = pa->coef * pb->coef; //系数相乘
                pc->expn = pa->expn + pb->expn; //指数相加
                while (prec->next != NULL && pc->expn < prec->next->expn) prec = prec->next; //降幂排列位置
                if (prec->next != NULL && pc->expn == prec->next->expn) {
                    prec->next->coef = prec->next->coef + pc->coef; //相同指数项合并
                    free(pc);
                } //幂相等，则系数相加合并两元素到prec->next->coef内，释放pc
                else {
                    pc->next = prec->next;
                    prec->next = pc; //插入到降幂位置
                    C->expn = C->expn + 1;
                }
                pb = pb->next; //B下移（但每次A循环B刷新回始位）
            }
            pa = pa->next; //A下移，直到A循环完毕
        }
    }
    return C;
} //多项式相乘子程序

void main() {
    poly_LinkList ListA, ListB, C, antiC/*,AmultiB, diffBminusdiffA*/;
    ListA = poly_Init(); ListB = poly_Init(); //初始化链表
    int i = 1;
    char in_name[AUTOLENGTH], out_name[AUTOLENGTH];
    printf("input 1st file:");
    scanf_s("%s", &in_name, AUTOLENGTH);
    poly_readfile(in_name, ListA); //读取指定文件并将内容存至指定链表

    printf("\ninput 2nd file:");
    scanf_s("%s", &in_name, AUTOLENGTH);
    poly_readfile(in_name, ListB);

    C = poly_plus(ListA, ListB); //将多项式链表A、B相加得到链表AplusB
    antiC = poly_plus(ListB, ListA);
    //AmultiB = poly_multi(ListA, ListB); //将多项式链表A、B相乘得到链表AmultiB
    //diffBminusdiffA = poly_minus(poly_diff(ListB), poly_diff(ListA)); //将多项式B的导减去多项式A的导，输出为diffBminusdiffA

    printf("\noutput 1st file:");
    scanf_s("%s", &out_name, AUTOLENGTH);
    poly_writefile(out_name, ListA); //输出指定链表内容到指定文件

    printf("\noutput 2nd file:");
    scanf_s("%s", &out_name, AUTOLENGTH);
    poly_writefile(out_name, ListB);

    printf("\noutput 1st+2nd file:");
    scanf_s("%s", &out_name, AUTOLENGTH);
    poly_writefile(out_name, C);

    printf("\noutput 2nd+1st file:");
    scanf_s("%s", &out_name, AUTOLENGTH);
    poly_writefile(out_name, antiC);
    /*
    printf("\noutput 1st*2nd file:");
    scanf_s("%s", &out_name, AUTOLENGTH);
    poly_writefile(out_name, AmultiB);

    printf("\noutput diff(2nd)-diff(1st)] file:");
    scanf_s("%s", &out_name, AUTOLENGTH);
    poly_writefile(out_name, diffBminusdiffA);
    
    //下面为自动输出，取值函数的使用
    printf("\nat 1st poly LinkList:\n");
    poly_select(ListA, 1);
    poly_select(ListA, 0);
    poly_select(ListA, 2);
    poly_select(ListA, -2);
    poly_select(ListA, 8);
    poly_select(ListA, 7); //链表A特定位置元素取值

    printf("\nat 1st+2nd poly LinkList:\n");
    for (int i = 1; i <= AplusB->expn; i++) poly_select(AplusB, i); //链表A+B后的所有元素取值展示
    */
    poly_destory(ListA);
    poly_destory(ListB);
    poly_destory(C);
    poly_destory(antiC);
    //poly_destory(AmultiB);
    //poly_destory(diffBminusdiffA); //删除链表
    return;
} //主函数
