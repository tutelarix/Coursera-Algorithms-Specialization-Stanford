#include <iostream>
#include <vector>
#include <fstream>
#include <list>

using namespace std;

#define maxN 875714
#define ushort unsigned int

class Vertex {
public:
    vector<ushort> to;
    vector<ushort> from;

    bool visited_1 = false;
    bool visited_2 = false;
};


ushort DFS(vector<Vertex> & vertices, vector<ushort> & sortedFinishTime, ushort startVertex, ushort &finishTime, bool isReverseSearch) {
    ushort count = 1;
    if(isReverseSearch)
    {
        vertices[startVertex].visited_1 = true;
        for(auto && vertex: vertices[startVertex].from) {
            if(!vertices[vertex].visited_1) {
                DFS(vertices, sortedFinishTime, vertex, finishTime, isReverseSearch);
            }
        }

        sortedFinishTime[finishTime] = startVertex;
        finishTime++;

    } else {
        vertices[startVertex].visited_2 = true;
        for(auto && vertex: vertices[startVertex].to) {
            if(!vertices[vertex].visited_2) {
                count+=DFS(vertices, sortedFinishTime, vertex, finishTime, isReverseSearch);
            }
        }
    }

    return count;
}

int main()
{
    ifstream dataFile;
    dataFile.open("data.txt");

    vector<Vertex> vertices(maxN);
    vector<ushort> sortedFinishTime(maxN);
    list<ushort> sccCounts;

    if(dataFile.is_open()) {
        while(!dataFile.eof()) {

            // Fill the data
            int vertexFrom;
            int vertexTo;
            dataFile >> vertexFrom;
            dataFile >> vertexTo;

            // Started from 1
            vertexFrom--;
            vertexTo--;

            vertices[vertexFrom].to.push_back(vertexTo);
            vertices[vertexTo].from.push_back(vertexFrom);
        }
    } else {
        printf("The file isn't found");
    }

    //DFS Loop
    ushort finishTime = 0;
    for(ushort i=maxN-1; i>0; --i) {
        if(!vertices[i].visited_1) {
            DFS(vertices, sortedFinishTime, i, finishTime, true);
        }
    }


    for(ushort i=maxN-1; i>0; --i) {
        ushort vertex = sortedFinishTime[i];
        if(!vertices[vertex].visited_2) {
            ushort count = DFS(vertices, sortedFinishTime, vertex, finishTime, false);
            sccCounts.push_back(count);
        }
    }

    sccCounts.sort();

    for(auto &&val:sccCounts) {
        cout << val << endl;
    }

    return 0;
}

