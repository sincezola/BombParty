import ApiResponse from "../../types/response/ApiResponse.ts";
import { Room } from "../../entities/Room.ts";

export default abstract class RoomServiceProtocol {
  abstract getAllRooms(): Promise<ApiResponse<{ message: string } | Room[]>>;
  abstract getRoomsByStatus(
    room_status: number
  ): Promise<ApiResponse<{ message: string } | Room[]>>;
  abstract getRoomById(
    room_key: number
  ): Promise<ApiResponse<{ message: string } | Room>>;
}
