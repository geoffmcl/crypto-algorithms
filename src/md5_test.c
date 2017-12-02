/*********************************************************************
* Filename:   md5_test.c
* Author:     Brad Conte (brad AT bradconte.com)
* Copyright:
* Disclaimer: This code is presented "as is" without any guarantees.
* Details:    Performs known-answer tests on the corresponding MD5
	          implementation. These tests do not encompass the full
	          range of available test vectors, however, if the tests
	          pass it is very, very likely that the code is correct
	          and was compiled properly. This code also serves as
	          example usage of the functions.
*********************************************************************/

/*************************** HEADER FILES ***************************/
#include <stdio.h>
#include <stdlib.h> /* for malloc, ... */
#include <memory.h>
#include <string.h>
#include "cry_utils.h"
#include "md5.h"

/*********************** FUNCTION DEFINITIONS ***********************/
int md5_test()
{
	BYTE text1[] = {""};
	BYTE text2[] = {"abc"};
	BYTE text3_1[] = {"ABCDEFGHIJKLMNOPQRSTUVWXYZabcde"};
	BYTE text3_2[] = {"fghijklmnopqrstuvwxyz0123456789"};
	BYTE hash1[MD5_BLOCK_SIZE] = {0xd4,0x1d,0x8c,0xd9,0x8f,0x00,0xb2,0x04,0xe9,0x80,0x09,0x98,0xec,0xf8,0x42,0x7e};
	BYTE hash2[MD5_BLOCK_SIZE] = {0x90,0x01,0x50,0x98,0x3c,0xd2,0x4f,0xb0,0xd6,0x96,0x3f,0x7d,0x28,0xe1,0x7f,0x72};
	BYTE hash3[MD5_BLOCK_SIZE] = {0xd1,0x74,0xab,0x98,0xd2,0x77,0xd9,0xf5,0xa5,0x61,0x1c,0x2c,0x9f,0x41,0x9d,0x9f};
	BYTE buf[16];
	MD5_CTX ctx;
	int pass = 1;

	md5_init(&ctx);
	md5_update(&ctx, text1, strlen(text1));
	md5_final(&ctx, buf);
	pass = pass && !memcmp(hash1, buf, MD5_BLOCK_SIZE);

	// Note the MD5 object can be reused.
	md5_init(&ctx);
	md5_update(&ctx, text2, strlen(text2));
	md5_final(&ctx, buf);
	pass = pass && !memcmp(hash2, buf, MD5_BLOCK_SIZE);

	// Note the data is being added in two chunks.
	md5_init(&ctx);
	md5_update(&ctx, text3_1, strlen(text3_1));
	md5_update(&ctx, text3_2, strlen(text3_2));
	md5_final(&ctx, buf);
	pass = pass && !memcmp(hash3, buf, MD5_BLOCK_SIZE);

	return(pass);
}

int test_main()
{
    int pass = md5_test();
	printf("MD5 tests: %s\n", pass ? "SUCCEEDED" : "FAILED");
	return (pass ? 0 : 1);
}

static const char *module = "md5_test";

static const char *usr_input = 0;
static int out_Upper = 0;
static BYTE check[MD5_BLOCK_SIZE];
static int got_Check = 0;
static int BSD_Style = 0;
static char *out_file = 0;
static char out_buf[1024];

void give_help(char *name)
{
    printf("%s: usage: [options] usr_input\n", module);
    printf("%s: %s\n", get_cry_utils_module(), get_cry_utils_version());
    printf("Options:\n");
    printf(" --help   (-h or -?) = This help and exit(0)\n");
    printf(" --upper        (-u) = Output string in uppercase.\n");
    printf(" --CHECK <hex>  (-C) = Compare results to this hex string.\n");
    printf(" --check <file> (-c) = Get checksum to validate and input file from file.\n");
    printf(" --TEST         (-T) = Run the original 'tests' and exit(0)\n");
    printf(" --tag          (-t) = Create a BSD-style checksum.\n");
    printf(" --out <file>   (-o) = Write checksum to this file.\n");
    printf(" --version      (-v) = Show help, utils version, and exit(0)\n");
    printf("\n");
    printf(" Print MD5 checksum for the 'input' file.\n");
    // TODO: More help
}

