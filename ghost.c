#include "defs.h"
/*
  Function: initGhost
  Purpose: initializes a ghost
  in/out:   The ghost
  in: The building
   return:   void
*/
void initGhost(GhostType* ghost, BuildingType* building){
  building->ghost = ghost;
  ghost->boredomTime = BOREDOM_MAX;

  int x = randInt(1, 5);
  if(x == 1){
    ghost->ghostType = POLTERGEIST;
  }
  else if(x==2){
    ghost->ghostType = BANSHEE;
  }
  else if(x==3){
    ghost->ghostType = BULLIES;
  }else{
    ghost->ghostType = PHANTOM;
  }

  ghost->currentRoom = NULL;

  initGhostLocation(ghost, building);
}
/*
  Function: ghostRoom
  Purpose: moves a ghost to an adjacent rooom
  in/out:   The ghost
   return:   void
*/
void ghostRoom(GhostType* ghost){
  int nextPos = 1;
  int high = ghost->currentRoom->roomCount;

  int x = randInt(0, high+1);
  if(x == 0){
    return;
    }
    else{
      RoomNodeType* currRoom = ghost->currentRoom->rooms->head;
      while (currRoom != NULL) {
        if (nextPos != x){
            currRoom = currRoom->next;
            ++nextPos;
        }
        else{
          break;
        }
      }

    sem_wait(&ghost->currentRoom->mutex);
    ghost->currentRoom->ghost= NULL;
    sem_post(&ghost->currentRoom->mutex);
    ghost->currentRoom = currRoom->room;
    sem_wait(&ghost->currentRoom->mutex);
    ghost->currentRoom->ghost = ghost;
    sem_post(&ghost->currentRoom->mutex);

    printf("The ghost is in %s\n", ghost->currentRoom->name);
    return;
  }
}
/*
  Function: initGhostLocation
  Purpose: sets the ghost at an initial location
  in/out:   The ghost
  in: The building
   return:   void
*/
void initGhostLocation(GhostType* ghost, BuildingType* building){
  int currPos = 0;
  int x = randInt(2, 13);
  RoomNodeType* currRoom = building->rooms->head;
  while (currRoom != NULL) {
    if (currPos != x){
      currRoom = currRoom->next;
      ++currPos;
    }else{
      break;
    }
  }
  ghost->currentRoom = currRoom->room;
  ghost->currentRoom->ghost = ghost;


  sleep(2);
  printf("The ghost is in %s\n", ghost->currentRoom->name);


}
/*
  Function: ghostIsBored
  Purpose: the thread function which calls an action for the ghost to take
  in/out:   The ghost
   return:   void
*/
void* ghostIsBored(void* g){
  GhostType* ghost = (GhostType*) g;

  while(ghost->boredomTime > 0){
    int x = randInt(1, 3);
    if(ghost->currentRoom->hunterCount != 0){//there's a hunter
      ghost->boredomTime = BOREDOM_MAX;
      if(x == 1){
        sleep(2);
        generateEvidence(ghost);
      }
    }
    else{
      if(x == 1){
        sleep(1);
        ghostRoom(ghost);
        ghost->boredomTime -= 1;

      }else{
        sleep(2);
        generateEvidence(ghost);
      }
    }
  }
  if(ghost->boredomTime <= 0){
    printf("Ghost was bored\n");
    return (void*) NULL;
  }
  return (void*) NULL;
}
/*
  Function: printGhost
  Purpose: Prints the ghost type
  in:   The ghost
   return:   void
*/
void printGhost(GhostType* ghost){
  if(ghost->ghostType == POLTERGEIST){
    printf("GHOST TYPE: %15s|", "POLTERGEIST");
  }
  else if(ghost->ghostType == PHANTOM){
    printf("GHOST TYPE: %15s|", "PHANTOM");
  }
  else if(ghost->ghostType == BULLIES){
    printf("GHOST TYPE: %15s|", "BULLIES");
  }
  else{
    printf("GHOST TYPE: %15s|", "BANSHEE");
  }
}
