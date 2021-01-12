#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <math.h>
#include <set>

using namespace std;

struct Clause{
    int a = 0;
    int b = 0;
    bool isAPositive = true;
    bool isBPositive = true;
};

using Items = vector<bool>;
using Clauses = vector<Clause>;


bool loadData(const string & fileName, Items & items, Clauses & clauses){
    ifstream file("../"+fileName);

    int totalCount = 0;

    if (file.is_open()) {
        file >> totalCount;

        items.clear();
        clauses.clear();
        items.resize(totalCount+1, false);
        clauses.resize(totalCount);

        for(int i=0; i<totalCount; ++i){
            int a, b;
            file >> a >> b;

            if (a<0){
                clauses[i].isAPositive = false;
                a = -a;
            }
            clauses[i].a = a;

            if (b<0){
                clauses[i].isBPositive = false;
                b = -b;
            }
            clauses[i].b = b;
        }

    } else {
        cout << "Cannot open file: " << fileName << endl;
        return false;
    }

    return true;
}

void randomInit(Items & items){
    for(int i =1 ; i<items.size(); ++i){
        bool value = rand() % 2;
        items[i] = value;
    }
}

bool evalClause(Items & items, Clause & clause){
    bool a = clause.isAPositive? items[clause.a]: not items[clause.a];
    bool b = clause.isBPositive? items[clause.b]: not items[clause.b];
    return a or b;
}

int evalClauses(Items & items, Clauses & clauses){
    vector<int> clausesEval;
    for (int i = 0; i< clauses.size(); ++i){
        bool evalRes = evalClause(items, clauses[i]);
        if (not evalRes){
            clausesEval.push_back(i);
        }
    }

    if (clausesEval.empty()) return -1;
    if (clausesEval.size() == 1) return clausesEval[0];
    int randClauseToChange = rand() % clausesEval.size();
    return clausesEval[randClauseToChange];
}

struct OptimizedItem {
    bool isPositive = false;
    bool isChecked = false;
    bool isToRemove = true;
};

void checkItem(vector<OptimizedItem> & optimizedItems, int value, bool isPositive){
    auto && optItem = optimizedItems[value];
    if(optItem.isChecked){
        if (optItem.isPositive != isPositive) {
            optItem.isToRemove = false;
        }
    } else {
        optItem.isChecked = true;
        optItem.isPositive = isPositive;
    }
}

int optimizeItemsClauses(Items & items, Clauses & clauses) {
    int uniqueNumber = 0;

    while(true) {
        vector<OptimizedItem> checkedItems = vector<OptimizedItem>(items.size());
        for (int i = 0; i < clauses.size(); ++i) {
            auto &&clause = clauses[i];
            checkItem(checkedItems, clause.a, clause.isAPositive);
            checkItem(checkedItems, clause.b, clause.isBPositive);
        }

        set<int> itemsToRemove;
        uniqueNumber = 0;
        for (int i = 1; i < checkedItems.size(); ++i) {
            if (checkedItems[i].isChecked) {
                ++uniqueNumber;
                if (checkedItems[i].isToRemove) {
                    itemsToRemove.insert(i);
                }
            }
        }
        if (itemsToRemove.empty()) break;

        Clauses updatedClauses = Clauses();
        updatedClauses.reserve(clauses.size());
        for (auto &&clause : clauses) {
            if (itemsToRemove.find(clause.a) == itemsToRemove.end() and
                itemsToRemove.find(clause.b) == itemsToRemove.end()) {
                updatedClauses.push_back(clause);
            }

        }
        clauses = updatedClauses;
    }

    cout << "Reduced to " << clauses.size() << " clauses" << endl;
    cout << "Unique items: " << uniqueNumber << endl;

    return uniqueNumber;
}

bool solve2SAT(Items & items, Clauses & clauses, int uniqueNumbers) {

    int globalIndexCount = round(log2(uniqueNumbers));
    cout << "Global index count: " << globalIndexCount << endl;
    for(int i=0; i<globalIndexCount; ++i)
    {
        cout << "Global index: " << i << endl;
        randomInit(items);
        long long minorIndexCount = uniqueNumbers*uniqueNumbers*2;
        cout << "Minor index count: " << minorIndexCount << endl;
        for (long long j = 0; j<minorIndexCount; ++j) {
            if (j%1000 == 0) {
                cout << "Minor index: " << j << endl;
            }
            int clauseToChange = evalClauses(items, clauses);
            if (clauseToChange == -1) return true;

            int itemToChange = rand() % 2 ? clauses[clauseToChange].a : clauses[clauseToChange].b;

            items[itemToChange] = not items[itemToChange];
        }
    }

    return false;
}

int main() {
    srand(time(nullptr));

    Items items;
    Clauses clauses;

    string fileName = "false1.txt";
    //fileName = "true1.txt";
    //fileName = "false2.txt";
    //fileName = "true2.txt";
    //fileName = "true3.txt";
    //fileName = "true4.txt";
    //fileName = "false3.txt";

    fileName = "data1.txt"; //1
    fileName = "data2.txt"; //0
    fileName = "data3.txt"; //1
    fileName = "data4.txt"; //1
    fileName = "data5.txt"; //0
    fileName = "data6.txt"; //0
    loadData(fileName, items, clauses);
    int uniqueNumbers = optimizeItemsClauses(items, clauses);
    bool result = 1;
    if (uniqueNumbers>0)
        result = solve2SAT(items, clauses, uniqueNumbers);
    cout << "Result for " << fileName << ": " << result << endl;

    return 0;
}


