const isValidRoomCapacity = (room_capacity: any): boolean => {
  if (typeof room_capacity !== "number") return false;

  if (!Number.isInteger(room_capacity)) return false;

  if (room_capacity < 2 || room_capacity > 5) return false;

  return true;
};

export default isValidRoomCapacity;
