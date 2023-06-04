#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <conio.h>
#include "binaryRR.h"
#include <Windows.h>

#define MAX_SIZE 260 // допустимый размер очереди 
#define BUFF_SIZE 100// размер статического буффера для записи кодов Хаффмана 

// Структура для узла бинарного дерева
struct node {
    unsigned char symbol; // Символ который хранится в этом узле
    int frequency; // Частота символа
    struct node* left_child; // Указатели на следующие узлы
    struct node* right_child;
};

// Структура для кодов Хаффмана
struct HuffmanCodes {
    unsigned char sym;
    char code[BUFF_SIZE];
};

// Cтруктура для очереди с приоритетом
typedef struct queue {
    struct node* elements[MAX_SIZE]; // массив для узлов дерева
    int size; // Размер очереди
}PriorityQueue;

// Вспомогательная структура для определения и сортировки частоты символов
struct symbol_freq {
    unsigned char symbol; // символ
    int frequency; // его частота
};

// Прототип функции по созданию нового узла дерева
struct node* create_node(char symbol, int frequency);

/*Определение функции create_node() с входными параметрами:
char symbol - Символ узла
int frequency - частота этого символа
*/
struct node* create_node(char symbol, int frequency) {
    struct node* new_node = (struct node*)malloc(sizeof(struct node)); // Выделение памяти под новый узел
    if (!new_node) { // Проверка выделения памяти
        printf_s("\n ERROR\n Press any key to exit: ");
        int zaa = _getch();
        exit(EXIT_FAILURE);
    }
    // Заполнение полей
    new_node->symbol = symbol;
    new_node->frequency = frequency;
    new_node->left_child = NULL;
    new_node->right_child = NULL;
    return new_node;
}

// Прототип функции по обмену узлов в очереди
void swap_in_queue(struct node** n1, struct node** n2);


// Определение этой функции с входными параметрами:
// struct node* n1, struct node* n2 - указатели на узлы дерева
void swap_in_queue(struct node** n1, struct node** n2) {
    struct node* temp = *n1;
    *n1 = *n2;
    *n2 = temp;
}

// Прототип функции по добавлению узла в очередь
void enqueue(PriorityQueue* pq, struct node* new);


// Определение этой функции с входными параметрами
// PriorityQueue* pq - указатель на очередь
// struct node* new - указатель на узел
void enqueue(PriorityQueue* pq, struct node* new) {
    if (pq->size >= MAX_SIZE) {
        printf("Queue overflow!\n"); // Проверка переполнения очереди
        return;
    }

    // Добавление узла
    pq->elements[pq->size] = new;
    // Увеличивнеие размер очереди
    pq->size++;

    // Сортировка очереди по частоте
    for (int i = 0; i < pq->size - 1; i++) {
        for (int j = 0; j < pq->size - i - 1; j++) {
            if (pq->elements[j]->frequency >= pq->elements[j + 1]->frequency) {
                // Перестановка элементов местами
                swap_in_queue(&pq->elements[j], &pq->elements[j + 1]);
            }
        }
    }
}

// Прототип функции по выделению элемента из очереди
struct node* dequeue(PriorityQueue* pq);

