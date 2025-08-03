#ifndef ROUTES_API_H
  #define ROUTES_API_H

// #define pszAPI_URL_ADDRESS     "127.0.0.1" // | !NO! NGINX
// #define API_HOST_PORT        "8080"      // | !NO! ADDRESS
// #define pszAPI_URL_ADDRESS     "localhost" // | NGINX
// #define API_HOST_PORT        "80"        // | ADDRESS
extern char pszAPI_URL_ADDRESS[1024];       // | render
#define API_HOST_PORT        ""             // | render
#define BASE_PATH            "api"
#define GET_ALL_ROOM_PATH    "Room/GetAllRooms"
#define GET_ROOM_BY_ID_PATH  "Room/GetRoomById"
#define GET_ROOM_BY_STS_PATH "Room/GetStatusRooms"
#define CREATE_ROOM_PATH     "Room/CreateRoom"
#define JOIN_ROOM_PATH       "RoomPlayer/JoinRoom"

#endif