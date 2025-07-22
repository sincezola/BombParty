import RoomPlayerRepositoryProtocol from "./RoomPlayerRepositoryProtocol.ts";
import { PrismaClient } from "@prisma/client";
import { RoomPlayer } from "../../entities/RoomPlayer.ts";

class RoomPlayerRepository implements RoomPlayerRepositoryProtocol {
  constructor(private readonly prisma: PrismaClient) {}

  async createPlayerRoom(
    room_id: number,
    player_id: number,
    room_player_type: number
  ): Promise<RoomPlayer | null> {
    try {
      const createdPlayerRoom = await this.prisma.roomPlayer.create({
        data: {
          room_id,
          player_id,
          room_player_type,
        },
        include: {
          room: true,
          player: true,
          type: true,
        },
      });

      if (!createdPlayerRoom) return null;

      return new RoomPlayer(createdPlayerRoom);
    } catch (err) {
      console.error(err);

      return null;
    }
  }
}

export default RoomPlayerRepository;
