#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <time.h>
#include <conio.h>
#include <stdlib.h>

#define FILENAME "data.txt"
#define MAX_CHAR 31
#define MAX_DATA 10
#define MAX_NUMBER 20
#define MAX_ITEMS 5

typedef struct player {
	/*
		0 = Addition
		1 = Subtration
		2 = Division
		3 = Multiplication
	*/

	char name[MAX_CHAR];
	int scores[4];

} Player;

// --------- Global Variables ---------

Player record[MAX_DATA];
char currentPlayer[MAX_CHAR];
int last = -1;

// --------- Function Prototypes ---------

void retrieve(); // Retrieving data from the txt file
void save(); // Save current data to txt file
void insert(Player new_player); // insert new data to array
int menu(); // prints menu and return chosen number
int *getRandomPair(); // returns an array of a random pair
void update(int scoreIndex, int score); // update the scores of the current player
int locate(char nameToLocate[]); // will locate the given name and return its index
void swap(Player* a, Player* b); // Swap two array elements
int partition (Player arr[], int low, int high); // quicksort utility function for sorting
void quickSort(Player arr[], int low, int high); // Sort record data
int computeGrade(int score); // compute scores to its equivalent grade using formula (score/maxscore)x100
float getGrade(); // Get the average grade of the current user
bool isEmpty(); // Check if the record is empty
bool isFull(); // check if the record is full
float getAverage(int scores[]); // Get the average of the passed scores

// Game Screens
void addition();
void subtraction();
void division();
void multiplication();
void leaderboard();

// --------- Main Function ---------

int main() {

	system("cls");

	retrieve();

	printf("ARITHMETIC PRACTICE GAME\n");
	printf("==============================================\n\n");

	printf("Enter Player Name: ");
	scanf(" %[^\n]s",  currentPlayer);

	Player new_player;

	strcpy(new_player.name, currentPlayer);;

	for(int i = 0; i < 4 ; i++) 
		new_player.scores[i] = 0;

	insert(new_player);

	while(true) {
		switch(menu()) {
			case 1:;
				// Addition
				addition();
				break;

			case 2:;
				// Subtraction
				subtraction();
				break;

			case 3:;
				// Division
				division();
				break;

			case 4:;
				// Multiplication
				multiplication();
				break;
			case 5:;
				// Leaderboard
				leaderboard();
				break;
			case 6:;
				save();
				exit(0);
				break;
			default:;
				printf("Please enter a valid number! \n");
		}
	}	

	return 0;
}

// --------- Function Definitions ---------

int menu() {
	system("cls");

	printf("ARITHMETIC PRACTICE GAME\n");
	printf("==============================================\n\n");

	printf("1. Addition\n");
	printf("2. Subtraction\n");
	printf("3. Division\n");
	printf("4. Multiplication\n");
	printf("5. Leaderboard\n");
	printf("6. Exit\n\n");

	printf("Current player: %s\n", currentPlayer);
	printf("Grade: %.2f\n\n", getGrade());

	int chosenNumber;

	printf("Enter the number of your choice: ");
	scanf(" %d", &chosenNumber);

	return chosenNumber;
}

// - UTILITY FUNCTIONS

bool isFull() {
  if (last == MAX_DATA-1)
    return true;
  else
    return false;
}

bool isEmpty() {
  if (last == -1)
    return true;
  else
    return false;
}


int * getRandomPair(int isDivOrSub) {
	srand(time(0));

	static int randomPair[2];
	int num1, num2;

	if(isDivOrSub == 0) { // Subtraction

		for(int i = 0; i < 2 ; i++) {
			num1 = rand() % MAX_NUMBER + 1;
			num2 = rand() % MAX_NUMBER + 1;

			while(true) {
				if(num2 > num1) {
					num2 = rand() % MAX_NUMBER + 1;
				}
				else {
					break;
				}
			}

		}
	}
	else if (isDivOrSub == 1) { // Division
		for(int i = 0; i < 2 ; i++) {
			num1 = rand() % MAX_NUMBER + 1;
			num2 = rand() % MAX_NUMBER + 1;

			while(true) {
				if(num2 > num1) {
					num2 = rand() % MAX_NUMBER + 1;
				}
				else {
					if((num1 % num2) == 0) {
						break;
					}
					else {
						num2 = rand() % MAX_NUMBER + 1;
					}
				}
			}
		}
	}
	else { // Addition and Multiplication

		num1 = rand() % MAX_NUMBER + 1;
		num2 = rand() % MAX_NUMBER + 1;
	}

	randomPair[0] = num1;
	randomPair[1] = num2;

	return randomPair;
}


