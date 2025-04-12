/*
 * @Author: 2405111B08 - 王子睿
 * @Date: 2024-11-26 19:06:28
 * @Description: QuickSort
 */
#include <iostream>
#include <vector>

using namespace std;

void quickSort(int arr[], int *left, int *right)
{
    if (left >= right) return;
    int *l = left, *r = right;
    int pivot = *left;
    while (l < r)
    {
        while (l < r && *r >= pivot) r --;
        if (l < r) *l = *r;
        while (l < r && *l <= pivot) l ++;
        if (l < r) *r = *l;
    }
    *l = pivot;
    quickSort(arr, left, l - 1);
    quickSort(arr, l + 1, right);
}
int main()
{
    int arr[] = {9,5,7,8,4,6,2,1,3};
    quickSort(arr, &arr[0], &arr[8]);
    for (int i = 0; i < 9; i ++) cout << arr[i] << " ";
    cout << endl;
}