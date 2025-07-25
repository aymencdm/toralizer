# toralizer

## Overview

toralizer is a dynamic library for transparent SOCKS4 proxying of any Linux command-line application. It is designed for offensive security researchers, red teamers, and penetration testers who need to route arbitrary tools through a SOCKS4 proxy (such as Tor) without modifying the source code of those tools.

By leveraging LD_PRELOAD, toralizev2 intercepts outgoing TCP connections and transparently redirects them through a configurable SOCKS4 proxy.

## Features

- Intercepts the `connect()` system call using LD_PRELOAD
- Transparently routes TCP connections via a SOCKS4 proxy
- Works with any dynamically linked command-line tool (curl, nc, nmap, etc.)
- Simple configuration via environment variables or header defines
- Useful for red teaming, C2, and network evasion

## How It Works

toralizev2 overrides the `connect()` function using LD_PRELOAD. When a program tries to connect to a remote host, the library:

1. Checks if the connection is already to the proxy (to avoid loops)
2. Opens a new socket to the SOCKS4 proxy
3. Sends a SOCKS4 connect request for the original destination
4. If successful, duplicates the proxy socket onto the original file descriptor
5. The application is now transparently communicating through the proxy

## Build

```
make
```

This will produce `toralize.so`.

## Usage

Run any command through the proxy by preloading the library:

```
LD_PRELOAD=./toralize.so ./your_command [args]
```

Example with curl:

```
LD_PRELOAD=./toralize.so curl http://example.com
```

Example with netcat:

```
LD_PRELOAD=./toralize.so nc scanme.nmap.org 80
```

## Configuration

Edit `toralize.h` to set your SOCKS4 proxy address, port, and username:

```
#define PROXY "127.0.0.1"
#define PORT 9050
#define USERNAME "offsec"
```

Rebuild after changing these values.

## Security/Offensive Use Context

This tool is intended for research, red teaming, and penetration testing in controlled environments. It enables stealthy network routing and can help bypass network restrictions or monitoring by tunneling traffic through a proxy (e.g., Tor).

**Use responsibly and only on systems/networks you are authorized to test.**

## Credits

Developed by an offensive security researcher. Inspired by classic LD_PRELOAD and proxy chaining techniques.

## License

For educational and research use only. No warranty. Use at your own risk.

---

> "With great power comes great responsibility."

