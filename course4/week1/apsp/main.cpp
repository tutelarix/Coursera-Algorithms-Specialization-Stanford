#include <iostream>
#include <fstream>
#include <vector>
#include <limits>
#include "tuple"

using namespace std;

struct SimpleEdge {
    bool isSet = false;
    int cost = 0;
};

struct Edge {
    Edge(uint from, uint to, int cost):
        from(from), to(to), cost(cost){}

    uint from;
    uint to;
    int cost;
};

struct Vertex {
    vector<Edge> edges;
};

tuple<bool, int> FloydWarshall(const vector<Vertex> & vertices, const vector<vector<SimpleEdge>> & edges) {
    cout << "Floyd-Warshall" << endl;
    uint size = vertices.size();

    vector<vector<vector<int>>> arr(size, vector<vector<int>>(size, vector<int>(size)));

    for(uint i = 1; i< size; ++i){
        for(uint j=1; j<size; ++j){
            arr[i][j][0] = 1000000;
            if(edges[i][j].isSet){
                arr[i][j][0] = edges[i][j].cost;
            } else if(i==j){
                arr[i][j][0] = 0;
            }
        }
    }

    cout << "Initialized" << endl;

    bool isNegativeCostCycle = false;
    for(uint k = 1; k<size; ++k){
        cout << "Processed: " << k << endl;
        for(uint i=1; i<size; ++i){
            for(uint j = 1; j<size; ++j){
                if(arr[i][j][k-1] <= arr[i][k][k-1] + arr[k][j][k-1]){
                    arr[i][j][k] = arr[i][j][k-1];
                } else {
                    arr[i][j][k] = arr[i][k][k-1] + arr[k][j][k-1];
                }
            }
        }

        // Check for negative-cost cycle
        for(uint i=1; i<size;++i) {
            if(arr[i][i][k]<0) {
                isNegativeCostCycle = true;
                break;
            }
        }

        if(isNegativeCostCycle) break;
    }

    cout << "Calculated. isNegeativeCostCycle: " << isNegativeCostCycle << endl;

    int shortestPath = 1000000;
    if(!isNegativeCostCycle) {
        for(uint i = 1; i<size; ++i){
            for(uint j=1; j<size; ++j){
                if(arr[i][j][size-1]<shortestPath)
                    shortestPath = arr[i][j][size-1];
            }
        }
    }

    return tuple<bool, int>(isNegativeCostCycle, shortestPath);
}

int getShortestPath(string fileName) {
    ifstream file("../"+fileName);

    int verticesNum = 0;
    int edgesNum = 0;
    vector<Vertex> vertices;
    vector<vector<SimpleEdge>> edges;

    if (file.is_open()) {
        file >> verticesNum;
        file >> edgesNum;
        vertices.resize(verticesNum+1);

        edges.resize(verticesNum+1);
        for(int i = 0; i<verticesNum+1; ++i){
            edges[i].resize(verticesNum+1);
        }

        uint vertexFrom;
        uint vertexTo;
        int cost;

        while(file.good()) {
            file >> vertexFrom;
            file >> vertexTo;
            file >> cost;

            vertices[vertexFrom].edges.emplace_back(vertexFrom, vertexTo, cost);
            if(!edges[vertexFrom][vertexTo].isSet){
                edges[vertexFrom][vertexTo].isSet = true;
                edges[vertexFrom][vertexTo].cost = cost;
            } else {
                if(edges[vertexFrom][vertexTo].cost>cost){
                    edges[vertexFrom][vertexTo].cost = cost;
                }
            }
        }


    } else {
        cout << "File isn't open: " << fileName << endl;
    }

    if(vertices.size()){
        auto result = FloydWarshall(vertices, edges);
        cout << get<0>(result) << " " <<get<1>(result) << endl;
    }

    return 0;
}

int main() {

    //getShortestPath("test1.txt");
    //getShortestPath("test2.txt");

    cout << "g1" << endl;
    getShortestPath("g1.txt");
    cout << "g2" << endl;
    getShortestPath("g2.txt");
    cout << "g3" << endl;
    getShortestPath("g3.txt");
    cout << "large" << endl;
    getShortestPath("large.txt");

    return 0;
}