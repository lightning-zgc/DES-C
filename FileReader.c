//
// Created by Lightning on 2017/4/29.
//

#include "FileReader.h"

void ascToBinary(int character, int *ones, int index) {
    int p = 128;
    for (int i = 0; i < 8; ++i) {
        ones[i + index] = (character & p) ? 1 : 0;
        p /= 2;
    }
}

char BinaryToasc(int *ones, int index) {
    int re = 0, p = 128;
    for (int i = 0; i < 8; ++i) {
        re += ones[i + index] * p;
        p /= 2;
    }
    return re;
}

void readBinary(char *temp, int *s) {
    int k = 0;
    for (int i = 0; i < 64; i += 8) {
        ascToBinary(temp[k++], s, i);
    }
}

void writeBinary(int *temp, char *s) {
    int k = 0;
    for (int i = 0; i < 64; i += 8) {
        s[k++] = BinaryToasc(temp, i);
    }
}

int readFile64(FILE *fd, int *s) {
    unsigned char temp[64] = {'\0'};
    int c;
    for (int i = 0; i < 8; ++i) {
        c = fgetc(fd);
        if (c != EOF) {
            temp[i] = (char) c;
        } else {
            printf("\nEND:%d\n", c);
            if (i == 0)return 0;
            temp[i] = '\0';
        }
    }
    readBinary(temp, s);
    return 1;
}

void writeFile64(FILE *fd, int *s) {
    char t[64] = {'\0'};
    writeBinary(s, t);
    fwrite(t, 8, 1, fd);
}


FILE *readFilefromPATH(const void *PATH) {
    FILE *fd;
    if ((fd = fopen(PATH, "rb")) == NULL) {
        printf("文件读取错误，文件不存在或占用");
    }
    return fd;
}

FILE *writeFilefromPATH(const void *PATH) {
    FILE *fd;
    if ((fd = fopen(PATH, "wb+")) == NULL) {
        printf("文件读取错误，文件不存在或占用");
    }
    return fd;
}

void closeFile(FILE *fd) {
    fclose(fd);
}
