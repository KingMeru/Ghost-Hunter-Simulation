#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <math.h>
#include <unistd.h>
#include <pthread.h>
#include <semaphore.h>

#define MAX_STR            64
#define FEAR_RATE           1
#define MAX_FEAR            99
#define MAX_HUNTERS         4
#define BOREDOM_MAX       100
#define ARR_SIZE            5

typedef enum { EMF, TEMPERATURE, FINGERPRINTS, SOUND } EvidenceClassType;
typedef enum { POLTERGEIST, BANSHEE, BULLIES, PHANTOM } GhostClassType;

typedef struct Room{
  char name[MAX_STR];
  struct RoomList* rooms;//list of all adjacent rooms
  struct Evidence** roomEvidence;//list of all ghost evidence
  struct Hunter* hunters[MAX_HUNTERS];//list of hunters in a room
  struct Ghost* ghost;
  int roomCount;
  int eviCapacity;
  int eviCount;
  int hunterCount;
  sem_t mutex;
} RoomType;

typedef struct RoomNode {
  struct Room* room;
  struct RoomNode* next;
}RoomNodeType;

typedef struct RoomList {
  RoomNodeType* head;
}RoomListType;

typedef struct Evidence{
  float value;
  EvidenceClassType evidenceType;
} EvidenceType;

typedef struct Hunter{
  char name[MAX_STR];
  int fear;
  struct Room* currentRoom;
  EvidenceClassType device;
  struct Evidence** collectedEvidence;
  int eviCapacity;
  int eviCount;
  int boredomTime;
} HunterType;

typedef struct Ghost{
  int boredomTime;
  struct Room* currentRoom;
  GhostClassType ghostType;
  EvidenceClassType* dropEvidence;
} GhostType;

typedef struct Building{
  struct RoomList* rooms;
  struct Hunter* hunters[MAX_HUNTERS];
  struct Ghost* ghost;
  int hunterCount;
  int cap;
  struct EvidenceType* allEvidence;
} BuildingType;

int randInt(int, int);          // Generates a pseudorandom integer between the parameters
float randFloat(float, float);  // Generates a pseudorandom float between the parameters

void initBuilding(BuildingType*);
void populateRooms(BuildingType*);// Populates the building with sample data for rooms
void initRoomList(RoomListType*);
void initRoom(RoomType*, char*);
void appendRoom(RoomListType*, RoomNodeType*);
void connectRooms(RoomType*, RoomType*);
void cleanRoomList(RoomListType*);
void cleanupRoomEvidence(RoomListType*);
void cleanupRoom(RoomListType*);

void generateEvidence(GhostType*);
void generateEMF(GhostType*);
void generateTEMP(GhostType*);
void generateFING(GhostType*);
void generateSOUND(GhostType*);
void appendEvidence(RoomType*, EvidenceType*);
void growEvidence(RoomType*);
void deleteEvidence(RoomType*, EvidenceType*);
void collectEvidence(HunterType*);
void generateHunterEvidence(HunterType*);
void generateHuntEMF(HunterType*);
void generateHuntTEMP(HunterType*);
void generateHuntFING(HunterType*);
void generateHuntSOUND(HunterType*);
void addHunterEvidence(HunterType*, EvidenceType*);
void growHuntEvidence(HunterType*);
void hunterEvidenceType(EvidenceType*, HunterType*);
void* hunterIsBored(void*);
int allEvidenceHunt(HunterType*);
void printScaredHunters(BuildingType*);
void cleanupHunterEvidence(BuildingType*);
void duplicateHunterEvidence(HunterType*, EvidenceType*);
void allEvidenceWin(HunterType*);



void ghostRoom(GhostType*);
void initGhost(GhostType*, BuildingType*);
void initGhostLocation(GhostType*, BuildingType*);
void printGhost(GhostType*);

void initHunter(char*, HunterType*, BuildingType*);
void appendHunter(RoomType*, HunterType*);
void moveHunter(HunterType*);
void deleteHunter(HunterType*, RoomType*);
void communicateHunter(HunterType*);
void* ghostIsBored(void*);

void addBuildingHunter(HunterType*, BuildingType*);
void cleanupBuilding(BuildingType *);
