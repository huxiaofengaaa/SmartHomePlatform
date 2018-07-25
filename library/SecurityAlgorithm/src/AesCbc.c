#include "Aes.h"
#include <string.h>
#include <stdio.h>

int AesCbcPKCS5PaddingEncrypt(const char* p_plainText, const int p_plainSize, char* key)
{
	int i = 0;
	int j = 0;
	char initializationVector[16] = { 0 };
	char currentPlainText[16] = { 0 };
	char lastChiperText[16] = { 0 };
	char currentTmpText[16] = { 0 };

	if(strlen(key) != 16)
	{
		printf("The key character length must be 16, Current length is %d\n", strlen(key));
		return -1;
	}

	for(i = 0; i< 16; i++)
	{
		if(i == 0)
		{
			printf("initializationVector ");
		}
		printf("0x%x ", initializationVector[i]);
		if(i+1 >= 16)
		{
			printf("\n");
		}
	}

	int l_plainIndex = 0;
	while(l_plainIndex < p_plainSize)
	{
		printf("=============================================\n");
		for(i = 0; i< 16; i++)
		{
			if(l_plainIndex + i < p_plainSize)
			{
				currentPlainText[i] = p_plainText[l_plainIndex + i];
			}
			else
			{
				currentPlainText[i] = 5;
			}
			printf("%c ", currentPlainText[i]);
		}
		printf("\n");

		if(strlen(lastChiperText))
		{
			for(i = 0; i< 16; i++)
			{
				currentTmpText[i] = currentPlainText[i] ^ lastChiperText[i];
				printf("0x%02x ", currentTmpText[i]);
			}
		}
		else
		{
			for(i = 0; i< 16; i++)
			{
				currentTmpText[i] = currentPlainText[i] ^ initializationVector[i];
				printf("0x%02x ", currentTmpText[i]);
			}
		}
		printf("\n");

		AES128Encrypt(currentTmpText, 16, lastChiperText, 16, key);

		l_plainIndex += 16;
		for(i = 0; i< 16; i++)
		{
			printf("%02x ", (char)lastChiperText[i]);
		}
		printf("\n");
	}

	return 0;
}


