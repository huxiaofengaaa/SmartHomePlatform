#include "AES.h"
#include <string.h>

int AES_ECB_Encrypt(char* p_plainText, int p_plainSize, char* p_chiperText,
		int p_chiperSize, char* p_key, PaddingType p_type)
{
	if(p_plainText == NULL || p_plainSize <= 0)
	{
		return -1;
	}

	int l_plainGrp = p_plainSize / 16;
	if(p_plainSize % 16 != 0)
	{
		l_plainGrp += 1;
	}
	int l_minChiperSize = l_plainGrp * 16;
	if(p_chiperText == NULL || p_chiperSize < l_minChiperSize
			|| p_key == NULL || strlen(p_key) == 0 || strlen(p_key) != 16)
	{
		return -1;
	}

	int i = 0;
	char currentPlainText[16] = { 0 };
	int l_plainIndex = 0;
	while(l_plainIndex < p_plainSize)
	{
		for(i = 0; i< 16; i++)
		{
			if(l_plainIndex + i < p_plainSize)
			{
				currentPlainText[i] = p_plainText[l_plainIndex + i];
			}
			else
			{
				currentPlainText[i] = p_type == PKCS5Padding ? '5' : '7';
			}
		}

		AES128Encrypt2(currentPlainText, 16, p_chiperText + l_plainIndex, 16, p_key);

		l_plainIndex += 16;
	}
	return 0;
}

int AES_ECB_Decrypt(char* p_chiperText, int p_chiperSize, char* p_plainText,
		int p_plainSize, char* p_key)
{
	if(p_chiperText == NULL || p_chiperSize <= 0 || p_chiperSize % 16 != 0
			|| p_plainText == NULL || p_plainSize < p_chiperSize
			|| p_key == NULL || strlen(p_key) < 16)
	{
		return -1;
	}
	int l_chiperIndex = 0;
	while(l_chiperIndex < p_chiperSize)
	{
		AES128Decrypt2(p_chiperText + l_chiperIndex, 16, p_plainText + l_chiperIndex, 16, p_key);
		l_chiperIndex += 16;
	}
	return 0;
}



