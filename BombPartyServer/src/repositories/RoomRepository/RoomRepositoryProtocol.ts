import { Room } from "../../entities/Room.ts";

export default abstract class RoomRepositoryProtocol {
  abstract getAllRooms(): Promise<Room[] | null>;
  abstract getRoomById(room_key: number): Promise<Room | null>;
}
