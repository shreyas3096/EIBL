
#include "sendAndFwd.h"
#include <fcntl.h>
#include <unistd.h>

/**
 * dataSend(char[],char[])
 *
 * method to send Ethernet frame
 *
 * @param etherPort (char[]) - interface to send on
 * @param inPayLoad (char[]) - payLoad to be sent
 *
 * @return status (int) - method return value
 */
int dataSend(char etherPort[20], unsigned char ipPacket[], char destTier[20], char srcTier[20], int ipPacketSize ) {

	int payLoad_Size = -1;
	int frame_Size = -1;

	int sockfd;
	struct ifreq if_idx;
	struct ifreq if_mac;

	int tx_len = 0;
	char ifName[IFNAMSIZ];

	
	uint8_t header[HEADER_SIZE];

	strcpy(ifName, etherPort);

	// Setting frame size
	payLoad_Size = ipPacketSize;
	frame_Size = HEADER_SIZE + 3 + 20 + 20 + payLoad_Size;

	unsigned char payLoad[payLoad_Size];
	memcpy(payLoad, ipPacket, payLoad_Size);

	// creating frame
	uint8_t frame[frame_Size];
	memset(frame, '\0', frame_Size);

	struct ether_header *eh = (struct ether_header *) header;
	struct sockaddr_ll socket_address;

	// Open RAW socket to send on
	if ((sockfd = socket(AF_PACKET, SOCK_RAW, IPPROTO_RAW)) == -1) {
		perror("ERROR: Socket Error");
	}
	memset(&if_idx, 0, sizeof(struct ifreq));
	strncpy(if_idx.ifr_name, ifName, IFNAMSIZ - 1);
	if (ioctl(sockfd, SIOCGIFINDEX, &if_idx) < 0)
		perror("ERROR: SIOCGIFINDEX - Misprint Compatibility");

	memset(&if_mac, 0, sizeof(struct ifreq));
	strncpy(if_mac.ifr_name, ifName, IFNAMSIZ - 1);
	if (ioctl(sockfd, SIOCGIFHWADDR, &if_mac) < 0)
		perror("ERROR: SIOCGIFHWADDR - Either interface is not correct or disconnected");

	// Initializing the Ethernet Header
	memset(header, 0, HEADER_SIZE);

	/*
	 *  Ethernet Header - 14 bytes
	 *
	 *  6 bytes - Destination MAC Address
	 *  6 bytes - Source MAC Address
	 *  2 bytes - EtherType
	 *
	 */
	eh->ether_shost[0] = ((uint8_t *) &if_mac.ifr_hwaddr.sa_data)[0];
	eh->ether_shost[1] = ((uint8_t *) &if_mac.ifr_hwaddr.sa_data)[1];
	eh->ether_shost[2] = ((uint8_t *) &if_mac.ifr_hwaddr.sa_data)[2];
	eh->ether_shost[3] = ((uint8_t *) &if_mac.ifr_hwaddr.sa_data)[3];
	eh->ether_shost[4] = ((uint8_t *) &if_mac.ifr_hwaddr.sa_data)[4];
	eh->ether_shost[5] = ((uint8_t *) &if_mac.ifr_hwaddr.sa_data)[5];

	eh->ether_dhost[0] = MY_DEST_MAC0;
	eh->ether_dhost[1] = MY_DEST_MAC1;
	eh->ether_dhost[2] = MY_DEST_MAC2;
	eh->ether_dhost[3] = MY_DEST_MAC3;
	eh->ether_dhost[4] = MY_DEST_MAC4;
	eh->ether_dhost[5] = MY_DEST_MAC5;

	eh->ether_type = htons(0x8850);

	tx_len += sizeof(struct ether_header);
	// Copying header to frame
	memcpy(frame, header, 14);

	// Copying messageType to frame
	u_int8_t msgData = MESSAGE_TYPE_DATA;
	memcpy(frame + 14, &msgData, 1);

	// Copying payLoad to frame
	/*
	 *  frame + 15 - Destination Tier Length (lenFDest) - 1 byte
	 *  frame + 16 - Destination Tier Value (destTier)  - variable
	 *
	 *  frame++    - Source Tier Length (lenFSrc)       - 1 byte
	 *  frame++    - Source Tier Value (srcTier)        - variable
	 *
	 *  frame++    - payload (IP Header + IP Payload)   - variable
	 *
	 */

	// Copying destination tier length and value
	u_int8_t lenFDest = strlen(destTier);
	memcpy(frame + 15, &lenFDest, 1);

	memcpy(frame + 16, destTier, lenFDest);

	// Copying source tier length and value
	u_int8_t lenFSrc = strlen(srcTier);
	memcpy(frame + 16+lenFDest, &lenFSrc, 1);
	memcpy(frame + 17+lenFDest, srcTier, lenFSrc);


	// Copying the payload to the frame
	memcpy(frame + 17+lenFDest+lenFSrc, payLoad, payLoad_Size);

	// Index of the network device
	socket_address.sll_ifindex = if_idx.ifr_ifindex;

	// Address length - 6 bytes
	socket_address.sll_halen = ETH_ALEN;

	// Destination MAC Address
	socket_address.sll_addr[0] = MY_DEST_MAC0;
	socket_address.sll_addr[1] = MY_DEST_MAC1;
	socket_address.sll_addr[2] = MY_DEST_MAC2;
	socket_address.sll_addr[3] = MY_DEST_MAC3;
	socket_address.sll_addr[4] = MY_DEST_MAC4;
	socket_address.sll_addr[5] = MY_DEST_MAC5;


	// Send packet
	if (sendto(sockfd, frame, tx_len + 3 + lenFDest + lenFSrc + payLoad_Size, 0,
			(struct sockaddr*) &socket_address, sizeof(struct sockaddr_ll)) < 0)
		printf("ERROR: Send failed\n");

	close(sockfd);
	return 0;
}
