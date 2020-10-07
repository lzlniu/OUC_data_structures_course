#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <cstring>
#include <cstdlib>
#include <cstdio>
#include <cmath>
#include <ctime>
#define OK 1
#define ERROR 0
#define FILENAME_LEN 32
#define ASCII_NUM 256

//author:Zelin Li
//date: 2020.06.26

typedef struct huffnode { //哈夫曼树
    int weight, data;
    int lchild, rchild, parent; //顺序存储结构
}huffnode, *hufftree;

typedef struct huffcode {
    int weight, data;
    int code[256], start;
}huffcode;

typedef struct node { //用于存储字符频数统计结果
    int weight, data;
    struct node *next;
}node, *list;

typedef struct character { //用于存储文章
    char ch;
    struct character* next;
}character, * story;

void min_two(hufftree HT, int n, int &s1, int &s2) {
    int min = 0;
    for (int i = 1; i <= n; i++) { 
        if (HT[i].parent == 0) {
            min = i;
            break;
        } //每个parent==0结点终止（此结点暂时无parent，但不代表其为叶子或真正的根，只有第一次循环时为叶子，最后一次为真正的根）
    }
    for (int i = 1; i <= n; i++)
        if (HT[i].parent == 0)
            if (HT[i].weight < HT[min].weight)
                min = i; //找到目前无parent结点当中最小的，记录为min
    s1 = min; //第一个parent为0的最小结点
    for (int i = 1; i <= n; i++) {
        if (HT[i].parent == 0 && i != s1) {
            min = i;
            break;
        } //同寻找s1的方法，但i不可等于s1
    }
    for (int i = 1; i <= n; i++)
        if (HT[i].parent == 0 && i != s1)
            if (HT[i].weight < HT[min].weight)
                min = i;
    s2 = min; //第二个parent为0的次小结点（s1之外的次小结点）
    return;
}

void print_hufftree(hufftree HT, int m) {
    printf("\nstructure of Huffman tree:\n");
    printf("num\tchar\tweight\tparent\tlchild\trchild\n");
    for (int i = 1; i <= m; i++) {
        if (HT[i].data >= 0 && HT[i].data < ASCII_NUM) {
            if (HT[i].data == 10) printf("%d\tLF(%d)\t%d\t%d\t%d\t%d\n", i, HT[i].data, HT[i].weight, HT[i].parent, HT[i].lchild, HT[i].rchild); //ascii为10，换行符LF
            else if (HT[i].data == 0) printf("%d\tNUL(%d)\t%d\t%d\t%d\t%d\n", i, HT[i].data, HT[i].weight, HT[i].parent, HT[i].lchild, HT[i].rchild); //ascii为0，NUL字符
            else if (HT[i].data == 13) printf("%d\tCR(%d)\t%d\t%d\t%d\t%d\n", i, HT[i].data, HT[i].weight, HT[i].parent, HT[i].lchild, HT[i].rchild); //ascii为13，回车符CR
            else if (HT[i].data == 9) printf("%d\tTAB(%d)\t%d\t%d\t%d\t%d\n", i, HT[i].data, HT[i].weight, HT[i].parent, HT[i].lchild, HT[i].rchild); //ascii为9，制表符TAB
            else printf("%d\t%c(%d)\t%d\t%d\t%d\t%d\n", i, HT[i].data, HT[i].data, HT[i].weight, HT[i].parent, HT[i].lchild, HT[i].rchild); //其他
        }
        else printf("%d\ttrunk\t%d\t%d\t%d\t%d\n", i, HT[i].weight, HT[i].parent, HT[i].lchild, HT[i].rchild); //非叶子结点，显示“树干”trunk
    }
    return;
} //显示Huffman树

