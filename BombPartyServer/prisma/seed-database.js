import { PrismaClient } from "@prisma/client";

const prisma = new PrismaClient();

async function main() {
  // ======================
  // Criar alguns jogadores fixos
  // ======================
  const playerNames = [
    "Alice", "Bob", "Carol", "Dave", "Eve",
    "Frank", "Grace", "Heidi", "Ivan", "Judy",
    "Mallory", "Oscar", "Peggy", "Trent", "Victor"
  ];

  let ii = 0;

  const players = await Promise.all(
    playerNames.map((name) =>
      prisma.player.upsert({
        where: { player_key: ++ii },
        update: {},
        create: { player_name: name },
      })

    )
  );

  // ======================
  // Helpers
  // ======================
  const pickPlayers = (count) => {
    const shuffled = [...players].sort(() => 0.5 - Math.random());
    return shuffled.slice(0, count);
  };

  const roomNames = [
    "Sala dos Iniciantes",
    "Desafio Médio",
    "Sala Hardcore",
    "Noobs Only",
    "Campeonato",
    "Sala Secreta",
    "Partida Rápida",
    "Treinamento",
    "Caos Total",
    "Batalha Final",
  ];

  // valores "chumbados" (sem lookup)
  const STATUS = { AVAILABLE: 1, IN_GAME: 2, CLOSED: 3 };
  const LEVEL = { EASY: 1, MEDIUM: 2, HARD: 3 };
  const ROLE = { OWNER: 1, PLAYER: 2, SPECTATOR: 3 };

  const passwords = [null, "1234", "senha", "abcd", null];

  // ======================
  // Criar 10 salas
  // ======================
  for (let i = 0; i < 10; i++) {
    const roomPlayers = pickPlayers(Math.floor(Math.random() * 3) + 2); // 2–4 jogadores

    const room = await prisma.room.create({
      data: {
        room_name: roomNames[i],
        room_password:
          passwords[Math.floor(Math.random() * passwords.length)],
        room_capacity: Math.floor(Math.random() * 5) + 2, // 2–6
        room_status:
          Object.values(STATUS)[
            Math.floor(Math.random() * Object.values(STATUS).length)
          ],
        room_level:
          Object.values(LEVEL)[
            Math.floor(Math.random() * Object.values(LEVEL).length)
          ],
        players: {
          create: roomPlayers.map((idx) => ({
            player_id: ii,
            room_player_type:
              idx === 0
                ? ROLE.OWNER // sempre o primeiro como dono
                : Math.random() > 0.5
                ? ROLE.PLAYER
                : ROLE.SPECTATOR,
          })),
        },
      },
    });

    console.log(`Sala criada: ${room.room_name}`);
  }

  console.log("Seed concluído com sucesso!");
}

main()
  .catch((e) => {
    console.error(e);
    process.exit(1);
  })
  .finally(async () => {
    await prisma.$disconnect();
  });
