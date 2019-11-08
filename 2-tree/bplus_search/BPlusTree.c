#include <stdio.h>
#include <time.h>
#include "BPlusTree.h"
#include < assert.h >
/*****��̬�ڴ�******/
void* bupt_malloc(size_t size) {
	if (size <= 0)
	{
		return NULL;
	}

	global_stats.mem += size;
	return malloc(size);
}
/*****�ַ��Ƚ�******/
int byte_cmp(char a, char b) {
	global_stats.cmpnum++;
	return a - b;
}
/*****�ַ����Ա�******/
int bupt_strcmp(const char* str1, const char* str2)
{
	while (byte_cmp(*str1, *str2) == 0)
	{
		assert((str1 != NULL) && (str2 != NULL));
		if (*str1 == '\0')
			return 0;
		str1++;
		str2++;
	}
	return *str1 - *str2;
}
/* ���ɽڵ㲢��ʼ�� */
static BPlusTree MallocNewNode() {
	BPlusTree NewNode;
	int i=0;
	NewNode = bupt_malloc(sizeof(struct BPlusNode));//����BPlusTree���͵��ڴ�ռ�
	if (NewNode == NULL)
	{
		printf("�������ʧ��");
		return NULL;
	}

	while (i < M + 1) {
		NewNode->Key[i] = NULL;
		NewNode->Children[i] = NULL;
		i++;
	}
	NewNode->Next = NULL;
	NewNode->KeyNum = 0;
	global_stats.node_num++;
	return NewNode;
}

/* ��ʼ�� */
extern BPlusTree Initialize() {

	BPlusTree T;
	if (M < (3)) {
		printf("M��С����3��");
		exit(EXIT_FAILURE);

	}
	/* ����� */
	T = MallocNewNode();
	return T;
}
//��λ���ýڵ�P����С����ߣ����� 
static Position FindMostLeft(Position P) {
	Position Tmp;

	Tmp = P;

	while (Tmp != NULL && Tmp->Children[0] != NULL) {
		Tmp = Tmp->Children[0];
	}
	return Tmp;
}
//��λ���ýڵ�P������ұߣ����� 
static Position FindMostRight(Position P) {
	Position Tmp;

	Tmp = P;

	while (Tmp != NULL && Tmp->Children[Tmp->KeyNum - 1] != NULL) {
		Tmp = Tmp->Children[Tmp->KeyNum - 1];//�ؼ��ִ�0��ʼ�����ΪKeyNum - 1
	}
	return Tmp;
}

/* Ѱ��һ���ֵܽڵ㣬��洢�Ĺؼ���δ�������򷵻�NULL */
static Position FindSibling(Position Parent, int i) {
	Position Sibling;
	int Limit;

	Limit = M;

	Sibling = NULL;
	if (i == 0) {
		if (Parent->Children[1]->KeyNum < Limit)
			Sibling = Parent->Children[1];
	}//�ȿ���ߵ��ֵܽڵ�ؼ�����û��
	else if (Parent->Children[i - 1]->KeyNum < Limit)
		Sibling = Parent->Children[i - 1];//�������ֵܹؼ���û�����ٿ��ұߵ��ֵ�
	else if (i + 1 < Parent->KeyNum && Parent->Children[i + 1]->KeyNum < Limit) {
		Sibling = Parent->Children[i + 1];
	}

	return Sibling;
}

/* ��Ҫ��X����Key��ʱ��i��X��Parent��λ�ã�j��KeyҪ�����λ��
   ��Ҫ��Parent����X�ڵ��ʱ��i��Ҫ�����λ�ã�Key��j��ֵû����
 */
static Position InsertElement(int isKey, Position Parent, Position X, KeyType Key, int i, int j) {

	int k;
	
	if (isKey) {//�Ƿ����key����1����0
		/* ����key */
		k = X->KeyNum - 1;
		for (k; k >= j;k--) {
			X->Key[k + 1] = X->Key[k];
		}

		//strcpy(X->Key[j], Key);
		X->Key[j] = Key;

		if (Parent != NULL)
			Parent->Key[i] = X->Key[0];
		//�ýڵ��ڸ��ڵ��λ�õ�keyֵ���ڸýڵ�����ڵ��ֵ
		X->KeyNum++;

	}
	else {
		/* ����ڵ� */

		if (X->Children[0] == NULL) {//�½ڵ���뵽��Ҷ��
			if (i > 0)
				Parent->Children[i - 1]->Next = X;
			X->Next = Parent->Children[i];
		}

		k = Parent->KeyNum - 1;
		while (k >= i) {//���ڵ���λ��λ��
			Parent->Children[k + 1] = Parent->Children[k];
			Parent->Key[k + 1] = Parent->Key[k];
			k--;
		}
		Parent->Key[i] = X->Key[0];
		Parent->Children[i] = X;
		Parent->KeyNum++;
		
	}
	return X;
}

