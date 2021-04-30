Firstly fill the Credentials&Path file with credentials and path directories. Credentials&Path file is a dictionary which passes the value to the scripts based on the get key. Key value pairs to be added in the Credentials&Path file are,
uname – GENI account’s user name 
password – GENI account’s password 
rspec_file – RSPEC file of the slice
keyLocation – GENI account’s private key 
localsrc – Local path of the EIBL code directory 
remoteDst – Destination path of the node, usually like, /users/{$uname}
cmd_file – Local Path of the cmd_file   

Each script performs unique operations
Start_EIBL – Will copy and execute the EIBL_code from the local directory path to remote directory path as mention in localsrc and remoteDst under Credentials&Path file respectively.
Run_EIBL – Will just execute the EIBL_code that exists in remote location path.
EIBL_stop – Will kill the EIBL process and erase the EIBL code from the remote location path
EIBL_stop_process – Will just kill the EIBL process, in order to restart the process execute Run_EIBL script 
clear – Will clear all the logs files in each node. 
rspec – The XML file which contains the connections and other details required to communicate with GENI VM’s. rspec can be found in GENI slice, click on ‘View Rspec’, and copy paste the contents to rspec file. 
UnreservedRSPEC – This XML file can be used to create a 13 node topology, just import this file in GENI while creating a new slice. 
