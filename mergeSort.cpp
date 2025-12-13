#include"mergeSort.h"
#include<vector>
#include<thread>
#include<iostream>

using namespace std;

void printArray(const vector<int> &arr){
    int n = arr.size();
    cout<< "Array size: " << n << endl;
    cout<<"[";
    for(int i=0; i <n-1; i++){
        cout<< arr[i] << " ";
    }
    cout<< arr[n-1] << "]"<< endl;
}

//Set random values in the arrays- two identical arrays for sorting speed comparison
void setRandomValues(vector<int> &arr1, vector<int> &arr2, int size){
    if((int)arr1.size() != size || (int)arr2.size() != size){
        cerr<< "Error: Array sizes do not match the specified size." << endl;
        return;
    }
    for (int i = 0; i < size; ++i) {
        // Random numbers between 0 and 9999
        arr1[i] = rand() % 10000; 
        arr2[i] = arr1[i]; 
    }
}

//Wrapper for merge sort implementation with 1 thread
void singleSort(vector<int> &arr){
    mergeSort(arr, 0, arr.size() - 1);
}

//Merge Sort implementation with 1 thread
void mergeSort(vector<int> &arr, int left, int right){
    if(left>=right) return;

    int mid= (left + right) / 2;
    mergeSort(arr, left, mid);
    mergeSort(arr, mid + 1, right);
    merge(arr, left, mid, right);

}

void merge(vector<int> &arr, int left, int mid, int right){
    vector<int> leftArr(arr.begin() + left, arr.begin() + mid + 1);
    vector<int> rightArr(arr.begin() + mid + 1, arr.begin() + right + 1);

    int l=0, r=0, i=left;
    int leftSize= leftArr.size();
    int rightSize= rightArr.size();

    while(l<leftSize && r<rightSize){
        if(leftArr[l]<= rightArr[r]){
            arr[i]= leftArr[l];
            l++;
        }else{
            arr[i]= rightArr[r];
            r++;
        }
        i++;
    }
    while(l<leftSize){
        arr[i]= leftArr[l];
        l++;
        i++;
    }
    while(r<rightSize){
        arr[i]= rightArr[r];
        r++;
        i++;
    }
    
}

int sum(int a, int b) {
    return a + b;
}

//Test main function
// int main(){
//     int n= 5000;
//     vector<int> arr(n);
//     vector<int> arr2(n);
//     setRandomValues(arr, arr2, n);
//     printArray(arr);
//     singleSort(arr);
//     printArray(arr);
//     cout<<thread::hardware_concurrency()<<endl;
//     return 0;
// }