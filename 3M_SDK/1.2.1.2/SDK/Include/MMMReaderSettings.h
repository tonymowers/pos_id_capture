#ifndef _MMMREADERSETTINGS_H_
#define _MMMREADERSETTINGS_H_

#include "MMMReaderDataTypes.h"

#ifndef MAX_PATH
#define MAX_PATH          260
#endif

#if !(defined(__linux__) && defined(__arm__))
#pragma pack(push, 1)
#endif

/// \cond PAGESDK

/// Image calibration settings struct.
typedef struct BrightnessSettings
{
	float gain;
	float exp;
	float rgain;
	float bgain;
	float gagain;
	float gbgain;
	float gamma;
	float scaleBrightness;
	int cmatrix; 
	int ledLevela;
	int ledLevelb;
	int puIlluminationBanks;
	MMMReaderAmbientRemovalMethod puAmbientRemoval;
	unsigned char puLightingProfile;
	unsigned char puHardwareWBG;
	MMMR_BOOL puUseAntiGlare;
	MMMR_BOOL puUseGammaInSnapshot;
	MMMR_BOOL puUseSequentialImaging;
} BRIGHTNESSSETTINGS;

/// Defines settings for images in the \pagereader.
typedef struct MMMReaderImageSettings
{
	char puImageName[32];
	char puImageBase[32];
	int puImageType;
	int puIllumination;
	int puQuality;
	BRIGHTNESSSETTINGS puBrightness;
	MMMR_BOOL puColour;
	MMMR_BOOL puSmall;
	MMMR_BOOL puNormalise;
	MMMR_BOOL puCrop;
	MMMR_BOOL puRotate;
	MMMR_BOOL puDebarrel;
	MMMR_BOOL puSharpen;
	MMMR_BOOL puScaleFullGrey;
	MMMR_BOOL puGamma;
	MMMR_BOOL puContrast;
	MMMR_BOOL puColourHue;
	MMMR_BOOL puEdgeEnhance;
	MMMR_BOOL puApplyColourMatrix;
} MMMReaderImageSettings;

/// Defines the calibration settings of the \pagereader camera.
///
/// \ingroup CameraModule CameraSettingsModule HLSettingsModule LLSettingsModule
///
/// \note This structure is only relevant if using a \pagereader.
///
/// \remarks Most of these settings are loaded from the \pagereader itself to specify how image
/// snapshots should be taken. The settings that are loaded from the \appconfig file are:
///		- #CAMERASET::uvstop
///		- #CAMERASET::uvstart
///		- #CAMERASET::uvonenable
///		- #CAMERASET::uvDisableTimeOff
///
/// \attention These calibration settings are not meant to be programatically modified
/// unless it is necessary to change the appearance of the images. Changes to these values may 
/// prevent the \pagereader from reading documents correctly. The only data members that can 
/// be modified safely are the ones loaded from the \appconfig file, listed above.
///
/// \sa #MMMReaderSettings::puCameraSettings
typedef struct CameraSettings
{
	/// Calibration settings for ultra-violet (UV) images.
	BRIGHTNESSSETTINGS uv;
	
	/// Calibration settings for infra-red (IR) images.
	BRIGHTNESSSETTINGS ir;
	
	/// Calibration settings for document detection images.
	BRIGHTNESSSETTINGS irlow;
	
	BRIGHTNESSSETTINGS red;
	
	BRIGHTNESSSETTINGS green;
	
	BRIGHTNESSSETTINGS blue;
	
	/// Calibration settings for 3M Confirm images.
	BRIGHTNESSSETTINGS reserved1;
	
	/// Calibration settings for 3M Confirm tamper images.
	BRIGHTNESSSETTINGS reserved2;
	
	/// Calibration settings for visible images.
	BRIGHTNESSSETTINGS vis;
	
	/// Calibration settings for IR left only illumination settings for the AT9000 Mk 2
	BRIGHTNESSSETTINGS irLeft;

	/// Calibration settings for IR right only illumination settings for the AT9000 Mk 2
	BRIGHTNESSSETTINGS irRight;

	/// Calibration settings for Vis left only illumination settings for the AT9000 Mk 2
	BRIGHTNESSSETTINGS visLeft;

	/// Calibration settings for Vis right only illumination settings for the AT9000 Mk 2
	BRIGHTNESSSETTINGS visRight;

	/// Calibration settings used for the "OVD1" image type - typically only used for testing images
	/// in the RTE8000. Obsoleted with the new OVD suppression in newer readers.
	BRIGHTNESSSETTINGS OVD1;
	
	/// Calibration settings used for the "OVD1" image type - typically only used for testing images
	/// in the RTE8000. Obsoleted with the new OVD suppression in newer readers.
	BRIGHTNESSSETTINGS OVD2;
	
	BRIGHTNESSSETTINGS ocrred;
	
	/// Calibration settings for barcode images.
	BRIGHTNESSSETTINGS barcode;
	
	/// Provides the serial number of the camera in the \pagereader.
	///
	/// \attention This member is provided for information purposes only. Do not modify.
	int serialNumber;
	
	/// Provides the image width of the \pagereader camera.
	///
	/// \attention This member is provided for information purposes only. Do not modify.
	int width;
	
	/// Provides the image height of the \pagereader camera.
	///
	/// \attention This member is provided for information purposes only. Do not modify.
	int height;
	
	int visible;
	
	MMMR_BOOL colour;
	
	MMMR_BOOL optiocr;
	
	MMMR_BOOL cropImages;
	
	/// Provides the \pagereader hood style used.
	///
	/// \attention This member is provided for information purposes only. Do not modify.
	int flap;
	
	/// Provides the time that the \sdk should wait after inactivity before switching off
	/// the UV tubes into life-saving mode.
	///
	/// \note This setting is only relevant for \pagereaders which contain UV tubes.
	///
	/// \remarks The time to wait is measured in seconds.
	///
	/// \sa #uvstart
	int uvstop;
	
	/// Provides the time that the \sdk should wait before taking a UV image after the
	/// UV tubes turn back on.
	///
	/// \note This setting is only relevant for \pagereaders which contain UV tubes.
	///
	/// \remarks The time to wait is measured in seconds.
	///
	/// \sa #uvstop
	int uvstart;
	
	/// Determines whether the \sdk should turn off UV when in a #READER_DISABLED state.
	MMMR_BOOL uvonenable;
	
	/// Provides the time that the \sdk should wait before turning off UV when going into
	/// a #READER_DISABLED state.
	///
	/// \note This setting is only relevant when #uvonenable is \c TRUE.
	///
	/// \remarks The time to wait is measured in seconds.
	int uvDisableTimeOff;
	
	MMMR_BOOL zeroBlack;

	/// Provides the serial number of the scanner.
	///
	/// \attention This member is provided for information purposes only. Do not modify.
	char scannerSerial[MAX_PATH];
	
	MMMR_BOOL puUseTrueColour;

	MMMR_BOOL puSplitImage;

	MMMR_BOOL puNeedsDebarrel;

	/// Camera horizontal dpi 
	int xDPI;

	/// Camera vertical dpi 
	int yDPI;

	/// Provides the capture image width (pixels) of the \pagereader camera.
	///
	/// \attention This member is provided for information purposes only. Do not modify.
	int widthCapture;
	
	/// Provides the capture image height (pixels) of the \pagereader camera.
	///
	/// \attention This member is provided for information purposes only. Do not modify.
	int heightCapture;

	/// Provides the capture horizontal offset (pixels) for the image capture of the \pagereader camera.
	///
	/// \attention This member is provided for information purposes only. Do not modify.
	int horizontalOffset;
	
	/// Provides the capture vertical offset (pixels) for the image capture of the \pagereader camera.
	///
	/// \attention This member is provided for information purposes only. Do not modify.
	int verticalOffset;

	/// Camera Scale Factor  1 to 100 % scaling
	int scaleFactor;


} CAMERASET;

