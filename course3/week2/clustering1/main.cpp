#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>

using namespace std;

#define uint unsigned int


struct Vertex {
    uint parent = 0;
    uint rank = 0;
};

struct Edge {

    uint from = 0;
    uint to = 0;
    uint cost = 0;
};

bool compareEdges(const Edge &first, const Edge &second) {
    return first.cost < second.cost;
}

uint findParent(vector<Vertex> &vertices, const uint &index, int deep = 0) {
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

void unite(vector<Vertex> &vertices, const uint &first, const uint &second) {
    uint firstParent = findParent(vertices, first);
    uint secondParent = findParent(vertices, second);

    if (vertices[firstParent].rank > vertices[secondParent].rank) {
        vertices[secondParent].parent = firstParent;
    } else {
        vertices[firstParent].parent = secondParent;

        if (vertices[firstParent].rank == vertices[secondParent].rank) {
            ++vertices[secondParent].rank;
        }
    }
}

int main() {
    fstream dataFile;
    dataFile.open("../data.txt");


    uint nodesNumber = 0;
    uint edgesNum = 0;
    vector<Vertex> vertices;
    vector<Edge> edges;
    uint requiredClustersNum = 4;


    if (dataFile.is_open()) {
        dataFile >> nodesNumber;

        vertices.resize(nodesNumber);
        edgesNum = nodesNumber * (nodesNumber - 1) / 2;
        edges.resize(edgesNum);

        uint edgesIndex = 0;
        for (uint vertexIndex = 0; vertexIndex < nodesNumber; ++vertexIndex) {
            vertices[vertexIndex].parent = vertexIndex;

            uint currentEdgesNum = nodesNumber - vertexIndex - 1;
            for (uint j = 0; j < currentEdgesNum; ++j) {
                dataFile >> edges[edgesIndex].from;
                dataFile >> edges[edgesIndex].to;
                dataFile >> edges[edgesIndex].cost;

                --edges[edgesIndex].from;
                --edges[edgesIndex].to;

                ++edgesIndex;
            }
        }
    } else {
        cout << "The file isn't found" << endl;
    }

    if (!vertices.empty()) {
        sort(edges.begin(), edges.end(), compareEdges);

        uint numClusters = nodesNumber;
        uint edgeIndex = 0;

        while (numClusters > requiredClustersNum) {
            uint vertexFrom = edges[edgeIndex].from;
            uint vertexTo = edges[edgeIndex].to;

            uint firstParent = findParent(vertices, vertexFrom);
            uint secondParent = findParent(vertices, vertexTo);

            if (firstParent != secondParent) {
                unite(vertices, vertexTo, vertexFrom);
                --numClusters;
            }

            ++edgeIndex;
        }

        uint firstParent = findParent(vertices, edges[edgeIndex].from);
        uint secondParent = findParent(vertices, edges[edgeIndex].to);
        while (firstParent == secondParent) {
            ++edgeIndex;
            firstParent = findParent(vertices, edges[edgeIndex].from);
            secondParent = findParent(vertices, edges[edgeIndex].to);
        }

        cout << edges[edgeIndex].cost << endl;
    }

    return 0;
}