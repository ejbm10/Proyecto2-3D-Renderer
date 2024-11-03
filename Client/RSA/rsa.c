//
// Created by ejbm0110 on 11/2/24.
//

#include <stdio.h>
#include <stdlib.h>
#include "rsa.h"
#include <string.h>

unsigned long long messageToASCII(const char *str) {
    unsigned long long num = 0;
    size_t len = strlen(str);

    // Combine ASCII values into a single integer
    for (size_t i = 0; i < len; i++) {
        num = num * 256 + (unsigned char)str[i]; // Shift and add the ASCII value
    }

    return num;
}

unsigned long long rsa_encrypt(unsigned long long message, const struct RSAKey *public_key) {
    unsigned long long result = 1;
    unsigned long long mod = public_key->modulus;
    unsigned long long exp = public_key->exponent;
    message = message % mod;

    while (exp > 0) {
        // If exp is odd, multiply base with result
        if (exp % 2 == 1) {
            result = (result * message) % mod;
        }

        // Square the base and reduce exp by half
        exp = exp >> 1; // equivalent to exp / 2
        message = (message * message) % mod;
    }
    return result;
}