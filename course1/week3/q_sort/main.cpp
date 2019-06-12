#include <iostream>
#include <fstream>
#include <utility>

using namespace std;

#define maxN 10000

int getMedianPivot(int * data, const int & len){
    int first = 0;
    int middle = (len-1)/2;
    int last = len-1;

    if (data[first] > data[middle]){
        if (data[first] > data[last]){
            if (data[middle] > data[last])
                return middle;
            else
                return last;
        }
        else return first;
    } else {
        if (data[first] > data[last]){
            return first;
        }
        else if (data[middle] > data[last])
            return last;
        else
            return middle;
    }
}

int getPivotIndex(int * data, const int & len){
    //return 0;
    //return len-1;

    return getMedianPivot(data, len);
}

int partition(int * data, const int & len){
    int i = 1;
    int j = 1;
    int pivot = data[0];

    for (;j<len;++j){
        if(data[j] < pivot){
            std:swap(data[i], data[j]);
            ++i;
        }
    }

    return i;
}


int qSort(int * data, const int & len){
    if(len == 0 || len == 1) return 0;
    int comparisonNumber = len-1;


    int pivotIndex = getPivotIndex(data, len);

    if(pivotIndex != 0){
        std::swap(data[pivotIndex], data[0]);
    }

    int pivot = partition(data, len);

    std::swap(data[0], data[pivot-1]);

    comparisonNumber += qSort(data, pivot-1);
    comparisonNumber += qSort(data+pivot, len-pivot);

    return comparisonNumber;
}

int main()
{
    int data[maxN];

    ifstream dataFile;
    dataFile.open("data.txt");

    if(!dataFile.is_open()) {
        cout << "File isn't opened" << endl;
        return -1;
    }

    for(size_t i=0; i<maxN; ++i) {
        dataFile >> data[i];
    }
    dataFile.close();

//    int len = 4;
//    int data1[len]={4, 5, 6, 7};
//    int testMedian = getMedianPivot(data1, len);
//    qSort(data1, len);

    int comparisonNumber = qSort(data, maxN);
    cout << comparisonNumber << endl;

    return 0;
}

//1) 162085

//2) 164123

//3) 138382
