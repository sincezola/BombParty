import RoomService from "../../services/RoomService/RoomService.ts";
import HttpStatusCode from "../../types/enums/HttpStatusCode.ts";
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

      const receivedRoom = await this.roomService.getRoomById(Number(id));

      res.status(receivedRoom.statusCode).json(receivedRoom.body);
    } catch (err) {
      console.error(err);

      res
        .status(HttpStatusCode.INTERNAL_SERVER_ERROR)
        .json({ message: "Sorry, Internal Server Error." });
    }
  }
}

export default RoomController;
