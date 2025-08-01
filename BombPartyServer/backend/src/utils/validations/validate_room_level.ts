import isValidInteger from "../isIntegerValue.ts";

const isValidRoomLevel = (room_level: never) => {
  if (typeof room_level != "number") return false;

  if (!isValidInteger(String(room_level))) return false;

  if (room_level < 1 || room_level > 3) return false;

  return true;
};

export default isValidRoomLevel;
