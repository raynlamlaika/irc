import socket
import time

HOST = "127.0.0.1"
PORT = 1234

def send_irc_handshake(sock):
    sock.sendall(b"PASS hamza\n")
    time.sleep(0.5)
    sock.sendall(b"NICK jijo\n")
    time.sleep(0.5)
    sock.sendall(b"USER hamza\n")


def wait_for_welcome(sock):
    print("Waiting for welcome message...")

    while True:
        data = sock.recv(1024).decode(errors="ignore")
        if not data:
            break

        print(data)

        if "wellcome" in data:
            print("Connected to IRC server!\n")
            break


def receive_file(sock):
    print("Waiting for file header...")

    buffer = b""

    # Read until we get the header
    while True:
        chunk = sock.recv(1024)
        if not chunk:
            return

        buffer += chunk

        if b"FILE" in buffer:
            break

    # Extract header
    try:
        header_end = buffer.find(b"\n")
        header = buffer[:header_end].decode().strip()
        print("Header:", header)

        _, filesize = header.split()
        filesize = int(filesize)

        # Remaining data after header
        file_data = buffer[header_end + 1:]

    except:
        print("Invalid header format")
        return

    print(f"Receiving file ({filesize} bytes)...")

    received = len(file_data)

    with open("received_file", "wb") as f:
        f.write(file_data)

        while received < filesize:
            chunk = sock.recv(512)
            if not chunk:
                break

            f.write(chunk)
            received += len(chunk)

            print(f"{received}/{filesize} bytes", end="\r")

    print("\nFile received successfully!")

def send_file(sock, filepath):
    try:
        
        with open(filepath, "rb") as f:
            # get file size
            f.seek(0, 2)
            filesize = f.tell()
            f.seek(0)

            # send header
            header = f"FILE {filesize}\n".encode()
            sock.sendall(header)
            print("Header sent:", header)
            time.sleep(2)

            # send file data
            
            sent = 0
            while True:
                chunk = f.read(512)
                if not chunk:
                    break

                sock.sendall(chunk)
                sent += len(chunk)

                print(f"{sent}/{filesize} bytes sent", end="\r")

        print("\nFile sent successfully!")

    except FileNotFoundError:
        print("File not found")

def main():
    sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    sock.connect((HOST, PORT))

    # Step 1: IRC login
    send_irc_handshake(sock)

    # Step 2: wait welcome
    wait_for_welcome(sock)

    # Step 3: wait and receive file
    # receive_file(sock)

    time.sleep(2)
    path = "/home/hkhairi/goinfre/final_new_irc/.Server/Client.cpp";
    # sock.sendall(b"SEND lilo /home/hkhairi/goinfre/final_new_irc/.Server/Client.cpp\n")
    send_file(sock, path)

    sock.close()


if __name__ == "__main__":
    main()