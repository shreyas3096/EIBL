#include "endNetworkUtils.h"

extern FILE *fptr;
extern int enableLogScreen;
extern int enableLogFiles;

struct addr_tuple *failedLL_head = NULL;

struct addr_tuple *tablehead = NULL;

void clearEntryState() {
	struct addr_tuple *current;
	for (current = tablehead; current != NULL; current = current->next) {
		current->isNew = false;
	}
}

char * allocate_strmem(int len) {
	void *tmp;

	if (len <= 0) {
		fprintf(stderr,
				"ERROR: Cannot allocate memory because len = %i in allocate_strmem().\n",
				len);
		exit(EXIT_FAILURE);
	}

	tmp = (char *) malloc(len * sizeof(char));
	if (tmp != NULL) {
		memset(tmp, 0, len * sizeof(char));
		return (tmp);
	} else {
		fprintf(stderr,
				"ERROR: Cannot allocate memory for array allocate_strmem().\n");
		exit(EXIT_FAILURE);
	}
}

uint8_t * allocate_ustrmem(int len) {
	void *tmp;

	if (len <= 0) {
		fprintf(stderr,
				"ERROR: Cannot allocate memory because len = %i in allocate_ustrmem().\n",
				len);
		exit(EXIT_FAILURE);
	}

	tmp = (uint8_t *) malloc(len * sizeof(uint8_t));
	if (tmp != NULL) {
		memset(tmp, '\0', len * sizeof(uint8_t));
		return (tmp);
	} else {
		fprintf(stderr,
				"ERROR: Cannot allocate memory for array allocate_ustrmem().\n");
		exit(EXIT_FAILURE);
	}
}

int *
allocate_intmem(int len) {
	void *tmp;

	if (len <= 0) {
		fprintf(stderr,
				"ERROR: Cannot allocate memory because len = %i in allocate_intmem().\n",
				len);
		exit(EXIT_FAILURE);
	}

	tmp = (int *) malloc(len * sizeof(int));
	if (tmp != NULL) {
		memset(tmp, 0, len * sizeof(int));
		return (tmp);
	} else {
		fprintf(stderr,
				"ERROR: Cannot allocate memory for array allocate_intmem().\n");
		exit(EXIT_FAILURE);
	}
}

void add_entry_LL(struct addr_tuple *node) {
	
	if (tablehead == NULL) {
		tablehead = node;
		tablehead->next = NULL;
	} else {
		struct addr_tuple *current = tablehead;
		while (current->next != NULL) {
			current = current->next;
		}
		current->next = node;
		node->next = NULL;
	}
}

void add_failed_entry_LL(struct addr_tuple *node) { 

	if (failedLL_head == NULL) {
		failedLL_head = node;
		failedLL_head->next = NULL;
	} else {
		struct addr_tuple *current = failedLL_head;
		while (current->next != NULL) {
			current = current->next;
		}
		current->next = node;
		node->next = NULL;
	}
}

struct addr_tuple* add_matched_entry(struct addr_tuple *node,
		struct addr_tuple *matched) {
	struct addr_tuple *ptr = matched;
	struct addr_tuple *temp = NULL;

	temp = (struct addr_tuple*) calloc(1, sizeof(struct addr_tuple));
	memcpy(temp, node, sizeof(struct addr_tuple));
	temp->next = NULL;

	if (ptr == NULL) {
		matched = temp;
	} else {
		while (ptr->next != NULL) {
			ptr = ptr->next;
		}
		ptr->next = temp;
	}
	return matched;
}

// delete entries in the table.
bool delete_entry_LL_IP(struct in_addr ip) {
        bool hasDeletions = false;

        if (tablehead == NULL) {
                return false;
        } else {
                struct addr_tuple *current = tablehead;
                struct addr_tuple *prev = NULL;
                while (current != NULL) {
                        if (ip.s_addr == current->ip_addr.s_addr) {
                                hasDeletions = true;
                                
                                if (tablehead == current) {
                                        tablehead = tablehead->next;
                                        free(current);
                                        current = tablehead;
                                        continue;
                                } else {
                                        prev->next = current->next;
                                        free(current);
                                        current = prev;
                                }
                        }
                        prev = current;
                        current= current->next;
                }
        }
        
        print_entries_LL();
        return hasDeletions;
}

bool delete_entry_LL_Addr(uint8_t* tier_addr){
	bool hasDeletions = false;

        if (tablehead == NULL) {
				printf("tablehead is NULL");
                return false;
        } else {
                struct addr_tuple *current = tablehead;
                struct addr_tuple *prev = NULL;
                while (current != NULL) {
					
                        if (strcmp(tier_addr,current->tier_addr)==0) {
							
                                hasDeletions = true;
                                
                                if (tablehead == current) {
                                        tablehead = tablehead->next;
                                        free(current);
                                        current = tablehead;
                                        continue;
                                } else {
                                        prev->next = current->next;
                                        free(current);
                                        current = prev;
                                }
                        }
                        prev = current;
                        current= current->next;
                }
        }
        return hasDeletions;
}

