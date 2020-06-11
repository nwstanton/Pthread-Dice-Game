#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <semaphore.h>
#include <time.h>

//mutex and cond to handle win synchronozastion
pthread_mutex_t winner = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t winCon = PTHREAD_COND_INITIALIZER;

//mutexs and conds to pass dice between players
pthread_mutex_t aDice = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t bDice = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t cDice = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t dDice = PTHREAD_MUTEX_INITIALIZER;


pthread_cond_t aHasDice = PTHREAD_COND_INITIALIZER;
pthread_cond_t bHasDice = PTHREAD_COND_INITIALIZER;
pthread_cond_t cHasDice = PTHREAD_COND_INITIALIZER;
pthread_cond_t dHasDice = PTHREAD_COND_INITIALIZER;

int roundA = 0;
int roundB = 0;
int roundC = 0;
int roundD = 0;

FILE *logFile;



char str[50];



int win = 0;

//flags for players to switch dice
int plAHas = 0;
int plBHas = 0;
int plCHas = 0;
int plDHas = 0;

int sumA = 0;
int sumB = 0;
int sumC = 0;
int sumD = 0;


void* playA()
{

   pthread_mutex_lock(&aDice);
   pthread_cond_wait(&aHasDice, &aDice);

while(win == 0){
    if(plAHas == 1){

    int r = rand() % 6 + 1;
    int e = rand() % 6 + 1;
    sumA = r + e;
    fprintf(logFile, "player A gets %d", r);
    fprintf(logFile,"and %d", e);
    fprintf(logFile, "for a sum of %d\n", sumA );

    printf(" Player A: %d",r);
    printf(" %d\n",e);

    roundA++;

//wait for wincon
pthread_mutex_lock(&winner);
pthread_cond_wait(&winCon, &winner);
    plBHas = 1;
    plAHas = 0;
pthread_mutex_unlock(&winner);

   pthread_cond_signal(&bHasDice);
    pthread_mutex_unlock(&aDice);
}
}
}



void* playB()
{

pthread_mutex_lock(&bDice);
pthread_cond_wait(&bHasDice, &bDice);

while(win == 0){
if(plBHas == 1){

 int r = rand() % 6 + 1;
 int e = rand() % 6 + 1;
 sumB = r + e;

 fprintf(logFile, "player B gets %d", r);
 fprintf(logFile,"and %d", e);
 fprintf(logFile, "for a sum of %d\n", sumB);
 printf(" Player B: %d",r);
 printf(" %d\n",e);
roundB++;

//wait for winCon
pthread_mutex_lock(&winner);
pthread_cond_wait(&winCon, &winner);
 plCHas = 1;
 plBHas = 0;
pthread_mutex_unlock(&winner);

pthread_cond_signal(&cHasDice);
 pthread_mutex_unlock(&bDice);

}
}
}
void* playC()
{

pthread_mutex_lock(&cDice);
pthread_cond_wait(&cHasDice, &cDice);

while(win == 0){
if(plCHas == 1){

 int r = rand() % 6 + 1;
 int e = rand() % 6 + 1;
 sumC = r + e;
 fprintf(logFile, "player C gets %d", r);
 fprintf(logFile,"and %d", e);
 fprintf(logFile, "for a sum of %d\n", sumC);
 printf(" Player C: %d",r);
 printf(" %d\n",e);

roundC++;

//wait here for wincon
pthread_mutex_lock(&winner);
pthread_cond_wait(&winCon, &winner);
 plDHas = 1;
 plCHas = 0;
 pthread_mutex_unlock(&winner);

 pthread_cond_signal(&dHasDice);
 pthread_mutex_unlock(&cDice);

}
}
}

void* playD()
{

pthread_mutex_lock(&dDice);
pthread_cond_wait(&dHasDice, &dDice);

while(win == 0){
 if(plDHas == 1){

 int r = rand() % 6 + 1;
 int e = rand() % 6 + 1;
 sumD = r + e;
 fprintf(logFile, "player D gets %d", r);
 fprintf(logFile,"and %d", e);
 fprintf(logFile, "for a sum of %d\n", sumD);
 printf(" Player D: %d",r);
 printf(" %d\n",e);

 roundD++;

//wait for wincon
pthread_mutex_lock(&winner);
pthread_cond_wait(&winCon, &winner);
 plAHas = 1;
 plDHas = 0;
pthread_mutex_unlock(&winner);

 pthread_cond_signal(&aHasDice);
 pthread_mutex_unlock(&dDice);
}
}
}

void* dealer()
{



int firstPlay = rand() % 4 +1;

  if (firstPlay == 1){
//set playerA has dice
   plAHas = 1;
   pthread_cond_signal(&aHasDice);

 }
 else if(firstPlay == 2){
//set playerB has dice
  plBHas = 1;
  pthread_cond_signal(&bHasDice);
}
else if(firstPlay == 3){
//set playerC has dice
 plCHas = 1;
pthread_cond_signal(&cHasDice);
}
else if(firstPlay == 4){
//set playerD has dice
 plDHas = 1;
pthread_cond_signal(&dHasDice);
}




while(win == 0)
{

  if(roundA == roundC && sumA == sumC && sumA!= 0 && sumC != 0)
  {
    win = 1;
    fprintf(logFile," %s\n",str);
    printf("Team A & C wins %s\n",str);
  }
  else if(roundB == roundD && sumB == sumD && sumB!= 0 && sumD != 0)
  {
    win = 1;
    fprintf(logFile, "Team B & D wins %s\n",str);
    printf("Team B & D wins %s\n",str);
  }
else
{
  pthread_cond_signal(&winCon);
}



}
}


int main()
{
//creating logfile
logFile = fopen("logNWS.txt","w");

pthread_t newThreadA;
pthread_t newThreadB;
pthread_t newThreadC;
pthread_t newThreadD;
pthread_t newThreadDL;


srand ( time(NULL) );
//create players

pthread_create(&newThreadA,NULL,playA,NULL);

pthread_create(&newThreadB,NULL,playB,NULL);

pthread_create(&newThreadC,NULL,playC,NULL);

pthread_create(&newThreadD,NULL,playD,NULL);

pthread_create(&newThreadDL,NULL,dealer,NULL);

//hack to insure main dosent finish before threads
sleep(10);
 fclose(logFile);
return 0;
}
