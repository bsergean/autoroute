FROM ruby:2.7.1-alpine

RUN apk add --no-cache --update build-base linux-headers openssl-dev

RUN gem install faye-websocket

COPY autoroute.rb /usr/bin/

CMD ["sh"]
