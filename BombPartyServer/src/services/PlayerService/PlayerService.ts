import PlayerRepository from "../../repositories/PlayerRepository/PlayerRepository.ts";
import ApiResponse from "../../types/response/ApiResponse.ts";
import PlayerServiceProtocol from "./PlayerServiceProtocol.ts";
import HttpStatusCode from "../../types/enums/HttpStatusCode.ts";
import { Player } from "../../entities/Player.ts";

class PlayerService implements PlayerServiceProtocol {
  constructor(private playerRepository: PlayerRepository) {}

  async createPlayer(
    player_name: string
  ): Promise<ApiResponse<{ message: string } | Player>> {
    try {
      const receivedPlayer = await this.playerRepository.createPlayer(
        player_name
      );

      if (!receivedPlayer) {
        return {
          statusCode: HttpStatusCode.INTERNAL_SERVER_ERROR,
          body: { message: "Sorry, Internal Server Error" },
        };
      }

      return {
        statusCode: HttpStatusCode.CREATED,
        body: receivedPlayer,
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

export default PlayerService;
