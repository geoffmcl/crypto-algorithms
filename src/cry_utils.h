/*\
 * cry_utils.h
 *
 * Copyright (c) 2017 - Geoff R. McLane
 * Licence: GNU GPL version 2
 *
\*/

#ifndef _CRY_UTILS_H_
#define _CRY_UTILS_H_

// add body
extern int hex_to_val(const char ch);

#ifdef _MSC_VER
#define M_IS_DIR _S_IFDIR
#else // !_MSC_VER
#define M_IS_DIR S_IFDIR
#endif

#define MDT_NONE 0
#define MDT_FILE 1
#define MDT_DIR  2

extern int is_file_or_directory(const char * path);
extern size_t get_last_file_size();


#endif // #ifndef _CRY_UTILS_H_
// eof - cry_utils.h
