.PHONY: docker

NAME   := ${DOCKER_REPO}/autoroute-libwebsockets
TAG    := $(cat ../../DOCKER_VERSION)
IMG    := ${NAME}:${TAG}
BUILD  := ${NAME}:build

docker:
	git clean -dfx
	docker build -t ${IMG} .
	docker tag ${IMG} ${BUILD}

docker_push:
	docker push ${LATEST}
	docker push ${IMG}

deploy: docker docker_push
