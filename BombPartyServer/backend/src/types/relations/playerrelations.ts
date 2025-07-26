import { Prisma } from "@prisma/client";

const playerInclude = {
  roomLinks: true,
} satisfies Prisma.PlayerInclude;

type PlayerWithRelations = Prisma.PlayerGetPayload<{
  include: typeof playerInclude;
}>;

export { PlayerWithRelations, playerInclude }