/// Defines the \sdk RF connection settings.
///
/// \ingroup RfidModule RfidSettingsModule HLSettingsModule LLSettingsModule
///
/// \remarks These settings are loaded from the \appconfig file.
///
/// \sa RFIDSettings
typedef struct RFInitSettings
{
	/// Provides the virtual COM port number to the RFID module.
	///
	/// \remarks The \sdk will auto-detect the correct COM port number to use and update this
	/// member when necessary.
    int puCOMPort;
    
    /// Provides the maximum time to wait for a response from the RFID chip.
    ///
    /// \remarks This value is measured in milliseconds (ms).
    ///
    /// \remarks A value in the range 2000 - 10000 is recommended.
    int puCOMTimeoutMilliseconds;

	/// Allows the behaviour of SCardDisconnect calls to be overriden. The default
	/// value of -1 causes the SDK to use it's preferred behaviour for a given reader,
	/// however if set to 0 or higher, this value is what is used when calling 
	/// SCardDisconnect, and so may be used as an override.
	int puDisconnectBehaviour;

	/// Indicates whether the RF antenna should be powered off between reads. This has
	/// an impact on the time it takes to read an RF document (turning it back on may
	/// slow things down), but may allow you to fix some issues with flaky documents on
	/// a re-read.
	MMMR_BOOL puPowerOffAntennaBetweenReads;
} RFInitSettings;

/// Defines the \sdk RF processing settings.
///
/// \ingroup RfidModule RfidSettingsModule HLSettingsModule LLSettingsModule
///
/// \remarks These settings are loaded from the \appconfig file.
///
/// \sa RFIDSettings
typedef struct RFProcessSettings
{
	/// Determines the order in which to check the RF antennas in the \pagereader. See
	/// #ANTENNA_MODE for an explanation of the available modes.
    ANTENNA_MODE puAntennaMode;
    
    /// Provides the maximum number of attempts to try when communicating with the RFID chip.
    ///
    /// \remarks Any individual command/response between the RFID chip and the \pagereader
    /// could potentially be corrupted. This is fairly common with large Data Groups, so the
    /// \sdk implements an automatic APDU retry mechanism. Any single APDU will be tried
    /// up to #puMaxAPDUAttempts times.
    ///
    /// \remarks Establishing Basic Access Control (BAC) on a 6982 error code is incorporated
    /// into this retry mechanism, so this value should be at least 2.
    int puMaxAPDUAttempts;
    
    /// Provides the default over-aid baud rate to use for RFID chip communication.
    ///
    /// \remarks If the RFID chip does not support the default baud rate, a lower one will be
    /// selected. However, some early e-Passports do not execute this baud rate lowering
    /// mechanism correctly, so often a default of 424 is recommended.
    int puDefaultChipBaudRate;
    
    /// Provides the default buffer size to request from the RFID chip on every Read Binary 
    /// APDU.
    int puReadBinaryBufferSize;
    
    /// Determines whether the \sdk will attempt to read the EF.ATR file on the RFID chip
    /// to determine the maxmimum Read Binary buffer size to optimise reading times of RF Data
    /// Groups.
    ///
    /// \remarks If no EF.ATR is present, the default #puReadBinaryBufferSize will be used
    /// instead.
    bool puReadBinaryUseEFATRIfPresent;
    
    /// Specifies how to load Document Signer Certificates (DSCs). See #DOC_SIGNER_CERT_MODE
    /// for an explanation of the available modes.
    DOC_SIGNER_CERT_MODE puDocSignerCertMode;
    
    /// Specifies how to retrieve Document Signer Certificates (DSCs). See #EXTERNAL_CERT_MODE
    /// for an explanation of the available modes.
    EXTERNAL_CERT_MODE puExternalDSCMode;
    
    /// Specifies how to retrieve Country Signer Certificates (CSCs). See #EXTERNAL_CERT_MODE
    /// for an explanation of the available modes.
    EXTERNAL_CERT_MODE puExternalCSCMode;
    
    /// Determines whether to check Document Signer Certificates (DSCs) for revocation against
    /// one or more Certificate Revocation Lists (CRLs).
    ///
    /// \sa #puDSC_CRLMode
    bool puCheckDSCRevocation;
    
    /// Specify how to retrieve Certificate Revocation Lists (CRLs) for Document Signer 
    /// Certificates (DSCs). See #EXTERNAL_CERT_MODE for an explanation of the available modes.
    ///
    /// \note This setting is only relevant if #puCheckDSCRevocation is \c true.
    EXTERNAL_CERT_MODE puDSC_CRLMode;
    
    /// Determines whether to check Country Signer Certificates (DSCs) for revocation against
    /// one or more Certificate Revocation Lists (CRLs).
    ///
    /// \sa #puCSC_CRLMode
    bool puCheckCSCRevocation;
    
    /// Specify how to retrieve Certificate Revocation Lists (CRLs) for Country Signer 
    /// Certificates (DSCs). See #EXTERNAL_CERT_MODE for an explanation of the available modes.
    ///
    /// \note This setting is only relevant if #puCheckCSCRevocation is \c true.
    EXTERNAL_CERT_MODE puCSC_CRLMode;
    
    /// Provides a folder location on the host machine where one or more certificates are 
    /// stored.
    ///
    /// \sa #EXTERNAL_CERT_MODE::ECM_CERT_FILE_STORE
    char puCertsDir[MAX_PATH];
    
    /// Determines whether the \sdk will search recursively through sub-folders in #puCertsDir 
    /// for certificates.
    ///
    /// \sa #EXTERNAL_CERT_MODE::ECM_CERT_FILE_STORE
    bool puCertsIncludeSubDirs;
    
    /// Provides a list of certificate file extensions to search for when searching #puCertsDir
    /// for certificates.
    ///
    /// \remarks This value allows only specified certificate types to be loaded.
    ///
    /// \remarks The format of this value should be each file extension (minus the period
    /// character) separated by semi-colons, e.g. \c "cer;der;pem;crt;crl"
    ///
    /// \sa #EXTERNAL_CERT_MODE::ECM_CERT_FILE_STORE
    char puCertFileExtensions[256];
    
    /// Determines whether to select the LDS application on an RFID chip.
    ///
    /// \remarks Typically a \pagereader is used to read from an RFID chip. However, it is
    /// also possible to write to an RFID chip. In this case, the LDS application will not be
    /// present on the chip, so it is necessary to disable automatic selection of the LDS
    /// application to avoid APDU errors.
    bool puSelectLDSApplication;
    
    /// Determines how long the SDK should wait for to open an RFID chip before giving up.
	int puOpenChipTimeout;

	/// Determines whether Extended Access Control (EAC) features are switched on in an
	/// EAC-enabled version of the \sdk.
    bool puEACEnabled;

	/// Determines whether Supplemental Access Control (SAC) features are switched on in an
	/// EAC-enabled version of the \sdk.
	bool puSACEnabled;
    
    /// \eac Specifies how to retrieve Country Verifying Certification Authority (CVCA)
    /// certificates. See #EXTERNAL_CERT_MODE for an explanation of the available modes.
    ///
    /// \note This setting is only relevant when #puEACEnabled is \c true.
    int puExternalCVCertsMode;
    
    /// \eac Specifies how to retrieve Inspection System (IS) private keys. See 
    /// #EXTERNAL_CERT_MODE for an explanation of the available modes.
    ///
    /// \note This setting is only relevant when #puEACEnabled is \c true.
    int puExternalPrivateKeyMode;
    
    /// \eac Specifies the Extended Access Control (EAC) specification to work against.
    ///
    /// \note This setting is only relevant when #puEACEnabled is \c true.
    ///
    /// \remarks The following values are acceptable:
    ///	<table>
    ///	<tr><th>Value</th><th>EAC Version</th></tr>
    ///
    /// <tr><td>1</td><td>1.0</td></tr>
    ///
    /// <tr><td>2</td><td>1.01</td></tr>
    ///
    /// </table>
    int puEACVersion;
    
} RFProcessSettings;

/// Defines the RF settings the \sdk should use.
///
/// \ingroup RfidModule RfidSettingsModule HLSettingsModule LLSettingsModule
///
/// \remarks These settings are loaded from the \appconfig file.
///
/// \sa #MMMReaderSettings::puRFIDSettings
typedef struct RFIDSettings
{
	/// Defines all the RF initialisation settings.
    RFInitSettings puRFInitSettings;
    
    /// Defines all the RF processing settings.
    RFProcessSettings puRFProcessSettings;

} RFIDSettings;

