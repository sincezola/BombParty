import { Request, Response } from "express";

export default abstract class {
  protected abstract getPlayerById(req: Request, res: Response): Promise<void>;
}
