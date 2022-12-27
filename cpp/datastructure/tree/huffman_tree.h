/**
哈夫曼树
又称最优二叉树，是一种带权路径长度最小的二叉树。
   所谓树的带权路径长度，就是树中所有的叶子结点的权
   值乘上其到根结点的路径长度之和。
头文件：
```
struct HNode // 静态三叉链表结点类型
{
    int weight;
    int parent;
    int LChild;
    int RChild;
};
struct HCode  // 哈夫曼编码表
{
    char data;
    char code[100];
};
class Huffman{
private:
    HNode* HTree;
    HCode* HCodeTable;
protected:
    // 选取权值最小的两个结点（x,y为游标）
    void SelectMin(int&x,int&y,int start,int NodeNum);  // !
    void Reverse(char *);    // ! 字符串逆序："abc"->"cba"
public:
    void CreateHTree(int a[],int n);
    void CreateCodeTable(char b[],int n);
    void Encode(char *s, char *d);  // !
    void Decode(char *s, char *d,int n);
    ~Huffman(){}
};
```
头文件的实现：
```
#include "huffman.h"
/*
功能：创建哈夫曼树HTree
输入：a[] - 字符权值（从小到大排序）
      n - 字符个数（叶子数）

void Huffman::CreateHTree(int a[],int n)
{
    int x,y,i;
    HTree= new HNode[2*n-1];
    for(i=0;i<n;i++)
    {
        HTree[i].weight=a[i];
        HTree[i].LChild=-1;
        HTree[i].RChild=-1;
        HTree[i].parent=-1;
    }
    for(i=n; i<2*n-1;i++)
    {
        SelectMin(x,y,0,i);    // !
        HTree[x].parent=HTree[y].parent=i;
        HTree[i].weight=HTree[x].weight+HTree[y].weight;
        HTree[i].LChild=x;
        HTree[i].RChild=y;
        HTree[i].parent=-1;
    }
}
/*
功能：构建哈夫曼编码表HCodeTable
输入：b[] - 字符表（已按权值从小到大排序）
      n - 字符个数

void Huffman::CreateCodeTable(char b[],int n)
{
    HCodeTable=new HCode[n];  // 定义编码表
    for(int i=0; i<n; i++)
    {
        HCodeTable[i].data=b[i];
        int child=i;
        int parent=HTree[i].parent;
        int k=0;
        while(parent!=-1)
        {
            if(child==HTree[parent].LChild)
                HCodeTable[i].code[k]='0';
            else
                HCodeTable[i].code[k]='1';
            k++;
            child=parent;
            parent=HTree[child].parent;
        }
        HCodeTable[i].code[k]='\0';
        Reverse(HCodeTable[i].code);
    }
}
/*
功能：对指定字符串进行哈夫曼编码
形参：s - 输入字符串
      d - 字符串的哈夫曼编码

void Huffman::Encode(char *s, char *d)
{
}
/*
功能：解码哈夫曼编码串为字符串
形参：s - 哈夫曼编码串
      d - 解码后的字符串
      n - 哈夫曼树叶子个数

void Huffman::Decode(char *s, char *d,int n)
{
    while(*s!='\0')
    {
        int parent=2*n-2;
        while(HTree[parent].LChild!=-1)
        {
            if(*s=='0')
                parent=HTree[parent].LChild;
            else
                parent=HTree[parent].RChild;
            s++;
        }
        *d=HCodeTable[parent].data;
        d++;
    }
    *d='\0';
}
/*
功能：为构建哈夫曼树，搜索权值最小的两个结点
形参：x- 最小权值结点游标
      y- 次小权值结点游标
      NodeNum- 权值表长度（权值个数）
      start- 搜索起点（游标）

void Huffman::SelectMin(int&x,int&y,int start,int NodeNum)
{

}
// 字符串反序："abc"->"cba"
void Huffman::Reverse(char *str)
{

}
```
测试文件：
```
#include "huffman.h"

#include "iostream"
using namespace std;

void main(void)
{
    char str[]="ZCBA";  // 000001011
    int weight[]={2,3,6,9};
    char strTest[]="0011100101"; // CAACB
    // ...

}
```

*/