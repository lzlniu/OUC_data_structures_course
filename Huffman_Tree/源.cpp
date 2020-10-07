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

typedef struct huffnode { //��������
    int weight, data;
    int lchild, rchild, parent; //˳��洢�ṹ
}huffnode, *hufftree;

typedef struct huffcode {
    int weight, data;
    int code[256], start;
}huffcode;

typedef struct node { //���ڴ洢�ַ�Ƶ��ͳ�ƽ��
    int weight, data;
    struct node *next;
}node, *list;

typedef struct character { //���ڴ洢����
    char ch;
    struct character* next;
}character, * story;

void min_two(hufftree HT, int n, int &s1, int &s2) {
    int min = 0;
    for (int i = 1; i <= n; i++) { 
        if (HT[i].parent == 0) {
            min = i;
            break;
        } //ÿ��parent==0�����ֹ���˽����ʱ��parent������������ΪҶ�ӻ������ĸ���ֻ�е�һ��ѭ��ʱΪҶ�ӣ����һ��Ϊ�����ĸ���
    }
    for (int i = 1; i <= n; i++)
        if (HT[i].parent == 0)
            if (HT[i].weight < HT[min].weight)
                min = i; //�ҵ�Ŀǰ��parent��㵱����С�ģ���¼Ϊmin
    s1 = min; //��һ��parentΪ0����С���
    for (int i = 1; i <= n; i++) {
        if (HT[i].parent == 0 && i != s1) {
            min = i;
            break;
        } //ͬѰ��s1�ķ�������i���ɵ���s1
    }
    for (int i = 1; i <= n; i++)
        if (HT[i].parent == 0 && i != s1)
            if (HT[i].weight < HT[min].weight)
                min = i;
    s2 = min; //�ڶ���parentΪ0�Ĵ�С��㣨s1֮��Ĵ�С��㣩
    return;
}

void print_hufftree(hufftree HT, int m) {
    printf("\nstructure of Huffman tree:\n");
    printf("num\tchar\tweight\tparent\tlchild\trchild\n");
    for (int i = 1; i <= m; i++) {
        if (HT[i].data >= 0 && HT[i].data < ASCII_NUM) {
            if (HT[i].data == 10) printf("%d\tLF(%d)\t%d\t%d\t%d\t%d\n", i, HT[i].data, HT[i].weight, HT[i].parent, HT[i].lchild, HT[i].rchild); //asciiΪ10�����з�LF
            else if (HT[i].data == 0) printf("%d\tNUL(%d)\t%d\t%d\t%d\t%d\n", i, HT[i].data, HT[i].weight, HT[i].parent, HT[i].lchild, HT[i].rchild); //asciiΪ0��NUL�ַ�
            else if (HT[i].data == 13) printf("%d\tCR(%d)\t%d\t%d\t%d\t%d\n", i, HT[i].data, HT[i].weight, HT[i].parent, HT[i].lchild, HT[i].rchild); //asciiΪ13���س���CR
            else if (HT[i].data == 9) printf("%d\tTAB(%d)\t%d\t%d\t%d\t%d\n", i, HT[i].data, HT[i].weight, HT[i].parent, HT[i].lchild, HT[i].rchild); //asciiΪ9���Ʊ��TAB
            else printf("%d\t%c(%d)\t%d\t%d\t%d\t%d\n", i, HT[i].data, HT[i].data, HT[i].weight, HT[i].parent, HT[i].lchild, HT[i].rchild); //����
        }
        else printf("%d\ttrunk\t%d\t%d\t%d\t%d\n", i, HT[i].weight, HT[i].parent, HT[i].lchild, HT[i].rchild); //��Ҷ�ӽ�㣬��ʾ�����ɡ�trunk
    }
    return;
} //��ʾHuffman��

