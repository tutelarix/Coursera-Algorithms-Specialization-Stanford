#include <iostream>
#include <stdlib.h>
#include <time.h>
#include <fstream>

#include "hash_tables.hpp"

#define INSERT_TIMES 25000

using namespace std;

template<typename HashTable>
bool test(){
    HashTable hashTable;

    hashTable.insert(10, 15);
    hashTable.insert(10, 20);

    unsigned int value = 0;
    hashTable.lookup(10, value);
    if(value!=20)
        return false;

    hashTable.del(10);
    if(hashTable.lookup(10, value))
        return false;

    hashTable.insert(12, 20);
    hashTable.insert(TABLE_SIZE+12, 30);
    hashTable.insert(TABLE_SIZE*2+12, 30);

    hashTable.del(TABLE_SIZE+12);

    if(hashTable.getCount() !=2)
        return false;

    return true;
}

template<typename HashTable>
float checkHashTable1() {
    HashTable hashTable;

    auto startTime = clock();

    for(unsigned int i = 0; i<INSERT_TIMES; ++i) {
        int key = rand() % (INSERT_TIMES*5);
        int value = rand() % 50000;
        hashTable.insert(key, value);
    }

    //cout << "Load Factor: " << hashTable.getLoad() << endl;
    //cout << "Collisions count: " << hashTable.getCollisionsCount() << endl;

    float difTime = clock()-startTime;
    difTime/=CLOCKS_PER_SEC;
    difTime*=1000;

    return difTime;
}

template<typename HashTable>
float checkHashTable2() {
    HashTable hashTable;

    for(unsigned int i = 0; i<INSERT_TIMES; ++i) {
        int key = rand() % (INSERT_TIMES*5);
        int value = rand() % 50000;
        hashTable.insert(key, value);
    }

    auto startTime = clock();

    for(unsigned int i = 0; i<INSERT_TIMES*10; ++i) {
        int key = rand() % (INSERT_TIMES*5);
        int value = rand() % 50000;
        int operation = rand() % 10;
        if(operation == 1)
            hashTable.insert(key, value);
        else if(operation<8)
            hashTable.del(key);
    }

    //cout << "Load Factor: " << hashTable.getLoad() << endl;
    //cout << "Collisions count: " << hashTable.getCollisionsCount() << endl;

    float difTime = clock()-startTime;
    difTime/=CLOCKS_PER_SEC;
    difTime*=1000;

    return difTime;
}

template<typename HashTable>
void compareHashTables() {
    srand (time(NULL));

//    if(!test<HashTable>())
//        cout << "Test failes" << endl;

    float sum = 0;
    int count = 200;
    for(unsigned int i=0; i<count; ++i) {
        sum+=checkHashTable1<HashTable>();
    }
    cout << "Time1: " << sum/count << endl;

    sum=0;
    for(unsigned int i=0; i<count; ++i) {
        sum+=checkHashTable2<HashTable>();
    }
    cout << "Time2: " << sum/count << endl;
}

#define SIZE 1000000
#define T_LOW -10000
#define T_HIGH 10000

long long int array[SIZE];

void task() {
    ifstream dataFile;
    dataFile.open("data.txt");

    long long int value = 0;
    unsigned int index = 0;
    //HashTableChaining<short> hashTable;
    HashTableOpenAddressing<short> hashTable;
    if(dataFile.is_open()) {
        while(!dataFile.eof()) {
            dataFile >> value;
            array[index] = value;
            index++;

            hashTable.insert(value, 0);
        }

        dataFile.close();
    } else {
        printf("The file isn't found");
    }

    cout << "Load Factor: " << hashTable.getLoad() << endl;
    cout << "Collisions count: " << hashTable.getCollisionsCount() << endl;

    unsigned int count = 0;
    for(int i = T_LOW; i<=T_HIGH; ++i) {
        for(int j = 0; j<SIZE; ++j) {
            long long int findValue = i-array[j];
            if(findValue == array[j]) continue;
            short getValue;
            if(hashTable.lookup(findValue, getValue)) {
                count++;
                break;
            }
        }
        cout << "Index: " << i << "; Count: " << count << endl;
    }

    cout << "Count: " << count << endl;
}

int main()
{
    compareHashTables<HashTableChaining<unsigned int>>();
    cout << endl;
    compareHashTables<HashTableOpenAddressing<unsigned int>>();

    //task();

    return 0;
}

