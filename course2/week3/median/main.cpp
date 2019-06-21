#include <iostream>
#include <fstream>
#include <time.h>

using namespace std;

#define SIZE 10000

enum HeapType {
    min = 0,
    max = 1
};

class Heap {
public:
    Heap(HeapType type) {
        m_Type = type;
    }

    void insert(int key) {
        m_Array[m_Size] = key;
        bubbleUp(m_Size++);
    }

    int extract() {
        int ret = m_Array[0];
        m_Array[0]=m_Array[--m_Size];
        bubbleDown(0);
        return ret;
    }

    int getValue() {
        return m_Array[0];
    }

    void print() {
        cout << endl;
        for(int i=0; i<m_Size; ++i){
            cout << m_Array[i] << " ";
        }
        cout << endl;
    }

    int size() {
        return m_Size;
    }

private:
    bool compare(int upper, int lower) {
        if(m_Array[upper] >= m_Array[lower] && m_Type == HeapType::max) {
            return true;
        } else if(m_Array[upper] <= m_Array[lower] && m_Type == HeapType::min) {
            return true;
        }

        return false;
    }


    void bubbleUp(int index){
        int child = index;
        int root = getRoot(index);

        while(root >= 0 && !compare(root, child)) {
            std::swap(m_Array[root], m_Array[child]);
            child = root;
            root = getRoot(child);
        }
    }

    void bubbleDown(int index){
        int root = index;
        int upIndex = getUpperIndex(getChild1(root), getChild2(root));

        while(upIndex>=0 && !compare(root, upIndex)) {
            std::swap(m_Array[root], m_Array[upIndex]);
            root = upIndex;
            upIndex = getUpperIndex(getChild1(root), getChild2(root));
        }
    }

    int getChild1(int index){
        int value = 2*index+1;
        if(value<m_Size) return value;

        return -1;
    }

    int getChild2(int index){
        int value = 2*index+2;
        if(value<m_Size) return value;

        return -1;
    }

    int getUpperIndex(int index1, int index2){
        if(index1>=0 && index2>=0)
            return compare(index1, index2)?index1:index2;
        else if(index1>0)
            return index1;
        else if(index2>0)
            return index2;
        return -1;
    }

    int getRoot(int index){
        if(index == 0) return -1;
        int value = (index-1)/2;
        return value;
    }

    HeapType m_Type = HeapType::min;
    int m_Array[SIZE];
    int m_Size = 0;
};

void testHeaps() {
    Heap minHeap(HeapType::min);
    Heap maxHeap(HeapType::max);

    int size = 8;
    int data[] = {5, 17, 1, 10, 2, 1, 17, 19};

    for(int i=0; i<size; ++i) {
        minHeap.insert(data[i]);
        //minHeap.print();
        maxHeap.insert(data[i]);
        //maxHeap.print();
    }

    //minHeap.print();
    maxHeap.print();

    for(int i=0; i<size; ++i) {
        //minHeap.extract();
        //minHeap.print();
        maxHeap.extract();
        maxHeap.print();
    }
}

int main()
{
    //testHeaps();

    ifstream dataFile;
    dataFile.open("data.txt");

    Heap minHeap(HeapType::min);
    Heap maxHeap(HeapType::max);

    long long medianSum = 0;

    auto startTime = clock();
    if(dataFile.is_open()) {
        int value = 0;
        dataFile >> value;

        maxHeap.insert(value);
        int median = value;
        medianSum+=median;

        dataFile >> value;
        while(!dataFile.eof()) {
            // Insert
            if(value > maxHeap.getValue()) {
                minHeap.insert(value);
            } else {
                maxHeap.insert(value);
            }

            // Rebalance
            if(minHeap.size() > maxHeap.size()) {
                value = minHeap.extract();
                maxHeap.insert(value);
            } else if(maxHeap.size()-1>minHeap.size()) {
                value = maxHeap.extract();
                minHeap.insert(value);
            }

            median = maxHeap.getValue();
            medianSum+=median;
            dataFile >> value;
        }

        dataFile.close();
    } else {
        cout << "The file isn't found" << endl;
    }


    cout << medianSum << endl;
    cout << medianSum %10000 << endl;

    float difTime = clock()-startTime;
    difTime/=CLOCKS_PER_SEC;
    difTime*=1000;
    cout << "Time: " << difTime << endl;

    return 0;
}

