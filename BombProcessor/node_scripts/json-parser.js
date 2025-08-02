let data = '';

const fileTitle = process.argv[1];
const createRoomTitle = "createroom.txt";
const joinRoomTitle = "joinroom.txt";
const getRoomTitle = "getroom.txt";
const exportGetRoomReg = "99";
const exportJoinRoomReg = "98";
const roomPropsReg = "01";
const statusReg = "02";
const levelReg = "03";
const playerlistReg = "04";

function parseJoinRoom(dataJSON, parsedString) {
  const roomProps = dataJSON.room;

  if ("roomProps" in roomProps) {
    parsedString += exportJoinRoomReg + "|";
    parsedString += roomPropsReg + "|";

    parsedString += String(roomProps.room_id) + "|";
    parsedString += String(roomProps.room_status) + "|";
    parsedString += String(roomProps.room_name) + "|";
    parsedString += String(roomProps.room_password) + "|";
    parsedString += String(roomProps.room_capacity) + "|";
    parsedString += String(roomProps.room_level) + "|";
    parsedString += "\n";
  }

  if ("type" in roomProps) {
    parsedString += exportJoinRoomReg + "|";
    parsedString += statusReg + "|";

    parsedString += String(roomProps.status.status_type_id) + "|";
    parsedString += String(roomProps.status.code) + "|";
    parsedString += "\n";
  }

  if ("level" in roomProps) {
    parsedString += exportJoinRoomReg + "|";
    parsedString += levelReg + "|";

    parsedString += String(roomProps.level.level_type_id) + "|";
    parsedString += String(roomProps.level.code) + "|";
    parsedString += "\n";
  }

  if ("players" in roomProps) {
    for (const tPlayer of roomProps.players) {
      parsedString += exportJoinRoomReg + "|";
      parsedString += playerlistReg + "|";

      parsedString += String(tPlayer.player.player_key) + "|";
      parsedString += tPlayer.player.player_name + "|";
      parsedString += "\n";
    }
  }
}
function parseCreateRoom(dataJSON, parsedString) {
  const roomProps = dataJSON.roomProps;

  if ("roomProps" in room) {
    parsedString += exportGetRoomReg + "|";
    parsedString += roomPropsReg + "|";

    parsedString += String(roomProps.room_key) + "|";
    parsedString += String(roomProps.room_status) + "|";
    parsedString += String(roomProps.room_name) + "|";
    parsedString += String(roomProps.room_password) + "|";
    parsedString += String(roomProps.room_capacity) + "|";
    parsedString += String(roomProps.room_level) + "|";
    parsedString += "\n";
  }

  if ("status" in roomProps) {
    parsedString += exportGetRoomReg + "|";
    parsedString += statusReg + "|";

    parsedString += String(roomProps.status.status_type_id) + "|";
    parsedString += String(roomProps.status.code) + "|";
    parsedString += "\n";
  }

  if ("level" in roomProps) {
    parsedString += exportGetRoomReg + "|";
    parsedString += levelReg + "|";

    parsedString += String(roomProps.level.level_type_id) + "|";
    parsedString += String(roomProps.level.code) + "|";
    parsedString += "\n";
  }

  if ("players" in roomProps) {
    for (const tPlayer of roomProps.players) {
      parsedString += exportGetRoomReg + "|";
      parsedString += playerlistReg + "|";

      parsedString += String(tPlayer.player.player_key) + "|";
      parsedString += tPlayer.player.player_name + "|";
      parsedString += "\n";
    }
  }
}
function parseGetRoom(dataJSON, parsedString) {
  for (const room of dataJSON) {
    const roomProps = room.roomProps;

    if ("roomProps" in room) {
      parsedString += exportGetRoomReg + "|";
      parsedString += roomPropsReg + "|";

      parsedString += String(roomProps.room_key) + "|";
      parsedString += String(roomProps.room_status) + "|";
      parsedString += String(roomProps.room_name) + "|";
      parsedString += String(roomProps.room_password) + "|";
      parsedString += String(roomProps.room_capacity) + "|";
      parsedString += String(roomProps.room_level) + "|";
      parsedString += "\n";
    }

    if ("status" in roomProps) {
      parsedString += exportGetRoomReg + "|";
      parsedString += statusReg + "|";

      parsedString += String(roomProps.status.status_type_id) + "|";
      parsedString += String(roomProps.status.code) + "|";
      parsedString += "\n";
    }

    if ("level" in roomProps) {
      parsedString += exportGetRoomReg + "|";
      parsedString += levelReg + "|";

      parsedString += String(roomProps.level.level_type_id) + "|";
      parsedString += String(roomProps.level.code) + "|";
      parsedString += "\n";
    }

    if ("players" in roomProps) {
      for (const tPlayer of roomProps.players) {
        parsedString += exportGetRoomReg + "|";
        parsedString += playerlistReg + "|";

        parsedString += String(tPlayer.player.player_key) + "|";
        parsedString += tPlayer.player.player_name + "|";
        parsedString += "\n";
      }
    }
  }
}

process.stdin.setEncoding('utf-8');

process.stdin.on('data', chunk => {
  data += chunk;
});

process.stdin.on('end', () => {
  const dataJSON = JSON.parse(data);

  let parsedString = "";

  if (fileTitle == getRoomTitle)
    parseGetRoom(dataJSON, parsedString);
  else if (fileTitle == joinRoomTitle)
    parseJoinRoom(dataJSON, parsedString);
  else if (fileTitle == createRoomTitle)
    parseCreateRoom(dataJSON, parsedString);


  parsedString = parsedString.slice(0, -1); // remove último \n
  console.log(parsedString);
});
