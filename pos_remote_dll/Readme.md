# Alika Point-Of-Sale Remote Service

The point-of-sale service runs as a Windows Service.  A desktop application can subcribe to service notifications via a 
WCF (Windows Communcation Foundation) named pipe. The code in this DLL can be used to implement the host service and client proxy.

## Learning References

- http://realfiction.net//2008/01/30/The-no-frills-bare-bones-example-to-Duplex-WCF/
- https://msdn.microsoft.com/en-us/library/ms734712.aspx
- https://www.codeproject.com/Articles/86007/ways-to-do-WCF-instance-management-Per-call-Per

## Building a Service Client

A service client can also be created in Visual Studio, via service metadata, by first starting the AlikaPosSerivce and then adding a service reference to 
a project using the string value of the RemoteFactory.PipeLocation ("net.pipe://localhost/AlikaPosService/Scanner") as the Service URL.