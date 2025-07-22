import RoomPlayerRepository from "../../repositories/RoomPlayer/RoomPlayerRepository.ts";
import HttpStatusCode from "../../types/enums/HttpStatusCode.ts";
import RoomPlayerServiceProtocol from "./RoomPlayerServiceProtocol.ts";
import ApiResponse from "../../types/response/ApiResponse.ts";
import { RoomPlayer } from "../../entities/RoomPlayer.ts";

class RoomPlayerService implements RoomPlayerServiceProtocol {
  constructor(private roomPlayerRepository: RoomPlayerRepository) {}

  async createRoomPlayer(
    room_id: number,
    player_id: number,
    room_player_type: number
  ): Promise<ApiResponse<{ message: string } | RoomPlayer | []>> {
    try {
      const createdRoomPlayer =
        await this.roomPlayerRepository.createPlayerRoom(
          room_id,
          player_id,
          room_player_type
        );

      if (!createdRoomPlayer)
        return {
          statusCode: HttpStatusCode.INTERNAL_SERVER_ERROR,
          body: { message: "Sorry, Internal Server Error" },
        };

      return {
        statusCode: HttpStatusCode.CREATED,
        body: createdRoomPlayer,
      };
    } catch (err) {
      console.error(err);

      return {
        statusCode: HttpStatusCode.INTERNAL_SERVER_ERROR,
        body: { message: "Sorry, Internal Server Error" },
      };
    }
  }
}

export default RoomPlayerService;
