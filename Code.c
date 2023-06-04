#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <conio.h>
#include "binaryRR.h"
#include <Windows.h>

#define MAX_SIZE 260 // ���������� ������ ������� 
#define BUFF_SIZE 100// ������ ������������ ������� ��� ������ ����� �������� 

// ��������� ��� ���� ��������� ������
struct node {
    unsigned char symbol; // ������ ������� �������� � ���� ����
    int frequency; // ������� �������
    struct node* left_child; // ��������� �� ��������� ����
    struct node* right_child;
};

// ��������� ��� ����� ��������
struct HuffmanCodes {
    unsigned char sym;
    char code[BUFF_SIZE];
};

// C�������� ��� ������� � �����������
typedef struct queue {
    struct node* elements[MAX_SIZE]; // ������ ��� ����� ������
    int size; // ������ �������
}PriorityQueue;

// ��������������� ��������� ��� ����������� � ���������� ������� ��������
struct symbol_freq {
    unsigned char symbol; // ������
    int frequency; // ��� �������
};

// �������� ������� �� �������� ������ ���� ������
struct node* create_node(char symbol, int frequency);

/*����������� ������� create_node() � �������� �����������:
char symbol - ������ ����
int frequency - ������� ����� �������
*/
struct node* create_node(char symbol, int frequency) {
    struct node* new_node = (struct node*)malloc(sizeof(struct node)); // ��������� ������ ��� ����� ����
    if (!new_node) { // �������� ��������� ������
        printf_s("\n ERROR\n Press any key to exit: ");
        int zaa = _getch();
        exit(EXIT_FAILURE);
    }
    // ���������� �����
    new_node->symbol = symbol;
    new_node->frequency = frequency;
    new_node->left_child = NULL;
    new_node->right_child = NULL;
    return new_node;
}

// �������� ������� �� ������ ����� � �������
void swap_in_queue(struct node** n1, struct node** n2);


// ����������� ���� ������� � �������� �����������:
// struct node* n1, struct node* n2 - ��������� �� ���� ������
void swap_in_queue(struct node** n1, struct node** n2) {
    struct node* temp = *n1;
    *n1 = *n2;
    *n2 = temp;
}

// �������� ������� �� ���������� ���� � �������
void enqueue(PriorityQueue* pq, struct node* new);


// ����������� ���� ������� � �������� �����������
// PriorityQueue* pq - ��������� �� �������
// struct node* new - ��������� �� ����
void enqueue(PriorityQueue* pq, struct node* new) {
    if (pq->size >= MAX_SIZE) {
        printf("Queue overflow!\n"); // �������� ������������ �������
        return;
    }

    // ���������� ����
    pq->elements[pq->size] = new;
    // ������������ ������ �������
    pq->size++;

    // ���������� ������� �� �������
    for (int i = 0; i < pq->size - 1; i++) {
        for (int j = 0; j < pq->size - i - 1; j++) {
            if (pq->elements[j]->frequency >= pq->elements[j + 1]->frequency) {
                // ������������ ��������� �������
                swap_in_queue(&pq->elements[j], &pq->elements[j + 1]);
            }
        }
    }
}

// �������� ������� �� ��������� �������� �� �������
struct node* dequeue(PriorityQueue* pq);

// ����������� �������
struct node* dequeue(PriorityQueue* pq) {
    // ������� ��������:
    // pq - ��������� ���� PriorityQueue* �� ������������ ������� 
    // ������������ ��������: 
    // ret - ��������� ���� struct node* �� ��������� ���� struct node, 
    if (pq->size == 0) {
        printf("Queue underflow!\n"); // �������� �� ������� �������
        struct node* empty = malloc(sizeof(struct node));
        if (!empty) {
            printf_s("\n\t ERROR!\n Press any key to exit");
            int baza = _getch();
            exit(EXIT_FAILURE);
        }
        empty->frequency = 0;
        empty->symbol = '\0';
        empty->left_child = empty->right_child = NULL;
        return (empty);
    }

