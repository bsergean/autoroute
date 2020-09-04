//
// swiftc autoroute.swift
// Adapted from this example:
// https://github.com/MichaelNeas/perpetual-learning/blob/master/ios-sockets/SwiftWebSockets/SwiftWebSockets/Networking/NativeWebSocket.swift
// 
// swiftc autoroute.swift
//
// got message 13196
// got message 15295
// got message 15777
// ...
//
// This code does not work on Linux yet, the websocket module is not available on this platform.
// Swift on Linux still seems to be in the early days, even if the situation now is better
// with docker containers available.
//
import Foundation

// define what WebSocketConnections can do
protocol WebSocketConnection {
    func connect()
    func listen()
    func disconnect()
    var delegate: WebSocketConnectionDelegate? { get set }
}

protocol WebSocketConnectionDelegate: class {
    func onConnected(connection: WebSocketConnection)
    func onDisconnected(connection: WebSocketConnection, error: Error?)
    func onError(connection: WebSocketConnection, error: Error)
    func onMessage(connection: WebSocketConnection, text: String)
    func onMessage(connection: WebSocketConnection, data: Data)
}

extension WebSocketConnectionDelegate {
    func onMessage(connection: WebSocketConnection, text: String) {}
    func onMessage(connection: WebSocketConnection, data: Data) {}
}

class NativeWebSocket: NSObject, WebSocketConnection, URLSessionWebSocketDelegate {
    weak var delegate: WebSocketConnectionDelegate?
    var webSocketTask: URLSessionWebSocketTask!
    var urlSession: URLSession!
    let delegateQueue = OperationQueue()
    private var pingTimer: Timer?
    var msgCount = 0
    
    init(url: URL, autoConnect: Bool = false) {
        super.init()
        urlSession = URLSession(configuration: .default, delegate: self, delegateQueue: delegateQueue)
        webSocketTask = urlSession.webSocketTask(with: url)
        if autoConnect {
            connect()
        }
    }
    
    func urlSession(_ session: URLSession, webSocketTask: URLSessionWebSocketTask, didOpenWithProtocol protocol: String?) {
        delegate?.onConnected(connection: self)
    }
    
    func urlSession(_ session: URLSession, webSocketTask: URLSessionWebSocketTask, didCloseWith closeCode: URLSessionWebSocketTask.CloseCode, reason: Data?) {
        delegate?.onDisconnected(connection: self, error: nil)
    }
    
    func connect() {
        // required to open socket
        webSocketTask.resume()
        listen()
    }
    
    // Be aware that if you want to receive messages continuously you need to call this again after you’ve finished receiving a message. One way is to wrap this in a function and call the same function recursively.
    func listen()  {
        webSocketTask.receive { [weak self] result in
            guard let self = self else { return }
            switch result {
            case .failure(let error):
                self.delegate?.onError(connection: self, error: error)
            case .success(let message):
                // print("got message \(self.msgCount)")
                self.msgCount += 1
                switch message {
                case .string(let text):
                    self.delegate?.onMessage(connection: self, text: text)
                case .data(let data):
                    self.delegate?.onMessage(connection: self, data: data)
                @unknown default:
                    fatalError()
                }
            }
            self.listen()
        }
    }
    
    func disconnect() {
        webSocketTask.cancel(with: .normalClosure, reason: nil)
        pingTimer?.invalidate()
    }
    
    func report() {
        print("got message \(self.msgCount)")
        self.msgCount = 0
    }
}

let msgCount = 1000000
let urlStr = "ws://localhost:8008/\(msgCount)"
let url = URL(string: urlStr)

let webSocketClient = NativeWebSocket(url: url!, autoConnect: true)

Timer.scheduledTimer(withTimeInterval: 1, repeats: true) { result in
    webSocketClient.report()
}

// dispatchMain() is supposed 
CFRunLoopRun()
