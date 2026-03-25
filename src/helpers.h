#pragma once

#include <cmath>
#include <vector>
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