/*
int get_check_file(const char *file)
Sort of an 'input' files, that gives
(a) checksum to CHECK against calculated
(b) filename as the input file
Order presently has to be 'checksum-in-hex  filename-to-CHECK'
*/
int get_check_file(const char *file)
{
    size_t len, sz, ii, off;
    FILE *fp;
    BYTE *fbuf;
    char *cp = NULL;
    int c;
    size_t sum_sz = MD5_BLOCK_SIZE;

    if (usr_input) {
        printf("%s: Already have input file '%s'! Can not process a 'check' file '%s'?\n", module, usr_input, file);
        return 1;
    }

    if (is_file_or_directory(file) != MDT_FILE) {
        printf("Error: Can NOT stat file '%s'\n", file);
        return 1;
    }
    off = get_last_file_size();
    if (off < ((sum_sz * 2) + 2)) {
        printf("Error: Size %u insufficient to contain checksum and file name!\n", (int)off);
        return 1;
    }
    fp = fopen(file, "rb");
    if (!fp) {
        printf("Error: Can NOT open file '%s'\n", file);
        return 1;
    }
    fbuf = (BYTE *)malloc(off + 1);
    if (!fbuf) {
        fclose(fp);
        printf("Error: Memory failed on %u bytes\n", (int)off);
        return 1;
    }
    memset(fbuf, 0, off + 1);
    sz = fread(fbuf, 1, off, fp);
    fclose(fp);
    if (sz != off) {
        free(fbuf);
        printf("Error: Read of file '%s' failed!\n", file);
        return 1;
    }
    while (off) {
        off--;
        c = fbuf[off];
        if (c > ' ') {
            off++;
            break;
        }
        fbuf[off] = 0;
    }
    if (off < ((sum_sz * 2) + 2)) {
        free(fbuf);
        printf("Error: File has insufficient characters %u to contain checksum and file name!\n", (int)off);
        return 1;
    }
    // fish around to find 'checksum file_name', in that order
    for (ii = 0, len = 0; ii < sum_sz; ii++, len += 2) {
        c = hex_to_val(fbuf[len]);
        if (c == -1) {
            c = fbuf[len];
            printf("%s: Error: checksum contains invalid char value %u\n", module, c);
            return 1;
        }
        check[ii] = c * 16;
        c = hex_to_val(fbuf[len + 1]);
        if (c == -1) {
            c = fbuf[len];
            printf("%s: Error: checksum contains invalid char value %u\n", module, c);
            return 1;
        }
        check[ii] += c;
    }
    // done leading checksum
    // get to first filename character
    for (; len < off; len++) {
        c = fbuf[len];
        if (c > ' ') {
            cp = &fbuf[len];    // set beginning of filename
            break;
        }
    }
    if (!cp) {
        free(fbuf);
        printf("Error: Could NOT find input file name in '%s'\n", file);
    }
    // ok cp should be the input file
    usr_input = strdup(cp);
    free(fbuf);
    got_Check = 1;
    return 0;
}

int parse_args(int argc, char **argv)
{
    int i, i2, c;
    char *arg, *sarg;
    size_t len, ii;
    for (i = 1; i < argc; i++) {
        arg = argv[i];
        i2 = i + 1;
        if (*arg == '-') {
            sarg = &arg[1];
            while (*sarg == '-')
                sarg++;
            c = *sarg;
            switch (c) {
            case 'h':
            case '?':
            case 'v':
                give_help(argv[0]);
                return 2;
                break;
            case 'u':
                out_Upper = 1;
                break;
            case 'c':
                if (i2 < argc) {
                    i++;
                    sarg = argv[i];
                    if (get_check_file(sarg))
                        return 1;
                }
                else {
                    printf("%s: Error: Expected check file to follow '%s'\n", module, arg);
                    return 1;
                }
                break;
            case 'C':
                if (i2 < argc) {
                    i++;
                    sarg = argv[i];
                    len = strlen(sarg);
                    if (len != (MD5_BLOCK_SIZE * 2)) {
                        printf("%s: Error: Expected MD5 hex checksum of length %u, not %u\n", module,
                            (int)(MD5_BLOCK_SIZE * 2), (int)len);
                        return 1;
                    }
                    for (ii = 0, len = 0; ii < MD5_BLOCK_SIZE; ii++, len += 2) {
                        c = hex_to_val(sarg[len]);
                        if (c == -1) {
                            printf("%s: Error: MD5 checksum contains invalid value %u\n", module, c);
                            return 1;
                        }
                        check[ii] = c * 16;
                        c = hex_to_val(sarg[len + 1]);
                        if (c == -1) {
                            printf("%s: Error: MD5 checksum contains invalid value %u\n", module, c);
                            return 1;
                        }
                        check[ii] += c;
                    }
                    got_Check = 1;
                }
                else {
                    printf("%s: Error: Expected MD5 hex checksum to follow '%s'\n", module, arg);
                    return 1;
                }

                break;
            case 'T':
                c = test_main();
                return (c ? 1 : 2);
                break;
            case 't':
                BSD_Style = 1;
                break;
            case 'o':
                if (i2 < argc) {
                    i++;
                    sarg = argv[i];
                    out_file = strdup(sarg);
                }
                else {
                    printf("%s: Error: Expected out file name to follow '%s'\n", module, arg);
                    return 1;
                }
                break;
            default:
                printf("%s: Unknown argument '%s'. Try -? for help...\n", module, arg);
                return 1;
            }
        }
        else {
            // bear argument
            if (usr_input) {
                printf("%s: Already have input '%s'! What is this '%s'?\n", module, usr_input, arg);
                return 1;
            }
            usr_input = strdup(arg);
        }
    }
    if (!usr_input) {
        printf("%s: No user input found in command!\n", module);
        return 1;
    }
    return 0;
}

