let data = '';

const fileTitle = process.argv[2];
const createRoomTitle = "createroom.txt";
const joinRoomTitle = "joinroom.txt";
const getRoomTitle = "getroom.txt";

const exportGetRoomReg = "99";
const exportJoinRoomReg = "98";
const roomPropsReg = "01";
const statusReg = "02";
const levelReg = "03";
const playerlistReg = "04";

function parseJoinRoom(dataJSON, arr) {
  const roomProps = dataJSON.room;
  if (!roomProps) return;

  arr.push(`${exportJoinRoomReg}|${roomPropsReg}|${roomProps.room_id}|${roomProps.room_status}|${roomProps.room_name}|${roomProps.room_password}|${roomProps.room_capacity}|${roomProps.room_level}`);

  if ("status" in roomProps) {
    arr.push(`${exportJoinRoomReg}|${statusReg}|${roomProps.status.status_type_id}|${roomProps.status.code}`);
  }

  if ("level" in roomProps) {
    arr.push(`${exportJoinRoomReg}|${levelReg}|${roomProps.level.level_type_id}|${roomProps.level.code}`);
  }

  if ("players" in roomProps) {
    for (const tPlayer of roomProps.players) {
      arr.push(`${exportJoinRoomReg}|${playerlistReg}|${tPlayer.player.player_key}|${tPlayer.player.player_name}`);
    }
  }
}

function parseCreateRoom(dataJSON, arr) {
  const roomProps = dataJSON.roomProps;
  if (!roomProps) return;

  arr.push(`${exportGetRoomReg}|${roomPropsReg}|${roomProps.room_key}|${roomProps.room_status}|${roomProps.room_name}|${roomProps.room_password}|${roomProps.room_capacity}|${roomProps.room_level}`);

  if ("status" in roomProps) {
    arr.push(`${exportGetRoomReg}|${statusReg}|${roomProps.status.status_type_id}|${roomProps.status.code}`);
  }

  if ("level" in roomProps) {
    arr.push(`${exportGetRoomReg}|${levelReg}|${roomProps.level.level_type_id}|${roomProps.level.code}`);
  }

  if ("players" in roomProps) {
    for (const tPlayer of roomProps.players) {
      arr.push(`${exportGetRoomReg}|${playerlistReg}|${tPlayer.player.player_key}|${tPlayer.player.player_name}`);
    }
  }
}

function parseGetRoom(dataJSON, arr) {
  const roomProps = dataJSON.roomProps;
  if (!roomProps) return;

  for (const room of roomProps) {
    arr.push(`${exportGetRoomReg}|${roomPropsReg}|${room.room_key}|${room.room_status}|${room.room_name}|${room.room_password}|${room.room_capacity}|${room.room_level}`);

    if ("status" in room) {
      arr.push(`${exportGetRoomReg}|${statusReg}|${room.status.status_type_id}|${room.status.code}`);
    }

    if ("level" in room) {
      arr.push(`${exportGetRoomReg}|${levelReg}|${room.level.level_type_id}|${room.level.code}`);
    }

    if ("players" in room) {
      for (const tPlayer of room.players) {
        arr.push(`${exportGetRoomReg}|${playerlistReg}|${tPlayer.player.player_key}|${tPlayer.player.player_name}`);
      }
    }
  }
}

process.stdin.setEncoding('utf-8');

process.stdin.on('data', chunk => {
  data += chunk;
});

process.stdin.on('end', () => {
  try {
    const dataJSON = JSON.parse(data);
    const parsedArray = [];

    if (fileTitle === getRoomTitle)
      parseGetRoom(dataJSON, parsedArray);
    else if (fileTitle === joinRoomTitle)
      parseJoinRoom(dataJSON, parsedArray);
    else if (fileTitle === createRoomTitle)
      parseCreateRoom(dataJSON, parsedArray);

    const parsedString = parsedArray.join('\n');
    console.log(parsedString);
  } catch (err) {
    console.error('Erro ao processar os dados:', err.message);
  }
});
