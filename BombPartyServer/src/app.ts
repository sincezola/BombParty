import express from "express";
import RoomRepository from "./repositories/RoomRepository/RoomRepository.ts";
import RoomService from "./services/RoomService/RoomService.ts";
import RoomController from "./controllers/RoomController/RoomController.ts";
import prisma from "./prisma/prisma.ts";

const app = express();

app.use(express.json());

const router = express.Router();

new RoomController(router, new RoomService(new RoomRepository(prisma)));

app.use(router);

export default app;
