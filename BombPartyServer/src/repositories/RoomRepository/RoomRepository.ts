import RoomRepositoryProtocol from "./RoomRepositoryProtocol.ts";
import { Room } from "../../entities/Room.ts";
import { PrismaClient } from "@prisma/client";

class RoomRepository implements RoomRepositoryProtocol {
  constructor(private readonly prisma: PrismaClient) {}

  async getAllRooms(): Promise<Room[] | null> {
    try {
      const receivedRooms = await this.prisma.room.findMany({
        include: {
          status: true,
          level: true,
          players: {
            include: { player: true, type: true },
          },
        },
      });

      if (!receivedRooms) return null;

      const retArr: Room[] = receivedRooms.map((room) => {
        return new Room(room);
      });

      return retArr;
    } catch (err) {
      console.error(err);

      return null;
    }
  }

  async getRoomById(room_key: number): Promise<Room | null> {
    try {
      const receivedRoom = await this.prisma.room.findFirst({
        where: { room_key },
        include: {
          status: true,
          level: true,
          players: {
            include: { player: true, type: true },
          },
        },
      });

      if (!receivedRoom) return null;

      return new Room(receivedRoom);
    } catch (err) {
      console.error(err);

      return null;
    }
  }
}

export default RoomRepository;
