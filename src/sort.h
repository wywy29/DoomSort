#pragma once

#include <vector>
#include <algorithm>
#include <functional>
#include <iostream>
#include <random>
#include "ui.h"
using namespace std;

//Quicksort Helper for partitioning the vector using pivot element
int partition(vector<Blob>& arr, int low, int high) {
    int mid = low + (high - low) / 2;
    swap(arr[low], arr[mid]);

    float pivot = arr[low].radius;
    int i = low, j = high;

    while (true) {
        while (arr[i].radius < pivot) i++;

        while (arr[j].radius > pivot) j--;

        if (i >= j) return j;

        swap(arr[i], arr[j]);

        i++;
        j--;
    }
}
//Helper for quicksort using recursion
void quickSortHelper(vector<Blob>& arr, int low, int high) {
    if (low < high) {
        int p = partition(arr, low, high);

        quickSortHelper(arr, low, p);
        quickSortHelper(arr, p + 1, high);
    }
}

//Quick Sort function, uses helper to achieve purpose
void quickSort(vector<Blob>& arr) {
    quickSortHelper(arr, 0, arr.size() - 1);
}

//Merge sort helper that merges two halves of vector into one whole section
void merge(vector<Blob>& arr, int left, int mid, int right) {
    vector<Blob> leftTemp(arr.begin() + left, arr.begin() + mid + 1);
    vector<Blob> rightTemp(arr.begin() + mid + 1, arr.begin() + right + 1);

    int i = 0;
    int j = 0;
    int k = left;

    while (i < leftTemp.size() && j < rightTemp.size()) {
        if (leftTemp[i].radius <= rightTemp[j].radius) {
            arr[k] = leftTemp[i];
            i++;
        }
        else {
            arr[k] = rightTemp[j];
            j++;
        }
        k++;
    }
    while (i < leftTemp.size()) {
        arr[k] = leftTemp[i];
        i++;
        k++;
    }

    while (j < rightTemp.size()) {
        arr[k] = rightTemp[j];
        j++;
        k++;
    }
}

//Merge Sort helper that uses recursion
void mergeSortHelper(vector<Blob>& arr, int left, int right) {
    if (left < right) {
        int mid = left + (right - left) / 2;
        mergeSortHelper(arr, left, mid);
        mergeSortHelper(arr, mid + 1, right);
        merge(arr, left, mid, right);
    }
}

//Merge Sort function that calls helper to do the sorting
void mergeSort(vector<Blob>& arr) {
    mergeSortHelper(arr, 0, arr.size() - 1);
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
