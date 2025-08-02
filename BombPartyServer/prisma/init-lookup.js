import { PrismaClient } from "@prisma/client";
const prisma = new PrismaClient();

async function main() {
  await prisma.$transaction([
    prisma.$executeRaw`INSERT INTO status_type (status_type_id, code) VALUES (1, 'created') ON CONFLICT (status_type_id) DO NOTHING`,
    prisma.$executeRaw`INSERT INTO status_type (status_type_id, code) VALUES (2, 'active') ON CONFLICT (status_type_id) DO NOTHING`,
    prisma.$executeRaw`INSERT INTO status_type (status_type_id, code) VALUES (3, 'closed') ON CONFLICT (status_type_id) DO NOTHING`,

    prisma.$executeRaw`INSERT INTO level_type (level_type_id, code) VALUES (1, 'easy') ON CONFLICT (level_type_id) DO NOTHING`,
    prisma.$executeRaw`INSERT INTO level_type (level_type_id, code) VALUES (2, 'medium') ON CONFLICT (level_type_id) DO NOTHING`,
    prisma.$executeRaw`INSERT INTO level_type (level_type_id, code) VALUES (3, 'hard') ON CONFLICT (level_type_id) DO NOTHING`,

    prisma.$executeRaw`INSERT INTO room_player_type (room_player_type_id, code) VALUES (1, 'owner') ON CONFLICT (room_player_type_id) DO NOTHING`,
    prisma.$executeRaw`INSERT INTO room_player_type (room_player_type_id, code) VALUES (2, 'participant') ON CONFLICT (room_player_type_id) DO NOTHING`,
    prisma.$executeRaw`INSERT INTO room_player_type (room_player_type_id, code) VALUES (3, 'spectator') ON CONFLICT (room_player_type_id) DO NOTHING`,
  ]);

  console.log("Lookup tables initialized!");
}

main()
  .catch((e) => console.error(e))
  .finally(() => prisma.$disconnect());
