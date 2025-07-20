import RoomServiceProtocol from "./RoomServiceProtocol.ts";
import ApiResponse from "../../types/response/ApiResponse.ts";
import RoomRepository from "../../repositories/RoomRepository/RoomRepository.ts";
import { Room } from "../../entities/Room.ts";

class RoomService implements RoomServiceProtocol {
  constructor(private roomRepository: RoomRepository) {}

  async getAllRooms(): Promise<ApiResponse<{ message: string } | Room[]>> {
    try {
      const receivedRooms = await this.roomRepository.getAllRooms();

      return {
        statusCode: 200,
        body: receivedRooms ? receivedRooms : [],
      };
    } catch (err) {
      console.error(err);

      return {
        statusCode: 500,
        body: { message: "Sorry, Internal Server Error" },
      };
    }
  }

  async getRoomById(
    room_key: number
  ): Promise<ApiResponse<{ message: string } | Room>> {
    try {
      const receivedRoom = await this.roomRepository.getRoomById(room_key);

      if (!receivedRoom) {
        return {
          statusCode: 404,
          body: { message: `Room with id: ${room_key} doesn't exist.` },
        };
      }

      return {
        statusCode: 200,
        body: receivedRoom,
      };
    } catch (err) {
      console.error(err);

      return {
        statusCode: 500,
        body: { message: "Sorry, Internal Server Error." },
      };
    }
  }
}

export default RoomService;
