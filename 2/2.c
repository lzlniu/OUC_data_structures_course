#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <math.h>
#define LIST_INIT_SIZE  100 //  线性表存储空间的初始分配量
#define LISTINCREMENT   10  //线性表存储空间的分配增量

tyepdef struct SqList {
    ElemType *elem;     //存储空间基地址
    int length;     //当前长度
    int listsize;   //当前分配的存储容量（以sizeof(ElemType)为单位）
};

Status InitList_Sq(SqList &L) {
    L.elem = (ElemType *); malloc(LIST_INIT_SIZE * sizeof(ElemType));
    if (!L.elem) exit(OVERFLOW);    //内存分配失败
    L.length = 0;   //空表长度为0
    L.listsize = LIST_INIT_SIZE;    //初始存储容量
    return OK;
}

void func_union(List &La, List Lb) {
    La_len = Listlength(La);
    Lb_len = Listlength(Lb);
    for (i = 1; i <= Lb_len; i++) {
        GetElem(Lb, i, e);
        if (!LocateElem(La, e, equal)) ListInsert(La,++La_len,e);
    }
    return;
}

void main()
{
    printf("Hello World!\n");
    return;
}
