#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>
#include "skinny.h"


static const unsigned char check[] = {
    /* Known Answer Test A */
    0xdf, 0x88, 0x95, 0x48, 0xcf, 0xc7, 0xea, 0x52, 0xd2, 0x96, 0x33, 0x93, 0x01, 0x79, 0x74, 0x49, /* tweakey */
    0xab, 0x58, 0x8a, 0x34, 0xa4, 0x7f, 0x1a, 0xb2, 0xdf, 0xe9, 0xc8, 0x29, 0x3f, 0xbe, 0xa9, 0xa5,
    0xab, 0x1a, 0xfa, 0xc2, 0x61, 0x10, 0x12, 0xcd, 0x8c, 0xef, 0x95, 0x26, 0x18, 0xc3, 0xeb, 0xe8,
    0xa3, 0x99, 0x4b, 0x66, 0xad, 0x85, 0xa3, 0x45, 0x9f, 0x44, 0xe9, 0x2b, 0x08, 0xf5, 0x50, 0xcb, /* plaintext */
    0x94, 0xec, 0xf5, 0x89, 0xe2, 0x01, 0x7c, 0x60, 0x1b, 0x38, 0xc6, 0x34, 0x6a, 0x10, 0xdc, 0xfa, /* ciphertext */
    /* Known Answer Test B */
    0xd1, 0xe4, 0xbe, 0xe2, 0xf0, 0x04, 0xc7, 0x61, 0xd8, 0x28, 0x20, 0x9e, 0xdb, 0x60, 0x6b, 0x38, /* tweakey */
    0xe5, 0xc0, 0x1e, 0xcc, 0x1f, 0x31, 0xc4, 0xf0, 0xa6, 0x25, 0x84, 0x2f, 0x27, 0x24, 0x6e, 0x03,
    0x94, 0x9a, 0x8d, 0xfd, 0xad, 0xc4, 0x85, 0xa1, 0xa8, 0x11, 0x77, 0x2a, 0x83, 0xed, 0xcd, 0xdc,
    0x60, 0xdf, 0xd0, 0x80, 0x30, 0xbc, 0xcb, 0x08, 0x8c, 0xbc, 0x39, 0xeb, 0x47, 0x3a, 0xc7, 0x55, /* plaintext */
    0x69, 0x95, 0xdb, 0xea, 0x1e, 0xcd, 0x11, 0x57, 0xc8, 0x60, 0xae, 0x0f, 0x63, 0x6e, 0xf8, 0x8f, /* ciphertext */
    /* Known Answer Test C */
    0xbf, 0xf6, 0x23, 0xc8, 0xba, 0xea, 0x62, 0x02, 0x5e, 0xd0, 0x24, 0x11, 0xe4, 0x59, 0x09, 0x4d, /* tweakey */
    0xbe, 0xe5, 0xb9, 0xab, 0xda, 0xd0, 0x93, 0x66, 0xc7, 0x1c, 0xbb, 0xd5, 0x5f, 0x20, 0xce, 0x59,
    0x67, 0x47, 0xe5, 0xaa, 0x91, 0xa0, 0x23, 0xc0, 0xf7, 0x9c, 0x0d, 0x15, 0x9a, 0x62, 0xd9, 0x73,
    0x31, 0xbb, 0x38, 0x76, 0x0b, 0xa9, 0x38, 0xc2, 0x72, 0x54, 0xa3, 0x5a, 0x93, 0x51, 0x4e, 0x5e, /* plaintext */
    0x8c, 0xc5, 0xaf, 0x59, 0x0d, 0x4f, 0xf1, 0x1b, 0x6e, 0xa2, 0x6d, 0xec, 0x43, 0xc6, 0xa6, 0xb7, /* ciphertext */
    /* Known Answer Test D */
    0x46, 0xe0, 0xb5, 0xdf, 0x98, 0xdb, 0x4e, 0x91, 0xd0, 0x16, 0x3a, 0x9a, 0xd2, 0x19, 0x29, 0x49, /* tweakey */
    0x8a, 0x90, 0xd1, 0xdf, 0x7a, 0xe1, 0xca, 0x44, 0xbf, 0x1e, 0x1e, 0x7f, 0x0b, 0x2f, 0xaf, 0xed,
    0x28, 0xdf, 0xa0, 0xd5, 0xc5, 0x46, 0x4f, 0x9d, 0x27, 0x41, 0xc2, 0x4b, 0x69, 0x65, 0x16, 0x26,
    0x05, 0x7e, 0x70, 0xdc, 0x85, 0xd0, 0xcc, 0xd1, 0xc0, 0x8e, 0x84, 0x7a, 0x46, 0xcf, 0x86, 0xa6, /* plaintext */
    0x85, 0x19, 0x1e, 0x7c, 0xa0, 0xcf, 0xfa, 0x9b, 0xaf, 0x15, 0xa7, 0xdf, 0x09, 0x6c, 0x94, 0x87  /* ciphertext */
};

int main(int argc, char *argv[]) {
    int i;
    const unsigned char *p = NULL;
    unsigned char c[16];

    (void)argc;
    (void)argv;
    p = check;
    const unsigned char * v = p+48;
    /* run all 4 KATs */
    for (i = 0; i < 4; i++) {
        skinny(c, p + 48, p);
        printf("%s\n", !memcmp(c, p + 64, 16) ? "PASS" : "FAIL");
        p += 80;
    }

    return 0;
}
