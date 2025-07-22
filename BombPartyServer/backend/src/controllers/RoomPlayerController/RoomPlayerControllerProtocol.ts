import { Request, Response } from "express";

export default abstract class RoomPlayerControllerProtocol {
  protected abstract createRoomPlayer(req: Request, res: Response): Promise<void>;
}