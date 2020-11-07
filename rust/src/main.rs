use tungstenite::{connect};
use url::Url;

use std::thread;
use std::time::Duration;
use std::time::Instant;

use std::sync::atomic::{AtomicUsize, Ordering};

static RECEIVED_MESSAGES: AtomicUsize = AtomicUsize::new(0);

fn main() {
    thread::spawn(|| {
        loop {
            println!("messages received: {}", RECEIVED_MESSAGES.load(Ordering::Relaxed));
            RECEIVED_MESSAGES.store(0, Ordering::Relaxed);
            thread::sleep(Duration::from_secs(1));
        }
    });

    let mut target = 1000000;
    let url = "ws://push:8008";
    let full_url = format!("{}/{}", url, target);

    let (mut socket, response) =
        connect(Url::parse(&full_url).unwrap()).expect("Can't connect");

    println!("Connected to the server");
    println!("Response HTTP code: {}", response.status());
    println!("Response contains the following headers:");
    for (ref header, value) in response.headers() {
        println!("* {} {:?}", header, value);
    }

    let start = Instant::now();

    loop {
        socket.read_message().expect("Error reading message");
        RECEIVED_MESSAGES.fetch_add(1, Ordering::Relaxed);

        target -= 1;
        if target == 0 {
            println!("done");
            let elapsed = start.elapsed();
            println!("AUTOROUTE rust :: {:.2?}", elapsed);
            socket.close(None);
            break;
        }
    }
    // socket.close(None);
}
