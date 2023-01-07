#include "defs.h"
/*
  Function: initRoom
  Purpose: initializes a room
  in/out:   The room
  in: The room's name
  return:   void
*/
void initRoom(RoomType* room, char* name){
  strcpy(room->name, name);
  room->roomCount = 0;
  room->eviCapacity = ARR_SIZE;
  room->eviCount = 0;
  room->hunterCount = 0;

  RoomListType* newRoomList = (RoomListType*) calloc(1, sizeof(RoomListType));
  initRoomList(newRoomList);
  room->rooms = newRoomList;

  room->roomEvidence = (EvidenceType**) calloc(room->eviCapacity, sizeof(EvidenceType*));

  room->ghost = NULL;

  sem_init(&room->mutex, 0, 1);
}
/*
  Function: initRoomList
  Purpose: initializes a room list
  in/out: a room list
  return:   void
*/
void initRoomList(RoomListType* rooms){
  rooms->head = NULL;
}
/*
  Function: appendRoom
  Purpose: adds a room to the end of a room list
  in/out: a room list
  in: the room node
  return:   void
*/
void appendRoom(RoomListType* list, RoomNodeType* room){
  room->next = NULL;

  if(list->head == NULL){
    list->head = room;
  }
  else{
    RoomNodeType* prev = list->head;
    RoomNodeType* temp =  prev->next;
    while(temp != NULL){
      prev = temp;
      temp = prev->next;
    }
    prev->next = room;
  }
}
/*
  Function: connectRooms
  Purpose: connects two rooms together
  in/out: a room
  in: a room to be connected
  return:   void
*/
void connectRooms(RoomType* room, RoomType* connectRoom){
  RoomNodeType* newNode = calloc(1, sizeof(RoomNodeType));
  newNode->room = connectRoom;
  appendRoom(room->rooms, newNode);
  room->roomCount += 1;
}
/*
  Function: cleanRoomList
  Purpose: cleans the list of rooms in  a room
  in/out: a room list
  return:   void
*/
void cleanRoomList(RoomListType* b){
  RoomNodeType* prev = b->head;

  while(prev != NULL){
    RoomNodeType* curr = prev->room->rooms->head;
    while(curr != NULL){
      RoomNodeType* next = curr->next;
      free(curr);
      curr = next;
    }
    free(prev->room->rooms);
    prev = prev->next;
  }
}
/*
  Function: cleanupRoomEvidence
  Purpose: frees all evidence in a room
  in/out: a room list
  return:   void
*/
void cleanupRoomEvidence(RoomListType* b){
  RoomNodeType* prev = b->head;

  while(prev != NULL){
    int i;
    int count = prev->room->eviCount;
    for(i = 0; i<count; i++){
      free(prev->room->roomEvidence[i]);
    }
    free(prev->room->roomEvidence);
    prev = prev->next;
  }
}
/*
  Function: cleanupRoom
  Purpose: frees all room and room nodes in a building
  in/out: a room list
  return:   void
*/
void cleanupRoom(RoomListType* b){
  RoomNodeType* prev = b->head;

  while(prev != NULL){
    RoomNodeType* next = prev->next;
    free(prev->room);
    free(prev);
    prev = next;
  }
}
