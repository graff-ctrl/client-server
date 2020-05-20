# client-server
Client server project for CPSC 5042

This is a client server project for educational purposes only. 

The Client-Server group project is a demonstration of the knowledge you have obtained regarding:

Network Programming (Sockets)
Concurrency (Threads, Critical Sections, Semaphores)
Building a Distributed Application (Client and Server)
The main objective is to create two pieces of software (Client and Server) that will exchange RPC's (Remote Procedure Calls). Remote Procedure call is an industry term to represent the concept of a function that has a client (who call's the function), and the server (who receives the input arguments, and returns some type of value). You will have the privilege of using your own imagination of what those RPC's will be. For instance you could have a thin client that would send "database SQL Queries", and then retrieve the output.  

You will be required to make a connection from the Client to the Server (using Socket and bind calls).  I would like you to create 5 RPC's. The first RPC will be the toughest, as you will have to create all the scaffolding and plumbing on both the client and server pieces. I have split the project into three major milestones that are listed below:

Milestone 1 - Design and Create simple Client/Server Applications.

Requirements:

Design five RPC's between the Client and the Server using the template below as an example. Try to make them relevant. Two of the five have to be:

int Connect (char *userName , char *password)

int Disconnect()

You will design at least three more. You have thre freedom to do whatever you want, but it would be interesting to make some sort of a game, calculator, or something relevant.

 

Create a "client" and a "server" on CS1. The server will open a new socket in which the client will make a call. 
When the server gets a new connection request from client, it will create a new thread. That new thread will then be solely responsible for talking to the client that just called it. The server will then to continue listening for more connection requests
The data that is transferred between the Client and Server should be string. It is beyond the scope of this class to pass integers in their raw form.
The client program should
Establish a connection to the server
Execute the Connect call
Sleep for a random amount of time between 1 and 10 seconds
Execute the Disconnect call
The three other RPC's can be coded and tested later
You will put the Client code in "client.cpp" and the Server code in "server.cpp"
You will have a make file that will build these components
You will put some debug messages into both client and server code that will end up going to stdout
The deliverable will be:
milestone1.pdf which contains:
A two paragraph summary of project
A listing of all five RPC's (including connect and disconnect). Please add any description to what the RPC is, and description to your input/output variable
Output (taken from screen) that shows your client and server running
Instructions on how you made and started your client and server binaries
milestone1.zip which contains:
client.cpp
server.cpp
makefile 
client and server binaries

With the architecture in place, this particular Milestone is actually fairly easy. Requirements are:

Implement the three other RPC' (code/test/debug) s in your Project
Insert the following UNIX Concurrency commands somewhere in your server code:
pthread_mutex_lock
pthread_mutex_unlock
pthread_mutex_init
sem_open
sem_wait
Run the program . Your program should still have some sort of delay, but it should also execute all of the RPC's that you have created.
Run the program multiple times at once (5 occurence). Please have the diagnostic output messages show up in relevant places.
Create a small Powerpoint that will describe your RPC's  and program. It should be about a 10 minute presentation (as a general rule one slide per minute)