    struct node* ret = pq->elements[0]; // ������������ ��������� ��������� � ����� ����
    pq->size--;
    pq->elements[0] = pq->elements[pq->size]; // ���������� ��������� ������� � ������
    pq->elements[pq->size] = NULL;

    // ��������� ������� �� �������
    for (int i = 1; i < pq->size; i++) {
        for (int j = i; j > 0 && pq->elements[j - 1]->frequency > pq->elements[j]->frequency; j--) {
            swap_in_queue(&pq->elements[j - 1], &pq->elements[j]);
        }
    }


    return ret;
}

// �������� ������� �� ���������� ������ ��������
void huffman_tree(struct node** root, struct symbol_freq* symbols, int n, PriorityQueue* ptr);

//����������� ������� �� ���������� ������ ��������
void huffman_tree(struct node** root, struct symbol_freq* symbols, int n, PriorityQueue* ptr) {
    // ������� ��������:
//root - ��������� �� ��������� ���� struct node** �� ������ ������ (������ ���� �������)
//symbols - ��������� ���� struct symbol_freq* �� ������ �������� struct symbol_freq
//n - ���������� ��������, ���� int
// pq - ��������� ���� PriorityQueue* �� ������������ �������
    if (n == 1) { // ���� ������ ����, �� ������ � ���� ��� ������ 
        *root = create_node(symbols[0].symbol, symbols[0].frequency);
        return;
    }

    for (int i = 0; i < n; i++) { // ������� � ��������� ���� � �������
        struct node* temp = create_node(symbols[i].symbol, symbols[i].frequency);
        enqueue(ptr, temp);
    }

    while (ptr->size > 0) {
        // ������� ��� ���� � ���������� �����������, �� ���� � ���������� �������� �������
        // ����� ���� ���������� � ���� ����, ������� ���� ��������� � �������
        struct node* num1 = dequeue(ptr);
        struct node* num2 = dequeue(ptr);

        struct node* new_host = create_node('\0', num1->frequency + num2->frequency);
        if (num1->frequency >= num2->frequency) {
            new_host->left_child = num2;
            new_host->right_child = num1;
        }
        else {
            new_host->left_child = num1;
            new_host->right_child = num2;
        }

        if (ptr->size) {
            enqueue(ptr, new_host);
        }
        else {
            *root = new_host;
        }
    }

}

// �������� ������� ���������� �����
void print_huffman_codes(struct node* root, char* code, int top, struct HuffmanCodes* ptr, int* i);

// ����������� �������
void print_huffman_codes(struct node* root, char* code, int top, struct HuffmanCodes* ptr, int* i) {
    // ������� ��������:
// root - ��������� ���� struct node* �� ������ ������ ��������
// code - ������ ���� char, � ������� ������������ ����������� ����
// top - ������� ���� int
// ptr - ��������� ���� struct HuffmanCodes* �� ������ �������� struct HuffmanCodes, � ������� ����� �������� ����
// i - ��������� ���� int* �� �������, � ������� �������� �� ����� ������������ ������ ����� � �������

    if (root->left_child) {
        code[top] = '0';
        print_huffman_codes(root->left_child, code, top + 1, ptr, i);
    }
    if (root->right_child) {
        code[top] = '1';
        print_huffman_codes(root->right_child, code, top + 1, ptr, i);
    }
    if (root->symbol != '\0') {
        printf(" Symbol: %c, Code: %s\n", root->symbol, code);
        ptr[*i].sym = root->symbol;
        strcpy_s(ptr[*i].code, 100, code);
        (*i)++;
    }
    else {
        code[top] = '\0';
    }
}

// �������� ������� ��� ����������� ������� ��������
void get_symbols_freq(char* str, struct symbol_freq* symbols, int* n);

// ����������� ������� ��� ����������� ������� ��������
void get_symbols_freq(char* str, struct symbol_freq* symbols, int* n) {
    // ������� ��������:
// str - ������ ���� char*, �� ������� ������������ ��������� �������
// symbols - ��������� ���� struct symbol_freq* �� ������ �������� struct symbol_freq, 
// � ������� ����� ������������ ���� ������� � �� �������
// n - ��������� ���� int* �� �������, ������������ ���������� ��������  
    int freq[256] = { 0 };
    int i = 0;
    char ch;

    // ���������� ������� ��� ����������� ������ ��� ������ ������� �� 256 ���������
    while (str[i] != '\0') {
        ch = str[i];
        freq[ch]++;
        i++;
    }

    for (i = 0; i < 256; i++) {
        if (freq[i] > 0) {
            symbols[*n].frequency = freq[i];
            symbols[*n].symbol = i;
            (*n)++;
        }
    }

}

