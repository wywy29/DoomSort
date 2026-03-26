#include "sort.h"
#include "helpers.h"
#include "ui.cpp"
#include <fstream>
#include <sstream>

int main() {
    //HomeScreenUI hs;
    //hs.show(); // test homescreen

    // runTest(quickSort, 100); // a quick test to see if quickSort() works or not

    vector<float> data;
    data.reserve(10000); // reserve space for 10000 data points to prevent constant resizing
    ifstream file("../resources/mobile_phone_screen_time_dataset.csv");

    if (!file.is_open()) {
        cerr << "Error: Unable to open file." << endl;
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

    augmentDataUsingNoise(data, 100000);
    //for (float& x : data) cout << x << " "; //Print the 100,000 data (10,000 read and the rest augmented)

    //Create a new vector containing reduced number of data points (100,000 to 100)
    //Done for colored blobs (each represent 1000 data points - not including user's blob)
    vector<float> resizedData;
    int reducedSize = 100;

    //Call resize function to reduce dataset using K-means clustering and add to resizedData vector
    resize(resizedData, data, reducedSize);

    //Print resize function results
    for(float d : resizedData) {
        cout << d << endl;
    }

    return 0;
}