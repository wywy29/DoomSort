#pragma once

#include <cmath>
#include <vector>
#include <random>
using namespace std;

float calculateStandardDeviation(const vector<float>& numbers) {
    int n = numbers.size();
    double sum = 0.0;
    for (const float& number : numbers)
        sum += number;

    double mean = sum / n;
    double sumOfDiffSquared = 0;
    for (const float& number : numbers)
        sumOfDiffSquared += (number - mean) * (number - mean);

    return static_cast<float>(sqrt(sumOfDiffSquared / n));
}

void augmentDataUsingNoise(vector<float>& numbers, int newSize) {
    float noise = 0.1f * calculateStandardDeviation(numbers);

    numbers.reserve(newSize);

    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<> indexDist(0, numbers.size() - 1); // get a random index
    normal_distribution<float> dist(0, noise); // calculate noise randomly

    while (numbers.size() < newSize) {
        float randomValue = numbers[indexDist(gen)]; // random value from the array
        randomValue += dist(gen); // add noise

        if (randomValue < 0.f) // ensure value is between 0 and 24 hours
            randomValue = 0.f;
        if (randomValue > 24.f)
            randomValue = 24.f;

        numbers.push_back(round(randomValue * 10.f) / 10.f); // round to 1 decimal places
    }
}

//Resize function to reduce dataset from 100,000 to 100 for a new vector (using K-means clustering)
//Source used as idea for implementing K-means clustering (using floats and vectors instead):
//https://www.geeksforgeeks.org/machine-learning/k-means-clustering-introduction/
void resize(vector<float>& newVector, const vector<float>& data, int k) {
    //Get original data size (Note: For K-means clustering, blobs will be referred to as centroids)
    int dataSize = data.size();

    //Vector for cluster labels to group points together + Boolean which tells if any point switched cluster
    vector<int> clusterLabels(dataSize, 0);
    bool switchMade = true;

    //Start by initializing centroids/blobs using the first k points
    vector<float> centroids(k, 0.0f);
    for(int i = 0; i < k; i++) {
        centroids[i] = data[i];
    }

    //Loop until no switch is made for any point in any cluster (centroids no longer change)
    while(switchMade == true) {
        switchMade = false;

        //Find the closest centroid for each point, assign point to corresponding cluster
        for(int i = 0; i < dataSize; i++) {
            float currMinDist = abs(data[i] - centroids[0]);
            int currClosestCluster = 0;

            for(int j = 1; j < k; j++) {
                float currClusterDist = abs(data[i] - centroids[j]);

                //If current distance of point from the current cluster is less than minimum distance, replace it
                if(currClusterDist < currMinDist) {
                    currClosestCluster = j;
                    currMinDist = currClusterDist;
                }
            }

            //Check if a switch is made for a point in a cluster
            if(clusterLabels[i] != currClosestCluster) {
                switchMade = true;
                clusterLabels[i] = currClosestCluster;
            }

        }

        //Update centroids, using cluster sum and point count, to be the average of newly assigned points
        vector<float> clusterSum(k, 0.0f);
        vector<int> clusterPointCount(k, 0);

        for(int i = 0; i < dataSize; i++) {
            int currLabel = clusterLabels[i];
            clusterSum[currLabel] += data[i];
            clusterPointCount[currLabel]++;
        }

        for(int j = 0; j < k; j++) {
            if(clusterPointCount[j] != 0) {
                centroids[j] = round((clusterSum[j] / static_cast<float>(clusterPointCount[j])) * 10.f) / 10.f; //one decimal place rounding
            }
        }
    }

    //New vector of 100 blobs will be stored as the centroids
    newVector = centroids;
}
