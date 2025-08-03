# BombParty

**BombParty** is a multiplayer game where players must type words that contain a randomly chosen **substring** before the "bomb" explodes.  
The project consists of three main modules:  

- **BombParty** – Client in **C** (player).  
- **BombProcessor** – Intermediate socket server in **C**.  
- **BombServer** – Backend in **TypeScript** with **Prisma** (API and persistence).  

---

## Architecture

```
+-----------------+        +-------------------+        +-----------------------+
|   BombParty     | <----> |  BombProcessor    | <----> |    BombPartyServer    |
| (Client - C)    |        | (Socket Server C) |        |   (API TS + Prisma)   |
+-----------------+        +-------------------+        +-----------------------+
```

- **BombParty (Client):**  
  Playable interface (terminal), connecting to BombProcessor to join rooms.  

- **BombProcessor (Socket Server):**  
  Responsible for managing rooms and communication between multiple clients.  
  Interacts with BombServer to register matches and players.  

- **BombServer (Backend):**  
  Developed in **TypeScript**, with **Prisma** for database access.  
  Provides routes for room creation, player registration, and match management.  

---

## Requirements

### Client and Processor (C):
- **GCC** or **Clang**  
- **Make**  
- **Libraries:**  
  - `libcurl` (HTTP requests).  
  - `pthread` (multithreading).  
  - `imagemagick` (*Optional* compilation that generates the executable icon).  

### Server (Backend):
- **Node.js** (>= 18)  
- **npm** or **yarn**  
- **Database:** PostgreSQL  

---

## Installation

### 1. Backend (BombServer)
```bash
cd BombPartyServer
npm install
npx prisma migrate dev
npm run start
```

Create a `.env` file in the `bombserver` folder (check `.env.example`) with:  
```env
DATABASE_URL="postgresql://user:password@localhost:5432/bombparty"
```

---

### 2. Socket Server (BombProcessor)
```bash
cd BombProcessor
make
./bin/BombProcessor
```

The socket server will start and wait for client connections.  

---

### 3. Client (BombParty)
```bash
cd BombParty
make
./bin/BombParty
```

Connect to the server, create or join a room, and start playing.  

---

## How to Play

1. Start the **BombServer** (backend).  
2. Start the **BombProcessor** (room server).  
3. Open **BombParty** (client) and connect.  
4. Join or create a room.  
5. In each round, a **substring** will be randomly selected.  
6. Type a valid word containing it before time runs out.  

---

## Folder Structure

```
/BombParty        # Client in C
    ├─ src/
    ├─ include/
    └─ Makefile

/BombProcessor    # Intermediate server in C
    ├─ src/
    ├─ include/
    └─ Makefile

/BombPartyServer  # Backend in TypeScript
    ├─ src/
    ├─ prisma/
    └─ package.json
```

---

## Next Steps
- Implement **ranking** and match history.  

---

## Contribution
Want to help improve BombParty?  
Open an **issue** or send a **pull request**.  

---

## License
Project distributed under the **MIT** license.
