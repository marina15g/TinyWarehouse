#include <math.h>
#include "TinyWareHouse.h"
#define INITIALBUCKETS 2
FILE * Input;

/*Inserting new record in List and HashTable or update it if it already exists*/
int InsertRecordInTWA(List * l,HashTable * h,Record * r){
	if(r!=NULL && l!=NULL && h!=NULL){
		ListNode * ln;
		ln=MakeListNode(r);
		if(!found(&ln,h)){
			if(ln!=NULL){
				if(InsertRecordInHashTable(h,ln) && InsertRecordInList(l,ln)){
					return 1;	/*If record successfully inserted return 1*/
				}
			}
		}else{
			/*If record updated return 2*/
			ReclassifyList(l,ln);
			return 2;
		}
	}
	return 0;
}

/*Inserting new Record to Tiny Warehouse, or updating Record if it already exists in Tiny Warehouse*/
void InsertRecord(List * l,HashTable * h, char * istring){
	if(l!=NULL && h!=NULL && istring!=NULL){
		Record * r;
		r=malloc(sizeof(Record));
		if(r!=NULL){
			sscanf(istring,"%ld %s %s %s %d %s %s %f",&(r->CustomerID),r->LastName,r->FirstName,r->Street,&(r->HouseID),r->PostCode,r->City,&(r->Amount));	/*Reading data from input string*/
			int f=InsertRecordInTWA(l,h,r);
			if(f==2){
				printf("record updated\n");
			}else if(f==1){
				printf("record inserted\n");
			}else{
				printf("an error occured while inserting record\n");
			}
		}
	}
}

/******************************   Make Functions   ******************************/

/*Allocating memory for a new Record and initializing Record*/
Record * MakeRecord(long custid,char * fname,char * lname,char * street,int houseid,char * pcode,char * city,float amount){
	Record * r;
	r=malloc(sizeof(Record));
	if(r!=NULL){
		r->CustomerID=custid;
		sscanf(fname,"%s",r->FirstName);
		sscanf(lname,"%s",r->LastName);
		sscanf(street,"%s",r->Street);
		r->HouseID=houseid;
		sscanf(pcode,"%s",r->PostCode);
		sscanf(city,"%s",r->City);
		r->Amount=amount;
		return r;
	}
	return NULL;
}


/*Allocating memory for a new ListNode
Returns pointer to the new ListNode,or NULL if malloc fails*/
ListNode * MakeListNode(Record * r){
	if(r==NULL){
		return NULL;
	}
	ListNode * ln;
	ln=malloc(sizeof(ListNode));
	if(ln!=NULL){
		ln->Customer=r;
		ln->Next=NULL;
		ln->Previous=NULL;
		return ln;
	}
	return NULL;
}

/*Allocating memory for a new List and returning pointer to this List.
If malloc fails, returns NULL*/
List * MakeList(void){
	List * l;
	l=malloc(sizeof(List));
	if(l!=NULL){
		l->RecordsInserted=0;
		l->FirstCustomer=NULL;
		l->LastCustomer=NULL;
		return l;
	}
	return NULL;
}

/*Allocating memory for a new BucketElement and returning pointer to this BucketElement.
If malloc fails, returns NULL*/
BucketElement * MakeBucketElement(void){
	BucketElement * El;
	El=malloc(sizeof(BucketElement));
	if(El!=NULL){
		El->Customer=NULL;
	}
	return El;
}

/*Allocating memory for a new BucketNode
Returns pointer to the new BucketNode, or NULL if malloc fails*/
BucketNode * MakeBucketNode(int bucketsize){
	BucketNode * bn;
	bn=malloc(sizeof(BucketNode));
	if(bn!=NULL){
		bn->SizeOfBucket=bucketsize;
		bn->Capacity=bucketsize;
		bn->Overflow=NULL;
		BucketElement ** Elptr;
		Elptr=malloc(bucketsize*sizeof(BucketElement *));
		if(Elptr!=NULL){
			int i,j;
			for(i=0;i<bucketsize;i++){
				Elptr[i]=MakeBucketElement();
				if(Elptr[i]==NULL){	/*If malloc fails while allocating space for an element of bucket, ...*/
					for(j=0;j<i;j++){	/*... elements that have already been allocated are now being freed*/
						free(Elptr[j]);
					}
					free(Elptr);	/*All allocated memory for the new BucketNode is being freed*/
					free(bn);
					return NULL;	/*NULL is returned*/
				}
			}
			bn->Bucket=Elptr;
			return bn;
		}else{
			free(bn);
			return NULL;
		}
	}
	return NULL;
}