// �������� ������� �� ����������� ������
char* encode_string(char* str, struct HuffmanCodes* table, int table_size);

// ����������� ������� 
char* encode_string(char* str, struct HuffmanCodes* table, int table_size) {
    // ������� ��������:
// str - ��������� ���� char* �� �������� ������
// table - ��������� ���� struct HuffmanCodes* �� ������� � ������
// table_size - ������ ������� ���� int
    // ������������ ��������:
// encoded - ��������� ���� char* �� �������������� ������
    int i, j;
    size_t length = strlen(str);
    char* encoded = (char*)calloc(length * 8 + 1, sizeof(char)); // ��������� ������ ��� �������������� ������
    if (!encoded) {
        printf_s("\n\t ERROR!\n Press any key to exit: ");
        int aza = _getch();
        exit(EXIT_FAILURE);
    }
    encoded[0] = '\0';
    for (i = 0; i < length; i++) {
        // ����� ������� � ������� ��������
        for (j = 0; j < table_size; j++) {
            if (str[i] == table[j].sym) {
                strcat_s(encoded, length * 8 + 1, table[j].code); // ���������� ���� ������� � �������������� ������
            }
        }
    }

    return (encoded);
}


// �������� ������� �� ������ ������
void compress_data(char* string, struct node** root, struct HuffmanCodes* codes);

// ����������� �������
void compress_data(char* string, struct node** root, struct HuffmanCodes* codes) {
    // ������� ��������:
// string - ��������� ���� char* �� ������, ������� ����� ��������� 
// codes - ��������� ���� struct HuffmanCodes* �� ������ �������� struct HuffmanCodes � ������ ��������
// root - ��������� ���� struct node**  �� ��������� �� ������ ������ ��������
    struct symbol_freq frequency[100] = { 0 }; // ������� ������ �������� ��� ������ �������� � �� �������
    int n = 0; // �������������� �������
    PriorityQueue ptr = { 0 };
    get_symbols_freq(string, &frequency[0], &n); // ���������� ������� ��������


    huffman_tree(root, &frequency[0], n, &ptr); // ������ ������

    // �������� ������ ��� ������� � ������ � ���������
    codes = (struct HuffmanCodes*)malloc(sizeof(struct HuffmanCodes) * n);
    if (!codes) {
        printf_s("\n\t ERROR!\n Press any key to exit: ");
        int aza = _getch();
        exit(EXIT_FAILURE);
    }

    char code[100] = { '\0' };
    int i = 0;

    printf_s("\n Huffman codes: \n\n");
    // ������������� � ���������� ���� � ������� struct HuffmanCodes
    print_huffman_codes(*root, code, 0, &codes[0], &i); 

    // �������� ������� �� ����
    char* encoded = encode_string(string, &codes[0], n);

    // ��������� � ��������� �������� ���� ��� ������
    FILE* f;
    fopen_s(&f, "file.bin", "wb");
    if (f == NULL)
    {
        perror("");
        exit(EXIT_FAILURE);
    }

    // ���������� ���������� ������������������ ����� � �������� ����
    WriteBytes(encoded, f);
    

    fclose(f);
    // ����������� ������
    free(string);
    free(encoded);
}


// �������� ������� �� ����������� ����
void decode(char* code, struct node* ptr, struct node* root, FILE* stream);

