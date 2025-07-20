import { Player } from "@prisma/client";

export default abstract class PlayerRepositoryProtocol {
  abstract createPlayer(player_name: string): Promise<Player | null>;
}
