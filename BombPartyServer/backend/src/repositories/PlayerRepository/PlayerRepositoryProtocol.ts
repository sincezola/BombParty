import { PlayerWithRelations } from "../../types/relations/playerrelations.ts";

export default abstract class PlayerRepositoryProtocol {
  abstract getPlayerById(
    player_key: number,
  ): Promise<PlayerWithRelations | null>;
  abstract createPlayer(
    player_name: string,
  ): Promise<PlayerWithRelations | null>;
  abstract deletePlayerById(
    player_key: number,
  ): Promise<PlayerWithRelations | null>;
}