// Определение функции
struct node* dequeue(PriorityQueue* pq) {
    // Входные параметы:
    // pq - указатель типа PriorityQueue* на приоритетную очередь 
    // Возвращаемое значение: 
    // ret - указатель типа struct node* на созданный узел struct node, 
    if (pq->size == 0) {
        printf("Queue underflow!\n"); // Проверка на пустоту очереди
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

    struct node* ret = pq->elements[0]; // Максимальный приоритет находится в корне кучи
    pq->size--;
    pq->elements[0] = pq->elements[pq->size]; // Перемещаем последний элемент в корень
    pq->elements[pq->size] = NULL;

    // Сортируем очередь по частоте
    for (int i = 1; i < pq->size; i++) {
        for (int j = i; j > 0 && pq->elements[j - 1]->frequency > pq->elements[j]->frequency; j--) {
            swap_in_queue(&pq->elements[j - 1], &pq->elements[j]);
        }
    }


    return ret;
}

// Прототип функции по построению дерева Хаффмана
void huffman_tree(struct node** root, struct symbol_freq* symbols, int n, PriorityQueue* ptr);

//Определение функции по построению дерева Хаффмана
void huffman_tree(struct node** root, struct symbol_freq* symbols, int n, PriorityQueue* ptr) {
    // Входные параметы:
//root - указатель на указатель типа struct node** на корень дерева (Должен быть занулен)
//symbols - указатель типа struct symbol_freq* на массив структур struct symbol_freq
//n - количество символов, типа int
// pq - указатель типа PriorityQueue* на приоритетную очередь
    if (n == 1) { // Если символ один, то корень и есть все дерево 
        *root = create_node(symbols[0].symbol, symbols[0].frequency);
        return;
    }

    for (int i = 0; i < n; i++) { // Создаем и добавляем узлы в очередь
        struct node* temp = create_node(symbols[i].symbol, symbols[i].frequency);
        enqueue(ptr, temp);
    }

    while (ptr->size > 0) {
        // Достаем два узла с наибольшим приоритетом, то есть с наименьшей частотой символа
        // После чего объединяем в один узел, который тоже добавляем в очередь
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

// Прототип функции распечатки кодов
void print_huffman_codes(struct node* root, char* code, int top, struct HuffmanCodes* ptr, int* i);

// Определение функции
void print_huffman_codes(struct node* root, char* code, int top, struct HuffmanCodes* ptr, int* i) {
    // Входные параметы:
// root - указатель типа struct node* на корень дерева Хаффмана
// code - буффер типа char, в который записываются накопленные биты
// top - счетчик типа int
// ptr - указатель типа struct HuffmanCodes* на массив структур struct HuffmanCodes, в которой будут хранится коды
// i - указатель типа int* на счетчик, с помощью которого мы будем осуществлять запись кодов в таблицу

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

// Прототип функции для определения частоты символов
void get_symbols_freq(char* str, struct symbol_freq* symbols, int* n);

// Определение функции для определения частоты символов
void get_symbols_freq(char* str, struct symbol_freq* symbols, int* n) {
    // Входные параметы:
// str - строка типа char*, по которой составляется частотная таблица
// symbols - указатель типа struct symbol_freq* на массив структур struct symbol_freq, 
// в которые будут записываться сами символы и их частота
// n - указатель типа int* на счетчик, определяющих количество символов  
    int freq[256] = { 0 };
    int i = 0;
    char ch;

    // Определяем сколько раз повторяется символ при помощи массива на 256 элементов
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

// Прототип функции по кодированию строки
char* encode_string(char* str, struct HuffmanCodes* table, int table_size);

// Определение функции 
char* encode_string(char* str, struct HuffmanCodes* table, int table_size) {
    // Входные параметы:
// str - указатель типа char* на исходную строку
// table - указатель типа struct HuffmanCodes* на таблицу с кодами
// table_size - размер таблицы типа int
    // Возвращаемое значение:
// encoded - указатель типа char* на закодированную строку
    int i, j;
    size_t length = strlen(str);
    char* encoded = (char*)calloc(length * 8 + 1, sizeof(char)); // выделение памяти для закодированной строки
    if (!encoded) {
        printf_s("\n\t ERROR!\n Press any key to exit: ");
        int aza = _getch();
        exit(EXIT_FAILURE);
    }
    encoded[0] = '\0';
    for (i = 0; i < length; i++) {
        // поиск символа в таблице Хаффмана
        for (j = 0; j < table_size; j++) {
            if (str[i] == table[j].sym) {
                strcat_s(encoded, length * 8 + 1, table[j].code); // добавление кода символа в закодированную строку
            }
        }
    }

    return (encoded);
}


// Прототип функции по сжатию данных
void compress_data(char* string, struct node** root, struct HuffmanCodes* codes);

// Определение функции
void compress_data(char* string, struct node** root, struct HuffmanCodes* codes) {
    // Входные параметы:
// string - указатель типа char* на строку, которую будет сжиматься 
// codes - указатель типа struct HuffmanCodes* на массив структур struct HuffmanCodes с кодами Хаффмана
// root - указатель типа struct node**  на указатель на корень дерева Хаффмана
    struct symbol_freq frequency[100] = { 0 }; // Создаем массив структур для записи символов и их частоты
    int n = 0; // Инициализируем счетчик
    PriorityQueue ptr = { 0 };
    get_symbols_freq(string, &frequency[0], &n); // Определяем частоты символов


    huffman_tree(root, &frequency[0], n, &ptr); // Строим дерево

    // Выделяем память под таблицу с кодами с проверкой
    codes = (struct HuffmanCodes*)malloc(sizeof(struct HuffmanCodes) * n);
    if (!codes) {
        printf_s("\n\t ERROR!\n Press any key to exit: ");
        int aza = _getch();
        exit(EXIT_FAILURE);
    }

    char code[100] = { '\0' };
    int i = 0;

    printf_s("\n Huffman codes: \n\n");
    // Распечатываем и записываем коды в таблицу struct HuffmanCodes
    print_huffman_codes(*root, code, 0, &codes[0], &i); 

    // Заменяем символы на коды
    char* encoded = encode_string(string, &codes[0], n);

    // Открываем с проверкой бинарный файл для записи
    FILE* f;
    fopen_s(&f, "file.bin", "wb");
    if (f == NULL)
    {
        perror("");
        exit(EXIT_FAILURE);
    }

    // Записываем полученную последовательность битов в бинарный файл
    WriteBytes(encoded, f);
    

    fclose(f);
    // Освобождаем память
    free(string);
    free(encoded);
}


// Прототип функции по расшифровке кода
void decode(char* code, struct node* ptr, struct node* root, FILE* stream);

// Определение функции
void decode(char* code, struct node* ptr, struct node* root, FILE* stream) {
    // Входные параметы:
// code - указатель типа char* на строку с последовательностью битов
// root - указатель типа struct node* на корень дерева Хаффмана
// ptr - указатель типа struct node*, который при передаче в функцию 
// тоже указывает на корень дерева, но во время выполнения функции
// с его помощью будет осуществляться перемещение по дереву
// stream - указатель типа FILE на открытый для записи файл
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

// Прототип функции по считыванию данных с файла и составлению зашифрованного кода
void decompress_data(struct node* root);

// Определение этой функции с входными параметрами: char* str - указатель типа char
void decompress_data(struct node* root) {
    // Входные параметы:
    // root - указатель типа struct node* на корень дерева
    // Открываем бинарный файл с закодированным текстом с проверкой
    FILE* f;
    fopen_s(&f, "file.bin", "rb");

    if (f == NULL)
    {
        perror("file.bin");
        exit(EXIT_FAILURE);
    }

    // Получаем код
    char* str = GetBits(f); 

    // Рассшифровываем и записываем в файл
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

// Прототип рекурсиной функции по удалению дерева
void freeTree(struct node* root);

// Определение функции
void freeTree(struct node* root) {
    // Входные параметы:
    // root - указатель типа struct node* на корень дерева
    if (root == NULL) {
        return;
    }
    freeTree(root->left_child);
    freeTree(root->right_child);
    free(root);
}

// Прототип функции по считыванию текста, который мы будем кодировать
char* FileReading(char* name);

// Определение этой функции с входным параметром: char* name - имя файла
char* FileReading(char* name) {
    // Входные параметы:
    // name - указатель типа char* на строку с именем файла
    // Возвращаемое
    FILE* fp;
    // Открытие файла с проверкой
    fopen_s(&fp, name, "r");
    if (!fp) {
        perror(name);
        printf_s("\n\t Press any key to exit: ");
        int baza = _getch();
        exit(EXIT_FAILURE);
    }

    size_t size = 0;
    char buffer[260]; // Буффер для считывания
    // Определяем количество символов в файле
    while (fgets(buffer, 260, fp) != NULL) {
        size += strlen(buffer);
    }

    // Выделяем память под весь текст
    char* ans_string = (char*)calloc(size + 1, sizeof(char));
    if (!ans_string) {
        perror(" ans_string - Memory Allocation Error");
        printf_s("\n\t Press any key to exit: ");
        int aza = _getch();
        exit(EXIT_FAILURE);
    }
    ans_string[size] = '\0';
    
    // Устанавливаем внутренний указатель позиции в начало 
    fseek(fp, 0L, SEEK_SET);

    // Записываем весь текст в заранее выделенный массив
    while (fgets(buffer, 260, fp) != NULL) {
        strcat_s(ans_string, size + 1, buffer);
    }

    return(ans_string);
}

wchar_t name[] = L"Дырин Глеб. Сжатие данных по алгоритму Хаффмана";

int main(void) {
    SetConsoleTitle(name);
    char* string = FileReading("test_file.txt");
    printf_s("\n Scanned text:\n\n%s\n", string);
    struct node* root = NULL;// Инициализируем зануленный указатель на корень
    struct HuffmanCodes* codes = NULL;// Инициализируем указатель на таблицу с кодами Хаффмана
    compress_data(string, &root, codes);

    printf_s("\n");
    decompress_data(root);

    // Освобождение памяти
    free(codes);
    freeTree(root);
    printf_s("\n Press any key to exit: ");
    int baza = _getch();
    return 0;
}

