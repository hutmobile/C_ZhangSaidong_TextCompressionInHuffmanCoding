//
//  main.c
//  TextCompressionInHuffmanCoding
//
//  Created by adong666666 on 2018/12/28.
//  Copyright © 2018 adong666666. All rights reserved.
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>//专门用于检测整型数据数据类型的表达值范围
#define N 96 //ASCII字符集包含至多N个可见字符
typedef struct    //Huffman树节点定义
{ char data; //字符值
        int weight; //权重
        int lchild; //左子结点
        int rchild; //右子结点
} huffnode; //huffman节点类型
struct charcode
{ int count; //字符出现的次数(频率)
        char code[N]; //字符的Huffman编码
} codeset[N]; //编码表,长为N,每项对应一个ascii码字符,下标i的项对应ascii编码为i+32的字符
huffnode * CreateHufftree(char data[], int weight[], int n)   //建立Huffman树的算法
{
        int i,k;
        int min1,min2,min_i1,min_i2;
        huffnode *tree;
        tree=(huffnode *)malloc((2*n-1)*sizeof(huffnode));  //为Huffman树分配2n-1个节点空间
        for (i=0;i<2*n-1;i++)  //初始化,将各字符和其频率填入Huffman树,作为叶子结点
            {
                    tree[i].lchild=tree[i].rchild=-1;
                    if (i<n) {
                            tree[i].data=data[i];
                            tree[i].weight=weight[i];
                        }
                    else tree[i].data=' ';
                }
        for (i=n;i<2*n-1;i++)   ////合并两棵树,作n-1遍
            {
                    min1=min2=INT_MAX; //INT_MAX为最大值
                    min_i1=min_i2=-1;
                    for (k=0;k<i;k++)  ////查找定位两个最小权重节点
                            if (tree[k].weight>=0)  //仅在根节点中找
                                {
                                        if (tree[k].weight<min1)
                                            {
                                                    min2=min1;
                                                    min_i2=min_i1;
                                                    min1=tree[k].weight;
                                                    min_i1=k;
                                                }
                                        else
                                                if (tree[k].weight<min2) {
                                                        min2=tree[k].weight;
                                
                                                        min_i2=k;
                                                    }
                                    }
                    tree[i].weight=min1+min2;  // 合并
                    tree[min_i1].weight *= -1;
                    tree[min_i2].weight *= -1;
                    tree[i].lchild=min_i1;
                    tree[i].rchild=min_i2;
                }
        return tree;
}
void CreateHuffcode(huffnode tree[], int i, char s[])//已知tree[i]节点的编码序列为s,求该节点下所有叶子节点的编码序列。
{ char s1[N],c;
        if(i!=-1)
        
            {
                    if (tree[i].lchild==-1 && tree[i].rchild==-1) {
                
                            c=tree[i].data;
                            strcpy(codeset[c-32].code, s);
                        }
                    else {
                            strcpy(s1, s);  strcat(s1, "0");
                            CreateHuffcode(tree, tree[i].lchild, s1);
                            strcpy(s1, s);  strcat(s1, "1");
                            CreateHuffcode(tree, tree[i].rchild, s1);
                        }
                }
        return;
    
}


