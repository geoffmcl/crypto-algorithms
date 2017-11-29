/*********************************************************************
* Filename:   sha1_test.c
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
#include "sha1.h"

/*********************** FUNCTION DEFINITIONS ***********************/
int sha1_test()
{
	BYTE text1[] = {"abc"};
	BYTE text2[] = {"abcdbcdecdefdefgefghfghighijhijkijkljklmklmnlmnomnopnopq"};
	BYTE text3[] = {"aaaaaaaaaa"};
	BYTE hash1[SHA1_BLOCK_SIZE] = {0xa9,0x99,0x3e,0x36,0x47,0x06,0x81,0x6a,0xba,0x3e,0x25,0x71,0x78,0x50,0xc2,0x6c,0x9c,0xd0,0xd8,0x9d};
	BYTE hash2[SHA1_BLOCK_SIZE] = {0x84,0x98,0x3e,0x44,0x1c,0x3b,0xd2,0x6e,0xba,0xae,0x4a,0xa1,0xf9,0x51,0x29,0xe5,0xe5,0x46,0x70,0xf1};
	BYTE hash3[SHA1_BLOCK_SIZE] = {0x34,0xaa,0x97,0x3c,0xd4,0xc4,0xda,0xa4,0xf6,0x1e,0xeb,0x2b,0xdb,0xad,0x27,0x31,0x65,0x34,0x01,0x6f};
	BYTE buf[SHA1_BLOCK_SIZE];
	int idx;
	SHA1_CTX ctx;
	int pass = 1;

	sha1_init(&ctx);
	sha1_update(&ctx, text1, strlen(text1));
	sha1_final(&ctx, buf);
	pass = pass && !memcmp(hash1, buf, SHA1_BLOCK_SIZE);

	sha1_init(&ctx);
	sha1_update(&ctx, text2, strlen(text2));
	sha1_final(&ctx, buf);
	pass = pass && !memcmp(hash2, buf, SHA1_BLOCK_SIZE);

	sha1_init(&ctx);
	for (idx = 0; idx < 100000; ++idx)
	   sha1_update(&ctx, text3, strlen(text3));
	sha1_final(&ctx, buf);
	pass = pass && !memcmp(hash3, buf, SHA1_BLOCK_SIZE);

	return(pass);
}

int test_main()
{
	printf("SHA1 tests: %s\n", sha1_test() ? "SUCCEEDED" : "FAILED");

	return(0);
}

static const char *module = "sha1_test";

static const char *usr_input = 0;
static int out_Upper = 0;
static BYTE check[SHA1_BLOCK_SIZE];
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
    printf(" --tag          (-t) = Output a BSD-style checksum.\n");
    printf(" --out <file>   (-o) = Write checksum to this file.\n");
    printf(" --version      (-v) = Show help, utils version, and exit(0)\n");
    printf("\n");
    printf(" Print SHA1 checksum for the 'input' file.\n");
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
    size_t sum_sz = SHA1_BLOCK_SIZE;

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
                    if (len != (SHA1_BLOCK_SIZE * 2)) {
                        printf("%s: Error: Expected SHA1 hex checksum of length %u, not %u\n", module,
                            (int)(SHA1_BLOCK_SIZE * 2), (int)len);
                        return 1;
                    }
                    for (ii = 0, len = 0; ii < SHA1_BLOCK_SIZE; ii++, len += 2) {
                        c = hex_to_val(sarg[len]);
                        if (c == -1) {
                            printf("%s: Error: SHA1 checksum contains invalid value %u\n", module, c);
                            return 1;
                        }
                        check[ii] = c * 16;
                        c = hex_to_val(sarg[len + 1]);
                        if (c == -1) {
                            printf("%s: Error: SHA1 checksum contains invalid value %u\n", module, c);
                            return 1;
                        }
                        check[ii] += c;
                    }
                    got_Check = 1;
                }
                else {
                    printf("%s: Error: Expected SHA1 hex checksum to follow '%s'\n", module, arg);
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
    SHA1_CTX ctx;
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
    sha1_init(&ctx);
    sha1_update(&ctx, fbuf, len);
    sha1_final(&ctx, buf);
    free(fbuf);
    return 0;
}

int main(int argc, char *argv[])
{
    int iret, i, c;
    BYTE buf[SHA1_BLOCK_SIZE];
    const char *form = "%02x";
    const char *file;
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
        printf("SHA1 (%s) = ", file);
        for (i = 0; i < SHA1_BLOCK_SIZE; i++) {
            c = buf[i] & 0xff;
            printf(form, c);
        }
        if (got_Check) {
            if (memcmp(check, buf, SHA1_BLOCK_SIZE)) {
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
            if (memcmp(check, buf, SHA1_BLOCK_SIZE)) {
                printf(" check FAILED!");
                iret = 1;
            }
            else {
                printf(" Valid");
            }
        }
        else {
            for (i = 0; i < SHA1_BLOCK_SIZE; i++) {
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
        for (i = 0; i < SHA1_BLOCK_SIZE; i++) {
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

/* eof - sha1_test.c */
