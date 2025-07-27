import { RoomWithRelations } from "../../types/relations/roomrelations.ts";

export default abstract class RoomRepositoryProtocol {
  abstract getAllRooms(): Promise<RoomWithRelations[] | null>;
  abstract getRoomsByStatus(
    room_status: number
  ): Promise<RoomWithRelations[] | null>;
  abstract getRoomById(room_key: number): Promise<RoomWithRelations | null>;
  abstract createRoom(
    room_name: string,
    room_capacity: number,
    room_level: number,
    room_password?: string
  ): Promise<RoomWithRelations | null>;
}
