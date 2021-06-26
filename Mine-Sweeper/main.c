#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <conio.h>
#include <stdlib.h>
#include<time.h>

#define MAXCHAR 7
#define MAXDATA 15
#define FILENAME "database.txt"
#define MINE 248
#define SPACE 32
#define SAFE 254

typedef struct player {
	char name[MAXCHAR];
	int highscore;

} Player;

// ----- Global Variables ------
Player database[MAXDATA];
int last = -1;
int numberOfTiles = 0;

// ----- Function Prototypes ------
void save();
void retrieve();
int locate_name(char nameToLocate[]);
int getHighestScore();
void insert(Player player);
void header();
void swap(Player* a, Player* b);
int partition (int low, int high);
void quickSort(int low, int high);
void display_board(int board[numberOfTiles][numberOfTiles]);

// Screens ------
int main_menu(Player currentPlayer);
void leaderboards(char currentPlayerNameName[]);
void play_mineSweeper(Player new_player);

// ----- Main Function ------
int main() {
	retrieve();

	header();

	char currentPlayerName[MAXCHAR];

	printf(" Enter Player Name: ");
	scanf(" %6[^\n]",  currentPlayerName);

	Player new_player;

	strcpy(new_player.name, currentPlayerName);
	new_player.highscore = 0;

	insert(new_player);

	fflush(stdin);

	while (true) {
		switch(main_menu(database[locate_name(new_player.name)])) {
			
			case 1:; // Play game
				play_mineSweeper(database[locate_name(new_player.name)]);
				break;

			case 2:; // Leaderboards
				leaderboards(currentPlayerName);
				break;

			case 3:; // Change name
				header();

				char newName[MAXCHAR];
				printf(" Enter the new name you want: ");
				scanf(" %s", newName);

				strcpy(database[locate_name(new_player.name)].name, newName);
				strcpy(new_player.name, newName);

				printf(" Change name successful! ");
				getch();
				break;

			case 4:; // Clear Data and Exit

				char choice;
				printf(" Are you sure you want to delete all data? (y/n) ");

				scanf(" %c", &choice);

				if(choice == 'y' || choice == 'Y') {
					remove(FILENAME);
					exit(0);
				}
				else {
					continue;
				}			
				break;

			case 5:; // Save and exit
				quickSort(0, last);
				save();
				exit(0);

			case 6:; // Exit without saving the data
				char isExit;
				printf("\nAre you sure to leave the game without saving your data? (y/n): ");
				scanf(" %c", &isExit);

				if(isExit == 'y' || isExit == 'Y')
					exit(0);
				else
					break;

			default:;
				printf("\nEnter a valid number!\n");
				getch();
		}
	}

	return 0;
}

// ----- Function Definitions ------

// Screens -------------------------

int main_menu(Player currentPlayer) {
	header();

	printf(" Name: [%s]\n High Score (Self): [%d]\n High Score (All): [%d]\n\n", database[locate_name(currentPlayer.name)].name, database[locate_name(currentPlayer.name)].highscore, getHighestScore());

	printf(" 1. Play Game\n");
	printf(" 2. Leaderboards\n");
	printf(" 3. Change Name\n");
	printf(" 4. Clear Data and Exit\n");
	printf(" 5. Save and Exit\n");
	printf(" 6. Exit Without Saving\n");

	int choice;

	printf("\n Enter the number of your choice: ");
	scanf(" %d", &choice);

	return choice;

}

void play_mineSweeper(Player player) {
	srand(time(NULL));

	int score = 0, mistakes = 0, unMarkedTiles;
	bool isWin;

	// Size of the board
	while(true) {
		header();

		printf(" Enter the size of the board: ");
		scanf(" %d", &numberOfTiles);

		if(numberOfTiles == 1 || numberOfTiles == 2) {
			printf("\n Please enter a number not lower than 3\n");
			getch();
			continue;
		}
		else {
			break;
		}
	}

	int board[numberOfTiles][numberOfTiles];
	int mines[numberOfTiles][numberOfTiles];

	// Initialize Board
	for (int row = 0; row < numberOfTiles ; row++) {
		for (int col = 0; col < numberOfTiles ; col++) {
			mines[row][col] = SPACE;
			board[row][col] = SPACE;
		}
	}
	
    // Initialize mines
    int randRow, randCol, iteration = 0;
    while(iteration != numberOfTiles) {
    	randRow = rand() % (numberOfTiles-1) + 0;
    	randCol = rand() % (numberOfTiles-1) + 0;

    	if(mines[randRow][randCol] == SPACE) {
    		mines[randRow][randCol] = MINE;
    		iteration++;
    	}
    	else {
    		continue;
    	}
    }

    // GAME
	while(true) {
		header();

		unMarkedTiles = 0;

		if (mistakes >= numberOfTiles) {
			isWin = false;
			break;
		}

		// Check if there are unmarked tiles
		for (int row = 0; row < numberOfTiles; row++)
	    {
	        for (int col = 0; col < numberOfTiles; col++)
	        {
	            if(mines[row][col] == SPACE)
	            	unMarkedTiles++;
	        }
	    }

	    if(unMarkedTiles == 0) {
	    	isWin = true;
	    	break;
	    }

	    printf(" [%c] - Safe tile, [%c] - Mine\n\n", SAFE, MINE);

		printf(" Highscore (Self): %d\n\n", player.highscore);
		printf(" Highscore (All): %d\n\n", getHighestScore());

		printf("\n Score: %d\n\n", score);

		display_board(board);

	    int chosenRow, chosenCol;

	    printf("\n Enter row: ");
	    scanf(" %d", &chosenRow);

	    printf(" Enter column: ");
	    scanf(" %d", &chosenCol);

	    // If the entered row or column exceeds the range of the board
	    if(chosenCol > numberOfTiles || chosenRow > numberOfTiles) {
	    	continue;
	    }

	    // Chosen tile is a mine
	    if(mines[chosenRow-1][chosenCol-1] == MINE) {
	    	board[chosenRow-1][chosenCol-1] = MINE;

	    	mistakes++;
	    	score--;
	    }
	    // If already marked
	    else if (board[chosenRow-1][chosenCol-1] == SAFE || board[chosenRow-1][chosenCol-1] == MINE) {
	    	continue;
	    }
	    // safe
	    else {
	    	board[chosenRow-1][chosenCol-1] = SAFE;
	    	mines[chosenRow-1][chosenCol-1] = SAFE;
	    	score++;
	    }

	}

	header();

	display_board(mines);

   	if(isWin == true) {
   		printf("\n You win!\n Score: %d", score);

   		if(score > player.highscore) {
   			database[locate_name(player.name)].highscore = score;
			printf("\n Congratulations! You beat your own highscore!! \n");
   		}
   		else if(score > getHighestScore()) {
   			database[locate_name(player.name)].highscore = score;
   			printf("\n Congratulations! You are now the player who has the highest score!! \n");
   		}
   		else {
   			
   		}
   		
   	} else {
   		if(score > player.highscore || score > getHighestScore()) {
   			database[locate_name(player.name)].highscore = score;
   		}

   		printf("\n You lose!\n Score: %d", score);
   	}

   	getch();
}

