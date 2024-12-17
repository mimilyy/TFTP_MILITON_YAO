# LAB2 - Lab Work Synthesis – TFTP Client
### MILITON Pierre and YAO Emilie 

## Objectives

The goal of this lab assignment is to develop a TFTP (Trivial File Transfer Protocol) client that can transfer files between a client and server using the RFC specifications for TFTP.
The objectives include:
- Building both a file download and upload client for TFTP.
- Implementing features such as blocksize optimization, error handling, and packet loss recovery.
- Testing the client with various file types and sizes while adhering to RFC standards.

## Answers
### Question 1  

The main function verifies if the number of command-line arguments is equal to 3. If not, it displays an error message with the correct usage instructions. It extracts the server host and filename from the command-line arguments and proceeds to call the puttftp and gettftp functions.

***Terminal output***

![Error](https://github.com/user-attachments/assets/a090f6c1-3198-47d9-ba72-932e0d344981)

### Question 2

The main function initializes a *struct addrinfo hints, res, and uses getaddrinfo to retrieve the server's address, and then extracts it.

***Terminal output***

![q2](https://github.com/user-attachments/assets/035d440d-6e78-4938-bcbb-c7f334027d7c)

### Question 3

The program creates a socket. If an error occurs during socket creation, it prints an error message, "Socket creation error," and exits. It also checks for any connection errors with the server, and if the connection fails, it displays an error message.

### Question 4

![image](https://github.com/user-attachments/assets/d6528dd1-8b6d-4df5-81b4-582d27d5b761)

### 4.a.

*Construction of the RRQ Packet*   
The opcode field of the rrqPacket is assigned the RRQ opcode. The htons is then used to convert the opcode to network byte order (big-endian).

*Transmission of the RRQ Packet to the Server*   
The RRQ packet is transmitted to the TFTP server using a UDP socket. The sendto function is used to send the RRQ packet to the server. The parameters used are as follows:   
- sockfd: The socket file descriptor.   
- &rrqPacket: A pointer to the RRQ packet.   
- sizeof(rrqPacket): The size of the RRQ packet in bytes.   
- 0: Flags set to zero.   
- server_addr: A pointer to the server's address structure.   
- sizeof(struct sockaddr): The size of the server's address structure, in bytes.

The sendto function is called as follows:
sendto(sockfd, &rrqPacket, sizeof(rrqPacket), 0, server_addr, sizeof(struct sockaddr));

***Captured by us on Wireshark***

![q4a](https://github.com/user-attachments/assets/d96c819f-c7c2-4b51-9174-166677aa76c1)

### 4.b.

The sendRRQ function is used to send a Read Request (RRQ) packet to the server. The receiveDAT function is responsible for receiving a DATA packet from the server, sending an Acknowledgment (ACK) packet back to the server, and writing the data to a file.

*sendRRQ function*:   
The first two bytes of the rrq_buffer are set to 0x00 and 0x01, representing the RRQ opcode. The filename is copied into the buffer starting at position 2 and is null-terminated. The mode is copied after the filename and is also null-terminated. Finally, the RRQ packet is sent to the server.

*receiveDAT function*:   
The recvfrom function is used to receive the DATA packet from the server. If an error occurs during the reception, an error message is printed. The function signature is as follows:
recvfrom(sockfd, data_buffer, MAX_BUFFER_SIZE, 0, server_addr, &server_addr_len);

The parameters are explained as follows:
- sockfd: The socket file descriptor.
- data_buffer: A pointer to the buffer where the received data will be stored.
- MAX_BUFFER_SIZE: The maximum number of bytes that can be received.
- 0: Flags set to zero.
- server_addr: A pointer to a variable where the server's address will be stored.
- &server_addr_len: A pointer to the size of the server address.

Once the DATA packet is received, the ACK packet is constructed. The first two bytes are set to 0x00 and 0x04, representing the ACK opcode. The data buffer is then copied into the packet before it is sent to the server. If an error occurs during the sending process, an error message is printed.

The filename is opened in binary write mode, and the data from the packet is written to the file, starting from the 5th byte, as the first 4 bytes are used for the opcode and block number.

Problem encountered:   
Small files can be sent successfully. However, due to the packet size limitation, larger files are split into several smaller packets.

***Captured by us on Wireshark***

![q4b](https://github.com/user-attachments/assets/63870a33-20fa-40f8-9ddb-18192976d714)

### 4.c.

The receiveFile function is responsible for receiving DATA (DAT) packets, sending corresponding Acknowledgment (ACK) packets to the server, and writing the received data into a file until the last packet is received. This function has been tested using the file ensea.png, and the process was captured using Wireshark.

***Captured by us on Wireshark***

![q4c](https://github.com/user-attachments/assets/ce67438b-271f-4b7b-a2e6-86174f06f71f)
