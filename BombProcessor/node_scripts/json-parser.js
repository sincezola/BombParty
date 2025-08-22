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

// ---------------- Funções de Parse ---------------- //
// "room":{"room_key":3,"room_status":1,"room_name":"SALA RENATO","room_password":null,"room_capacity":4,"room_level":1,"created_at":"2025-08-16T14:54:26.312Z","last_changed":"2025-08-16T14:54:26.312Z"},"player":{"player_key":6,"player_name":"since"},"type":{"room_player_type_id":2,"code":"participant"}}}] **/

function parseJoinRoom(dataJSON, arr) {
  const roomProps = dataJSON.roomPlayerProps.room;
  const player = dataJSON.roomPlayerProps.player;
  if (!roomProps) return;

  arr.push(`${exportJoinRoomReg}|${roomPropsReg}|${roomProps.room_key}|${roomProps.room_status}|${roomProps.room_name}|${roomProps.room_password ?? "-1"}|${roomProps.room_capacity}|${roomProps.room_level}`);

  // if ("status" in roomProps) {
  //   arr.push(`${exportJoinRoomReg}|${statusReg}|${roomProps.status.status_type_id}|${roomProps.status.code}`);
  // }

  // if ("level" in roomProps) {
  //   arr.push(`${exportJoinRoomReg}|${levelReg}|${roomProps.level.level_type_id}|${roomProps.level.code}`);
  // }

  arr.push(`${exportJoinRoomReg}|${playerlistReg}|${player.player_key}|${player.player_name}`);
}

function parseCreateRoom(dataJSON, arr) {
  let roomProps = dataJSON.roomProps;
  if (!roomProps) return;

  // Garante que seja array
  if (!Array.isArray(roomProps)) {
    roomProps = [roomProps];
  }

  for (const room of roomProps) {
    arr.push(`${exportGetRoomReg}|${roomPropsReg}|${room.room_key}|${room.room_status}|${room.room_name}|${room.room_password ?? "-1"}|${room.room_capacity}|${room.room_level}`);

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

function parseGetRoom(dataJSON, arr) {
  if (!Array.isArray(dataJSON)) return;

  for (const obj of dataJSON) {
    const room = obj.roomProps;
    if (!room) continue;

    arr.push(`${exportGetRoomReg}|${roomPropsReg}|${room.room_key}|${room.room_status}|${room.room_name}|${room.room_password ? room.room_password : "-1"}|${room.room_capacity}|${room.room_level}`);

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

// ---------------- Entrada de Dados ---------------- //

process.stdin.setEncoding('utf-8');

process.stdin.on('data', chunk => {
  data += chunk;
});

process.stdin.on('end', () => {
  try {
    let dataJSON = JSON.parse(data);

    // ([[{...}]] -> {...})
    while (Array.isArray(dataJSON) && dataJSON.length === 1) {
      dataJSON = dataJSON[0];
    }

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
