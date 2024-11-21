//
// Created by ejbm0110 on 11/2/24.
//

#ifndef RSA_H
#define RSA_H

struct RSAKey {
    unsigned long long modulus;
    unsigned long long exponent;
};

unsigned long long messageToASCII(const char *str, int block, struct RSAKey *key);
unsigned long long rsa_encrypt(unsigned long long message, const struct RSAKey *public_key);

#endif //RSA_H
