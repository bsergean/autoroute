all: docker
	docker-compose up

.PHONY: docker

DOCKER_REPO ?= localhost
export DOCKER_REPO

NAME   := ${DOCKER_REPO}/autoroute-swift
TAG    := $(shell cat DOCKER_VERSION)
IMG    := ${NAME}:${TAG}
LATEST := ${NAME}:latest
BUILD  := ${NAME}:build

docker_test:
	docker build -t ${BUILD} .

docker:
	git clean -dfx
	docker build -t ${IMG} .
	docker tag ${IMG} ${BUILD}

docker_push:
	docker tag ${IMG} ${LATEST}
	docker push ${LATEST}
	docker push ${IMG}
