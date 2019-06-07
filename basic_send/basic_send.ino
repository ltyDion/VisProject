//************************************************************
// this is a simple example that uses the painlessMesh library
//
// 1. sends a silly message to every node on the mesh at a random time between 1 and 5 seconds
// 2. prints anything it receives to Serial.print
//
//
//************************************************************
#include "painlessMesh.h"

#define   MESH_PREFIX     "MeshNetworkWIFI"
#define   MESH_PASSWORD   "qwertyuiop"
#define   MESH_PORT       5555

Scheduler userScheduler; // to control your personal task
painlessMesh  mesh;

// User stub
void sendMessage() ; // Prototype so PlatformIO doesn't complain

Task taskSendMessage( TASK_SECOND * 1 , TASK_FOREVER, &sendMessage );
int toRead = 0;

void sendMessage() {
  String msg;
  if (toRead == 1){
    msg = "hallRead value: ";
    int val = digitalRead(1);
    msg += val;
    msg += "from node: ";
    msg += mesh.getNodeId();
      
  } else {
    msg = "Hello from node: ";
    msg += mesh.getNodeId();
  }
  mesh.sendBroadcast( msg );
  taskSendMessage.setInterval( random( TASK_SECOND * 1, TASK_SECOND * 5 ));
  Serial.printf("Node send");
}

// Needed for painless library
void receivedCallback( uint32_t from, String &msg ) {
  if (msg.indexOf("read") >= 0){
    toRead = 1;
  } 
  Serial.printf("Node: Received from %u msg=%s%d\n", from, msg.c_str(), toRead);
}

void newConnectionCallback(uint32_t nodeId) {
    Serial.printf("--> startHere: New Connection, nodeId = %u\n", nodeId);
}

void changedConnectionCallback() {
  Serial.printf("Changed connections\n");
}

void nodeTimeAdjustedCallback(int32_t offset) {
    Serial.printf("Adjusted time %u. Offset = %d\n", mesh.getNodeTime(),offset);
}

void setup() {
  Serial.begin(115200);
  pinMode(0, INPUT);

  //mesh.setDebugMsgTypes( ERROR | MESH_STATUS | CONNECTION | SYNC | COMMUNICATION | GENERAL | MSG_TYPES | REMOTE ); // all types on
  mesh.setDebugMsgTypes( ERROR | STARTUP  );  // set before init() so that you can see startup messages

  mesh.init( MESH_PREFIX, MESH_PASSWORD, &userScheduler, MESH_PORT, WIFI_AP_STA );
  mesh.onReceive(&receivedCallback);
  mesh.onNewConnection(&newConnectionCallback);
  mesh.onChangedConnections(&changedConnectionCallback);
  mesh.onNodeTimeAdjusted(&nodeTimeAdjustedCallback);

  userScheduler.addTask( taskSendMessage );
  taskSendMessage.enable();
  Serial.printf("Node setUp");
}

void loop() {
  userScheduler.execute(); // it will run mesh scheduler as well
  mesh.update();
}
