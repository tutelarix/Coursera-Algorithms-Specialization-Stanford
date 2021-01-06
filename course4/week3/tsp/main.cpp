#include <iostream>
#include <vector>
#include <fstream>
#include <math.h>
#include <limits>

using namespace std;

typedef double disType;

struct Point {
    disType x = 0;
    disType y = 0;
};

#define isDebug false

bool getNext(vector<vector<disType>> & distances, int current, vector<bool> & visited, int & found, disType & distance) {

    disType minDistance = numeric_limits<disType>::max();
    int minItem = 0;
    bool isFound = false;

    for(int i=1; i < visited.size(); ++i){
        if(!visited[i]){
            isFound = true;
            disType distance = distances[current][i];
            if (distance < minDistance){
                minDistance = distance;
                minItem = i;
            }
        }
    }

    found = minItem;
    distance = minDistance;

    return isFound;
}


disType getTsp(int totalCount, vector<vector<disType>> & distances, int & lastItem, vector<int> & visitedOrder, vector<disType> & visitedDistances) {
    disType totalDis = 0;

    vector<bool> visited = vector<bool>(totalCount, false);
    int current = 0;
    visited[current] = true;

    visitedOrder.push_back(current);
    visitedDistances.push_back(0.0);

    int found = 0;
    disType distance = 0;
    while(getNext(distances, current, visited, found, distance)) {
        totalDis +=distance;
        visited[found] = true;
        current = found;

        if(isDebug) {
            visitedOrder.push_back(found);
            visitedDistances.push_back(totalDis);
        }
    }

    lastItem = current;
    return totalDis;
}

disType getDistance(disType x1, disType y1, disType x2, disType y2)
{
    return pow(pow(x1-x2, 2) + pow(y1-y2, 2), 0.5);
}

int main() {
    string fileName = "data.txt";
    ifstream file("../"+fileName);

    int totalCount = 0;
    vector<vector<disType>> distances;

    if (file.is_open()) {
        file >> totalCount;

        vector<Point> points(totalCount);
        distances = vector<vector<disType>>(totalCount, vector<disType>(totalCount));

        for (int i = 0; i < totalCount; ++i) {
            int number;
            file >> number;

            file >> points[i].x;
            file >> points[i].y;

            if(i%1000==0 && isDebug) {
                cout << "Reading and cal distances: " << i << endl;
            }

            for (int j = 0; j<i; j++){
                disType distance = getDistance(points[i].x, points[i].y, points[j].x, points[j].y);
                distances[j][i] = distance;
                distances[i][j] = distance;
            }
        }

    } else {
        cout << "Cannot open file: " << fileName << endl;
        return 0;
    }

    cout << "Reading and calc distances done";

    int lastItem = 0;
    vector<int> visitedOrder;
    vector<disType> visitedDistances;

    std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();

    disType minimum_cost = getTsp(totalCount, distances, lastItem, visitedOrder, visitedDistances);
    minimum_cost+=distances[lastItem][0];

    std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();

    if(isDebug) {
        for (int i = 0; i < visitedOrder.size(); ++i) {
            cout << endl << visitedOrder[i] << " " << visitedDistances[i];
        }
    }

    cout << endl << endl << "Full time = " << chrono::duration_cast<chrono::seconds>(end - begin).count() << "[ms]";
    cout << endl << fixed << "Minimum cost: " << minimum_cost << endl;
    return 0;
}


