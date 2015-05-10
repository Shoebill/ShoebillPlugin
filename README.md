# ShoebillPlugin

![Build Status](http://ci.gtaun.net/buildStatus/icon?job=ShoebillPlugin)

# Download

The newest version can always be found here (always 32-Bit):
- Windows (0.3.7): [Download](http://catboy5.bplaced.net/updater/binaries/Shoebill.dll)
- Linux (0.3.7): [Download](http://catboy5.bplaced.net/updater/binaries/Shoebill)

# What is the Shoebill Plugin?

The shoebill-plugin is the heart of the shoebill project. It is directly used by the samp-server process. Internally,
it will create a Java Virtual Machine and it will launch the shoebill-launcher artifact. The plugin will invoke
callbacks in the shoebill java side, and respond to native function calls from shoebill-launcher.

# Version supported

The shoebill plugin currently supports version 0.3.7 of SAMP

# Used dependencies

- [libiconv](https://www.gnu.org/software/libiconv/)
- [jni](http://en.wikipedia.org/wiki/Java_Native_Interface) (jdk has the needed files included, just install jdk if you want to build the plugin)
- [cmake](http://cmake.org)

# How to build it?
(You need to make sure that cmake will use a 32-bit jdk, which it won't in my case. I always build my project on a 32-bit ubuntu machine)
Clone the project into a directory and then execute the build.sh script.
After the build has successfully finished, you will find a Shoebill.so file in the src directory. Rename it to just "Shoebill" (without .so)
and then you should be able to use it with the samp-server-
