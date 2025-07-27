import RoomServiceProtocol from "./RoomServiceProtocol.ts";
import ApiResponse from "../../types/response/ApiResponse.ts";
import HttpStatusCode from "../../types/enums/HttpStatusCode.ts";
import RoomRepository from "../../repositories/RoomRepository/RoomRepository.ts";
import PlayerService from "../PlayerService/PlayerService.ts";
import RoomPlayerService from "../RoomPlayerService/RoomPlayerService.ts";
import { Room } from "../../entities/Room.ts";
import { RoomWithRelations } from "../../types/relations/roomrelations.ts";
import { Player } from "../../entities/Player.ts";
import { RoomPlayer } from "../../entities/RoomPlayer.ts";

class RoomService implements RoomServiceProtocol {
  constructor(
    private roomRepository: RoomRepository,
    private playerService: PlayerService,
    private roomPlayerService: RoomPlayerService
  ) {}

  private handleError() {
    return {
      statusCode: HttpStatusCode.INTERNAL_SERVER_ERROR,
      body: { message: "Sorry, Internal Server Error" },
    };
  }

  private mapRooms(rawRooms: RoomWithRelations[]): Room[] {
    return rawRooms.map((raw) => new Room(raw));
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
        body: new Room(receivedRoom),
      };
    } catch (err) {
      console.error(err);

      return this.handleError();
    }
  }

  async createRoomPlayerAndAssociate(
    player_name: string,
    room_name: string,
    room_capacity: number,
    room_level: number,
    room_password?: string
  ): Promise<ApiResponse<{ message: string } | Room>> {
    try {
      const receivedPlayer = await this.playerService.createPlayer(player_name); // Create the player to enter the room

      if (!(receivedPlayer.body instanceof Player)) return this.handleError();

      const beforeRelationRoom = await this.roomRepository.createRoom(
        room_name,
        room_capacity,
        room_level,
        room_password ? room_password : undefined
      );

      if (!beforeRelationRoom) return this.handleError();

      const receivedRelation = await this.roomPlayerService.createRoomPlayer(
        beforeRelationRoom.room_key,
        receivedPlayer.body.player_key,
        1
      );

      if (!receivedRelation) return this.handleError();

      const associatedRoom = await this.getRoomById(beforeRelationRoom.room_key);

      return {
        statusCode: HttpStatusCode.CREATED,
        body: associatedRoom.body,
      };
    } catch (err) {
      console.error(err);

      return this.handleError();
    }
  }

  async joinRoom(
    player_name: string,
    room_key: number
  ): Promise<ApiResponse<{ message: string } | RoomPlayer>> {
    try {
      const receivedRoom = await this.getRoomById(room_key);

      if (!(receivedRoom.body instanceof Room)) {
        return {
          statusCode: HttpStatusCode.NOT_FOUND,
          body: { message: `Room with id: ${room_key} is not in the database` },
        };
      }

      const receivedPlayer = await this.playerService.createPlayer(player_name); // Create the player to enter the room

      if (!(receivedPlayer.body instanceof Player)) return this.handleError();

      const receivedAssociation = await this.roomPlayerService.createRoomPlayer(
        room_key,
        receivedPlayer.body.player_key,
        2
      );

      if (!(receivedAssociation.body instanceof RoomPlayer))
        return this.handleError();

      return {
        statusCode: HttpStatusCode.CREATED,
        body: receivedAssociation.body,
      };
    } catch (err) {
      console.error(err);

      return this.handleError();
    }
  }
}

export default RoomService;
