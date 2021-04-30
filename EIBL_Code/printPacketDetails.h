#ifndef PRINTPACKETDETAILS_H_

extern FILE *fptr;
extern int enableLogScreen;
extern int enableLogFiles;


void printIPPacketDetails(unsigned char ipPacket[], int nIPSize) {


	int j = 0;

	for (; j < nIPSize; j++) {

		if(enableLogScreen)
			printf("IP Content : %02x  \n", ipPacket[j] & 0xff);
		if(enableLogFiles)
			fprintf(fptr,"IP Content : %02x  \n", ipPacket[j] & 0xff);
	}

	if(enableLogScreen)
		printf("\n");
	if(enableLogFiles)
		fprintf(fptr,"\n");

	u_int8_t ipVHL;

	memcpy(&ipVHL, ipPacket, 1);

	if(enableLogScreen){
		printf("          IPVHL d : %d \n", ipVHL);
		printf("        IPVHL 02x : %02x \n", ipVHL);
	}
	if(enableLogFiles){
		fprintf(fptr,"          IPVHL d : %d \n", ipVHL);
		fprintf(fptr,"        IPVHL 02x : %02x \n", ipVHL);
	}

	// TODO
	// Fix length
	int ipVersion = ipVHL / 10;
	int ipheaderLength = ipVersion * (ipVHL % 10);
	int protoType = (signed char) ipPacket[9];

	if(enableLogFiles){
		printf("       IP Version : %02x \n", ipVersion);
		printf("            IP HL : %d \n", ipheaderLength);
		printf("   Protocol Value : %d  \n", protoType);
	}
	if(enableLogFiles){
		fprintf(fptr,"       IP Version : %02x \n", ipVersion);
		fprintf(fptr,"            IP HL : %d \n", ipheaderLength);
		fprintf(fptr,"   Protocol Value : %d  \n", protoType);
	}

	if (protoType == 1){
		if(enableLogScreen)
			printf("  Protocol Type : ICMP \n");
		if(enableLogFiles)
			fprintf(fptr,"  Protocol Type : ICMP \n");
	}
	else if (protoType == 2){
		if(enableLogScreen)
			printf("  Protocol Type : IGMP \n");
		if(enableLogFiles)
			fprintf(fptr,"  Protocol Type : IGMP \n");
	}
	else if (protoType == 6){
		if(enableLogScreen)
			printf("  Protocol Type : TCP \n");
		if(enableLogFiles)
			fprintf(fptr,"  Protocol Type : TCP \n");
	}
	else if (protoType == 17){
		if(enableLogScreen)
			printf("  Protocol Type : UDP \n");
		if(enableLogFiles)
			fprintf(fptr,"  Protocol Type : UDP \n");
	}
	else if (protoType == 91){
		if(enableLogScreen)
			printf("  Protocol Type : LARP \n");
		if(enableLogFiles)
			fprintf(fptr,"  Protocol Type : LARP \n");
	}
	else if (protoType == 54){
		if(enableLogScreen)
			printf("  Protocol Type : NARP \n");
		if(enableLogFiles)
			fprintf(fptr,"  Protocol Type : NARP \n");
	}
	else{
		if(enableLogScreen)
			printf("  Protocol Type : OTHER \n");
		if(enableLogFiles)
			fprintf(fptr,"  Protocol Type : OTHER \n");
	}

	unsigned char ipSourceTemp[20];
	sprintf(ipSourceTemp, "%u.%u.%u.%u", ipPacket[12], ipPacket[13],
			ipPacket[14], ipPacket[15]);

	unsigned char ipDestTemp[20];
	sprintf(ipDestTemp, "%u.%u.%u.%u", ipPacket[16], ipPacket[17], ipPacket[18],
			ipPacket[19]);

	if(enableLogScreen){
		printf("       IP Source : %s  \n", ipSourceTemp);
		printf("  IP Destination : %s  \n", ipDestTemp);
	}
	if(enableLogFiles){
		fprintf(fptr,"       IP Source : %s  \n", ipSourceTemp);
		fprintf(fptr,"  IP Destination : %s  \n", ipDestTemp);
	}
}

void printMPLRPacketDetails(unsigned char mplrPacket[], int nSize) {


	int j = 0;
	for (; j < nSize - 14; j++) {

		if(enableLogScreen)
			printf("Content : %02x  \n", mplrPacket[j] & 0xff);
		if(enableLogFiles)
			fprintf(fptr,"Content : %02x  \n", mplrPacket[j] & 0xff);
	}

	if(enableLogScreen)
		printf("\n");	
	if(enableLogFiles)
		fprintf(fptr,"\n");	


}

#endif 