// ����������� �������
void decode(char* code, struct node* ptr, struct node* root, FILE* stream) {
    // ������� ��������:
// code - ��������� ���� char* �� ������ � ������������������� �����
// root - ��������� ���� struct node* �� ������ ������ ��������
// ptr - ��������� ���� struct node*, ������� ��� �������� � ������� 
// ���� ��������� �� ������ ������, �� �� ����� ���������� �������
// � ��� ������� ����� �������������� ����������� �� ������
// stream - ��������� ���� FILE �� �������� ��� ������ ����
    int i = 0;
    while (code[i] != '\0') {
        if (code[i] == '0') {
            ptr = ptr->left_child;
        }
        if (code[i] == '1') {
            ptr = ptr->right_child;
        }
        if (ptr->symbol != '\0') {
            fprintf(stream, "%c", ptr->symbol);
            ptr = root;
        }
        i++;
    }

   
}

// �������� ������� �� ���������� ������ � ����� � ����������� �������������� ����
void decompress_data(struct node* root);

// ����������� ���� ������� � �������� �����������: char* str - ��������� ���� char
void decompress_data(struct node* root) {
    // ������� ��������:
    // root - ��������� ���� struct node* �� ������ ������
    // ��������� �������� ���� � �������������� ������� � ���������
    FILE* f;
    fopen_s(&f, "file.bin", "rb");

    if (f == NULL)
    {
        perror("file.bin");
        exit(EXIT_FAILURE);
    }

    // �������� ���
    char* str = GetBits(f); 

    // ��������������� � ���������� � ����
    FILE* fp;
    fopen_s(&fp, "result.txt", "w");
    if (fp == NULL)
    {
        perror("file.bin");
        exit(EXIT_FAILURE);
    }
    struct node* ptr = root;
    decode(str, ptr, root, fp);
    printf_s("\n The result of decoding was recorded to txt file named \"result.txt\" \n\n");

    free(str);
    fclose(f);
    fclose(fp);
}

// �������� ���������� ������� �� �������� ������
void freeTree(struct node* root);

// ����������� �������
void freeTree(struct node* root) {
    // ������� ��������:
    // root - ��������� ���� struct node* �� ������ ������
    if (root == NULL) {
        return;
    }
    freeTree(root->left_child);
    freeTree(root->right_child);
    free(root);
}

// �������� ������� �� ���������� ������, ������� �� ����� ����������
char* FileReading(char* name);

// ����������� ���� ������� � ������� ����������: char* name - ��� �����
char* FileReading(char* name) {
    // ������� ��������:
    // name - ��������� ���� char* �� ������ � ������ �����
    // ������������
    FILE* fp;
    // �������� ����� � ���������
    fopen_s(&fp, name, "r");
    if (!fp) {
        perror(name);
        printf_s("\n\t Press any key to exit: ");
        int baza = _getch();
        exit(EXIT_FAILURE);
    }

    size_t size = 0;
    char buffer[260]; // ������ ��� ����������
    // ���������� ���������� �������� � �����
    while (fgets(buffer, 260, fp) != NULL) {
        size += strlen(buffer);
    }

    // �������� ������ ��� ���� �����
    char* ans_string = (char*)calloc(size + 1, sizeof(char));
    if (!ans_string) {
        perror(" ans_string - Memory Allocation Error");
        printf_s("\n\t Press any key to exit: ");
        int aza = _getch();
        exit(EXIT_FAILURE);
    }
    ans_string[size] = '\0';
    
    // ������������� ���������� ��������� ������� � ������ 
    fseek(fp, 0L, SEEK_SET);

    // ���������� ���� ����� � ������� ���������� ������
    while (fgets(buffer, 260, fp) != NULL) {
        strcat_s(ans_string, size + 1, buffer);
    }

    return(ans_string);
}

wchar_t name[] = L"����� ����. ������ ������ �� ��������� ��������";

int main(void) {
    SetConsoleTitle(name);
    char* string = FileReading("test_file.txt");
    printf_s("\n Scanned text:\n\n%s\n", string);
    struct node* root = NULL;// �������������� ���������� ��������� �� ������
    struct HuffmanCodes* codes = NULL;// �������������� ��������� �� ������� � ������ ��������
    compress_data(string, &root, codes);

    printf_s("\n");
    decompress_data(root);

    // ������������ ������
    free(codes);
    freeTree(root);
    printf_s("\n Press any key to exit: ");
    int baza = _getch();
    return 0;
}

