/*
 * MD5.h
 *
 *  Created on: 2018Äê7ÔÂ20ÈÕ
 *      Author: Administrator
 */

#ifndef CLIENT_MD5_MD5_H_
#define CLIENT_MD5_MD5_H_

#ifdef __cplusplus
extern "C"
{
#endif

/*
 * @function - file Standard md5 calculation, 32 bits
 * @param
 * 	file_path - a File path to calculate md5 value
 * 	md5_str   - Used to store calculation results, At least 33 bytes in length
 * @return value - 0 when success, -1 when failed
 * */
int Compute_file_md5(const char *file_path, char *md5_str);

/*
 * @function - string Standard md5 calculation, 32 bits
 * @param
 * 	str     - a string need to be calculate
 * 	md5_str - Used to store calculation results, At least 33 bytes in length
 * @return value - 0 when success, -1 when failed
 * */
int Compute_string_md5(char *str, char *md5_str);

#ifdef __cplusplus
}
#endif


#endif /* CLIENT_MD5_MD5_H_ */
