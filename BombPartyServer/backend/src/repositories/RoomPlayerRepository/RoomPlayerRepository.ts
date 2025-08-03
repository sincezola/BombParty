import RoomPlayerRepositoryProtocol from "./RoomPlayerRepositoryProtocol.ts";
import { PrismaClient } from "@prisma/client";
import {
  roomPlayerInclude,
  RoomPlayerWithRelations,
} from "../../types/relations/roomplayerrelations.ts";

class RoomPlayerRepository implements RoomPlayerRepositoryProtocol {
  constructor(private readonly prisma: PrismaClient) {}

  async getRoomPlayer(
    room_player_id: number
  ): Promise<RoomPlayerWithRelations | null> {
    try {
      return await this.prisma.roomPlayer.findUnique({
        where: { room_player_id },
        include: roomPlayerInclude,
      });
    } catch (err) {
      console.error(err);

      return null;
    }
  }

  async getRoomPlayerByPlayerId(
    player_id: number
  ): Promise<RoomPlayerWithRelations | null> {
    try {
      return await this.prisma.roomPlayer.findUnique({
        where: { player_id },
        include: roomPlayerInclude,
      });
    } catch (err) {
      console.error(err);

      return null;
    }
  }

  async createRoomPlayer(
    room_id: number,
    player_id: number,
    room_player_type: number
  ): Promise<RoomPlayerWithRelations | null> {
    try {
      return await this.prisma.roomPlayer.create({
        data: {
          room_id,
          player_id,
          room_player_type,
        },
        include: roomPlayerInclude,
      });
    } catch (err) {
      console.error(err);

      return null;
    }
  }

  async deleteRoomPlayerByPlayerId(
    player_id: number
  ): Promise<RoomPlayerWithRelations | null> {
    try {
      return await this.prisma.roomPlayer.delete({
        where: { player_id },
        include: {
          room: true,
          player: true,
          type: true,
        },
      });
    } catch (err) {
      console.error(err);

      return null;
    }
  }
}

export default RoomPlayerRepository;