/*Allocating memory for new HashTable
Returns pointer to the new HashTable or NULL if malloc fails*/
HashTable * MakeHashTable(int bucketsize,float loadfactor){
	HashTable * h;
	h=malloc(sizeof(HashTable));
	if(h!=NULL){
		BucketNode ** bt;
		bt=malloc(INITIALBUCKETS*sizeof(BucketNode *));
		if(bt!=NULL){
			int i,j;
			for(i=0;i<INITIALBUCKETS;i++){
				bt[i]=MakeBucketNode(bucketsize);
				if(bt[i]==NULL){	/*If malloc fails while allocating space for a Node of HashTable, ...*/
					for(j=0;j<i;j++){	/*... Nodes that have already been allocated are now being freed*/
						free(bt[j]);
					}
					free(bt);	/*All allocated memory for the new HashTable is being freed*/
					free(h);
					return NULL;	/*NULL is returned*/
				}
			}
			h->BucketTable=bt;
			h->LoadFactor=loadfactor;
			h->Round=0;
			h->SizeOfBucket=bucketsize;
			h->BucketToBeSplit=0;
			h->LastBucket=INITIALBUCKETS-1;
			h->RecordsInserted=0;
			return h;
		}else{
			free(h);
			return NULL;
		}
	}
	return NULL;
}

/******************************   List Functions   ******************************/

/*Checking if a List is empty.
Returns 1 if list is empty,else returns 0
Condition: List must have been created and l must be other than NULL*/
int EmptyList(List * l){
	return (l->FirstCustomer==NULL);
}

void PrintRecord(Record * r){
	if(r!=NULL){
	printf("%ld: %s~%s~%s~%d~%s~%s~%.2f\n",r->CustomerID,r->FirstName,r->LastName,r->Street,r->HouseID,r->City,r->PostCode,r->Amount);
	}
}

/*Inserting New List Node in the proper position of the List*/
int InsertRecordInList(List * l,ListNode * ln){
	if(ln!=NULL && l!=NULL){
		if(EmptyList(l)){	/*If List is empty, first record is inserted*/
			l->FirstCustomer=ln;	/*List's pointers to first and last ListNode are modified properly*/
			l->LastCustomer=ln;
			l->RecordsInserted++;
			return 1;
		}else{
			if(l->FirstCustomer->Customer->Amount<ln->Customer->Amount){
				ln->Next=l->FirstCustomer;	/*If Node is to be inserted in the beginnig of the List, ... */ 
				l->FirstCustomer->Previous=ln;	/*...  List's pointers are also modified*/
				l->FirstCustomer=ln;
				l->RecordsInserted++;
				return 1;
			}else{
				int inserted;
				inserted=0;
				ListNode * current;	/*ListNode pointer current is used to access all the nodes in the List*/
				current=l->FirstCustomer;	/*starts from the beginning of the List*/
				do{
					if(current->Next==NULL){	/*If Node is to be inserted in the end of the List,..*/
						ln->Previous=current;	/*... List's pointers are also modified*/
						current->Next=ln;
						l->LastCustomer=ln;
						l->RecordsInserted++;
						return 1;
					}else if(current->Next->Customer->Amount<ln->Customer->Amount){
						current->Next->Previous=ln;
						ln->Next=current->Next;
						ln->Previous=current;
						current->Next=ln;
						l->RecordsInserted++;
						return 1;
					}else{		/*If ListNode's position is yet to be found, ... */
						current=current->Next;	/*... current pointer is moved to the next Node*/
					}
				}while(!inserted);	/*Iteration ends when record is finally inserted*/
			}
		}
	}
	return 0;
}

/*Placing updated ListNode in the right position, in order for List to remain classified in descending order*/
void ReclassifyList(List * l, ListNode * updated){
	ListNode * current;	/*ListNode current is being used to trace previous or next nodes of updated ListNode*/
	current=updated;
	if(current!=NULL && l!=NULL){
		int moved;
		moved=0;
		if(current->Next!=NULL){	/*if current Node is not the last Node of the list,*/
			while(moved==0){	/*it checks if current must move forwards*/
				if(current->Customer->Amount<current->Next->Customer->Amount){
		/*If current Node is to be moved forwards,current Node and it's next node are being swaped*/
					SwapListNodes(l,current,current->Next);
					if(current->Next==NULL){
						moved=1;
					}
				}else{
					moved=1;
				}
			}
		}
		moved=0;
		if(current->Previous!=NULL){	/*if current Node is not the first Node of the list,*/
			while(!moved){		/*it checks if current must move backwards*/
				if(current->Customer->Amount>current->Previous->Customer->Amount){
		/*If current Node is to be moved backwards,current Node and it's previous node are being swaped*/
					SwapListNodes(l,current->Previous,current);
					if(current->Previous==NULL){
						moved=1;
					}
				}else{
					moved=1;
				}
			}
		}
	}
}

