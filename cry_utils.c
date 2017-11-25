/*\
 * cry_utils.c
 *
 * Copyright (c) 2017 - Geoff R. McLane
 * Licence: GNU GPL version 2
 *
\*/

#include <sys/types.h>
#include <sys/stat.h>
#include <stdio.h>
#include "cry_utils.h"

static const char *module = "cry_utils";

// implementation
/* Function which converts a hexadecimal digit character to its integer value */
int hex_to_val(const char ch)
{
    if (ch >= '0' && ch <= '9')
        return ch - '0';  /* Simple ASCII arithmetic */
    else if (ch >= 'a' && ch <= 'f')
        return 10 + ch - 'a';  /* Because hex-digit a is ten */
    else if (ch >= 'A' && ch <= 'F')
        return 10 + ch - 'A';  /* Because hex-digit A is ten */
    else
        return -1;  /* Not a valid hexadecimal digit */
}

/* stat a file or directory */
static struct stat buf;
int is_file_or_directory(const char * path)
{
    if (!path)
        return MDT_NONE;
    if (stat(path, &buf) == 0)
    {
        if (buf.st_mode & M_IS_DIR)
            return MDT_DIR;
        else
            return MDT_FILE;
    }
    return MDT_NONE;
}

size_t get_last_file_size()
{
    return buf.st_size;
}


// eof = cry_utils.c
