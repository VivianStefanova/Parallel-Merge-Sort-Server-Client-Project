#ifndef MERGESORT_H
#define MERGESORT_H

#include <vector>

void printArray(const std::vector<int> &arr);
void setRandomValues(std::vector<int> &arr1, std::vector<int> &arr2, int n);

void singleSort(std::vector<int> &arr);
void mergeSort(std::vector<int> &arr, int left, int right);
void merge(std::vector<int> &arr, int left, int mid, int right);

int sum(int a, int b);

#endif // MERGESORT_H