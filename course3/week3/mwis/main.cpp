#include <iostream>
#include <fstream>
#include <vector>

using namespace std;

int main() {
    fstream dataFile;
    dataFile.open("../data.txt");

    vector<uint> vertices;

    if (dataFile.is_open()) {
        uint count = 0;
        dataFile >> count;
        vertices.resize(count);

        for (uint i = 0; i < count; ++i) {
            dataFile >> vertices[i];
        }

    } else {
        cout << "Data file isn't found" << endl;
    }

    if (vertices.size()) {
        vector<unsigned long long> mwis(vertices.size() + 1);
        mwis[0] = 0;
        mwis[1] = vertices[0];

        for (uint i = 2; i < mwis.size(); ++i) {
            mwis[i] = max(mwis[i - 1], mwis[i - 2] + vertices[i - 1]);
        }

        // reconstruction
        vector<int> resultPath(vertices.size() + 1, 0);
        int i = mwis.size();
        while (i >= 0) {
            if (mwis[i - 1] >= mwis[i - 2] + vertices[i - 1]) {
                --i;
            } else {
                // due to shift in the verices
                resultPath[i] = 1;
                i = i - 2;
            }
        }

        int indexes[] = {1, 2, 3, 4, 17, 117, 517, 997};
        for (uint i = 0; i < 8; ++i) {
            int index = indexes[i];
            if (index < resultPath.size()) {
                cout << resultPath[index];
            } else {
                cout << 0;
            }
        }
    }

    return 0;
}
