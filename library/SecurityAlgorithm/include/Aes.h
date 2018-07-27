#ifndef LIBRARY_AES_AES_H_
#define LIBRARY_AES_AES_H_

#ifdef __cplusplus
extern "C"
{
#endif

typedef enum
{
	AES_SUCCESS          = 0,
	AES_ERROR_KEY        = -1001,
	AES_ERROR_INITVECTOR = -1002,
	AES_ERROR_PLAINTEXT  = -1003,
	AES_ERROR_CHIPERTEXT = -1004
}AES_STATUS;

typedef enum
{
	PKCS5Padding,
	PKCS7Padding
}PaddingType;

int AES128Encrypt2(const char *p_plainText, const int p_plainSize, char* p_chiperText,
		const int p_chiperSize, const char *p_key);

int AES128Decrypt2(const char *p_chiperText, const int p_chiperSize, char* p_plainText,
		const int p_plainSize, const char *p_key);

AES_STATUS AES_CBC_Encrypt(const char* p_plainText, const int p_plainSize, char* p_chiperText,
		const int p_chiperSize, const char* p_key, const char* p_initVector, PaddingType p_type);

AES_STATUS AES_CBC_Decrypt(const char* p_chiperText, const int p_chiperSize,
		char* p_plainText, const int p_plainSize, const char* p_key, const char* p_initVector);

#ifdef __cplusplus
}
#endif

#endif /* LIBRARY_AES_AES_H_ */
