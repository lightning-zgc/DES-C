//
// Created by Lightning on 2017/4/25.
//

#include "DES_Resourse.h"
#include "Encrypt.h"

int Keyi[16][48];

void printA(int *in, int len) { //测试打印输出
    for (int i = 0; i < len; ++i) {
        printf("%d", in[i]);
    }
    printf("\n");
}

void *DES(int *M, int *Key, int *C, int mode) { //M为64位明文（密文），Key为64位密钥，bool为模式，0为加密、1为解密
    int L[2][32] = {0}, R[2][32] = {0}; //L为左数据块，R为右数据块，使用二维数组进行循环计算优化空间
    memset(C, 0, sizeof(C));
    printA(Key, 64);
    //密钥生成
    Keygen(Key);
    //IP置换
    printA(M, 64);
    IP(M, L[0], R[0]);
    printA(L[0], 32);
    printA(R[0], 32);
    switch (mode) {
        case 0: //加密
            for (int i = 1; i <= 16; ++i) {
                memcpy((void *) L[i % 2], (void *) R[(i + 1) % 2], sizeof(R[(i + 1) % 2])); //Li=R(i-1)
                LtoR(R[i % 2], L[(i + 1) % 2], R[(i + 1) % 2], Keyi[i - 1]);
            }
            break;
        case 1: //解密
            for (int i = 1; i <= 16; ++i) {
                memcpy((void *) L[i % 2], (void *) R[(i + 1) % 2], sizeof(R[(i + 1) % 2])); //Li=R(i-1)
                LtoR(R[i % 2], L[(i + 1) % 2], R[(i + 1) % 2], Keyi[16 - i]); //解密时，K使用顺序反向
            }
            break;
        default:
            break;
    }
    printA(L[0], 32);
    printA(R[0], 32);
    FP(C, L[0], R[0]);
    printA(C, 64);
}

void Keygen(int *K) {
    int C[28], D[28];
    for (int i = 0; i < 56; ++i) {
        if (i < 28) {
            C[i] = K[table_PC1[i] - 1];
        } else {
            D[i - 28] = K[table_PC1[i] - 1];
        }
    }
    for (int i = 0; i < 16; ++i) {
        RotateKeyLeft(C, table_LS[i]);
        RotateKeyLeft(D, table_LS[i]);
        PC2(C, D, i);
    }
    printA(Keyi[0], 56);
}

void IP(int *M, int *L0, int *R0) { //IP置换，输入64位明文块输出2个32位的数组
    for (int i = 0; i < 32; ++i) {
        L0[i] = M[table_IP[i] - 1];
    }
    for (int i = 32; i < 64; ++i) {
        R0[i - 32] = M[table_IP[i] - 1];
    }
}

void FP(int *C, int *L16, int *R16) { //FP置换，输入2个32位的数组，输出1个64位密文块
    int index;
    for (int i = 0; i < 64; ++i) {
        index = table_arcIP[i];
        if (index <= 32) {
            C[i] = R16[index - 1];
        } else {
            C[i] = L16[index - 33];
        }
    }
}

void LtoR(int *Ri, int *Li_1, int *Ri_1, int *Ki) {
    int f_re[32];
    f(f_re, Ri_1, Ki);
    for (int i = 0; i < 32; ++i) {
        Ri[i] = Li_1[i] ^ f_re[i];
    }
}

void f(int *f_re, int *Ri_1, int *Ki) {
    int Ei[48], Si[48], Pi[32], *Sbox;
    E(Ri_1, Ei);
    for (int i = 0; i < 48; ++i) {
        Si[i] = Ei[i] ^ Ki[i];
    }
    for (int i = 0; i < 8; ++i) {
        switch (i) {
            case 0:
                Sbox = S1;
                break;
            case 1:
                Sbox = S2;
                break;
            case 2:
                Sbox = S3;
                break;
            case 3:
                Sbox = S4;
                break;
            case 4:
                Sbox = S5;
                break;
            case 5:
                Sbox = S6;
                break;
            case 6:
                Sbox = S7;
                break;
            case 7:
                Sbox = S8;
                break;
        }
        S(Si, Pi, Sbox, i * 6);
    }
    P(Pi, f_re);
}

void E(int *Ri_1, int *Ei) {
    for (int i = 0; i < 48; ++i) {
        Ei[i] = Ri_1[table_E[i] - 1];
    }
}

void S(int *Si, int *Pi, int *Sbox, int index) {
    int index_p = index * 2 / 3;
    int C = 0, D = 0;
    C = Si[0 + index] * 2 + Si[5 + index];
    D = Si[1 + index] * 8 + Si[2 + index] * 4 + Si[3 + index] * 2 + Si[4 + index] * 1;
    InttoBarr(Sbox[C * 16 + D], Pi, index_p, 4);
}

void P(int *Pi, int *f_re) {
    for (int i = 0; i < 32; ++i) {
        f_re[i] = Pi[table_P[i] - 1];
    }
}

void InttoBarr(int dec, int *arr, int index, int bit) { //整数转换为二进制数组
    for (int i = bit - 1; i >= 0; i--) {
        if (dec & (1 << i))
            arr[(bit - i - 1) + index] = 1;
        else
            arr[(bit - i - 1) + index] = 0;
    }
}

void RotateKeyLeft(int *K, int index) {
    int temp;
    for (int j = 0; j < index; ++j) {
        temp = K[0];
        for (int i = 0; i < 27; ++i) {
            K[i] = K[i + 1];
        }
        K[27] = temp;
    }
}

void PC2(int *C, int *D, int id) {
    int index;
    for (int i = 0; i < 48; ++i) {
        index = table_PC2[i];
        if (index <= 28) {
            Keyi[id][i] = C[index - 1];
        } else {
            Keyi[id][i] = D[index - 29];
        }
    }
}

void fIV(int *M, int *IV) {
    for (int i = 0; i < 64; ++i) {
        M[i] ^= IV[i];
    }
}