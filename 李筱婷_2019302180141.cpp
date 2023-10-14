#include <Windows.h>
#include <stdlib.h>
#include <malloc.h>
#include <string.h>
#include <stdio.h>

#define N 256		//Ҷ�ӽ����
#define M 2*N-1		//���н������

typedef struct
{
	int weight;		//Ȩ��
	int parent;		//˫�׽ڵ�
	int lchild;		//���ӽڵ�
	int rchild;		//�Һ��ӽڵ�
} HTNode;

typedef struct
{
	char* cd;		//��Ź���������
	int Code_long;///�ñ���ĳ���
} HCode;

struct HEAD// �ļ�ͷ
{
	int length;		// ԭ�ļ�����
	int weight[256];	// Ȩֵ��ֵ
};

//�����������
void CreateHT(HTNode ht[])  //��ht��Ҷ�ӽ�㹹�������Ĺ�������
{
	int i, k, lnode, rnode;
	int min1, min2;
	for (i = 0; i < M; i++)			//���н���������ó�ֵ-1
		ht[i].parent = ht[i].lchild = ht[i].rchild = -1;
	for (i = N; i < M; i++)			//������������ķ�֧���
	{
		min1 = min2 = 32767;			//lnode��rnodeΪ��СȨ�ص��������λ��
		lnode = rnode = -1;
		for (k = 0; k <= i - 1; k++)		//������С�ʹ�С�Ľ��
			if (ht[k].parent == -1)	//ֻ����δ����������Ľ���в���
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
		ht[rnode].parent = i;	//�ϲ�������С�ʹ�С�Ľ��
		ht[i].weight = ht[lnode].weight + ht[rnode].weight;
		ht[i].lchild = lnode;
		ht[i].rchild = rnode;
	}
}

//�������������
void DispHCode(HCode hcd[])	//�������������
{
	int i, k;
	printf("����������Ϊ��\n");
	for (i = 0; i < N; i++)
	{
		printf("%d:  ", i);
		for (k = 0; hcd[i].cd[k] != 't'; k++)//�����ڱ�tֹͣ��һ���ڵ�ı��������ϣ�
			printf("%d", hcd[i].cd[k]);
		printf("\n");
	}
}

//�õݹ�ķ������ɹ���������
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

	if (ht[pos].lchild == -1 && ht[pos].rchild == -1)//���Һ��Ӿ�Ϊ��ʱ�ݹ���ֹ
	{
		hcd[pos].cd = (char*)malloc((n + 1) * sizeof(char));
		for (int i = 0; i < n; i++)
			hcd[pos].cd[i] = R[i];
		hcd[pos].Code_long = n;
		hcd[pos].cd[n] = 't';//��t���ڱ�
		return;
	}
	HuffCode(ht, hcd, l, R, n + 1, 0);//��������������
	HuffCode(ht, hcd, r, R, n + 1, 1);//�����������Һ���

}

//���ַ���ת�����ֽ�
char TransSB(char* p)
{
	char b = 0x00;
	for (int i = 0; i < 8; i++)
	{
		b = b << 1;		//����һλ
		if (p[i] == '1')
		{
			b = b | 0x01;
		}
	}
	return b;
}

//�������ļ�
int CreatFile(char* pBuffer, int nSize)
{
	HEAD FHead;//��ʼ��
	FHead.length = 0;
	for (int i = 0; i < N; i++)
		FHead.weight[i] = 0;

	FILE* out = fopen("tt.bmp.huf", "wb");	//�Զ���������ʽ���ļ�
	fwrite(&FHead, sizeof(HEAD), 1, out);	//д�ļ�ͷ
	fwrite(pBuffer, sizeof(char), nSize, out);//дѹ����ı���	
	fclose(out);//�ر��ļ����ͷ��ļ�ָ��
	out = NULL;
	return 1;
}

//ѹ������
int Compress(HCode* pHC, int nSize)
{
	char* pBuffer = NULL;
	FILE* in = fopen("tt.bmp", "rb");
	pBuffer = (char*)malloc(nSize * sizeof(char));//���ٻ�����
	if (!pBuffer)
	{
		printf("���ٻ�����ʧ��");//����ʧ������
		return 0;
	}

	int k, i,j=0;
	char ch;
	int p = 0;//�ֽ�ָ��
	char cd[3500];//�����봢�浽һ���ַ�����
//	printf("������ϢΪ:");
	for (i = 0; i < N; i++)
	{
		k = 0;
		for (; pHC[i].cd[k] != 't'; k++)//tΪ�ڱ�
		{
			cd[j] = pHC[i].cd[k] + '0';
		//	printf("%c", cd[j]);
			j++;//��¼�ַ�������
		}
	}

	while ((ch = fgetc(in)) != EOF)
	{
		while (j >= 8)
		{
			pBuffer[p++] = TransSB(cd);//��ȡ�ַ�����ߵ�8���ַ���������ֽ�
			for (i = 0; i < j - 8; i++)
				cd[i] = cd[8 + i];//�ַ�����������
			j = j - 8; 
		}

	}

	if (j > 0)//�����ַ�����˸�
		pBuffer[p++] = TransSB(cd);
	fclose(in);

	CreatFile(pBuffer, nSize);//�����ļ�
}

int show()
{
	int tempch;
	int i, k;
	int weight[N] = { 0 };
	
	//�򿪲�ɨ���ļ�
	FILE* fp= fopen("tt.bmp", "rb");
	if (fp == NULL)
	{
		printf("���ļ�ʧ��");//��ʧ������
		fclose(fp);
		return 0;
	}
	while ((tempch = getc(fp)) != EOF)//��ȡȨ��
		weight[tempch]++;
	
	printf("ԭ�ļ�ÿ���ַ���Ȩ��Ϊ��\n");//��ʾ256���ֽڳ��ֵĴ���
	for (i = 0; i < N; i++)
		printf("%d:%d\n", i, weight[i]);
	printf("\n");
	fclose(fp);

	
	HTNode pHT[M];
	HCode pHC[N];
	for (i = 0; i <= N; i++)//���θ�ֵ
		pHT[i].weight = weight[i];
	CreateHT(pHT);//�����������
	
	for (k = 0; k < M; k++)//�ҵ����ڵ� 
	{
		if (pHT[k].parent == -1)
			break;
	}
	char Rn[N] = { 0 };
	HuffCode(pHT, pHC, k, Rn, 0, -1);//���ɹ���������
	DispHCode(pHC);//�����������

	//����Ӧ���ɵĻ�������С
	int nSize = 0;
	for (int i = 0; i < N; i++)
		nSize += weight[i] * pHC[i].Code_long;
	nSize = (nSize % 8) ? nSize / 8 + 1 : nSize / 8;//������ȡ��

	Compress(pHC, nSize);//�Ա����ļ�����ѹ��
	return 1;//ѹ���ɹ�
}

int main()
{
	if (show() == 1)
		printf("\nѹ����ɣ�");
	else
		printf("\nѹ��ʧ�ܣ�");
	return 1;
}

