cd server_files
python3 -m http.server 8000

cc client.c -o client
./client