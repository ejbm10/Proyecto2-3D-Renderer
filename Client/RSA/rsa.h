//
// Created by ejbm0110 on 11/2/24.
//

#ifndef RSA_H
#define RSA_H

struct RSAKey {
    long long modulus;
    long long exponent;
};

unsigned long long messageToASCII(const char *str);
unsigned long long rsa_encrypt(unsigned long long message, const struct RSAKey *public_key);

#endif //RSA_H
