import ApiResponse from "../../types/response/ApiResponse.ts";
import { RoomPlayer } from "@prisma/client";
import { Room } from "../../entities/Room.ts";

export default abstract class RoomServiceProtocol {
  abstract getAllRooms(): Promise<ApiResponse<{ message: string } | Room[]>>;
  abstract getRoomsByStatus(
    room_status: number
  ): Promise<ApiResponse<{ message: string } | Room[]>>;
  abstract getRoomById(
    room_key: number
  ): Promise<ApiResponse<{ message: string } | Room>>;
  abstract createRoomPlayerAndAssociate(
    player_name: string,
    room_name: string,
    room_capacity: number,
    room_level: number,
    room_password?: string
  ): Promise<ApiResponse<{ message: string } | Room>>;
  abstract joinRoom(
    player_name: string,
    room_id: number
  ): Promise<ApiResponse<{ message: string } | RoomPlayer>>;
}
