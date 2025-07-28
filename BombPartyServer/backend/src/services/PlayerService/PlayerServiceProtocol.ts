import ApiResponse from "../../types/response/ApiResponse.ts";
import { Player } from "../../entities/Player.ts";

export default abstract class PlayerServiceProtocol {
  abstract getPlayerById(
    player_key: number,
  ): Promise<ApiResponse<{ message: string } | Player>>;
  abstract createPlayer(
    player_name: string,
  ): Promise<ApiResponse<{ message: string } | Player>>;
  abstract deletePlayerById(
    player_key: number,
  ): Promise<ApiResponse<{ message: string } | Player>>;
}
