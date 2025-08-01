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
      "/api/RoomPlayer/Create/",
      this.createRoomPlayer.bind(this)
    );
    this.router.delete(
      "/api/RoomPlayer/Delete/:id",
      this.deleteRoomPlayer.bind(this)
    );
    this.router.post("/api/RoomPlayer/JoinRoom/", this.joinRoom.bind(this));
  }

  protected async deleteRoomPlayer(req: Request, res: Response): Promise<void> {
    try {
      const { id } = req.params;

      if (!isValidInteger(id)) {
        res
          .status(HttpStatusCode.BAD_REQUEST)
          .json({ message: `invalid id received.` });

        return;
      }

      const receivedRoomPlayer = await this.roomPlayerService.deleteRoomPlayer(
        Number(id)
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

  protected async createRoomPlayer(req: Request, res: Response): Promise<void> {
    try {
      if (
        !isValidInteger(req.body.room_id) ||
        !isValidInteger(req.body.player_id) ||
        !isValidInteger(req.body.room_player_type)
      ) {
        res.status(HttpStatusCode.BAD_REQUEST).json({
          message:
            "room_id, player_id, and room_player_type are invalid.",
        });
        
        return;
      }

      const createdRoomPlayer = await this.roomPlayerService.createRoomPlayer(
        Number(req.body.room_id),
        Number(req.body.player_id),
        Number(req.body.room_player_type)
      );

      const { statusCode, body } = createdRoomPlayer;

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
      if (!isValidInteger(req.body.room_key)) {
        res.status(HttpStatusCode.BAD_REQUEST).json({
          message: `room_id is invalid.`,
        });

        return;
      }

      if (!(typeof req.body.player_name == "string")) {
        res
          .status(HttpStatusCode.BAD_REQUEST)
          .json({ message: "player_name is invalid." });

        return;
      }

      const receivedRelation = await this.roomService.joinRoom(
        req.body.player_name,
        req.body.room_key
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
