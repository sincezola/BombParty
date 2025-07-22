import { RoomPlayer } from "../../entities/RoomPlayer.ts";

export default abstract class RoomPlayerRepositoryProtocol {
  abstract createPlayerRoom(
    room_id: number,
    player_id: number,
    room_player_type: number
  ): Promise<RoomPlayer | null>;
}
