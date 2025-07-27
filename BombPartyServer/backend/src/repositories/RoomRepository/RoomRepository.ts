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

  async createRoom(room_name: string, room_capacity: number, room_level: number, room_password?: string): Promise<RoomWithRelations | null> {
    try {
      return await this.prisma.room.create({
        data: {
          room_name,
          room_capacity,
          room_level,
          room_status: 1,
          room_password,
        }, include: roomInclude
      })
    } catch (err) {
      console.error(err);

      return null;
    }
  }
}

export default RoomRepository;