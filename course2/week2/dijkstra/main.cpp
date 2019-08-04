#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>
#include <string.h>
#include <algorithm>

using namespace std;
#define verNum 200
#define verLen 201

struct Edge {
    Edge(int v, int w) : vertex(v), weight(w) {};
    int vertex;
    int weight;
};

struct Vertex {
    std::vector<Edge> edges;
    bool visited = false;
    int num = -1;

    int heapPosition = -1;
    int shortestPath = 1000000;
};

struct Heap {
    Vertex *extractMin() {
        swap(arr[0], arr[arr.size() - 1]);
        arr[0]->heapPosition = 0;

        Vertex *min = arr.back();
        min->heapPosition = -1;
        arr.pop_back();
        bubbleDown(0);
        return min;
    };

    void add(Vertex *vertex) {
        arr.push_back(vertex);
        int position = arr.size() - 1;
        vertex->heapPosition = position;

        bubbleUp(position);
    };

    void remove(Vertex *vertex) {
        if (vertex->heapPosition == -1) {
            printf("Wrong heap position");
        }

        int vertexPos = vertex->heapPosition;
        if (vertexPos == arr.size() - 1) {
            arr.pop_back();
            vertex->heapPosition = -1;
        } else {
            swap(arr[vertexPos], arr[arr.size() - 1]);
            arr[vertexPos]->heapPosition = vertexPos;
            arr.pop_back();
            vertex->heapPosition = -1;

            Vertex *updVertex = arr[vertexPos];
            bubbleUp(updVertex->heapPosition);
            bubbleDown(updVertex->heapPosition);
        }
    }

    void bubbleUp(int position) {
        while (position != 0) {
            int parent = (position - 1) / 2;
            if (arr[parent]->shortestPath <= arr[position]->shortestPath)
                break;

            swap(arr[parent], arr[position]);
            arr[parent]->heapPosition = parent;
            arr[position]->heapPosition = position;

            position = parent;
        }
    };

    void bubbleDown(int position) {
        while (true) {
            int child1 = position * 2 + 1;
            int child2 = position * 2 + 2;

            int minChild = -1;
            if (child1 >= arr.size() && child2 >= arr.size())
                break;
            if (child2 >= arr.size())
                minChild = child1;
            else {
                minChild = arr[child1]->shortestPath <= arr[child2]->shortestPath ? child1 : child2;
            }

            if (arr[position]->shortestPath <= arr[minChild]->shortestPath)
                break;
            else {
                swap(arr[position], arr[minChild]);
                arr[position]->heapPosition = position;
                arr[minChild]->heapPosition = minChild;

                position = minChild;
            }
        }
    };

    bool empty() {
        return arr.empty();
    }

private:
    vector<Vertex *> arr;
    int size = 0;
};


Vertex vertices[verLen];
Heap heap;
int visitedNum = 0;

void dijkstra() {
    while (visitedNum < verNum) {

        // Unconnected nodes
        if (heap.empty()) break;

        Vertex *vertex = heap.extractMin();
        vertex->visited = true;

        for (auto &&edge:vertex->edges) {
            Vertex *neigVertex = &vertices[edge.vertex];
            if (neigVertex->visited == false) {
                int updShortestPath = vertex->shortestPath + edge.weight;
                if (neigVertex->heapPosition != -1)
                    if (neigVertex->shortestPath <= updShortestPath) {
                        continue;
                    } else {
                        heap.remove(neigVertex);
                    }

                neigVertex->shortestPath = updShortestPath;
                heap.add(neigVertex);
            }
        }
    }
}

int main() {
    ifstream dataFile;
    dataFile.open("data.txt");


    int value = 0;
    unsigned int index = 0;
    string line;
    while (getline(dataFile, line)) {
        size_t initialPos = line.find("\t");
        if (initialPos != string::npos) {
            index = stoi(line.substr(0, initialPos));
            istringstream iss(line.substr(initialPos + 1));
            string edge;
            while (getline(iss, edge, '\t')) {
                size_t comaPos = edge.find(",");
                int vertex = stoi(edge.substr(0, comaPos));
                int weight = stoi(edge.substr(comaPos + 1));
                vertices[index].edges.emplace_back(vertex, weight);
            }
        } else {
            index++;
        }

        vertices[index].num = index;
    }

    if (dataFile.is_open()) {
        while (!dataFile.eof()) {
            dataFile >> value;
        }

        dataFile.close();
    } else {
        printf("The file isn't found");
    }

    // Prepare for start vertex
    for (auto &&edge: vertices[1].edges) {
        Vertex *vertex = &vertices[edge.vertex];
        vertex->shortestPath = edge.weight;
        heap.add(vertex);
    }
    vertices[1].visited = true;
    vertices[1].shortestPath = 0;
    visitedNum = 1;

    dijkstra();

    //7,37,59,82,99,115,133,165,188,197
    cout << vertices[7].shortestPath << ",";
    cout << vertices[37].shortestPath << ",";
    cout << vertices[59].shortestPath << ",";
    cout << vertices[82].shortestPath << ",";
    cout << vertices[99].shortestPath << ",";
    cout << vertices[115].shortestPath << ",";
    cout << vertices[133].shortestPath << ",";
    cout << vertices[165].shortestPath << ",";
    cout << vertices[188].shortestPath << ",";
    cout << vertices[197].shortestPath;

    return 0;
}