void gen_hufftree(list L, hufftree &HT) {
    struct node* p = L->next;
    int i, m = 2 * L->data - 1, s1, s2;	//m为所有结点的个数，此值事先可求出（2倍叶子结点数-1）
    if (L->data <= 1) return; //头结点的L->data存放list中元素数，如果此值小于等于1则无需生成Huffman树
    HT = new huffnode[m + 1]; //前面存放叶子，后面存放非叶子
    for (i = 1; i <= m; ++i) {
        HT[i].weight = 0;
        HT[i].data = -1;
        HT[i].parent = 0;
        HT[i].lchild = 0;
        HT[i].rchild = 0;
    } //遍历结点初始化
    for (i = 1; i <= L->data; ++i) {
        if (p) {
            HT[i].weight = p->weight;
            HT[i].data = p->data;
            p = p->next;
        }
    } //给叶子结点赋权值、ASCII值（放入树（顺序存储）中）
    for (i = L->data + 1; i <= m; ++i) { //从非叶子结点开始创建
        min_two(HT, i - 1, s1, s2); //找到当前树中两个权重最小的结点（存到s1、s2）
        HT[s1].parent = i;
        HT[s2].parent = i; //给这两个最小的叶子结点共同parent
        HT[i].lchild = s1;
        HT[i].rchild = s2; //让该parent的左右孩子为这两个结点
        HT[i].weight = HT[s1].weight + HT[s2].weight; //权重为这两结点加和
    }
    return;
}

void print_huffcode(huffcode HC[], int n, char out_file_name[]) {
    FILE* out;
    if ((out = fopen(out_file_name, "w")) == NULL) { printf("can not open %s\n", out_file_name); return; }
    printf("\nthere are %d kind of characters in the story, each character frequence and their Huffman code are:\n", n);
    for (int i = 1; i <= n; i++) {
        if (HC[i].data == 10) {
            printf("LF(%d)\t%d\t", HC[i].data, HC[i].weight);
            fprintf(out, "LF(%d)\t", HC[i].data);  
        }
        else if (HC[i].data == 0) {
            printf("NUL(%d)\t%d\t", HC[i].data, HC[i].weight);
            fprintf(out, "NUL(%d)\t", HC[i].data);
        }
        else if (HC[i].data == 13) {
            printf("CR(%d)\t%d\t", HC[i].data, HC[i].weight);
            fprintf(out, "CR(%d)\t", HC[i].data);
        }
        else if (HC[i].data == 9) {
            printf("TAB(%d)\t%d\t", HC[i].data, HC[i].weight);
            fprintf(out, "TAB(%d)\t", HC[i].data);
        }
        else {
            printf("%c(%d)\t%d\t", HC[i].data, HC[i].data, HC[i].weight);
            fprintf(out, "%c(%d)\t", HC[i].data, HC[i].data);
        }
        for (int j = HC[i].start + 1; j < n; j++) {
            printf("%d", HC[i].code[j]);
            fprintf(out, "%d", HC[i].code[j]); //输出Huffman编码及其字符对应到文件
        }
        printf("\n");
        fprintf(out, "\n");
    } //输出显示Huffman编码
    return;
}

void gen_huffcode(int size, hufftree HT, huffcode HC[]) {
    struct huffcode p;
    int i, j, m, n;
    if (HT != NULL) {
        for (i = 1; i <= size; i++) {
            p.start = size - 1; //初始编码位置
            m = i; //m为本结点位置
            n = HT[m].parent; //n为parent结点位置
            while (n != 0) { //该结点有parent结点时，执行循环
                if (HT[n].lchild == m) p.code[p.start] = 0; //本结点在parent的左子树，记0
                else p.code[p.start] = 1; //本结点在parent的右子树，记1
                p.start--; //编码起始位置上移
                m = n; //本结点上移（到原parent结点处）
                n = HT[m].parent; //下个循环
            }
            for (j = p.start + 1; j < size; j++) HC[i].code[j] = p.code[j];
            HC[i].start = p.start; //start记录每个Huffman编码在code中的起始位置
            HC[i].data = HT[i].data;
            HC[i].weight = HT[i].weight;
        }
    }
    return;
}

void code_huff(story P, huffcode HC[], int n, char out_file_name[]) {
    struct character* p = P;
    FILE* out;
    if ((out = fopen(out_file_name, "w")) == NULL) { printf("can not open %s\n", out_file_name); return; }
    while (p) {
        if (p->ch >= 0 && p->ch < ASCII_NUM) {
            for (int i = 1; i <= n; i++) {
                if (p->ch == HC[i].data) {
                    for (int j = HC[i].start + 1; j < n; j++) fprintf(out, "%d", HC[i].code[j]);
                } //找到该字符对应的编码输出
            }
        } //非特殊字符，正常找到其对应编码进行编码
        else {
            for (int i = 1; i <= n; i++) {
                if ('\0' == HC[i].data) {
                    for (int j = HC[i].start + 1; j < n; j++) fprintf(out, "%d", HC[i].code[j]);
                } //找到该字符对应的编码输出
            }
        } //特殊字符转化为NUL（'\0'）对应的编码
        p = p->next;
    } //遍历文章
    fclose(out);
    return;
} //根据输入的文章、Huffman编码方式；进行Huffman编码输出编码结果到文件

