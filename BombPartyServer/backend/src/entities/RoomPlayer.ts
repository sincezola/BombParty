import { RoomPlayerType, Player, Room } from "@prisma/client";

export interface RoomPlayerProps {
  room_player_id: number;
  room_id: number;
  player_id: number;
  room_player_type: number;
  type: RoomPlayerType;
  room: Room;
  player: Player;
}

export class RoomPlayer {
  constructor(private roomPlayerProps: RoomPlayerProps) {}

  get room_player_id() {
    return this.roomPlayerProps.room_player_id;
  }

  get room_id() {
    return this.roomPlayerProps.room_id;
  }

  get player_id() {
    return this.roomPlayerProps.player_id;
  }

  get room_player_type() {
    return this.roomPlayerProps.room_player_type;
  }

  get room() {
    return this.roomPlayerProps.room;
  }

  get player() {
    return this.roomPlayerProps.player;
  }
}
