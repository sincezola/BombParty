import PlayerRepositoryProtocol from "./PlayerRepositoryProtocol.ts";
import { PrismaClient } from "@prisma/client";
import { playerInclude, PlayerWithRelations } from "../../types/relations/playerrelations.ts";

class PlayerRepository implements PlayerRepositoryProtocol {
  constructor(private readonly prisma: PrismaClient) {}

  async getPlayerById(player_key: number): Promise<PlayerWithRelations | null> {
    try {
      return await this.prisma.player.findUnique({
        where: { player_key }, include: playerInclude
      });
    } catch (err) {
      console.error(err);

      return null;
    }
  }

  async createPlayer(player_name: string): Promise<PlayerWithRelations | null> {
    try {
      return await this.prisma.player.create({
        data: { player_name }, include: playerInclude
      });
    } catch (err) {
      console.error(err);

      return null;
    }
  }

  async deletePlayerById(player_key: number): Promise<PlayerWithRelations | null> {
    try {
      return await this.prisma.player.delete({
        where: { player_key }, include: playerInclude 
      });
    } catch (err) {
      console.error(err);

      return null;
    }
  }
}

export default PlayerRepository;
