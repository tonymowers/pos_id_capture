# Point-Of-Sale Hardware DLL (AlikaPosHardware)

This DLL provides functionality for interfacing the 3M CR100 SwipeReader.  The 3M CR100 is used for reading the MRZ (machine readable zone)
on IDs such as passports, licenses, and identity cards.

It's provides the code for connecting to the device and registering event handlers which are called whenever I document is scanned.

## Technical Details

The package using the LibLog logging abstraction.

## References

- 3M CR100 SDK can be downloaded, after registering with 3M, from www.3m.com/readersoftware 
- LibLog https://github.com/damianh/LibLog https://github.com/damianh/LibLog/wiki

