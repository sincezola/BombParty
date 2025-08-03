import RoomPlayerServiceProtocol from "./RoomPlayerServiceProtocol.ts";
import ApiResponse from "../../types/response/ApiResponse.ts";
import HttpStatusCode from "../../types/enums/HttpStatusCode.ts";
import RoomPlayerRepository from "../../repositories/RoomPlayerRepository/RoomPlayerRepository.ts";
import PlayerService from "../PlayerService/PlayerService.ts";
import { RoomPlayer } from "../../entities/RoomPlayer.ts";
import { RoomPlayerWithRelations } from "../../types/relations/roomplayerrelations.ts";
import { Player } from "../../entities/Player.ts";

class RoomPlayerService implements RoomPlayerServiceProtocol {
  constructor(
    private roomPlayerRepository: RoomPlayerRepository,
    private playerService: PlayerService
  ) {}

  private handleError() {
    return {
      statusCode: HttpStatusCode.INTERNAL_SERVER_ERROR,
      body: { message: "Sorry, Internal Server Error" },
    };
  }

  private mapRoomPlayer(raw: RoomPlayerWithRelations): RoomPlayer {
    return new RoomPlayer(raw);
  }

  async createRoomPlayer(
    room_key: number,
    player_id: number,
    room_player_type: number
  ): Promise<ApiResponse<{ message: string } | RoomPlayer>> {
    try {
      const overlappingRoomPlayer =
        await this.roomPlayerRepository.getRoomPlayerByPlayerId(player_id);

      if (overlappingRoomPlayer) {
        return {
          statusCode: HttpStatusCode.CONFLICT,
          body: {
            message: `Player Id: ${player_id} already have a relation with a room in database.`,
          },
        };
      }

      const getPlayer = await this.playerService.getPlayerById(player_id);

      if (!(getPlayer.body instanceof Player)) {
        const { statusCode, body } = getPlayer;

        return {
          statusCode,
          body,
        };
      }

      const createdRoomPlayer =
        await this.roomPlayerRepository.createRoomPlayer(
          room_key,
          player_id,
          room_player_type
        );

      if (!createdRoomPlayer) {
        return {
          statusCode: HttpStatusCode.INTERNAL_SERVER_ERROR,
          body: { message: "Failed to create RoomPlayer" },
        };
      }

      return {
        statusCode: HttpStatusCode.CREATED,
        body: this.mapRoomPlayer(createdRoomPlayer),
      };
    } catch (err) {
      console.error(err);

      return this.handleError();
    }
  }

  async leaveRoom(
    player_id: number
  ): Promise<ApiResponse<{ message: string } | RoomPlayer>> {
    try {
      const searchedRoomPlayer =
        await this.roomPlayerRepository.getRoomPlayerByPlayerId(player_id);

      if (!searchedRoomPlayer) {
        return {
          statusCode: HttpStatusCode.NOT_FOUND,
          body: {
            message: `Room player with player_id: ${player_id} is not in the database.`,
          },
        };
      }

      const deletedRoomPlayer =
        await this.roomPlayerRepository.deleteRoomPlayerByPlayerId(player_id);

      if (!deletedRoomPlayer) {
        return this.handleError();
      }

      // Deleting the player corresponding to the relation
      const deletedPlayer = await this.playerService.deletePlayerById(
        deletedRoomPlayer.player.player_key
      );

      if (!(deletedPlayer.body instanceof Player)) {
        const { statusCode, body } = deletedPlayer;

        return {
          statusCode,
          body,
        };
      }

      return {
        statusCode: HttpStatusCode.OK,
        body: new RoomPlayer(deletedRoomPlayer),
      };
    } catch (err) {
      console.error(err);

      return this.handleError();
    }
  }
}

export default RoomPlayerService;
