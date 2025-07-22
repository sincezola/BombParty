import RoomPlayerService from "../../services/RoomPlayer/RoomPlayerService.ts";
import RoomPlayerControllerProtocol from "./RoomPlayerControllerProtocol.ts";
import HttpStatusCode from "../../types/enums/HttpStatusCode.ts";
import { Request, Response, Router } from "express";

class RoomPlayerController extends RoomPlayerControllerProtocol {
  constructor(
    private readonly router: Router,
    private readonly roomPlayerService: RoomPlayerService
  ) {
    super();
    this.registerRoutes();
  }

  private registerRoutes() {
    this.router.post(
      "/api/RoomPlayer/Create/",
      this.createRoomPlayer.bind(this)
    );
  }

  protected async createRoomPlayer(req: Request, res: Response): Promise<void> {
    try {

      console.log("ok")

      const { room_id, player_id, room_player_type } = req.body;

      const createdRoomPlayer = await this.roomPlayerService.createRoomPlayer(
        room_id,
        player_id,
        room_player_type
      );

      const { statusCode, body } = createdRoomPlayer;

      res.status(statusCode).json(body);
    } catch (err) {
      console.error(err);

      res
        .status(HttpStatusCode.INTERNAL_SERVER_ERROR)
        .json({ message: "Sorry, Internal Server Error" });
    }
  }
}

export default RoomPlayerController;
