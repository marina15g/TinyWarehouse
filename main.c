#include <stdio.h>
#include <stdlib.h>
#include "Prompt.h"

int main(int argc,char ** argv){
	if(argc>1){
		int i;
		int BucketEntries;
		BucketEntries=0;
		double LoadFactor;
		LoadFactor=0.0;
		char * DataFile,*OperationsFile,*ConfigurationFile;
		DataFile="";
		OperationsFile="";
		ConfigurationFile="";
		for(i=0;i<argc;i++){
			if(StringsEqual(argv[i],"-b") && i<argc-1){
				BucketEntries=atoi(argv[i+1]);
			}
			if(StringsEqual(argv[i],"-f") && i<argc-1){
				LoadFactor=atof(argv[i+1]);
			}
			if(StringsEqual(argv[i],"-l") && i<argc-1){
				DataFile=argv[i+1];
			}
			if(StringsEqual(argv[i],"-p") && i<argc-1){
				OperationsFile=argv[i+1];
			}
			if(StringsEqual(argv[i],"-c") && i<argc-1){
				ConfigurationFile=argv[i+1];
			}
		}
		if(BucketEntries==0 || LoadFactor==0.0){
			printf("\nMissing parameters!\nProgram could not be executed\n\n");
		}else{	/*If BucketEntries and LoadFactor are valid,execution of program starts*/
			List * L;	/*List and HashTable are being created*/
			L=MakeList();
			HashTable * H;
			H=MakeHashTable(BucketEntries,LoadFactor);
			if(!StringsEqual(DataFile,"")){	/*If there is DataFile given from command line,program attempts to insert records from the file*/
				InsertDataFromFile(DataFile,L,H);
			}
			if(!StringsEqual(OperationsFile,"")){	/*If there is OperationsFile given from command line,program attempts to execute the operations from the file*/
				ExecuteOperationsFromFile(OperationsFile,L,H);
			}
			Menu(L,H);	/*using prompt to allow user to access the Data Stucture and make modifications*/
		}
	}else{
		printf("\nMissing parameters!\nProgram could not be executed\n\n");
	}
}
