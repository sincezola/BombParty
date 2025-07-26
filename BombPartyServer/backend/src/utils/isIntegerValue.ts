const isValidInteger = (value: string): boolean => {
  const num = Number(value);
  return !isNaN(num) && Number.isInteger(num);
};
export default isValidInteger;
