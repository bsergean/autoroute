FROM alpine:3.12

RUN apk add libwebsockets-dev openssl-dev g++

COPY autoroute.cpp .
RUN g++ -O3 --std=c++14 autoroute.cpp -o /usr/bin/autoroute -lwebsockets

CMD ["sh"]
