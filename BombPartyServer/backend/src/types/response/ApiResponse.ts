import type httpStatusCode from "../enums/HttpStatusCode.ts";

type ApiResponse<T> = {
  statusCode: httpStatusCode;
  body: T;
};

export default ApiResponse;
