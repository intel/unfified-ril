DISCONTINUATION OF PROJECT.

This project will no longer be maintained by Intel.

Intel has ceased development and contributions including, but not limited to, maintenance, bug fixes, new releases, or updates, to this project. 

Intel no longer accepts patches to this project.

If you have an ongoing need to use this project, are interested in independently developing it, or would like to maintain patches for the open source software community, please create your own fork of this project. 
Unified-RIL (URIL)

URIL is a component running on linux and based on client / server model.
It is a primary interface to cellular modem capabilities but can evolve to expose other radios services such as WiFi, GNSS or Bluetooth.
Currently URIL implements the adaptation for and Android RIL based on ril.h, but can later offer other modem adapters to invoke modem functions through proprierary interfaces.
The main concept of URIL is to expose high level APIs to clients running in the user space, with a consistent and versionned APIs set.
Clients can run on the same platform as the modem or can run other devices, URIL will abstract the communication in between without modifying the clients.
URIL is capable of handling multiple connections, multiple requests from client. All these request are handled asynchronously as a message based API.

The main TODOs for URIL are:
- Make the compilation of AndroidRilAdapter conditionnal to avoid requiring ril.h for non Android devices.
- Implement service side voice calls, SMS and eCall.
- Enhance unit tests
- Offer an interface for GNSS

