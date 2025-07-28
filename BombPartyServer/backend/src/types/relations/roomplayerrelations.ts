import { Prisma } from "@prisma/client";

const roomPlayerInclude = {
  room: true,
  player: true,
  type: true,
} satisfies Prisma.RoomPlayerInclude;

type RoomPlayerWithRelations = Prisma.RoomPlayerGetPayload<{
  include: typeof roomPlayerInclude;
}>;

export { RoomPlayerWithRelations, roomPlayerInclude };