void gen_hufftree(list L, hufftree &HT) {
    struct node* p = L->next;
    int i, m = 2 * L->data - 1, s1, s2;	//mΪ���н��ĸ�������ֵ���ȿ������2��Ҷ�ӽ����-1��
    if (L->data <= 1) return; //ͷ����L->data���list��Ԫ�����������ֵС�ڵ���1����������Huffman��
    HT = new huffnode[m + 1]; //ǰ����Ҷ�ӣ������ŷ�Ҷ��
    for (i = 1; i <= m; ++i) {
        HT[i].weight = 0;
        HT[i].data = -1;
        HT[i].parent = 0;
        HT[i].lchild = 0;
        HT[i].rchild = 0;
    } //��������ʼ��
    for (i = 1; i <= L->data; ++i) {
        if (p) {
            HT[i].weight = p->weight;
            HT[i].data = p->data;
            p = p->next;
        }
    } //��Ҷ�ӽ�㸳Ȩֵ��ASCIIֵ����������˳��洢���У�
    for (i = L->data + 1; i <= m; ++i) { //�ӷ�Ҷ�ӽ�㿪ʼ����
        min_two(HT, i - 1, s1, s2); //�ҵ���ǰ��������Ȩ����С�Ľ�㣨�浽s1��s2��
        HT[s1].parent = i;
        HT[s2].parent = i; //����������С��Ҷ�ӽ�㹲ͬparent
        HT[i].lchild = s1;
        HT[i].rchild = s2; //�ø�parent�����Һ���Ϊ���������
        HT[i].weight = HT[s1].weight + HT[s2].weight; //Ȩ��Ϊ�������Ӻ�
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
            fprintf(out, "%d", HC[i].code[j]); //���Huffman���뼰���ַ���Ӧ���ļ�
        }
        printf("\n");
        fprintf(out, "\n");
    } //�����ʾHuffman����
    return;
}

void gen_huffcode(int size, hufftree HT, huffcode HC[]) {
    struct huffcode p;
    int i, j, m, n;
    if (HT != NULL) {
        for (i = 1; i <= size; i++) {
            p.start = size - 1; //��ʼ����λ��
            m = i; //mΪ�����λ��
            n = HT[m].parent; //nΪparent���λ��
            while (n != 0) { //�ý����parent���ʱ��ִ��ѭ��
                if (HT[n].lchild == m) p.code[p.start] = 0; //�������parent������������0
                else p.code[p.start] = 1; //�������parent������������1
                p.start--; //������ʼλ������
                m = n; //��������ƣ���ԭparent��㴦��
                n = HT[m].parent; //�¸�ѭ��
            }
            for (j = p.start + 1; j < size; j++) HC[i].code[j] = p.code[j];
            HC[i].start = p.start; //start��¼ÿ��Huffman������code�е���ʼλ��
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
                } //�ҵ����ַ���Ӧ�ı������
            }
        } //�������ַ��������ҵ����Ӧ������б���
        else {
            for (int i = 1; i <= n; i++) {
                if ('\0' == HC[i].data) {
                    for (int j = HC[i].start + 1; j < n; j++) fprintf(out, "%d", HC[i].code[j]);
                } //�ҵ����ַ���Ӧ�ı������
            }
        } //�����ַ�ת��ΪNUL��'\0'����Ӧ�ı���
        p = p->next;
    } //��������
    fclose(out);
    return;
} //������������¡�Huffman���뷽ʽ������Huffman����������������ļ�

void decode_huff(hufftree HT, int n, char in_file_name[], char out_file_name[]) {
    FILE* in, * out;
    int temp, m = n;
    if ((in = fopen(in_file_name, "r")) == NULL) { printf("can not open %s\n", in_file_name); return; } //�������ļ�
    if ((out = fopen(out_file_name, "w")) == NULL) { printf("can not open %s\n", out_file_name); return; } //������ļ�
    while (!feof(in)) {
        if (HT[m].lchild != 0 || HT[m].rchild != 0) {
            temp = (int)fgetc(in) - 48; //fgetc�õ���Ϊ�ַ�0��1����Ҫ��ȥ48������ֵ0��1
            if (temp == 0) {
                m = HT[m].lchild;
            } //ǰ�����ӽ��
            else if (temp == 1) {
                m = HT[m].rchild;
            } //ǰ���Һ��ӽ��
        } //��Ҷ�ӽ��ʱ����������
        else if (HT[m].lchild == 0 && HT[m].rchild == 0) {
            fprintf(out, "%c", HT[m].data);
            m = n;
        } //Ҷ�ӽ��
    }
    fclose(in);
    fclose(out);
    return;
} //��ȡ�����������������ݹ������������������룩���������ԭ����

list init_list() {
    list L = (list)malloc(sizeof(node));
    if (!L) { printf("overflow!\n"); exit(0); }
    L->weight = 0; //ͷ����ʼ��Ϊ0
    L->data = 0;
    L->next = NULL; //ָ����ָ��NULL
    return L;
}

