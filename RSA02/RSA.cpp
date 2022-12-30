
// NOT: decode işlemi düzgün sırayla olmuyor. 8 li bloklara ayrılıyor.
#define P1 17
#define P2 4007
// modulus = p1*p2 >= 18446744073709551616 olmalı
#define MAX_bigInt 0xFFFFFFFFFFFFFFFF
#define debugMod 1
#define INITIAL_VALUE_pubKey 12
#define INITIAL_VALUE_priKey 12

#include <iostream>
using namespace std;
#include "RSA.h"

RSA::RSA()
{
  
   RSAPElement = new RSAP_t;

   RSAPElement->prime1 = P1;
   RSAPElement->prime2 = P2;

}

RSA::~RSA()
{
    delete RSAPElement;
    RSAPElement = nullptr;
}

bool RSA::RSAConstructor()

{
  
   RSAPElement->modulus = ( RSAPElement->prime1 ) * ( RSAPElement->prime2 ); // N = P*Q

   RSAPElement->totient = ( RSAPElement->prime1 - 1 ) * ( RSAPElement->prime2 - 1 ); // T = (P-1) * (Q-1)

   this->status = RSA::generatePubKey(RSAPElement->pubKey);

   if(this->status == false) 
   {
    #if debugMod
    printf ("Public Key Uretilemedi !");
    #endif
    return false;
   }

   this->status = RSA::generatePriKey(RSAPElement->priKey);

   if(this->status == false)
   {
     #if debugMod
     printf ("Private Key Uretilemedi !");
     #endif
     return false;
   }

    #if debugMod
    // seedData:
    cout<<"\n"<<"Prime-1: "<< RSAPElement->prime1 <<"\n";
    cout<<"\n"<<"Prime-2: "<< RSAPElement->prime2 <<"\n";
    cout<<"\n"<<"Modulus: "<< RSAPElement->modulus <<"\n";
    cout<<"\n"<<"Totient: "<< RSAPElement->totient <<"\n";
    cout<<"\n"<<"Private Key: "<< RSAPElement->priKey <<"\n";
    cout<<"\n"<<"Public Key: "<< RSAPElement->pubKey <<"\n";
    cout<<"\n"<<"Acik Anahtar Cifti: "<< RSAPElement->pubKey <<","<< RSAPElement->modulus <<"\n";
    cout<<"\n"<<"Gizli Anahtar Cifti: "<< RSAPElement->priKey <<","<< RSAPElement->modulus <<"\n";
    #endif

    bigInt InversTest = ( RSAPElement->pubKey * RSAPElement->priKey - 1 ) % RSAPElement->totient;

     if(InversTest == 0)
     {
        #if debugMod
        cout<<"\nInvers Islemi BASARILI\n";
        #endif
     }

     else if(1)
     {
        #if debugMod
        cout<<"\nInvers Islemi BASARISIZ\n";
        #endif

        return false;
     }

}

bool RSA::generatePubKey(bigInt &pubKey)
{
  pubKey = INITIAL_VALUE_pubKey;

  while(true)
  {
    do
    {
       pubKey++;

       if( pubKey == MAX_bigInt || pubKey >= RSAPElement->totient ) return false;

    } while ( !isPrime( pubKey ) );
    
    if( GCD( RSAPElement->totient, pubKey) == 1 ) return true;
    
  }
}

bool RSA::generatePriKey ( bigInt &priKey )
{
   priKey = INITIAL_VALUE_priKey;

   while( true )
   {

    bigInt carpim = priKey * (this->RSAPElement->pubKey);
    
    bigInt sonuc = carpim % (this->RSAPElement->totient);

    if(sonuc == 1) return true;

    priKey++;

    if(priKey == MAX_bigInt) return false;
  }

}