void decode_huff(hufftree HT, int n, char in_file_name[], char out_file_name[]) {
    FILE* in, * out;
    int temp, m = n;
    if ((in = fopen(in_file_name, "r")) == NULL) { printf("can not open %s\n", in_file_name); return; } //打开输入文件
    if ((out = fopen(out_file_name, "w")) == NULL) { printf("can not open %s\n", out_file_name); return; } //打开输出文件
    while (!feof(in)) {
        if (HT[m].lchild != 0 || HT[m].rchild != 0) {
            temp = (int)fgetc(in) - 48; //fgetc得到的为字符0或1，其要减去48才是数值0或1
            if (temp == 0) {
                m = HT[m].lchild;
            } //前往左孩子结点
            else if (temp == 1) {
                m = HT[m].rchild;
            } //前往右孩子结点
        } //非叶子结点时，持续遍历
        else if (HT[m].lchild == 0 && HT[m].rchild == 0) {
            fprintf(out, "%c", HT[m].data);
            m = n;
        } //叶子结点
    }
    fclose(in);
    fclose(out);
    return;
} //读取哈夫曼编码结果，根据哈夫曼树（哈夫曼编码）将其译码成原文章

list init_list() {
    list L = (list)malloc(sizeof(node));
    if (!L) { printf("overflow!\n"); exit(0); }
    L->weight = 0; //头结点初始化为0
    L->data = 0;
    L->next = NULL; //指针域指向NULL
    return L;
}

void bubble_sort(list L) {
    struct node* pre, * p, * tail; //设立尾指针以方便定位
    tail = NULL;
    while ((L->next->next) != tail) {
        pre = L;
        p = L->next;
        while (p->next != tail) {
            if (p->weight > p->next->weight) { //从小到大排序
                pre->next = p->next;
                p->next = p->next->next;
                pre->next->next = p;
                p = pre->next;
            }
            p = p->next;
            pre = pre->next;
        }
        tail = p;
    }
    return;
} //冒泡排序

void print_list(list L) {
    struct node* pre;
    pre = L->next;
    while (pre) {
        if (pre->data == 10) printf("LF(%d)\t%d\n", pre->data, pre->weight); //ascii为10，换行符LF
        else if (pre->data == 0) printf("NUL(%d)\t%d\n", pre->data, pre->weight); //ascii为0，NUL字符
        else if (pre->data == 13)printf("CR(%d)\t%d\n", pre->data, pre->weight); //ascii为13，回车符CR
        else if (pre->data == 9)printf("TAB(%d)\t%d\n", pre->data, pre->weight); //ascii为9，制表符TAB
        else printf("%c(%d)\t%d\n", pre->data, pre->data, pre->weight);
        pre = pre->next;
    }
    printf("there are %d kind of ascii characters in this list\n", L->data);
    return;
} //显示存储频数数据线性表的内容

list array_to_list(int ascii[]) {
    struct node* p, * pre; int i, j = 0;
    list L = init_list(); //带头结点，初始化
    pre = L;
    for (i = 0; i < ASCII_NUM; i++) {
        if (ascii[i] > 0) {
            p = (list)malloc(sizeof(node));
            if (!p) { printf("overflow!\n"); exit(0); }
            p->weight = (int)ascii[i]; //权重即为频数
            p->data = i; //数组序号即为字符本身的ascii值，需要记录
            p->next = pre->next;
            pre->next = p; //插入新元素
            ++j; //增加字符种数
        } //只统计数量多于0的字符
    }
    L->data = j; //记录字符种数
    bubble_sort(L); //对链表内容进行冒泡排序
    return L;
} //将数组的统计结果存入线性表

void print_story(story P) {
    struct character* pre;
    pre = P->next;
    while (pre) {
        printf("%c", pre->ch);
        pre = pre->next;
    }
    return;
} //显示存储在线性表的文章

