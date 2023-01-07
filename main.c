#include "defs.h"

int main(){
  GhostType ghost;
  HunterType hunters[MAX_HUNTERS];
  pthread_t hunter_threads[MAX_HUNTERS];
  pthread_t ghost_thread;
  char hunter1[MAX_STR];
  char hunter2[MAX_STR];
  char hunter3[MAX_STR];
  char hunter4[MAX_STR];
  srand(time(NULL)); // Initialize a random seed for the random number generators
  BuildingType building;
  initBuilding(&building);
  populateRooms(&building);


  printf("Please enter the hunter names separated by spaces: ");
  scanf("%s %s %s %s", hunter1, hunter2, hunter3, hunter4);
  while(getchar() != '\n');

  initHunter(hunter1, hunters+0, &building);
  initHunter(hunter2, hunters+1, &building);
  initHunter(hunter3, hunters+2, &building);
  initHunter(hunter4, hunters+3, &building);

  initGhost(&ghost, &building);

  pthread_create(&ghost_thread, NULL, ghostIsBored, &ghost);

  for (int i = 0; i < MAX_HUNTERS; i++){
    pthread_create(&hunter_threads[i], NULL, hunterIsBored, hunters+i);
  }

  pthread_join(ghost_thread, NULL);

  for (int i = 0; i < MAX_HUNTERS; i++){
    pthread_join(hunter_threads[i], NULL);
  }

  printScaredHunters(&building);
  printf("\n");
  printGhost(&ghost);

  cleanupBuilding(&building);

  return 0;
}

/*
  Function:  randInt
  Purpose:   returns a pseudo randomly generated number,
             in the range min to (max - 1), inclusively
       in:   upper end of the range of the generated number
   return:   randomly generated integer in the range [min, max-1)
*/
int randInt(int min, int max){
  if(min == max){
    return min;
  }
  return rand() % (max - min) + min;
}
/*
  Function:  randFloat
  Purpose:   returns a pseudo randomly generated number,
             in the range min to max, inclusively
       in:   upper end of the range of the generated number
   return:   randomly generated integer in the range [0, max-1)
*/
float randFloat(float a, float b) {
    // Get a percentage between rand() and the maximum
    float random = ((float) rand()) / (float) RAND_MAX;
    // Scale it to the range we want, and shift it
    return random * (b - a) + a;
}
