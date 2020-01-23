#include <iostream>
#include <fstream>
#include <vector>
#include <queue>
#include <algorithm>

using namespace std;
struct Node {

    uint weight = 0;
    int index = -1;

    int child1 = -1;
    int child2 = -1;
};

bool nodesComp(Node &first, Node &second) {
    return first.weight < second.weight;
}

int pop(queue<int> &queue) {
    int result = queue.front();
    queue.pop();
    return result;
}

int getMin(queue<int> &queue1, queue<int> &queue2, const vector<Node> &nodes) {
    if (queue1.empty()) {
        return pop(queue2);
    }

    if (queue2.empty()) {
        return pop(queue1);
    }

    if (nodes[queue1.front()].weight < nodes[queue2.front()].weight) {
        return pop(queue1);
    } else {
        return pop(queue2);
    }
}

void dfs(vector<Node> &nodes, int rootIndex, uint &min, uint &max, uint level) {
    int child1 = nodes[rootIndex].child1;
    int child2 = nodes[rootIndex].child2;

    if (child1 == -1 && child2 == -1) {
        // found leaf
        if (max < level) max = level;
        if (min > level) min = level;
        return;
    }

    if (child1 != -1)
        dfs(nodes, child1, min, max, level + 1);

    if (child2 != -1)
        dfs(nodes, child2, min, max, level + 1);
}

int main() {
    fstream dataFile;
    dataFile.open("../data.txt");

    vector<Node> nodes;
    if (dataFile.is_open()) {
        uint count = 0;
        dataFile >> count;
        nodes.resize(count);

        for (uint i = 0; i < count; ++i) {
            dataFile >> nodes[i].weight;
        }
    } else {
        cout << "Data file isn't found" << endl;
    }

    if (nodes.size()) {
        sort(nodes.begin(), nodes.end(), nodesComp);
        queue<int> singleNodes;
        queue<int> sumNodes;

        for (int i = 0; i < nodes.size(); ++i) {
            nodes[i].index = i;
            singleNodes.push(i);
        }

        while (!(singleNodes.empty() && sumNodes.size() == 1)) {
            Node &left = nodes[getMin(singleNodes, sumNodes, nodes)];
            Node &right = nodes[getMin(singleNodes, sumNodes, nodes)];

            Node parent;
            parent.weight = left.weight + right.weight;
            parent.child1 = left.index;
            parent.child2 = right.index;
            parent.index = nodes.size();

            nodes.push_back(parent);
            sumNodes.push(parent.index);
        }

        // Reconstruction
        uint min = nodes.size();
        uint max = 0;
        dfs(nodes, nodes.back().index, min, max, 0);

        cout << "Min: " << min << "; Max: " << max << endl;
    }

    return 0;
}
