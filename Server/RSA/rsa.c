//
// Created by ejbm0110 on 11/2/24.
//

#include "rsa.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int power(int b, int e) {
    if (e == 0) {
        return 1;
    }
    return b * power(b, e - 1);
}

int is_prime(int const num, int const i_num) {
    if (i_num <= 1) {
        return 1;
    } if (num > i_num && num % i_num == 0) {
        return 0;
    }
    return is_prime(num, i_num - 1);
}

int is_coprime(long long a, long long b) {
    while (b != 0) {
        long long temp = b;
        b = a % b;
        a = temp;
    }
    return a == 1 ? 1 : 0;
}

char* ASCIIToMessage(unsigned long long num) {
    int length = 0;
    unsigned long long tmp = num;
    while (tmp > 0) {
        length++;
        tmp /= 256;
    }

    char* message = malloc(length + 1);

    if (!message) return NULL;

    char *ptr = message + length;
    *ptr-- = '\0';

    while (num > 0) {
        *ptr-- = (char) (num % 256);
        num /= 256;
    }

    return message;
}

long long generate_prime(int const bits) {
    int const low_bound = power(2, bits-1);
    int const high_bound = power(2, bits);

    int n_rand;

    do {
        n_rand = low_bound + rand() % (high_bound - low_bound);
    } while (!is_prime(n_rand, n_rand));

    return n_rand;
}

long long generate_coprime(long long n) {
    long long e;
    do {
        e = rand();
    } while (!is_coprime(n, e));
    return e;
}

// Function to perform the Extended Euclidean Algorithm
// It returns the gcd of a and b, and finds x and y such that ax + by = gcd(a, b)
long long extended_gcd(long long a, long long b, long long *x, long long *y) {
    if (b == 0) {
        *x = 1;
        *y = 0;
        return a;
    }
    long long x1, y1;
    long long gcd = extended_gcd(b, a % b, &x1, &y1);
    *x = y1;
    *y = x1 - (a / b) * y1;
    return gcd;
}

// Function to calculate modular inverse of e mod phi
long long mod_inverse(long long e, long long phi) {
    long long x, y;
    long long gcd = extended_gcd(e, phi, &x, &y);
    if (gcd != 1) {
        // Modular inverse doesn't exist if e and phi are not coprime
        return -1; // or handle the error as needed
    }
    // x might be negative, so convert it to a positive value
    return (x % phi + phi) % phi;
}

struct RSAKeyPair* generate_keys() {
    const long long p = generate_prime(16);
    const long long q = generate_prime(16);

    const long long n = p * q;
    const long long phi = (p - 1) * (q - 1);

    const long long e = generate_coprime(phi);

    const long long d = mod_inverse(e, phi);

    struct RSAKeyPair *keys = malloc(sizeof(struct RSAKeyPair));

    keys->public_key = malloc(sizeof(struct RSAKey));
    keys->public_key->modulus = n;
    keys->public_key->exponent = e;

    keys->private_key = malloc(sizeof(struct RSAKey));
    keys->private_key->modulus = n;
    keys->private_key->exponent = d;

    return keys;
}

unsigned long long rsa_decrypt(unsigned long long ciphertext, const struct RSAKey *private_key) {
    unsigned long long result = 1;
    unsigned long long mod = private_key->modulus;
    unsigned long long exp = private_key->exponent;
    ciphertext = ciphertext % mod;

    while (exp > 0) {
        // If exp is odd, multiply base with result
        if (exp % 2 == 1) {
            result = (result * ciphertext) % mod;
        }

        // Square the base and reduce exp by half
        exp = exp >> 1; // equivalent to exp / 2
        ciphertext = (ciphertext * ciphertext) % mod;
    }
    return result;
}