void PrintHufftree(huffnode tree[], int n)      //输出tree中的Huffman树
{
        int i;
        printf("Huffman tree :\n");
        printf("i\tValue\tLchild\tRchild\tWeight\n");
        for(i=2*n-2;i>=0;i--)
            {
                    printf("%d\t",i);
            
                    printf("%c\t",tree[i].data);
                    printf("%d\t",tree[i].lchild);
                    printf("%d\t",tree[i].rchild);
                    printf("%d\t",tree[i].weight);
                    printf("\n");
                }
}
void EnCoding(char str[], char hfmstr[])
{//根据codeset编码表,逐个将str字符串中的字符转化为它的huffman编码,结果编码串放在hfmstr字符串中
        int i, j;
        hfmstr[0]='\0';//把hfmstr串赋空
        i=0;
        while(str[i]!='\0')//从第头开始扫描str的每个字符,一直到该字符的结束
            {
                    j=str[i]-32;//执行字符到huffman的转换
                    strcat(hfmstr, codeset[j].code);//把codest编码串添加到hfmstr结尾处
                    i++;//每次循环完i的值加1
                }
}
void DeCoding(huffnode tree[], int n, char hfmstr[], char decodestr[])
//根据tree数组中的huffman树,逐个对hfmstr字符串中的字符进行译码,结果放在decodestr字符串中
{
        int i=0,j=0;
    
        huffnode p;
    
        p=tree[2*n-2];//序号2*n-2节点就是树根节点
        while(hfmstr[i]!='\0')//从头开始扫描每个字符,直到结束
            {while(p.lchild!=-1&&p.rchild!=-1)//指针为空，儿子的值取完了
                {
                        if(hfmstr[i]=='0')//为0则向左子树走
                            {
                                    p=tree[p.lchild];//取出叶子节点中的字符
                                }
                        else if(hfmstr[i]=='1')//为1则向右子树走
                    
                            {
                                    p=tree[p.rchild];//取出叶子节点中的字符
                                }
                        i++;
                    }
                    decodestr[j]=p.data;j++;//对字符进行译码,结果放在decodestr字符串中
                    p=tree[2*n-2];//返回根节点
                }
}

int main()
{
        int i,j;
    
        huffnode * ht; //Huffman树
        char data[N]; //要编码的字符集合
        int  weight[N]; //字符集合中各字符的权重(频率)
        int n=0; //字符集合中字符的个数
        char str[1000];         //需输入的原始字符串
        char hfm_str[1000]=""; //编码后的字符串
        char decode_str[1000]="";//解码后的字符串
        printf("请输入要转换的字符串\n");
        gets(str);
    
        for(i=0;i<N;i++) { //初始化编码表,频率为0,编码串为空串
        
                codeset[i].count=0;
                codeset[i].code[0]='\0';
        
        
            }
        i=0;
        while(str[i]!='\0') { //统计原始字符串中字符出现的频率,存入编码表
                j=str[i]-32;
                codeset[j].count++; //codeset[0]~[95]对应ascii码32~127的字符
                i++;
            }
        for(i=0;i<N;i++) //统计原始字符串中出现的字符个数
                if(codeset[i].count!=0) n++;
        printf("字符频率统计:\n"); //显示统计结果
        for(i=0;i<N;i++)
                if(codeset[i].count!=0) printf("%c:%d, ", i+32, codeset[i].count);
        printf("\n");
        j=0;
        for(i=0;i<N;i++) //生成要编码的字符集合,以及权重
                if (codeset[i].count!=0) {
                        data[j]=i+32;
                        weight[j]=codeset[i].count;
                        j++;
                    }
        ht=CreateHufftree(data,weight,n);  //建立Huffman树,根节点是ht[2*n-2]
        PrintHufftree(ht,n); //显示Huffman树的存储结果
        CreateHuffcode(ht, 2*n-2, "");  //以ht[2*n-2]为根,以空字符串为起始编码字符串,求出各叶子节点的编码字符串
        //显示codeset中的Huffman编码,参见"显示频率统计结果"的代码.
        printf("haffman编码为:\n");
        for(i=0;i<N;i++){
                if(codeset[i].count!=0)
                        printf("%c:%s\n",i+32,codeset[i].code );
            }
        EnCoding(str, hfm_str);  //对str字符串进行编码,放在hfm_str字符串
        printf("编码序列: %s\n",hfm_str);
        DeCoding(ht, n, hfm_str, decode_str); //对hfm_str字符串进行译码,放在decode_str字符串中
        printf("解码后的字符串: %s\n",decode_str);
        free(ht);  //释放Huffman树
}
