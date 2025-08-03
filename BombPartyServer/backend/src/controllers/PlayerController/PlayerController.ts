import PlayerControllerProtocol from "./PlayerControllerProtocol.ts";
import HttpStatusCode from "../../types/enums/HttpStatusCode.ts";
import PlayerService from "../../services/PlayerService/PlayerService.ts";
import isValidInteger from "../../utils/isIntegerValue.ts";
import { Request, Response, Router } from "express";

class PlayerController extends PlayerControllerProtocol {
  constructor(
    private readonly router: Router,
    private readonly playerService: PlayerService
  ) {
    super();
    this.registerRoutes();
  }

  private registerRoutes() {
    this.router.get(
      "/api/Player/GetPlayerById/:id",
      this.getPlayerById.bind(this)
    );
  }

  protected async getPlayerById(req: Request, res: Response): Promise<void> {
    try {
      const { id } = req.params;

      if (!isValidInteger(id) || Number(id) < 1) {
        res
          .status(HttpStatusCode.BAD_REQUEST)
          .json({ message: `invalid id provided.` });

        return;
      }

      const receivedPlayer = await this.playerService.getPlayerById(Number(id));

      const { statusCode, body } = receivedPlayer;

      res.status(statusCode).json(body);
    } catch (err) {
      console.error(err);

      res
        .status(HttpStatusCode.INTERNAL_SERVER_ERROR)
        .json({ message: "Sorry, Internal Server Error." });
    }
  }
}

export default PlayerController;
