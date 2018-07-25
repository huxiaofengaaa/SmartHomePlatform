#ifndef LIBRARY_AES_AES_H_
#define LIBRARY_AES_AES_H_

#ifdef __cplusplus
extern "C"
{
#endif

int AES128Encrypt(char *p_plainText, int p_plainSize, char* p_chiperText,
		int p_chiperSize, char *key);

int AES128Decrypt(char* p_chiperText, int p_chiperSize, char* p_plainText,
		int p_plainSize, char* key);

int AES128Encrypt2(const char *p_plainText, int p_plainSize, char* p_chiperText,
		int p_chiperSize, const char *p_key);

char subByte(char p_byte);

#ifdef __cplusplus
}
#endif

#endif /* LIBRARY_AES_AES_H_ */
