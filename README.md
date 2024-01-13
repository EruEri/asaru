# Asaru

A little commmand line program to explore files inside your IPhone.

It's the rewrite of an older project, [iexplore](https://github.com/EruEri/iexplore)

## Build
You will need:
- C compiler compliant c99
- pkg-config
- The following C librarie:
  - [libimobiledevice](https://libimobiledevice.org/)
- GNU make
- Probably an Unix/Unix-like (MacOS/ Linux/ FreeBSD / ...):
    - I don't know if it will work on Windows.

And run:
```
$ git clone https://github.com/EruEri/asaru
$ make
```
### Linux
You will probably need to install and enable the service **usbmuxd**, otherwise your phone won't be detected while plugged in USB.