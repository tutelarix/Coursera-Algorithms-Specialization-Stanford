#include <iostream>
#include <fstream>
#include <vector>
#include <float.h>
#include <math.h>
#include <map>
#include <chrono>

using namespace std;

struct Point
{
    float x = 0;
    float y = 0;
};

typedef map<vector<int>, vector<float>> mapType;

float getDistance(float x1, float y1, float x2, float y2)
{
    return pow(pow(x1-x2, 2) + pow(y1-y2, 2), 0.5);
}

void generateMasks(vector<vector<bool>> & result, vector<bool> & currentPerm, int start, int & currentCount, int requiredCount,
                   int totalCount) {
    if (currentCount == requiredCount){
        result.push_back(currentPerm);
        return;
    }

    if (start == totalCount) {
        return;
    }

    currentPerm[start] = 1;
    ++currentCount;
    generateMasks(result, currentPerm, start+1, currentCount, requiredCount, totalCount);

    currentPerm[start] = 0;
    --currentCount;
    generateMasks(result, currentPerm, start+1, currentCount, requiredCount, totalCount);
}

void generateToCalculate(map<vector<bool>, vector<float>> & toCalculate, int count, int totalCount){
    std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();

    vector<bool> perm(totalCount);
    perm[0] = true;
    int start = 1;
    int currentCount = 1;
    vector<vector<bool>> masks;

    generateMasks(masks, perm, start, currentCount, count, totalCount);
    for(auto & mask : masks) {
        toCalculate[mask] = vector<float>(totalCount, -1);
    }

    std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
    std::cout << "generateToCalculate = " << std::chrono::duration_cast<std::chrono::milliseconds>(end - begin).count() << "[ms]" << std::endl;
}

vector<int> convertMaskToNumbers(const vector<bool> & mask) {
    vector<int> result;
    for(int i = 0; i<mask.size(); ++i) {
        if(mask[i]){
            result.push_back(i);
        }
    }
    return  result;
}

void generateToCalculateOpt(mapType & toCalculate, int count, int totalCount){
    std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();
    vector<bool> perm(totalCount);
    perm[0] = true;
    int start = 1;
    int currentCount = 1;
    vector<vector<bool>> masks;

    generateMasks(masks, perm, start, currentCount, count, totalCount);
    for(auto & mask : masks) {
        toCalculate.emplace(convertMaskToNumbers(mask), vector<float>(totalCount, -1));
    }

    std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
    std::cout << "generateToCalculateOpt = " << std::chrono::duration_cast<std::chrono::milliseconds>(end - begin).count() << "[ms]" << std::endl;
}



float getTsp(int totalCount, const vector<vector<float>> & distances) {
    map<vector<bool>, vector<float>> calculated;
    map<vector<bool>, vector<float>> toCalculate;

    vector<bool> init(totalCount);
    init[0] = true;
    calculated[init] = vector<float>(totalCount, -1);
    calculated[init][0] = 0;

    for(int i = 2; i <= totalCount; ++i){
        cout << "Subproblem size: " << i <<endl;
        std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();

        generateToCalculate(toCalculate, i, totalCount);
        for(auto & toCalcItem : toCalculate) {
            auto mask = toCalcItem.first;
            for(int endPointIndex = 1; endPointIndex<totalCount; ++endPointIndex){
                if(mask[endPointIndex] == true) {
                    float minValue = FLT_MAX;
                    mask[endPointIndex] = false;
                    for(int middlePointIndex=0; middlePointIndex<totalCount; ++middlePointIndex){
                        if(mask[middlePointIndex] && calculated[mask][middlePointIndex] != -1){
                            float value = calculated[mask][middlePointIndex] + distances[endPointIndex][middlePointIndex];
                            if(value < minValue){
                                minValue = value;
                            }
                        }
                    }
                    mask[endPointIndex] = true;
                    toCalculate[mask][endPointIndex] = minValue;
                }
            }
        }
        calculated = toCalculate;
        toCalculate.clear();

        std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
        std::cout << "Subproblem = " << std::chrono::duration_cast<std::chrono::milliseconds>(end - begin).count() << "[ms]" << std::endl;
    }

    float minValue = FLT_MAX;
    auto finalCalc = *calculated.begin();
    for(int i = 1; i<totalCount; ++i) {
        float value = finalCalc.second[i] + distances[i][0];
        if(value < minValue){
            minValue = value;
        }
    }

    return minValue;
}


// Optimized solution in 10 times
float getTspOpt(int totalCount, const vector<vector<float>> & distances) {
    mapType calculated;
    mapType toCalculate;

    mapType * calculatedP = &calculated;
    mapType * toCalculateP = &toCalculate;

    vector<int> init{0};
    calculated[init] = vector<float>(totalCount, -1);
    calculated[init][0] = 0;

    for(int m = 2; m <= totalCount; ++m){
        cout << endl;
        cout << "Subproblem size: " << m <<endl;
        std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();

        generateToCalculateOpt(*toCalculateP, m, totalCount);
        for(auto & toCalcItem : *toCalculateP) {
            auto & endPoints = toCalcItem.first;
            for(int i=1; i<endPoints.size(); ++i) {
                auto & endPoint = endPoints[i];
                float minValue = FLT_MAX;

                vector<int> midPoints = endPoints;
                midPoints.erase(midPoints.begin() + i);

                for (auto & midPoint: midPoints) {
                    auto & toCheck = (*calculatedP)[midPoints];
                    auto & prevValue = toCheck[midPoint];
                    if (prevValue != -1) {
                        float value = prevValue + distances[endPoint][midPoint];
                        if (value < minValue) {
                            minValue = value;
                        }
                    }
                }
                toCalcItem.second[endPoint] = minValue;
            }
        }

        swap(toCalculateP, calculatedP);
        toCalculateP->clear();

        std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
        std::cout << "Subproblem = " << std::chrono::duration_cast<std::chrono::milliseconds>(end - begin).count() << "[ms]" << std::endl;
    }

    float minValue = FLT_MAX;
    auto finalCalc = *calculatedP->begin();
    for(int i = 1; i<totalCount; ++i) {
        float value = finalCalc.second[i] + distances[i][0];
        if(value < minValue){
            minValue = value;
        }
    }

    return minValue;
}

int main() {
    string fileName = "data.txt";
    ifstream file("../"+fileName);

    int totalCount = 0;
    vector<vector<float>> distances;

    if (file.is_open()) {
        file >> totalCount;

        vector<Point> points(totalCount);
        distances = vector<vector<float>>(totalCount, vector<float>(totalCount));

        for (int i = 0; i < totalCount; ++i) {
            file >> points[i].x;
            file >> points[i].y;

            for (int j = 0; j<i; j++){
                float distance = getDistance(points[i].x, points[i].y, points[j].x, points[j].y);
                distances[j][i] = distance;
                distances[i][j] = distance;
            }
        }

    } else {
        cout << "File isn't open: " << fileName << endl;
        return 0;
    }

    std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();
    float minimum_cost = getTspOpt(totalCount, distances);
    std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
    cout << endl;
    std::cout << "Full time = " << std::chrono::duration_cast<std::chrono::seconds>(end - begin).count() << "[ms]" << std::endl;

    cout << "Minimum cost: " << minimum_cost << endl;
    return 0;
}