void bubble_sort(list L) {
    struct node* pre, * p, * tail; //����βָ���Է��㶨λ
    tail = NULL;
    while ((L->next->next) != tail) {
        pre = L;
        p = L->next;
        while (p->next != tail) {
            if (p->weight > p->next->weight) { //��С��������
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
} //ð������

void print_list(list L) {
    struct node* pre;
    pre = L->next;
    while (pre) {
        if (pre->data == 10) printf("LF(%d)\t%d\n", pre->data, pre->weight); //asciiΪ10�����з�LF
        else if (pre->data == 0) printf("NUL(%d)\t%d\n", pre->data, pre->weight); //asciiΪ0��NUL�ַ�
        else if (pre->data == 13)printf("CR(%d)\t%d\n", pre->data, pre->weight); //asciiΪ13���س���CR
        else if (pre->data == 9)printf("TAB(%d)\t%d\n", pre->data, pre->weight); //asciiΪ9���Ʊ��TAB
        else printf("%c(%d)\t%d\n", pre->data, pre->data, pre->weight);
        pre = pre->next;
    }
    printf("there are %d kind of ascii characters in this list\n", L->data);
    return;
} //��ʾ�洢Ƶ���������Ա������

list array_to_list(int ascii[]) {
    struct node* p, * pre; int i, j = 0;
    list L = init_list(); //��ͷ��㣬��ʼ��
    pre = L;
    for (i = 0; i < ASCII_NUM; i++) {
        if (ascii[i] > 0) {
            p = (list)malloc(sizeof(node));
            if (!p) { printf("overflow!\n"); exit(0); }
            p->weight = (int)ascii[i]; //Ȩ�ؼ�ΪƵ��
            p->data = i; //������ż�Ϊ�ַ������asciiֵ����Ҫ��¼
            p->next = pre->next;
            pre->next = p; //������Ԫ��
            ++j; //�����ַ�����
        } //ֻͳ����������0���ַ�
    }
    L->data = j; //��¼�ַ�����
    bubble_sort(L); //���������ݽ���ð������
    return L;
} //�������ͳ�ƽ���������Ա�

void print_story(story P) {
    struct character* pre;
    pre = P->next;
    while (pre) {
        printf("%c", pre->ch);
        pre = pre->next;
    }
    return;
} //��ʾ�洢�����Ա������

void count_ascii(story P, int ascii[]) {
    int i; //i��¼asciiֵ
    for (i = 0; i < ASCII_NUM; i++) ascii[i] = 0; //��ʼ���ɾ�Ϊ0
    struct character* pre;
    pre = P->next;
    while (pre) {
        if (pre->ch >= 0 && pre->ch < ASCII_NUM) {
            //printf("%c", ch); //��ʾascii�ַ�
            ascii[(int)pre->ch]++;
        } //��ascii�ַ�������ַ�������1
        else {
            //printf("%c", 0); //��ascii�ַ���ʾΪNUL
            ascii[0]++;
        } //������ascii�ַ�������ַ���ΪNUL�ַ�����asciiֵΪ0
        pre = pre->next;
    }
    /*
    for (i = 0; i < ASCII_NUM; i++) if (ascii[i] != 0) {
        if (i == 10) printf("LF(%d)\t%d\n",i, ascii[i]); //asciiΪ10�����з�LF
        else if (i == 0) printf("NUL(%d)\t%d\n", i, ascii[i]); //asciiΪ0��NUL�ַ�
        else if (i == 13)printf("CR(%d)\t%d\n", i, ascii[i]); //asciiΪ13���س���CR
        else if (i == 9)printf("TAB(%d)\t%d\n", i, ascii[i]); //asciiΪ9���Ʊ��TAB
        else printf("%c(%d)\t%d\n", i, i, ascii[i]);
    } //������г��ֹ���ascii�ַ�
    printf("there are %d kind of ascii characters in %s\n\n", ascii_num, file_name);
    */
    return;
}

story read_txt(char file_name[]) {
    story P = (story)malloc(sizeof(character));
    if (!P) { printf("overflow!\n"); exit(0); } //�ռ俪��ʧ��
    P->next = NULL; //��ʼ��P����ͷ���
    character* p, * pre; int k = 0; //k��¼��ʼ״̬����λʱΪ0������λʱ����0
    FILE* fp;
    if ((fp = fopen(file_name, "r")) == NULL) { printf("can not open %s\n", file_name); exit(0); } //���ļ�
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
        } //��˳���������ݴ洢�����Ա�ṹ
        ++k; //��¼���ַ���
    }
    printf("\ntotal characters amount in %s is %d\n", file_name, k);
    //print_story(P); //��ʾ����
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
} //����ascii�ַ���asciiֵ����������Ʊ�ʾ����������ascii_bin[]�У��˴����鳤��Ϊ8

void code_ascii_bin (story P, char out_file_name[]) {
    int i, bin[8];
    struct character* p = P;
    FILE* out;
    if ((out = fopen(out_file_name, "w")) == NULL) { printf("can not open %s\n", out_file_name); return; } //������ļ�
    while (p) {
        memset(bin, 0, sizeof(bin));
        ascii_to_bin(p->ch, bin); //���ַ�ת��Ϊ8λ��������
        if (p->ch >= 0 && p->ch < ASCII_NUM) for (i = 0; i < 8; i++) fprintf_s(out, "%d", bin[i]); //��0-255�ڣ���ascii�ַ�����asciiת�����Ƶ�������ļ�
        else fprintf_s(out, "00000000"); //��ascii�ַ�����ΪNUL����������뼴Ϊ00000000
        p = p->next;
    } //���������ַ�
    fclose(out);
    return;
}

void decode_ascii_bin(char in_file_name[], char out_file_name[]) {
    int sum = 0, temp, k = 1;
    FILE* in, * out;
    if ((in = fopen(in_file_name, "r")) == NULL) { printf("can not open %s\n", in_file_name); return; } //�������ļ�
    if ((out = fopen(out_file_name, "w")) == NULL) { printf("can not open %s\n", out_file_name); return; } //������ļ�
    while (!feof(in)) {
        if (k <= 8) {
            temp = (int)fgetc(in) - 48; //fgetc�õ���Ϊ�ַ�0��1����Ҫ��ȥ48������ֵ0��1
            sum = (int)(sum + temp * (int)pow(2, 8 - k)); //8λ������ת10����
            ++k;
        } //��8���ַ�Ϊ��λ�������Ӻ͵õ�sum����ö��������Ӧ��asciiֵ
        else {
            k = 1;
            fputc(sum, out); //���
            sum = 0;
        } //��k����8ʱ���洢���sumֵ��asciiֵ����Ӧ�ַ�����������sumΪ0
    }
    fclose(in);
    fclose(out);
    return;
}

void char_compare(story P, char file2_name[]) {
    char ch; int different = 0;
    struct character* p = P;
    FILE* fp;
    if ((fp = fopen(file2_name, "r")) == NULL) { printf("can not open %s\n", file2_name); return; } //�������ļ�
    while (p) {
        ch = fgetc(fp);
        if (ch != p->ch) {
            if (ch != 0) {
                //printf("%c is different from %c  (%d)\n", ch, p->ch, p->ch); //��ĳλ���ַ���ͬ�����λ�õ����ļ������ַ�������ʾ
                ++different;
            } //NUL�ַ�ƥ�����еķ�ascii�룻ֻ���ַ���ͬʱ�ż�¼
        } //����ͬһλ�õ��ַ���ͬʱ��¼
        p = p->next;
    } //���ַ��Ƚ������ļ������߶�Ҫ��������ȫ
    fclose(fp);
    printf("\nthere are %d differences in %s compare to the original story\n", different, file2_name); //ͳ�Ʋ�ͬ�ַ���������Ƚϵ���ͬһ�ļ����롢����ǰ����죬�����ӦΪ0
    return;
}

int main() {
    int start_time, end_time;
    FILE* t;
    if ((t = fopen("TimeCompare.txt", "w")) == NULL) { printf("can not open TimeCompare.txt\n"); return 0; }
    char short_s[FILENAME_LEN] = "ShortStory.txt", long_s[FILENAME_LEN] = "LongStory.txt", short_ascii_code[FILENAME_LEN] = "ShortASCCode.txt", long_ascii_code[FILENAME_LEN] = "LongASCCode.txt", short_ascii_decode[FILENAME_LEN] = "ShortASCDecode.txt", long_ascii_decode[FILENAME_LEN] = "LongASCDecode.txt", short_huff[FILENAME_LEN] = "ShortHuf.txt", long_huff[FILENAME_LEN] = "LongHuf.txt", short_huff_code[FILENAME_LEN] = "ShortCode.txt", long_huff_code[FILENAME_LEN] = "LongCode.txt", short_huff_decode[FILENAME_LEN] = "ShortDecode.txt", long_huff_decode[FILENAME_LEN] = "LongDecode.txt";
    int asciiS[ASCII_NUM], asciiL[ASCII_NUM]; //�������ڽ��ܺ�������ĸ��ַ�Ƶ��
    hufftree htreeS, htreeL;
    huffcode hcodeS[256], hcodeL[256];
    printf("Zelin Li | 2020.06.29\n");

    printf("\nShort story processing:\n");
    story short_story = read_txt(short_s); //�����´��ļ���ȡ�����Ա�ṹ��
    count_ascii(short_story, asciiS); //�ַ�Ƶ��ͳ�ƽ����������
    list list_asciiS = array_to_list(asciiS); //���ַ�Ƶ��ͳ�ƽ��������Ǩ�Ƶ������Ա�������򣨴Ӵ�С��
    //print_list(list_asciiS); //��ʾ�ַ�Ƶ��ͳ�ƽ�������˽��Ҳ�����Ƶ���huffcode���У����Ժ����print_huffcodeҲչʾ�˴����ݣ�
    gen_hufftree(list_asciiS, htreeS); //�ɸ����Ա�����Huffman��
    print_hufftree(htreeS, 2 * list_asciiS->data - 1); //��ʾ���ɵ�Huffman����������
    gen_huffcode(list_asciiS->data , htreeS, hcodeS); //��Huffman������Huffman����
    print_huffcode(hcodeS, list_asciiS->data, short_huff); //��������ʾ�ַ�Ƶ��ͳ�ƽ����Huffman��������Huffman���뼰���Ӧ�ַ�������ļ�

    start_time = clock();
    code_huff(short_story, hcodeS, list_asciiS->data, short_huff_code); //��ԭ�Ľ���Huffman���룬���������ļ�
    end_time = clock();
    fprintf(t, "%s Huffman coding time:%d ms\n", short_s, end_time - start_time);

    start_time = clock();
    decode_huff(htreeS, 2 * list_asciiS->data - 1, short_huff_code, short_huff_decode); //��Huffman���������ʽ�������ݷ���Ϊ�����ַ����洢���ļ��У�ͨ��Huffman����
    end_time = clock();
    fprintf(t, "%s Huffman decoding time:%d ms\n", short_s, end_time - start_time);

    start_time = clock();
    code_ascii_bin(short_story, short_ascii_code); //�����Ա�ṹ����������Ϊascii���������ʽ�洢���ļ���
    end_time = clock();
    fprintf(t, "%s binary-ASCII coding time:%d ms\n", short_s, end_time - start_time);

    start_time = clock();
    decode_ascii_bin(short_ascii_code, short_ascii_decode); //��ascii���������ʽ�ļ����룬�õ��������������ݴ洢���ļ���
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

    char_compare(short_story, short_ascii_decode); //���ַ��ȽϽ�������º�ԭ���µ����𣬲���������ʾ
    char_compare(short_story, short_huff_decode);
    char_compare(long_story, long_ascii_decode);
    char_compare(long_story, long_huff_decode);

    fclose(t);
    system("pause");
    return 0;
}

/*
//��¼��δ����㷨����
node pop(list L) {
    struct node popnode; //�˷�ָ�룬����node�ṹ����¼������Ԫ��
    struct node* p; //��ʱָ��
    if (L->data <= 0) { //�ַ�����Ϊ0
        printf("no more types of character\n");
        popnode.weight = 0; //��Ԫ��ʱ����������һ��Ȩ��Ϊ0��popnode������û��Ԫ����
        return popnode;
    } //����Ԫ��
    popnode.data = L->next->data; //��¼��һ��Ԫ���ַ���Ϣ
    popnode.weight = L->next->weight; //��¼��һ��Ԫ�ص�Ȩ����Ϣ
    popnode.next = NULL;
    p = L->next;
    L->next = L->next->next; //Ԫ�����ƣ���Ϊ��һ��Ԫ�ص�������λ����Ҫ�����
    free(p); //�ͷ�ԭ��ͷ������Ԫ��ռ�ÿռ�
    L->data--; //����Ԫ�������ַ���������1
    return popnode;
} //���ơ���ջ��

hufftree create_hufftree(list L) {
    struct huffnode* hn, * lc, * rc;
    struct node pn1, pn2;
    int k = 0; //��¼״̬���Ƿ����״μ�¼Ҷ�ӣ��״���Ϊ0���������Ϊ1
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
            k = 1; //ˢ��״̬
        }
        else if (k == 1 && L->next->next != NULL && pn1.weight <= L->next->weight && pn1.weight <= L->next->next->weight) {

        }
    }
    return hn;
} //δ���
*/
