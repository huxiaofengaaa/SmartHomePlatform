#include <string.h>
#include <stdio.h>

static const int SBOX[16][16] =
{
	0x63, 0x7c, 0x77, 0x7b, 0xf2, 0x6b, 0x6f, 0xc5, 0x30, 0x01, 0x67, 0x2b, 0xfe, 0xd7, 0xab, 0x76,
    0xca, 0x82, 0xc9, 0x7d, 0xfa, 0x59, 0x47, 0xf0, 0xad, 0xd4, 0xa2, 0xaf, 0x9c, 0xa4, 0x72, 0xc0,
    0xb7, 0xfd, 0x93, 0x26, 0x36, 0x3f, 0xf7, 0xcc, 0x34, 0xa5, 0xe5, 0xf1, 0x71, 0xd8, 0x31, 0x15,
    0x04, 0xc7, 0x23, 0xc3, 0x18, 0x96, 0x05, 0x9a, 0x07, 0x12, 0x80, 0xe2, 0xeb, 0x27, 0xb2, 0x75,
    0x09, 0x83, 0x2c, 0x1a, 0x1b, 0x6e, 0x5a, 0xa0, 0x52, 0x3b, 0xd6, 0xb3, 0x29, 0xe3, 0x2f, 0x84,
    0x53, 0xd1, 0x00, 0xed, 0x20, 0xfc, 0xb1, 0x5b, 0x6a, 0xcb, 0xbe, 0x39, 0x4a, 0x4c, 0x58, 0xcf,
    0xd0, 0xef, 0xaa, 0xfb, 0x43, 0x4d, 0x33, 0x85, 0x45, 0xf9, 0x02, 0x7f, 0x50, 0x3c, 0x9f, 0xa8,
    0x51, 0xa3, 0x40, 0x8f, 0x92, 0x9d, 0x38, 0xf5, 0xbc, 0xb6, 0xda, 0x21, 0x10, 0xff, 0xf3, 0xd2,
    0xcd, 0x0c, 0x13, 0xec, 0x5f, 0x97, 0x44, 0x17, 0xc4, 0xa7, 0x7e, 0x3d, 0x64, 0x5d, 0x19, 0x73,
    0x60, 0x81, 0x4f, 0xdc, 0x22, 0x2a, 0x90, 0x88, 0x46, 0xee, 0xb8, 0x14, 0xde, 0x5e, 0x0b, 0xdb,
    0xe0, 0x32, 0x3a, 0x0a, 0x49, 0x06, 0x24, 0x5c, 0xc2, 0xd3, 0xac, 0x62, 0x91, 0x95, 0xe4, 0x79,
    0xe7, 0xc8, 0x37, 0x6d, 0x8d, 0xd5, 0x4e, 0xa9, 0x6c, 0x56, 0xf4, 0xea, 0x65, 0x7a, 0xae, 0x08,
    0xba, 0x78, 0x25, 0x2e, 0x1c, 0xa6, 0xb4, 0xc6, 0xe8, 0xdd, 0x74, 0x1f, 0x4b, 0xbd, 0x8b, 0x8a,
    0x70, 0x3e, 0xb5, 0x66, 0x48, 0x03, 0xf6, 0x0e, 0x61, 0x35, 0x57, 0xb9, 0x86, 0xc1, 0x1d, 0x9e,
    0xe1, 0xf8, 0x98, 0x11, 0x69, 0xd9, 0x8e, 0x94, 0x9b, 0x1e, 0x87, 0xe9, 0xce, 0x55, 0x28, 0xdf,
    0x8c, 0xa1, 0x89, 0x0d, 0xbf, 0xe6, 0x42, 0x68, 0x41, 0x99, 0x2d, 0x0f, 0xb0, 0x54, 0xbb, 0x16 };

