#pragma once

#include <vector>
#include <algorithm>
#include <functional>
#include <iostream>
#include <random>
using namespace std;

int partition(vector<float>& arr, int low, int high) {
    int mid = low + (high - low) / 2;
    swap(arr[low], arr[mid]);

    float pivot = arr[low];
    int i = low, j = high;

    while (true) {
        while (arr[i] < pivot) i++;

        while (arr[j] > pivot) j--;

        if (i >= j) return j;

        swap(arr[i], arr[j]);

        i++;
        j--;
    }
}

void quickSortHelper(vector<float>& arr, int low, int high) {
    if (low < high) {
        int p = partition(arr, low, high);

        quickSortHelper(arr, low, p);
        quickSortHelper(arr, p + 1, high);
    }
}

void quickSort(vector<float>& arr) {
    quickSortHelper(arr, 0, arr.size() - 1);
}

/// @brief Runs a test on both quick and merge sort functions to check if they work. Note that the tests must be called in main() with the appropriate function names.
/// @param sortFunction the name of the sorting function with the exact same signature
/// @param n number of elements you want in the array that's used to test your function
void runTest(function<void(vector<float>&)> sortFunction, int n) {
    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<> distr(1, 10 * n); // generate random numbers between 1 and 10n

    vector<float> arr(n), copy(n);

    for (int i = 0; i < n; i++) {
        arr[i] = distr(gen);
        copy[i] = arr[i];
    }

    cout << "Before sorting" << endl;
    for (int i = 0; i < n; i++) cout << arr[i] << " ";

    sortFunction(arr);
    sort(copy.begin(), copy.end());

    cout << endl << endl << "After sorting" << endl;
    for (int i = 0; i < n; i++) cout << arr[i] << " ";

    cout << endl << endl << "Test " << (arr == copy ? "passed" : "failed") << endl;
}


