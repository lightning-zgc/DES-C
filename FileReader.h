//
// Created by Lightning on 2017/5/1.
//

#ifndef DES2_0_FILEREADER_H
#define DES2_0_FILEREADER_H

#include "require.h"

void ascToBinary(int character, int *ones, int index);

char BinaryToasc(int *ones, int index);

void readBinary(char *temp, int *s);

void writeBinary(int *temp, char *s);

int readFile64(FILE *fd, int *s);

void writeFile64(FILE *fd, int *s);

FILE *readFilefromPATH(const void *PATH);

FILE *writeFilefromPATH(const void *PATH);

void closeFile(FILE *fd);

#endif //DES2_0_FILEREADER_H
