#include <iostream>
#include <string>
#include <stdio.h>
#include "AES.h"

void showArray(char* ptr, int size, std::string section)
{
	printf("========================================%s\n", section.c_str());
	printf("%s\n", ptr);
	for(int i = 0 ; i < size ; i++)
	{
		int value = ptr[i];
		printf("0x%02x ", (value & 0x000000FF));
	}
	printf("\n\n\n");
}

int main(int argc, char** argv)
{
	char src[128]  = { 0 };
	char key[32] = { 0 };

	snprintf(src, sizeof(src), "%s", "1234567890abcdef1234567890abcdef");
	snprintf(key, sizeof(key), "%s", "1234567890abcdef");
	showArray(src, 32, "src");
	showArray(key, 16, "key");

#if 0
	char l_AES128Encrypt[128] = { 0 };
	AES128Encrypt(src, 32, l_AES128Encrypt, 32, key);
	showArray(l_AES128Encrypt, 32, "l_AES128Encrypt");

	char l_AES128Decrypt[128] = { 0 };
	AES128Decrypt(l_AES128Encrypt, 32, l_AES128Decrypt, 32, key);
	showArray(l_AES128Decrypt, 32, "l_AES128Decrypt");
#endif

	char l_AES128Encrypt2[128] = { 0 };
	AES128Encrypt2(src, 32, l_AES128Encrypt2, 32, key);
	showArray(l_AES128Encrypt2, 32, "l_AES128Encrypt2");

	char l_AES128Decrypt2[128] = { 0 };
	AES128Decrypt2(l_AES128Encrypt2, 32, l_AES128Decrypt2, 32, key);
	showArray(l_AES128Decrypt2, 32, "l_AES128Decrypt2");

#if 0
	// ECB
	char l_AES_ECB_Encrypt[128] = { 0 };
	AES_ECB_Encrypt(src, 31, l_AES_ECB_Encrypt, 32, key, PKCS5Padding);
	showArray(l_AES_ECB_Encrypt, 32, "l_AES_ECB_Encrypt");

	char l_AES_ECB_Decrypt[128] = { 0 };
	AES_ECB_Decrypt(l_AES_ECB_Encrypt, 32, l_AES_ECB_Decrypt, 32, key);
	showArray(l_AES_ECB_Decrypt, 32, "l_AES_ECB_Decrypt");
#endif

	// CBC
	char l_AES_CBC_Encrypt[128] = { 0 };
	AES_CBC_Encrypt(src, 31, l_AES_CBC_Encrypt, 32, key, NULL, PKCS5Padding);
	showArray(l_AES_CBC_Encrypt, 32, "l_AES_CBC_Encrypt");

	char l_AES_CBC_Decrypt[128] = { 0 };
	AES_CBC_Decrypt(l_AES_CBC_Encrypt, 32, l_AES_CBC_Decrypt, 32, key, NULL);
	showArray(l_AES_CBC_Decrypt, 32, "l_AES_CBC_Decrypt");

#if 0
	// CFB
	char l_AES_CFB_Encrypt[128] = { 0 };
	AES_CFB_Encrypt(src, 31, l_AES_CFB_Encrypt, 32, key, NULL, PKCS5Padding);
	showArray(l_AES_CFB_Encrypt, 32, "l_AES_CFB_Encrypt");

	char l_AES_CFB_Decrypt[128] = { 0 };
	AES_CFB_Decrypt(l_AES_CFB_Encrypt, 32, l_AES_CFB_Decrypt, 32, key, NULL);
	showArray(l_AES_CFB_Decrypt, 32, "l_AES_CFB_Decrypt");

	// OFB
	char l_AES_OFB_Encrypt[128] = { 0 };
	AES_OFB_Encrypt(src, 31, l_AES_OFB_Encrypt, 32, key, NULL, PKCS5Padding);
	showArray(l_AES_OFB_Encrypt, 32, "l_AES_OFB_Encrypt");

	char l_AES_OFB_Decrypt[128] = { 0 };
	AES_OFB_Decrypt(l_AES_OFB_Encrypt, 32, l_AES_OFB_Decrypt, 32, key, NULL);
	showArray(l_AES_OFB_Decrypt, 32, "l_AES_OFB_Decrypt");
#endif
	return 0;
}


