let data = '';

const exportRoomReg = "99";
const roomPropsReg = "01";
const statusReg = "02";
const levelReg = "03";
const playerlistReg = "04";

process.stdin.setEncoding('utf-8');

process.stdin.on('data', chunk => {
  data += chunk;
});

process.stdin.on('end', () => {
  const dataJSON = JSON.parse(data);

  let parsedString = "";

  for (const room of dataJSON) {
    const roomProps = room.roomProps;

    if ("roomProps" in room) {
      parsedString += exportRoomReg + "|";
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
      parsedString += exportRoomReg + "|";
      parsedString += statusReg + "|";

      parsedString += String(roomProps.status.status_type_id) + "|";
      parsedString += String(roomProps.status.code) + "|";
      parsedString += "\n";
    }

    if ("level" in roomProps) {
      parsedString += exportRoomReg + "|";
      parsedString += levelReg + "|";

      parsedString += String(roomProps.level.level_type_id) + "|";
      parsedString += String(roomProps.level.code) + "|";
      parsedString += "\n";
    }

    if ("players" in roomProps) {
      for (const tPlayer of roomProps.players) {
        parsedString += exportRoomReg + "|";
        parsedString += playerlistReg + "|";

        parsedString += String(tPlayer.player.player_key) + "|";
        parsedString += tPlayer.player.player_name + "|";
        parsedString += "\n";
      }
    }
  }

  parsedString = parsedString.slice(0, -1); // remove último \n
  console.log(parsedString);
});
