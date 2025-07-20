import app from "./app.ts";
import { config } from "dotenv";

config(); // Config .env variables

const port = process.env.PORT || 8080;

app.listen(port, () => {
  console.log(`Server listening on port ${port}`);
});
