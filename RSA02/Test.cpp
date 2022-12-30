

#include "RSA.cpp"
#include <iostream>
using namespace std;

int main ()
{ // kodlama kısmında farklı bir şekilde kodlama yapmaya başladı
    RSA algo;
    algo.mesaj.plainTextString = "Muhammed12345678";
    algo.RSAConstructor();
    algo.RSAEncoding();
    algo.RSAEncryption();
    algo.RSADecryption();
    algo.RSADecoding();

    bigInt element = 0x3231;
    //algo.RSAEP(element);

    /*END*/
    cout<<"\nMain Fonksiyonu tamamlandi \n";
}