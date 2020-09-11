FROM alpine:3.12

RUN apk add g++ openssl-dev boost-dev

COPY autoroute.cpp .
RUN g++ -O3 --std=c++14 autoroute.cpp -o /usr/bin/autoroute

CMD ["sh"]
