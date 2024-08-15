# TCP connection<br/>
&emsp; This a simple c code that implements client/server communication over TCP protocol. Client opens a socket to the server and server simply just returns the date time. This repo contains these two different approach for the server:
### 1. single client at a time:<br/>
&emsp; In this approach, server only listens and accepts connection request from client and return relevant response.

### 2. multi client(multi-thread) at a time:<br/>
&emsp; In this approach, server listens and accepts from clients and can only response to limited number of clients. After accepting a connection from client, server make a new thread for that client to response if number of clients at that moment, 
have reached the limit, it returns "Max Client Number Is Reached" error to the client. But if number of clients haven't reached that limit, it makes a thread for that client and runs it. Of course that the server returns date time so fast that we couldn't 
validate if it was working correctly or not, so we added a sleep(60) to avoid that thread from returning the result and gets free for other clients, So we can validate our code easier.
