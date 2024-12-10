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
