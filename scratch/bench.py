import socket
import time
import threading
import concurrent.futures

SERVER_ADDRESS = ('localhost', 8888)
NUM_REQUESTS = 1000
CONCURRENT_CONNECTIONS = 50  # Limit the number of concurrent connections
DOMAINS = ["example.com", "google.com", "facebook.com", "twitter.com", "github.com"]

def send_request(domain):
    with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as s:
        s.connect(SERVER_ADDRESS)
        start_time = time.time()
        s.sendall(domain.encode('utf-8'))
        response = s.recv(1024)
        end_time = time.time()
        elapsed_time = end_time - start_time
        return domain, response.decode('utf-8').strip(), elapsed_time

def benchmark():
    total_time = 0
    with concurrent.futures.ThreadPoolExecutor(max_workers=CONCURRENT_CONNECTIONS) as executor:
        futures = []
        for i in range(NUM_REQUESTS):
            domain = DOMAINS[i % len(DOMAINS)]
            futures.append(executor.submit(send_request, domain))

        for future in concurrent.futures.as_completed(futures):
            domain, response, elapsed_time = future.result()
            print(f"Domain: {domain}, Response: {response}, Time: {elapsed_time:.6f} seconds")
            total_time += elapsed_time

    avg_time = total_time / NUM_REQUESTS
    print(f"Average response time: {avg_time:.6f} seconds")

if __name__ == "__main__":
    benchmark()