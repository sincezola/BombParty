#ifndef ROUTES_API_H
  #define ROUTES_API_H

// #define API_HOST_ADDRESS     "127.0.0.1" // | !NO! NGINX
// #define API_HOST_PORT        "8080"      // | !NO! ADDRESS
#define API_HOST_ADDRESS     "localhost" // | NGINX
#define API_HOST_PORT        "80"        // | ADDRESS
#define BASE_PATH            "api"
#define GET_ALL_ROOM_PATH    "Room/GetAllRooms"
#define GET_ROOM_BY_ID_PATH  "Room/GetRoomById"
#define GET_ROOM_BY_STS_PATH "Room/GetRoomById"
#define CREATE_ROOM_PATH     "Room/CreateRoom"

#endif