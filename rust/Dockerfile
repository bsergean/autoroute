FROM rust:1.46

RUN mkdir /app
COPY . /app
WORKDIR /app
RUN cargo build --release

CMD ["sh"]
