import PlayerControllerProtocol from "./PlayerControllerProtocol.ts";
import HttpStatusCode from "../../types/enums/HttpStatusCode.ts";
import PlayerService from "../../services/PlayerService/PlayerService.ts";
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
    this.router.post(
      "/api/Player/CreatePlayer/:name",
      this.createPlayer.bind(this)
    );
  }

  protected async createPlayer(req: Request, res: Response): Promise<void> {
    try {
      const { name } = req.params;

      const receivedPlayer = await this.playerService.createPlayer(name);

      const { statusCode, body } = receivedPlayer;

      res.status(statusCode).json(body);
    } catch (err) {
      console.error(err);

      res.send(HttpStatusCode.INTERNAL_SERVER_ERROR);
    }
  }
}

export default PlayerController;
