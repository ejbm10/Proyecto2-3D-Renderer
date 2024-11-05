//
// Created by ejbm0110 on 11/2/24.
//

#include <stdio.h>
#include <stdlib.h>
#include "rsa.h"
#include <string.h>

unsigned long long messageToASCII(const char *str, int block, struct RSAKey *key) {
    unsigned long long ascii_num = 0, previous_ascii_num = 0;
    size_t len = strlen(str);
    int current_block = 0;

    for (size_t i = 0; i < len; i++) {
        previous_ascii_num = ascii_num; // Store the last valid block value
        ascii_num = ascii_num * 256 + (unsigned char)str[i];

        // Check if the current ascii_num exceeds or matches the modulus
        if (ascii_num >= key->modulus) {
            if (block == current_block) {  // If we're at the requested block, return the last valid value
                return previous_ascii_num;
            }
            // Reset for the next block and increment block counter
            ascii_num = (unsigned char)str[i]; // Start the new block with the current character
            current_block++;
        }
    }

    // Final block handling: if block matches, return the last valid ascii_num
    return (block == -1) ? current_block + 1 : ascii_num;
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