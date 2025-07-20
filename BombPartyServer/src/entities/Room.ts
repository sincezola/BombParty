import { LevelType, RoomPlayer, StatusType } from "@prisma/client";

export interface RoomProps {
  room_key: number;
  room_name: string;
  room_password: string | null;
  room_capacity: number;
  status: StatusType;
  level: LevelType;
  players: RoomPlayer[];
  created_at?: Date;
  last_changed?: Date;
}

export class Room {
  constructor(private roomProps: RoomProps) {}

  get room_key() {
    return this.roomProps.room_key;
  }
  get room_name() {
    return this.roomProps.room_name;
  }
  get room_password() {
    return this.roomProps.room_password;
  }
  get room_capacity() {
    return this.roomProps.room_capacity;
  }
  get status() {
    return this.roomProps.status;
  }
  get level() {
    return this.roomProps.level;
  }
  get players() {
    return this.roomProps.players;
  }
  get created_at() {
    return this.roomProps.created_at;
  }
  get last_changed() {
    return this.roomProps.last_changed;
  }

  toDTO() {
    return {
      room_key: this.room_key,
      room_name: this.room_name,
      room_capacity: this.room_capacity,
      status: this.status.code,
      level: this.level.code,
      players: this.players,
      created_at: this.created_at,
      last_changed: this.last_changed,
    };
  }
}
