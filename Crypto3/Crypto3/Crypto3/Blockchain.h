#pragma once
#ifndef Blockchain_h
#define Blockchain_h

#include <vector>
#include "Block.h"

// Blockchain Class
class Blockchain
{
private:
    Block createGenesisBlock();
    std::vector<Block> chain;

public:
    // Constuctor
    Blockchain();

    // Public Functions
    std::vector<Block> getChain();
    Block* getLatestBlock();
    bool isChainValid();
    void addBlock(TransactionData data);
    void printChain();
};

#endif /* Blockchain_h */