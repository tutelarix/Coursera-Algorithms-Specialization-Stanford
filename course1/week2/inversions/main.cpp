#include <iostream>
#include <fstream>

using namespace std;

#define maxN 100000
#define uint unsigned int

unsigned int numberOfInversions(int *data, unsigned int len, int *sortedResult) {
    if (len == 1) {
        return 0;
    }


    uint firstHalfLen = len/2;
    uint secondHalfLen = len - firstHalfLen;
    unsigned int invCount = numberOfInversions(sortedResult, firstHalfLen, data);
    invCount += numberOfInversions(sortedResult + firstHalfLen, secondHalfLen, data + firstHalfLen);

    // merge
    int firstHalfI = 0;
    int secondHalfI = firstHalfLen;
    secondHalfLen+=secondHalfI;

//    if(sortedResult[firstHalfLen-1] > sortedResult[secondHalfI])
//        return 0;

    for(uint i = 0; i<len; ++i) {
        if(firstHalfI == firstHalfLen)
        {
            data[i] = sortedResult[secondHalfI];
            ++secondHalfI;
            continue;
        }

        if(secondHalfI == secondHalfLen)
        {
            data[i] = sortedResult[firstHalfI];
            ++firstHalfI;
            continue;
        }

        if(sortedResult[firstHalfI] < sortedResult[secondHalfI]) {
            data[i] = sortedResult[firstHalfI];
            ++firstHalfI;
        } else {
            data[i] = sortedResult[secondHalfI];
            ++secondHalfI;
            invCount+=firstHalfLen-firstHalfI;
        }
    }

    return invCount;
}

int main()
{
    cout << "Number of inversions" << endl;
    int data[maxN];
    int sortedData[maxN];

    ifstream dataFile;
    dataFile.open("data.txt");

    if(!dataFile.is_open()) {
        cout << "File isn't opened" << endl;
        return -1;
    }

    for(size_t i=0; i<maxN; ++i) {
        dataFile >> data[i];
        sortedData[i] = data[i];
    }
    dataFile.close();

    const unsigned int len = 5;
    int dataTest[len] = {5, 7, 3, 2, 1};
    //int dataTest[len] = {5, 7, 3};
    int sortedDataTest[len] = {5, 7, 3, 2, 1};

    auto number = numberOfInversions(data, maxN, sortedData);

    for(uint i = 0; i< maxN; ++i)
    {
        cout << data[i] << " ";
    }
    cout << endl;
    cout << "Number of inversions: " << number << endl;
    return 0;
}

