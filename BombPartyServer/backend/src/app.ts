import express from "express";
import prisma from "./prisma/prisma.ts";
import RoomController from "./controllers/RoomController/RoomController.ts";
import RoomService from "./services/RoomService/RoomService.ts";
import RoomRepository from "./repositories/RoomRepository/RoomRepository.ts";
import PlayerController from "./controllers/PlayerController/PlayerController.ts";
import PlayerService from "./services/PlayerService/PlayerService.ts";
import PlayerRepository from "./repositories/PlayerRepository/PlayerRepository.ts";
import RoomPlayerController from "./controllers/RoomPlayerController/RoomPlayerController.ts";
import RoomPlayerService from "./services/RoomPlayerService/RoomPlayerService.ts";
import RoomPlayerRepository from "./repositories/RoomPlayerRepository/RoomPlayerRepository.ts";

// Index

const app = express();

app.use(express.json());

const router = express.Router();

const playerService = new PlayerService(new PlayerRepository(prisma)); // One instance (singleton)

new RoomController(router, new RoomService(new RoomRepository(prisma)));
new PlayerController(router, playerService);
new RoomPlayerController(
  router,
  new RoomPlayerService(new RoomPlayerRepository(prisma), playerService)
);

app.use(router);

export default app;
