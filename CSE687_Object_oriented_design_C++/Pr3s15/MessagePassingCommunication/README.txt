ORDER OF EXECUTION
==================
BUILDING/GENERATING
-------------------------------------------------------------------------------------------------------------------
STEP1 - compile.bat 	// used to compile
STEP2 - run.bat	// will run the executive with command line arguments.

Please check below notes also.

Notes :-
1) To show all the requirements, I have shown peer to peer communication between Client1,server1 and Server2 as well as Client2 ,server1 and server2
2) files would be send from send folder and it would be received at receive folder.
3) Command line args details :

---All Clients will take command line args like this :

Syntax : exename [clientip:clientport]
 [server1ip:server1port@fileName] [server2ip:server2port@fileName]


1st argument : client ip and port where client will start to listen data.
2nd argument : is optional to demonstarte file or string. if it is "/f" then it will demonstarte files transfer.
                          if it is "/s" then it would be demonstarte strings transfer.
Onwards 3rd Argument : it will have destination port and destination ip with filename as mentioned above.

Debug\Client1.exe localhost:8080 localhost:9080@send\demo.txt localhost:9081@send\big1.jpg

-- All Servers will take command line args like this :
exename [ipAddress:port]


example : Debug\Server2.exe localhost:9081


4) Due to thread and concurrency sometimes console logs would not be in sequence, so please consider that.



Thanking You
Saurabh Patel







