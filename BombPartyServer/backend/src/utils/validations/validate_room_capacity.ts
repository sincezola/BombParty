import isValidInteger from "../isIntegerValue.ts";

const isValidRoomCapacity = (room_capacity: number) => {
  if (!isValidInteger(String(room_capacity))) return false;

  if (room_capacity < 2 || room_capacity > 5) return false;

  return true;
};

export default isValidRoomCapacity;
