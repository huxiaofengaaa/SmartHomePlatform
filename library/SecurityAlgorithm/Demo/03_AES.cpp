#include <iostream>
#include <string>
#include <stdio.h>
#include "AesCbc.h"
#include "Aes.h"

int main(int argc, char** argv)
{
	char src[] = "1234567890abcdef";
	char l_key[] = "1234567890abcdef";
	char chiper[33];

//	printf("0x%02x \n", subByte(0x31));

	AES128Encrypt2(src, 32, chiper, 32, l_key);

	//AesCbcPKCS5PaddingEncrypt(src, 32, l_key);
	return 0;
}


