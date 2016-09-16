% title: usnic_devinfo(1)
% tagline: Cisco usNIC

# NAME

usnic_devinfo \- Simple utility to list information about usnic interfaces

# SYNOPSYS
```
usnic_devinfo -h
usnic_devinfo [OPTIONS]
```

# DESCRIPTION

The usnic_devinfo utility can be used to query information about available
Cisco usNIC interfaces. The usnic_devinfo utility can filter based on device
name and can provide either a summary, or a detailed information listing about
each interface.

# OPTIONS

*-b*
: Instead of showing all information about an interface, show only a brief
  summary.

*-d*
: Specify the local device by name or number. usNIC interfaces are named
  following the pattern usnic_X where X is a non-negative integer. This option
  accepts either the full name or the number after the usnic_ prefix.

# USAGE EXAMPLES

```
$ usnic_devinfo -d usnic_0
```

Show information about the device named "usnic_0".

```
$ usnic_devinfo -d 0
```

This form is the same as the example above; it will display information about
the device named "usnic_0".

```
$ usnic_devinfo -d usnic_2 -b
```

Display brief information about the device named "usnic_2".

# OUTPUT

By default, the full information is displayed:

```
$ usnic_devinfo -d 0
usnic_0:
        Interface:               eth1
        MAC Address:             58:ac:78:f2:84:e8
        IP Address:              10.10.10.2
        Netmask:                 255.255.0.0
        Prefix len:              16
        MTU:                     9000
        Link State:              UP
        Bandwidth:               40 Gb/s
        Device ID:               UCSC-PCIE-C40Q-03 [VIC 1385] [0x014d]
        Vendor ID:               4407
        Vendor Part ID:          207
        Firmware:                4.1(1d)
        VFs:                     64
        CQ per VF:               6
        QP per VF:               6
        Interrupts per VF:       6
        Max CQ:                  384
        Max CQ Entries:          65535
        Max QP:                  384
        Max Send Credits:        4095
        Max Recv Credits:        4095
        Capabilities:
          Map per res:           yes
          PIO sends:             yes
          CQ interrupts:         no
```

```
$ usnic_devinfo -d 0 -b
 usnic_0:  eth1 58:ac:78:f2:84:e8     10.10.10.2/16   VIC 1385    4.1(1d)   UP
```
