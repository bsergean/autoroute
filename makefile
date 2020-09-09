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

all:
	for folder in $(folders) ; do \
		(cd $$folder && make) ; \
	done