/*Swaps the positions of the ListNodes l1 and l2
Condition:l1 and l2 must be sequential,with l2 being l1's next and l1 being l2's previous*/
void SwapListNodes(List * l,ListNode * l1,ListNode * l2){
	if(l!=NULL && l1!=NULL && l2!=NULL){
		if(l2->Next!=NULL){
			l2->Next->Previous=l1;
		}else{	/*If l2 is the last Node of the List,List is also modified*/
			l->LastCustomer=l1;
		}
		if(l1->Previous!=NULL){
			l1->Previous->Next=l2;
		}else{	/*If l1 is the first Node of the List,List is also modified*/
			l->FirstCustomer=l2;
		}
		l1->Next=l2->Next;
		l2->Next=l1;
		l2->Previous=l1->Previous;
		l1->Previous=l2;
	}
}

/*Calculating average expense of Records in the List*/
float Average(List * l){
	float total=0;
	ListNode * current;
	current=l->FirstCustomer;
	while(current!=NULL){
		total=total+current->Customer->Amount;
		current=current->Next;
	}
	float avrg;
	avrg=total/l->RecordsInserted;
	return avrg;
}

/*Printing Records between r1 and r2 in ascending order.Records r1 and r2 are not being printed */
void PrintList(ListNode * ln1,ListNode * ln2){
	if(ln1!=ln2){
		ListNode * from,* to;
		if(ln1->Customer->Amount<ln2->Customer->Amount){
			from=ln1;
			to=ln2;
		}else{
			from=ln2;
			to=ln1;
		}
		ListNode * current;
		current=from->Previous;
		while(current!=to){
			PrintRecord(current->Customer);
			current=current->Previous;
		}
	}
}


/******************************   HashTable Functions   ******************************/

int HashFunction(long id,int round){
	int x,y,r;
	x=pow(2,round);
	y=x*INITIALBUCKETS;
	r=id%y;
	return r;
}

/*Inserting new BucketElement pointing to ListNode ln in Bucket Node bn*/
int InsertRecordInBucket(BucketNode * bn,ListNode *ln){
	if(bn!=NULL && ln!=NULL){
		BucketNode * current;
		current=bn;	/*BucketNode pointer current is being used to access all the possible overflow BucketNodes of bn*/
		int inserted=0;
		while(!inserted){
			if(current->Capacity>0){	/*If BucketNode current is not full,the new Element is being inserted in it*/
				current->Bucket[current->SizeOfBucket-current->Capacity]->Customer=ln;
				current->Bucket[current->SizeOfBucket-current->Capacity]->CustomerID=ln->Customer->CustomerID;
				current->Capacity--;
				return 1;
			}else if(current->Overflow==NULL){/*If current is full and does not have an Overflow BucketNode,*/
				BucketNode * new;		/*a new BucketNode is being added*/
				new=MakeBucketNode(bn->SizeOfBucket);
				if(new!=NULL){
					if(InsertRecordInBucket(new,ln)){	/*Element is inserted in new overflow BucketNode*/
						current->Overflow=new;
						return 1;
					}
				}
				return 0;
			}else{	/*current is being movet to the next BucketNode of the overflow list*/
				current=current->Overflow;
			}
		}
		return 1;
	}
	return 0;
}

/*Inserting new Bucket Element in the right BucketNode of the HashTable*/
int InsertRecordInHashTable(HashTable * h,ListNode * ln){
	if(h!=NULL && ln!=NULL){
		int to;	/*HashFunction is being used to calculate the position of new BucketElement in the BucketTable*/
		to=HashFunction(ln->Customer->CustomerID,h->Round);
		if(to<h->BucketToBeSplit){
			to=HashFunction(ln->Customer->CustomerID,h->Round+1);
		}
		/*InsertRecordInBucket is being called to insert new BucketElement in the list BucketTable[to] */
		if(InsertRecordInBucket(h->BucketTable[to],ln)){
			h->RecordsInserted++;
			if(OverLoadedTable(h)){	/*If load factor exceeds the given LoadFactor value we have a split*/
				Split(h);
				if(h->LastBucket+1==INITIALBUCKETS*pow(2,h->Round+1)){
					h->Round++;	/*If BucketTable's size doubles, we have a new round*/
					h->BucketToBeSplit=0;
				}else{
					h->BucketToBeSplit++;
				}
			}
			return 1;
		}
	}
	return 0;
}

