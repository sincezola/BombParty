import PlayerRepository from "../../repositories/PlayerRepository/PlayerRepository.ts";
import ApiResponse from "../../types/response/ApiResponse.ts";
import PlayerServiceProtocol from "./PlayerServiceProtocol.ts";
import HttpStatusCode from "../../types/enums/HttpStatusCode.ts";
import { Player } from "../../entities/Player.ts";

class PlayerService implements PlayerServiceProtocol {
  constructor(private playerRepository: PlayerRepository) {}

  private handleError() {
    return {
      statusCode: HttpStatusCode.INTERNAL_SERVER_ERROR,
      body: { message: "Sorry, Internal Server Error" },
    };
  }

  async getPlayerById(player_key: number): Promise<ApiResponse<{ message: string; } | Player>> {
    try {
      const receivedPlayer = await this.playerRepository.getPlayerById(player_key);

      if (!receivedPlayer) {
        return {
          statusCode: HttpStatusCode.NOT_FOUND,
          body: { message: `Player with id: ${player_key} is not in the database.` },
        }
      }

      return {
        statusCode: HttpStatusCode.OK,
        body: new Player(receivedPlayer),
      }
    } catch (err) {
      console.error(err);

      return this.handleError();
    }
  }

  async createPlayer(
    player_name: string
  ): Promise<ApiResponse<{ message: string } | Player>> {
    try {
      const receivedPlayer = await this.playerRepository.createPlayer(
        player_name
      );

      if (!receivedPlayer) {
        return this.handleError();
      }

      return {
        statusCode: HttpStatusCode.CREATED,
        body: new Player(receivedPlayer),
      };
    } catch (err) {
      console.error(err);

      return this.handleError();
    }
  }

  async deletePlayerById(
    player_key: number
  ): Promise<ApiResponse<{ message: string } | Player>> {
    try {
      const receivedPlayer = await this.playerRepository.getPlayerById(
        player_key
      );

      if (!receivedPlayer) {
        return {
          statusCode: HttpStatusCode.NOT_FOUND,
          body: {
            message: `Player with id: ${player_key} is not in the database.`,
          },
        };
      }

      const deletedPlayer = await this.playerRepository.deletePlayerById(
        player_key
      );

      if (!deletedPlayer) {
        return this.handleError();
      }

      return {
        statusCode: HttpStatusCode.OK,
        body: new Player(deletedPlayer),
      };
    } catch (err) {
      console.error(err);

      return this.handleError();
    }
  }
}

export default PlayerService;
