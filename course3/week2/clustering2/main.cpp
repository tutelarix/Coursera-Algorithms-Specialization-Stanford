#include <iostream>
#include <fstream>
#include <unordered_map>
#include <vector>
#include <sstream>
#include <algorithm>

using namespace std;

using uset=unordered_map<string, int>;
using uint = unsigned int;

struct Node {
    Node(uint parent, string data) :
            parent(parent), data(data) {};

    uint parent = 0;
    uint rank = 0;
    string data = "";
};

uint findParent(vector<Node> &vertices, const uint &index, int deep = 0) {
    if (index == vertices[index].parent) {
        if (deep > 3) {
            int stop = 1;
        }

        return index;
    }

    uint parent = findParent(vertices, vertices[index].parent, deep + 1);

    // path compression
    vertices[index].parent = parent;

    return parent;
}

bool unite(vector<Node> &vertices, const uint &first, const uint &second) {
    uint firstParent = findParent(vertices, first);
    uint secondParent = findParent(vertices, second);
    if(firstParent == secondParent) return false;

    if (vertices[secondParent].rank > vertices[firstParent].rank) {
        vertices[firstParent].parent = secondParent;
    } else {
        vertices[secondParent].parent = firstParent;

        if (vertices[firstParent].rank == vertices[secondParent].rank) {
            ++vertices[firstParent].rank;
        }
    }

    return true;
}
void reverseBit(string & value, uint bit){
    value[bit] = value[bit] == '0' ? '1': '0';
}

int main() {
    fstream dataFile;
    dataFile.open("../data.txt");

    uint nodesNum = 0;
    uint bitsNum = 0;

    uset nodesTable;
    vector<Node> nodesArray;

    if (dataFile.is_open()) {
        string line;

        getline(dataFile, line);
        istringstream iss(line);
        iss >> nodesNum;
        iss >> bitsNum;
        nodesArray.reserve(nodesNum);


        while(getline(dataFile, line)) {
            line.erase(remove(line.begin(), line.end(), ' '), line.end());
            if (nodesTable.find(line) == nodesTable.end()) {
                uint nodeIndex = nodesArray.size();
                nodesTable[line] = nodeIndex;
                nodesArray.emplace_back(nodeIndex, line);
            }
        }
    } else {
        cout << "The file isn't found" << endl;
    }

    uint clustersNum = nodesArray.size();
    for(uint nodeIndex = 0; nodeIndex<nodesArray.size(); ++nodeIndex){
        string nodeData = nodesArray[nodeIndex].data;

        for(uint i = 0; i<bitsNum; ++i){
            reverseBit(nodeData, i);

            if (nodesTable.find(nodeData) != nodesTable.end()) {
                if(unite(nodesArray, nodeIndex, nodesTable[nodeData])) {
                    --clustersNum;
                }
            }

            for(uint j = 0; j<bitsNum; ++j){
                if(i == j) continue;
                reverseBit(nodeData, j);

                if (nodesTable.find(nodeData) != nodesTable.end()) {
                    if(unite(nodesArray, nodeIndex, nodesTable[nodeData])) {
                        --clustersNum;
                    }
                }
                reverseBit(nodeData, j);
            }
            reverseBit(nodeData, i);
        }
    }

    cout << clustersNum << endl;
}