void count_ascii(story P, int ascii[]) {
    int i; //i记录ascii值
    for (i = 0; i < ASCII_NUM; i++) ascii[i] = 0; //初始化成均为0
    struct character* pre;
    pre = P->next;
    while (pre) {
        if (pre->ch >= 0 && pre->ch < ASCII_NUM) {
            //printf("%c", ch); //显示ascii字符
            ascii[(int)pre->ch]++;
        } //是ascii字符，则该字符计数加1
        else {
            //printf("%c", 0); //非ascii字符显示为NUL
            ascii[0]++;
        } //若不是ascii字符，则该字符记为NUL字符，其ascii值为0
        pre = pre->next;
    }
    /*
    for (i = 0; i < ASCII_NUM; i++) if (ascii[i] != 0) {
        if (i == 10) printf("LF(%d)\t%d\n",i, ascii[i]); //ascii为10，换行符LF
        else if (i == 0) printf("NUL(%d)\t%d\n", i, ascii[i]); //ascii为0，NUL字符
        else if (i == 13)printf("CR(%d)\t%d\n", i, ascii[i]); //ascii为13，回车符CR
        else if (i == 9)printf("TAB(%d)\t%d\n", i, ascii[i]); //ascii为9，制表符TAB
        else printf("%c(%d)\t%d\n", i, i, ascii[i]);
    } //输出所有出现过的ascii字符
    printf("there are %d kind of ascii characters in %s\n\n", ascii_num, file_name);
    */
    return;
}

story read_txt(char file_name[]) {
    story P = (story)malloc(sizeof(character));
    if (!P) { printf("overflow!\n"); exit(0); } //空间开辟失败
    P->next = NULL; //初始化P，无头结点
    character* p, * pre; int k = 0; //k记录初始状态，首位时为0，非首位时大于0
    FILE* fp;
    if ((fp = fopen(file_name, "r")) == NULL) { printf("can not open %s\n", file_name); exit(0); } //打开文件
    pre = P;
    while (!feof(fp)) {
        if (k < 1) P->ch= fgetc(fp);
        else {
            p = (story)malloc(sizeof(character));
            if (!p) { printf("overflow!\n"); exit(0); }
            p->ch = fgetc(fp);
            p->next = NULL;
            pre->next = p;
            pre = pre->next;
        } //按顺序将文章内容存储入线性表结构
        ++k; //记录总字符数
    }
    printf("\ntotal characters amount in %s is %d\n", file_name, k);
    //print_story(P); //显示文章
    fclose(fp);
    return P;
}

void ascii_to_bin(char ascii, int ascii_bin[]) {
    int i;
    for (i = 0; i < 8; i++) {
        ascii_bin[7 - i] = ascii % 2;
        ascii = ascii / 2;
    }
    return;
} //根据ascii字符的ascii值，求其二进制表示，存入数组ascii_bin[]中，此处数组长度为8

void code_ascii_bin (story P, char out_file_name[]) {
    int i, bin[8];
    struct character* p = P;
    FILE* out;
    if ((out = fopen(out_file_name, "w")) == NULL) { printf("can not open %s\n", out_file_name); return; } //打开输出文件
    while (p) {
        memset(bin, 0, sizeof(bin));
        ascii_to_bin(p->ch, bin); //将字符转化为8位二进制码
        if (p->ch >= 0 && p->ch < ASCII_NUM) for (i = 0; i < 8; i++) fprintf_s(out, "%d", bin[i]); //在0-255内，是ascii字符，将ascii转二进制到输出到文件
        else fprintf_s(out, "00000000"); //非ascii字符，设为NUL，其二进制码即为00000000
        p = p->next;
    } //遍历文章字符
    fclose(out);
    return;
}

void decode_ascii_bin(char in_file_name[], char out_file_name[]) {
    int sum = 0, temp, k = 1;
    FILE* in, * out;
    if ((in = fopen(in_file_name, "r")) == NULL) { printf("can not open %s\n", in_file_name); return; } //打开输入文件
    if ((out = fopen(out_file_name, "w")) == NULL) { printf("can not open %s\n", out_file_name); return; } //打开输出文件
    while (!feof(in)) {
        if (k <= 8) {
            temp = (int)fgetc(in) - 48; //fgetc得到的为字符0或1，其要减去48才是数值0或1
            sum = (int)(sum + temp * (int)pow(2, 8 - k)); //8位二进制转10进制
            ++k;
        } //以8个字符为单位检索，加和得到sum求出该二进制码对应的ascii值
        else {
            k = 1;
            fputc(sum, out); //输出
            sum = 0;
        } //当k大于8时，存储输出sum值（ascii值，对应字符），并重置sum为0
    }
    fclose(in);
    fclose(out);
    return;
}

