import RoomService from "../../services/RoomService/RoomService.ts";
import HttpStatusCode from "../../types/enums/HttpStatusCode.ts";
import isValidInteger from "../../utils/isIntegerValue.ts";
import isValidRoomCapacity from "../../utils/validations/validate_room_capacity.ts";
import isValidRoomLevel from "../../utils/validations/validate_room_level.ts";
import RoomControllerProtocol from "./RoomControllerProtocol.ts";
import { Request, Response, Router } from "express";

class RoomController extends RoomControllerProtocol {
  constructor(
    private readonly router: Router,
    private readonly roomService: RoomService
  ) {
    super();
    this.registerRoutes();
  }

  private registerRoutes() {
    this.router.get("/api/Room/GetRoomById/:id", this.getRoomById.bind(this));
    this.router.get(
      "/api/Room/GetStatusRooms/:status",
      this.getRoomsByStatus.bind(this)
    );
    this.router.get("/api/Room/GetAllRooms/", this.getAllRooms.bind(this));
    this.router.post(
      "/api/Room/CreateRoom/",
      this.createRoomPlayerAndAssociate.bind(this)
    );
    this.router.patch(
      "/api/Room/PatchRoom/",
      this.changeRoomProperties.bind(this)
    );
  }

  protected async getAllRooms(_req: Request, res: Response): Promise<void> {
    try {
      const receivedRooms = await this.roomService.getAllRooms();

      const { statusCode, body } = receivedRooms;

      res.status(statusCode).json(body);
    } catch (err) {
      console.error(err);

      res
        .status(HttpStatusCode.INTERNAL_SERVER_ERROR)
        .json({ message: "Sorry, Internal Server Error" });
    }
  }

  protected async getRoomsByStatus(req: Request, res: Response): Promise<void> {
    try {
      const { status } = req.params;

      if (!isValidInteger(status) || Number(status) > 3 || Number(status) < 1) {
        res
          .status(HttpStatusCode.BAD_REQUEST)
          .json({ message: `Status ${status} is not a valid status.` });
        return;
      }

      const receivedRooms = await this.roomService.getRoomsByStatus(
        Number(status)
      );

      const { statusCode, body } = receivedRooms;

      res.status(statusCode).json(body);
    } catch (err) {
      console.error(err);

      res
        .status(HttpStatusCode.INTERNAL_SERVER_ERROR)
        .json({ message: "Sorry, Internal Server Error." });
    }
  }

  protected async getRoomById(req: Request, res: Response): Promise<void> {
    try {
      const { id } = req.params;

      if (!isValidInteger(id)) {
        res
          .status(HttpStatusCode.BAD_REQUEST)
          .json({ message: `${id} Is not a valid id.` });
        return;
      }

      const receivedRoom = await this.roomService.getRoomById(Number(id));

      res.status(receivedRoom.statusCode).json(receivedRoom.body);
    } catch (err) {
      console.error(err);

      res
        .status(HttpStatusCode.INTERNAL_SERVER_ERROR)
        .json({ message: "Sorry, Internal Server Error." });
    }
  }

  protected async createRoomPlayerAndAssociate(
    req: Request,
    res: Response
  ): Promise<void> {
    try {
      const {
        player_name,
        room_name,
        room_capacity,
        room_level,
        room_password,
      } = req.body;

      if (
        !isValidRoomLevel(room_level) ||
        !isValidRoomCapacity(room_capacity)
      ) {
        res.status(HttpStatusCode.BAD_REQUEST).json({
          message: `Room capacity: ${room_capacity} or Room level: ${room_level} invalid.`,
        });

        return;
      }
      const receivedAssociation =
        await this.roomService.createRoomPlayerAndAssociate(
          player_name,
          room_name,
          room_capacity,
          room_level,
          room_password ? room_password : null
        );

      const { statusCode, body } = receivedAssociation;

      res.status(statusCode).json(body);
    } catch (err) {
      console.error(err);

      res
        .status(HttpStatusCode.INTERNAL_SERVER_ERROR)
        .json({ message: "Sorry, Internal Server Error." });
    }
  }

  protected async changeRoomProperties(
    req: Request,
    res: Response
  ): Promise<void> {
    try {
      const { room_key, room_level, room_capacity } = req.body;

      console.log(room_level);

      if (room_level == undefined && room_capacity == undefined) {
        res
          .status(HttpStatusCode.BAD_REQUEST)
          .json({ message: "Missing properties room_level & room_capacity." });
      }

      if (
        (room_level != undefined && !isValidRoomLevel(room_level)) ||
        (room_capacity != undefined && !isValidRoomCapacity(room_capacity))
      ) {
        res
          .status(HttpStatusCode.BAD_REQUEST)
          .json({ message: "Request fields are not valid values." });

        return;
      }

      const patchedRoom = await this.roomService.changeRoomProperties(
        room_key,
        room_level,
        room_capacity
      );

      const { statusCode, body } = patchedRoom;

      res.status(statusCode).json(body);
    } catch (err) {
      console.error(err);

      res
        .status(HttpStatusCode.INTERNAL_SERVER_ERROR)
        .json({ message: "Sorry, Internal Server Error." });
    }
  }
}

export default RoomController;