/// Defines which RF data items the \sdk should return to the host application.
///
/// \ingroup HighLevelSettingsModule HLSettingsModule LLSettingsModule
///
/// \note This structure is only relevant if using the \hlapi.
///
/// \remarks This structure specifies both which data items to return and in what order.
/// Setting a data member to 0 will disable/not return that data item, whereas setting a 
/// value greater than zero will enable and define the numbered order to read RF data items.
/// If the order is not important, all settings can be set to 1.
///
/// \sa DATASENDSET
typedef struct RFIDDataToSend
{
	/// Determines whether to return the raw EF.COM file data.	
    int puEFComFile;
    
    /// Determines whether to return the raw EF.SOD file data.
    int puEFSodFile;

	/// Determines whether to return the individual Data Group files.
	///
	/// \remarks This setting is an array of enabled flags. The array uses a non-zero-based,
	/// meaning that accessing a Data Group requires the same value. For example, DG1 is
	/// accessed by #puDGFile[1], DG2 is accessed by #puDGFile[2], and so on.
    int puDGFile[17];
    
    /// Determines whether to return the decoded DG1 data, which represents an ICAO codeline.
    int puDG1MRZData;
    
    /// Determines whether to return the decoded DG2 data, which represents a photo image.
    int puDG2FaceJPEG;
    
    /// \eac Determines whether to return the decoded DG3 data, which represents biometric
    /// fingerprint data.
	int puDG3Fingerprints;

    /// Determines whether to validate the individual Data Group files.
    ///
    /// \remarks This setting is an array of enabled flags. The array uses a non-zero-based,
	/// meaning that accessing a Data Group requires the same value. For example, DG1 is
	/// accessed by #puDGFile[1], DG2 is accessed by #puDGFile[2], and so on.
    int puValidateDG[17];
    
    /// Determines whether to validate the signed attributes.
    int puValidateSignedAttrs;
    
    /// Determines whether to validate the signature.
    int puValidateSignature;
    
    /// Determines whether to return the RF chip ID.
    int puChipID;
    
    /// Determines whether to return the over-aid baud rate used for RFID chip communication.
    int puAirBaudRate;
    
    /// Determines whether to perform Basic Access Control (BAC).
    int puGetBACStatus;
    
    /// Determines whether to perform Active Authentication.
    int puActiveAuthentication;
    
    /// Determines whether to validate the Document Signer Certificate (DSC).
	int puValidateDocSignerCert;
	
	/// \eac Determines whether to perform Terminal Authentication.
	int puGetTerminalAuthStatus;
	
	/// \eac Determines whether to perform Chip Authentication.
	int puGetChipAuthStatus;
	
	/// \eac Determines whether to return the Country Verifying Certification Authority (CVCA)
	/// file.
	int puEFCVCAFile;

	/// \eac Determines whether to return the result of crosschecking EF.COM and EF.SOD
	int puCrosscheckEFComEFSod;

	/// Determines whether to perform Passive Authentication;
	int puPassiveAuthentication;

	/// Determines whether to return the raw EF.CardAccess file data.	
	int puEFCardAccessFile;

	/// Determines whether to return the status of Supplemental Access Control (SAC).
	/// Note that SAC will always be performed if present and puSACEnabled is set.
	int puGetSACStatus;

	/// Determines whether to return a 2 byte bit map of data groups (DG1-DG16) present in the EF.COM.
	int puEFComDGMap;

	/// Determines whether to return a 2 byte bit map of data group hashes (DG1-DG16) present in the EF.SOD.
	int puEFSodHashMap;

	/// Determines whether to return the Document Signer Certificate in the EF.SOD, if present.
	int puGetDocSignerCert;

} RFIDDataToSend;

/// Defines which UHF data items the \sdk should return to the host application.
///
/// \ingroup HighLevelSettingsModule HLSettingsModule LLSettingsModule
///
/// \note This structure is only relevant if using the \hlapi.
///
/// \remarks This structure specifies both which data items to return and in what order.
/// Setting a data member to 0 will disable/not return that data item, whereas setting a 
/// value greater than zero will enable and define the numbered order to read UHF data items.
/// If the order is not important, all settings can be set to 1.
///
/// \sa DATASENDSET
typedef struct UHFDataToSend
{
	/// Determines whether to return the TagID data.	
    int puTagID;

	/// Determines whether to return the EPC (Electronic Product Code) data.
	int puEPC;

	/// Determines whether to return all the memory from the tag, as specified
	/// in the UHFReadMap.xml file.
	int puMemoryMap;

} UHFDataToSend;

/// If puDataToSend.send has this bit set, the High Level SDK will attempt to read a codeline
/// from documents.
///
/// \remarks Any codeline read will be returned as #CD_CODELINE will be returned through 
/// the #MMMReaderHLDataCallback.
#define SSCODELINE		0x000001

/// If puDataToSend.send has this bit set, the High Level SDK will capture an image in
/// visible light. If puDataToSend.special has this bit set, the image will be in colour.
/// If the puDataToSend.special bit is not set the image will be greyscale.
///
/// \remarks The image captured will be returned as #CD_IMAGEVIS will be returned through 
/// the #MMMReaderHLDataCallback.
#define SSVISIBLEIMAGE	0x000002

/// If puDataToSend.send has this bit set, the High Level SDK will capture an image in IR
/// light. If puDataToSend.special has this bit set, the image will be cropped to the codeline.
/// If the puDataToSend.special is not set, the IR image will only crop to the document.
#define SSIRIMAGE		0x000004

/// If puDataToSend.send has this bit set, the High Level SDK will capture an image in UV light.
/// If puDataToSend.special has this bit set, the image will also have ambient subtraction applied.
/// If puDataToSend.special is not set, the UV image will not apply ambient subtraction.
#define SSUVIMAGE		0x000008

#define SSRESERVED1		SSCOAXIMAGE

/// If puDataToSend.send has this bit set, the High Level SDK will capture a visible light image 
/// using the coaxial filter.
#define SSCOAXIMAGE		0x000010

/// If puDataToSend.send has this bit set, the High Level SDK will capture a visible image
/// and crop to the area of an expected photo image.
#define SSPHOTOIMAGE	0x000020

/// If puDataToSend.send has this bit set, the High Level SDK will calculate the checksums
// from an ICAO compliant codeline.
#define SSCHECKSUM		0x000080

#define SSRESERVED2		SSCOAXIRIMAGE

/// If puDataToSend.send has this bit set, the High Level SDK will capture an IR light image 
/// using the coaxial filter.
#define SSCOAXIRIMAGE	0x000200

/// If puDataToSend.send has this bit set, the High Level SDK will perform QA measurements.
#define SSQA			0x000400

/// If puDataToSend.send has this bit set, the High Level SDK will capture a special UV image
/// to give an indication as to whether the document has been reprinted on standard paper.
#define SSSECURITYCHECK	0x000800

/// If puDataToSend.send has this bit set, the High Level SDK will capture data using a smart
/// card device.
#define SSSMARTCARD		0x002000

/// If puDataToSend.send has this bit set, events will be sent for the start and end of document
/// data.
#define SSDOCMARKERS	0x004000

/// Only applies to RTE8000
#define	SSOVDIMAGE1		0x008000

/// Only applies to RTE8000
#define	SSOVDIMAGE2		0x010000

/// If puDataToSend.send has this bit set, the High Level SDK will capture a special visible image 
/// used for barcode decoding.
#define SSBARCODEIMAGE	0x040000

/// If puDataToSend.send has this bit set, the High Level SDK will capture data from a \SwipeReader device.
#define SSSWIPE			0x080000

/// If puDataToSend.send has this bit set, the High Level SDK will attempt to decode barcode and MSR data
/// using the AAMVA decoder.
#define SSAAMVA			0x100000

/// If puDataToSend.send has this bit set, the High Level SDK will calculate the checksums
/// from an ICAO compliant codeline and return an extended struct.
#define SSCHECKSUMEXTENDED	0x200000

/// If puDataToSend.send has this bit set, the High Level SDK will capture data from a UHF device.
#define SSUHF			0x400000

/// If puDataToSend.send has this bit set, the High Level SDK will capture data from a Mag Stripe Reader device.
#define SSMAGDATA		0x800000

