//************************************************************
// this is a simple example that uses the painlessMesh library to 
// connect to a node on another network. Please see the WIKI on gitlab
// for more details
// https://gitlab.com/painlessMesh/painlessMesh/wikis/bridge-between-mesh-and-another-network
//************************************************************
#include "painlessMesh.h"

#define   MESH_PREFIX     "MeshNetworkWIFI"
#define   MESH_PASSWORD   "qwertyuiop"
#define   MESH_PORT       5555

#define   STATION_SSID     "LJX-4530"
#define   STATION_PASSWORD "qwertyuiop"
#define   STATION_PORT     5556
uint8_t   station_ip[4] =  {192,168,0,181}; // IP of the server

// prototypes
void receivedCallback( uint32_t from, String &msg );

Scheduler userScheduler; // to control your personal task
painlessMesh  mesh;
int toRead = 0;

void sendMessage(); 

Task taskSendMessage( TASK_SECOND * 1 , TASK_FOREVER, &sendMessage );

void sendMessage() {
  String msg;
  if (toRead == 0){
    msg = "Hello from Bridge ";
    msg += mesh.getNodeId();
  } else {
    msg = "Read sensor";
  } 
  mesh.sendBroadcast( msg );
  taskSendMessage.setInterval( random( TASK_SECOND * 1, TASK_SECOND * 5 ));
  Serial.printf("Bridge send");
}




void setup() {
  Serial.begin(115200);
  mesh.setDebugMsgTypes( ERROR | STARTUP | CONNECTION );  // set before init() so that you can see startup messages
  // Channel set to 6. Make sure to use the same channel for your mesh and for you other
  // network (STATION_SSID)
  mesh.init( MESH_PREFIX, MESH_PASSWORD, &userScheduler, MESH_PORT, WIFI_AP_STA, 6 );
  mesh.stationManual(STATION_SSID, STATION_PASSWORD, STATION_PORT, station_ip);
  // Bridge node, should (in most cases) be a root node. See [the wiki](https://gitlab.com/painlessMesh/painlessMesh/wikis/Possible-challenges-in-mesh-formation) for some background
  mesh.setRoot(true);
  // This and all other mesh should ideally now the mesh contains a root
  mesh.setContainsRoot(true);


  mesh.onReceive(&receivedCallback);
  userScheduler.addTask( taskSendMessage );
  taskSendMessage.enable();
}

void loop() {
  userScheduler.execute();
  mesh.update();
}

void receivedCallback( uint32_t from, String &msg ) {
  Serial.printf("bridge: Received from %u msg=%s\n", from, msg.c_str());
  if (msg.indexOf("read") >= 0) {
    toRead = 1;
  }
}
