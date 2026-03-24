#pragma once

#include <vector>
#include <algorithm>
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

