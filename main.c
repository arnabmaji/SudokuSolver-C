#include <stdio.h>
#define MAX_LENGTH 9

//Global Variables goes here...
int grid[MAX_LENGTH][MAX_LENGTH];
int maxGridHeight = 3;
int maxGridWidth = 3;
int maxDigit = 9;
int vacantSpaces = 0;
int runTimes = 0;
int isCrossHatchRunning = 1;
int maxRunTimes = 50;


//Function declarations goes here...
void startSolving();
void countVacantSpaces();
void crossHatch();
void rowWiseCheck();
int getSuitableColoumns();
void analyzeGrid(int, int);
int getMissingNumbersList(int*, int, int);
void inputGrid();
void getSuitablePlaces(int,int,int,int*);
int isAlreadyInThisColoumn(int,int);
int isAlreadyInThisRow(int,int);
void displayGrid();

//Function definitions goes here...
void startSolving(){
	while(vacantSpaces > 0){
		int previousVacantSpaces = vacantSpaces;
		if (isCrossHatchRunning){
			crossHatch();
			if(previousVacantSpaces == vacantSpaces){
				isCrossHatchRunning = 0;
			}
		} else {
			rowWiseCheck();
			if(previousVacantSpaces == vacantSpaces){
				isCrossHatchRunning = 1;
			}
		}
		
		if(runTimes == maxRunTimes){
			printf("DEBUG: MAXIMUM RUNTIMES ALLOWED, REACHED\n");
			printf("DEBUG: CURRENT VACANT SPACES: %d\n",vacantSpaces);
			break;
		}
	}
}

void crossHatch(){
	for (int i = 0 ; i < MAX_LENGTH ; i += maxGridHeight){
		for(int j = 0 ; j < MAX_LENGTH ; j +=  maxGridWidth){
			analyzeGrid(i,j);
		}
	}
}

void analyzeGrid(int x, int y){
	int isNewNumberAdded = 0;
	int missingNumbersList[MAX_LENGTH];
	int length = getMissingNumbersList(missingNumbersList,x,y);
	
	//Iterate for each missing number and find available places...
	for(int n=0;n<length;n++){
		int places[2];
		getSuitablePlaces(missingNumbersList[n],x,y,places);
		if(places[0] != -1){
			grid[places[0]][places[1]] = missingNumbersList[n];
			vacantSpaces--;
			isNewNumberAdded = 1;// If a number is added we need to re-analyze the same grid
		}
	}
	
	if(isNewNumberAdded){
		analyzeGrid(x,y);
	}
}

int getMissingNumbersList(int *missingNumbersList, int x, int y){
	int len = 0;
	int maxHeight = x + maxGridHeight;
	int maxWidth = y + maxGridWidth;
	
	for(int num=1;num<=maxDigit;num++){
		int isMissing = 1;
		for(int i=x;i<maxHeight;i++){
			for(int j=y;j<maxWidth;j++){
				if(grid[i][j] == num){
					isMissing = 0;
					break;
				}
			}
		}
		if(isMissing){
			missingNumbersList[len++] = num;
		}	
	}
	
	return len;
}

void getSuitablePlaces(int num, int x, int y,int *places){
	int numberOfSuitablePlaces = 0;
	
	int maxHeight = x + maxGridHeight;
	int maxWidth = y + maxGridWidth;
	
	for(int i=x;i<maxHeight && numberOfSuitablePlaces<2;i++){
		for(int j=y;j<maxWidth && numberOfSuitablePlaces<2;j++){
			if(grid[i][j] == 0 && !isAlreadyInThisRow(num,i) && !isAlreadyInThisColoumn(num,j)){
				places[0] = i;
				places[1] = j;
				numberOfSuitablePlaces++;
			}
		}
	}
	
	if(numberOfSuitablePlaces == 2){
		places[0] = -1; //-1 denotes the missing number has more than one suitable places
		places[1] = -1;
	}
}

int isAlreadyInThisColoumn(int n, int col){
	for(int i=0;i<MAX_LENGTH;i++){
		if(grid[i][col] == n){
			return 1;
		}
	}
	return 0;
}

int isAlreadyInThisRow(int n, int row){
	for(int i=0;i<MAX_LENGTH;i++){
		if(grid[row][i] == n){
			return 1;
		}
	}
	return 0;
}

void inputGrid(){
	for(int i=0;i<MAX_LENGTH;i++){
		for(int j=0;j<MAX_LENGTH;j++){
			scanf("%d",&grid[i][j]);
		}
	}
}

void displayGrid(){
	printf("--------------------------------------------------\n");
	for(int i=0;i<MAX_LENGTH;i++){
		for(int j=0;j<MAX_LENGTH;j++){
			printf("%d ",grid[i][j]);
		}
		printf("\n");
	}
}

void countVacantSpaces(){
	for(int i=0;i<MAX_LENGTH;i++){
		for(int j=0;j<MAX_LENGTH;j++){
			if(grid[i][j] == 0){
				vacantSpaces++;
			}
		}
	}
}


void rowWiseCheck(){
	for(int n=1;n<=maxDigit;n++){
		for(int i=0;i<MAX_LENGTH;i++){
			if(!isAlreadyInThisRow(n,i)){ //Check if it's not already in the row
				int col = getSuitableColoumns(n,i);
				if(col != -1){
					grid[i][col] = n;
					vacantSpaces--;
				}
			}
		}
	}
}

int getSuitableColoumns(int n,int row){
	int suitableColoumns[2];
	int len = 0;
	for(int i=0;i<MAX_LENGTH && len<2;i++){
		if(grid[row][i] == 0 && !isAlreadyInThisColoumn(n,i)){
			suitableColoumns[len++] = i;
		}
	}
	
	if(len == 2){
		return -1; //-1 denotes it has more than one positions
	}
	return suitableColoumns[0];
}


//MAIN function here...
int main(){
	
	inputGrid();
	countVacantSpaces();
	startSolving();
	displayGrid();
	
	return 0;
}
