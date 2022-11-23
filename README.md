# iot_project
This project allows you to remotely control GPIO pins with websockets in a raspberry pi,
The server program runs on the raspberry and the client program runs in the other device.

To set the ip address of de the server edit the constant DEFAULT_IP in the common.h file

NOTE: the client program only works in linux because it uses the sockets linux API, i pretend to develop the client for other OS soon.
