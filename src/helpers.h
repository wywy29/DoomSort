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

        numbers.push_back(randomValue);
    }
}

//Resize function to reduce dataset from 100,000 to 100 for a new vector
void resize(vector<float>& newVector, const vector<float>& data, int newSize) {
    /*for (int i = 0; i < newSize; i++) {

    }*/
}
