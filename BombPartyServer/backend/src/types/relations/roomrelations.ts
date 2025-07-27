import { Prisma } from "@prisma/client";

const roomInclude = {
  status: true,
  level: true,
  players: {
    include: { player: true },
  },
} satisfies Prisma.RoomInclude;

type RoomWithRelations = Prisma.RoomGetPayload<{
  include: typeof roomInclude;
}>;

export { RoomWithRelations, roomInclude };
