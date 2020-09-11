FROM alpine:3.12

RUN apk add g++ openssl-dev boost-dev websocket++

COPY autoroute.cpp .
RUN g++ -O3 --std=c++14 autoroute.cpp -o /usr/bin/autoroute -lboost_system -lssl -lcrypto

CMD ["sh"]
