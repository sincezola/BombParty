import isValidInteger from "../isIntegerValue.ts";

const isValidRoomCapacity = (room_capacity: never) => {
  if (typeof room_capacity != "number") return false;

  if (!isValidInteger(String(room_capacity))) return false;

  if (room_capacity < 2 || room_capacity > 5) return false;

  return true;
};

export default isValidRoomCapacity;
