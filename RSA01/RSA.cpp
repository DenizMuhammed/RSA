#include <stdio.h>
#include <cstdint>
#include <string.h>

uint64_t P; // prime 1
uint64_t Q; // prime 2
uint64_t T; // totient
uint64_t N; // modulus
uint64_t E; // public key
uint64_t D; // private key;

bool status = false;

bool RSAConstructor(void);
bool generetedE (uint64_t&);
bool generetedD (uint64_t&);
bool isPrime (uint64_t);
int GCD (uint64_t, uint64_t);


 char mesaj[] = "muhammedDeniz";

size_t mesajLenght = strlen(mesaj);
uint64_t  *Emesaj = new uint64_t[255] {0};
char *Dmesaj = new char [255] {0};

bool RSAEncoding();
bool RSADecoding ();

uint64_t RSAEP (char*);

char RSADP (uint64_t*);


int main()
{
   RSAConstructor();

 /*/TEST:
 N = 17;
 E = 13;
  char m[] ="A";
  uint64_t sonuc = RSAEP (m);

  printf(" Bolmeden kalan : %d \n",sonuc);
*/
   status = RSAEncoding();

   if(status == true)
   {
    printf("\nOrjinal Mesaj: %s", mesaj);
    
    printf("\nEncod Edilmis Mesaj: ");
    for(uint64_t i =0; i<mesajLenght; i++)
    {
      printf("%x, ",Emesaj[i]);
    }

   }

   status = RSADecoding();

   if(status == true)

   {
    printf("\nDecod Edilmis Mesaj: ");
    for(uint64_t i =0 ; i<mesajLenght; i++)
    {
      printf("%c, ",Dmesaj[i]);
    }
   }
 
 printf("\n\n\n");
}


bool RSAEncoding()

{
    for(size_t i=0; i<mesajLenght; i++)
    {
      Emesaj[i] = RSAEP(mesaj + i);
    }
    
    return true;
}

bool RSADecoding ()
{
  for(size_t i=0; i<mesajLenght; i++)
  {
    Dmesaj[i] = RSADP( Emesaj + i);
  }
}

char RSADP (uint64_t *Echr)

{
     uint64_t result = *Echr;

   for(uint64_t i =0 ; i < D ; i++)
   {
     result %= N;
     if(i + 1 != D)
     result *= *Echr;
   }

   return (char)result;
}


uint64_t RSAEP (char *chr) // RSA Encryption Primitive
{
   // sonuc = chr^E % N

   uint64_t result = (uint64_t) *chr;

   for(uint64_t i =0 ; i < E ; i++)
   {
     result %= N;
     if(i + 1 != E)
     result *= (uint64_t) *chr;
   }

   return result;
}


bool RSAConstructor(void)
{
  P = 101;
  Q = 907;
  
  N = P * Q;

  T = (P-1) * (Q-1);

  status = generetedE(E);

  if(status == false)
  {
     printf ("Public Key Uretilemedi !");
     return false;
  }

  status = generetedD(D);

  if(status == false)
  {
    printf ("Private Key Uretilemedi !");
    return false;
  }

#if 1
// seedData:
 printf("\n");
 printf("P = %d \n", P);
 printf("Q = %d \n", Q);
 printf("N = %d \n", N);
 printf("T = %d \n", T);
 printf("E = %d \n", E);
 printf("D = %d \n", D);

 uint64_t DE_1T = (D*E-1) / T;

 printf("(D * E -1) / T = %d \n", DE_1T);
#endif

  return true;
}



bool isPrime (uint64_t prime) // özel olarak 2 burada asal sayı kabul edilmiyor !
{
    if(prime<3) return false;

    for(uint64_t cnt = 3; cnt < prime; cnt++)
    {
       if(prime % cnt == 0) return false;
    }

    //printf("%d asaldir",prime);
    return true;
}

int GCD (uint64_t T, uint64_t E) // Aralarında asallık testi. Aralarında asal ise 1, değil ise 0 döner.
{
   if (T % E == 0) return 0;
   else return 1;
}


bool generetedE (uint64_t &E)
{
   E=16;
   while(true)
   {

   do
   {
      E++;
      if ( E == 0xFFFFFFFFFFFFFFFF || E >= T ) return false; // MAX Range.
   }
   while(!isPrime(E));

   if( GCD(T,E) == 1 ) return true;
   
   }

}

bool generetedD (uint64_t &D)

{
    D= 1000;

    while(true)
    {

    uint64_t carpim = D * E;

    uint64_t sonuc = carpim % T;
    
    if(sonuc == 1) return true;

     D++;

     if(D == 0xFFFFFFFFFFFFFFFF) return false;
    }
}