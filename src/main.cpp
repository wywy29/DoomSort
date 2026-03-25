#include "sort.h"
#include "helpers.h"
#include <fstream>
#include <sstream>

int main() {
    // runTest(quickSort, 100); // a quick test to see if quickSort() works or not

    vector<float> data;
    data.reserve(10000); // reserve space for 10000 data points to prevent constant resizing
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

    augmentDataUsingNoise(data, 100000);
    //for (float& x : data) cout << x << " "; //Print the 100,000 data (10,000 read and the rest augmented)

    //Note from Dylan: The daily screen time ranges from 0-13.1 hours, I don't think its a problem
    //because you would probably suffer a lot of consequences for using a device for a little over 13 hours lol

    //Create a new vector containing reduced number of data points (100,000 to 100)
    //Done for blobs
    vector<float> resizedData;
    int reducedSize = 100;

    //Call resize function to reduce dataset and add to above vector
    resize(resizedData, data, reducedSize);

    return 0;
}