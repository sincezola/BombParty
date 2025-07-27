import { Request, Response } from "express";

export default abstract class RoomControllerProtocol {
  protected abstract getAllRooms(_req: Request, res: Response): Promise<void>;
  protected abstract getRoomById(req: Request, res: Response): Promise<void>;
  protected abstract getRoomsByStatus(
    req: Request,
    res: Response
  ): Promise<void>;
  protected abstract createRoomPlayerAndAssociate(req: Request, res: Response): Promise<void>;
}
