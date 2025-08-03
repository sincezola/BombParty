#ifndef ROUTES_API_H
  #define ROUTES_API_H

// #define pszAPI_URL_ADDRESS     "127.0.0.1" // | !NO! NGINX
// #define pszAPI_URL_ADDRESS     "localhost" // | NGINX
// #define API_HOST_PORT        ""            // | external service
// #define API_HOST_PORT        "80"          // | ADDRESS
#define API_HOST_PORT        "8080"           // | !NO! ADDRESS
extern char pszAPI_URL_ADDRESS[1024];  
#define BASE_PATH            "api"
#define GET_ALL_ROOM_PATH    "Room/GetAllRooms"
#define GET_ROOM_BY_ID_PATH  "Room/GetRoomById"
#define GET_ROOM_BY_STS_PATH "Room/GetStatusRooms"
#define CREATE_ROOM_PATH     "Room/CreateRoom"
#define JOIN_ROOM_PATH       "RoomPlayer/JoinRoom"
#define LEAVE_ROOM_PATH      "RoomPlayer/LeaveRoom"
#define PATCH_ROOM_PATH      "Room/PatchRoom"
#define DELETE_ROOM_PATH     "RoomPlayer/LeaveRoom"

#endif