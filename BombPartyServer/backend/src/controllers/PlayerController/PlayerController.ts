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
    this.router.post(
      "/api/Player/CreatePlayer/:name",
      this.createPlayer.bind(this)
    );
    this.router.delete(
      "/api/Player/DeletePlayer/:id",
      this.deletePlayer.bind(this)
    );
  }

  protected async getPlayerById(req: Request, res: Response): Promise<void> {
    try {
      const { id } = req.params;

      if (!isValidInteger(id)) {
        res
          .status(HttpStatusCode.BAD_REQUEST)
          .json({ message: `Id: ${id} is not a valid id.` });
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

  protected async createPlayer(req: Request, res: Response): Promise<void> {
    try {
      const { name } = req.params;

      const receivedPlayer = await this.playerService.createPlayer(name);

      const { statusCode, body } = receivedPlayer;

      res.status(statusCode).json(body);
    } catch (err) {
      console.error(err);

      res
        .status(HttpStatusCode.INTERNAL_SERVER_ERROR)
        .json({ message: "Sorry, Internal Server Error." });
    }
  }

  protected async deletePlayer(req: Request, res: Response): Promise<void> {
    try {
      const { id } = req.params;

      if (!isValidInteger(id)) {
        res
          .status(HttpStatusCode.BAD_REQUEST)
          .json({ message: `Id: ${id} is not a valid id.` });
      }

      const deletedPlayer = await this.playerService.deletePlayerById(Number(id));

      const { statusCode, body } = deletedPlayer;

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