bool RSA::RSAEncoding()
{

  std::cout<<"\nOrjinal Mesaj: "<<std::hex<<this->mesaj.plainTextString<<"\n";

  size_t lenght = this->mesaj.plainTextString.length();

  size_t missingPart = ( byteNumber - (lenght % byteNumber) ) % byteNumber; // girilen mesaj 8'in katı olmalı. Eksik kısım missingPart'da belirtilir.

  this->mesaj.plainTextString.append(missingPart, paddingChar); // padding

  lenght += missingPart;

  //std::cout<<"\n length: "<<lenght <<"\n";

  size_t indis = 0;

  size_t indis2 = 0;

  bigInt buf = 0;

  this->mesaj.plainTextArray = new bigInt [this->mesaj.plainTextString.length()];
   
   for(size_t i=0; i<this->mesaj.plainTextString.length(); i++) // string text array'e yukleniyor.
   {
      this->mesaj.plainTextArray[i] = (bigInt) this->mesaj.plainTextString[i];
   }

   //std::cout<<"\nplain array: "<< mesaj.plainTextArray[1]<<"\n";

     for(int rotor = 0; true; rotor++, indis++)
     {

      if( indis > lenght) { break; }

      if(rotor == byteNumber) { rotor = 0; mesaj.EncodeTextArray[indis2++] = buf; buf =0; }

      buf |= ( mesaj.plainTextArray[indis] << ( byteNumber * (rotor) ) );

     }

     mesaj.EncodeTextArrayLength = indis2;


  /********************************************************************************/
     //std::cout<<"Encoded Mesaj Lenght: "<<mesaj.EncodeTextArrayLenght<<std::endl;
     std::cout<<"Encoded Mesaj: ";

     for(size_t i =0; i<mesaj.EncodeTextArrayLength; i++)
     {
       std::cout<<std::hex<<mesaj.EncodeTextArray[i]<<",";
     }

     std::cout<<std::endl;

}

 bool RSA::RSADecoding()
 {

  bigInt chr64;
  uint8_t *chr = new uint8_t[500];
  size_t lengthEncode = this->mesaj.EncodeTextArrayLength;

  size_t &lengthDecode = this->mesaj.DecodeTextArrayLength;
  
  std::cout<<"\nDE-CODING: ";
    do
    {

     for(size_t rotor=0; rotor<byteNumber; rotor++)
      {
    
        chr64 = (bigInt) ((0xFFULL<<(rotor * 8)) & mesaj.EncodeTextArray[lengthEncode - 1]);

        chr64 = chr64 >> (rotor * 8);

        chr[lengthDecode] = (uint8_t) chr64;

        std::cout<<chr[lengthDecode];

        lengthDecode++;

      }
      
    } while (--lengthEncode > 0);


 }

bigInt RSA::RSAEncryption()  // RSAEP: RSA Encryption Primitive
{
 size_t EncodeTextArrayLength = this->mesaj.EncodeTextArrayLength;

 //std::cout<<"\nEncodeTextArrayLength: "<<EncodeTextArrayLength;

for(size_t i = 0; i< EncodeTextArrayLength ; i++) // kodlanmış açık mesajın şifrelenmesi ( pub key ile)
{
  this->mesaj.EncryptionTextArray[i] = this->RSAEP(this->mesaj.EncodeTextArray[i]);
  //std::cout<<"\nEncod array ->: "<<this->mesaj.EncodeTextArray[i]<<"\n";
  //std::cout<<"\nRSAEP ->: "<<this->mesaj.EncryptionTextArray[i]<<"\n";
  this->mesaj.EncryptionTextArrayLength += 1;
}

//std::cout<<"\nEncryptionTextArrayLength: "<<this->mesaj.EncryptionTextArrayLength;
// test yazdırması:
 std::cout<<"\nEncrypted Mesaj: ";

for(size_t i =0; i<this->mesaj.EncryptionTextArrayLength; i++)
{
  std::cout<< std::hex << this->mesaj.EncryptionTextArray[i]<<",";
}

 }

bigInt RSA::RSADecryption()
{
  size_t EncryptionTextArrayLength = this->mesaj.EncryptionTextArrayLength;

  for(size_t i = 0; i< EncryptionTextArrayLength ; i++)
  {
    this->mesaj.DecryptionTextArray[i] = this->RSADP(this->mesaj.EncryptionTextArray[i]);
    this->mesaj.DecryptionTextArrayLength += 1;
  }

// test yazdırması:
 std::cout<<"\nDecrypted Mesaj: ";

for(size_t i =0; i<this->mesaj.DecryptionTextArrayLength; i++)
{
   std::cout<< std::hex << this->mesaj.DecryptionTextArray[i]<<",";
}

}

bigInt RSA::RSAEP(bigInt _element) // kodlanmış açık mesajın şifrelenmesi ( pub key ile)
 {
    bigInt modulus = this->RSAPElement->modulus;
    bigInt pubKey  = this->RSAPElement->pubKey;
    bigInt result  = _element;
    bigInt element;

    element = _element % modulus;

    for(bigInt i =0; i < pubKey; i++)
    {
      result %= modulus;
      if( i + 1 != pubKey)
      result *= element;
      //std::cout<<"\nRSAEP result: "<<std::dec<<result<<"\n";
    }
   
   //std::cout<<"\nTest RSAEP result: "<<std::hex<<result <<"\n";
   return result;

 }

bigInt RSA::RSADP(bigInt _element) // şifrelenmiş kodlu mesajın açık hali ( pri key ile)
 {
   bigInt modulus = this->RSAPElement->modulus;
   bigInt priKey  = this->RSAPElement->priKey;
   bigInt result  = _element;

   bigInt element;

   element = _element % modulus;

    for(bigInt i =0; i <priKey; i++)
    {
      result = result % modulus;
      if( i + 1 != priKey)
      result = result * element; 
    }
   
    //std::cout<<"RSADP ->"<<std::hex<<result;
    return result;

 }

Message::Message()
{
   
}