static Position RemoveElement(int isKey, Position Parent, Position X, int i, int j) {

	int k, Limit;

	if (isKey) {
		Limit = X->KeyNum;
		/* ɾ��key */
		k = j + 1;
		while (k < Limit) {
			X->Key[k - 1] = X->Key[k]; k++;
		}

		X->Key[X->KeyNum - 1] = NULL;

		Parent->Key[i] = X->Key[0];

		X->KeyNum--;
	}
	else {
		/* ɾ���ڵ� */

		/* �޸���Ҷ�ڵ������ */
		if (X->Children[0] == NULL && i > 0) {
			Parent->Children[i - 1]->Next = Parent->Children[i + 1];
		}
		Limit = Parent->KeyNum;
		k = i + 1;
		while (k < Limit) {
			Parent->Children[k - 1] = Parent->Children[k];
			Parent->Key[k - 1] = Parent->Key[k];
			k++;
		}

		Parent->Children[Parent->KeyNum - 1] = NULL;
		Parent->Key[Parent->KeyNum - 1] = NULL;

		Parent->KeyNum--;
		global_stats.node_num--;
	}
	return X;
}

/* Src��Dst���������ڵĽڵ㣬i��Src��Parent�е�λ�ã�
 ��Src��Ԫ���ƶ���Dst�� ,n���ƶ�Ԫ�صĸ���*/
static Position MoveElement(Position Src, Position Dst, Position Parent, int i, int n) {
	KeyType TmpKey;
	Position Child;
	int j, SrcInFront;

	SrcInFront = 0;

	if (bupt_strcmp(Src->Key[0] , Dst->Key[0])<0)
		SrcInFront = 1;

	j = 0;
	/* �ڵ�Src��Dstǰ�� */
	if (SrcInFront) {
		if (Src->Children[0] != NULL) {//�м�ڵ�
			while (j < n) {
				Child = Src->Children[Src->KeyNum - 1];
				RemoveElement(0, Src, Child, Src->KeyNum - 1, NULL);
				InsertElement(0, Dst, Child, NULL, 0, NULL);
				j++;
			}
		}
		else {//Ҷ�ӽڵ�
			while (j < n) {
				TmpKey = Src->Key[Src->KeyNum - 1];
				RemoveElement(1, Parent, Src, i, Src->KeyNum - 1);
				InsertElement(1, Parent, Dst, TmpKey, i + 1, 0);
				j++;
			}

		}

		Parent->Key[i + 1] = Dst->Key[0];
		/* ����Ҷ�ڵ��������� */
		if (Src->KeyNum > 0)
			FindMostRight(Src)->Next = FindMostLeft(Dst);

	}
	else {
		if (Src->Children[0] != NULL) {
			while (j < n) {
				Child = Src->Children[0];
				RemoveElement(0, Src, Child, 0, NULL);
				InsertElement(0, Dst, Child, NULL, Dst->KeyNum, NULL);
				j++;
			}

		}
		else {
			while (j < n) {
				TmpKey = Src->Key[0];
				RemoveElement(1, Parent, Src, i, 0);
				InsertElement(1, Parent, Dst, TmpKey, i - 1, Dst->KeyNum);
				j++;
			}

		}

		Parent->Key[i] = Src->Key[0];
		if (Src->KeyNum > 0)
			FindMostRight(Dst)->Next = FindMostLeft(Src);

	}

	return Parent;
}
//���ѽڵ�
static BPlusTree SplitNode(Position Parent, Position X, int i) {
	int j, k, Limit;
	Position NewNode;

	NewNode = MallocNewNode();

	k = 0;//x�½ڵ㺢����
	j = X->KeyNum / 2;
	Limit = X->KeyNum;//�����ѽڵ�key����
	while (j < Limit) {
		if (X->Children[0] != NULL) {//x�ĺ��Ӳ�Ϊ�գ���һ��ĺ��Ӹ��½ڵ�
			NewNode->Children[k] = X->Children[j];
			X->Children[j] = NULL;
		}
		NewNode->Key[k] = X->Key[j]; //��һ���key���½ڵ�
		X->Key[j] = NULL;
		NewNode->KeyNum++; 
		X->KeyNum--;
		j++;
		k++;
	}

	if (Parent != NULL)//�����ڵ�����½ڵ�
		InsertElement(0, Parent, NewNode, NULL, i + 1, NULL);
	else {//�����X�Ǹ�����ô�����µĸ�������
		Parent = MallocNewNode();
		InsertElement(0, Parent, X, NULL, 0, NULL);
		InsertElement(0, Parent, NewNode, NULL, 1, NULL);

		return Parent;
	}

	return X;
}


