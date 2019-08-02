#include "main.h"

void handlehostpacket(packet_t* packet) {
 message_t* message;
 byte_t content[128];
 int i;
 
 for (i = 0; i < packet->messagecount; i++) {
  message = &packet->messages[i];
  
  switch (message->type) {
  case MSG_ADDCLIENT:
   //reprintstring(message->data);
   
   //directmessage(&message, MSG_ADDCLIENT, content, 15);
   //appendclientmessage(&message, packet->sender);
   
   //putclient();
   
   //directmessage(&message, MSG_UNITWORDS, DEFAULTQUEUE, content, ~);
   //appendclientmessage(&message, );
   break;
   
  case MSG_LEVELBITMAP:
   break;
   
  case MSG_REMOVECLIENT:
   break;
   
  case MSG_COMMAND:
   // command, operand (crafted item)
   break;
   
  default:
   LOGREPORT("received an unhandled message type.");
   break;
  }
 }
}

void handleclientpacket(packet_t* packet) {
 message_t* message;
 int i;
 
 for (i = 0; i < packet->messagecount; i++) {
  message = &packet->messages[i];
  
  switch (message->type) {
  case MSG_ADDCLIENT:
   //
   break;
   
  case MSG_REMOVECLIENT:
   // nada
   closehost();
   break;
   
  case MSG_PLAYSOUND:
   // x, y, sound (15 characters)
   playsound("");
   break;
   
  case MSG_SYNCHRONIZE:
   break;
   
  case MSG_UPDATECHUNK:
   // level, section, tiles
   break;
   
  case MSG_UPDATEUNIT:
   // level, unit (unit id, unit data)
   break;
   
  default:
   LOGREPORT("received an unhandled message type.");
   break;
  }
 }
 
}

void routepacket(packet_t* packet) {
 // TODO: handle other packet types
 
 switch (hoststate()) {
 case HOST_CLIENT:
  handleclientpacket(packet);
  break;
  
 case HOST_LOCAL:
  handlehostpacket(packet);
  break;
  
 case HOST_UNMADE:
  /* no break */
  
 default:
  LOGREPORT("unpredicted or invalid host state.");
  break;
 }
}
