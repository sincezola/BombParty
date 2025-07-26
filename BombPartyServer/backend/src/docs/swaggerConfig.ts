import swaggerJSdoc from "swagger-jsdoc";

const options: Parameters<typeof swaggerJSdoc>[0] = {
  definition: {
    openapi: "3.0.0",
    info: {
      title: "BombParty Backend",
      version: "1.0.0",
      description: "Documentação do Servidor do BombParty",
    },
  },
  apis: ["./src/controllers/**/*.ts"],
};

const swaggerSpec = swaggerJSdoc(options);

export default swaggerSpec;
