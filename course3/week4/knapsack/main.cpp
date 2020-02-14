#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>

using namespace std;

struct Item {
    uint value;
    uint weight;
};

int main() {
    fstream dataFile;
    dataFile.open("../data2.txt");

    uint knSize = 0;
    vector <Item> items;

    if (dataFile.is_open()) {
        dataFile >> knSize;
        uint count = 0;
        dataFile >> count;
        items.resize(count);

        for (uint i = 0; i < count; ++i) {
            dataFile >> items[i].value;
            dataFile >> items[i].weight;
        }

        dataFile.close();
    } else {
        cout << "Data file isn't found" << endl;
    }

    if(!items.empty()){
        vector<vector<uint>> table(2, vector<uint>(knSize+1, 0));

        int currentIndex = 1;
        int prevIndex = 0;
        for(uint i = 0; i<items.size(); ++i) {
            for(uint j=0; j<=knSize; ++j) {
                if(j >= items[i].weight) {
                    table[currentIndex][j] = max(table[prevIndex][j],
                                                 table[prevIndex][j - items[i].weight] + items[i].value);
                } else {
                    table[currentIndex][j] = table[prevIndex][j];
                }

                uint a = table[currentIndex][j];
                              a+=10;
            }

            swap(currentIndex, prevIndex);
            cout << i << endl;
        }


        cout << table[prevIndex][knSize] << endl;
    }
}
