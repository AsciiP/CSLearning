#include<iostream>
using namespace std;
int main()
{
    int arr[17]={7,9,10,5,8,4,2,1,6,3,7,9,10,5,8,4,2};
    int N=100;
    string b;
    for(int j=0;j<N;j++)
  {
    for(int i=0;i<18;i++)
    {
        cin>> b;
        if(b[i]>='0'&&b[i]<='9')
        {
           int b[i]=(string) b;
        }
        else
            cout<<b[i];
    }
    for(int i=0;i<17;i++)
    {
        int sum=0;
        sum+=b[i]*arr[i];
    }
    char a[2][11]={'0','1','2','3','4','5','6','7','8','9','10',
           '1','0','X','9','8','7','6','5','4','3','2'}
    char Z[11]=a[0][11];
    char M[11]=a[1][11];
  }
    int i;
    Z[i]=sum%11;
    for(int j=0;j<N;j++)
    {
        if(M[i]=b[j][18])
        cout<<"All passed";
        else
        cout<<b[j][i];
    }
    return 0;
}