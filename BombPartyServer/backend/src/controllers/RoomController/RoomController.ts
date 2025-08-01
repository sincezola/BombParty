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
          .json({ message: `invalid status received.` });

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
          .json({ message: `invalid id received.` });

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
      if (
        !isValidRoomLevel(req.body.room_level as never) ||
        !isValidRoomCapacity(req.body.room_capacity as never)
      ) {
        res.status(HttpStatusCode.BAD_REQUEST).json({
          message: `room_capacity or room_level invalid.`,
        });

        return;
      }

      if (
        typeof req.body.room_name != "string" ||
        typeof req.body.player_name != "string"
      ) {
        res.status(HttpStatusCode.BAD_REQUEST).json({
          message: `Room_name or Player_name are invalid data.`,
        });

        return;
      }
      // Fields validated !
      const { player_name, room_name, room_capacity, room_level } = req.body;

      const receivedAssociation =
        await this.roomService.createRoomPlayerAndAssociate(
          player_name,
          room_name,
          room_capacity,
          room_level,
          req.body.room_password ? req.body.room_password : null
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
      if (
        typeof req.body.room_level != "number" &&
        typeof req.body.room_capacity != "number"
      ) {
        res
          .status(HttpStatusCode.BAD_REQUEST)
          .json({ message: "Missing properties room_level & room_capacity." });

        return;
      }

      if (
        (typeof req.body.room_level != "number" &&
          !isValidRoomLevel(req.body.room_level as never)) ||
        (typeof req.body.room_capacity != "number" &&
          !isValidRoomCapacity(req.body.room_capacity as never))
      ) {
        res
          .status(HttpStatusCode.BAD_REQUEST)
          .json({ message: "Request fields are not valid values." });

        return;
      }

      const patchedRoom = await this.roomService.changeRoomProperties(
        req.body.room_key,
        req.body.room_level,
        req.body.room_capacity
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