int OverLoadedTable(HashTable * h){
	int capacity;
	capacity=(h->LastBucket+1)*h->SizeOfBucket;
	float r;
	r=(float)h->RecordsInserted/(float)capacity;
	return(r>h->LoadFactor);
}

/*Adding a new BucketNode, and distributing BucketElements between the BucketToBeSplit and the new BucketNode*/
void Split(HashTable * h){
	if(h!=NULL){
		BucketNode ** new;
		new=realloc(h->BucketTable,(h->LastBucket+2)*sizeof(BucketNode *));
		if(new!=NULL){
			BucketNode * bn;
			bn=MakeBucketNode(h->SizeOfBucket);
			if(bn!=NULL){
				h->BucketTable=new;
				h->LastBucket++;
				h->BucketTable[h->LastBucket]=bn;
				BucketNode * remaining, * current,*temp;
				remaining=MakeBucketNode(h->SizeOfBucket);
			/*Using a BucketNode List for the Elements that are not removed to the new Bucket*/
				if(remaining!=NULL){
					/*current pointer is being used to access all the overflow BucketNodes*/
					current=h->BucketTable[h->BucketToBeSplit];
					int i;
					while(current!=NULL){
						for(i=0;i<h->SizeOfBucket-current->Capacity;i++){
							/*Inserting each Element in the proper BucketNode*/
							if(HashFunction(current->Bucket[i]->CustomerID,h->Round+1)==h->BucketToBeSplit){
								InsertRecordInBucket(remaining,current->Bucket[i]->Customer);
							}else{
								InsertRecordInBucket(h->BucketTable[h->LastBucket],current->Bucket[i]->Customer);
							}
						}
						temp=current;
						current=current->Overflow;	/*moving pointer to the next BucketNode*/
						free(temp->Bucket);		/*freeing unuseful memory*/
						free(temp);
					}
					h->BucketTable[h->BucketToBeSplit]=remaining;
				}
			}
		}
	}
}

/*Checking if the ListNode that is pointed by ln, already exists in TinyWareHouse
If so, 1 is returned.Else, 0 is retured*/
int found(ListNode ** ln,HashTable * h){
	int to;
	if(ln!=NULL && h!=NULL){
		to=HashFunction((*ln)->Customer->CustomerID,h->Round);
		if(to<h->BucketToBeSplit){
			to=HashFunction((*ln)->Customer->CustomerID,h->Round+1);
		}
		int i;
		BucketNode * current;	/*current pointer is being used to access all the overflow BucketNodes*/
		current=h->BucketTable[to];
		while(current!=NULL){
			for(i=0;i<current->SizeOfBucket-current->Capacity;i++){
				if(current->Bucket[i]->CustomerID==(*ln)->Customer->CustomerID){
					current->Bucket[i]->Customer->Customer->Amount=current->Bucket[i]->Customer->Customer->Amount+(*ln)->Customer->Amount;
					*ln=current->Bucket[i]->Customer;
					return 1;	/*If ListNode is found,ln is now pointing to this Node*/
				}
			}
			current=current->Overflow;
		}
	}
	return 0;
}


/******************************   Print Functions   ******************************/


/*Printing k Records with the hightest expense in descending order*/
void PrintTop(List * l,char * istring){
	int k;
	sscanf(istring,"%d",&k);	/*istring is used as input*/
	if(EmptyList(l)){
		printf("The list is empty\n");
	}else{
		ListNode * ln;
		ln=l->FirstCustomer;
		int i=1;
		while(ln!=NULL && i<=k){
			PrintRecord(ln->Customer);
			ln=ln->Next;
			i++;
		}
	}
}

/*Printing k Records with the lowest expense in ascending order*/
void PrintBottom(List * l,char * istring){ int k;
	sscanf(istring,"%d",&k);
	if(EmptyList(l)){
		printf("The list is empty\n");
	}else{
		ListNode * ln;
		ln=l->LastCustomer;
		int i=1;
		while(ln!=NULL && i<=k){
			PrintRecord(ln->Customer);
			ln=ln->Previous;
			i++;
		}
	}

}

