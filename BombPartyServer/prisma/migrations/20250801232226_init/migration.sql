-- CreateTable
CREATE TABLE "status_type" (
    "status_type_id" SERIAL NOT NULL,
    "code" TEXT NOT NULL,

    CONSTRAINT "status_type_pkey" PRIMARY KEY ("status_type_id")
);

-- CreateTable
CREATE TABLE "level_type" (
    "level_type_id" SERIAL NOT NULL,
    "code" TEXT NOT NULL,

    CONSTRAINT "level_type_pkey" PRIMARY KEY ("level_type_id")
);

-- CreateTable
CREATE TABLE "room_player_type" (
    "room_player_type_id" SERIAL NOT NULL,
    "code" TEXT NOT NULL,

    CONSTRAINT "room_player_type_pkey" PRIMARY KEY ("room_player_type_id")
);

-- CreateTable
CREATE TABLE "room" (
    "room_key" SERIAL NOT NULL,
    "room_status" INTEGER NOT NULL,
    "room_name" TEXT NOT NULL,
    "room_password" TEXT,
    "room_capacity" INTEGER NOT NULL,
    "room_level" INTEGER NOT NULL,
    "created_at" TIMESTAMP(3) NOT NULL DEFAULT CURRENT_TIMESTAMP,
    "last_changed" TIMESTAMP(3) NOT NULL,

    CONSTRAINT "room_pkey" PRIMARY KEY ("room_key")
);

-- CreateTable
CREATE TABLE "player" (
    "player_key" SERIAL NOT NULL,
    "player_name" TEXT NOT NULL,

    CONSTRAINT "player_pkey" PRIMARY KEY ("player_key")
);

-- CreateTable
CREATE TABLE "room_player" (
    "room_player_id" SERIAL NOT NULL,
    "room_id" INTEGER NOT NULL,
    "player_id" INTEGER NOT NULL,
    "room_player_type" INTEGER NOT NULL,

    CONSTRAINT "room_player_pkey" PRIMARY KEY ("room_player_id")
);

-- CreateIndex
CREATE UNIQUE INDEX "status_type_code_key" ON "status_type"("code");

-- CreateIndex
CREATE UNIQUE INDEX "level_type_code_key" ON "level_type"("code");

-- CreateIndex
CREATE UNIQUE INDEX "room_player_type_code_key" ON "room_player_type"("code");

-- CreateIndex
CREATE UNIQUE INDEX "room_player_room_id_player_id_key" ON "room_player"("room_id", "player_id");

-- AddForeignKey
ALTER TABLE "room" ADD CONSTRAINT "room_room_status_fkey" FOREIGN KEY ("room_status") REFERENCES "status_type"("status_type_id") ON DELETE RESTRICT ON UPDATE CASCADE;

-- AddForeignKey
ALTER TABLE "room" ADD CONSTRAINT "room_room_level_fkey" FOREIGN KEY ("room_level") REFERENCES "level_type"("level_type_id") ON DELETE RESTRICT ON UPDATE CASCADE;

-- AddForeignKey
ALTER TABLE "room_player" ADD CONSTRAINT "room_player_room_id_fkey" FOREIGN KEY ("room_id") REFERENCES "room"("room_key") ON DELETE CASCADE ON UPDATE CASCADE;

-- AddForeignKey
ALTER TABLE "room_player" ADD CONSTRAINT "room_player_player_id_fkey" FOREIGN KEY ("player_id") REFERENCES "player"("player_key") ON DELETE CASCADE ON UPDATE CASCADE;

-- AddForeignKey
ALTER TABLE "room_player" ADD CONSTRAINT "room_player_room_player_type_fkey" FOREIGN KEY ("room_player_type") REFERENCES "room_player_type"("room_player_type_id") ON DELETE RESTRICT ON UPDATE CASCADE;
