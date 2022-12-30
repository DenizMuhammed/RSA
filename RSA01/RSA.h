
#ifndef _RSA_H
#define _RSA_H
#include <cstdint>

namespace R
{
uint64_t P; // prime 1
uint64_t Q; // prime 2
uint64_t T; // totient
uint64_t N; // modulus
uint64_t E; // public key
uint64_t D; // private key;


void RSAConstructor(uint64_t &P, uint64_t &Q, uint64_t &N);


}
#endif