static const int S2BOX[16][16] =
{
	0x52, 0x09, 0x6a, 0xd5, 0x30, 0x36, 0xa5, 0x38, 0xbf, 0x40, 0xa3, 0x9e, 0x81, 0xf3, 0xd7, 0xfb,
    0x7c, 0xe3, 0x39, 0x82, 0x9b, 0x2f, 0xff, 0x87, 0x34, 0x8e, 0x43, 0x44, 0xc4, 0xde, 0xe9, 0xcb,
    0x54, 0x7b, 0x94, 0x32, 0xa6, 0xc2, 0x23, 0x3d, 0xee, 0x4c, 0x95, 0x0b, 0x42, 0xfa, 0xc3, 0x4e,
    0x08, 0x2e, 0xa1, 0x66, 0x28, 0xd9, 0x24, 0xb2, 0x76, 0x5b, 0xa2, 0x49, 0x6d, 0x8b, 0xd1, 0x25,
    0x72, 0xf8, 0xf6, 0x64, 0x86, 0x68, 0x98, 0x16, 0xd4, 0xa4, 0x5c, 0xcc, 0x5d, 0x65, 0xb6, 0x92,
    0x6c, 0x70, 0x48, 0x50, 0xfd, 0xed, 0xb9, 0xda, 0x5e, 0x15, 0x46, 0x57, 0xa7, 0x8d, 0x9d, 0x84,
    0x90, 0xd8, 0xab, 0x00, 0x8c, 0xbc, 0xd3, 0x0a, 0xf7, 0xe4, 0x58, 0x05, 0xb8, 0xb3, 0x45, 0x06,
    0xd0, 0x2c, 0x1e, 0x8f, 0xca, 0x3f, 0x0f, 0x02, 0xc1, 0xaf, 0xbd, 0x03, 0x01, 0x13, 0x8a, 0x6b,
    0x3a, 0x91, 0x11, 0x41, 0x4f, 0x67, 0xdc, 0xea, 0x97, 0xf2, 0xcf, 0xce, 0xf0, 0xb4, 0xe6, 0x73,
    0x96, 0xac, 0x74, 0x22, 0xe7, 0xad, 0x35, 0x85, 0xe2, 0xf9, 0x37, 0xe8, 0x1c, 0x75, 0xdf, 0x6e,
    0x47, 0xf1, 0x1a, 0x71, 0x1d, 0x29, 0xc5, 0x89, 0x6f, 0xb7, 0x62, 0x0e, 0xaa, 0x18, 0xbe, 0x1b,
    0xfc, 0x56, 0x3e, 0x4b, 0xc6, 0xd2, 0x79, 0x20, 0x9a, 0xdb, 0xc0, 0xfe, 0x78, 0xcd, 0x5a, 0xf4,
    0x1f, 0xdd, 0xa8, 0x33, 0x88, 0x07, 0xc7, 0x31, 0xb1, 0x12, 0x10, 0x59, 0x27, 0x80, 0xec, 0x5f,
    0x60, 0x51, 0x7f, 0xa9, 0x19, 0xb5, 0x4a, 0x0d, 0x2d, 0xe5, 0x7a, 0x9f, 0x93, 0xc9, 0x9c, 0xef,
    0xa0, 0xe0, 0x3b, 0x4d, 0xae, 0x2a, 0xf5, 0xb0, 0xc8, 0xeb, 0xbb, 0x3c, 0x83, 0x53, 0x99, 0x61,
    0x17, 0x2b, 0x04, 0x7e, 0xba, 0x77, 0xd6, 0x26, 0xe1, 0x69, 0x14, 0x63, 0x55, 0x21, 0x0c, 0x7d };

const int Rcon[10] =
{
	0x01000000, 0x02000000,
    0x04000000, 0x08000000,
    0x10000000, 0x20000000,
    0x40000000, 0x80000000,
    0x1b000000, 0x36000000
};


int mergeFourIntoInteger(int* p_ptr)
{
    int one = p_ptr[0];
    one = (one << 24) & 0xFF000000;

    int two = p_ptr[1];
    two = (two << 16) & 0x00FF0000;

    int three = p_ptr[2];
    three = (three << 8) & 0x0000FF00;

    int four = p_ptr[3] & 0x000000FF;
    return one | two | three | four;
}

