#include "defs.h"
/*
  Function: populateRooms
  Purpose:  creates rooms and adds them to the building
  in/out:   The building with rooms
   return:   void
*/
void populateRooms(BuildingType* building) {
    // First, create each room. Perhaps you want to include more data
    // in the init parameters?
    RoomType* van = calloc(1, sizeof(RoomType));
    initRoom(van, "Van");
    RoomType* hallway = calloc(1, sizeof(RoomType));
    initRoom(hallway, "Hallway");
    RoomType* master_bedroom = calloc(1, sizeof(RoomType));
    initRoom(master_bedroom, "Master Bedroom");
    RoomType* boys_bedroom = calloc(1, sizeof(RoomType));
    initRoom(boys_bedroom, "Boy's Bedroom");
    RoomType* bathroom = calloc(1, sizeof(RoomType));
    initRoom(bathroom, "Bathroom");
    RoomType* basement = calloc(1, sizeof(RoomType));
    initRoom(basement, "Basement");
    RoomType* basement_hallway = calloc(1, sizeof(RoomType));
    initRoom(basement_hallway, "Basement Hallway");
    RoomType* right_storage_room = calloc(1, sizeof(RoomType));
    initRoom(right_storage_room, "Right Storage Room");
    RoomType* left_storage_room = calloc(1, sizeof(RoomType));
    initRoom(left_storage_room, "Left Storage Room");
    RoomType* kitchen = calloc(1, sizeof(RoomType));
    initRoom(kitchen, "Kitchen");
    RoomType* living_room = calloc(1, sizeof(RoomType));
    initRoom(living_room, "Living Room");
    RoomType* garage = calloc(1, sizeof(RoomType));
    initRoom(garage, "Garage");
    RoomType* utility_room = calloc(1, sizeof(RoomType));
    initRoom(utility_room, "Utility Room");

    // Now create a linked list of rooms using RoomNodeType in the Building
    RoomNodeType* van_node = calloc(1, sizeof(RoomNodeType));
    van_node->room = van;
    RoomNodeType* hallway_node = calloc(1, sizeof(RoomNodeType));
    hallway_node->room = hallway;
    RoomNodeType* master_bedroom_node = calloc(1, sizeof(RoomNodeType));
    master_bedroom_node->room = master_bedroom;
    RoomNodeType* boys_bedroom_node = calloc(1, sizeof(RoomNodeType));
    boys_bedroom_node->room = boys_bedroom;
    RoomNodeType* bathroom_node = calloc(1, sizeof(RoomNodeType));
    bathroom_node->room = bathroom;
    RoomNodeType* basement_node = calloc(1, sizeof(RoomNodeType));
    basement_node->room = basement;
    RoomNodeType* basement_hallway_node = calloc(1, sizeof(RoomNodeType));
    basement_hallway_node->room = basement_hallway;
    RoomNodeType* right_storage_room_node = calloc(1, sizeof(RoomNodeType));
    right_storage_room_node->room = right_storage_room;
    RoomNodeType* left_storage_room_node = calloc(1, sizeof(RoomNodeType));
    left_storage_room_node->room = left_storage_room;
    RoomNodeType* kitchen_node = calloc(1, sizeof(RoomNodeType));
    kitchen_node->room = kitchen;
    RoomNodeType* living_room_node = calloc(1, sizeof(RoomNodeType));
    living_room_node->room = living_room;
    RoomNodeType* garage_node = calloc(1, sizeof(RoomNodeType));
    garage_node->room = garage;
    RoomNodeType* utility_room_node = calloc(1, sizeof(RoomNodeType));
    utility_room_node->room = utility_room;

    appendRoom(building->rooms, van_node);
    appendRoom(building->rooms, hallway_node);
    appendRoom(building->rooms, master_bedroom_node);
    appendRoom(building->rooms, boys_bedroom_node);
    appendRoom(building->rooms, bathroom_node);
    appendRoom(building->rooms, basement_node);
    appendRoom(building->rooms, basement_hallway_node);
    appendRoom(building->rooms, right_storage_room_node);
    appendRoom(building->rooms, left_storage_room_node);
    appendRoom(building->rooms, kitchen_node);
    appendRoom(building->rooms, living_room_node);
    appendRoom(building->rooms, garage_node);
    appendRoom(building->rooms, utility_room_node);

    // Now connect the rooms. It is possible you do not need a separate
    // function for this, but it is provided to give you a starting point.
    connectRooms(van, hallway);
    connectRooms(hallway, master_bedroom);
    connectRooms(hallway, boys_bedroom);
    connectRooms(hallway, bathroom);
    connectRooms(hallway, kitchen);
    connectRooms(hallway, basement);
    connectRooms(basement, basement_hallway);
    connectRooms(basement_hallway, right_storage_room);
    connectRooms(basement_hallway, left_storage_room);
    connectRooms(kitchen, living_room);
    connectRooms(kitchen, garage);
    connectRooms(garage, utility_room);
}
/*
  Function: initBuilding
  Purpose:  initializes a Building
  in/out:   The initialized Building
   return:   void
*/
void initBuilding(BuildingType* b){
  b->hunterCount = 0;
  b->rooms = NULL;

  RoomListType* house = (RoomListType*) calloc(1, sizeof(RoomListType));
  initRoomList(house);
  b->rooms = house;
}
/*
  Function: cleanupBuilding
  Purpose:  frees all allocated memory in the program
  in/out:   The building to be cleaned
   return:   void
*/
void cleanupBuilding(BuildingType* b){
  cleanupRoomEvidence(b->rooms);//cleans the evidence in the room
  cleanRoomList(b->rooms);//cleans the list of rooms in the room
  cleanupRoom(b->rooms);//cleans the room and its node

  cleanupHunterEvidence(b);//cleans all hunter's evidence
  free(b->rooms);

}