//#ifdef DCA
/// controls  PT_TextZone, PT_SubImage, PT_Anchor, PT_Barcode, PT_MagSwipe, PT_RF, PT_UHF, PT_Plugables,  PT_IcaoMrz, PT_Custom
#define SSDCAINPSECTION		0x000001

/// controls  PT_PatternMatch, PT_BlankArea, PT_Confirm, PT_Tamper, PT_TextCompare
#define SSDCAAUTHENTICATION	0x000004

/// controls  CD_IDENTIFIED_RESULT
#define SSDCAIDENTIFYRESULT	0x000008

/// controls  CD_COMPLETION_RESULT
#define SSDCACOMPLETIONRESULT	0x000010

/// controls  CD_VERIFIER_RESULT
#define SSDCAVERIFIERRESULT	0x000020


//#endif //DCA
/// Defines which data items the \sdk should return to the host application.
///
/// \ingroup HighLevelSettingsModule HLSettingsModule LLSettingsModule
///
/// \note This structure is only relevant if using the \hlapi.
///
/// \sa #MMMReaderSettings::puDataToSend
typedef struct DataSendSet
{
	/// Provides a bitwise mask of all the data items to return.
	///
	/// \remarks The bits present in this data member signify whether certain data items
	/// are enabled. For example:
	///
	/// \remarks \code
	///	puDataToSend.send = SSCODELINE | SSSMARTCARD | SSVISIBLEIMAGE | SSIRIMAGE;
	/// \endcode
	///
	/// \remarks The above demonstrates returning the codeline data, RF data items, visible
	/// image and IR image respectively.
	///
	/// \remarks Some values also have a "special" mode to add to the capabilities of some
	/// data items. See #special for more details.
	///
	/// \remarks The following bitwise flags can be used as part of this bitwise mask. See
	/// the documentation for these constants for more details:
	///		- #SSCODELINE
	///		- #SSVISIBLEIMAGE	
	///		- #SSIRIMAGE		
	///		- #SSUVIMAGE		
	///		- #SSPHOTOIMAGE
	///		- #SSSAVEDONTSEN
	///		- #SSCHECKSUM
	///		- #SSSECURITYCHECK
	///		- #SSSMARTCARD
	///		- #SSDOCMARKERS
	///		- #SSOVDIMAGE1
	///		- #SSOVDIMAGE2
	///		- #SSBARCODEIMAGE
	///		- #SSSWIPE
	///		- #SSAAMVA
	///		- #SSCHECKSUMEXTENDED
	///		- #SSUHF
	///
	int send;
	
	/// Provides a bitwise mask of certain "special" modes for data items set in #send.
	///
	/// \remarks The bits present in this data member signify whether certain "special" modes
	/// for data items are enabled. For example:
	///
	/// \remarks \code
	///	puDataToSend.special = SSVISIBLEIMAGE | SSIRIMAGE;
	/// \endcode
	///
	/// \remarks The above demonstrates returning the visible image in colour and the IR image 
	/// cropped to the codeline respectively.
	///
	/// \remarks The following bitwise flags can be used as part of this bitwise mask. See
	/// the documentation for these constants for more details:
	///		- #SSVISIBLEIMAGE	
	///		- #SSIRIMAGE		
	///		- #SSUVIMAGE		
	///		- #SSPHOTOIMAGE
	///		- #SSSECURITYCHECK
	///		- #SSDOCMARKERS
	///		- #SSOVDIMAGE1
	///		- #SSOVDIMAGE2
	///		- #SSBARCODEIMAGE
	///
	int special;
	
	/// Provides a bitwise mask for certain rear image types.
	///
	/// \remarks The bits present in this data member signify whether certain data items
	/// are enabled similiar to send member
	///
	/// \remarks The following bitwise flags can be used as part of this bitwise mask. See
	/// the documentation for these constants for more details:
	///		- #SSVISIBLEIMAGE	
	///		- #SSIRIMAGE		
	///		- #SSUVIMAGE		
	///		- #SSBARCODEIMAGE
	///
	int sendRear;
	
	/// Determines the compression rate to use for exported JPEG images.
	///
	/// \remarks This value should be in the range 0 - 100; 0 represents no compression, and
	/// 100 represents the maximum compression.
	int compression;
	
	/// Specifies the size of the exported image as a percentage of the original.
	float size;
	
	char IRFname[MAX_PATH];
	char UVFname[MAX_PATH];
	char VisFname[MAX_PATH];
	char PhotoFname[MAX_PATH];
	char Reserved1Fname[MAX_PATH];
	char Reserved2Fname[MAX_PATH];
	
	MMMR_BOOL sendSerial;
	MMMR_BOOL sendSockets;
	int sendWaitTime;

	MMMR_BOOL puTrueColourUV;
	
	/// \deprecated Use #startupState and #autoDisableAfterRead instead.
	///
	/// Specify what #ReaderState the \pagereader should start in.
	///
	/// \remarks This is a bitwise value and the following values can be bitwise OR'd
	/// together.
	/// <table>
	///	<tr><th>Value</th><th>Description</th></tr>
	///
	/// <tr><td>0</td><td>Start the \pagereader in a #READER_DISABLED state.</td></tr>
	///
	/// <tr><td>1</td><td>Start the \pagereader in a #READER_ENABLED state.</td></tr>
	///
	/// <tr><td>2</td><td>Set the \pagereader state to #READER_DISABLED after every 
	/// document read.</td></tr>
	///
	/// </table>
	///
	/// \remarks For example, setting \code enable = 3; // 1 | 2 \endcode states that 
	/// the \pagereader will initially start in a #READER_ENABLED state, but once a document
	/// is read it will auto-disable.
	int enabled;

	/// Specify which #ReaderState the \pagereader should start in.
	///
	/// \remarks This setting overrides the old deprecated member #enabled
	ReaderState startupState;

	/// Set the \pagereader state to #READER_DISABLED after every document read
	MMMR_BOOL autoDisableAfterRead;

	/// Specifies what RFID data items to return.
	///
	/// \note These settings are only relevant when the #SSSMARTCARD flag is included in
	/// #send.
	///
	/// \sa RFIDDataToSend
	RFIDDataToSend rfid;
	
	/// Determines whether to notify the host application of the progress of the current
	/// document read.
	///
	/// \remarks If set to \c TRUE, the data item #CD_READ_PROGRESS will be returned through 
	/// the #MMMReaderHLDataCallback.
	MMMR_BOOL progress;
	
	/// Specify what format images should be returned to the host application.
	MMMReaderImageFormats imageFormat;

	/// Specifiy what UHF data to read from a tag.
	///
	/// \note These settings are only relevant when the #SSUHF flag is included in
	/// #send.
	///
	/// \sa UHFDataToSend
	UHFDataToSend uhf;
	
	/// Bitwise mask to specify options for determining document orientation using MMMReaderAutoOrientation;
	///
	/// \sa MMMReaderAutoOrientation
	int autoOrientation;

//#ifdef DCA
	/// Provides a bitwise mask of all the dca data items to return.
	///
	/// \remarks The bits present in this data member signify whether certain dca data items
	/// are enabled. For example:
	///
	/// \remarks \code
	///	puDataToSend.send = SSDCAINPSECTION | SSDCAIDENTIFYRESULT;
	/// \endcode
	///
	/// \remarks The above demonstrates returning the inspection data, validation data and
	/// identify result respectively.
	///
	/// \remarks The following bitwise flags can be used as part of this bitwise mask. See
	/// the documentation for these constants for more details:
	///		- #SSDCAINPSECTION
	///		- #SSDCAAUTHENTICATION		
	///		- #SSDCAIDENTIFYRESULT		
	///		- #SSDCACOMPLETIONRESULT
	///
	/// \todo Check these are correct
	int sendDCA;
//#endif //DCA
} DATASENDSET;


/// Defines the area within which a document may be positioned.
typedef struct DocPos
{
	int dtype;					// 0, 44 36 or 30
	int height;
	int width;
	int bottom;
	int left;
	int rgt30;
	int rgt44;
	int rgt36;
	int top30;
	int top44;
	int top36;
	float angle;
} DOCPOS;



