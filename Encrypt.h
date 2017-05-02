//
// Created by Lightning on 2017/5/1.
//

#ifndef DES2_0_ENCRYPT_H
#define DES2_0_ENCRYPT_H

#include "require.h"

//定义函数
void *DES(int *M, int *Key, int *C, int mode);

void Keygen(int *K);

void IP(int *M, int *L0, int *R0);

void FP(int *C, int *L16, int *R16);

void LtoR(int *Ri, int *Li_1, int *Ri_1, int *Ki);

void f(int *f_re, int *Ri_1, int *Ki);

void E(int *Ri_1, int *Ei);

void S(int *Si, int *Pi, int *Sbox, int index);

void P(int *Pi, int *f_re);

void InttoBarr(int dec, int *arr, int index, int bit);

void RotateKeyLeft(int *K, int index);

void PC2(int *C, int *D, int id);

void fIV(int *M, int *IV);

#endif //DES2_0_ENCRYPT_H