static BPlusTree RecursiveInsert(BPlusTree T, KeyType Key, int i, BPlusTree Parent) {
	int Limit;
	Position Sibling;
	int j = 0;//��֧λ��
	while (j < T->KeyNum && bupt_strcmp( Key , T->Key[j])>=0) {
		if (bupt_strcmp(Key,T->Key[j]) == 0)//�ظ�������
			return T;
		j++;
	}
	if (j != 0 && T->Children[0] != NULL) j--;

	if (T->Children[0] == NULL)//��Ҷ
		T = InsertElement(1, Parent, T, Key, i, j);
	else//�ڲ��ڵ�ݹ�
		T->Children[j] = RecursiveInsert(T->Children[j], Key, j, T);

	Limit = M;//���ֵ����

	if (T->KeyNum > Limit) {//�����ڵ�
		if (Parent == NULL) {//���ڵ����
			T = SplitNode(Parent, T, i);
		}
		else {
			Sibling = FindSibling(Parent, i);
			if (Sibling != NULL) {
				/* ��T��һ��Ԫ�أ�Key����Child���ƶ���Sibing�� */
				MoveElement(T, Sibling, Parent, i, 1);
			}
			else {
				/* ���ѽڵ� */
				T = SplitNode(Parent, T, i);
			}
		}

	}
	//���¸��ڵ�����ֵ
	if (Parent != NULL)
		Parent->Key[i] = T->Key[0];


	return T;
}

/* ���� */
extern BPlusTree Insert(BPlusTree T, KeyType Key) {
	return RecursiveInsert(T, Key, 0, NULL);
}

int RecursiveSearch(BPlusTree T, KeyType Key) {

	int j = 0;//��֧λ��
	while (j< T->KeyNum && bupt_strcmp(Key, T->Key[j]) >= 0)
	{
		if (bupt_strcmp(Key, T->Key[j]) == 0) {
			return 1;
		}
		j++;
	}
	if (j != 0 && T->Children[0] != NULL) {
		j--;
	}
	else {
		return 0;
	}
	RecursiveSearch(T->Children[j], Key);
}
//��ѯ
int Search(BPlusTree T, KeyType Key) {
	return RecursiveSearch(T, Key);
}
/* ���� */
extern BPlusTree Destroy(BPlusTree T) {
	int i, j;
	if (T != NULL) {
		i = 0;
		while (i < T->KeyNum + 1) {
			Destroy(T->Children[i]); i++;
		}
		j = 0;
		free(T);
		T = NULL;
	}

	return T;
}

static void RecursiveTravel(BPlusTree T, int Level) {
	int i;
	if (T != NULL) {
		printf("  ");
		printf("[Level:%d]-->", Level);
		printf("(");
		i = 0;
		while (i < T->KeyNum)/*  T->Key[i] != NULL*/
			printf("%s ", T->Key[i++]);
		printf(")\n");

		Level++;

		i = 0;
		while (i <= T->KeyNum) {
			RecursiveTravel(T->Children[i], Level);
			i++;
		}


	}
}

/* ���� */
extern void Travel(BPlusTree T) {
	RecursiveTravel(T, 0);
	printf("\n");
}

/* ������Ҷ�ڵ������ */
extern void TravelData(BPlusTree T) {
	Position Tmp;
	int i;
	if (T == NULL)
		return;
	Tmp = T;
	while (Tmp->Children[0] != NULL)
		Tmp = Tmp->Children[0];
	/* ��һƬ��Ҷ */
	while (Tmp != NULL) {
		i = 0;
		while (i < Tmp->KeyNum)
			printf(" %s", Tmp->Key[i++]);
		Tmp = Tmp->Next;
	}
}