/// Defines the document detection calibration settings of the \pagereader.
///
/// \ingroup CameraModule DocDetectSettingsModule HLSettingsModule LLSettingsModule
///
/// \note This structure is only relevant if using a \pagereader.
///
/// \remarks Most of these settings are loaded from the \pagereader itself to specify how 
/// documents should be detected by the \sdk. The settings that are loaded from the \appconfig 
/// file are:
///		- #DOCDETECTSETTINGS::findDarkObjects
///		- #DOCDETECTSETTINGS::flashIllum
///		- #DOCDETECTSETTINGS::detectDuringRF
///
/// \attention These calibration settings are not meant to be programatically modified
/// unless it is necessary to change the appearance of the images. Changes to these values may 
/// prevent the \pagereader from reading documents correctly. The only data members that can 
/// be modified safely are the ones loaded from the \appconfig file, listed above.
///
/// \sa #MMMReaderSettings::puDocDetectSettings
typedef struct DocDetectSettings
{
	/// \deprecated Use #boxes instead.
	Box box;
	
	/// \deprecated Use #boxes instead.
	Box boxb;
	
	/// \deprecated Use #boxes instead.
	Box boxc;
	
	/// \deprecated Use #boxes instead.
	Box boxd;

	/// Detection Insert Box 
	Box boxDetectInsert;

	/// Detection Removal Box
	Box boxDetectRemove;
	
	/// Provides an array of document detection regions to use.
	Box boxes[20];
	
	/// Specifies the number of document detection #boxes in use.
	int numBoxes;
	
	/// DocDetect "boxes" filled box count threshold.
	int boxesRequired;
	
	/// Determines the colour of light to flash during document detection.
	///
	/// \remarks This is a visual aid to locate the \pagereader only.
	///
	/// \remarks The values 1, 2 and 4 represents red, green and blue respectively. The
	/// bitwise OR operator is used to turn on multiple lights, e.g. 7 represents all three
	/// lights.
	int flashIllum;
	
	/// DocDetect "boxes" threshold
	///
	/// \remarks Setting this value too low will cause false document detect triggers
	int boxesWhiteThresh;
	
	/// Specifies the delay between taking successive document detect images when there
	/// is not document on the \pagereader window..
	///
	/// \remarks This value will affect the responsiveness of document detection. The longer
	/// the delay, the less responsive it will become, but this will also reduce the CPU
	/// load.
	int delay;
	
	/// \internal
	int bottom;
	
	/// \internal
	int left;

	/// Determines whether the standard top of document is used or if we should search for
	/// the top of the document.
	///
	/// \sa #locateTop
	MMMR_BOOL locateTop;
	
	/// DocDetect "Active Video" threshold.
	///
	/// \remarks Setting this value too low will cause false document detect triggers
	int activeVideoWhiteThresh;

	/// Specifies the intial value for percentage of edge pixels that change across
	/// an image between snapshots to indicate movement when dark objects are turned on
	int darkMovementThreshInitial;

	/// Specifies the percentage increase in moving edge pixels to go from no movement to movement.
	/// e.g. if 20% of edges are moving as standard and this value is 40 then 60% of edges need to move
	/// for movement to trigger
	int darkMovementThreshIncrease;

	/// The number of cycles to look back on to set the ambient edge moving over.
	/// this needs to be longer than documents will be moving for, but short enough to be dynamic
	// if this is set to 0 then the threshold just stays at darkMovementThreshInitial 
	int darkMovementHistoryCycles;

	/// Amount of pixel greylevels the edge has to change before it's counted as the edge has moved
	/// default = 15 pixel levels
	int darkMovementPixelChange;

	/// Specifies the number of pixels that change from black to white (or vice versa) across
	/// an image between snapshots to indicate movement.
	int movementThresh;
	
	/// Specifies the number of pixels that must be "white" across an image to indicate the
	/// presence of a document.
	///
	/// \sa #docDetectThresh
	int activeVideoPixelCountThresh;
	
	/// Determines whether the \sdk will attempt to find "dark objects" not usually picked up
	/// by document detection.
	///
	/// \remarks "Dark objects" are defined as anything which include a dark region around the
	/// area of interest. An example of this are mobile phones which have images of barcodes.
	MMMR_BOOL findDarkObjects;

	/// When a mobile phone is removed from the scanner the dark object count decreases and
	/// this value is the amount it decreases by before we take notice
	///
	/// \remarks "Dark objects" are defined as anything which include a dark region around the
	/// area of interest. An example of this are mobile phones which have images of barcodes.
	int darkObjectsDrop;

	/// The max number of dark objects before we say there is a phone present. This is just the
	/// starting value, it changes automatically internally. If this value is zero then it will
	/// start dynamically as the present number of objects + darkObjectsDrop
	///
	/// \remarks "Dark objects" are defined as anything which include a dark region around the
	/// area of interest. An example of this are mobile phones which have images of barcodes.
	int darkObjectsMax;
	
	/// Determines whether the \sdk will detect document movement during an RF read.
	///
	/// \remarks Generally, this feature requires more resources as more work is being done
	/// in background. However, it also allows the ability to abort the current RF read if it
	/// has been detected that a document has been removed. See the MMMReader_RFAbort() API
	/// for more details.
	MMMR_BOOL detectDuringRF;

	/// Distance in millimeters from the back of document window, where the activeReader detection will 
	/// check for a document. 
	int activeReaderBottomOffset;
	
	/// Instead of using the IR Detect illumination for document edge location, use the first image captured. 
	///   The Locate function will get called by the first PostPorcessing call 
	MMMR_BOOL useFirstImageCapturedForLocate;

	/// Use IR image for document detection.
	MMMR_BOOL useIRForDetect;
	
	/// Use binned image for document detection.  Used on certain larger mega pixel cameras for faster detection.
	MMMR_BOOL useBinnedForDetect;

} DOCDETECTSETTINGS;

/// \internal Defines settings used for OCR of a document's codeline.
typedef struct OCRSettings
{
	int minCWidth;
	int minCHeight;
	int maxCHeight;
	int maxUnknownChars;

	int maxDensity;
	int minDensity;
	int maxBrokenChars;

	int fxEnd;
	int fxTD1;
	int fx44;
	int fx36;
	int edgeTol;
	int startedgeTol;
	int endedgeTol;
	int fTD1;
	int f44;
	int f36;

	int searchHeight;
	int optCentreDist;
	int maxCentreDist;
	int minCentreDist;
	int noiseCentreDist;
	int deskew;

	float qa_pixelsize;
	float qa_yoffset;
	
	CODELINECONTEXT rule;
//  char doQA;

	/// Determines whether to read documents upside-down.
	int upsideDown;

} OCRSET;


/// Settings for dirtdetection on the scanner glass
typedef struct DirtDetectSet
{
	int windowx; // 15				// default number of windows in x direction
	int windowy; // 10				// default number of windows in y direction
	int history; // 5				// num scans to collect stats over
	int minbadcharcount;  // 5	// num of bad chars over which we don't include in stats
	float dirtthresh; // 0.1f	// bad chars / total chars over this value = dirt
	MMMR_BOOL status;       // on or off? 
} DIRTDETECTSET;


/// Defines user configurations for codeline OCR data.
typedef struct OCRUserSet
{
	/// set the context level
	int context;
	
	/// Determines whether space detection is enabled.
	int useSpaceChars;
	
	/// Specifies which character is considered to be the space character.
	///
	/// \remarks The default space character is underline ("_").
	char spaceChar;
	
	/// Specifies which character is considered to be the "bad" character, i.e. the 
	/// character to use as "unrecognised".
	///
	/// \remarks The default bad character is asterisk ("*").
	char badChar;
	
	/// Determines whether to save thresholded image.
	MMMR_BOOL saveThreshImages;

	/// run multiple times for accuracy but takes longer (default on)
	MMMR_BOOL accuracy;
	
	/// Determines whether to check for mrz on labels (mrz positioned higher than normal).
	MMMR_BOOL visaLabelCheck;

	/// Determines whether to check for mrz on labels (mrz positioned higher than normal).
	MMMR_BOOL despeckleCodelineImage;

} OCRUSERSET;

