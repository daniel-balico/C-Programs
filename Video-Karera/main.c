#include <stdio.h>
#include <windows.h>
#include <stdlib.h>
#include <conio.h>
#include<time.h>

#define SPEED 25

// ----- Function Prototypes ------------

void delay(unsigned int mseconds);
void gotoxy(int x,int y);
void racingScreen(int numberOfCars, int carProgress[], char *car[]);
void header();


// ----- Main Function ------------

int main(){
  int numberOfCars;

  system("cls");
  header();

  gotoxy(5, 4);
  printf("Enter the number of cars: ");
  scanf(" %d", &numberOfCars);

  char *car[numberOfCars];
  int carProgress[numberOfCars]; //carProgress[i] is the x position of car[i]

  for(int i = 0 ; i < numberOfCars ; i++) {
    gotoxy(5, 6+i);
    printf("Enter Car %d: ", i+1);
    scanf(" %s", &car[i]);

    carProgress[i] = 5; // Starting position of every car is at 5 xpos
  }

  system("cls");
  header();

  gotoxy(5, 8);
  printf("=====================================================================================================");
  gotoxy(5, 25);
  printf("=====================================================================================================");
  for (int i=0;i < numberOfCars ; i++){
    gotoxy(carProgress[i],10+i);
    printf(">%s>>",car+i);
  }


  gotoxy(40,25);
  printf("[ Press any key to start race. ]");
  getch();

  racingScreen(numberOfCars, carProgress, car);

  return 0;
}

// ----- Function Definitions ------------

void gotoxy(int x,int y){
    COORD coord = {0,0};
    coord.X=x;
    coord.Y=y;
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE),coord);
}

void delay(unsigned int mseconds)
{
    clock_t goal = mseconds + clock();
    while (goal > clock());
}

void header() {
  gotoxy(5, 1);
  printf("CAR RACING GAME");
  gotoxy(5, 2);
  printf("===========================\n\n");
}

void racingScreen(int numberOfCars, int carProgress[], char *car[]) {
  srand(time(NULL));
  system("cls");
  header();

  int c;
  int x = 0;

  gotoxy(5, 8);
  printf("=====================================================================================================");
  gotoxy(5, 25);
  printf("=====================================================================================================");
  while(x != 1) {
    c = rand() % numberOfCars-1 + 0;

    // Print a space before car to hide the traces
    for (int i=0;i<numberOfCars;i++){
      gotoxy(carProgress[i]-10, 10+i);
      printf("          ");
    }

    delay(SPEED);

    // Checking if any car has reached an x position of 100
    for(int j = 0; j < numberOfCars; j++) {
      if(carProgress[j] >= 100) {
        gotoxy(40,25);
        printf(" [ %s Wins! ] ", &car[j]);
        getch();
        x = 1;
        continue;
      }
    }

    // Display cars in carProgress[i] x position
    for (int i = 0; i < numberOfCars; i++) {
      gotoxy(carProgress[i], 10+i);
      printf(">%s>>",car+i);

    }
    
    // Randomly increment elements
    carProgress[c]++;

  }

  system("cls");
}