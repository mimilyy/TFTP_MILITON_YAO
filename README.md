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
