#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <math.h>
int allVariables[200];
int variableIndex=0;
void insertHash(char *name, int index) ;
int search(char * );
void addOperationList (int operationType, int inputCount, int outputCount, int *inputVariables, int *outputVariables) ;
int getTokenType ( char * str){
      if (strcmp(str, "INPUT") == 0)
        return 1;
      if (strcmp(str, "OUTPUT") == 0)
	return 2;
      if (strcmp(str, "DECODER") == 0)
	return 3;
      if (strcmp(str, "MULTIPLEXER") == 0)
	return 4;
      if (strcmp(str, "XOR") == 0)
	return 5;
      if (strcmp(str, "AND") == 0)
	return 6;
      if (strcmp(str, "NAND") == 0)
	return 7;
      if (strcmp(str, "NOR") == 0)
	return 8;
      if (strcmp(str, "OR") == 0)
	return 9;
      if (strcmp(str, "NOT") == 0)
	return 10;
      if (strcmp(str, "PASS") == 0)
	return 11;
      return 0;
}
int searchAndInsert( char * str){

  int searchValue = search(str);
  int retValue = 0;
   if (searchValue  == -1 ) {
	insertHash(str,variableIndex);
        retValue = variableIndex;
	variableIndex++;
    }
    else{
	retValue =  searchValue;
	
	}
    return retValue;
}

void processLine (char * line) {
  	const char delim[] = " ";
   	char *token;
	char ops[50];
	int nvariables=0;
	int  *variablesArray ;
	int *variablesOutArray = NULL;
	token = strtok(line, delim);
	strcpy(ops, token);
	int tokenType = getTokenType (token);
	switch (tokenType)
	{	
		case  1: //INPUT: 
		case  2: //OUTPUT: :
			token = strtok(NULL, delim);
			nvariables = atoi(token);
			variablesArray = (int *)malloc(nvariables*sizeof(int));
			variablesOutArray = (int *)malloc(sizeof(int));
			int i;	
			for (i =0; i<nvariables; i++) {
				token = strtok(NULL, delim);
				variablesArray[i] = searchAndInsert(token);
				}
			if (tokenType == 1)
				addOperationList (tokenType,nvariables, 0, variablesArray, variablesOutArray); 
			else

				addOperationList (tokenType,0,nvariables, variablesOutArray,  variablesArray ); 
		break;
		case 5: //XOR: 
		case 6: //AND: 
		case 7: //NAND: 
		case 8: //NOR:	
                case 9: //OR
			token = strtok(NULL, delim);
			variablesArray = (int *)malloc(2* sizeof(int));
			variablesArray[0] = searchAndInsert(token);
			token = strtok(NULL, delim);
			variablesArray[1] = searchAndInsert(token);
			variablesOutArray = (int *)malloc(sizeof(int));
			token = strtok(NULL, delim);
			variablesOutArray[0] = searchAndInsert(token);
			addOperationList (tokenType,2,1, variablesArray, variablesOutArray);
		break;
		case 10: //NOT  
		case 11: //PASS
			token = strtok(NULL, delim);
			variablesArray = (int*)malloc(sizeof(int));
			variablesArray[0]= searchAndInsert(token);
			token = strtok(NULL, delim);
			variablesOutArray = (int*)malloc(sizeof(int));
			variablesOutArray[0] = searchAndInsert(token);
			addOperationList (tokenType,1,1, variablesArray, variablesOutArray);
		
		break;
		case 3:  //DECODER
		break;
		case 4:  //MULTIPLXER
		break;
	}

		
}


struct hashmaplist {
	int index;
	char *name;
	struct hashmaplist *next;
};
struct hashmaplist* hashHead = NULL;
int search(char *name) {
	struct hashmaplist* temp;
	temp = hashHead;
	while (temp!=NULL) {
		if (strcmp(temp->name, name)==0){
			return temp->index;
		}
		temp = temp->next;
	}
	return -1;	

}
void insertHash(char *name, int index) {
	struct hashmaplist *newNode;
	struct hashmaplist *temp;
	newNode = malloc(sizeof(struct hashmaplist));
	newNode->index = index;
	newNode->name = malloc(strlen(name)*(sizeof(char*)));
	newNode->name = name;
	if (hashHead == NULL) {
		hashHead = newNode;
	}
	else{
		temp = hashHead;
		while(temp->next != NULL) {
			temp = temp->next;
		}
		temp->next = newNode;
	}
}	
struct operationList {
	int operationType;
	int inputCount;
	int outputCount;
	int *inputVariables;
	int *outputVariables;
	int currInput;
	int currOutput;
	struct operationList *next;	
};
struct operationList* head = NULL;

void addOperationList (int operationType, int inputCount, int outputCount, int *inputVariables, int *outputVariables) {
	struct operationList *newNode;
	struct operationList *temp;
	newNode = malloc(sizeof(struct operationList));
	newNode->operationType = operationType;
	newNode->inputCount = inputCount;
	newNode->outputCount = outputCount;
	newNode->inputVariables = (int*)malloc(inputCount*sizeof(int));
	int j;
	for (j = 0; j <inputCount; j++) {
		newNode->inputVariables[j] = inputVariables[j];
	}
	newNode->outputVariables = (int*)malloc(outputCount*sizeof(int));
	for (j = 0; j <outputCount; j++) {
		newNode->outputVariables[j] = outputVariables[j];
	}
	newNode->next = NULL;
	if (head ==NULL) {
		head = newNode;
	}
	else{
		temp = head;
		while(temp->next != NULL) {
			temp = temp->next;
		}
		temp->next = newNode;
	}
}		