/// Defines global image settings.
typedef struct ImgSettings
{
	int maxBlack;
	int peakstart;
	int edgetol;
	int threshhgt;
	float tLighter;
	float tDarker;
	float tNormal;
	float tSpanish;
	int left;					// left hand side of the image
	int bottom;
	int twidth;
	int theight;
//	int smoothRotate;
	
	/// Determines whether to debarrel images.
	///
	/// \remarks The camera in a \pagereader produces images with a slight barrelling effect.
	/// This is normally not important, but may hinder the reading of very precise image 
	/// features. All images taken by the \sdk are not debarrelled by default.
	///
	/// \remarks Use the \c TRUE or \c FALSE values to set this setting.
	int debarrelImages;
	
	int rotateCodeline;

	/// Determines whether to use a grayscale of the visible image captured (or a separate gray image capture with visible leds).
	///
	/// \remarks Use the \c TRUE or \c FALSE values to set this setting.
	int useVisibleForBarcode;

	/// Determines whether to check the initial IR iage for glare (only when the reader is capable of antiglare).
	///
	/// \remarks Use the \c TRUE or \c FALSE values to set this setting.
	MMMR_BOOL allowGlareDetection;

	/// Determines whether to check the document for visible only OCR if IR returns UNKNOWN DOCUMENT.
	///
	/// \remarks Use the \c TRUE or \c FALSE values to set this setting.

	MMMR_BOOL checkVisibleOcr;

	/// Makes the cr5400 image top/bottom aligned
	///
	/// \remarks Use the \c TRUE or \c FALSE values to set this setting.
	MMMR_BOOL alignTopBottom;
} IMGSET;

/// Defines the settings for determining the position of the photo image returned by the
/// #CD_IMAGEPHOTO data item.
///
/// \ingroup ImageProcModule ImageSettingsModule HLSettingsModule LLSettingsModule
///
/// \note This structure is only relevant if using a \pagereader.
///
/// \remarks Most of these settings are loaded from the \appconfig file.
///
/// \sa #MMMReaderSettings::puPhotoSettings
typedef struct PhotoSettings
{
	/// Determines the width of the photo image.
	///
	/// \remarks This value is measure in millimeters (mm).
	int width;
	
	/// Determines the height of the photo image.
	///
	/// \remarks This value is measure in millimeters (mm).
	int height;
	
	/// Determines the distance from the bottom of the document to the bottom of the
	/// photo image.
	///
	/// \remarks This value is measure in millimeters (mm).
	int bottom;
	
	/// Determines the distance from the left edge of the document to the photo image.
	///
	/// \remarks This value is measure in millimeters (mm).
	int left;
	
	/// Specifies the dots per inch of the photo.
	///
	/// \attention This is a \pagereader calibration setting read from the \pagereader itself;
	/// do not modify.
	float dpmm;
	
} PHOTOSET;

/// Defines debug settings that the \sdk can use to diagnose problems.
///
/// \ingroup DebugSettingsModule HLSettingsModule LLSettingsModule
///
/// \note This structure is only relevant if using a \pagereader.
///
/// \remarks These settings are loaded from the \appconfig file.
///
/// \sa #MMMReaderSettings::puImgDebug
typedef struct ImgDebug
{
	/// \internal
	MMMR_BOOL rte;
	
	//int logLevel;							// DEPRECATED: Use LoggingSettings struct instead
	
	/// Determines whether to save captured images to the current directory.
	///
	/// \remarks Images will be saved in the specified image format, as stated by 
	/// #DATASENDSET::imageFormat.
	///
	/// \remarks Each document read will overwrite the images from the last document.
	MMMR_BOOL puSaveImages;
	
	//unsigned int flushLogFileMaxLines;	// DEPRECATED: Use LoggingSettings struct instead

	/// Determines the number of each type of image to save. Images will have a
	/// number between 0-puMaxSaveImageCount prepended to their file name. Once
	/// puMaxSaveImageCount has been used the number will loop back around to 0
	/// and images will be overwritten.
	int puMaxSaveImageCount;
	
} IMGDEBUG;

/// \deprecated This structure is no longer used. LEDs can be modified for the following 
/// \pagereaders. Contact \ssd for more details.
///		- \rte8000
///		- \rte8000hs
typedef struct LedSettings_s
{
//  LEDSCHEME scheme[NUMEVENTS];
	int unused;
} LedSettings;

/// Defines the settings required to play a beeping sound.
///
/// \ingroup SignalModule SignalSettingsModule HLSettingsModule LLSettingsModule
///
/// \note This structure is only relevant if using a \pagereader.
///
/// \sa SoundSettings
typedef struct SoundData
{
	/// Defines the frequency of the sound to play.
	///
	/// \remarks This value is measured in hertz (hz).
	int puFreq;
	
	/// Defines the duration of the sound to play.
	///
	/// \remarks This value is measured in milliseconds (ms).
	int puTime;
	
} SoundData;

/// Defines the sound settings the \sdk should use.
///
/// \ingroup SignalModule SignalSettingsModule HLSettingsModule LLSettingsModule
///
/// \note This structure is only relevant if using a \pagereader.
///
/// \sa #MMMReaderSettings::puSoundSettings
typedef struct SoundSettings_s
{
	/// Specifies which sound mode the \sdk should use. See #MMMReaderSoundMode for 
	/// an explanation of the available modes.	
	MMMReaderSoundMode puMode;
	
	/// Provides an array of flags, one for each #MMMReaderSignal, to determine whether a
	/// sound should be played for a particular #MMMReaderSignal.
	///
	/// \remarks A #MMMReaderSignal can be used as an index to this array, for example:
	///
	/// \remarks \code puUseSound[SIGNAL_READ]; \endcode
	MMMR_BOOL puUseSound[SIGNAL_COUNT];
	
	/// Provides an array of #SoundData structures, one for each #MMMReaderSignal, to 
	/// determine the settings to use when playing a beep sound for a #MMMReaderSignal.
	///
	/// \note This array is only used when #puMode is set to #SM_BEEP, and each individual
	/// #SoundData setting is used only if its corresponding #puUseSound flag is set to
	/// \c TRUE.
	///
	/// \remarks A #MMMReaderSignal can be used as an index to this array, for example:
	///
	/// \remarks \code puBeep[SIGNAL_READ]; \endcode
	SoundData puBeep[SIGNAL_COUNT];
	
} SoundSettings;

/// \endcond
// PAGESDK

/// Defines the hardware settings the \sdk should be configured with to use a \swipereader.
///
/// \ingroup SwipeModule SwipeSettingsModule HLSettingsModule LLSettingsModule
///
/// \note This structure is only relevant if using a \swipereader.
///
/// \attention These hardware settings are not meant to be modified, either programatically 
/// or via the \swipeconfig configuration file. Changes to these values may prevent the 
/// \swipereader and/or \sdk from working correctly. Use the \swipeconfigwizard to safely 
/// configure the \sdk to use a \swipereader.
///
/// \sa MMMSwipeSettings::Hardware, \ref SettingsPageSwipeHardwareSubSec
typedef struct MMMSwipeHardwareSettings
{
	/// Specifies the type of \swipereader connected. See #MMMSwipeDeviceType for an
	/// explanation of available \swipereaders.
	MMMSwipeDeviceType DeviceType;
	
	/// Determine whether the \sdk should attempt to reboot the \swipereader if an initial
	/// connection failed.
	///
	/// \note This setting is only relevant for USB \swipereaders.
	///
	/// \remarks If set to \c TRUE and the \sdk is unable to connect to the \swipereader,
	/// it will attempt to "reboot" the \swipereader hardware, meaning it will disable and
	/// enable the hardware at an operating system level. This is done so that the operating
	/// system will hopefully recognise the presence of the \swipereader again so that the
	/// \sdk can attempt to connect to it again.
	MMMR_BOOL RebootOnFailedConnection;
	
	/// Specifies the maximum number of "reboot" attempts to carry out.
	///
	/// \note This setting is only relevant if #RebootOnFailedConnection is \c TRUE.
	///
	/// \remarks The \sdk will attempt to reboot and connect to the \swipereader this many
	/// times, or until it connects successfully. If it reaches this limit and it still fails 
	/// to connect to the \swipereader, an error is raised.
	unsigned int RebootRetryAttempts;
	
	/// Specifies how long the \sdk should wait between disabling the \swipereader hardware
	/// and enabling it again.
	///
	/// \note This setting is only relevant if #RebootOnFailedConnection is \c TRUE.
	///
	/// \remarks This value is measured in milliseconds (ms).
	unsigned int RebootWaitTimeout;
	
	/// Specifies how long the \sdk should wait until it attempts to recover an existing
	/// connection to the \swipereader.
	///
	/// \remarks This setting is relevant when the \sdk is already connected to the 
	/// \swipereader and the connection is lost somehow (e.g. the \swipereader is physically
	/// disconnected from the host machine whilst the \sdk is running). At this point, the
	/// \sdk will trigger a #BEGIN_RESOLVING_ERROR event and wait until the \swipereader 
	/// connection is recovered, or the \sdk is shutdown.
	///
	/// \remarks This value is measured in milliseconds (ms).
	unsigned int RecoverWaitTimeout;
	
	/// Provides the type of connection used to connect to the \swipereader. See 
	/// #MMMSerialConnectionType for an explanation of available connections.
	MMMSerialConnectionType ConnectionType;
	
	/// Determines if the \sdk should automatically detect the COM port number to use to
	/// connect to the \swipereader.
	///
	/// \note This setting is only relevant if using a USB \swipereader and the #ConnectionType
	/// is #MSCT_USB_VIRTUAL_COM.
	///
	/// \remarks If this setting is \c TRUE, a #MMMSerialConnectionSettings::PortNumber does
	/// not need to be provided.
	MMMR_BOOL USBAutoDetectPort;

} MMMSwipeHardwareSettings;

