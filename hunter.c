#include "defs.h"
/*
  Function: initHunter
  Purpose: initializes the hunter
  in:   The name of the hunter, and the Building
  in/out: The hunter
   return:   void
*/
void initHunter(char* name, HunterType* hunter, BuildingType* building){
  strcpy(hunter->name, name);
  hunter->eviCapacity = ARR_SIZE;
  hunter->eviCount = 0;
  int x = randInt(1, 4);
  if(x == 1){
    hunter->device = POLTERGEIST;
  }
  else if(x==2){
    hunter->device = BANSHEE;
  }
  else if(x==3){
    hunter->device = BULLIES;
  }else{
    hunter->device = PHANTOM;
  }

  sleep(1);
  hunter->currentRoom = building->rooms->head->room;
  printf("%s is in %s\n", hunter->name, hunter->currentRoom->name);


  addBuildingHunter(hunter, building);

  hunter->collectedEvidence = (EvidenceType**) calloc(hunter->eviCapacity, sizeof(EvidenceType*));

  hunter->fear = 0;
  hunter->boredomTime = BOREDOM_MAX;

  appendHunter(building->rooms->head->room, hunter);
}
/*
  Function: moveHunter
  Purpose: moves the hunter to an adjacent room
  in/out:   The hunter
   return:   void
*/
void moveHunter(HunterType* hunter){
  int nextPos = 1;
    int high = hunter->currentRoom->roomCount;
    int x = randInt(0, high+1);
    if(x == 0){
      return;
    }
    else{
      RoomNodeType* currRoom = hunter->currentRoom->rooms->head;
      while (currRoom != NULL) {
        if (nextPos != x){
            currRoom = currRoom->next;
            ++nextPos;
        }
        else{
          break;
        }
      }

      sem_wait(&(hunter->currentRoom->mutex));
      deleteHunter(hunter, hunter->currentRoom);
      sem_post(&(hunter->currentRoom->mutex));

      sem_wait(&(currRoom->room->mutex));
      hunter->currentRoom = currRoom->room;
      appendHunter(hunter->currentRoom, hunter);
      sem_post(&(currRoom->room->mutex));

      printf("%s is in %s\n",hunter->name, hunter->currentRoom->name);
  }
    return;
}
/*
  Function: appendHunter
  Purpose: adds a hunter to the end of a room's list
  in/out:   The room
  in: the hunter
   return:   void
*/
void appendHunter(RoomType* room, HunterType* hunt) {
    room->hunters[room->hunterCount] = hunt;
    room->hunterCount++;
}
/*
  Function: deleteHunter
  Purpose: deletes a hunter from a room's list
  in/out:    the room
  in: The hunter,
   return:   void
*/
void deleteHunter(HunterType* hunt, RoomType* room){
  for(int i = 0; i < room->hunterCount; i++){
    if(room->hunters[i] == hunt){
      for(int j = i; j < room->hunterCount; ++j){
        room->hunters[j] = room->hunters[j+1];
      }
      room->hunterCount -= 1;
    }
  }
}
/*
  Function: communicateHunter
  Purpose:  shares evidence with a fellow hunter
  in: the hunter
   return:   void
*/
void communicateHunter(HunterType* hunter){
  int count = hunter->currentRoom->hunterCount;
  int x = randInt(0, count);
  while (hunter->currentRoom->hunters[x] == hunter){
    if(count > 1){
      x = randInt(0, count);
    }
    else{
      return;
    }
  }

  int i = 0;
  for(i=0; i < hunter->eviCount; i++){
    if(hunter->collectedEvidence[i]->evidenceType == EMF && (hunter->collectedEvidence[i]->value > 4.9)){
      if(hunter->currentRoom->hunters[x]->eviCount > 0){
        int j = 0;
        while(j < hunter->currentRoom->hunters[x]->eviCount){
          if(hunter->collectedEvidence[i] == hunter->currentRoom->hunters[x]->collectedEvidence[j]){
            break;
          }
          else{
            j++;
          }
        }
        if(j == hunter->currentRoom->hunters[x]->eviCount){
          duplicateHunterEvidence(hunter->currentRoom->hunters[x], hunter->collectedEvidence[i]);
          printf("%s shared ghostly EMF evidence with %s\n", hunter->name, hunter->currentRoom->hunters[x]->name);
        }

      }
      else{
        duplicateHunterEvidence(hunter->currentRoom->hunters[x], hunter->collectedEvidence[i]);
        printf("%s shared ghostly EMF evidence with %s\n", hunter->name, hunter->currentRoom->hunters[x]->name);
      }

    }
    else if(hunter->collectedEvidence[i]->evidenceType == TEMPERATURE && (hunter->collectedEvidence[i]->value < 0)){
      if(hunter->currentRoom->hunters[x]->eviCount > 0){
        int j = 0;
        while(j < hunter->currentRoom->hunters[x]->eviCount){
          if(hunter->collectedEvidence[i] == hunter->currentRoom->hunters[x]->collectedEvidence[j]){
            break;
          }
          else{
            j++;
          }
        }
        if(j == hunter->currentRoom->hunters[x]->eviCount){
          duplicateHunterEvidence(hunter->currentRoom->hunters[x], hunter->collectedEvidence[i]);
          printf("%s shared ghostly TEMPERATURE evidence with %s\n", hunter->name, hunter->currentRoom->hunters[x]->name);
        }

      }
      else{
        duplicateHunterEvidence(hunter->currentRoom->hunters[x], hunter->collectedEvidence[i]);
        printf("%s shared ghostly TEMPERATURE evidence with %s", hunter->name, hunter->currentRoom->hunters[x]->name);
      }
    }
    else if(hunter->collectedEvidence[i]->evidenceType == FINGERPRINTS && (hunter->collectedEvidence[i]->value > 0)){
      if(hunter->currentRoom->hunters[x]->eviCount > 0){
        int j = 0;
        while(j < hunter->currentRoom->hunters[x]->eviCount){
          if(hunter->collectedEvidence[i] == hunter->currentRoom->hunters[x]->collectedEvidence[j]){
            break;
          }
          else{
            j++;
          }
        }
        if(j == hunter->currentRoom->hunters[x]->eviCount){
          duplicateHunterEvidence(hunter->currentRoom->hunters[x], hunter->collectedEvidence[i]);
          printf("%s shared ghostly FINGERPRINTS evidence with %s\n", hunter->name, hunter->currentRoom->hunters[x]->name);
        }

      }
      else{
        duplicateHunterEvidence(hunter->currentRoom->hunters[x], hunter->collectedEvidence[i]);
        printf("%s shared ghostly FINGERPRINTS evidence with %s\n", hunter->name, hunter->currentRoom->hunters[x]->name);
      }
    }
    else if(hunter->collectedEvidence[i]->evidenceType == SOUND && (hunter->collectedEvidence[i]->value > 70)){
      if(hunter->currentRoom->hunters[x]->eviCount > 0){
        int j = 0;
        while(j < hunter->currentRoom->hunters[x]->eviCount){
          if(hunter->collectedEvidence[i] == hunter->currentRoom->hunters[x]->collectedEvidence[j]){
            break;
          }
          else{
            j++;
          }
        }
        if(j == hunter->currentRoom->hunters[x]->eviCount){
          duplicateHunterEvidence(hunter->currentRoom->hunters[x], hunter->collectedEvidence[i]);
          printf("%s shared ghostly SOUND evidence with %s\n", hunter->name, hunter->currentRoom->hunters[x]->name);
        }

      }
      else{
        duplicateHunterEvidence(hunter->currentRoom->hunters[x], hunter->collectedEvidence[i]);
        printf("%s shared ghostly SOUND evidence with %s\n", hunter->name, hunter->currentRoom->hunters[x]->name);
      }
    }
  }
  return;
}
/*
  Function: addBuildingHunter
  Purpose: adds a hunter to the building's hunter's list
  in/out:   The building
  in: the hunter
   return:   void
*/
void addBuildingHunter(HunterType* hunt, BuildingType* building){
  building->hunters[building->hunterCount] = hunt;
  building->hunterCount++;
}
/*
  Function: cleanupHunterEvidence
  Purpose: frees the hunter's evidence list
  in/out:   The building
   return:   void
*/
void cleanupHunterEvidence(BuildingType* b){
  int i, j;
  for(i = 0; i<MAX_HUNTERS; i++){
    for(j = 0; j< b->hunters[i]->eviCount; j++){
      free(b->hunters[i]->collectedEvidence[j]);
    }
    free(b->hunters[i]->collectedEvidence);
  }
}
/*
  Function: hunterIsBored
  Purpose: hunter's thread function that gives a hunter an action
  in/out:   The hunter
  return:   void
*/
void* hunterIsBored(void* hunt){
  HunterType* hunter = (HunterType*) hunt;
  int check = allEvidenceHunt(hunter);
  while((hunter->boredomTime > 0)&& (hunter->fear < MAX_FEAR) && (check == 0)){

    if(hunter->currentRoom->ghost != NULL){
      hunter->fear += FEAR_RATE;
      hunter->boredomTime = BOREDOM_MAX;
    }
    int x = randInt(1, 4);

    if(x == 1){
      sleep(2);
      moveHunter(hunter);
      hunter->boredomTime -= 1;
    }
    else if(x == 2){
      sleep(2);
      collectEvidence(hunter);

    }
    else if(x==3){

      if(hunter->currentRoom->hunterCount > 1){
        sem_wait(&(hunter->currentRoom->mutex));
        communicateHunter(hunter);
        sem_post(&(hunter->currentRoom->mutex));
        hunter->boredomTime -= 1;
      }
    }

  }
  if(hunter->boredomTime <= 0){
    printf("%s was bored\n", hunter->name);
    return (void*) NULL;
  }
  else if(hunter->fear >= MAX_FEAR){
    printf("%s was scared\n", hunter->name);
    return (void*) NULL;
  }else if(check > 0){
    allEvidenceWin(hunter);
    return (void*) NULL;
  }
  return (void*) NULL;
}
/*
  Function: allEvidenceHunt
  Purpose: checks if substantial evidence has been collected to identify the ghost
  in:   The hunter
  return:   void
*/
int allEvidenceHunt(HunterType* hunter){
  int emfCount = 0;
  int tempCount = 0;
  int soundCount = 0;
  int fingerCount = 0;

  int i;
  if(hunter->eviCount == 0){
    return 0;
  }

  for(i = 0; i < hunter->eviCount; i++){
    if((hunter->collectedEvidence[i]->evidenceType == EMF) && (hunter->collectedEvidence[i]->value > 4.9)){
      emfCount += 1;
    }
    else if((hunter->collectedEvidence[i]->evidenceType == TEMPERATURE) && (hunter->collectedEvidence[i]->value < 0)){
      tempCount += 1;
    }
    else if((hunter->collectedEvidence[i]->evidenceType == FINGERPRINTS) && (hunter->collectedEvidence[i]->value > 0)){
      fingerCount += 1;
    }
    else if((hunter->collectedEvidence[i]->evidenceType == SOUND) && (hunter->collectedEvidence[i]->value > 70)){
      soundCount += 1;
    }
  }

  if(emfCount > 0 && tempCount > 0 && fingerCount > 0){
    printf("The ghost is POLTERGEIST\n");
    return 1;
  }
  else if(emfCount > 0 && tempCount > 0 && soundCount > 0){
    printf("The ghost is BANSHEE\n");
    return 1;
  }
  else if(emfCount > 0 && fingerCount > 0 && soundCount > 0){
    printf("The ghost is BULLIES\n");
    return 1;
  }
  else if(fingerCount > 0 && tempCount > 0 && soundCount > 0){
    printf("The ghost is PHANTOM\n");
    return 1;
  }
  else{
    return 0;
  }
}
/*
  Function: allEvidenceWin
  Purpose:  prints the speculated ghost type if substantial evidence was collected
  in/out:   The hunter
  return:   void
*/
void allEvidenceWin(HunterType* hunter){
  int emfCount = 0;
  int tempCount = 0;
  int soundCount = 0;
  int fingerCount = 0;

  int i;
  if(hunter->eviCount == 0){
    return;
  }

  for(i = 0; i < hunter->eviCount; i++){
    if((hunter->collectedEvidence[i]->evidenceType == EMF) && (hunter->collectedEvidence[i]->value > 4.9)){
      emfCount += 1;
    }
    else if((hunter->collectedEvidence[i]->evidenceType == TEMPERATURE) && (hunter->collectedEvidence[i]->value < 0)){
      tempCount += 1;
    }
    else if((hunter->collectedEvidence[i]->evidenceType == FINGERPRINTS) && (hunter->collectedEvidence[i]->value > 0)){
      fingerCount += 1;
    }
    else if((hunter->collectedEvidence[i]->evidenceType == SOUND) && (hunter->collectedEvidence[i]->value > 70)){
      soundCount += 1;
    }
  }

  if(emfCount > 0 && tempCount > 0 && fingerCount > 0){
    printf("The spec ghost is POLTERGEIST\n");

  }
  else if(emfCount > 0 && tempCount > 0 && soundCount > 0){
    printf("The spec ghost is BANSHEE\n");

  }
  else if(emfCount > 0 && fingerCount > 0 && soundCount > 0){
    printf("The spec ghost is BULLIES\n");

  }
  else if(fingerCount > 0 && tempCount > 0 && soundCount > 0){
    printf("The spec ghost is PHANTOM\n");
  ;
  }
  else{
    return;
  }
}
/*
  Function: printScaredHunters
  Purpose: prints the final result of the ghost hunt
  in:   The building
  return:   void
*/
void printScaredHunters(BuildingType* b){
  int i;
  int count = 0;
  int bored = 0;
  for(i = 0; i<MAX_HUNTERS; i++){
    if(b->hunters[i]->fear >= 100){
      printf("%s is a scaredy cat\n", b->hunters[i]->name);
      count += 1;
    }
    if(b->hunters[i]->boredomTime <= 0){
      bored += 1;
    }

    else{
      allEvidenceWin(b->hunters[i]);
    }
  }
  if (count == 4){
    printf("The ghost won\n");
  }
  else if(bored != 4){
    printf("The Hunters won\n");
  }
}