void splitIntegerIntoFour(int p_value, int* p_ptr)
{
	p_ptr[0] = (p_value >> 24) & 0x000000FF;
	p_ptr[1] = (p_value >> 16) & 0x000000FF;
	p_ptr[2] = (p_value >> 8)  & 0x000000FF;
	p_ptr[3] = (p_value >> 0)  & 0x000000FF;
}

int T(int p_value, int p_round)
{
	int i = 0;
	int l_array[4] = { 0 };
	splitIntegerIntoFour(p_value, l_array);

	int tmp = l_array[0];
	l_array[0] = l_array[1];
	l_array[1] = l_array[2];
	l_array[2] = l_array[3];
	l_array[3] = tmp;

	for(i = 0 ; i < 4 ; i++)
	{
		int l_row = (l_array[i] >> 4) & 0x0000000F;
		int l_column = l_array[i] & 0x0000000F;
		l_array[i] = SBOX[l_row][l_column];
	}

	int l_result = mergeFourIntoInteger(l_array);
	return l_result ^ Rcon[p_round];
}


void doExternKey(const char* p_key, int* p_externKey)
{
	int i = 0;
	int j = 0;
	for(i = 0 ; i < 16 ; i++)
	{
		int l_curKey = (int)p_key[i];
		p_externKey[i] = l_curKey & 0x000000FF;
	}

	int l_round = 0;
	int l_column = 0;
	for(l_round = 1 ; l_round <= 10 ; l_round++)
	{
		for(l_column = 0 ; l_column < 4 ; l_column++)
		{
			int l_currentColumnWord = 0;
			int* l_currentPosition = p_externKey + l_round * 16 + l_column * 4;

			int l_columnWordDe4 = mergeFourIntoInteger(l_currentPosition - 16);
			int l_columnWordDe1 = mergeFourIntoInteger(l_currentPosition - 4);

			if((l_round * 4 + l_column) % 4 == 0)
			{
				l_currentColumnWord = l_columnWordDe4 ^ T(l_columnWordDe1, l_round - 1);
			}
			else
			{
				l_currentColumnWord = l_columnWordDe4 ^ l_columnWordDe1;
			}
			splitIntegerIntoFour(l_currentColumnWord, l_currentPosition);
		}
	}

#if 0
	printf("========================doExternKey\n");
	for(i = 0 ; i < 11; i++)
	{
		for(j = 0 ; j < 16 ; j++)
		{
			printf("0x%02x ", p_externKey[i * 16 + j]);
		}
		printf("\n");
	}
	printf("===================================\n");
#endif
}

void convertString2Array(const char* p_str, int p_array[][4])
{
	//printf("========================convertString2Array\n");
	int l_row = 0;
	int l_column = 0;
	for(l_row = 0; l_row < 4; l_row++)
	{
		for(l_column = 0; l_column < 4; l_column++)
		{
			int value = p_str[l_column * 4 + l_row];
			p_array[l_row][l_column] = value & 0x000000FF;
		}
	}
}

void convertArray2String(char* p_str, int p_array[][4])
{
	//printf("========================convertArray2String\n");
	int l_row = 0;
	int l_column = 0;
	for(l_row = 0; l_row < 4; l_row++)
	{
		for(l_column = 0; l_column < 4; l_column++)
		{
			int value = p_array[l_row][l_column];
			p_str[l_column * 4 + l_row] = (char)(value & 0x000000FF);
		}
	}
}

void showArray(int p_array[][4])
{
	int l_row = 0;
	int l_column = 0;
	for(l_row = 0; l_row < 4; l_row++)
	{
		for(l_column = 0; l_column < 4; l_column++)
		{
			printf("0x%02x ", p_array[l_row][l_column]);
		}
		printf("\n");
	}
}

