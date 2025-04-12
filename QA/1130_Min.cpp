/*
 * @Author: 2405111B08 - 王子睿
 * @Date: 2024-11-30 20:11:11
 * @Description: 
 */
#include<iostream>
#include<vector>
using namespace std;
int main()
{
    int n;
    cin>>n;
    vector<int>arr(n);
    for(int i=0;i<n;++i)
    {
        cin>>arr[i];
    }

    int min=arr[0];
    int minIndex = 0;
    for(int i = 1; i < n; ++i) {
        if(arr[i] < min) {
            min = arr[i];
            minIndex = i;
        }
    }
    cout<<min<<" "<<minIndex;
    cout<<endl;
    return 0;
}