void delete_failed_LL_Addr(uint8_t* tier_addr){

        if (failedLL_head == NULL) {
				printf("failedLL_head is NULL");
               
        } else {
                struct addr_tuple *current = failedLL_head;
                struct addr_tuple *prev = NULL;
                while (current != NULL) {
					
                        if (strcmp(tier_addr,current->tier_addr)==0) {
							
                                if (failedLL_head == current) {
                                        failedLL_head = failedLL_head->next;
                                        free(current);
                                        current = failedLL_head;
                                        continue;
                                } else {
                                        prev->next = current->next;
                                        free(current);
                                        current = prev;
                                }
                        }
                        prev = current;
                        current= current->next;
                }
        }
}
// match the longest prefix
/*
struct addr_tuple {
	int if_index;
	bool isNew;
	char tier_addr[MAX_TIER_ADDR_SIZE];
	struct in_addr ip_addr;
	uint8_t cidr;
	char etherPortName[10];
	struct addr_tuple *next;
};
this function takes ip addresses and tier addresses corresponding to an edge node
It tries to locate  an IP address mapped to Tieraddres in
struct tablehead 

*/
struct addr_tuple *find_entry_LL(struct in_addr *ip, char *tierAddr) {
	if (tablehead == NULL) {
		return NULL;
	} else {
		struct addr_tuple *current = tablehead;
		while (current != NULL) {
			struct in_addr temp;
			memcpy(&temp, ip, sizeof(struct in_addr));

			temp.s_addr = ntohl(temp.s_addr);
			temp.s_addr = ((temp.s_addr >> (32 - current->cidr))
					<< (32 - current->cidr));
			temp.s_addr = htonl(temp.s_addr);

			if ((current->ip_addr.s_addr == temp.s_addr)
					&& (strncmp(tierAddr, current->tier_addr, strlen(tierAddr))
							== 0)) {
				return current;
			}
			current = current->next;
		}
		return NULL;
	}
}

void print_entries_LL() {
	struct addr_tuple *current;

	if(enableLogScreen)
		printf("\nTier Address\t\tIP Address\n");
	if(enableLogFiles)
		fprintf(fptr,"Tier Address\t\tIP Address\n");
	for (current = tablehead; current != NULL; current = current->next) {
		if(enableLogScreen)
			printf("%s\t\t\t%s/%u\n", current->tier_addr, inet_ntoa(current->ip_addr), current->cidr);
		if(enableLogFiles)
			fprintf(fptr,"%s\t\t\t%s/%u\n", current->tier_addr, inet_ntoa(current->ip_addr), current->cidr);

	}
}

int buildPayload(uint8_t *data, int msgLen, int checkIndex) {
	struct addr_tuple *current;

	int payloadLen = 3;
	int entries = 0;

	if (msgLen == 2) {
		for (current = tablehead; current != NULL; current = current->next) {

			if (current->isNew == true && checkIndex != current->if_index) {

				uint8_t tierLen = (uint8_t) strlen(current->tier_addr);
				data[payloadLen] = tierLen;
				payloadLen++;

				memcpy(&data[payloadLen], current->tier_addr, tierLen);
				payloadLen = payloadLen + tierLen;

				uint8_t ipLen = (uint8_t) sizeof(struct in_addr);
				data[payloadLen] = ipLen;
				payloadLen++;

				memcpy(&data[payloadLen], &current->ip_addr, ipLen);
				payloadLen = payloadLen + ipLen;

				data[payloadLen] = current->cidr;
				payloadLen++;
				entries++;
			}
		}
	} else {
		for (current = tablehead; current != NULL; current = current->next) {
			uint8_t tierLen = (uint8_t) strlen(current->tier_addr);
			data[payloadLen] = tierLen;
			payloadLen++;

			memcpy(&data[payloadLen], current->tier_addr, tierLen);
			payloadLen = payloadLen + tierLen;

			uint8_t ipLen = (uint8_t) sizeof(struct in_addr);
			data[payloadLen] = ipLen;
			payloadLen++;

			memcpy(&data[payloadLen], &current->ip_addr, ipLen);
			payloadLen = payloadLen + ipLen;

			data[payloadLen] = current->cidr;
			payloadLen++;
			entries++;
		}
	}

	if (entries > 0) {

		data[0] = (uint8_t) MESSAGE_TYPE_ENDNW;
		data[1] = entries;
		data[2] = MESSAGE_TYPE_ENDNW_ADD;
	} else {
		payloadLen = 0;
	}
	return payloadLen;
}


