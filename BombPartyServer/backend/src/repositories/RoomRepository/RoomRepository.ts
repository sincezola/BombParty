import RoomRepositoryProtocol from "./RoomRepositoryProtocol.ts";
import PatchData from "../../types/patch_data.ts";
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

  async getRoomsByStatus(
    room_status: number
  ): Promise<RoomWithRelations[] | null> {
    try {
      return await this.prisma.room.findMany({
        where: { room_status },
        include: roomInclude,
      });
    } catch (err) {
      console.error(err);

      return null;
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

  async createRoom(
    room_name: string,
    room_capacity: number,
    room_level: number,
    room_password?: string
  ): Promise<RoomWithRelations | null> {
    try {
      return await this.prisma.room.create({
        data: {
          room_name,
          room_capacity,
          room_level,
          room_status: 1,
          room_password,
        },
        include: roomInclude,
      });
    } catch (err) {
      console.error(err);

      return null;
    }
  }

  async patchRoom(
    room_key: number,
    room_level?: number,
    room_capacity?: number
  ): Promise<RoomWithRelations | null> {
    try {
      const data: PatchData = {};

      if (room_level !== undefined) data.room_level = room_level;
      if (room_capacity !== undefined) data.room_capacity = room_capacity;

      return await this.prisma.room.update({
        where: { room_key },
        data,
        include: roomInclude,
      });
    } catch (err) {
      console.error(err);

      return null;
    }
  }

  async deleteRoomById(room_key: number): Promise<RoomWithRelations | null> {
    try {
      return await this.prisma.room.delete({
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
