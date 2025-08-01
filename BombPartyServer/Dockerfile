FROM node:18-alpine

# Diretório de trabalho
WORKDIR /usr/src/app

# Copia apenas package.json e lock para instalar dependências
COPY package*.json ./

# Instala dependências
RUN npm install

# Copia os arquivos do Prisma e gera o client
COPY prisma ./prisma
RUN npx prisma generate

# Copia o restante do código
COPY . .

# Exponha a porta
EXPOSE 3000

# Comando padrão (será sobrescrito no docker-compose)
CMD ["npm", "run", "dev"]