#include <stdio.h>
#include <stdlib.h>
#define SIZEOFBUFFER 20
#define POSTCODEDIGITS 5

typedef struct Record{
	long CustomerID;
	char FirstName[SIZEOFBUFFER];
	char LastName[SIZEOFBUFFER];
	char Street[SIZEOFBUFFER];
	int HouseID;
	char City[SIZEOFBUFFER];
	char PostCode[POSTCODEDIGITS];
	float Amount;
} Record;

typedef struct ListNode{
	Record * Customer;
	struct ListNode * Previous;
	struct ListNode * Next;
}ListNode;

typedef struct List{
	int RecordsInserted;
	ListNode * FirstCustomer;
	ListNode * LastCustomer;
}List;

typedef struct BucketElement{
	long CustomerID;
	ListNode * Customer;
}BucketElement;

typedef struct BucketNode{
	BucketElement ** Bucket;
	struct BucketNode * Overflow;
	int SizeOfBucket;
	int Capacity;
}BucketNode;

typedef struct HashTable{
	BucketNode ** BucketTable;
	float LoadFactor;
	int Round;
	int SizeOfBucket;
	int BucketToBeSplit;
	int LastBucket;
	int RecordsInserted;
}HashTable;

int InsertRecordInTWA(List *,HashTable *,Record *);
void InsertRecord(List *,HashTable *, char *);
Record * MakeRecord(long,char *,char *,char *,int,char *,char *,float);
ListNode * MakeListNode(Record *);
List * MakeList(void);
BucketElement * MakeBucketElement(void);
BucketNode * MakeBucketNode(int);
HashTable * MakeHashTable(int,float);
int EmptyList(List *);
void PrintRecord(Record *);
int InsertRecordInList(List *,ListNode *);
void ReclassifyList(List *, ListNode *);
void SwapListNodes(List *,ListNode *,ListNode *);
float Average(List *);
void PrintList(ListNode *,ListNode *);
int HashFunction(long,int);
int InsertRecordInBucket(BucketNode *,ListNode *);
int InsertRecordInHashTable(HashTable *,ListNode *);
int OverLoadedTable(HashTable *);
void Split(HashTable*);
int found(ListNode **,HashTable *);
void PrintTop(List *,char *);
void PrintBottom(List *,char *);
int Query(HashTable *,char *);
void PrintPercentile(List *,HashTable *,char *);
void RangePrint(List *,HashTable *,char *);
int DeleteListNode(ListNode *);
void DeleteList(List *);
int DeleteBucketNode(BucketNode *);
void DeleteHashTable(HashTable *);
