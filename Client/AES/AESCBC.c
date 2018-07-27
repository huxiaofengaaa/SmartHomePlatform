#include <string.h>
#include <stdio.h>
#include "AES.h"

int AES_CBC_Encrypt(char* p_plainText, int p_plainSize, char* p_chiperText,
		int p_chiperSize, char* p_key, char* p_initVector, PaddingType p_type)
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
			|| p_key == NULL || strlen(p_key) == 0 || strlen(p_key) != 16
			|| (p_initVector != NULL && strlen(p_initVector) < 16))
	{
		return -1;
	}
	if(p_initVector == NULL)
	{
		p_initVector = "1234567890123456";
	}


	int l_plainIndex = 0;
	int i = 0;
	char currentPlainText[16] = { 0 };
	char currentTmpText[16] = { 0 };
	while(l_plainIndex < p_plainSize)
	{
		for(i = 0; i< 16; i++)
		{
			if(l_plainIndex + i >= p_plainSize)
			{
				currentPlainText[i] = p_type == PKCS5Padding ? '5' : '7';
				continue;
			}
			currentPlainText[i] = p_plainText[l_plainIndex + i];
		}
		for(i = 0; i< 16; i++)
		{
			int l_plain = ((int)currentPlainText[i]) & 0x000000FF;
			int l_chiperByte = ((int)p_initVector[i]) & 0x000000FF;
			currentTmpText[i] = (char)((l_plain ^ l_chiperByte) & 0x000000FF);
		}
		AES128Encrypt2(currentTmpText, 16, p_chiperText + l_plainIndex, 16, p_key);
		p_initVector = p_chiperText + l_plainIndex;
		l_plainIndex += 16;
	}
	return 0;
}

int AES_CBC_Decrypt(char* p_chiperText, int p_chiperSize,
		char* p_plainText, int p_plainSize, char* p_key, char* p_initVector)
{
	if(p_chiperText == NULL || p_chiperSize <= 0 || p_chiperSize % 16 != 0
			|| p_plainText == NULL || p_plainSize < p_chiperSize
			|| p_key == NULL || strlen(p_key) < 16
			|| (p_initVector != NULL && strlen(p_initVector) < 16))
	{
		return -1;
	}
	if(p_initVector == NULL)
	{
		p_initVector = "1234567890123456";
	}
	int l_chiperIndex = 0;
	int i = 0;
	while(l_chiperIndex < p_chiperSize)
	{
		AES128Decrypt2(p_chiperText + l_chiperIndex, 16, p_plainText + l_chiperIndex, 16, p_key);
		for(i = 0; i< 16; i++)
		{
			int l_plain = ((int)p_plainText[l_chiperIndex + i]) & 0x000000FF;
			int l_chiperByte = ((int)p_initVector[i]) & 0x000000FF;
			p_plainText[l_chiperIndex + i] = (char)((l_plain ^ l_chiperByte) & 0x000000FF);
		}
		p_initVector = p_chiperText + l_chiperIndex;
		l_chiperIndex += 16;
	}
	return 0;
}
