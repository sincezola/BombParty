import ApiResponse from "../../types/response/ApiResponse.ts";
import { RoomPlayer } from "../../entities/RoomPlayer.ts";

export default abstract class RoomPlayerServiceProtocol {
  abstract createRoomPlayer(
    room_id: number,
    player_id: number,
    room_player_type: number
  ): Promise<ApiResponse<{ message: string } | RoomPlayer | []>>;
}
