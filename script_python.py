import socket
import threading
import time

HOST = "127.0.0.1"
PORT = 6667

NUM_THREADS = 20
CLIENTS_PER_THREAD = 50   # total = 1000 clients

clients = []
lock = threading.Lock()
last_activity = time.time()


def client_worker(start_id, count):
    global last_activity

    local_clients = []

    for i in range(count):
        cid = start_id + i
        try:
            s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
            s.connect((HOST, PORT))
            s.setblocking(False)

            nick = f"bot{cid}"
            user = f"bot{cid}"

            msg = (
                f"PASS hamza\r\n"
                f"NICK {nick}\r\n"
                f"USER {user} 0 * {user}\r\n"
                f"JOIN #room\r\n"
            )

            s.send(msg.encode())
            local_clients.append(s)

        except Exception as e:
            print(f"[CONNECT ERROR {cid}] {e}")

    with lock:
        clients.extend(local_clients)

    # 🔥 keep sending data (force server work)
    while True:
        for s in local_clients:
            try:
                s.send(b"JOIN #room\r\n")
            except:
                pass

            try:
                data = s.recv(1024)
                if data:
                    with lock:
                        last_activity = time.time()
            except:
                pass

        time.sleep(0.1)


# 🔥 start threads
threads = []
for t in range(NUM_THREADS):
    th = threading.Thread(
        target=client_worker,
        args=(t * CLIENTS_PER_THREAD, CLIENTS_PER_THREAD),
        daemon=True
    )
    th.start()
    threads.append(th)

print(f"[INFO] Started {NUM_THREADS} threads")

# 🔍 monitor blocking
while True:
    time.sleep(2)

    with lock:
        total = len(clients)
        idle_time = time.time() - last_activity

    print(f"[STATUS] clients={total} idle_for={idle_time:.2f}s")

    if idle_time > 5:
        print("🔥 REAL BLOCK DETECTED (server not responding)")
