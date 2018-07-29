#include <string.h>
#include <stdio.h>
#include "Aes.h"

AES_STATUS AES_CBC_Encrypt(const char* p_plainText, const int p_plainSize, char* p_chiperText,
		const int p_chiperSize, const char* p_key, const char* p_initVector, PaddingType p_type)
{
	if(p_key == NULL || strlen(p_key) < 16)
	{
		return AES_ERROR_KEY;
	}
	if(p_initVector != NULL && strlen(p_initVector) < 16)
	{
		return AES_ERROR_INITVECTOR;
	}
	if(p_plainText == NULL || p_plainSize <= 0 )
	{
		return AES_ERROR_PLAINTEXT;
	}
	if(p_chiperText == NULL || p_chiperSize <= 0)
	{
		return AES_ERROR_CHIPERTEXT;
	}

	int l_chiperActualSize = ((int)(p_chiperSize / 16)) * 16;
	int l_plainActualSize = ((int)(p_plainSize / 16)) * 16 + (p_plainSize % 16 == 0 ? 0 : 16);
	int l_textLength = l_chiperActualSize <= l_plainActualSize ? l_chiperActualSize : l_plainActualSize;

	char l_LastInput[32] = { 0 };
	if(p_initVector == NULL)
	{
		snprintf(l_LastInput, sizeof(l_LastInput), "%s", "1234567890123456");
	}
	else
	{
		snprintf(l_LastInput, sizeof(l_LastInput), "%s", p_initVector);
	}

	int l_plainIndex = 0;
	int i = 0;
	char currentPlainText[16] = { 0 };
	char currentTmpText[16] = { 0 };
	while(l_plainIndex < l_textLength)
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
			int l_chiperByte = ((int)l_LastInput[i]) & 0x000000FF;
			currentTmpText[i] = (char)((l_plain ^ l_chiperByte) & 0x000000FF);
		}

		AES128Encrypt2(currentTmpText, 16, p_chiperText + l_plainIndex, 16, p_key);

		for(i = 0; i< 16; i++)
		{
			l_LastInput[i] = p_chiperText[l_plainIndex + i];
		}

		l_plainIndex += 16;
	}
	return AES_SUCCESS;
}

AES_STATUS AES_CBC_Decrypt(const char* p_chiperText, const int p_chiperSize,
		char* p_plainText, const int p_plainSize, const char* p_key, const char* p_initVector)
{
	if(p_key == NULL || strlen(p_key) < 16)
	{
		return AES_ERROR_KEY;
	}
	if(p_initVector != NULL && strlen(p_initVector) < 16)
	{
		return AES_ERROR_INITVECTOR;
	}
	if(p_plainText == NULL || p_plainSize <= 0 )
	{
		return AES_ERROR_PLAINTEXT;
	}
	if(p_chiperText == NULL || p_chiperSize <= 0)
	{
		return AES_ERROR_CHIPERTEXT;
	}

	int l_chiperActualSize = ((int)(p_chiperSize / 16)) * 16;
	int l_plainActualSize = ((int)(p_plainSize / 16)) * 16;
	int l_textLength = l_chiperActualSize <= l_plainActualSize ? l_chiperActualSize : l_plainActualSize;

	char l_LastInput[32] = { 0 };
	if(p_initVector == NULL)
	{
		snprintf(l_LastInput, sizeof(l_LastInput), "%s", "1234567890123456");
	}
	else
	{
		snprintf(l_LastInput, sizeof(l_LastInput), "%s", p_initVector);
	}

	int l_chiperIndex = 0;
	int i = 0;
	while(l_chiperIndex < l_textLength)
	{
		AES128Decrypt2(p_chiperText + l_chiperIndex, 16, p_plainText + l_chiperIndex, 16, p_key);
		for(i = 0; i< 16; i++)
		{
			int l_plain = ((int)p_plainText[l_chiperIndex + i]) & 0x000000FF;
			int l_chiperByte = ((int)l_LastInput[i]) & 0x000000FF;
			p_plainText[l_chiperIndex + i] = (char)((l_plain ^ l_chiperByte) & 0x000000FF);
		}

		for(i = 0; i< 16; i++)
		{
			l_LastInput[i] = p_chiperText[l_chiperIndex + i];
		}

		l_chiperIndex += 16;
	}
	return AES_SUCCESS;
}
