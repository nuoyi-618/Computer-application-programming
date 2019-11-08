#include <stdio.h>
#include <time.h>
#include "BPlusTree.h"
#include < assert.h >
/*****动态内存******/
void* bupt_malloc(size_t size) {
	if (size <= 0)
	{
		return NULL;
	}

	global_stats.mem += size;
	return malloc(size);
}
/*****字符比较******/
int byte_cmp(char a, char b) {
	global_stats.cmpnum++;
	return a - b;
}
/*****字符串对比******/
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
/* 生成节点并初始化 */
static BPlusTree MallocNewNode() {
	BPlusTree NewNode;
	int i=0;
	NewNode = bupt_malloc(sizeof(struct BPlusNode));//申请BPlusTree类型的内存空间
	if (NewNode == NULL)
	{
		printf("创建结点失败");
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

/* 初始化 */
extern BPlusTree Initialize() {

	BPlusTree T;
	if (M < (3)) {
		printf("M最小等于3！");
		exit(EXIT_FAILURE);

	}
	/* 根结点 */
	T = MallocNewNode();
	return T;
}
//定位到该节点P的最小（左边）孩子 
static Position FindMostLeft(Position P) {
	Position Tmp;

	Tmp = P;

	while (Tmp != NULL && Tmp->Children[0] != NULL) {
		Tmp = Tmp->Children[0];
	}
	return Tmp;
}
//定位到该节点P的最大（右边）孩子 
static Position FindMostRight(Position P) {
	Position Tmp;

	Tmp = P;

	while (Tmp != NULL && Tmp->Children[Tmp->KeyNum - 1] != NULL) {
		Tmp = Tmp->Children[Tmp->KeyNum - 1];//关键字从0开始，最后为KeyNum - 1
	}
	return Tmp;
}

/* 寻找一个兄弟节点，其存储的关键字未满，否则返回NULL */
static Position FindSibling(Position Parent, int i) {
	Position Sibling;
	int Limit;

	Limit = M;

	Sibling = NULL;
	if (i == 0) {
		if (Parent->Children[1]->KeyNum < Limit)
			Sibling = Parent->Children[1];
	}//先看左边的兄弟节点关键字满没满
	else if (Parent->Children[i - 1]->KeyNum < Limit)
		Sibling = Parent->Children[i - 1];//如果左边兄弟关键字没满，再看右边的兄弟
	else if (i + 1 < Parent->KeyNum && Parent->Children[i + 1]->KeyNum < Limit) {
		Sibling = Parent->Children[i + 1];
	}

	return Sibling;
}

/* 当要对X插入Key的时候，i是X在Parent的位置，j是Key要插入的位置
   当要对Parent插入X节点的时候，i是要插入的位置，Key和j的值没有用
 */
static Position InsertElement(int isKey, Position Parent, Position X, KeyType Key, int i, int j) {

	int k;
	
	if (isKey) {//是否插入key，是1，否0
		/* 插入key */
		k = X->KeyNum - 1;
		for (k; k >= j;k--) {
			X->Key[k + 1] = X->Key[k];
		}

		//strcpy(X->Key[j], Key);
		X->Key[j] = Key;

		if (Parent != NULL)
			Parent->Key[i] = X->Key[0];
		//该节点在父节点该位置的key值等于该节点最左节点的值
		X->KeyNum++;

	}
	else {
		/* 插入节点 */

		if (X->Children[0] == NULL) {//新节点插入到树叶中
			if (i > 0)
				Parent->Children[i - 1]->Next = X;
			X->Next = Parent->Children[i];
		}

		k = Parent->KeyNum - 1;
		while (k >= i) {//父节点移位空位置
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
		/* 删除key */
		k = j + 1;
		while (k < Limit) {
			X->Key[k - 1] = X->Key[k]; k++;
		}

		X->Key[X->KeyNum - 1] = NULL;

		Parent->Key[i] = X->Key[0];

		X->KeyNum--;
	}
	else {
		/* 删除节点 */

		/* 修改树叶节点的链接 */
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

/* Src和Dst是两个相邻的节点，i是Src在Parent中的位置；
 将Src的元素移动到Dst中 ,n是移动元素的个数*/
static Position MoveElement(Position Src, Position Dst, Position Parent, int i, int n) {
	KeyType TmpKey;
	Position Child;
	int j, SrcInFront;

	SrcInFront = 0;

	if (bupt_strcmp(Src->Key[0] , Dst->Key[0])<0)
		SrcInFront = 1;

	j = 0;
	/* 节点Src在Dst前面 */
	if (SrcInFront) {
		if (Src->Children[0] != NULL) {//中间节点
			while (j < n) {
				Child = Src->Children[Src->KeyNum - 1];
				RemoveElement(0, Src, Child, Src->KeyNum - 1, NULL);
				InsertElement(0, Dst, Child, NULL, 0, NULL);
				j++;
			}
		}
		else {//叶子节点
			while (j < n) {
				TmpKey = Src->Key[Src->KeyNum - 1];
				RemoveElement(1, Parent, Src, i, Src->KeyNum - 1);
				InsertElement(1, Parent, Dst, TmpKey, i + 1, 0);
				j++;
			}

		}

		Parent->Key[i + 1] = Dst->Key[0];
		/* 将树叶节点重新连接 */
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
//分裂节点
static BPlusTree SplitNode(Position Parent, Position X, int i) {
	int j, k, Limit;
	Position NewNode;

	NewNode = MallocNewNode();

	k = 0;//x新节点孩子数
	j = X->KeyNum / 2;
	Limit = X->KeyNum;//被分裂节点key数量
	while (j < Limit) {
		if (X->Children[0] != NULL) {//x的孩子不为空，分一半的孩子给新节点
			NewNode->Children[k] = X->Children[j];
			X->Children[j] = NULL;
		}
		NewNode->Key[k] = X->Key[j]; //分一半的key给新节点
		X->Key[j] = NULL;
		NewNode->KeyNum++; 
		X->KeyNum--;
		j++;
		k++;
	}

	if (Parent != NULL)//往父节点里插新节点
		InsertElement(0, Parent, NewNode, NULL, i + 1, NULL);
	else {//如果是X是根，那么创建新的根并返回
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
	int j = 0;//分支位置
	while (j < T->KeyNum && bupt_strcmp( Key , T->Key[j])>=0) {
		if (bupt_strcmp(Key,T->Key[j]) == 0)//重复不插入
			return T;
		j++;
	}
	if (j != 0 && T->Children[0] != NULL) j--;

	if (T->Children[0] == NULL)//树叶
		T = InsertElement(1, Parent, T, Key, i, j);
	else//内部节点递归
		T->Children[j] = RecursiveInsert(T->Children[j], Key, j, T);

	Limit = M;//最大值限制

	if (T->KeyNum > Limit) {//调整节点
		if (Parent == NULL) {//根节点分裂
			T = SplitNode(Parent, T, i);
		}
		else {
			Sibling = FindSibling(Parent, i);
			if (Sibling != NULL) {
				/* 将T的一个元素（Key或者Child）移动的Sibing中 */
				MoveElement(T, Sibling, Parent, i, 1);
			}
			else {
				/* 分裂节点 */
				T = SplitNode(Parent, T, i);
			}
		}

	}
	//更新父节点索引值
	if (Parent != NULL)
		Parent->Key[i] = T->Key[0];


	return T;
}

/* 插入 */
extern BPlusTree Insert(BPlusTree T, KeyType Key) {
	return RecursiveInsert(T, Key, 0, NULL);
}

int RecursiveSearch(BPlusTree T, KeyType Key) {

	int j = 0;//分支位置
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
//查询
int Search(BPlusTree T, KeyType Key) {
	return RecursiveSearch(T, Key);
}
/* 销毁 */
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

/* 遍历 */
extern void Travel(BPlusTree T) {
	RecursiveTravel(T, 0);
	printf("\n");
}

/* 遍历树叶节点的数据 */
extern void TravelData(BPlusTree T) {
	Position Tmp;
	int i;
	if (T == NULL)
		return;
	Tmp = T;
	while (Tmp->Children[0] != NULL)
		Tmp = Tmp->Children[0];
	/* 第一片树叶 */
	while (Tmp != NULL) {
		i = 0;
		while (i < Tmp->KeyNum)
			printf(" %s", Tmp->Key[i++]);
		Tmp = Tmp->Next;
	}
}

