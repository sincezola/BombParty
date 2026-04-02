# 💣 BombParty

A multiplayer **word-reaction game** where players must type a valid word containing a required substring before the bomb explodes.

This repository contains the full stack of the game, split into three services:

- **BombPartyClient** → terminal game client in **C**
- **BombProcessor** → socket gateway/server in **C**
- **BombPartyServer** → REST backend in **TypeScript + Express + Prisma**

---

## ✨ Overview

BombParty is designed as a layered architecture:

```text
+-------------------+      TCP      +-------------------+      HTTP      +------------------------+
| BombPartyClient   | <-----------> | BombProcessor     | <-----------> | BombPartyServer        |
| Terminal UI (C)   |               | Socket Gateway(C) |               | API + DB (TS/Prisma)   |
+-------------------+               +-------------------+               +------------------------+
```

### What each module does

#### 1) BombPartyClient (C)
- Runs the terminal UI/game flow.
- Lets players create/join rooms and play rounds.
- Supports multiplayer mode and local/singleplayer mode hooks.

#### 2) BombProcessor (C)
- Accepts client TCP connections.
- Parses command protocol messages (`CMD|...`).
- Calls backend API endpoints and returns normalized responses to clients.

#### 3) BombPartyServer (TypeScript)
- Exposes room/player APIs.
- Persists data with Prisma ORM.
- Stores game room/player relations in PostgreSQL.

---

## 🧱 Repository Structure

```text
.
├── BombPartyClient/      # Terminal client (C)
├── BombProcessor/        # Socket processor/gateway (C)
├── BombPartyServer/      # Backend API (TypeScript + Prisma)
├── LICENSE
└── README.md
```

---

## ✅ Prerequisites

## 1) For BombPartyClient and BombProcessor (C)
- GCC or Clang
- Make
- `libcurl`
- `pthread`
- Optional: ImageMagick (if using icon-related build targets)

### 2) For BombPartyServer (TypeScript)
- Node.js **18+**
- npm
- PostgreSQL

---

## 🚀 Quick Start (Full Stack)

> Recommended run order: **Backend → Processor → Client**

### Step 1 — Start BombPartyServer

```bash
cd BombPartyServer
npm install
```

Create `.env` file:

```env
DATABASE_URL="postgresql://user:password@localhost:5432/bombparty"
PORT=8080
```

Then run migrations and start server:

```bash
npx prisma migrate dev
npm run start
```

---

### Step 2 — Start BombProcessor

In a new terminal:

```bash
cd BombProcessor
make
./bin/BombProcessor
```

Optional args:

```bash
./bin/BombProcessor <port> <api_base_url>
```

Example:

```bash
./bin/BombProcessor 5050 http://localhost
```

---

### Step 3 — Start BombPartyClient

In another terminal:

```bash
cd BombPartyClient
make
./bin/BombParty
```

Follow the in-game prompts to:
- choose mode
- create/join rooms
- play rounds before the timer runs out

---

## 🕹️ How to Play

1. Start backend, processor, and client.
2. Create or join a room.
3. Each round presents a substring (e.g. `"str"`).
4. Type a valid word containing it before the bomb explodes.
5. Last surviving player wins the room match.

---

## 🔌 Protocol (Client ↔ Processor)

The processor expects a pipe-delimited protocol:

```text
CMD|<ID>|<ARG1>|<ARG2>|...
```

Known command IDs include:
- `001` → Create room
- `002` → Join room
- `003` → Delete room
- `004` → Leave room / disconnect flow
- `005` → Patch room
- `006` → Get room info

Reference: `BombProcessor/doc/client_server_protocol.txt`

---

## 🧪 Development Commands

### Backend

```bash
cd BombPartyServer
npm run dev         # watch mode
npm run start       # normal start
npm run studio-dev  # prisma studio with .env
```

### Client docs

```bash
cd BombPartyClient
doxygen Doxyfile
```

### Client icon build (optional)

```bash
cd BombPartyClient
make with_icon
```

---

## 🗄️ Data Model (Backend)

Main Prisma entities:
- `Room`
- `Player`
- `RoomPlayer`

Lookup tables:
- `StatusType`
- `LevelType`
- `RoomPlayerType`

Schema file:
- `BombPartyServer/prisma/schema.prisma`

---

## 🛠️ Troubleshooting

### Processor starts but room actions fail
- Verify backend is running.
- Check processor API base URL/port args.
- Confirm backend `PORT` and processor endpoint config match.

### Backend DB errors
- Confirm PostgreSQL is up.
- Verify `DATABASE_URL`.
- Run:
  ```bash
  npx prisma migrate dev
  ```

### Client cannot connect
- Ensure processor is listening on expected port.
- Confirm no firewall/network restriction on local port.

---

## 🧭 Roadmap Ideas

- Ranking/leaderboard
- Match history
- Replay/analytics
- Better bot/singleplayer tuning
- CI pipeline (C build + TS lint/typecheck/test)

---

## 🤝 Contributing

Contributions are welcome.

Suggested flow:
1. Fork repository
2. Create a feature branch
3. Commit your changes
4. Open a pull request with context and testing notes

---

## 📄 License

This project is licensed under the **MIT License**.
See [`LICENSE`](./LICENSE).
