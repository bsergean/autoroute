import 'dart:async';
import 'dart:convert';
import 'dart:io';
import 'dart:math';

main() async {
  Random r = new Random();
  String key = base64.encode(List<int>.generate(8, (_) => r.nextInt(255)));

  HttpClient client = HttpClient(/* optional security context here */);
  HttpClientRequest request = await client.get('localhost', 8008,
      '/foo/ws?api_key=myapikey'); // form the correct url here
  request.headers.add('Connection', 'upgrade');
  request.headers.add('Upgrade', 'websocket');
  request.headers.add('sec-websocket-version', '13'); // insert the correct version here
  request.headers.add('sec-websocket-key', key);

  HttpClientResponse response = await request.close();
  // todo check the status code, key etc
  Socket socket = await response.detachSocket();

  WebSocket ws = WebSocket.fromUpgradedSocket(
    socket,
    serverSide: false,
  );
}
