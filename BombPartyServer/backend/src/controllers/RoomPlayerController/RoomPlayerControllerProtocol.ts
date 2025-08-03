import { Request, Response } from "express";

export default abstract class RoomPlayerControllerProtocol {
  protected abstract leaveRoom(req: Request, res: Response): Promise<void>;
  protected abstract joinRoom(req: Request, res: Response): Promise<void>;
}
