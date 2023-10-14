#include <Windows.h>
#include <stdlib.h>
#include <malloc.h>
#include <string.h>
#include <stdio.h>

#define N 256		//叶子结点数
#define M 2*N-1		//树中结点总数

typedef struct
{
	int weight;		//权重
	int parent;		//双亲节点
	int lchild;		//左孩子节点
	int rchild;		//右孩子节点
} HTNode;

typedef struct
{
	char* cd;		//存放哈夫曼编码
	int Code_long;///该编码的长度
} HCode;

struct HEAD// 文件头
{
	int length;		// 原文件长度
	int weight[256];	// 权值数值
};

//构造哈夫曼树
void CreateHT(HTNode ht[])  //由ht的叶子结点构造完整的哈夫曼树
{
	int i, k, lnode, rnode;
	int min1, min2;
	for (i = 0; i < M; i++)			//所有结点的相关域置初值-1
		ht[i].parent = ht[i].lchild = ht[i].rchild = -1;
	for (i = N; i < M; i++)			//构造哈夫曼树的分支结点
	{
		min1 = min2 = 32767;			//lnode和rnode为最小权重的两个结点位置
		lnode = rnode = -1;
		for (k = 0; k <= i - 1; k++)		//查找最小和次小的结点
			if (ht[k].parent == -1)	//只在尚未构造二叉树的结点中查找
			{
				if (ht[k].weight < min1)
				{
					min2 = min1;
					rnode = lnode;
					min1 = ht[k].weight;
					lnode = k;
				}
				else if (ht[k].weight < min2)
				{
					min2 = ht[k].weight;
					rnode = k;
				}
			}
		ht[lnode].parent = i;
		ht[rnode].parent = i;	//合并两个最小和次小的结点
		ht[i].weight = ht[lnode].weight + ht[rnode].weight;
		ht[i].lchild = lnode;
		ht[i].rchild = rnode;
	}
}

//输出哈夫曼编码
void DispHCode(HCode hcd[])	//输出哈夫曼编码
{
	int i, k;
	printf("哈弗曼编码为：\n");
	for (i = 0; i < N; i++)
	{
		printf("%d:  ", i);
		for (k = 0; hcd[i].cd[k] != 't'; k++)//遇到哨兵t停止（一个节点的编码输出完毕）
			printf("%d", hcd[i].cd[k]);
		printf("\n");
	}
}

//用递归的方法生成哈夫曼编码
void HuffCode(HTNode ht[], HCode hcd[], int pos, char R[], int n, int m)
{
	if (m == -1)
		;
	else if (m == 1)
		R[n - 1] = 1;
	else
		R[n - 1] = 0;
	int l = ht[pos].lchild;
	int r = ht[pos].rchild;

	if (ht[pos].lchild == -1 && ht[pos].rchild == -1)//左右孩子均为空时递归终止
	{
		hcd[pos].cd = (char*)malloc((n + 1) * sizeof(char));
		for (int i = 0; i < n; i++)
			hcd[pos].cd[i] = R[i];
		hcd[pos].Code_long = n;
		hcd[pos].cd[n] = 't';//用t做哨兵
		return;
	}
	HuffCode(ht, hcd, l, R, n + 1, 0);//将函数推向左孩子
	HuffCode(ht, hcd, r, R, n + 1, 1);//将函数推向右孩子

}

//将字符串转换成字节
char TransSB(char* p)
{
	char b = 0x00;
	for (int i = 0; i < 8; i++)
	{
		b = b << 1;		//左移一位
		if (p[i] == '1')
		{
			b = b | 0x01;
		}
	}
	return b;
}

//生成新文件
int CreatFile(char* pBuffer, int nSize)
{
	HEAD FHead;//初始化
	FHead.length = 0;
	for (int i = 0; i < N; i++)
		FHead.weight[i] = 0;

	FILE* out = fopen("tt.bmp.huf", "wb");	//以二进制流形式打开文件
	fwrite(&FHead, sizeof(HEAD), 1, out);	//写文件头
	fwrite(pBuffer, sizeof(char), nSize, out);//写压缩后的编码	
	fclose(out);//关闭文件，释放文件指针
	out = NULL;
	return 1;
}

//压缩编码
int Compress(HCode* pHC, int nSize)
{
	char* pBuffer = NULL;
	FILE* in = fopen("tt.bmp", "rb");
	pBuffer = (char*)malloc(nSize * sizeof(char));//开辟缓冲区
	if (!pBuffer)
	{
		printf("开辟缓冲区失败");//开辟失败提醒
		return 0;
	}

	int k, i,j=0;
	char ch;
	int p = 0;//字节指针
	char cd[3500];//将编码储存到一个字符串中
//	printf("编码信息为:");
	for (i = 0; i < N; i++)
	{
		k = 0;
		for (; pHC[i].cd[k] != 't'; k++)//t为哨兵
		{
			cd[j] = pHC[i].cd[k] + '0';
		//	printf("%c", cd[j]);
			j++;//记录字符串长度
		}
	}

	while ((ch = fgetc(in)) != EOF)
	{
		while (j >= 8)
		{
			pBuffer[p++] = TransSB(cd);//截取字符串左边的8个字符，编码成字节
			for (i = 0; i < j - 8; i++)
				cd[i] = cd[8 + i];//字符串整体左移
			j = j - 8; 
		}

	}

	if (j > 0)//余下字符不足八个
		pBuffer[p++] = TransSB(cd);
	fclose(in);

	CreatFile(pBuffer, nSize);//生成文件
}

int show()
{
	int tempch;
	int i, k;
	int weight[N] = { 0 };
	
	//打开并扫描文件
	FILE* fp= fopen("tt.bmp", "rb");
	if (fp == NULL)
	{
		printf("打开文件失败");//打开失败提醒
		fclose(fp);
		return 0;
	}
	while ((tempch = getc(fp)) != EOF)//获取权重
		weight[tempch]++;
	
	printf("原文件每个字符的权重为：\n");//显示256种字节出现的次数
	for (i = 0; i < N; i++)
		printf("%d:%d\n", i, weight[i]);
	printf("\n");
	fclose(fp);

	
	HTNode pHT[M];
	HCode pHC[N];
	for (i = 0; i <= N; i++)//依次赋值
		pHT[i].weight = weight[i];
	CreateHT(pHT);//构造哈夫曼树
	
	for (k = 0; k < M; k++)//找到根节点 
	{
		if (pHT[k].parent == -1)
			break;
	}
	char Rn[N] = { 0 };
	HuffCode(pHT, pHC, k, Rn, 0, -1);//生成哈夫曼编码
	DispHCode(pHC);//输出哈夫曼码

	//计算应生成的缓冲区大小
	int nSize = 0;
	for (int i = 0; i < N; i++)
		nSize += weight[i] * pHC[i].Code_long;
	nSize = (nSize % 8) ? nSize / 8 + 1 : nSize / 8;//除八上取整

	Compress(pHC, nSize);//对编码文件进行压缩
	return 1;//压缩成功
}

int main()
{
	if (show() == 1)
		printf("\n压缩完成！");
	else
		printf("\n压缩失败！");
	return 1;
}

