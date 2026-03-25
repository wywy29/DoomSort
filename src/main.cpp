#include "sort.h"
#include <fstream>
#include <sstream>

int main() {
    // runTest(quickSort, 100); // a quick test to see if quickSort() works or not

    vector<float> data;
    data.reserve(100000); // reserve space for 100000 data points to prevent constant resizing
    ifstream file("../resources/mobile_phone_screen_time_dataset.csv");

    if (!file.is_open()) {
        cerr << "Unable to open file" << endl;
        return -1;
    }

    string line;
    getline(file, line); // get the column headers to skip over them when the loop below begins

    while (getline(file, line)) {
        istringstream iss(line);
        string screentimeStr;

        for (int i = 0; i < 9; i++) // at the end of this loop, screentimeStr will have the data from the 9th column
            getline(iss, screentimeStr, ',');

        data.push_back(stof(screentimeStr));
    }

    for (float& x : data) cout << x << " ";

    return 0;
}