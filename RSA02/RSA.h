#ifndef _RSA_H
#define _RSA_H

#include <iostream>
#include <string>

#define byteNumber 8
#define paddingChar '\0'

typedef uint64_t bigInt;


class Message
{
   public:
  bigInt *plainTextArray = nullptr;
  std::string plainTextString;

  bigInt *EncodeTextArray = new bigInt [500] {0} ;
  size_t EncodeTextArrayLength = 0;

  bigInt *DecodeTextArray = new bigInt [500] {0} ;
  size_t DecodeTextArrayLength = 0;

  bigInt *EncryptionTextArray = new bigInt [500] {0} ;
  size_t EncryptionTextArrayLength = 0;

  bigInt *DecryptionTextArray = new bigInt [500] {0};
  size_t DecryptionTextArrayLength = 0;



  std::string DecodeText;
 

  Message(); // Constructed Method

};

class RSA
{
   public:
  typedef struct RSAP // RSA Primitive
  {
   bigInt prime1; // P
   bigInt prime2; // Q
   bigInt totient; // T
   bigInt modulus; // N
   bigInt pubKey; // E
   bigInt priKey; // D

   bigInt &pubExponent = pubKey;
   bigInt &priExponent = priKey;
   
    
  }RSAP_t;

  typedef struct RSACRT // RSA Chinese Remainder Theorem
  {
  // çin kalan teoremi implementasyonu için
  }RSACRT_t;
  
  RSAP_t *RSAPElement = nullptr;
  RSACRT_t *RSACRTElement = nullptr;

  Message mesaj;

 public:

   int status = 0;

     RSA();
    ~RSA();

    bool RSAConstructor(void);

    bool generatePubKey(bigInt &pubKey);
    bool generatePriKey(bigInt &priKey);

    bool RSAEncoding();
    bool RSADecoding();

    bigInt RSAEncryption();
    bigInt RSAEP(bigInt element);

    bigInt RSADecryption();
    bigInt RSADP(bigInt element);


}; // END OF CLASS

static bool isPrime (bigInt prime)
{
    if(prime<3) return false;

    for(bigInt cnt = 3; cnt < prime/2 + 1; cnt++)
    {
       if(prime % cnt == 0) return false;
    }
    //printf("%d asaldir",prime);
    return true;
}

static int GCD (bigInt totient, bigInt pubKey) // Aralarında asallık testi. Aralarında asal ise 1, değil ise 0 döner.
{
   if (totient % pubKey == 0) return 0;
   else return 1;
}

#endif