import PlayerRepositoryProtocol from "./PlayerRepositoryProtocol.ts";
import { Player } from "../../entities/Player.ts";
import { PrismaClient } from "@prisma/client";

class PlayerRepository implements PlayerRepositoryProtocol {
  constructor(private readonly prisma: PrismaClient) {}

  async createPlayer(player_name: string): Promise<Player | null> {
    try {
      const receivedPlayer = await this.prisma.player.create({
        data: { player_name },
      });

      if (!receivedPlayer) return null;

      return new Player(receivedPlayer);
    } catch (err) {
      console.error(err);

      return null;
    }
  }
}

export default PlayerRepository;