int get_checksum(const char *file, BYTE *buf)
{
    size_t len, sz;
    FILE *fp;
    BYTE *fbuf;
    MD5_CTX ctx;
    if (is_file_or_directory(file) != MDT_FILE) {
        printf("Error: Can NOT stat file '%s'\n", file);
        return 1;
    }
    len = get_last_file_size();
    fp = fopen(file, "rb");
    if (!fp) {
        printf("Error: Can NOT open file '%s'\n", file);
        return 1;
    }
    fbuf = (BYTE *)malloc(len + 1);
    if (!fbuf) {
        fclose(fp);
        printf("Error: Memory failed on %u bytes\n", (int)len);
        return 1;
    }
    sz = fread(fbuf, 1, len, fp);
    fclose(fp);
    if (sz != len) {
        free(fbuf);
        printf("Error: Read of file '%s' failed!\n", file);
        return 1;
    }
    md5_init(&ctx);
    md5_update(&ctx, fbuf, len);
    md5_final(&ctx, buf);
    free(fbuf);
    return 0;
}


int main(int argc, char *argv[])
{
    int iret, i, c;
    BYTE buf[MD5_BLOCK_SIZE];
    const char *file;
    const char *form = "%02x";
    iret = parse_args(argc, argv);
    if (iret) {
        if (iret == 2)
            iret = 0;
        return iret;
    }
    file = usr_input;
    iret = get_checksum(file, buf);
    if (iret)
        return iret;

    /* output */
    if (out_Upper)
        form = "%02X";
    if (BSD_Style) {
        printf("MD5 (%s) = ", file);
        for (i = 0; i < MD5_BLOCK_SIZE; i++) {
            c = buf[i] & 0xff;
            printf(form, c);
        }
        if (got_Check) {
            if (memcmp(check, buf, MD5_BLOCK_SIZE)) {
                printf(" check FAILED!");
                iret = 1;
            }
            else {
                printf(" Valid");
            }
        }
    }
    else {
        if (got_Check) {
            printf("%s: ", file);
            if (memcmp(check, buf, MD5_BLOCK_SIZE)) {
                printf(" check FAILED!");
                iret = 1;
            }
            else {
                printf(" Valid");
            }
        }
        else {
            for (i = 0; i < MD5_BLOCK_SIZE; i++) {
                c = buf[i] & 0xff;
                printf(form, c);
            }
            printf("  %s", file);
        }
    }
    printf("\n");
    if (out_file) {
        char *cp = out_buf;
        FILE *fp;
        *cp = 0;
        for (i = 0; i < MD5_BLOCK_SIZE; i++) {
            c = buf[i] & 0xff;
            sprintf(EndBuf(cp), form, c);
        }
        sprintf(EndBuf(cp), "  %s", file);
        i = (int)strlen(cp);
        fp = fopen(out_file, "wb");
        if (fp) {
            c = (int)fwrite(cp, 1, i, fp);
            fclose(fp);
            if (c == i) {
                printf("%s: Checksum written to output '%s'!\n", module, out_file);
            }
            else {
                printf("%s: Error: Failed writting output '%s'!\n", module, out_file);
                iret = 1;
            }
        }
        else {
            printf("%s: Error: Can NOT open output '%s'!\n", module, out_file);
            iret = 1;
        }

    }
    return iret;
}

/* eof - ms5_test.c */
