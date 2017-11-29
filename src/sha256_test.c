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
    printf(" Print SHA256 checksum for the 'input' file.\n");
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
    size_t sum_sz = SHA256_BLOCK_SIZE;

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
    SHA256_CTX ctx;
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
    sha256_init(&ctx);
    sha256_update(&ctx, fbuf, len);
    sha256_final(&ctx, buf);
    free(fbuf);
    return 0;
}

int main(int argc, char *argv[])
{
    int iret, i, c;
    BYTE buf[SHA256_BLOCK_SIZE];
    const char *form = "%02x";
    iret = parse_args(argc, argv);
    if (iret) {
        if (iret == 2)
            iret = 0;
        return iret;
    }
    const char *file = usr_input;
    iret = get_checksum(file, buf);
    if (iret)
        return iret;

    /* output */
    if (out_Upper)
        form = "%02X";
    if (BSD_Style) {
        printf("SHA256 (%s) = ", file);
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
    }
    else {
        if (got_Check) {
            printf("%s: ", file);
            if (memcmp(check, buf, SHA256_BLOCK_SIZE)) {
                printf(" check FAILED!");
                iret = 1;
            }
            else {
                printf(" Valid");
            }
        }
        else {
            for (i = 0; i < SHA256_BLOCK_SIZE; i++) {
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
        for (i = 0; i < SHA256_BLOCK_SIZE; i++) {
            c = buf[i] & 0xff;
            sprintf(EndBuf(cp), form, c);
        }
        sprintf(EndBuf(cp),"  %s", file);
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

/* eof - sha256_test.c */
