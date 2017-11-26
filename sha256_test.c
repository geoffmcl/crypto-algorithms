/*********************************************************************
* Filename:   sha256.c
* Author:     Brad Conte (brad AT bradconte.com)
* Copyright:
* Disclaimer: This code is presented "as is" without any guarantees.
* Details:    Performs known-answer tests on the corresponding SHA1
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
#include "sha256.h"

/*********************** FUNCTION DEFINITIONS ***********************/
int sha256_test()
{
	BYTE text1[] = {"abc"};
	BYTE text2[] = {"abcdbcdecdefdefgefghfghighijhijkijkljklmklmnlmnomnopnopq"};
	BYTE text3[] = {"aaaaaaaaaa"};
	BYTE hash1[SHA256_BLOCK_SIZE] = {0xba,0x78,0x16,0xbf,0x8f,0x01,0xcf,0xea,0x41,0x41,0x40,0xde,0x5d,0xae,0x22,0x23,
	                                 0xb0,0x03,0x61,0xa3,0x96,0x17,0x7a,0x9c,0xb4,0x10,0xff,0x61,0xf2,0x00,0x15,0xad};
	BYTE hash2[SHA256_BLOCK_SIZE] = {0x24,0x8d,0x6a,0x61,0xd2,0x06,0x38,0xb8,0xe5,0xc0,0x26,0x93,0x0c,0x3e,0x60,0x39,
	                                 0xa3,0x3c,0xe4,0x59,0x64,0xff,0x21,0x67,0xf6,0xec,0xed,0xd4,0x19,0xdb,0x06,0xc1};
	BYTE hash3[SHA256_BLOCK_SIZE] = {0xcd,0xc7,0x6e,0x5c,0x99,0x14,0xfb,0x92,0x81,0xa1,0xc7,0xe2,0x84,0xd7,0x3e,0x67,
	                                 0xf1,0x80,0x9a,0x48,0xa4,0x97,0x20,0x0e,0x04,0x6d,0x39,0xcc,0xc7,0x11,0x2c,0xd0};
	BYTE buf[SHA256_BLOCK_SIZE];
	SHA256_CTX ctx;
	int idx;
	int pass = 1;

	sha256_init(&ctx);
	sha256_update(&ctx, text1, strlen(text1));
	sha256_final(&ctx, buf);
	pass = pass && !memcmp(hash1, buf, SHA256_BLOCK_SIZE);

	sha256_init(&ctx);
	sha256_update(&ctx, text2, strlen(text2));
	sha256_final(&ctx, buf);
	pass = pass && !memcmp(hash2, buf, SHA256_BLOCK_SIZE);

	sha256_init(&ctx);
	for (idx = 0; idx < 100000; ++idx)
	   sha256_update(&ctx, text3, strlen(text3));
	sha256_final(&ctx, buf);
	pass = pass && !memcmp(hash3, buf, SHA256_BLOCK_SIZE);

	return(pass);
}

int test_main()
{
	printf("SHA-256 tests: %s\n", sha256_test() ? "SUCCEEDED" : "FAILED");

	return(0);
}

static const char *module = "sha256_test";

static const char *usr_input = 0;
static int out_Upper = 0;
static BYTE check[SHA256_BLOCK_SIZE];
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
    printf(" Print BSD-style SHA256 checksum for the 'input' file.\n");
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
                    if (len != (SHA256_BLOCK_SIZE * 2)) {
                        printf("%s: Error: Expected SHA256 hex checksum of length %u, not %u\n", module, 
                            (int)(SHA256_BLOCK_SIZE * 2), (int)len);
                        return 1;
                    }
                    for (ii = 0, len = 0; ii < SHA256_BLOCK_SIZE; ii++, len += 2) {
                        c = hex_to_val(sarg[len]);
                        if (c == -1) {
                            printf("%s: Error: SHA256 checksum contains invalid value %u\n", module, c);
                            return 1;
                        }
                        check[ii] = c * 16;
                        c = hex_to_val(sarg[len+1]);
                        if (c == -1) {
                            printf("%s: Error: SHA256 checksum contains invalid value %u\n", module, c);
                            return 1;
                        }
                        check[ii] += c;
                    }
                    got_Check = 1;
                }
                else {
                    printf("%s: Error: Expected SHA256 hex checksum to follow '%s'\n", module, arg);
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
    BYTE buf[SHA256_BLOCK_SIZE];
    SHA256_CTX ctx;
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
    fbuf = (BYTE *)malloc(len+1);
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
    sha256_init(&ctx);
    sha256_update(&ctx, fbuf, len);
    sha256_final(&ctx, buf);
    free(fbuf);
    printf("SHA256 (%s) = ", file);
    if (out_Upper)
        form = "%02X";
    for (i = 0; i < SHA256_BLOCK_SIZE; i++) {
        c = buf[i] & 0xff;
        printf(form, c);
    }
    if (got_Check) {
        if (memcmp(check, buf, SHA256_BLOCK_SIZE)) {
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

/* eof - sha256_test.c */