int locate(char nameToLocate[]) {
	for (int i = 0; i <= last ; i++) {
		if(strcmp(nameToLocate, record[i].name) == 0)
			return i;
		}

	return -1;
}

void insert(Player new_player) {
	if(isFull()) {
		printf("Database is full.");
		getch();
	} 
	else {
		if(locate(currentPlayer) > -1) {
			return;
		}
		else {
			last++;
			record[last] = new_player;
		}
	}
}


void update(int scoreIndex, int score) {
	int index = locate(currentPlayer);
	int newGrade;

	if(index == -1)
		return;
	else {
		if(record[index].scores[scoreIndex] == 0) {
			newGrade = computeGrade(score);
		} 
		else {
			newGrade = (record[index].scores[scoreIndex] + computeGrade(score))/2;
		}

		record[index].scores[scoreIndex] = newGrade;
	}
}

float getAverage(int scores[]) {
  int sum = 0; 
  for(int i = 0; i < 4 ; i++) {
    sum += scores[i];
  }
  return (float)sum/4;
}

void swap(Player* a, Player* b)
{
    Player t = *a;
    *a = *b;
    *b = t;
}
 
int partition (Player arr[], int low, int high)
{
    float pivot = getAverage(arr[high].scores);
    int i = (low - 1);
 
    for (int j = low; j <= high - 1; j++)
    {
        if (getAverage(arr[j].scores) > pivot)
        {
            i++;
            swap(&arr[i], &arr[j]);
        }
    }

    swap(&arr[i + 1], &arr[high]);
    return (i + 1);
}
 
void quickSort(Player arr[], int low, int high)
{
    if (low < high)
    {
        int pi = partition(arr, low, high);
 
        quickSort(arr, low, pi - 1);
        quickSort(arr, pi + 1, high);
    }
}

void save() {
  FILE* fp;

  fp = fopen(FILENAME,"w");

  for (int i = 0; i<=last ; i++)
        fprintf(fp,"%s- %d %d %d %d\n", record[i].name, record[i].scores[0], record[i].scores[1], record[i].scores[2], record[i].scores[3]);

  fclose(fp);

}

void retrieve() {
  FILE *fp;

  Player retrieve_player;
  fp = fopen(FILENAME,"r");

  if (fp==NULL){
    return;
  }
  else {
    while(!feof(fp)) {
        fscanf(fp,"%[^-]- %d %d %d %d\n", &retrieve_player.name, &retrieve_player.scores[0], &retrieve_player.scores[1], &retrieve_player.scores[2], &retrieve_player.scores[3]);
        insert(retrieve_player);

    }
  }

  fclose(fp);
}

int computeGrade(int score) {
	float percentage = (float)score/MAX_ITEMS;

	int totalGrade = percentage*100;

	return totalGrade;
}

float getGrade() {
	int index = locate(currentPlayer);

	return getAverage(record[index].scores);
}

// - GAME SCREENS

void addition() {
	system("cls");

	int *rPair, answer;
	int correctAnswers = 0;

	printf("ARITHMETIC PRACTICE GAME\n");
	printf("==============================================\n\n");

	printf("Player name: %s\n", currentPlayer);
	printf("Operation: Addition\n");

	for(int i = 0 ; i < MAX_ITEMS ; i++) {
		rPair = getRandomPair(2);

		printf("\n%d.) What is %d + %d? ", i+1, *(rPair), *(rPair + 1));
		scanf(" %d", &answer);

		if(answer == *(rPair) + *(rPair + 1)) {
			printf("\nYour answer is correct!\n");
			correctAnswers++;
			getch();
		} else {
			printf("\nYour answer is wrong! The correct answer is %d \n", *(rPair) + *(rPair + 1));
			getch();
		}
	}

	printf("\n==============================================\n");

	if(correctAnswers > MAX_ITEMS/2) {
		printf("\nCongratulations! You got %d out of %d!\n", correctAnswers, MAX_ITEMS);
	} 
	else {
		printf("\nYou need to practice more! You got %d out of %d!\n", correctAnswers, MAX_ITEMS);
	}

	printf("\n==============================================\n");

	getch();

	update(0, correctAnswers);

}

