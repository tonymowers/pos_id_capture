# Alika Point-Of-Sale Console

This is a command line application which can be run to help debug the AlikaPosService.

It can be started in two modes:
- Proxy: subscribes to AlikaPosService Window Service and prints event information as the service processes scanned documents
- Local: runs standalone service which directly interacts with 3M Scanners and Web Service for delivering scans

To run the proxy mode start the console without any command line parameters. (AlikaPosService Windows Service must be running)

To run the local server start the console with any command line parameter. (AlikaPosService Windows Service must NOT be running)

 