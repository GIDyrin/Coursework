#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// �������� ������� �� ��������� ����� � ����� � ����������� ��� ������ � �������� ����
void WriteBytes(char data[], FILE* fp);

// ����������� ���� ������� � �������� �����������:
// char data[] - �������������� ������
// FILE* fp - ��������� �� �������� ��� ������ �������� ����
void WriteBytes(char data[], FILE* fp)
{
    // ���������� ����
    unsigned char byte;
    // ����� ������
    int size = (int)strlen(data);
    // �������� �� ������������� ���� ������
    int flag = size % 8;
    // ������� �� �������� �������� ������
    int len = 0;

    // ���������� � ���� ������� ���� ��������� ���������
    unsigned int control_size = size / 8;
    fwrite(&control_size, sizeof(unsigned int), 1, fp);
    // �������� ������ ��� ����� � ������������ � ����� �������� � ���������� � �������� ����
    for (int k = 0; k < size / 8; k++) {
        byte = 0;
        for (int i = 0; i < 8; i++, len++)
        {
            byte <<= 1;

            byte |= (data[len] == '1') ? 1 : 0;
        }
        fwrite(&byte, 1, 1, fp);
    }

    // ���� �� ��� ����� ��������� ���������, �� ��� �������� ������������ �������������
    // �������� ��������� ���� ���: 0...1 � ������ ���
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



// �������� ������� �� ���������� ������ �� ��������� ����� � ����������� �������� ������� ���� ����� �����
char* GetBits(FILE* fp);

// ����������� ���� ������� � �������� �����������:
// char* str - ���������� ������� ������ �������� REALLOC_SIZE + 1
// FILE* fp - ��������� �� �������� ��� ������ �������� ����
char* GetBits(FILE* fp)
{
    //size ��� �������� ������� �����
    //temp ��� ������������ ��������� �������������� ������
    size_t size = 0;
    char bits[9] = {'\0'}; // ������ ��� �����
    unsigned char byte;// ����������� ���� 
    // ��������� �������� ����������� ������
    unsigned int control_size;
    fread(&control_size, sizeof(unsigned int), 1, fp);
    // ���������� ������ ���������� ������ ��� ���������
    while (fread(&byte, 1, 1, fp) == 1) {
        size += 1;
    }

    size *= 8; // � ����� 8 ���

    char* string = calloc(size + 1, sizeof(char));
    if (!string) {
        perror("char* string - Memory Allocation Error");
        printf("\n\t Press any key to exit: ");
        int aza = _getch();
        exit(EXIT_FAILURE);
    }

    fseek(fp, sizeof(unsigned int), SEEK_SET);

    // ��������� � ��������� ����� 
    while (fread(&byte, 1, 1, fp) == 1)
    {
        for (int i = 0; i < 8; ++i)
        {
            bits[7 - i] = (byte & 1) ? '1' : '0';
            byte >>= 1;
        }
        strcat_s(string, size + 1, bits);
    }

    // �������� ������������� ���������� �����
    // ���� ����������,�� ��� ����,��� �������� ����� ������� ��������� ����
    // � ��������� �����, ������ ��� ���
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
