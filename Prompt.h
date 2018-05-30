#include <stdio.h>
#include <stdlib.h>
#include "TinyWareHouse.h"
int StringsEqual(char *, char *);
Record * CreateRecord();
void InsertDataFromFile(char *,List *,HashTable *);
void ExecuteOperationsFromFile(char *,List *,HashTable *);
