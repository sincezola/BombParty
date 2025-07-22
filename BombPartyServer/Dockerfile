FROM node:18-alpine

WORKDIR /usr/src/app

COPY package*.json ./

RUN npm install

COPY prisma ./prisma
COPY backend ./backend
COPY .env.docker ./

RUN npx prisma generate

EXPOSE 3000

CMD ["npx", "tsx", "backend/src/server.ts"]
