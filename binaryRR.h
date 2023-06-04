#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Прототип функции по изменению битов в байте и последующей его записи в бинарный файл
void WriteBytes(char data[], FILE* fp);

// Определение этой функции с входными параметрами:
// char data[] - закодированная строка
// FILE* fp - указатель на открытый для записи бинарный файл
void WriteBytes(char data[], FILE* fp)
{
    // Изменяемый байт
    unsigned char byte;
    // Длина строки
    int size = (int)strlen(data);
    // Проверка на заполняемость всех байтов
    int flag = size % 8;
    // Счетчик по текущему элементу строки
    int len = 0;

    // Записываем в файл сколько байт заполнены полностью
    unsigned int control_size = size / 8;
    fwrite(&control_size, sizeof(unsigned int), 1, fp);
    // Изменяем каждый бит байта в соответствии с кодом Хаффмана и записываем в бинарный файл
    for (int k = 0; k < size / 8; k++) {
        byte = 0;
        for (int i = 0; i < 8; i++, len++)
        {
            byte <<= 1;

            byte |= (data[len] == '1') ? 1 : 0;
        }
        fwrite(&byte, 1, 1, fp);
    }

    // Если не все байты заполнены полностью, то для удобства последующего декодирования
    // Заполним последний байт так: 0...1 и дальше код
    if (flag) {
        byte = 0;
        for (int j = 0; j < 8 - flag; j++) {
            byte <<= 1;

            if (j == 8 - flag - 1) {
                byte |= 1;
            }
            else {
                byte |= 0;
            }
        }

        for (; len < size; len++) {
            byte <<= 1;
            byte |= (data[len] == '1') ? 1 : 0;
        }
        fwrite(&byte, 1, 1, fp);
    }
}



// Прототип функции по считыванию байтов из бинарного файла и определению значений каждого бита этого байта
char* GetBits(FILE* fp);

// Определение этой функции с входными параметрами:
// char* str - выделенный участок памяти размером REALLOC_SIZE + 1
// FILE* fp - указатель на открытый для чтений бинарный файл
char* GetBits(FILE* fp)
{
    //size для контроля размера файла
    //temp для последующего выделения дополнительной памяти
    size_t size = 0;
    char bits[9] = {'\0'}; // Массив для байта
    unsigned char byte;// Считываемый байт 
    // Считываем значение заполненных байтов
    unsigned int control_size;
    fread(&control_size, sizeof(unsigned int), 1, fp);
    // Определяем нужное количество памяти для выделения
    while (fread(&byte, 1, 1, fp) == 1) {
        size += 1;
    }

    size *= 8; // В байте 8 бит

    char* string = calloc(size + 1, sizeof(char));
    if (!string) {
        perror("char* string - Memory Allocation Error");
        printf("\n\t Press any key to exit: ");
        int aza = _getch();
        exit(EXIT_FAILURE);
    }

    fseek(fp, sizeof(unsigned int), SEEK_SET);

    // Считываем с бинарного файла 
    while (fread(&byte, 1, 1, fp) == 1)
    {
        for (int i = 0; i < 8; ++i)
        {
            bits[7 - i] = (byte & 1) ? '1' : '0';
            byte >>= 1;
        }
        strcat_s(string, size + 1, bits);
    }

    // Проверка заполненности последнего байта
    // Если незаполнен,то все биты,что записаны после первого значащего бита
    // в последнем байте, нужный нам код
    unsigned int length = (unsigned int)strlen(string);

    if (control_size != size) {
        unsigned int check = length - 8;
        while (string[check] != '1') {
            check++;
        }
        check += 1;
        unsigned int end = length - check;
        for (int i = length - 8; check < length; check++, i++) {
            string[i] = string[check];
        }
        string[length - 8 + end] = '\0';
    }

    return(string);
}
