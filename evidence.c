#include "defs.h"
/*
  Function: generateEvidence
  Purpose:  generates evidence for ghost to place in rooms
  in:   The ghost
   return:   void
*/
void generateEvidence(GhostType* ghost){
  int x = randInt(1, 4);
  if(ghost->ghostType == POLTERGEIST){
    if(x == 1){
      generateEMF(ghost);
    }else if(x == 2){
      generateTEMP(ghost);
    }else if(x==3){
      generateFING(ghost);
    }
  }
  else if(ghost->ghostType == BANSHEE){
    if(x == 1){
      generateEMF(ghost);
    }else if(x == 2){
      generateTEMP(ghost);
    }else if(x==3){
      generateSOUND(ghost);
    }
  }
  else if(ghost->ghostType == BULLIES){
    if(x == 1){
      generateEMF(ghost);
    }else if(x == 2){
      generateFING(ghost);
    }else if(x==3){
      generateSOUND(ghost);
    }
  }
  else if(ghost->ghostType == BULLIES){
    if(x == 1){
      generateTEMP(ghost);
    }else if(x == 2){
      generateFING(ghost);
    }else if(x==3){
      generateSOUND(ghost);
    }
  }
  return;
}
/*
  Function: generateEMF
  Purpose:  generates EMF evidence for the ghost
  in:   The Ghost
   return:   void
*/
void generateEMF(GhostType* ghost){
  EvidenceType* newEvi = calloc(1, sizeof(EvidenceType));
  newEvi->evidenceType = EMF;
  newEvi->value = randFloat(0.0, 5.0);

  sem_wait((&ghost->currentRoom->mutex));
  appendEvidence(ghost->currentRoom, newEvi);
  sem_post((&ghost->currentRoom->mutex));

  if(newEvi->value > 4.9){
    printf("The ghost generated Ghostly EMF\n");
  }
  else{
    printf("The ghost generated standard EMF\n");
  }
  return;
}
/*
  Function: generateTEMP
  Purpose:  generates TEMPERATURE evidence for the ghost
  in:  The Ghost
  return:  void
*/
void generateTEMP(GhostType* ghost){
  EvidenceType* newEvi = calloc(1, sizeof(EvidenceType));
  newEvi->evidenceType = TEMPERATURE;
  newEvi->value = randFloat(-10.0, 27.0);
  sem_wait((&ghost->currentRoom->mutex));
  appendEvidence(ghost->currentRoom, newEvi);
  sem_post((&ghost->currentRoom->mutex));

  if(newEvi->value < 0){
    printf("The ghost generated Ghostly TEMPERATURE\n");
  }
  else{
    printf("The ghost generated standard TEMPERATURE\n");
  }
  return;
}
/*
  Function: generateFING
  Purpose:  generates FINGERPRINTS evidence for the ghost
  in:   The Ghost
   return:   void
*/
void generateFING(GhostType* ghost){
  EvidenceType* newEvi = calloc(1, sizeof(EvidenceType));
  newEvi->evidenceType = FINGERPRINTS;
  newEvi->value = randFloat(0.0, 1);
  sem_wait((&ghost->currentRoom->mutex));
  appendEvidence(ghost->currentRoom, newEvi);
  sem_post((&ghost->currentRoom->mutex));

  if(newEvi->value > 0){
    printf("The ghost generated Ghostly FINGERPRINTS\n");
  }
  else{
    printf("The ghost generated standard FINGERPRINTS\n");
  }
  return;
}
/*
  Function: generateSOUND
  Purpose:  generates SOUND evidence for the ghost
  in:   The Ghost
   return:   void
*/
void generateSOUND(GhostType* ghost){
  EvidenceType* newEvi = calloc(1, sizeof(EvidenceType));
  newEvi->evidenceType = SOUND;
  newEvi->value = randFloat(40.0, 75.0);
  sem_wait((&ghost->currentRoom->mutex));
  appendEvidence(ghost->currentRoom, newEvi);
  sem_post((&ghost->currentRoom->mutex));

  if(newEvi->value > 70){
    printf("The ghost generated Ghostly SOUND\n");
  }
  else{
    printf("The ghost generated standard SOUND\n");
  }
  return;
}
/*
  Function: appendEvidence
  Purpose:  adds evidence to a rooms list
  in:   The Evidence
  in/out: The room
   return:   void
*/
void appendEvidence(RoomType* room, EvidenceType* evi) {
  if((room->eviCount) >= (room->eviCapacity)){
    growEvidence(room);
    room->roomEvidence[room->eviCount] = evi;
    room->eviCount ++;
  }
  else{
    room->roomEvidence[room->eviCount] = evi;
    room->eviCount++;
  }
  return;
}
/*
  Function: growEvidence
  Purpose: increases the size of a room's evidence list
  in/out:   The room
   return:   void
*/
void growEvidence(RoomType* room) {
  room->eviCapacity *= 2;
  EvidenceType** newEvi = (EvidenceType**) calloc(room->eviCapacity, sizeof(EvidenceType*));
  for(int i = 0; i<(room->eviCount); i++){
    newEvi[i] = room->roomEvidence[i];
  }
  free(room->roomEvidence);
  room->roomEvidence = newEvi;
  return;
}
/*
  Function: deleteEvidence
  Purpose: removes evidence from a room's evidence list
  in/out:   The room
  in: The evidence to be removed
   return:   void
*/
void deleteEvidence(RoomType* room, EvidenceType* evi){
  for(int i = 0; i < room->eviCount; i++){
    if(room->roomEvidence[i] == evi){
      free(room->roomEvidence[i]);
      for(int j = i; j < room->eviCount; ++j){
        room->roomEvidence[j] = room->roomEvidence[j+1];
      }
      room->eviCount -= 1;
    }
  }
  return;
}
/*
  Function: collectEvidence
  Purpose: copies evidence from the room to the hunter's collection
  in/out:   The hunter
   return:   void
*/
void collectEvidence(HunterType* hunter){
  if(hunter->currentRoom->eviCount == 0){
    generateHunterEvidence(hunter);
    hunter->boredomTime -= 1;
  }
  else{
    for(int i = 0; i < hunter->currentRoom->eviCount; i++){
      if(hunter->currentRoom->roomEvidence[i]->evidenceType == hunter->device){
        sem_wait(&(hunter->currentRoom->mutex));
        duplicateHunterEvidence(hunter, hunter->currentRoom->roomEvidence[i]);
        hunterEvidenceType(hunter->currentRoom->roomEvidence[i], hunter);
        deleteEvidence(hunter->currentRoom, hunter->currentRoom->roomEvidence[i]);
        sem_post(&(hunter->currentRoom->mutex));
      }
    }
  }

  return;
}
/*
  Function: generateHunterEvidence
  Purpose: generates evidence for hunter if no evidence in current room
  in/out:   The hunter
   return:   void
*/
void generateHunterEvidence(HunterType* hunter){
  if(hunter->device == EMF){
    generateHuntEMF(hunter);
    printf("%s, generated EMF evidence\n", hunter->name);
  }
  else if(hunter->device == TEMPERATURE){
    generateHuntTEMP(hunter);
    printf("%s, generated TEMPERATURE evidence\n", hunter->name);
  }
  else if(hunter->device == FINGERPRINTS){
    generateHuntFING(hunter);
    printf("%s, generated FINGERPRINTS evidence\n", hunter->name);
  }
  else if(hunter->device == SOUND){
    generateHuntSOUND(hunter);
    printf("%s, generated SOUND evidence\n", hunter->name);
  }
  return;
}
/*
  Function: generateHuntEMF
  Purpose: generates EMF evidence for the Hunter
  in/out:   The hunter
   return:   void
*/
void generateHuntEMF(HunterType* hunter){
  EvidenceType* newEvi = calloc(1, sizeof(EvidenceType));
  newEvi->evidenceType = EMF;
  newEvi->value = randFloat(0.0, 4.9);
  addHunterEvidence(hunter, newEvi);
  return;
}
/*
  Function: generateHuntTEMP
  Purpose: generates TEMP evidence for the Hunter
  in/out:   The hunter
   return:   void
*/
void generateHuntTEMP(HunterType* hunter){
  EvidenceType* newEvi = calloc(1, sizeof(EvidenceType));
  newEvi->evidenceType = TEMPERATURE;
  newEvi->value = randFloat(0.0, 27.0);
  addHunterEvidence(hunter, newEvi);
  return;
}
/*
  Function: generateHuntFING
  Purpose: generates FINGERPRINTS evidence for the Hunter
  in/out:   The hunter
   return:   void
*/
void generateHuntFING(HunterType* hunter){
  EvidenceType* newEvi = calloc(1, sizeof(EvidenceType));
  newEvi->evidenceType = FINGERPRINTS;
  newEvi->value = 0;
  addHunterEvidence(hunter, newEvi);
  return;
}
/*
  Function: generateHuntSOUND
  Purpose: generates SOUND evidence for the Hunter
  in/out:   The hunter
   return:   void
*/
void generateHuntSOUND(HunterType* hunter){
  EvidenceType* newEvi = calloc(1, sizeof(EvidenceType));
  newEvi->evidenceType = SOUND;
  newEvi->value = randFloat(40.0, 70.0);
  addHunterEvidence(hunter, newEvi);
  return;
}
/*
  Function: addHunterEvidence
  Purpose: appends evidence to the end of the hunter's evidence list
  in/out:   The hunter
   return:   void
*/
void addHunterEvidence(HunterType* hunter, EvidenceType* newEvi){
  if((hunter->eviCount) >= (hunter->eviCapacity)){
    growHuntEvidence(hunter);
    hunter->collectedEvidence[hunter->eviCount] = newEvi;
    hunter->eviCount ++;
  }
  else{
    hunter->collectedEvidence[hunter->eviCount] = newEvi;
    hunter->eviCount++;
  }
  return;
}
/*
  Function: duplicateHunterEvidence
  Purpose: copies evidence for hunter's list
  in/out:   The hunter
  in: The evidence to be copied
   return:   void
*/
void duplicateHunterEvidence(HunterType* hunter, EvidenceType* newEvi){
  if((hunter->eviCount) >= (hunter->eviCapacity)){
    growHuntEvidence(hunter);
    EvidenceType* dupEvi = calloc(1, sizeof(EvidenceType));
    dupEvi->evidenceType = newEvi->evidenceType;
    dupEvi->value = newEvi->value;

    hunter->collectedEvidence[hunter->eviCount] = newEvi;
    hunter->eviCount ++;
  }
  else{
    EvidenceType* dupEvi = calloc(1, sizeof(EvidenceType));
    dupEvi->evidenceType = newEvi->evidenceType;
    dupEvi->value = newEvi->value;
    hunter->collectedEvidence[hunter->eviCount] = newEvi;
    hunter->eviCount++;
  }
  return;
}
/*
  Function: growHuntEvidence
  Purpose: increases the size of the Hunters evidence array
  in/out:   The hunter
   return:   void
*/
void growHuntEvidence(HunterType* hunter){
  hunter->eviCapacity *= 2;
  EvidenceType** newEvi = (EvidenceType**) calloc(hunter->eviCapacity, sizeof(EvidenceType*));
  for(int i = 0; i<(hunter->eviCount); i++){
    newEvi[i] = hunter->collectedEvidence[i];
  }
  free(hunter->collectedEvidence);
  hunter->collectedEvidence = newEvi;
  return;
}
/*
  Function: hunterEvidenceType
  Purpose: checks if the hunter's collected evidence is ghostly or not and set the boredom time accordingly
  in/out:   The hunter
  in: The evidence
   return:   void
*/
void hunterEvidenceType(EvidenceType* evi, HunterType* hunter){
  if((evi->evidenceType == EMF) && (hunter->device == EMF) && (evi->value > 4.9)){
    hunter->boredomTime = BOREDOM_MAX;
    printf("%s collected ghostly EMF evidence\n", hunter->name);
  }
  else if((evi->evidenceType == TEMPERATURE) && (hunter->device == TEMPERATURE) && (evi->value < 0)){
    hunter->boredomTime = BOREDOM_MAX;
    printf("%s collected ghostly TEMPERATURE evidence\n", hunter->name);
  }
  else if((evi->evidenceType == FINGERPRINTS) && (hunter->device == FINGERPRINTS) && (evi->value > 0)){
    hunter->boredomTime = BOREDOM_MAX;
    printf("%s collected ghostly FINGERPRINTS evidence\n", hunter->name);
  }
  else if((evi->evidenceType == SOUND) && (hunter->device == SOUND) && (evi->value > 70)){
    hunter->boredomTime = BOREDOM_MAX;
    printf("%s collected ghostly SOUND evidence\n", hunter->name);
  }
  else{
    hunter->boredomTime -= 1;
    printf("%s collected standard ghost evidence\n", hunter->name);
  }
  return;
}
