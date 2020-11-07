folders := \
	go \
	python/websocket_client \
	python/websockets \
	rust \
	cpp/ixwebsocket \
	cpp/libwebsockets \
	cpp/beast \
	cpp/websocketpp \
	csharp \
	node \
	ruby

# swift

log_file := /tmp/autoroute.log

all:
	rm -f $(log_file)
	for folder in $(folders) ; do \
		(cd $$folder && make | tee -a $(log_file)) ; \
	done

	grep AUTOROUTE $(log_file)