int buildPayloadRemoveAdvts(uint8_t *data, struct addr_tuple *failedIPS, int msg) {
        struct addr_tuple *current;
                                
        // To reserve byte for keeping track of the entries and message_type
        int payloadLen = 3;     
        int entries = 0;                
                                        
        for (current=failedIPS; (current != NULL) && (current->isNew == true) ; current = current->next) {
                uint8_t tierLen = (uint8_t)strlen(current->tier_addr);
                data[payloadLen] = tierLen;
                payloadLen++;           
                                                
                memcpy(&data[payloadLen], current->tier_addr, tierLen);
                payloadLen = payloadLen + tierLen;      
                                                
                uint8_t ipLen = (uint8_t)sizeof(struct in_addr);
                data[payloadLen] = ipLen;
                payloadLen++;           
                                        
                memcpy(&data[payloadLen], &current->ip_addr, ipLen );
                payloadLen = payloadLen + ipLen;
                        
                data[payloadLen] = current->cidr;

                current->isNew = false;
                payloadLen++;
                entries++;
        }
        // fill number of entries added to payload, do this only if entries exist.
        if (entries > 0) {
                // Message type 5, used for advertising TierAdd<->Ipaddress entries.
                data[0] = (uint8_t) MESSAGE_TYPE_ENDNW;
                data[1] = entries;
                data[2] = msg;
		} else  {
                payloadLen = 0;
        }
        return payloadLen;
}

/* function to update the last 4 bytes*/
char* updateEndTierAddr(char destinationInterfaceIPAddr[]) {

	struct in_addr ip;
	if (inet_pton(AF_INET, destinationInterfaceIPAddr, &ip) == -1) { //inet_pton - convert IPv4 and IPv6 addresses from text to binary form
		if(enableLogScreen)
			printf("Error: inet_pton() returned error");
		if(enableLogFiles)
			fprintf(fptr,"Error: inet_pton() returned error");
	}

	struct addr_tuple *current = tablehead;
	while (current != NULL) {
		struct in_addr temp;
		memcpy(&temp, &ip, sizeof(struct in_addr));
		temp.s_addr = ntohl(temp.s_addr);

		temp.s_addr = (temp.s_addr >> (32 - current->cidr))
				<< (32 - current->cidr);

		temp.s_addr = htonl(temp.s_addr); //The htonl() function converts the unsigned integer hostlong from host byte order to network byte order.
		if (temp.s_addr == current->ip_addr.s_addr) {

			printf("updateEndTierAddr %s Found tier\n", current->tier_addr);
			return current->tier_addr;
		}

		current = current->next;
	}

	return NULL;
}

struct in_addr* getNetworkIP(char destinationInterfaceIPAddr[]) {

	struct in_addr ip;
	if (inet_pton(AF_INET, destinationInterfaceIPAddr, &ip) == -1) {
		if(enableLogScreen)
			printf("Error: inet_pton() returned error");
		if(enableLogFiles)
			fprintf(fptr,"Error: inet_pton() returned error");
	}

	struct addr_tuple *current = tablehead;
	while (current != NULL) {
		struct in_addr temp;
		memcpy(&temp, &ip, sizeof(struct in_addr));

		temp.s_addr = ntohl(temp.s_addr);

		temp.s_addr = (temp.s_addr >> (32 - current->cidr))
				<< (32 - current->cidr);

		temp.s_addr = htonl(temp.s_addr);
		if (temp.s_addr == current->ip_addr.s_addr) {

			return &current->ip_addr;
		}

		current = current->next;
	}

	return NULL;
}

char* findPortName(struct in_addr *nwIP) {

	struct addr_tuple *current = tablehead;
	while (nwIP != NULL && current != NULL) {
		if (current->ip_addr.s_addr == nwIP->s_addr) {
			
			return current->etherPortName;
		}

		current = current->next;
	}
	return NULL;
}
//Deletes the label from the Label List(LL) and adds the label to the failedLL_head
void modify_LL(char *addr){
	
	if (tablehead == NULL) {
                //return false;
        } else {
                struct addr_tuple *current = tablehead;
                
                while (current != NULL) {
                        if(strcmp(current->tier_addr, addr)==0) {
                                
								
								struct addr_tuple *a = (struct addr_tuple*) calloc (1, sizeof(struct addr_tuple));
                        		strcpy(a->tier_addr, current->tier_addr);
                        
                        		a->if_index = -1;
                        		a->isNew = true;
                        		memcpy(&a->ip_addr, &current->ip_addr, sizeof(struct in_addr));
                        		a->cidr = current->cidr;
								strcpy(a->etherPortName, current->etherPortName);

								add_failed_entry_LL(a);
								delete_entry_LL_Addr(a->tier_addr);
                                
                        }
						
                        current= current->next;
                }
        }
}

struct addr_tuple* getFailedAddr(){
	return failedLL_head;
}