void leaderboards(char currentPlayerName[]) {
	quickSort(0, last);

	header();

	printf(" ==============================================");
	printf("\n NO.\tNAME\t\t\tHighscore\n");
	printf(" ==============================================\n");


	for(int i = 0; i <= last ; i++) {

		printf(" %1d\t", i+1);

		if(strcmp(database[i].name, currentPlayerName) == 0) {
			printf("%2s (YOU)", database[i].name);
			printf("%13d\n", database[i].highscore);
		}
		else if(strlen(database[i].name) < 4) {
			printf("%2s", database[i].name);
			printf("%22d\n", database[i].highscore);
		}
		else {
			printf("%2s", database[i].name);
			printf("%20d\n", database[i].highscore);
		}

	}

	getch();
}

// Utilities -------------------------

void header() {
	system("cls");
	printf(" MINE SWEEPER GAME\n");
	printf(" ==============================================\n\n");
}

bool isFull() {
  if (last == MAXDATA-1)
    return true;
  else
    return false;
}

// SORTING
void swap(Player* a, Player* b)
{
    Player t = *a;
    *a = *b;
    *b = t;
}
 
int partition (int low, int high)
{
    float pivot = database[high].highscore;
    int i = (low - 1);
 
    for (int j = low; j <= high - 1; j++)
    {
        if (database[j].highscore > pivot)
        {
            i++;
            swap(&database[i], &database[j]);
        }
    }

    swap(&database[i + 1], &database[high]);
    return (i + 1);
}
 
void quickSort(int low, int high)
{
    if (low < high)
    {
        int pi = partition(low, high);
 
        quickSort(low, pi - 1);
        quickSort(pi + 1, high);
    }
}
// END SORTING

int locate_name(char nameToLocate[]) {
	for (int i = 0; i <= last ; i++) {
		if(strcmp(nameToLocate, database[i].name) == 0)
			return i;
		}

	return -1;
}

int getHighestScore() {
	if(last == 0) {
		return database[last].highscore;
	}
	else {
		int maxNumber;

		for(int i = 0 ; i < last ; i++) {
			if(maxNumber < database[i].highscore)
				maxNumber = database[i].highscore;
		}

		return maxNumber;
	}
}

void insert(Player new_player) {
	if(isFull()) {
		printf("Database is full.");
		getch();
	} 
	else {
		if(locate_name(new_player.name) > -1) {
			return;
		}
		else {
			last++;
			database[last] = new_player;
		}
	}
}

// --- FILE HANDLING

void save() {
  FILE* fp;

  fp = fopen(FILENAME,"w");

  for (int i = 0; i<=last ; i++)
        fprintf(fp,"%s- %d\n", database[i].name, database[i].highscore);

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
        fscanf(fp,"%[^-]- %d\n", &retrieve_player.name, &retrieve_player.highscore);

        insert(retrieve_player);
    }
  }

  fclose(fp);
}

// --- GAME UTILITIES

void display_board(int board[numberOfTiles][numberOfTiles]) {

	printf("  ");
	for(int colValue = 0; colValue < numberOfTiles ; colValue++)
		printf("%3d ", colValue+1);

	printf("\n");

	for (int row = 0; row < numberOfTiles; row++)
    {	
    	if(row+1 >= 10)
    		printf(" %d", row+1);
    	else
    		printf(" %d ", row+1);
        for (int col = 0; col < numberOfTiles; col++)
        {
            printf(" [%c]", board[row][col]);
        }

        printf("\n");
    }
}