char * mystrcpy( char *str){
    char * tmp = (char *) malloc(strlen(str));
    strncpy(tmp, str, strlen(str) - 1);
    tmp[strlen(str)-1] = '\0';
    int i ;
    for ( i = 0 ; i < strlen(tmp); i++){
	if (tmp[i] == ':')
	  tmp[i] = ' ';
    }  
    return tmp;
}	
 
void getInput ( FILE * fp){
	char * line = NULL;
	size_t len = 0;
	ssize_t read;
	while ((read = getline(&line, &len, fp)) != -1) {
	   processLine(mystrcpy(line));
	}
}
void printOperationList(struct operationList* tmp) {
     struct operationList* ptr = head;
     while( ptr != NULL) {
	printf("operation: %d ",ptr->operationType);
	printf("input count: %d ",ptr->inputCount);
	printf("output count: %d ",ptr->outputCount);
        printf("input vars: ");	
	int j;
	for (j = 0; j <ptr->inputCount; j++) {
		printf(" %d ", ptr->inputVariables[j]);
	}
        printf("output vars: ");	
	for (j = 0; j <ptr->outputCount; j++) {
		printf("%d ", ptr->outputVariables[j]);
	}
	printf("\n");
        ptr= ptr-> next;
      }
}
int andOperation(int a, int b) {
	if (a ==1 && b ==1) {
		return 1;
	}
	return 0;
}

int nandOperation(int a, int b) {
	if ( a == 1 && b == 1) {
           return 0;
	} 
	else {
		return 1;
	}
}	

int orOperation(int a, int b) {
	if (a ==1 || b == 1) {
		return 1;
	} else {
		return 0;
	}

}

int norOperation(int a, int b) {
 if (a ==1 || b == 1) {
                return 0;
        } else {
                return 1;
        }



}

int xorOperation(int a, int b) {
	if (a!=b) {
		return 1;
	}
	return 0;
}

int notOperation (int a) {
	if (a==1) {
		return 0;
	}
	else {
	return 1;
	}
}
void circuitDesign( ){

	struct operationList* inputOperation = head;
	struct operationList* outputOperation= head->next;
	struct operationList* thisOperation = head->next->next;
	int *inputList ;
	int *outputList;
	int result=0;
	while ( thisOperation != NULL){
		inputList = thisOperation->inputVariables;
	    	outputList = thisOperation->outputVariables;
		switch (thisOperation->operationType)
		{
		case 5: //XOR: 
			result = xorOperation(allVariables[inputList[0]],allVariables[inputList[1]]);
			break;
		case 6: //AND: 
			result = andOperation(allVariables[inputList[0]],allVariables[inputList[1]]);
			break;
		case 7: //NAND: 
			result = nandOperation(allVariables[inputList[0]],allVariables[inputList[1]]);
			break;
		case 8: //NOR:	
			result = norOperation(allVariables[inputList[0]],allVariables[inputList[1]]);
			break;
                case 9: //OR
			result = orOperation(allVariables[inputList[0]],allVariables[inputList[1]]);
			break;
                case 10: //NOT
			result = notOperation(allVariables[inputList[0]]);
			break;
                case 11: //PASS
			result = allVariables[inputList[0]];
			break;
		}
		allVariables[outputList[0]] = result;
                thisOperation = thisOperation->next;
	}
	int inputCount=inputOperation->inputCount;
	int outputCount=outputOperation->outputCount; 
	int i,j;
	for ( i=0; i < inputCount ; i++) {
	    printf("%d ",allVariables[i+2]);
	}
	printf("|");
	for ( j=0; j < outputCount ; j++) {
	    printf(" %d",allVariables[outputOperation->outputVariables[j]]);
	}
        printf("\n");

}
int mypow(a,b){
   int retValue = 1;
   int i ;
   for ( i = 0; i < b; i++){
      retValue = retValue *2;
        }
   return retValue;
}
void printCircuitDesign(int n, int cnt)
{
      int i,j,p;
	p = mypow( 2, cnt);
      for ( i = 0; i < p; i++)
      {
           
	for(j = cnt - 1; j >= 0; j--) {
		int vale = ( ((i >> j) & 1));
	  allVariables[ cnt -j -1   + 2 ] = vale ;
	}
     	circuitDesign();
      }
}


/*char * truthTableValues;

void printCircuitDesign(int n, int cnt){
	if(n<1){
		int i;
                printf("Bin = %s\n", truthTableValues);
		for( i = 0; i < cnt; i++)
		{
			allVariables[2+i] = truthTableValues[i] - '0';
		}
                circuitDesign();
		}
	else{
		truthTableValues[n-1] = '0';
		printCircuitDesign(n-1, cnt);
		truthTableValues[n-1] = '1';
		printCircuitDesign(n-1, cnt );
	}
}
*/

int main (int argc, char * argv[]){
	if(argc !=2) {
		printf("Incorrect inputs");
		return -1;
	}
	FILE * filePointer = fopen(argv[1], "r");
	if (filePointer == NULL) {
		printf("File does not exist");
		return -1;
	}

	searchAndInsert("0");
	searchAndInsert("1");
	getInput( filePointer);	
	int i;
	for (i=0; i < 200; i++){
		allVariables[i] = 0;
	}
	allVariables[1] = 1;
	int countOfInputVariables = head->inputCount;
	printCircuitDesign( countOfInputVariables, countOfInputVariables);
	return 0;
}

	
