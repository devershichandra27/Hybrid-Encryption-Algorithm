#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "blowfish.c"
#include "aes.c"
#include <time.h>

typedef struct
{
    uint p[18];
    uint s[4][256];
} KeyStruct;



int main(int argc, char const *argv[])
{ 
  clock_t begin = clock();
  FILE* f1 = NULL; //file one for input in plaintext
  FILE* f2=NULL; //file for storing cipher text
  FILE* f3=NULL; // second handler for cipher.txt file
  FILE* f4=NULL; // third file for final plaintext after decryption

  uchar plainText[16];
  memset(plainText,0,sizeof(plainText));
  f1 = fopen("test.txt","r+");
  f2 = fopen("cipher.txt", "w+");
  int read = 0;
  uchar key[] = "Devershi";
  KeyStruct ptr1;
  keyExpansionBlowfish(&ptr1, key);
  uchar * cipher = malloc(16);
  uchar * expandedKey = keyExpansion(key);
  uint l1, l2, r1, r2;
  uchar *c1 = malloc(4);
  uchar *c2 = malloc(4);;
  uchar *c3 = malloc(4);;
  uchar *c4 = malloc(4);;
  uchar * outPut = malloc(16);
  while((read= fread(plainText,16,1,f1))>0)
  {
    AESEncryption(plainText, expandedKey, cipher);
    l1 = (0x00000000ffffffff)&((cipher[0] << 24) | (cipher[1] << 16)  | (cipher[2] << 8) | (cipher[3])), r1= (0x00000000ffffffff)&((cipher[4] << 24) | (cipher[5] << 16)  | (cipher[6] << 8) | (cipher[7])), l2 = (0x00000000ffffffff)&((cipher[8] << 24) | (cipher[9] << 16)  | (cipher[10] << 8) | (cipher[11])), r2= (0x00000000ffffffff)&((cipher[12] << 24) | (cipher[13] << 16)  | (cipher[14] << 8) | (cipher[15]));
    BlowfishEncryption(&ptr1, &l1, &r1);
    BlowfishEncryption(&ptr1, &l2, &r2);
    fprintf(f2, "%lX %lX %lX %lX ",l1, r1, l2, r2);
  }
  fclose(f2);
  fclose(f1);
  f3 = fopen("cipher.txt", "r+");
  f4 = fopen("del.txt", "w+");
  l1=0x00, l2= 0x00, r1=0x00, r2=0x00;
  while(!feof(f3))
  {
    fscanf(f3, "%lx", &l1);
    fscanf(f3, "%lx", &r1);
    fscanf(f3, "%lx", &l2);
    fscanf(f3, "%lx", &r2);
    BlowfishDecryption(&ptr1, &l1, &r1);
    BlowfishDecryption(&ptr1, &l2, &r2);
    c1 = (uchar *)&l1;
    c2 = (uchar *)&r1;
    c3 = (uchar *)&l2;
    c4 = (uchar *)&r2;
    for (int i = 0; i < 4; ++i)
    {
        cipher[i] = c1[3-i];
        cipher[4+i] = c2[3-i];
        cipher[8+i] = c3[3-i];
        cipher[12+i]= c4[3-i];
    }
    AESDecryption(cipher, expandedKey, outPut);
    fprintf(f4, "%s", outPut );
  }
  fclose(f3);
  fclose(f4);
  clock_t end = clock();
	printf("Elapsed time: %.f clock ticks\n", (double)((end-begin)) );
	return 0;
}
