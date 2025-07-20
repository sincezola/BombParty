/*
  Warnings:

  - You are about to drop the column `description` on the `level_type` table. All the data in the column will be lost.
  - You are about to drop the column `description` on the `room_player_type` table. All the data in the column will be lost.
  - You are about to drop the column `description` on the `status_type` table. All the data in the column will be lost.

*/
-- AlterTable
ALTER TABLE "level_type" DROP COLUMN "description";

-- AlterTable
ALTER TABLE "room_player_type" DROP COLUMN "description";

-- AlterTable
ALTER TABLE "status_type" DROP COLUMN "description";
