# EIBP (Expedited Internet Bypass Protocol)
Code Execution Procedure 
In order to run the EIBP code, a slice with a 13 node topology should be up and live in GENI. A 13 node unreserved topology can be found under EIBP_script folder, UnreservedRSPEC, this XML file can create a 13 node topology, suitable site with good number of VM’s should be selected in order to activate the 13 node topology. 

The site for the slice can be chosen from the following webpage,
https://flsmonitor.fed4fire.eu/fls?testbedcategory=instageni  

The code can be used for N node topology, but based on the number of node in the topology the cmd_file should be configured. The cmd_file is configured based on the GENI slice, make sure the cmd_file is configured appropriately since cmd_file is essential to run EIBP code.   

Cmd_file Configuration steps
The N node topology is split into 3 tiers, Core Routers [tier1], Distribution Routers [tier2] and Access Routers [tier3]. Each tier has slight changes in configuring the nodes,

Tier1

node-0,sudo ./EIBP -T 1 -L 1.1 -N 1

‘node-0’ is the name of the node obtained from GENI

‘./EIBP’ is the source code that runs EIBP protocol on node-0

‘-T 1’ is to denote that it is a tier 1 router 

‘1.1’ is the label assigned to the node [Only Core routers needs to be labeled]

‘-N 1’ is to denote that it is not an end node/Access Router/tier 3 Router

Tier2 

node-3,sudo ./EIBP -T 2 -N 1

‘-T 2’ is to denote that it is a tier 2 router

Tier3

node-8,sudo ./EIBP -T 3 -N 0 10.10.26.1 24 eth3

‘-T 3’ is to denote that it is a tier 3 router

‘-N 0’ is to denote that it is an end node/Access Router/tier 3 Router

‘10.10.26.1’ IP address of the ip-node to which ‘node-8’ is connected to.

‘24’ CIDR 

‘eth3’ interface to which ‘node-8’ is connected to ‘ip-node’

Prepare a cmd_file similar to the cmd_file that can be found in EIBP_script folder with respect to the GENI slice. 