void char_compare(story P, char file2_name[]) {
    char ch; int different = 0;
    struct character* p = P;
    FILE* fp;
    if ((fp = fopen(file2_name, "r")) == NULL) { printf("can not open %s\n", file2_name); return; } //打开输入文件
    while (p) {
        ch = fgetc(fp);
        if (ch != p->ch) {
            if (ch != 0) {
                //printf("%c is different from %c  (%d)\n", ch, p->ch, p->ch); //如某位置字符不同，则该位置的两文件差异字符将被显示
                ++different;
            } //NUL字符匹配所有的非ascii码；只有字符不同时才记录
        } //当两同一位置的字符不同时记录
        p = p->next;
    } //逐字符比较找两文件，二者都要被遍历完全
    fclose(fp);
    printf("\nthere are %d differences in %s compare to the original story\n", different, file2_name); //统计不同字符数，如果比较的是同一文件编码、解码前后差异，则差异应为0
    return;
}

int main() {
    int start_time, end_time;
    FILE* t;
    if ((t = fopen("TimeCompare.txt", "w")) == NULL) { printf("can not open TimeCompare.txt\n"); return 0; }
    char short_s[FILENAME_LEN] = "ShortStory.txt", long_s[FILENAME_LEN] = "LongStory.txt", short_ascii_code[FILENAME_LEN] = "ShortASCCode.txt", long_ascii_code[FILENAME_LEN] = "LongASCCode.txt", short_ascii_decode[FILENAME_LEN] = "ShortASCDecode.txt", long_ascii_decode[FILENAME_LEN] = "LongASCDecode.txt", short_huff[FILENAME_LEN] = "ShortHuf.txt", long_huff[FILENAME_LEN] = "LongHuf.txt", short_huff_code[FILENAME_LEN] = "ShortCode.txt", long_huff_code[FILENAME_LEN] = "LongCode.txt", short_huff_decode[FILENAME_LEN] = "ShortDecode.txt", long_huff_decode[FILENAME_LEN] = "LongDecode.txt";
    int asciiS[ASCII_NUM], asciiL[ASCII_NUM]; //数组用于接受函数输出的各字符频数
    hufftree htreeS, htreeL;
    huffcode hcodeS[256], hcodeL[256];
    printf("Zelin Li | 2020.06.29\n");

    printf("\nShort story processing:\n");
    story short_story = read_txt(short_s); //将文章从文件读取到线性表结构中
    count_ascii(short_story, asciiS); //字符频数统计结果存入数组
    list list_asciiS = array_to_list(asciiS); //将字符频数统计结果由数组迁移到了线性表并完成排序（从大到小）
    //print_list(list_asciiS); //显示字符频数统计结果（但此结果也被复制到了huffcode当中，所以后面的print_huffcode也展示了此内容）
    gen_hufftree(list_asciiS, htreeS); //由该线性表生成Huffman树
    print_hufftree(htreeS, 2 * list_asciiS->data - 1); //显示生成的Huffman树到命令行
    gen_huffcode(list_asciiS->data , htreeS, hcodeS); //由Huffman树生成Huffman编码
    print_huffcode(hcodeS, list_asciiS->data, short_huff); //命令行显示字符频数统计结果和Huffman编码结果，Huffman编码及其对应字符输出到文件

    start_time = clock();
    code_huff(short_story, hcodeS, list_asciiS->data, short_huff_code); //对原文进行Huffman编码，结果输出到文件
    end_time = clock();
    fprintf(t, "%s Huffman coding time:%d ms\n", short_s, end_time - start_time);

    start_time = clock();
    decode_huff(htreeS, 2 * list_asciiS->data - 1, short_huff_code, short_huff_decode); //将Huffman码二进制形式文章内容翻译为正常字符并存储到文件中（通过Huffman树）
    end_time = clock();
    fprintf(t, "%s Huffman decoding time:%d ms\n", short_s, end_time - start_time);

    start_time = clock();
    code_ascii_bin(short_story, short_ascii_code); //将线性表结构文章内容译为ascii码二进制形式存储到文件中
    end_time = clock();
    fprintf(t, "%s binary-ASCII coding time:%d ms\n", short_s, end_time - start_time);

    start_time = clock();
    decode_ascii_bin(short_ascii_code, short_ascii_decode); //将ascii码二进制形式文件解码，得到解码后的文章内容存储到文件中
    end_time = clock();
    fprintf(t, "%s binary-ASCII decoding time:%d ms\n", short_s, end_time - start_time);

    printf("\nLong story processing:\n");
    story long_story = read_txt(long_s);
    count_ascii(long_story, asciiL);
    list list_asciiL = array_to_list(asciiL);
    //print_list(list_asciiL);
    gen_hufftree(list_asciiL, htreeL);
    print_hufftree(htreeL, 2 * list_asciiL->data - 1);
    gen_huffcode(list_asciiL->data, htreeL, hcodeL);
    print_huffcode(hcodeL, list_asciiL->data, long_huff);

    start_time = clock();
    code_huff(long_story, hcodeL, list_asciiL->data, long_huff_code);
    end_time = clock();
    fprintf(t, "%s Huffman coding time:%d ms\n", long_s, end_time - start_time);

    start_time = clock();
    decode_huff(htreeL, 2 * list_asciiL->data - 1, long_huff_code, long_huff_decode);
    end_time = clock();
    fprintf(t, "%s Huffman decoding time:%d ms\n", long_s, end_time - start_time);

    start_time = clock();
    code_ascii_bin(long_story, long_ascii_code);
    end_time = clock();
    fprintf(t, "%s binary-ASCII coding time:%d ms\n", long_s, end_time - start_time);

    start_time = clock();
    decode_ascii_bin(long_ascii_code, long_ascii_decode);
    end_time = clock();
    fprintf(t, "%s binary-ASCII decoding time:%d ms\n", long_s, end_time - start_time);

    char_compare(short_story, short_ascii_decode); //逐字符比较解码后文章和原文章的区别，差异数会显示
    char_compare(short_story, short_huff_decode);
    char_compare(long_story, long_ascii_decode);
    char_compare(long_story, long_huff_decode);

    fclose(t);
    system("pause");
    return 0;
}

