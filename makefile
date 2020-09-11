folders := \
	go \
	python/websocket_client \
	python/websockets \
	rust \
	cpp/ixwebsocket \
	cpp/libwebsockets \
	cpp/beast \
	csharp \
	node \
	ruby

# swift

log_file := /tmp/autoroute.log

all:
	for folder in $(folders) ; do \
		(cd $$folder && make | tee $(log_file)) ; \
	done

	grep autoroute_1 $(log_file)
