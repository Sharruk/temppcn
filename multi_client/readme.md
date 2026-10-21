# TCP Multi-Client (Port 8080)
cc tserver.c -o tserver -pthread
./tserver

# In separate terminals (Run 2 or more clients):
cc tclient.c -o tclient
./tclient

---

# UDP Multi-Client (Port 8081)
cc userver.c -o userver
./userver

# In separate terminals:
cc uclient.c -o uclient
./uclient
