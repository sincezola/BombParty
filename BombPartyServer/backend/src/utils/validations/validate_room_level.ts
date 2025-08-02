// eslint-disable-next-line @typescript-eslint/no-explicit-any
const isValidRoomLevel = (room_level: any): boolean => {
  if (typeof room_level !== "number") return false;

  if (!Number.isInteger(room_level)) return false;

  if (room_level < 1 || room_level > 3) return false;

  return true;
};

export default isValidRoomLevel;
