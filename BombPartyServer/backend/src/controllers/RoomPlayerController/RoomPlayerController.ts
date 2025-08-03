import RoomPlayerService from "../../services/RoomPlayerService/RoomPlayerService.ts";
import RoomPlayerControllerProtocol from "./RoomPlayerControllerProtocol.ts";
import HttpStatusCode from "../../types/enums/HttpStatusCode.ts";
import isValidInteger from "../../utils/isIntegerValue.ts";
import RoomService from "../../services/RoomService/RoomService.ts";
import { Request, Response, Router } from "express";

class RoomPlayerController extends RoomPlayerControllerProtocol {
  constructor(
    private readonly router: Router,
    private readonly roomPlayerService: RoomPlayerService,
    private readonly roomService: RoomService
  ) {
    super();
    this.registerRoutes();
  }

  private registerRoutes() {
    this.router.post(
      "/api/RoomPlayer/LeaveRoom/:player_id",
      this.leaveRoom.bind(this)
    );
    this.router.post("/api/RoomPlayer/JoinRoom/", this.joinRoom.bind(this));
  }

  protected async leaveRoom(req: Request, res: Response): Promise<void> {
    try {
      // Delete relation & player by player_id
      const { player_id } = req.params;

      if (!isValidInteger(player_id) || Number(player_id) < 1) {
        res
          .status(HttpStatusCode.BAD_REQUEST)
          .json({ message: `invalid player id provided.` });

        return;
      }

      const receivedRoomPlayer = await this.roomPlayerService.leaveRoom(
        Number(player_id)
      );

      const { statusCode, body } = receivedRoomPlayer;

      res.status(statusCode).json(body);
    } catch (err) {
      console.error(err);
      res.status(HttpStatusCode.INTERNAL_SERVER_ERROR).json({
        message: "Sorry, Internal Server Error",
      });
    }
  }

  protected async joinRoom(req: Request, res: Response): Promise<void> {
    try {
      const { player_name, room_key } = req.body ?? {};

      if (!isValidInteger(room_key) || Number(room_key) < 1) {
        res.status(HttpStatusCode.BAD_REQUEST).json({
          message: `room_key is invalid.`,
        });

        return;
      }

      if (!(typeof player_name == "string")) {
        res
          .status(HttpStatusCode.BAD_REQUEST)
          .json({ message: "player_name is invalid." });

        return;
      }

      const receivedRelation = await this.roomService.joinRoom(
        player_name,
        room_key
      );

      const { statusCode, body } = receivedRelation;

      res.status(statusCode).json(body);
    } catch (err) {
      console.error(err);
      res.status(HttpStatusCode.INTERNAL_SERVER_ERROR).json({
        message: "Sorry, Internal Server Error",
      });
    }
  }
}

export default RoomPlayerController;
