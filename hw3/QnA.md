## What is the difference between the commands gcc and aarch64-linux-gnu-gcc?
aarch64-linux-gnu-gcc is a compiler specificaly for arm64 architecture. gcc compiles code in the architecture of the running machine.
## What is the difference between the commands ld and aarch64-linux-gnu-ld?
ld returns diagnostic information of an object file but only if the objectfile was compiled to the running system architecture.
ld can't identify the architecture so "aarch64-linux-gnu-ld" is used to check linking for arm64 object files.
## Questions Please look at the diff between the newly saved kernel config and the one from homework 2.
### Which configure option(s) reflect(s) the architecture change?
### Which ARM platform has been activated by default?
## What is the name of the serial port that you need to pass to console=... in order to get console output on the PL011 device?
AMA0
## What is the meaning of the host option for this configure script?
To specify the system where built programs and libraries will run. 
## Please explain your choice for the machine and CPU types.
Trial and Error