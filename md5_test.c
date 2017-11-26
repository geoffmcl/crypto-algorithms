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
	printf("MD5 tests: %s\n", md5_test() ? "SUCCEEDED" : "FAILED");

	return(0);
}

static const char *module = "md5_test";

static const char *usr_input = 0;
static int out_Upper = 0;
static BYTE check[MD5_BLOCK_SIZE];
static int got_Check = 0;

void give_help(char *name)
{
    printf("%s: usage: [options] usr_input\n", module);
    printf("Options:\n");
    printf(" --help  (-h or -?) = This help and exit(0)\n");
    printf(" --upper       (-u) = Output string in uppercase.\n");
    printf(" --check <hex> (-c) = Compare results to this hex string.\n");
    printf(" --TEST        (-T) = Run the original 'tests' and exit(0)\n");
    printf("\n");
    printf(" Print BSD-style MD5 checksum for the 'input' file.\n");
    // TODO: More help
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
                test_main();
                return 2;
                break;
                // TODO: Other arguments
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

int main(int argc, char *argv[])
{
    int iret, i, c;
    size_t len, sz;
    BYTE *fbuf;
    FILE *fp;
    BYTE buf[MD5_BLOCK_SIZE];
    MD5_CTX ctx;
    const char *form = "%02x";
    iret = parse_args(argc, argv);
    if (iret) {
        if (iret == 2)
            iret = 0;
        return iret;
    }
    const char *file = usr_input;
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
    printf("MD5 (%s) = ", file);
    if (out_Upper)
        form = "%02X";
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
    printf("\n");
    return iret;
}

/* eof - ms5_test.c */
