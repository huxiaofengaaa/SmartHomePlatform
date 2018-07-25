#ifndef LIBRARY_SECURITYALGORITHM_INCLUDE_AESCBC_H_
#define LIBRARY_SECURITYALGORITHM_INCLUDE_AESCBC_H_

#ifdef __cplusplus
extern "C"
{
#endif

int AesCbcPKCS5PaddingEncrypt(const char* p_plainText, const int p_plainSize, char* key);

#ifdef __cplusplus
}
#endif

#endif /* LIBRARY_SECURITYALGORITHM_INCLUDE_AESCBC_H_ */
