#include <iostream>
#include <fstream>
#include <list>
#include <algorithm>

using namespace std;

struct Job{
    Job(const int & index, const int & weight, const int & length)
    : index(index), weight(weight), length(length)
    {}

    int index;
    int weight;
    int length;
};

void calcResult(const list<Job> &jobs);

inline bool Comp1 (const Job & left, const Job & right) {
    int leftScore = left.weight - left.length;
    int rightScore = right.weight - right.length;

    if (leftScore == rightScore)
        return left.weight > right.weight;

    return leftScore > rightScore;
}

inline bool Comp2 (const Job & left, const Job & right) {
    float leftScore = (float)left.weight / left.length;
    float rightScore = (float)right.weight / right.length;

    return leftScore > rightScore;
}

int main()
{
    ifstream dataFile;
    dataFile.open("jobs.txt");

    list<Job> jobs;

    if(dataFile.is_open()) {
        int index = 1;
        int weight;
        int length;
        int count;
        dataFile >> count;
        while(!dataFile.eof()) {
            dataFile >> weight >> length;
            jobs.emplace_back(index, weight, length);
            ++index;
        }
    } else {
        cout << "The file isn't found" << endl;
    }

    cout << "First task" << endl;
    jobs.sort(Comp1);
    calcResult(jobs);

    cout << "Second task" << endl;
    jobs.sort(Comp2);
    calcResult(jobs);

    return 0;
}

void calcResult(const list<Job> &jobs) {
    long long compTime = 0;
    long long sum = 0;
    for (auto && value : jobs ){
        compTime +=value.length;
        sum+=value.weight*compTime;
    }

    cout << "Total comp time: " << compTime << endl;
    cout << "Total weighted comp time: " << sum << endl;
}

