//
// Created by ejbm0110 on 11/2/24.
//

#ifndef RSA_H
#define RSA_H

struct RSAKeyPair {
    struct RSAKey *public_key;
    struct RSAKey *private_key;
};

struct RSAKey {
    long long modulus;
    long long exponent;
};

struct RSAKeyPair* generate_keys();
char* ASCIIToMessage(unsigned long long num);
unsigned long long rsa_decrypt(unsigned long long ciphertext, const struct RSAKey *private_key);

#endif //RSA_H
