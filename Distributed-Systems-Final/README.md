# Distributed Systems Final
This repository contains the final practice of the UC3M Distributed Systems course.

In order to deploy and execute the application:

    - Execute the Makefile writing "make" in the command line
    - Execute the following two programs first:

        - Execute the storage server for RPCs writing "./storage_server" in the command line
        - Execute the broker server writing "./broker -p <port-number>" in the command line

    - Once these two programs are running, execute the two following ones:

        - Execute the editor (content publisher) writing "./editor -h <ip-address> -p <port-number> -t <topic> -m <message>" in the command line
        - Execute the java client writing "java -cp . suscriptor -s <ip-address> -p <port-number>" in the command line

    - Inside the suscriptor command line:

        - SUBSCRIBE <topic>
        - UNSUBSCRIBE <topic>
        - QUIT (to finalize the execution)

    - In case of wanted to go back to the pre-deployment stage write "make clean" in the command line

    - To regenerate the files from the .x file, execute "rpcgen -a -N -M storage.x", but not recommended since the server class will be entirely erased, so it has to be written again
