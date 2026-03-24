#pragma once

#include <vector>
#include <algorithm>
#include <functional>
#include <iostream>
#include <random>
using namespace std;

int partition(vector<int>& arr, int low, int high) {
    int mid = low + (high - low) / 2;
    swap(arr[low], arr[mid]);

    int pivot = arr[low];
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

void quickSortHelper(vector<int>& arr, int low, int high) {
    if (low < high) {
        int p = partition(arr, low, high);

        quickSortHelper(arr, low, p);
        quickSortHelper(arr, p + 1, high);
    }
}

void quickSort(vector<int>& arr) {
    quickSortHelper(arr, 0, arr.size() - 1);
}

// runs a test to see if the given sorting function's result is identical to that of the standard sort
// method's and also outputs the sorted array
void runTest(function<void(vector<int>&)> sortFunction, int n) {
    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<> distr(1, 10 * n); // generate random numbers between 1 and 10n

    vector<int> arr(n), copy(n);

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

    cout << endl << endl << (arr == copy ? "Passed" : "Failed") << endl;
}


