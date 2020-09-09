folders := \
	go \
	python/websocket_client \
	python/websockets \
	rust \
	cpp/ixwebsocket \
	cpp/libwebsockets \
	swift \
	csharp \
	node \
	ruby

log_file := /tmp/autoroute.log

all:
	for folder in $(folders) ; do \
		(cd $$folder && make | tee $(log_file)) ; \
	done

	grep autoroute_1 $(log_file)
