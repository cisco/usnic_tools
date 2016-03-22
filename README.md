# Overview

This is a simple tool for extracting some diagnostics and informational
meta data out of Cisco usNIC devices using the Cisco usNIC extensions
in [Libfabric](http://libfabric.org/).

The `usnic_devinfo` executable will return information about your usNIC
Linux devices.  For example:

```
$ usnic_devinfo
usnic_0:
	Interface:               eth4
	MAC Address:             58:ac:78:f2:8b:de
	IP Address:              10.5.20.195
	Netmask:                 255.255.0.0
	Prefix len:              16
	MTU:                     9000
	Link State:              UP
	Bandwidth:               40 Gb/s
	Device ID:               UCSC-PCIE-C40Q-03 [VIC 1385] [0x014d]
	Vendor ID:               4407
	Vendor Part ID:          207
	Firmware:                6.0(1.239)
	VFs:                     64
	CQ per VF:               4
	QP per VF:               6
	Interrupts per VF:       1
	Max CQ:                  256
	Max CQ Entries:          65535
	Max QP:                  384
	Max Send Credits:        4095
	Max Recv Credits:        4095
	Capabilities:
	  Map per res:           yes
	  PIO sends:             yes
	  CQ interrupts:         yes
```

# Download

Visit the [Downloads page](https://github.com/cisco/usnic_tools/wiki/Downloads).

# Building and Installing

The usnic_tools package requires [Libfabric](http://libfabric.org),
version 1.3 or higher, built with Cisco usNIC support.

## Building from a source tarball

If you are building from a usnic_tools tarball, first run `configure`,
and then build and install it:

```sh
$ tar xf usnic_tools-1.0.0.0.tar.bz2
$ cd usnic_tools-1.0.0.0

$ ./configure --prefix=/place/to/install
# ... lots of output ...

$ make
# ... lots of output ...

$ sudo make install
# ... lots of output ...
```

Optionally, you can specify the location of your Libfabric
installation via the `--with-libfabric` CLI option to `configure`:

```sh
$ ./configure --prefix=/place/to/install --with-libfabric=/libfabric/install/location
```

## Building from a git clone

Building from a git clone is just like building from a tarball, but
first you must have the GNU Autoconf, Automake, and Libtool tools
installed, and then run the `autogen.sh` script:

```sh
$ ./autogen.sh
# ... lots of output ...

# ...then follow the configure / make / make install procedure from
# building a source tarball.
```
