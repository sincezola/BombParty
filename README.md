
# BombParty

**BombParty** é um jogo multiplayer em que os jogadores precisam digitar palavras que contenham uma **substring** sorteada antes que a “bomba” exploda.  
O projeto é composto por três módulos principais:  

- **BombParty** – Cliente em **C** (jogador).  
- **BombProcessor** – Servidor socket intermediário em **C**.  
- **BombServer** – Backend em **TypeScript** com **Prisma** (API e persistência).  

---

## Arquitetura

```
+-----------------+        +-------------------+        +-----------------------+
|   BombParty     | <----> |  BombProcessor    | <----> |    BombPartyServer    |
| (Client - C)    |        | (Socket Server C) |        |   (API TS + Prisma)   |
+-----------------+        +-------------------+        +-----------------------+
```

- **BombParty (Cliente):**  
  Interface jogável (terminal), conectando-se ao BombProcessor para participar das salas.  

- **BombProcessor (Servidor Socket):**  
  Responsável por gerenciar as salas e a comunicação entre múltiplos clientes.  
  Interage com o BombServer para registrar partidas e jogadores.  

- **BombServer (Backend):**  
  Desenvolvido em **TypeScript**, com **Prisma** para acesso ao banco.  
  Fornece as rotas de criação de salas, cadastro de jogadores e gerenciamento de partidas.  

---

## Requisitos

### Cliente e Processor (C):
- **GCC** ou **Clang**  
- **Make**  
- **Bibliotecas:**  
  - `libcurl` (requisições HTTP).  
  - `pthread` (multithreading).
  - `imagemagick` (*Opcional* compilacao que gera o icone do executavel).  

### Servidor (Backend):
- **Node.js** (>= 18)  
- **npm** ou **yarn**  
- **Banco de dados:** PostgreSQL  

---

## Instalação

### 1. Backend (BombServer)
```bash
cd BombPartyServer
npm install
npx prisma migrate dev
npm run start
```

Crie um arquivo `.env` na pasta `bombserver` (checar .env.example) com:  
```env
DATABASE_URL="postgresql://usuario:senha@localhost:5432/bombparty"
```

---

### 2. Servidor Socket (BombProcessor)
```bash
cd BombProcessor
make
./BombProcessor
```

O servidor socket será iniciado e aguardará conexões dos clientes.  

---

### 3. Cliente (BombParty)
```bash
cd BombParty
make
./BombParty
```

Conecte-se ao servidor, crie ou entre em uma sala e comece a jogar.  

---

## Como Jogar

1. Inicie o **BombServer** (backend).  
2. Inicie o **BombProcessor** (servidor de salas).  
3. Abra o **BombParty** (cliente) e conecte-se.  
4. Entre ou crie uma sala.  
5. A cada rodada, uma **substring** será sorteada.  
6. Digite uma palavra válida que a contenha antes do tempo acabar.  

---

## Estrutura de Pastas

```
/BombParty        # Cliente em C
    ├─ src/
    ├─ include/
    └─ Makefile

/BombProcessor    # Servidor intermediário em C
    ├─ src/
    ├─ include/
    └─ Makefile

/BombPartyServer  # Backend em TypeScript
    ├─ src/
    ├─ prisma/
    └─ package.json
```

---

## Próximos Passos
- Implementar **ranking** e histórico de partidas.
  
---

## Contribuição
Quer ajudar a melhorar o BombParty?  
Abra uma **issue** ou envie um **pull request**.  

---

## Licença
Projeto distribuído sob a licença **MIT**.
