#ifndef LIBRARY_AES_AES_H_
#define LIBRARY_AES_AES_H_

#ifdef __cplusplus
extern "C"
{
#endif

typedef enum
{
	PKCS5Padding,
	PKCS7Padding
}PaddingType;

int AES128Encrypt(char *p_plainText, int p_plainSize, char* p_chiperText,
		int p_chiperSize, char *key);

int AES128Decrypt(char* p_chiperText, int p_chiperSize, char* p_plainText,
		int p_plainSize, char* key);

int AES128Encrypt2(char *p_plainText, int p_plainSize, char* p_chiperText,
		int p_chiperSize, char *p_key);

int AES128Decrypt2(char *p_chiperText, int p_chiperSize, char* p_plainText,
		int p_plainSize, char *p_key);

int AES_ECB_Encrypt(char* p_plainText, int p_plainSize,
		char* p_chiperText, int p_chiperSize, char* p_key, PaddingType p_type);

int AES_ECB_Decrypt(char* p_chiperText, int p_chiperSize,
		char* p_plainText, int p_plainSize, char* p_key);

int AES_CBC_Encrypt(char* p_plainText, int p_plainSize, char* p_chiperText,
		int p_chiperSize, char* p_key, char* p_initVector, PaddingType p_type);

int AES_CBC_Decrypt(char* p_chiperText, int p_chiperSize,
		char* p_plainText, int p_plainSize, char* p_key, char* p_initVector);

#ifdef __cplusplus
}
#endif

#endif /* LIBRARY_AES_AES_H_ */
