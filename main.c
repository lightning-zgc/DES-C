#include "FileReader.h"
#include "Encrypt.h"

//加密模式
void ECB(FILE *fd, FILE *Sf, int *K, int mode) { //ECB模式加\解密
    int M[64] = {0}, C[64] = {0};
    while (readFile64(fd, M)) {
        DES(M, K, C, mode);
        writeFile64(Sf, C);
        memset(M, 0, sizeof(M));
    }
    rewind(fd);
}

void CBC(FILE *fd, FILE *Sf, int *IV, int *K, int mode) { //CBC模式加\解密
    int M[64] = {0}, C[64];
    while (readFile64(fd, M)) {
        if (mode == 0)fIV(M, IV);
//        C = DES(M, K, mode);
        DES(M, K, C, mode);
        if (mode == 1)fIV(C, IV);
        if (mode == 1)memcpy((void *) IV, (void *) M, sizeof(M));
        writeFile64(Sf, C);
        memset(M, 0, sizeof(M));
        if (mode == 0)memcpy((void *) IV, (void *) C, sizeof(C));
    }
    rewind(fd);
}

void print_usage(char *cmd) { //打印参数错误提示信息
    fprintf(stderr, " %s usage:\n", cmd);
    fprintf(stderr, "%s PlainText Key IV SaveFile mode(0 or 1)\n", cmd);

}

int main(int argc, char **argv) {
    printf("*****************************************\n");
    printf("*****************************************\n");
    printf("*******DES 加密演示程序(C语言实现)**********\n");
    printf("                  GLT                    \n");
    printf("*****************************************\n");
    printf("*****************************************\n");
    printf("*****************************************\n");

    char M_PATH[100], K_PATH[100], S_PATH[100], IV_PATH[100], S_PATH_T[100];
    int mode, K[64] = {0};
    switch (argc) {
        case 1:
            printf("加密请输入0，解密请输入1：\n");
            scanf("%d", &mode);
            if (mode == 0) { //加密模式
                int IV[64] = {0};

                printf("请输入待加密文件的路径：\n");
                scanf("%s", M_PATH);
                printf("请输入加密密钥的文件路径：\n");
                scanf("%s", K_PATH);
                printf("请输入CBC模式的初始化向量文件路径：\n");
                scanf("%s", IV_PATH);
                printf("请输入加密文件输出文件名（自动添加ECB和CBC后缀名）：\n");
                scanf("%s", S_PATH);

                FILE *Mf = readFilefromPATH(M_PATH);
                FILE *Kf = readFilefromPATH(K_PATH);
                FILE *IVf = readFilefromPATH(IV_PATH);
                strcpy(S_PATH_T, S_PATH);
                strcat(S_PATH_T, "_ECB.txt");
                FILE *Sf_ECB = writeFilefromPATH(S_PATH_T);
                strcpy(S_PATH_T, S_PATH);
                strcat(S_PATH_T, "_CBC.txt");
                FILE *Sf_CBC = writeFilefromPATH(S_PATH_T);

                readFile64(Kf, K);
                readFile64(IVf, IV);

                closeFile(Kf);
                closeFile(IVf);

                ECB(Mf, Sf_ECB, K, mode);
                CBC(Mf, Sf_CBC, IV, K, mode);

                closeFile(Mf);
                closeFile(Sf_ECB);
                closeFile(Sf_CBC);
            } else { //解密模式
                printf("请输入密文文件路径：\n");
                scanf("%s", M_PATH);
                printf("请输入密钥的文件路径：\n");
                scanf("%s", K_PATH);
                printf("请输入明文文件输出文件名：\n");
                scanf("%s", S_PATH);
                printf("请输入加密模式，ECB输入0，CBC输入1：\n");
                scanf("%d", &mode);

                FILE *Mf = readFilefromPATH(M_PATH);
                FILE *Kf = readFilefromPATH(K_PATH);
                FILE *Sf = writeFilefromPATH(S_PATH);
                readFile64(Kf, K);

                if (mode == 0) { //ECB模式解密
                    mode = 1;
                    ECB(Mf, Sf, K, mode);
                } else { //CBC模式解密
                    mode = 1;
                    int IV[64] = {0};
                    printf("请输入CBC模式的初始化向量文件路径：\n");
                    scanf("%s", IV_PATH);
                    FILE *IVf = readFilefromPATH(IV_PATH);
                    readFile64(IVf, IV);
                    closeFile(IVf);
                    CBC(Mf, Sf, IV, K, mode);
                }
                closeFile(Mf);
                closeFile(Kf);
                closeFile(Sf);
            }
            break;
        case 6:
            memcpy((void *) M_PATH, (void *) argv[1], sizeof(argv[1]));
            memcpy((void *) K_PATH, (void *) argv[2], sizeof(argv[2]));
            memcpy((void *) IV_PATH, (void *) argv[3], sizeof(argv[3]));
            memcpy((void *) S_PATH, (void *) argv[4], sizeof(argv[4]));
            mode = atoi(argv[5]);

            int IV[64] = {0};

            FILE *Mf = readFilefromPATH(M_PATH);
            FILE *Kf = readFilefromPATH(K_PATH);
            FILE *IVf = readFilefromPATH(IV_PATH);
            strcpy(S_PATH_T, S_PATH);
            strcat(S_PATH_T, "_ECB.txt");
            FILE *Sf_ECB = writeFilefromPATH(S_PATH_T);
            strcpy(S_PATH_T, S_PATH);
            strcat(S_PATH_T, "_CBC.txt");
            FILE *Sf_CBC = writeFilefromPATH(S_PATH_T);

            readFile64(Kf, K);
            readFile64(IVf, IV);

            closeFile(Kf);
            closeFile(IVf);

            ECB(Mf, Sf_ECB, K, mode);
            CBC(Mf, Sf_CBC, IV, K, mode);

            closeFile(Mf);
            closeFile(Sf_ECB);
            closeFile(Sf_CBC);

            break;
        default:
            print_usage(argv[0]);
            exit(1);
    }
    return 0;
}