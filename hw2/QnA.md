## What does the 2>&1 do in the given command? Why is it needed?
2 is the file descriptor stderr and 1 stdout. The & ensures that 1 is also a descriptor and not a file. > is to redirect the output from stderr to stdin.
Without it the output from less is messed up.
## Which kernel options did you activate in addition to the config from Homework 1 and what are they needed for?
VIRTIO (Dependancy)
VIRTIO_INPUT (Support Virtio devices (keyboard))
VIRTIO_NET (Virtual Network Driver)
NETDEVICES (Enable if you want to connect to other computers)
NET_CORE (Network core drivers)
Net (Network support)
VIRTIO_CONSOLE (Data transfer guest/host)
VIRTIO_PCI (Support for virtual device drivers over PCI)
PCI (Support PCI)
INET (Support IP)
IP_PNP_DHCP (Support DHCP)
## How is the DHCP lease applied to the interface by udhcpc?
The client notifies some scripts and communicates with the DHCP server.
It accepts the address that the server offers.
## What does the example simple.script do on a new lease?
The IP adress will not change but the other DHCP parameters(default gateway, subnet mask, dns server) may change. It simply renews the other parameters.
## Which applets does the dropbearmulti binary support and what is their purpose?
dropbear - Dropbearserver
dbclient - Dropbearclient
dropbearkey - Key generator
dropbearconvert - Key converter
## What is shadow, and how can we login to the system if it's not used?
passwd contains our account information. If we set an x in the password field we securely safe our information in the shadow file. The shadow file can be edited by root only. It's readable by user but encrypted.
## What are the uid and gid numbers in Linux and how are they related to the passwd and group files in etc?
The GID field in passwd marks to with group the user belongs.
UID (User ID) 0 is reserved for root acounts. 1-99 for predefined accounts and 100-999 are reserved for groups, system accounts and admins.
Groups are used for monitoring, user management and special file access.
## How can you display the public key portion of the key file?
dropbearkey -y file_name
## Which file within a user's home directory is used by the SSH server to know which public keys are authorized for the respective user?
etc/dropbear/authorized_keys
## What is the difference between dynamic linking at compile time and using dlopen() in the code, and why can't the latter be detected with file and ldd?
It isn't really possible to compile it completely statically because it still relies on glibc shared libraries. These files are needed at runtime.
## How does the hostfwd=... help us to access the dropbear instance inside the VM?
It redirects the incoming TCP connection to the host port. Our dropbear instance listens on port 22.
## What is the full command that can be executed on your container to log in to the VM via SSH as the root user?
ssh root@localhost -p 22222 (In Addition, to avoid Host Key Checking you can disable it by passing -o StrictHostKeyChecking=no)
## What is the devpts filesystem mounted at /dev/pts used for when dropbear is configured with --enable-openpty (default)?
Openpty() finds pseudoterminal in the folder location dev/pts
## Which component on the system is responsible for creating these device nodes?
makedev