/*Printing Record with CustomerID=id and return 1.
If such record does not exist in TWA, 0 is returned*/
int Query(HashTable * h,char * istring){
	if(h!=NULL && istring!=NULL){
		long id;
		sscanf(istring,"%ld",&id);
		int to,i;
		to=HashFunction(id,h->Round);
		if(to<h->BucketToBeSplit){
			to=HashFunction(id,h->Round+1);
		}
		BucketNode * current;
		current=h->BucketTable[to];

		while(current!=NULL){
			for(i=0;i<current->SizeOfBucket-current->Capacity;i++){
				if(current->Bucket[i]->CustomerID==id){
					PrintRecord(current->Bucket[i]->Customer->Customer);
					return 1;
				}
			}
			current=current->Overflow;
		}
		return 0;
	}
}

/*Calculating and printing percentile of customer with CustomerID=id
If such record does not exist in TWA,proper message is being printed*/
void PrintPercentile(List * l,HashTable *h,char * istring){
	if(EmptyList(l)){
		printf("The list is empty\n");
	}else{
		long id;
		int to;
		sscanf(istring,"%ld",&id);
		to=HashFunction(id,h->Round);
		if(to<h->BucketToBeSplit){
			to=HashFunction(id,h->Round+1);
		}
		Record * r;
		r=MakeRecord(id,"","","",0,"","",0.0);
		ListNode * ln;
		ln=MakeListNode(r);
		if(!found(&ln,h)){
			printf("Customer with ID %ld does not exist\n",id);
		}else{
			int total=1;
			ListNode * current;
			current=ln;
			while(current!=l->LastCustomer){
				total++;
				current=current->Next;
			}
			int perc;
			perc=((total)*100/(l->RecordsInserted));
			printf("%d%%\n\n",perc);
		}
	}
}

void RangePrint(List * l,HashTable * h,char * istring){
	if(EmptyList(l)){
		printf("The list is empty\n");
	}else{
		long custid1,custid2;
		sscanf(istring,"%ld %ld",&custid1,&custid2);
		int to;
		to=HashFunction(custid1,h->Round);
		if(to<h->BucketToBeSplit){
			to=HashFunction(custid1,h->Round+1);
		}
		Record * r1;
		r1=MakeRecord(custid1,"","","",0,"","",0.0);
		ListNode  * ln1;
		ln1=MakeListNode(r1);
		if(ln1!=NULL){
			if(!found(&ln1,h)){
				printf("Customer with ID %ld does not exist\n",custid1);
			}else{
				to=HashFunction(custid2,h->Round);
				if(to<h->BucketToBeSplit){
					to=HashFunction(custid2,h->Round+1);
				}
				Record * r2;
				r2=MakeRecord(custid2,"","","",0,"","",0.0);
				ListNode * ln2;
				ln2=MakeListNode(r2);
				if(ln2!=NULL){
					if(!found(&ln2,h)){
						printf("Customer with ID %ld does not exist\n",custid2);
					}else{
						PrintList(ln1,ln2); /*If customers exist, the records between them are being printed*/
					}
				}
			}
		}
	}
}


/******************************   Delete Functions   ******************************/



int DeleteListNode(ListNode * ln){
	if(ln->Next==NULL){
		free(ln);
		return 1;
	}else{
		if(DeleteListNode(ln->Next)){
			free(ln);
			return 1;
		}
	}
}
void DeleteList(List * l){
	if(l!=NULL){
		if(!EmptyList(l)){
			DeleteListNode(l->FirstCustomer);
		}
		free(l);
	}
}

int DeleteBucketNode(BucketNode * bn){
	int i;
	if(bn->Overflow==NULL){
		for(i=0;i<bn->SizeOfBucket;i++){
			free(bn->Bucket[i]);
		}
		free(bn->Bucket);
		free(bn);
		return 1;
	}else{
		if(DeleteBucketNode(bn->Overflow)){
			for(i=0;i<bn->SizeOfBucket;i++){
				free(bn->Bucket[i]);
			}
			free(bn->Bucket);
			free(bn);
			return 1;
		}
	}
}

void DeleteHashTable(HashTable * h){
	int i;
	for(i=0;i<=h->LastBucket;i++){
		DeleteBucketNode(h->BucketTable[i]);
	}
	free(h->BucketTable);
	free(h);
}
