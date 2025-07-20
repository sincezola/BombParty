import express from "express";
import RoomRepository from "./repositories/RoomRepository/RoomRepository.ts";
import RoomService from "./services/RoomService/RoomService.ts";
import RoomController from "./controllers/RoomController/RoomController.ts";
import prisma from "./prisma/prisma.ts";
import PlayerController from "./controllers/PlayerController/PlayerController.ts";
import PlayerService from "./services/PlayerService/PlayerService.ts";
import PlayerRepository from "./repositories/PlayerRepository/PlayerRepository.ts";

// Index

const app = express();

app.use(express.json());

const router = express.Router();

new RoomController(router, new RoomService(new RoomRepository(prisma)));
new PlayerController(router, new PlayerService(new PlayerRepository(prisma)));

app.use(router);

export default app;