void addRoundKey(int p_array[][4], int* p_currentKey)
{
	//printf("========================addRoundKey\n");
	int l_column = 0;
	int l_row = 0;

	for(l_row = 0 ; l_row < 4 ; l_row++)
	{
		for(l_column = 0; l_column < 4; l_column++)
		{
			int l_key = p_currentKey[4 * l_column + l_row];
			int l_value = p_array[l_row][l_column];
			p_array[l_row][l_column] = l_key ^ l_value;
		}
	}
}

void subBytesArray(int p_array[][4])
{
	int l_Row = 0;
	int l_Column = 0;
	for(l_Row = 0 ; l_Row < 4 ; l_Row++)
	{
		for(l_Column = 0 ; l_Column < 4 ; l_Column++)
		{
			int l_currentValue = p_array[l_Row][l_Column];
			int l_rowNumber    = (l_currentValue & 0x000000F0) >> 4 ;
			int l_columnNumber =  l_currentValue & 0x0000000F;
			p_array[l_Row][l_Column] = SBOX[l_rowNumber][l_columnNumber];
		}
	}
}

void deSubBytesArray(int p_array[][4])
{
	int l_Row = 0;
	int l_Column = 0;
	for(l_Row = 0 ; l_Row < 4 ; l_Row++)
	{
		for(l_Column = 0 ; l_Column < 4 ; l_Column++)
		{
			int l_currentValue = p_array[l_Row][l_Column];
			int l_rowNumber    = (l_currentValue & 0x000000F0) >> 4 ;
			int l_columnNumber =  l_currentValue & 0x0000000F;
			p_array[l_Row][l_Column] = S2BOX[l_rowNumber][l_columnNumber];
		}
	}
}

void leftLoop4int(int array[4], int step)
{
	int i = 0;
    int temp[4];
    for(i = 0; i < 4; i++)
    {
        temp[i] = array[i];
    }

    int index = step % 4 == 0 ? 0 : step % 4;
    for(i = 0; i < 4; i++)
    {
        array[i] = temp[index];
        index++;
        index = index % 4;
    }
}

void rightLoop4int(int array[4], int step)
{
	int i = 0;
    int temp[4];
    for(i = 0; i < 4; i++)
    {
    	temp[i] = array[i];
    }

    int index = step % 4 == 0 ? 0 : step % 4;
    index = 3 - index;
    for(i = 3; i >= 0; i--)
    {
        array[i] = temp[index];
        index--;
        index = index == -1 ? 3 : index;
    }
}

void shiftRows(int array[4][4])
{
	int i = 0;
    int rowTwo[4], rowThree[4], rowFour[4];
    //复制状态矩阵的第2,3,4行
    for(i = 0; i < 4; i++)
    {
        rowTwo[i] = array[1][i];
        rowThree[i] = array[2][i];
        rowFour[i] = array[3][i];
    }
    //循环左移相应的位数
    leftLoop4int(rowTwo, 1);
    leftLoop4int(rowThree, 2);
    leftLoop4int(rowFour, 3);

    //把左移后的行复制回状态矩阵中
    for(i = 0; i < 4; i++)
    {
        array[1][i] = rowTwo[i];
        array[2][i] = rowThree[i];
        array[3][i] = rowFour[i];
    }
}

void deShiftRows(int array[4][4])
{
	int i = 0;
    int rowTwo[4], rowThree[4], rowFour[4];
    for(i = 0; i < 4; i++)
    {
        rowTwo[i] = array[1][i];
        rowThree[i] = array[2][i];
        rowFour[i] = array[3][i];
    }

    rightLoop4int(rowTwo, 1);
    rightLoop4int(rowThree, 2);
    rightLoop4int(rowFour, 3);

    for(i = 0; i < 4; i++)
    {
        array[1][i] = rowTwo[i];
        array[2][i] = rowThree[i];
        array[3][i] = rowFour[i];
    }
}

const int colM[4][4] =
{
	2, 3, 1, 1,
    1, 2, 3, 1,
    1, 1, 2, 3,
    3, 1, 1, 2
};

