#include <iostream>
#include <fstream>
#include <vector>

using namespace std;

struct Edge {
    Edge(const int & to, const int & cost)
    : to(to), cost(cost) {}

    int to;
    int cost;
};

struct Vertex{
    vector<Edge> edges;
    int heapIndex = -1;
};

struct HeapItem {
    HeapItem(){}

    HeapItem(const HeapItem & item){
        this->vertex = item.vertex;
        this->key = item.key;
    }

    void operator = (const HeapItem & item){
        this->vertex = item.vertex;
        this->key = item.key;
    }

    int vertex = 0;
    int key = 100000;
};

inline bool operator < (const HeapItem & first, const HeapItem & second) {
    return first.key < second.key;
}

vector<Vertex> vertices;

struct Heap {
    int extractMin(int & key) {
       int vertexIndex = -1;

       if(items.size()){
           vertexIndex = items[0].vertex;
           key = items[0].key;
           del(0);
       }

       return vertexIndex;
    }

    void del(int index) {
        if(items.size()>1)
            swap(index, items.size()-1);

        vertices[items[items.size()-1].vertex].heapIndex = -1;
        items.pop_back();

        index = bubbleUp(index);
        bubbleDown(index);
    }

    int bubbleUp(int index) {
        if(items.size()<2) return index;

        while(true) {
            int parent = (index-1)/2; //0 if index == 0
            if (parent != index && items[index] < items[parent]){
                swap(parent, index);
                index = parent;
            } else {
                break;
            }
        }

        return index;
    }

    int bubbleDown(int index) {
        if(items.size()<2) return index;

        while(true){
            int child1 = (index*2+1);
            int child2 = (index*2+2);
            int smallest = index;

            if(child1<items.size() && items[child1]< items[smallest])
                smallest = child1;

            if(child2<items.size() && items[child2]< items[smallest])
                smallest = child2;

            if(smallest != index){
                swap(smallest, index);
                index = smallest;
            } else {
                break;
            }
        }

        return index;
    }

    void updateIndex(int index){
        index = bubbleUp(index);
        bubbleDown(index);
    }

    void swap(const int & first, const int & second){
        HeapItem temp = items[first];
        items[first] = items[second];
        items[second] = temp;

        vertices[items[first].vertex].heapIndex = first;
        vertices[items[second].vertex].heapIndex = second;
    }

    vector<HeapItem> items;
};

Heap heap;

void processVertex(Vertex & vertex){
    for(auto && edge:vertex.edges){
        int neigHeapIndex = vertices[edge.to].heapIndex;
        if(neigHeapIndex != -1){
            int newCost = edge.cost;
            if(newCost < heap.items[neigHeapIndex].key){
                heap.items[neigHeapIndex].key = newCost;
                heap.updateIndex(neigHeapIndex);
            }
        }
    }
}

int main() {
    ifstream dataFile;
    dataFile.open("../edges.txt");


    int nodesNumber;
    int edgesNumber;

    if(dataFile.is_open()) {
        dataFile >> nodesNumber;
        dataFile >> edgesNumber;
        vertices.resize(nodesNumber);

        while(!dataFile.eof()) {
            int vertex1;
            int vertex2;
            int cost;

            dataFile >> vertex1 >> vertex2 >> cost;

            --vertex1;
            --vertex2;

            vertices[vertex1].edges.emplace_back(vertex2, cost);
            vertices[vertex2].edges.emplace_back(vertex1, cost);
        }
    } else {
        cout << "The file isn't found" << endl;
    }

    // Can be done while reading, but moved out to separate the algorithm from data reading
    heap.items.resize(nodesNumber);

    for(int i=0; i<nodesNumber; ++i) {
        heap.items[i].vertex = i;
        vertices[i].heapIndex = i;
    }

    heap.del(vertices[0].heapIndex);
    processVertex(vertices[0]);

    long long totalSum = 0;
    int key = 0;
    while(true){
        int processedVertex = heap.extractMin(key);
        if(processedVertex != -1) {
            processVertex(vertices[processedVertex]);
            totalSum+=key;
        } else {
            break;
        }
    }

    cout << "Total sum: " << totalSum << endl;

    return 0;
}
