#include "cryptlib.h"
#include "rijndael.h"
#include "modes.h"
#include "files.h"
#include "osrng.h"
#include "hex.h"

#include <iostream>
#include <ctime>
#include <vector>
#include <iostream>
#include <string>
#include "Block.h"
#include "Blockchain.h"
#include "TransactionData.h"

int main(int argc, char* argv[])
{
    using namespace CryptoPP;

    AutoSeededRandomPool prng;
    HexEncoder encoder(new FileSink(std::cout));

    SecByteBlock key(AES::DEFAULT_KEYLENGTH);
    SecByteBlock iv(AES::BLOCKSIZE);

    prng.GenerateBlock(key, key.size());
    prng.GenerateBlock(iv, iv.size());

    std::string plain = "CRYPTO3-BLUEPRINT library to TVC compilation ";
    std::string cipher, recovered;

    std::cout << "plain text: " << plain << std::endl;

    /*********************************\
    \*********************************/

    try
    {
        CBC_Mode< AES >::Encryption e;
        e.SetKeyWithIV(key, key.size(), iv);

        StringSource s(plain, true, 
            new StreamTransformationFilter(e,
                new StringSink(cipher)
            ) // StreamTransformationFilter
        ); // StringSource
    }
    catch(const Exception& e)
    {
        std::cerr << e.what() << std::endl;
        exit(1);
    }

    /*********************************\
    \*********************************/

    std::cout << "key: ";
    encoder.Put(key, key.size());
    encoder.MessageEnd();
    std::cout << std::endl;

    std::cout << "iv: ";
    encoder.Put(iv, iv.size());
    encoder.MessageEnd();
    std::cout << std::endl;

    std::cout << "cipher text: ";
    encoder.Put((const byte*)&cipher[0], cipher.size());
    encoder.MessageEnd();
    std::cout << std::endl;
    
    /*********************************\
    \*********************************/

    try
    {
        CBC_Mode< AES >::Decryption d;
        d.SetKeyWithIV(key, key.size(), iv);

        StringSource s(cipher, true, 
            new StreamTransformationFilter(d,
                new StringSink(recovered)
            ) // StreamTransformationFilter
        ); // StringSource

        std::cout << "" << recovered << std::endl;
    }
    catch(const Exception& e)
    {
        std::cerr << e.what() << std::endl;
        exit(1);
    }
    Blockchain awesomeCoin;

    // Data for first block
    time_t data1Time;
    TransactionData data1(1.5, "Joe", "Sally", time(&data1Time));
    awesomeCoin.addBlock(data1);

    time_t data2Time;
    TransactionData data2(0.2233, "Martha", "Fred", time(&data2Time));
    awesomeCoin.addBlock(data2);

    // Let's see what's in the awesomeCoin blockchain!
    awesomeCoin.printChain();

    // Is it valid?
    printf("\nIs chain still valid? %d\n", awesomeCoin.isChainValid());

    // Someone's getting sneaky
    Block* hackBlock = awesomeCoin.getLatestBlock();
    hackBlock->data.amount = 10000; // Oh yeah!
    hackBlock->data.receiverKey = "Jon"; // mwahahahaha!

    // Let's look at data
    awesomeCoin.printChain();

    // Awww! Why is it not valid?
    printf("\nIs chain still valid? %d\n", awesomeCoin.isChainValid());

    return 0;
}