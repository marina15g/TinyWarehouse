#include "Prompt.h"
#define MAXCOMMANDSIZE 15
#define MAXLINESIZE 100
FILE * Input;

/*Checking if strings s1 and s2 are equal.
If so returns 1,else returns 0*/
int StringsEqual(char * s1,char * s2){
	if(s1==NULL || s2==NULL){
		return 0;
	}
	int equal=1;
	int i=0;
	while(equal){
		if(s1[i]!=s2[i]){
			return 0;
		}else if(s1[i]=='\0'){
			return 1;
		}
		i++;
	}
}
/*Menu function allows user to access TinyWareHouse and make modifications
Function terminates when user types "e" or "exit" in stdin*/
void Menu(List * l,HashTable * h){
	char command[MAXCOMMANDSIZE],parameter[MAXLINESIZE];
	do{
		printf(">");
		scanf("%s",command);
		if(StringsEqual(command,"l") || StringsEqual(command,"load")){
			scanf( " %[^\n]s",parameter);
			printf("\n");
			InsertDataFromFile(parameter,l,h);
		}else if(StringsEqual(command,"i") || StringsEqual(command,"insert")){
			scanf( " %[^\n]s",parameter);
			printf("\n");
			InsertRecord(l,h,parameter);
		}else if(StringsEqual(command,"q") || StringsEqual(command,"query")){
			scanf( " %[^\n]s",parameter);
			printf("\n");
			if(EmptyList(l)){
				printf("The list is empty\n");
			}else{
				if(!Query(h,parameter)){
					printf("There is no customer with id %s\n",parameter);
				}
			}
		}else if(StringsEqual(command,"t") || StringsEqual(command,"top")){
			scanf( " %[^\n]s",parameter);
			printf("\n");
			PrintTop(l,parameter);
		}else if(StringsEqual(command,"b") || StringsEqual(command,"bottom")){
			scanf( " %[^\n]s",parameter);
			printf("\n");
			PrintBottom(l,parameter);
		}else if(StringsEqual(command,"a") || StringsEqual(command,"average")){
			printf("\n");
			if(EmptyList(l)){
				printf("The list is empty\n\n");
			}else{
				printf("%f\n",Average(l));
			}
		}else if(StringsEqual(command,"r") || StringsEqual(command,"range")){
			scanf( " %[^\n]s",parameter);
			printf("\n");
			RangePrint(l,h,parameter);
		}else if(StringsEqual(command,"p") || StringsEqual(command,"percentile")){
			scanf( " %[^\n]s",parameter);
			printf("\n");
			PrintPercentile(l,h,parameter);
		}else if(StringsEqual(command,"o") || StringsEqual(command,"operations")){
			scanf( " %[^\n]s",parameter);
			printf("\n");
			/*OperationsFile can also be given from prompt*/
			ExecuteOperationsFromFile(parameter,l,h);
		}else if(!StringsEqual(command,"e") && !StringsEqual(command,"exit")){
			printf("\n");
			printf("Invalid command!\n");
		}
		printf("\n");
	}while(!StringsEqual(command,"e") && !StringsEqual(command,"exit"));
	DeleteList(l);
	DeleteHashTable(h);
}

/*CreateRecord Function returns a pointer to a Record that was created after reading data from file Input. If there is malloc fail or end of file, NULL is returned */
Record * CreateRecord(){
	Record * r;
	r=malloc(sizeof(Record));
	if(!feof(Input) && r!=NULL){
		fscanf(Input,"%ld %s %s %s %d %s %s %f",&(r->CustomerID),r->FirstName,r->LastName,r->Street,&(r->HouseID),r->City,r->PostCode,&(r->Amount));
		if(feof(Input)){
			return NULL;
		}
		return r;
	}else{
		return NULL;
	}
}

/*InsertDataFromFile function attempts to insert data from file named datafile into TinyWareHouse.
If file is succesfully opened, InsertNewRecordInTWA function is being called,using CreateRecord function,to insert data in main memory */
void InsertDataFromFile(char * datafile,List * l,HashTable * h){
	if(l!=NULL && h!=NULL){
		int recordsinserted;
		recordsinserted=l->RecordsInserted;
		Input=fopen(datafile,"r");
		if(Input==NULL){
			printf("Could not open file %s\n",datafile);
		}else{
			while(!feof(Input)){
				InsertRecordInTWA(l,h,CreateRecord());
			}
			fclose(Input);
		}
		recordsinserted=l->RecordsInserted-recordsinserted;
		printf("%d records inserted\n",recordsinserted);
	}
}

/*Function ExecuteOperationsFile attempts to execute operations in file named operationsfile.
If file is succesfully opened,operations are being executed serially, until Input reaches end of file*/
void ExecuteOperationsFromFile(char * operationsfile,List *l,HashTable * h){
	Input=fopen(operationsfile,"r");
	if(Input==NULL){
		printf("Could not open file %s\n",operationsfile);
	}else{
		char  command[MAXCOMMANDSIZE],parameter[MAXLINESIZE];
		while(!feof(Input)){
			fscanf(Input,"%s",command);
			if(!feof(Input)){
				if(StringsEqual(command,"l") || StringsEqual(command,"load")){
					fscanf(Input," %[^\n]s",parameter);
					printf("\n");
					InsertDataFromFile(parameter,l,h);
				}else if(StringsEqual(command,"i") || StringsEqual(command,"insert")){
					fscanf( Input," %[^\n]s",parameter);
					printf("\n");
					InsertRecord(l,h,parameter);
				}else if(StringsEqual(command,"q") || StringsEqual(command,"query")){
					fscanf(Input," %[^\n]s",parameter);
					printf("\n");
					if(EmptyList(l)){
						printf("The list is empty\n");
					}else{
						if(!Query(h,parameter)){
							printf("There is no record with id %s\n",parameter);
						}
					}
				}else if(StringsEqual(command,"t") || StringsEqual(command,"top")){
					fscanf(Input," %[^\n]s",parameter);
					printf("\n");
					PrintTop(l,parameter);
				}else if(StringsEqual(command,"b") || StringsEqual(command,"bottom")){
					fscanf(Input," %[^\n]s",parameter);
					printf("\n");
					PrintBottom(l,parameter);
				}else if(StringsEqual(command,"a") || StringsEqual(command,"average")){
					printf("\n");
					if(EmptyList(l)){
						printf("The list is empty\n");
					}else{
						printf("%f\n",Average(l));
					}
				}else if(StringsEqual(command,"r") || StringsEqual(command,"range")){
					fscanf(Input," %[^\n]s",parameter);
					printf("\n");
					RangePrint(l,h,parameter);
				}else if(StringsEqual(command,"p") || StringsEqual(command,"percentile")){
					fscanf(Input," %[^\n]s",parameter);
					printf("\n");
					PrintPercentile(l,h,parameter);
				}else if(!StringsEqual(command,"e") && !StringsEqual(command,"exit")){
					printf("\n");
					printf("Invalid command!\n");
				}
			}
		}
		fclose(Input);
	}
}