/// Defines the settings required for connecting to a serial device, such as a \swipereader.
///
/// \ingroup SwipeModule SwipeSettingsModule HLSettingsModule LLSettingsModule
///
/// \note Unless you understand serial port communication concepts, when using a \swipereader 
/// it is strongly advised that the \swipeconfigwizard be used to safely configure the \sdk 
/// with the \swipereader. The \swipeconfigwizard will automatically determine the most 
/// suitable settings to use.
///
/// \sa MMMSwipeSettings::Connection, \ref SettingsPageSerialConnectionSubSec
typedef struct MMMSerialConnectionSettings
{
	/// Provides the COM port number to connect to.
	///
	/// \remarks When using a \swipereader, if the #MMMSwipeHardwareSettings::USBAutoDetectPort
	/// setting is \c TRUE then this setting need not be supplied. Once the \swipereader has been
	/// automatically detected, this setting is updated with the current COM port number in 
	/// use.
	unsigned int PortNumber;
	
	/// Provides the baud rate at which the serial device operates.
	unsigned int BaudRate;
	
	/// Provides the number of bits in the bytes transmitted and received.
	unsigned int ByteSize;
	
	/// Provides the parity scheme to be used. See #MMMSerialParityType for an explanation of
	/// the supported parity types.
	MMMSerialParityType Parity;
	
	/// Provides the number of stop its to be used. See #MMMSerialStopBitType for an 
	/// explanation of the supported stop bit types.
	MMMSerialStopBitType StopBits;

} MMMSerialConnectionSettings;

/// Defines settings specific to a \swipereader which is configured with a RTE variant
/// protocol.
///
/// \ingroup SwipeModule SwipeSettingsModule HLSettingsModule LLSettingsModule
///
/// \note This structure is only relevant when using a \swipereader configured with a
/// RTE variant protocol.
///
/// \attention These settings are not meant to be modified, either programatically 
/// or via the \swipeconfig configuration file. Changes to these values may prevent the 
/// \swipereader and/or \sdk from working correctly. Use the \swipeconfigwizard to safely 
/// configure the \sdk to use a \swipereader.
///
/// \sa MMMSwipeProtocolSettings::RTE, \ref SettingsPageProtocolsRteSubSec
typedef struct MMMSwipeProtocolSettings_RTE
{
	/// Determines whether messages received from the \swipereader include a checksum at
	/// the end of the data.
	MMMR_BOOL UseBCC;
	
	/// Determines whether the \sdk should automatically enable the \swipereader after
	/// receiving data.
	///
	/// \remarks The protocol variants \c RTE_INTERRUPT and \c RTE_POLLED require that an
	/// "Enable" command be sent to the \swipereader after successfully receiving data; until
	/// this command is sent and acknowledged, the \swipereader will not accept and transmit
	/// any more data.
	///
	/// \remarks With this feature enabled, the \sdk will automatically send this command
	/// to the \swipereader after every document read, meaning the command does not have to
	/// be sent manually.
	///
	/// \remarks This feature is enabled by default.
	///
	/// \remarks This setting has no effect if the \swipereader is configured with the
	/// \c RTE_NATIVE protocol variant.
	///
	/// \sa MMMReader_SwipeRTE_EnableDevice()
	MMMR_BOOL AutoSendEnableDevice;
	
} MMMSwipeProtocolSettings_RTE;

/// Defines the protocol settings that a \swipereader uses.
///
/// \ingroup SwipeModule SwipeSettingsModule HLSettingsModule LLSettingsModule
///
/// \note This structure is only relevant when using a \swipereader.
///
/// \attention These settings are not meant to be modified, either programatically 
/// or via the \swipeconfig configuration file. Changes to these values may prevent the 
/// \swipereader and/or \sdk from working correctly. Use the \swipeconfigwizard to safely 
/// configure the \sdk to use a \swipereader.
///
/// \sa MMMSwipeSettings::Protocol, \ref SettingsPageProtocolsSubSec
typedef struct MMMSwipeProtocolSettings
{
	/// Provides the name of the protocol the \swipereader uses.
	///
	/// \attention This setting should be managed by the \swipeconfigwizard to ensure the
	/// correct value is used.
	RTCHAR ProtocolName[MAX_PATH];
	
	/// Specifies how long the \sdk should wait for a response after sending a command to the
	/// \swipereader.
	///
	/// \note This setting is only relevant for \swipereaders that have been configured with
	/// either the \c RTE_INTERRUPT or \c RTE_POLLED protocols; these are the only protocols
	/// that the \sdk can send commands to.
	///
	/// \remarks This value is measured in milliseconds (ms).
	unsigned int CommandTimeout;
	
	/// Provides additional settings if using a \swipereader configured with a RTE variant
	/// protocol.
	///
	/// \remarks For all other protocols, these settings have no effect. 
	MMMSwipeProtocolSettings_RTE RTE;

} MMMSwipeProtocolSettings;

/// Defines which Swipe data items the \sdk should return to the host application.
///
/// \ingroup HighLevelSettingsModule SwipeModule SwipeSettingsModule HLSettingsModule LLSettingsModule
///
/// \note This structure is only relevant if using a \swipereader.
///
/// \sa MMMSwipeSettings
typedef struct SwipeDataToSend
{
	/// Determines whether to parse MSR data and return any AAMVA data decoded.
	/// Setting the value to 0 will disable AAMVA parsing of MSR data.
    int puAAMVA;

} SwipeDataToSend;

/// Defines all the settings required for the \sdk to use a \swipereader.
///
/// \ingroup SwipeModule SwipeSettingsModule HLSettingsModule LLSettingsModule
///
/// \note This structure is only relevant when using a \swipereader.
///
/// \attention Most of these settings are not meant to be modified, either programatically 
/// or via the \swipeconfig configuration file. Changes to these values may prevent the 
/// \swipereader and/or \sdk from working correctly. Use the \swipeconfigwizard to safely 
/// configure the \sdk to use a \swipereader.
///
/// \if PAGESDK
/// \sa #MMMReaderSettings::puSwipeSettings
/// \endif
typedef struct MMMSwipeSettings
{
	/// Provides all the hardware configuration settings to use.
	MMMSwipeHardwareSettings Hardware;
	
	/// Provides all the connection settings to use.
	MMMSerialConnectionSettings Connection;
	
	/// Provides all the protocol settings to use.
	MMMSwipeProtocolSettings Protocol;

	/// Specifies which optional data elements to return.
	SwipeDataToSend puDataToSend;

} MMMSwipeSettings;

/// \cond PAGESDK