const int deColM[4][4] =
{
	0x0000000E, 0x0000000B, 0x0000000D, 0x00000009,
    0x00000009, 0x0000000E, 0x0000000B, 0x0000000D,
    0x0000000D, 0x00000009, 0x0000000E, 0x0000000B,
    0x0000000B, 0x0000000D, 0x00000009, 0x0000000E
};

int GFMul2(int s)
{
    int result = s << 1;
    int a7 = result & 0x00000100;

    if(a7 != 0)
    {
        result = result & 0x000000ff;
        result = result ^ 0x1b;
    }

    return result;
}

int GFMul(int n, int s)
{
    int result;

    if(n == 1)
    {
    	return s;
    }
    else if(n == 2)
    {
    	return GFMul2(s);
    }
    else if(n == 3)
    {
    	return GFMul2(s) ^ s;
    }
    else if(n == 4)
    {
    	return GFMul2(GFMul2(s));
    }
    else if(n == 8)
    {
    	return GFMul2(GFMul(4, s));
    }
    else if(n == 0x9)
    {
    	return GFMul(8, s) ^ s;
    }
    else if(n == 0xb)//11
    {
    	return GFMul(8, s) ^ s ^ GFMul2(s);
    }
    else if(n == 0xd)//13
    {
    	return GFMul(8, s) ^ GFMul(4, s) ^ s;
    }
    else if(n == 0xe)//14
    {
    	return GFMul(8, s) ^ GFMul(4, s) ^ GFMul2(s);
    }

    return result;
}

void mixColumns(int p_array[][4])
{
	int l_Row = 0;
	int l_Column = 0;

	int l_arrayTmp[4][4] = { 0 };
	for(l_Row = 0 ; l_Row < 4 ; l_Row++)
	{
		for(l_Column = 0 ; l_Column < 4 ; l_Column++)
		{
			l_arrayTmp[l_Row][l_Column] = p_array[l_Row][l_Column];
		}
	}
	for(l_Row = 0 ; l_Row < 4 ; l_Row++)
	{
		for(l_Column = 0 ; l_Column < 4 ; l_Column++)
		{
			p_array[l_Row][l_Column] =
					  GFMul(colM[l_Row][0], l_arrayTmp[0][l_Column])
					^ GFMul(colM[l_Row][1], l_arrayTmp[1][l_Column])
					^ GFMul(colM[l_Row][2], l_arrayTmp[2][l_Column])
					^ GFMul(colM[l_Row][3], l_arrayTmp[3][l_Column]);

		}
	}
}

void deMixColumns(int p_array[][4])
{
	int l_Row = 0;
	int l_Column = 0;

	int l_arrayTmp[4][4] = { 0 };
	for(l_Row = 0 ; l_Row < 4 ; l_Row++)
	{
		for(l_Column = 0 ; l_Column < 4 ; l_Column++)
		{
			l_arrayTmp[l_Row][l_Column] = p_array[l_Row][l_Column];
		}
	}
	for(l_Row = 0 ; l_Row < 4 ; l_Row++)
	{
		for(l_Column = 0 ; l_Column < 4 ; l_Column++)
		{
			p_array[l_Row][l_Column] =
					  GFMul(deColM[l_Row][0], l_arrayTmp[0][l_Column])
					^ GFMul(deColM[l_Row][1], l_arrayTmp[1][l_Column])
					^ GFMul(deColM[l_Row][2], l_arrayTmp[2][l_Column])
					^ GFMul(deColM[l_Row][3], l_arrayTmp[3][l_Column]);
		}
	}
}

void addRoundTowArray(int aArray[4][4],int bArray[4][4])
{
	int l_Row = 0;
	int l_Column = 0;
	for(l_Row = 0 ; l_Row < 4 ; l_Row++)
    {
		for(l_Column = 0 ; l_Column < 4 ; l_Column++)
        {
            aArray[l_Row][l_Column] =
            		aArray[l_Row][l_Column] ^ bArray[l_Row][l_Column];
        }
    }
}

