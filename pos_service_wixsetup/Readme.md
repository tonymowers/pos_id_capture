# Alika Point-Of-Sale Service Installer

This project builds an installer for the Alika Point-Of-Sale service.  See the Alika Point-Of-Sale readme for more details.

This installer is built using the WIX toolkit. To build this installer you will need to install the following tools on your machine in this order:
- WiX Toolset v3.11
- WiX Toolset Visual Studio 2010 Extension

The WIX toolkit can be downloaded from:
- http://wixtoolset.org

## Learning Resources

To learn about how to use the WIX installer I relied upon the following websites:
- https://developingsoftware.com/wix-toolset-install-windows-service/
- https://github.com/rstropek/Samples/tree/master/WiXSamples
- http://wixtoolset.org

## Creating Componet Lists

To create the numerous components needed to be listed in the WIX project I used the "heat.exe" tool, 
which comes packaged in the WIX toolset, to help create some examples.

## How To Install Necessary Drivers

The next step will be to build a bootstrap installer which installs the necessary device drivers before the service.s

- http://wixtoolset.org/documentation/manual/v3/bundle/