/// Defines the \sdk UHF connection settings.
///
/// \ingroup UHFModule UHFSettingsModule HLSettingsModule LLSettingsModule
///
/// \remarks These settings are loaded from the \appconfig file.
typedef struct UHFSettings
{
	/// Provides the virtual COM port number to the UHF module.
	///
	/// \remarks If the COM port number is set to '0', the \sdk will use the first
	/// UHF module found.
	///
	/// \remarks If a UHF module is not found on this COM port, the \sdk will 
	/// auto-detect the correct COM port number to use and update this member
	/// with value used (if the value is '0' it will always be left as '0').
    int puCOMPort;
    
    /// Provides the maximum time to wait for a response from a UHF tag.
    ///
    /// \remarks This value is measured in milliseconds (ms).
    ///
    /// \remarks A value in the range 600 - 2000 is recommended.
    int puCOMTimeoutMilliseconds;

} UHFSettings;

//#ifdef DCA
/// Defines the \sdk DocProcessing settings.
///
/// \ingroup HLSettingsModule LLSettingsModule
///
/// \remarks These settings are loaded from the \appconfig file.
///
/// \if PAGESDK
/// \sa #MMMReaderSettings::puDocProcessingSettings
/// \endif

typedef struct DocProcessingSettings
{
	/// Provides the method to purpose documents with muliply sides.
	///
	/// \remarks A value of 0 indicates side side processing, 1 indicates two 
	/// sided document processing when a flip side docuemnt is defined in the 
	/// database, 2 indicates two sided document processing, except for when the 
	/// first side is an ICAO mrz
	///
    int puMultiSideProcessing;

	/// A user settable flag to indicate if the SDK can be used to capture data from a document
	bool  puDataCapture;

	/// A user settable flag to indicate if the SDK can be used to authenticate data from the document
	bool  puAuthentication;

	/// A user settable flag to send DCA specific data messages from the document processing.  This does 
	/// not apply to the image data messages
	bool puDataMessagesDCA;

	/// A user settable flag to  send original data messages from the document processing. This does 
	/// not apply to the image data messages
	bool puDataMessagesPreDCA;

	/// A user settable flag to disable all image capture/processing for visible image 
	bool puVisibleImageDisableOverride;

	/// A user settable flag to disable all image capture/processing for Ultraviolet image 
	bool puUltravioletImageDisableOverride;

	/// A user settable flag to disable all image capture/processing for Confirm/tamper image 
	bool puConfirmImageDisableOverride;

	/// A user settable flag to send image only required by Layout database script 
	bool puSendImagesAccordingToLayoutDB;

} DocProcessingSettings;
//#endif //DCA

/// \cond PAGESDK


/// Defines all the settings required for the \sdk.
///
/// \ingroup SettingsModule HLSettingsModule LLSettingsModule
///
/// \note This structure is mostly relevant if using a \pagereader. However, the \sdk is
/// capable of using a \swipereader at the same time, in which case the 
/// #MMMReaderSettings::puSwipeSettings become relevant.
///
/// \sa For the \hlapi:
///		- MMMReader_GetSettings()
///		- MMMReader_UpdateSettings()
///		- MMMReader_SaveSettings()
///
/// \sa For the \llapi:
///		- MMMReader_LL_LoadSettings()
///		- MMMReader_LL_LoadSettingsFromIniFile()
///		- MMMReader_LL_InitialiseSettings()
///		- MMMReader_LL_SaveSettings()
typedef struct MMMReaderSettings
{
	/// Provides all the settings related to the calibration of a \pagereader camera.
	CAMERASET puCameraSettings;
	
	/// Provides all the settings related to \pagereader document detection.
	DOCDETECTSETTINGS puDocDetectSettings;
	
	/// Provides all the settings related to the \pagereader document positions.
	DOCPOS puDocumentPositionSettings;

	/// Provides all the settings related to the \pagereader document positions for 
	/// side 2 of the document on double sided reader.
	DOCPOS puDocumentPositionSettingsSide2;
	
	/// Provides all the settings related to the \pagereader imaging capabilities.
	IMGSET puImageSettings;
	
	/// Provides all the settings related to the \pagereader photo features.
	PHOTOSET puPhotoSettings;
	
	/// Provides all the settings related to Optical Character Recognition.
	OCRSET puOcrSettings;
	
	/// Provides all the user settings related to Optical Character Recognition.
	OCRUSERSET puOcrUserSettings;
	
	/// Provides all the settings related to dirt detection on the scanner glass
	DIRTDETECTSET puDirtDetectSettings;
	
	/// Provides all the settings related to sound schemes for a \pagereader.
	SoundSettings puSoundSettings;
	
	/// \deprecated These settings are no longer used. LEDS can be modified for the following
	/// \pagereaders. Contact \ssd for more details.
	///		- \rte8000
	///		- \rte8000hs
	LedSettings puLedSettings;
	
	/// Provides all the settings related to RFID operations.
    RFIDSettings puRFIDSettings;

	/// Provides all the settings related to image debugging.
	IMGDEBUG puImgDebug;

	/// Provides all the settings related to controlling which data items the \hlapi should
	/// process.
	DATASENDSET puDataToSend;
	
	/// Provides all the settings related to controlling the \sdk logging features.
	LoggingSettings puLoggingSettings;
	
	/// Provides all the settings related to using a \swipereader.
	MMMSwipeSettings puSwipeSettings;

	/// Provides all the settings related to using a \UHF.
	UHFSettings puUHFSettings;

//#ifdef DCA
	/// Provides all the settings related to processing of the document by the ReaderThread_Ex.
	DocProcessingSettings puDocProcessingSettings;
//#endif //DCA
	/// Provides a pointer to a null-terminated string which contains the folder location
	/// of the \pagereader Mass Storage Device (MSD) file directory.
	///
	/// \note Only the \rte8000 and \rte8000hs have a Mass Storage Device. For all other
	/// \pagereaders, this value will be the same as #puExeDir.
	///
	/// \attention This value is provided for information purposes only. Do not modify.
	///
	/// \sa MMMReader_LL_GetReaderDir()
	RTCHAR *puReaderDir;
	
	/// Provides a pointer to a null-terminated string which contains the folder location
	/// where the current host application is running from.
	///
	/// \attention This value is provided for information purposes only. Do not modify.
	///
	/// \sa MMMReader_LL_GetExeDir()
	RTCHAR *puExeDir;

	/// Provides a pointer to a null-terminated string which contains the folder location
	/// of the \sdk Binary ("Bin") folder.
	///
	/// \remarks This value could be the same as #puExeDir but not necessarily.
	///
	/// \attention This value is provided for information purposes only. Do not modify.
	///
	/// \sa MMMReader_LL_GetBinDir()
	RTCHAR *puBinDir;
	
	/// Provides a pointer to a null-terminated string which contains the folder location
	/// of the \sdk Configuration ("Config") folder.
	///
	/// \attention This value is provided for information purposes only. Do not modify.
	///
	/// \sa MMMReader_LL_GetConfigDir()
	RTCHAR *puCfgDir;
	
	/// Provides a pointer to a null-terminated string which contains the folder location
	/// of the \sdk Plugins folder.
	///
	/// \attention This value is provided for information purposes only. Do not modify.
	///
	/// \sa MMMReader_LL_GetPluginDir()
	RTCHAR *puPluginDir;
	
	/// Provides a pointer to a null-terminated string which contains the folder location
	/// of the \sdk Data folder.
	///
	/// \attention This value is provided for information purposes only. Do not modify.
	///
	/// \sa MMMReader_LL_GetDataDir()
	RTCHAR *puDataDir;

	/// Provides a pointer to a null-terminated string which contains the folder location
	/// of the \sdk Log folder.
	///
	/// \attention This value is provided for information purposes only. Do not modify.
	///
	/// \sa MMMReader_LL_GetLogDir()
	RTCHAR *puLogDir;

//#ifdef DCA

	/// Provides a pointer to a null-terminated string which contains the folder location
	/// of the \sdk layout Database folder.
	///
	/// \attention This value is provided for information purposes only. Do not modify.
	///
	/// \sa MMMReader_LL_GetLayoutDatabaseDir()
	RTCHAR *puLayoutDatabaseDir;
//#endif //DCA
} MMMReaderSettings;

/// \endcond
// PAGESDK

#if !(defined(__linux__) && defined(__arm__))
#pragma pack(pop)
#endif

#endif //_MMMREADERSETTINGS_H_
