import { RoomPlayerWithRelations } from "../../types/relations/roomplayerrelations.ts";

export default abstract class RoomPlayerRepositoryProtocol {
  abstract getRoomPlayer(
    room_player_id: number,
  ): Promise<RoomPlayerWithRelations | null>;
  abstract findByRoomAndPlayer(
    room_id: number,
    player_id: number,
  ): Promise<RoomPlayerWithRelations | null>;
  abstract createRoomPlayer(
    room_id: number,
    player_id: number,
    room_player_type: number,
  ): Promise<RoomPlayerWithRelations | null>;
  abstract deleteRoomPlayer(
    room_player_id: number,
  ): Promise<RoomPlayerWithRelations | null>;
}
