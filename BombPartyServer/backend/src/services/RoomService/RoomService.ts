import RoomServiceProtocol from "./RoomServiceProtocol.ts";
import ApiResponse from "../../types/response/ApiResponse.ts";
import HttpStatusCode from "../../types/enums/HttpStatusCode.ts";
import RoomRepository from "../../repositories/RoomRepository/RoomRepository.ts";
import { Room } from "../../entities/Room.ts";
import { RoomWithRelations } from "../../types/relations/roomrelations.ts";

class RoomService implements RoomServiceProtocol {
  constructor(private roomRepository: RoomRepository) {}

  private handleError() {
    return {
      statusCode: HttpStatusCode.INTERNAL_SERVER_ERROR,
      body: { message: "Sorry, Internal Server Error" },
    };
  }

  private mapRooms(rawRooms: RoomWithRelations[]): Room[] {
    return rawRooms.map((raw) => new Room(raw));
  }

  private mapRoom(rawRoom: RoomWithRelations): Room {
    return new Room(rawRoom);
  }

  async getAllRooms(): Promise<ApiResponse<{ message: string } | Room[]>> {
    try {
      const receivedRooms = await this.roomRepository.getAllRooms();

      if (!receivedRooms) {
        return this.handleError();
      }
      
      const rooms = this.mapRooms(receivedRooms);
      return {
        statusCode: HttpStatusCode.OK,
        body: rooms,
      };
    } catch (err) {
      console.error(err);
      return this.handleError();
    }
  }

  async getRoomsByStatus(
    room_status: number
  ): Promise<ApiResponse<{ message: string } | Room[]>> {
    try {
      const receivedRooms = await this.roomRepository.getRoomsByStatus(
        room_status
      );

      if (!receivedRooms) {
        return this.handleError();
      }

      const rooms = this.mapRooms(receivedRooms);

      return {
        statusCode: HttpStatusCode.OK,
        body: rooms,
      };
    } catch (err) {
      console.error(err);
      return this.handleError();
    }
  }

  async getRoomById(
    room_key: number
  ): Promise<ApiResponse<{ message: string } | Room>> {
    try {
      const receivedRoom = await this.roomRepository.getRoomById(room_key);

      if (!receivedRoom) {
        return {
          statusCode: HttpStatusCode.NOT_FOUND,
          body: { message: `Room with id: ${room_key} doesn't exist.` },
        };
      }
      
      return {
        statusCode: HttpStatusCode.OK,
        body: this.mapRoom(receivedRoom),
      };
    } catch (err) {
      console.error(err);
      return this.handleError();
    }
  }
}

export default RoomService;
