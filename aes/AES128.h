
// 암호화 모드
#define ENC 1 
// 복호화 모드
#define DEC 0 

typedef unsigned char BYTE;

// 128비트 AES 암복호화 인터페이스
void AES128(BYTE *plain, BYTE *cipher, BYTE *key, int mode);

void expandKey(BYTE *key, BYTE *roundKey);
BYTE* subBytes(BYTE *block, int mode);
BYTE* shiftRows(BYTE *block, int mode);
BYTE* mixColums(BYTE *block, int mode);
BYTE* addRoundKey(BYTE *block, BYTE *rKey);