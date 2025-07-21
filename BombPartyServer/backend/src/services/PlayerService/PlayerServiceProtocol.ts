import { Player } from "../../entities/Player.ts";
import ApiResponse from "../../types/response/ApiResponse.ts";

export default abstract class PlayerServiceProtocol {
  abstract createPlayer(
    player_name: string
  ): Promise<ApiResponse<{ message: string } | Player>>;
}
