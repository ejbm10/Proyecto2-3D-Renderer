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
void rsa_encrypt();
void rsa_decrypt();

#endif //RSA_H
