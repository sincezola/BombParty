import RoomRepositoryProtocol from "./RoomRepositoryProtocol.ts";
import {
  roomInclude,
  RoomWithRelations,
} from "../../types/relations/roomrelations.ts";
import { PrismaClient } from "@prisma/client";

class RoomRepository implements RoomRepositoryProtocol {
  constructor(private readonly prisma: PrismaClient) {}

  async getAllRooms(): Promise<RoomWithRelations[] | null> {
    try {
      return await this.prisma.room.findMany({
        include: roomInclude,
      });
    } catch (err) {
      console.error(err);

      return null;
    }
  }

  async getRoomsByStatus(room_status: number): Promise<RoomWithRelations[] | null> {
    try {
      return await this.prisma.room.findMany({
        where: { room_status },
        include: roomInclude,
      });
    } catch (err) {
      console.error(err);

      return null
    }
  }

  async getRoomById(room_key: number): Promise<RoomWithRelations | null> {
    try {
      return await this.prisma.room.findUnique({
        where: { room_key },
        include: roomInclude,
      });
    } catch (err) {
      console.error(err);

      return null;
    }
  }
}

export default RoomRepository;