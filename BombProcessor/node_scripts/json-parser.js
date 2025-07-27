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

  // console.log(dataJSON);

  if ("roomProps" in dataJSON) {
    parsedString += exportRoomReg + "|";
    parsedString += roomPropsReg + "|"

    parsedString = parsedString + String(dataJSON.roomProps.room_key) + "|";
    parsedString = parsedString + String(dataJSON.roomProps.room_status) + "|";
    parsedString = parsedString + String(dataJSON.roomProps.room_name) + "|";
    parsedString = parsedString + String(dataJSON.roomProps.room_password) + "|";
    parsedString = parsedString + String(dataJSON.roomProps.room_capacity) + "|";
    parsedString += String(dataJSON.roomProps.room_level) + "|";
    parsedString += "\n";
  }

  if ("status" in dataJSON.roomProps) {
    parsedString += exportRoomReg + "|";
    parsedString += statusReg + "|"

    parsedString += String(dataJSON.roomProps.status.status_type_id) + "|";
    parsedString += String(dataJSON.roomProps.status.code) + "|";
    parsedString += "\n";
  }

  if ("level" in dataJSON.roomProps) {
    parsedString += exportRoomReg + "|";
    parsedString += levelReg + "|";

    parsedString += String(dataJSON.roomProps.level.level_type_id) + "|";
    parsedString += String(dataJSON.roomProps.level.code) + "|";
    parsedString += "\n";
  }

  if ("players" in dataJSON.roomProps) {
    for (const rPlayer in dataJSON.roomProps.players) {
      parsedString += exportRoomReg + "|"
      parsedString += playerlistReg + "|";
      const tPlayer = dataJSON.roomProps.players[rPlayer]

      parsedString += String(tPlayer.player.player_key) + "|";
      parsedString += tPlayer.player.player_name + "|";
      parsedString += "\n";
    }
  }

  parsedString = parsedString.slice(0, -1);
  console.log(parsedString);
})

// node json-parser.js < input.txt > output.txt