void subtraction() {
	system("cls");

	int *rPair, answer;
	int correctAnswers = 0;

	printf("ARITHMETIC PRACTICE GAME\n");
	printf("==============================================\n\n");

	printf("Player name: %s\n", currentPlayer);
	printf("Operation: Subtraction\n");

	for(int i = 0 ; i < MAX_ITEMS ; i++) {
		rPair = getRandomPair(0);

		printf("\n%d.) What is %d - %d? ", i+1, *(rPair), *(rPair + 1));
		scanf(" %d", &answer);

		if(answer == *(rPair) - *(rPair + 1)) {
			printf("\nYour answer is correct!\n");
			correctAnswers++;
			getch();
		} else {
			printf("\nYour answer is wrong! The correct answer is %d \n", *(rPair) - *(rPair + 1));
			getch();
		}
	}

	printf("\n==============================================\n");

	if(correctAnswers > MAX_ITEMS/2) {
		printf("\nCongratulations! You got %d out of %d!\n", correctAnswers, MAX_ITEMS);
	} 
	else {
		printf("\nYou need to practice more! You got %d out of %d!\n", correctAnswers, MAX_ITEMS);
	}

	printf("\n==============================================\n");

	getch();

	update(1, correctAnswers);

}

void division() {
	system("cls");

	int *rPair, answer;
	int correctAnswers = 0;

	printf("ARITHMETIC PRACTICE GAME\n");
	printf("==============================================\n\n");

	printf("Player name: %s\n", currentPlayer);
	printf("Operation: Division\n");

	for(int i = 0 ; i < MAX_ITEMS ; i++) {
		rPair = getRandomPair(1);

		printf("\n%d.) What is %d / %d? ", i+1, *(rPair), *(rPair + 1));
		scanf(" %d", &answer);

		if(answer == *(rPair) / *(rPair + 1)) {
			printf("\nYour answer is correct!\n");
			correctAnswers++;
			getch();
		} else {
			printf("\nYour answer is wrong! The correct answer is %d \n", *(rPair) / *(rPair + 1));
			getch();
		}
	}

	printf("\n==============================================\n");

	if(correctAnswers > MAX_ITEMS/2) {
		printf("\nCongratulations! You got %d out of %d!\n", correctAnswers, MAX_ITEMS);
	} 
	else {
		printf("\nYou need to practice more! You got %d out of %d!\n", correctAnswers, MAX_ITEMS);
	}

	printf("\n==============================================\n");

	getch();

	update(2, correctAnswers);

}

void multiplication() {
	system("cls");

	int *rPair, answer;
	int correctAnswers = 0;

	printf("ARITHMETIC PRACTICE GAME\n");
	printf("==============================================\n\n");

	printf("Player name: %s\n", currentPlayer);
	printf("Operation: Multiplication\n");

	for(int i = 0 ; i < MAX_ITEMS ; i++) {
		rPair = getRandomPair(2);

		printf("\n%d.) What is %d * %d? ", i+1, *(rPair), *(rPair + 1));
		scanf(" %d", &answer);

		if(answer == *(rPair) * *(rPair + 1)) {
			printf("\nYour answer is correct!\n");
			correctAnswers++;
			getch();
		} else {
			printf("\nYour answer is wrong! The correct answer is %d \n", *(rPair) * *(rPair + 1));
			getch();
		}
	}

	printf("\n==============================================\n");

	if(correctAnswers > MAX_ITEMS/2) {
		printf("\nCongratulations! You got %d out of %d!\n", correctAnswers, MAX_ITEMS);
	} 
	else {
		printf("\nYou need to practice more! You got %d out of %d!\n", correctAnswers, MAX_ITEMS);
	}

	printf("\n==============================================\n");

	getch();

	update(3, correctAnswers);

}

void leaderboard() {
	system("cls");

	quickSort(record, 0, last);

	printf("ARITHMETIC PRACTICE GAME\n");
	printf("==============================================\n\n");

	printf("===============================================================================================================");
	printf("\nNO.\tNAME\t\t\tAddition\tSubtraction\tDivision\tMultiplication\t\tAVERAGE \n");
	printf("===============================================================================================================\n");


	for(int i = 0; i <= last ; i++) {

		printf("%d\t", i+1);

		if(strcmp(record[i].name, currentPlayer) == 0) 
			printf("%s (YOU)\t\t", record[i].name);
		else
			printf("%s\t\t\t", record[i].name);


		for(int j = 0; j < 4; j++) {
		  	printf("%d\t\t", record[i].scores[j]);
		}

		printf("\t%.2f\n", getAverage(record[i].scores));

	}

	getch();
}