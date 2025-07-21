import { Request, Response } from "express";

export default abstract class {
  protected abstract createPlayer(req: Request, res: Response): Promise<void>;
}