int AES128Encrypt2(char *p_plainText, int p_plainSize, char* p_chiperText,
		int p_chiperSize, char *p_key)
{
	int l_keySize =strlen(p_key);
	int l_plainIndex = 0;

	if(p_plainText == NULL || p_plainSize == 0 || p_plainSize % 16 != 0
			|| p_chiperText == NULL || p_chiperSize < p_plainSize
			|| l_keySize < 16)
	{
		printf("p_plainSize = %d, p_chiperSize = %d, l_keySize = %d \n",
				p_plainSize, p_chiperSize, l_keySize);
		return -1;
	}

	int l_externKey[16 * 11] = { 0 };
	doExternKey(p_key, l_externKey);

	int l_CurrentArray[4][4] = { 0 };
	for(l_plainIndex = 0; l_plainIndex < p_plainSize; l_plainIndex += 16)
	{
		convertString2Array(p_plainText + l_plainIndex, l_CurrentArray);
		addRoundKey(l_CurrentArray, l_externKey + (0 * 16));

		int l_currentRound = 1;
		for(l_currentRound = 1 ; l_currentRound <= 9 ; l_currentRound++)
		{
			subBytesArray(l_CurrentArray);
            shiftRows(l_CurrentArray);
            mixColumns(l_CurrentArray);
            addRoundKey(l_CurrentArray, l_externKey + (l_currentRound * 16));
		}
		//showArray(l_CurrentArray);
		subBytesArray(l_CurrentArray);//字节代换
        shiftRows(l_CurrentArray);//行移位
        addRoundKey(l_CurrentArray, l_externKey + (10 * 16));
        convertArray2String(p_chiperText + l_plainIndex, l_CurrentArray);
	}

	return 0;
}

int AES128Decrypt2(char *p_chiperText, int p_chiperSize, char* p_plainText,
		int p_plainSize, char *p_key)
{
    int l_row = 0;
    int l_column = 0;
	int l_keySize =strlen(p_key);
	int l_chiperIndex = 0;

	if(p_plainText == NULL || p_plainSize == 0
			|| p_chiperText == NULL || p_chiperSize % 16 != 0
			|| l_keySize < 16)
	{
		printf("p_plainSize = %d, p_chiperSize = %d, l_keySize = %d \n",
				p_plainSize, p_chiperSize, l_keySize);
		return -1;
	}
	int l_externKey[16 * 11] = { 0 };
	doExternKey(p_key, l_externKey);

	int l_CurrentArray[4][4] = { 0 };
	for(l_chiperIndex = 0; l_chiperIndex < p_chiperSize; l_chiperIndex += 16)
	{
		convertString2Array(p_chiperText + l_chiperIndex, l_CurrentArray);
		addRoundKey(l_CurrentArray, l_externKey + (10 * 16));
		int l_currentRound = 9;
		for(l_currentRound = 9 ; l_currentRound >= 1 ; l_currentRound--)
		{
            deSubBytesArray(l_CurrentArray);
            deShiftRows(l_CurrentArray);
            deMixColumns(l_CurrentArray);

            int l_currentKey[4][4] = { 0 };
            for(l_row = 0 ; l_row < 4 ; l_row++)
            {
            	for(l_column = 0 ; l_column < 4 ; l_column++)
            	{
            		l_currentKey[l_row][l_column] =
            				l_externKey[l_currentRound * 16 + 4 * l_column + l_row];
            	}
            }
            deMixColumns(l_currentKey);
            addRoundTowArray(l_CurrentArray, l_currentKey);
		}
		deSubBytesArray(l_CurrentArray);
        deShiftRows(l_CurrentArray);
        addRoundKey(l_CurrentArray, l_externKey + (0 * 16));
        convertArray2String(p_plainText + l_chiperIndex, l_CurrentArray);
	}
	return 0;
}