/*
//附录：未完成算法尝试
node pop(list L) {
    struct node popnode; //此非指针，而是node结构，记录弹出的元素
    struct node* p; //临时指针
    if (L->data <= 0) { //字符种数为0
        printf("no more types of character\n");
        popnode.weight = 0; //无元素时，函数返回一个权重为0的popnode，表明没有元素了
        return popnode;
    } //已无元素
    popnode.data = L->next->data; //记录第一个元素字符信息
    popnode.weight = L->next->weight; //记录第一个元素的权重信息
    popnode.next = NULL;
    p = L->next;
    L->next = L->next->next; //元素下推（因为第一个元素弹出，此位置需要清除）
    free(p); //释放原表头弹出的元素占用空间
    L->data--; //表内元素数（字符种数）减1
    return popnode;
} //类似“出栈”

hufftree create_hufftree(list L) {
    struct huffnode* hn, * lc, * rc;
    struct node pn1, pn2;
    int k = 0; //记录状态：是否是首次记录叶子，首次则为0，其他情况为1
    while (L->next) {
        pn1 = pop(L);
        if (pn1.weight == 0) return;
        if (k <= 0 && L->next != NULL) {
            pn2 = pop(L);
            if (pn2.weight == 0) return;
            lc = (hufftree)malloc(sizeof(huffnode));
            if (!lc) { printf("overflow!\n"); }
            lc->data = pn1.data;
            lc->weight = pn1.weight;
            lc->parent = hn;
            lc->lchild = NULL;
            lc->rchild = NULL;
            rc = (hufftree)malloc(sizeof(huffnode));
            if (!rc) { printf("overflow!\n"); }
            rc->data = pn2.data;
            rc->weight = pn2.weight;
            rc->parent = hn;
            rc->lchild = NULL;
            rc->rchild = NULL;
            hn = (hufftree)malloc(sizeof(huffnode));
            if (!hn) { printf("overflow!\n"); }
            hn->weight = pn1.weight + pn2.weight;
            hn->lchild = lc;
            hn->rchild = rc;
            k = 1; //刷新状态
        }
        else if (k == 1 && L->next->next != NULL && pn1.weight <= L->next->weight && pn1.weight <= L->next->next->weight) {

        }
    }
    return hn;
} //未完成
*/
