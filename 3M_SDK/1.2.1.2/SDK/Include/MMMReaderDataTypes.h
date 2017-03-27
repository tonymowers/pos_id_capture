#ifndef _MMMReaderDataTypes_H_
#define _MMMReaderDataTypes_H_

//Backward compatability EAC reserved names
#include "MMMReaderEAC.h"
#include "MMMReaderOCRDataTypes.h"

#if !(defined(__linux__) && defined(__arm__))
#pragma pack(push, 1)
#endif

// Warning: some of the content within this file is subject to change in the next releae of the API.
//
// In particular the structs towards the bottom, and additional values may be added to the 
// enumerations. Do not rely upon the numeric values.

/// Type definition of a boolean; provided for compatibility with C.
///
/// \ingroup GeneralModule
#ifndef __cplusplus
typedef enum { false, true } bool;
#endif

/// Type definition for a 1-byte boolean value. This is provided as this header file
/// needs to be compatible with C, which does not provide the C++ \c bool type.
///
/// \ingroup GeneralModule
///
/// \remarks Possible values are:
///		- \c TRUE (non-zero)
///		- \c FALSE (zero)
typedef unsigned char MMMR_BOOL;

#ifdef UNDER_CE
/// Define a character as a wide-character for Unicode compatibility.
#define	RTCHAR	wchar_t
#else
/// Define a character as a single-character for ANSI compatibility.
///
/// \ingroup GeneralModule
#define RTCHAR	char
#endif

/// Typedef of an single byte (8-bit unsigned). This is equivalent to an unsigned char.
///
/// \ingroup GeneralModule
#ifndef MMM_BYTE_DEFINED
typedef unsigned char MMMByte;
#define MMM_BYTE_DEFINED
#endif	//	MMM_BYTE_DEFINED

/// Typedef for beginning of message to denote a unicode string in a byte array.
///
/// \ingroup GeneralModule
#define UNICODE_BOM		0xfeff
#define UNICODE_BOM_STR	 "\377\376"



/// \cond PAGESDK

/// Defines a 4-point region, usually used in relation to plugins.
///
/// \ingroup PluginsModule HighLevelDataModule
///
/// \note This structure is only relevant if using a \pagereader.
///
/// \sa #MMMReaderPluginData::puRegion
typedef struct MMMRegion
{
	long puTopLeft_X;
	long puTopLeft_Y;

	long puTopRight_X;
	long puTopRight_Y;

	long puBottomRight_X;
	long puBottomRight_Y;

	long puBottomLeft_X;
	long puBottomLeft_Y;
} MMMRegion;

#define LIGHT_RED     1
#define LIGHT_GREEN   2
#define LIGHT_BLUE    4
#define	LIGHT_WHITE		(LIGHT_RED | LIGHT_GREEN | LIGHT_BLUE)
#define LIGHT_IRFULL  8
#define LIGHT_IRPART 16
#define	LIGHT_IR		(LIGHT_IRPART | LIGHT_IRFULL)

#define LIGHT_UV     32
#define	LIGHT_COAX_IR	64
#define	LIGHT_COAX_VIS	128

// map through the old names, for anyone who might be using them
#define LIGHT_RESERVED2	LIGHT_COAX_IR
#define LIGHT_RESERVED1	LIGHT_COAX_VIS


/// These two constants are maintained for backwards compatibility, but it is recommended that code
/// is switched away from setting flags in the aLight parameter of TakeSnapshot and instead set the
/// aBrightnessSettings->puAmbientRemoval value to the preferred value.
#define LIGHT_BACKGROUNDSUBTRACT_POSTGAIN 0x2000
#define LIGHT_BACKGROUNDSUBTRACT 0x4000

/// The different methods of ambient light removal.
typedef enum MMMReaderAmbientRemovalMethod
{
	/// No ambient removal is applied
	MRAS_NO_AMBIENT_REMOVAL = 0,

	/// Use the simple approach to ambient light removal. Doesn't work in extreme cases, but should
	/// work with most hardware devices.
	MRAS_BASIC_AMBIENT_REMOVAL,
	
	/// Use the post-gain algorithm to remove higher levels of ambient light, with a small trade-off 
	/// on image quality if there isn't much ambient light.
	MRAS_POSTGAIN_AMBIENT_REMOVAL,

	/// Some readers can capture with hardware ambient removal, and this is likely preferable to using
	/// the non-hardware based algorithms, with the least trade-off on image quality
	MRAS_HARDWARE_AMBIENT_REMOVAL,

	/// Perform ambient removal in the hardware with a trade-off on image quality to cope with a medium
	/// range of ambient light.
	MRAS_HARDWARE_AMBIENT_REMOVAL_MEDIUM,

	/// Perform ambient removal in the hardware with a trade-off on image quality to cope with a greater
	/// range of ambient light.
	MRAS_HARDWARE_AMBIENT_REMOVAL_EXTREME,

	/// This algorithm is only valid for IRDetect images, and is only used inside the LocateDocument / 
	/// DetectDocument APIs. This will cause us to capture an ambient image only for the purposes of
	/// locating a document, but not for detection itself.
	MRAS_LOCATE_AMBIENT_REMOVAL,

} MMMReaderAmbientRemovalMethod;

#define	LIGHT_BANK_LEFT		1
#define	LIGHT_BANK_FRONT	2
#define	LIGHT_BANK_RIGHT	4
#define	LIGHT_BANK_BACK		8

#define	LIGHT_BANK_ALL	(LIGHT_BANK_LEFT | LIGHT_BANK_BACK	| LIGHT_BANK_RIGHT | LIGHT_BANK_FRONT)

#define MAX_BARCODE_DATA_FIELD_LENGTH	2710

/// \endcond	
// PAGESDK

#ifndef CFREADER_DLL
/// The different log levels provided by the \sdk.
///
/// \ingroup LoggingModule
typedef enum MMMReaderLogLevel
{
	LOG_LVL_ERROR = 0,			///< Log errors only.
	LOG_LVL_WARNING,			///< Log errors and warnings.
	LOG_LVL_DEBUG_LOW,			///< Log errors, warnings and low level debug messages.
	LOG_LVL_DEBUG_HIGH,			///< Log errors, warnings, low and high level debug messages.
	
	LOG_LVL_ALL					///< Log all details to the log file.
	
} MMMReaderLogLevel;
#endif //CFREADER_DLL

/// The different logging strategies provided by the \sdk. A logging strategy is defined as
/// the method the \sdk will create and add to its log files.
///
/// \ingroup LoggingModule
typedef enum MMMReaderLoggingStrategy
{
	/// Append all log messages to the end of the existing log file. If no log file of a
	/// particular name exists, then a new file is created.
	///
	/// \note This is the default behaviour of the \sdk error handling module.
	MRLS_APPEND = 0,
	
	/// If a log file already exists, the contents will be cleared (truncated) and all future
	/// log messages will be appended starting from the beginning of the log file. If no log
	/// file of a particular name exists, then a new file is created.
	MRLS_TRUNCATE,
	
	/// Similar to #MRLS_APPEND, except that when the current log file reaches a certain 
	/// predetermined size, the current log file is closed and a new one is started.
	MRLS_FILE_SIZE

} MMMReaderLoggingStrategy;

/// Structure to configure the logging settings of the \sdk. This structure is used as a
/// parameter to the MMMReader_SetLoggingOptions() function.
///
/// \ingroup LoggingModule HLErrorsLoggingModule LLErrorsLoggingModule HLSettingsModule LLSettingsModule
typedef struct LoggingSettings
{
	/// Specify the log level to use. This represents how detailed the \sdk log file should be.
	/// Use zero (#LOG_LVL_ERROR) for errors and exceptions only, and 5 for
	/// the most detailed logs.
	///
	/// \note \ssd may ask you for log files generated at runtime to assist in helping with
	/// any problems. Setting the log level to 5 will allow \ssd to see the exact order of
	/// events occuring with the \sdk to troubleshoot problems.
	int logLevel;
	
	/// Specify the log mask to filter log messages. The log mask is used to view only messages
	/// belonging to a particular category, which can assist you or \ssd to view only the
	/// necessary debug information. Typically, this would be set to #LOGMASK_ALL to log
	/// all messages.
	unsigned int logMask;
	
	/// Specify how many log lines to keep in memory before flushing to the file. By default,
	/// the \sdk waits for 100 messages to be logged before flushing to the log file; this 
	/// speeds up performance. To view messages entering the file at a more regular pace,
	/// set a lower value or set it to zero to force every message to be logged immediately.
	unsigned int flushLogFileMaxLines;
	
	/// Specify which logging strategy to use. See #MMMReaderLoggingStrategy for more details.
	MMMReaderLoggingStrategy loggingStrategy;
	
	/// If #loggingStrategy is set to #MRLS_FILE_SIZE, specify the
	/// maximum file size for one log file, measured in bytes. When the current log file
	/// reaches or exceeds this limit, it will be closed and a new log file will be created
	/// as per the logging strategy.
	unsigned int maxLogFileSize;

	/// This setting can be used to enable detailed logging of the reader. Note that this
	/// logfile doesn't go into the default logfile or respect logmask etc, and is intended
	/// for debugging errors associated with "camera driver error".
	int readerLogLevel;

} LoggingSettings;

/// \addtogroup LoggingModule
/// @{
#define LOGMASK_ALL			0xfffffffful	///< Log mask value to log all messages.

/// \cond PAGESDK
#define	LOGMASK_OCR			0x00000001ul	///< Log mask value to log OCR messages.
#define	LOGMASK_IMAGE		0x00000002ul	///< Log mask value to log image processing messages.
#define	LOGMASK_CAMERA		0x00000004ul	///< Log mask value to log camera messages.
#define	LOGMASK_SIGNAL		0x00000008ul	///< Log mask value to log signalling related messages.
#define	LOGMASK_PLUGINS		0x00000010ul	///< Log mask value to log plugin related messages.
/// \endcond
// PAGESDK

#define	LOGMASK_MISC		0x00000020ul	///< Log mask value to log miscellaneous messages that do not have a particular category.

/// \cond PAGESDK
#define	LOGMASK_HIGHLEVEL	0x00000040ul	///< Log mask value to log High Level API related messages.
#define	LOGMASK_DOCDETECT	0x00000400ul	///< Log mask value to log document detection messages.
#define	LOGMASK_RFID		0x00001000ul	///< Log mask value to log RFID related messages.
#define LOGMASK_UHF			0x00000800ul	///< Log mask value to log UHF related message.

//#ifdef DCA
#define LOGMASK_LAYOUTDB    0x00002000ul	///< Log mask value to log Layout database related message.
#define LOGMASK_DATACAPTURE 0x00004000ul	///< Log mask value to log Layout database related message.
#define LOGMASK_LICENSE		0x00008000ul	///< Log mask value to log License related message.
//#endif // DCA

/// \endcond
// PAGESDK

#define LOGMASK_SERIAL		0x00000100ul	///< Log mask value to log serial communication messages.
#define LOGMASK_USB			0x00000200ul	///< Log mask value to log USB related messages.
/// @}	// LoggingModule


// *** IF YOU ADD TO THIS ENUM PLEASE EDIT C# AND JAVA WRAPPERS ***
/// Defines all the different events that the \sdk can raise to the host application.
///
/// \ingroup EventsModule
///
/// \sa #MMMReaderEventCallback
typedef enum MMMReaderEventCode
{
	/// \if PAGESDK
	/// This event is triggered when a new document has been detected on the window of a
	/// \pagereader.
	///
	/// \note This event is only triggered when using a \pagereader.
	///
	/// \note This event will only be fired when the \pagereader is in the #READER_ENABLED
	/// or #READER_DISABLED states.
	///
	/// \else
	///	\attention This event is not used by the \sdk.
	/// \endif
	DOC_ON_WINDOW,
	
	/// \if PAGESDK
	/// This event is triggered when a document that has been detected has been removed from
	/// the window of a \pagereader.
	///
	/// \note This event is only triggered when using a \pagereader.
	///
	/// \note This event will only be fired when the \pagereader is in the #READER_ENABLED
	/// or #READER_DISABLED states.
	///
	/// \else
	///	\attention This event is not used by the \sdk.
	/// \endif
	DOC_REMOVED,
	
	/// \if PAGESDK
	/// This event is triggered immediately before the first data item is read from a document.
	///
	/// \note This event is only triggered when using a \pagereader. The equivalent 
	/// \swipereader event is #START_OF_SWIPE_DATA.
	///
	/// \else
	///	\attention This event is not used by the \sdk.
	/// \endif
	START_OF_DOCUMENT_DATA,
	
	/// \if PAGESDK
	/// This event is triggered immediately after the last data item is read from a document.
	/// A typical use for this event might be to process all the data that has been received.
	///
	/// \note This event is only triggered when using a \pagereader. The equivalent 
	/// \swipereader event is #END_OF_SWIPE_DATA.
	///
	/// \else
	///	\attention This event is not used by the \sdk.
	/// \endif
	END_OF_DOCUMENT_DATA,
	
	/// \if PAGESDK
	/// \internal
	/// \else
	///	\attention This event is not used by the \sdk.
	/// \endif
	AUTOMATIC_STATE_CHANGE,
	
	/// \ifnot SWIPESDK
	/// This event is triggered when an RF chip in a document has been successfully opened for
	/// further reading. This indicates that there is a chip present. If the chip is a recognised
	/// type, then this will be followed by an RF_APPLICATION_OPENED_SUCCESSFULLY event.
	///
	/// \note This event is only triggered when using a \pagereader with an RFID component and 
	/// when the RF data item has been enabled.
	///
	/// \note If this event is triggered, then the #RF_CHIP_OPEN_FAILED event will never get
	/// triggered during the same document read.
	///
	/// \else
	///	\attention This event is not used by the \sdk.
	/// \endif
	RF_CHIP_OPENED_SUCCESSFULLY,
	
	/// \ifnot SWIPESDK
	/// This event is triggered when an RF chip in a document has been successfully opened as
	/// a known chip type. If a chip that does not use the "MRTD" application is detected, or
	/// there is another problem selecting the application, this event will not be raised,
	/// although #RF_CHIP_OPENED_SUCCESSFULLY will be.
	///
	/// \note This event is only triggered when using a \pagereader with an RFID component and 
	/// when the RF data item has been enabled.
	///
	/// \else
	///	\attention This event is not used by the \sdk.
	/// \endif
	RF_APPLICATION_OPENED_SUCCESSFULLY,

	/// \ifnot SWIPESDK
	/// This event is triggered when an RF chip in a document failed to open.
	///
	/// \note This event is only triggered when using a \pagereader with an RFID component.
	///
	/// \note If this event is triggered, then the #RF_CHIP_OPENED_SUCCESSFULLY event will 
	/// never get triggered during the same document read.
	///
	/// \else
	///	\attention This event is not used by the \sdk.
	/// \endif
	RF_CHIP_OPEN_FAILED,
	
	/// This event is triggered when an internal error was encountered and has automatically
	/// been resolved.
	///
	/// \if PAGESDK
	/// \note This event is only triggered when using a \pagereader with the \hlapi.
	///
	/// \note When an internal error occurs, the \hlapi will automatically switch to the
	/// #READER_ERRORED state and then attempt to restart the \pagereader. When the \pagereader
	/// successfully re-initialises, the \sdk will be automatically restored to the previous
	/// state and trigger this event.
	///
	/// \endif
	///
	/// \sa BEGIN_RESOLVING_ERROR
	READER_ERROR_RESOLVED,
	
	/// \if PAGESDK
	/// This event is triggered during the MMMReader_Initialise() call immediately after the
	/// \sdk configuration settings have been loaded but prior to any other initialisation. 
	/// This is provided so that any settings may be updated by the host application via
	/// MMMReader_UpdateSettings() prior to the initialisation of the \hlapi so that any
	/// changes may be applied before any document already on the \pagereader is processed.
	///
	/// \note This event is only triggered when using a \pagereader with the \hlapi.
	///
	/// \note This event can be used to programatically configure the host application to 
	/// ensure that the application will always use the correct settings.
	///
	/// \else
	///	\attention This event is not used by the \sdk.
	/// \endif
	SETTINGS_INITIALISED,
	
	/// \if PAGESDK
	/// This event is triggered during the MMMReader_Initialise() call immediately after the
	/// plugins have been loaded and initialised. This is provided so that applications may
	/// enumerate and enable/disable plugins as required prior to the initialisation of the
	/// \hlapi.
	///
	/// \note This event is only triggered when using a \pagereader with the \hlapi.
	///
	/// \sa MMMReader_EnablePlugin(), MMMReader_IsPluginEnabled(), MMMReader_GetPluginName(),
	/// MMMReader_SetPluginOrder()
	///
	/// \else
	///	\attention This event is not used by the \sdk.
	/// \endif
	PLUGINS_INITIALISED,
	
	/// \if PAGESDK
	/// This event is triggered immediately before decoding images using the plugins.
	///
	/// \note This event is only triggered when using a \pagereader with the \hlapi, and
	/// when at least one plugin is enabled.
	///
	/// \else
	///	\attention This event is not used by the \sdk.
	/// \endif
	START_OF_PLUGINS_DECODE,
	
	/// \ifnot SWIPESDK
	/// This event is triggered when waiting for an RF chip in a document to be placed on
	/// the \pagereader takes too long and the operation is aborted.
	///
	/// \note This event is only triggered when using a \pagereader with an RFID component.
	///
	/// \note This event only applies to the \llapi. 
	///
	/// \sa MMMReader_RFWaitForOpen()
	///
	/// \else
	///	\attention This event is not used by the \sdk.
	/// \endif
	RF_CHIP_OPEN_TIMEOUT,
	
	/// \ifnot SWIPESDK
	/// This event is triggered when an RF chip in a document has been removed from the
	/// a \pagereader.
	///
	/// \note This event is only triggered when using a \pagereader with an RFID component.
	///
	/// \note This event only applies to the \llapi.
	///
	/// \sa MMMReader_RFWaitForRemoval()
	///
	/// \else
	///	\attention This event is not used by the \sdk.
	/// \endif
	RF_CHIP_REMOVAL_SUCCESS,
	
	/// \ifnot SWIPESDK
	/// This event is triggered when waiting for an RF chip in a document to be removed from
	/// the \pagereader takes too long and the operation is aborted.
	///
	/// \note This event is only triggered when using a \pagereader with an RFID component.
	///
	/// \note This event only applies to the \llapi.
	///
	/// \sa MMMReader_RFWaitForRemoval()
	///
	/// \else
	///	\attention This event is not used by the \sdk.
	/// \endif
	RF_CHIP_REMOVAL_TIMEOUT,
	
	/// \ifnot SWIPESDK
	/// This event is triggered immediately before waiting for the RF chip in a document to
	/// be opened.
	///
	/// \note This event is only triggered when using a \pagereader with an RFID component.
	///
	/// \else
	///	\attention This event is not used by the \sdk.
	/// \endif
	READY_FOR_SMARTCARD,
	
	/// This event is triggered when an internal error was encountered and the \sdk is about
	/// to automatically resolve it. If the error could be resolved, a #READER_ERROR_RESOLVED
	/// event will then be triggered.
	BEGIN_RESOLVING_ERROR,
	
	/// \internal
	COM_PORT_OPEN,
	
	/// \internal
	COM_PORT_CLOSED,
	
	/// \internal
	READING_DATA,
	
	/// \internal
	DATA_READ,
	
	/// This event is triggered immediately before the first data item is read from a swiped
	/// document.
	///
	/// \note This event is only triggered when using a \swipereader.
	START_OF_SWIPE_DATA,
	
	/// This event is triggered immediately after the last data item is read from a swiped
	/// document.
	///
	/// \note This event is only triggered when using a \swipereader.
	END_OF_SWIPE_DATA,
	
	/// \internal
	DEVICE_CONNECTED,
	
	/// \internal
	DEVICE_DISCONNECTED,
	
	/// This event is triggered immediately after the \sdk successfully connects to a
	/// \swipereader.
	///
	/// \note This event is only triggered when using a \swipereader.
	SWIPE_READER_CONNECTED,
	
	/// This event is triggered immediately after the \sdk successfully disconnects from
	/// a \swipereader.
	///
	/// \note This event is only triggered when using a \swipereader.
	SWIPE_READER_DISCONNECTED,

	/// This event is triggered when the state of the \pagereader has changed.
	///
	/// \note This event is only triggered when using the \hlapi.
	///
	/// \note To find out the state of the reader call MMMReader_GetState().
	///
	///
	/// \sa MMMReader_GetState(), MMMReader_SetState()
	READER_STATE_CHANGED,

	/// This event is triggered when a UHF command takes too long and the 
	/// opperation is aborted.
	///
	/// \sa MMMReader_UHFWaitForTagID(), MMMReader_UHFWaitForEPC(), 
	/// MMMReader_UHFWaitForMemoryBankData()
	UHF_READ_TIMEOUT,

	/// This event is triggered when a UHF command has completed
	///
	/// \remarks If there are no tags in the field UHF read functions
	/// will not trigger data callback and this event can be used to
	/// signal that no tag data was read.
	///
	/// \sa MMMReader_UHFGetForTagID(), MMMReader_UHFWaitForTagID(), 
	/// MMMReader_UHFGetForEPC(), MMMReader_UHFWaitForEPC(), 
	/// MMMReader_UHFGetForMemoryBankData(), MMMReader_UHFWaitForMemoryBankData()
	UHF_READ_COMPLETE,
	
	/// \ifnot SWIPESDK
	/// This event is triggered when a motor feed of a document is complete
	///
	/// \note This event is only triggered when using a \pagereader with a motor feed
	///
	/// \sa MMMReader_CameraFeedDocument()
	///
	/// \else
	///	\attention This event is not used by the \sdk.
	/// \endif
	DOC_FEED_COMPLETE,

	/// \ifnot SWIPESDK
	/// This event is triggered when a motor feed of a document has failed
	/// due to the feeder hopper being empty.
	///
	/// \note This event is only triggered when using a \pagereader with a motor feed
	///
	/// \sa MMMReader_CameraFeedDocument()
	///
	/// \else
	///	\attention This event is not used by the \sdk.
	/// \endif
	DOC_FEED_FAILED,

	/// This event is triggered if dirt is found on the scanner window
	///
	/// \note This event is only triggered if dirt detection is turned on and dirt is found
	DIRT_DETECTED_ON_SCANNER_WINDOW,

//#ifdef DCA
	/// This event is triggered to request the reader operator to swipe the document on the magnetic strip reader .
	///
	/// \note This event is only triggered when using the Initialize_Ex \hlapi.
	///
	SWIPE_REQUESTED,
	
	/// This event is triggered to request the reader operator to position the document over/close by the uhf antenna .
	///
	/// \note This event is only triggered when using the Initialize_Ex \hlapi.
	///
	UHF_REQUESTED,
	
	/// \ifnot SWIPESDK
	/// This event is triggered when an RF chip hes been detected
	///
	/// \note This event is only triggered when using a \pagereader with an RFID component and 
	/// when the RF data item has been enabled.
	///
	/// \else
	///	\attention This event is not used by the \sdk.
	/// \endif
	RF_CHIP_DETECTED,

	/// This event is triggered to request the reader operator to flip the document over for further processing.
	///
	/// \note This event is only triggered when using the Initialize_Ex \hlapi.
	///
	FLIP_DOCUMENT_OVER,

//#endif //DCA

	/// This event is triggered immediately after the \sdk successfully connects to a
	/// \ 3M reader.
	///
	/// \note This event is only triggered when using the \hlapi.
	READER_CONNECTED,
	
	/// This event is triggered immediately after the \sdk successfully disconnects from
	/// a \ 3M reader.
	///
	/// \note This event is only triggered when using the \hlapi.
	READER_DISCONNECTED,

	// This must be the last "normal" event code, and is used for testing validity
	/// \internal
	NUM_EVENT_CODES
	
} MMMReaderEventCode;
// *** IF YOU ADD TO THIS ENUM PLEASE EDIT C# AND JAVA WRAPPERS ***

/// Defines the different types of data that the \sdk can handle. One of these values is returned 
/// via the #MMMReaderDataCallback.
///
/// \ifnot SWIPESDK
/// One of these values is also returned as part of the #MMMReaderPluginData structure.
/// \endif
///
/// \ingroup GeneralModule
typedef enum MMMReaderDataFormat
{
	/// Defines a string type.
	///
	/// \remarks This type differs from the #DF_STRING_ASCII type in that the underlying
	/// character type of the string is dependent on whether the \sdk is built as an ASCII
	/// or Unicode build. By default, the \sdk is built so that all strings are
	/// single-byte character strings (i.e. ASCII), in which case #DF_STRING and 
	/// #DF_STRING_ASCII are usually interchangeable.
	///
	/// \remarks The length of the string buffer is returned through the 
	/// #MMMReaderDataCallback via the \a aDataLen parameter.
	///
	/// \ifnot SWIPESDK
	/// \remarks The length of the string buffer is specified in the #MMMReaderPluginData
	/// structure via the \a puDataLen member.
	/// \endif
	DF_STRING,
	
	/// Defines an ASCII string type.
	///
	/// \remarks This type differs from the #DF_STRING type in that this string type always
	/// represents a single-byte character string (i.e. ASCII). Although the \sdk is not built
	/// with Unicode in mind, if in the future it were to be, this string type would still
	/// always represent an ASCII string.
	///
	/// \remarks The length of the string buffer is returned through the 
	/// #MMMReaderDataCallback via the \a aDataLen parameter.
	///
	/// \ifnot SWIPESDK
	/// \remarks The length of the string buffer is specified in the #MMMReaderPluginData
	/// structure via the \a puDataLen member.
	/// \endif
	DF_STRING_ASCII,

	/// \if PAGESDK
	/// Defines an Unicode string type.
	///
	/// \remarks This type differs from the #DF_STRING type in that this string type always
	/// represents a Unicode string. Although the \sdk is not currently built
	/// with Unicode in mind, if in the future it were to be, this string type would still
	/// always represent an Unicode string, but could end up being the same as #DF_STRING.
	///
	/// \remarks The length of the string buffer is returned through the 
	/// #MMMReaderDataCallback via the \a aDataLen parameter.
	///
	/// \remarks The length of the string buffer is specified in the #MMMReaderPluginData
	/// structure via the \a puDataLen member.
	///
	/// \else
	/// \attention This value is not used in the \sdk.
	/// \endif
	DF_STRING_UNICODE,
	
	/// \if PAGESDK
	/// Defines an image type. This represents a pointer to a GDI+ Bitmap object.
	///
	/// \note This data type is generally used for internal purposes only, unless you are
	/// using the \llapi directly.
	///
	/// \else
	///	\attention This value is not used by the \sdk.
	/// \endif
	DF_IMAGE,

	/// \if PAGESDK
	/// Defines an image type. This represents a pointer to byte array containing an image
	/// in the jpeg format.
	///
	/// \remarks The length of the image buffer is returned through the 
	/// #MMMReaderDataCallback via the \a aDataLen parameter.
	///
	/// \remarks The length of the image buffer is specified in the #MMMReaderPluginData
	/// structure via the \a puDataLen member.
	///
	/// \else
	///	\attention This value is not used by the \sdk.
	/// \endif
	DF_IMAGE_JPEG,
	
	/// \if PAGESDK
	/// Defines a pointer to some binary data.
	///
	/// \note This represents a non-integral type, so it could represent a pointer to a 
	/// structure.
	///
	/// \else
	///	\attention This event is not used by the \sdk.
	/// \endif
	DF_BINARYDATA,
	
	/// Defines a 32-bit signed integer.
	DF_INT,
	
	/// Defines an 8-bit unsigned byte, equivalent to #MMMByte.
	DF_BYTE,
	
	/// Defines an array of raw binary data in the form of byte values (#MMMByte).
	///
	/// \remarks The length of the byte array is returned through the #MMMReaderDataCallback
	/// via the \a aDataLen parameter.
	DF_BYTE_ARRAY,
	
	/// Defines an 8-bit boolean value, equivalent to #MMMR_BOOL.
	DF_BOOLEAN,
	
	/// Defines a single-precision floating point number.
	DF_FLOAT,
	
	/// Defines a pointer to a structure.
	///
	/// \remarks The total size of the structure is returned, in bytes, through the
	/// #MMMReaderDataCallback via the \a aDataLen parameter.
	DF_STRUCT,

	/// \if PAGESDK
	/// Defines a pointer to an array of bytes that can be interpreted differently,
	/// based on the Extended Channel Interpretation (ECI) value used to encode it.
	///
	/// \remarks Data of this type will be passed in a structure that contains the
	/// ECI number used, such as the plugin data #MMMReaderPluginData.
	///
	/// \else
	/// \attention This value is not used by the \sdk.
	/// \endif
	DF_ECI_DATA,
	
//#ifdef DCA
	/// Defines a pointer to a MMMReaderDate structure.
	DF_STRUCT_DATE,
	/// Defines a pointer to a MMMReaderBarcodeInformation structure.
	DF_STRUCT_BARCODE_INFORMATION,
	/// Defines a pointer to a MMMReaderAAMVAData structure.
	DF_STRUCT_AAMVA,
	/// Defines a pointer to a MMMReaderCodelineData structure.
	DF_STRUCT_CODELINEDATA,
	/// Defines a pointer to a MMMReaderRfResults structure.
	DF_STRUCT_RF,
	/// Defines a pointer to a MMMReaderMsrData structure.
	DF_STRUCT_MSR_DATA,
	/// Defines a pointer to a DG3FingerprintData structure.
	DF_STRUCT_FINGERPRINT,
	/// Defines a pointer to a QaCompareMeasurements structure.
	DF_STRUCT_QA_COMPARE_MEASUREMENTS,
	/// Defines a pointer to a MMMReaderUHFTagIDData structure.
	DF_STRUCT_UHF_TAG_ID_DATA,
	/// Defines a pointer to a MMMReaderUHFTagMemoryData structure.
	DF_STRUCT_UHF_TAG_MEMORY_DATA,
//#endif // DCA

	DF_NumOfDataFormat // here to assist with conditional insert of additional DF types
	
} MMMReaderDataFormat;

/// \cond PAGESDK

/// Defines the set of signals that the \sdk can be sent to provide an external notification to
/// the user. Signals can be used to switch on/off an LED on the \pagereader, play a sound,
/// or do a combination of both.
///
/// \ingroup SignalModule
///
/// \note These constants are only relevant if using a \pagereader.
///
/// \note Signals are a part of the \llapi only. The \hlapi will automatically use the
/// correct signals to correspond to suitable events during a document read.
///
/// \sa MMMReader_SignalEvent(), MMMReader_LedEvent(), MMMReader_SoundEvent()
typedef enum MMMReaderSignal
{	
	/// Signal to state that a document is being read.
	///
	/// \remarks This signal can have both an LED and a Sound associated with it.
	SIGNAL_READ = 0, 
	
	/// \deprecated This signal was used in a previous generation of the \sdk and is not
	/// required anymore.
	SIGNAL_UPDATE, 
	
	/// Signal to state that the \pagereader is in a standby/waiting mode.
	///
	/// \remarks This signal can have both an LED and a Sound associated with it.
	SIGNAL_STANDBY, 
	
	/// Signal to state that an error occurred during a document read.
	///
	/// \remarks This signal can have both an LED and a Sound associated with it.
	SIGNAL_READERROR, 
	
	/// Signal to state that a document read was successful.
	///
	/// \remarks This signal can have both an LED and a Sound associated with it.
	SIGNAL_SUCCESS,
	
	/// Signal to state that a new document was found on the window of the \pagereader.
	///
	/// \remarks This signal can have both an LED and a Sound associated with it.
	SIGNAL_DOCFOUND, 
	
	/// Signal to state that the current document <em>can be</em> removed from the window of 
	/// the \pagereader. Note that this is not the same as when a document <em>has been</em>
	/// removed.
	///
	/// \remarks This signal can have both an LED and a Sound associated with it.
	SIGNAL_DOCREMOVE, 
	
	/// Signal to state that the current document <em>has been</em> removed from the window
	/// of the \pagereader.
	///
	/// \remarks This signal can have both an LED and a Sound associated with it.
	SIGNAL_DOCOFFWINDOW,
	
	/// Signal to state that the \sdk has finished processing a document.
	///
	/// \remarks This signal can have both an LED and a Sound associated with it.
	SIGNAL_ENDOFDOC,
	
	/// Signal to state that a generic error occurred.
	///
	/// \remarks This signal can have both an LED and a Sound associated with it.
	SIGNAL_ERROR, 

	/// \deprecated This signal was used in a previous generation of the \sdk and is not
	/// required anymore.
	SIGNAL_NOCONNECT,
	
	/// Signal to state that the \pagereader is in an "Enabled" state, i.e. it is able to
	/// process document reads.
	///
	/// \remarks This signal can only have an LED associated with it.
	SIGNAL_ENABLED, 
	
	/// Signal to state that the \pagereader is in a "Disabled" state, i.e. it is running but
	/// not able to process document reads.
	///
	/// \remarks This signal can only have an LED associated with it.
	SIGNAL_DISABLED, 
	
	/// Signal to state that the \pagereader is in an "Asleep" state, i.e. it is in a low power
	/// state.
	///
	/// \remarks This signal can only have an LED associated with it.
	SIGNAL_ASLEEP,

	/// Signal to state that the \pagereader has started up.
	///
	/// \remarks This signal can only have an LED associated with it.
	SIGNAL_POWER, 
	
	/// Signal to state the the \pagereader has shutdown.
	///
	/// \remarks This signal can only have an LED associated with it.
	SIGNAL_SHUTDOWN, 
	
	/// \deprecated This signal was used in a previous generation of the \sdk and is not
	/// required anymore.
	SIGNAL_TESTCARD,

	/// User-defined signal that can be configured to trigger an LED and/or Sound.
	SIGNAL_CUSTOM1, 
	
	/// User-defined signal that can be configured to trigger an LED and/or Sound.
	SIGNAL_CUSTOM2, 
	
	/// User-defined signal that can be configured to trigger an LED and/or Sound.
	SIGNAL_CUSTOM3, 
	
	/// User-defined signal that can be configured to trigger an LED and/or Sound.
	SIGNAL_CUSTOM4, 
	
	/// User-defined signal that can be configured to trigger an LED and/or Sound.
	SIGNAL_CUSTOM5,

	/// Signal to state that a mag read error occurred during a mag swipe.
	SIGNAL_MAG_READ_ERROR, 
	
	/// Signal to state that a mag read success occurred during a mag swipe.
	SIGNAL_MAG_READ_SUCCESS,

	// this is here simply to give the number signals that are defined.
	/// \internal
	SIGNAL_COUNT
	
} MMMReaderSignal;

/// \endcond	
// PAGESDK


// *** IF YOU ADD TO THIS ENUM PLEASE EDIT C# AND JAVA WRAPPERS ***
// *** Error string table defined in ErrorString.cpp must match with this enum. ***
/// Defines the error codes that can be returned from the functions in \sdk.
///
/// \ingroup ErrorsModule
typedef enum MMMReaderErrorCode
{
	NO_ERROR_OCCURRED = 0,					///< No error occurred, an operation was successful
	UNKNOWN_ERROR_OCCURRED,					///< An unknown error occurred. Contact \ssd for further help.
	ERROR_FEATURE_NOT_ENABLED,				///< A particular feature of the \sdk has not been enabled.
	ERROR_FEATURE_NOT_SUPPORTED,			///< A particular feature of the \sdk is not supported by the connected scanner

// Initialisation issues
	ERROR_NOT_INITIALISED,					///< A module has not yet been initialised.
	ERROR_ALREADY_INITIALISED,				///< A module has already been initialised.
	ERROR_INITIALISATION_FAILED,			///< Initialisation of a module failed. See the log file for more details.
	ERROR_DEVICE_SUSPENDED,					///< An operation could not be carried out because a device is currently suspended.
	ERROR_DEVICE_ALREADY_IN_USE,			///< An operation could not be carried out because it is already in use by something else.
	
	/// \if PAGESDK
	/// A \pagereader is not connected.
	/// \else
	/// \attention This error code is not used by the \sdk
	/// \endif
	ERROR_READER_NOT_CONNECTED,
	
// Data errors
	ERROR_UNKNOWN_DATA_FORMAT,				///< \internal

// File based errors
	ERROR_FILE_DOES_NOT_EXIST,				///< A particular file does not exist.
	ERROR_READING_FILE,						///< Failed to read a file.
	ERROR_WRITING_FILE,						///< Failed to write to a file.
	ERROR_CANNOT_OBTAIN_VERSION,			///< Cannot retrieve binary executable version information.
	ERROR_CANNOT_OBTAIN_MODULE_INFO,		///< Cannot retrieve binary executable module information.
	
	/// \if PAGESDK
	/// Cannot retrieve the serial number of a \pagereader.
	/// \else
	/// \attention This error code is not used by the \sdk.
	/// \endif
	ERROR_CANNOT_LOCATE_SERIAL,
	
	/// An error occurred loading an external library module at runtime.
	///
	/// \remarks On Windows&tm; operating systems, the message returned from the Windows&tm; API
	/// detailing why the error occurred will be included as part of the error and log message.
	ERROR_LOADING_DLL,						
	
// Settings issues
	ERROR_NO_ROOT_INI_FILE,					///< \internal
	ERROR_INVALID_CUSTOM_SETTINGS_PATH,		///< \internal

	/// The format of a config file was incorrect
	ERROR_INVALID_CONFIG_FILE_FORMAT,
	
// OS issues
	
	/// An error occurred whilst using an API from the operating system.
	///
	/// \remarks On Windows&tm; operating systems, the message returned from the Windows&tm; API
	/// detailing why the error occurred will be included as part of the error and log message.
	ERROR_OS_ERROR,

	/// A memory allocation error occurred.
	ERROR_ALLOCATING_MEMORY,

// Issues relating to input parameters
	ERROR_PARAMETER_INVALID,				///< A parameter passed into a \sdk function was incorrect.
	ERROR_INDEX_OUT_OF_BOUNDS,				///< An index value was "out of bounds", i.e. it was not within the minimum and maximim allowed range.

	/// A string buffer passed into a \sdk function was not large enough for the operation to continue.
	///
	/// \remarks If a function returns this error code, it will also update the appropariate
	/// parameter to represent the correct length required. Therefore, this information can
	/// be used to construct a buffer of the appropriate size and try again.
	ERROR_STRING_BUFFER_TOO_SMALL,			
	
	/// A data buffer passed into a \sdk function was not large enough for the operation to continue.
	///
	/// \remarks If a function returns this error code, it will also update the appropariate
	/// parameter to represent the correct length required. Therefore, this information can
	/// be used to construct a buffer of the appropriate size and try again.
	ERROR_DATA_BUFFER_TOO_SMALL,	
	
	/// \if PAGESDK
	/// \internal
	/// \else
	/// \attention This error code is not used by the \sdk
	/// \endif
	ERROR_IMAGE_TOO_SMALL,
	
	/// \if PAGESDK
	/// An image is not in a format the \sdk recognises.
	/// \else
	/// \attention This error code is not used by the \sdk
	/// \endif
	ERROR_IMAGE_WRONG_FORMAT,
	
	/// \if PAGESDK
	/// Failed to gain access to the internal data of an image.
	///
	/// \remarks This error is raised from the GDI+ operation "LockBits" to gain access to a
	/// readable/writable buffer of image information. See the GDI+ documentation for more
	/// details.
	///
	/// \else
	/// \attention This error code is not used by the \sdk
	/// \endif
	ERROR_IMAGE_LOCKBITS_FAILED,

// State errors
	ERROR_TIMED_OUT,						///< An operation took too long to complete.
	ERROR_CURRENTLY_IN_USE,					///< Something is currently in use, blocking the next operation.
	ERROR_OPERATION_CANCELLED,				///< The \sdk aborted an operation.
	ERROR_ALREADY_STARTED,					///< An operation/process has already started.
	ERROR_NOT_STARTED,						///< An operation/process has not started yet.

// Hardware issues

	/// \if PAGESDK
	/// The camera of a \pagereader could not be detected.
	/// \else
	/// \attention This error code is not used by the \sdk
	/// \endif
	ERROR_CAMERA_NOT_FOUND,
	
	/// \if PAGESDK
	/// Unable to gain exclusive access to the \pagereader camera.
	/// \else
	/// \attention This error code is not used by the \sdk
	/// \endif
	ERROR_OPENING_CAMERA,
	
	/// \if PAGESDK
	/// A camera hardware error occurred. Contact \ssd for further help.
	/// \else
	/// \attention This error code is not used by the \sdk
	/// \endif
	ERROR_CAMERA_DRIVER_ERROR,

	/// \if PAGESDK
	/// A camera hardware error occurred. Contact \ssd for further help.
	/// \else
	/// \attention This error code is not used by the \sdk
	/// \endif
	ERROR_PREVIOUS_CAMERA_DRIVER_ERROR,
	
	/// \if PAGESDK
	/// The camera device settings and the \sdk configuration settings do not match.
	/// \else
	/// \attention This error code is not used by the \sdk
	/// \endif
	ERROR_CAMERA_AND_SETTINGS_MISMATCH,

	/// \if PAGESDK
	/// The settings object buffer provided is of a different size to the one used by the SDK.
	/// \else
	/// \attention This error code is not used by the \sdk
	/// \endif
	ERROR_WRONG_SIZE_SETTINGS_BUFFER,

	/// \if PAGESDK
	/// USB 1.1 connection is not supported for this \pagereader.	
	/// \else
	/// \attention This error code is not used by the \sdk
	/// \endif
	ERROR_USB1_NOT_SUPPORTED,
	
	/// \if PAGESDK
	/// The correct camera drivers are not installed.
	/// \else
	/// \attention This error code is not used by the \sdk
	/// \endif
	ERROR_INCORRECT_DRIVERS,
	
	/// \if PAGESDK
	/// \internal
	/// \else
	/// \attention This error code is not used by the \sdk
	/// \endif
	ERROR_GPIO,
	
	/// \ifnot SWIPESDK
	/// RFID hardware was not found.
	/// \else
	/// \attention This error code is not used by the \sdk
	/// \endif
	ERROR_RF_DEVICE_NOT_FOUND,
	
	ERROR_UNRECOGNISED_DEVICE,				///< \internal
	ERROR_DEVICE_NOT_PRESENT,				///< A device/hardware component is not present.
	ERROR_CANNOT_DISABLE_DEVICE,			///< Cannot disable a hardware component via the operating system.
	ERROR_CANNOT_ENABLE_DEVICE,				///< Cannot enable a hardware component via the operating system.
	ERROR_VOLUME_MISSING,					///< \internal
	ERROR_NO_AVAILABLE_DRIVES,				///< \internal
	ERROR_CANNOT_GET_DEVICE_DETAILS,		///< Cannot get details for a hardware component.
	ERROR_UNKNOWN_USB_REPORT,				///< \internal
	ERROR_GETTING_USB_VALUE,				///< \internal
	ERROR_CAMERA_DEVICE_DISABLED,			///< Camera device driver appears to be disabled.

// RF issues

	/// \ifnot SWIPESDK
	/// Cannot connect to an RF chip in a document.
	/// \else
	/// \attention This error code is not used by the \sdk
	/// \endif
	ERROR_RF_BIOCHIP_DEAD,
	
	/// \ifnot SWIPESDK
	/// An invalid data group was requested from an RF chip.
	/// \else
	/// \attention This error code is not used by the \sdk
	/// \endif
	ERROR_RF_INVALID_DATAGROUP,
	
	/// \ifnot SWIPESDK
	/// Cannot select an RF chip in a document.
	/// \else
	/// \attention This error code is not used by the \sdk
	/// \endif
	ERROR_RF_SELECT_APP_FAILED,
	
	/// \ifnot SWIPESDK
	/// A requested data group was not present in the RF chip.
	/// \else
	/// \attention This error code is not used by the \sdk
	/// \endif
	ERROR_RF_DG_NOT_PRESENT,

	/// \ifnot SWIPESDK
	/// Failed to retrieve a data item from the RF chip.
	/// \else
	/// \attention This error code is not used by the \sdk
	/// \endif
	ERROR_RF_GET_DATA_ITEM_FAILED,
	
	/// \ifnot SWIPESDK
	/// Failed to validate a data item from the RF chip.
	/// \else
	/// \attention This error code is not used by the \sdk
	/// \endif
	ERROR_RF_VALIDATE_DATA_ITEM_FAILED,
	
	/// \ifnot SWIPESDK
	/// Failed to decode a data item from the RF chip.
	/// \else
	/// \attention This error code is not used by the \sdk
	/// \endif
	ERROR_RF_DECODE_DATA_ITEM_FAILED,
	
	/// \ifnot SWIPESDK
	/// Successfully decoded a data item from the RF chip, but the data is empty.
	/// \else
	/// \attention This error code is not used by the \sdk
	/// \endif
	ERROR_RF_DECODE_DATA_ITEM_EMPTY,

	/// \ifnot SWIPESDK
	/// Basic Access Control (BAC) failed.
	/// \else
	/// \attention This error code is not used by the \sdk
	/// \endif
	ERROR_RF_BAC_FAILURE,
	
	/// \ifnot SWIPESDK
	/// Active Authentication failed.
	/// \else
	/// \attention This error code is not used by the \sdk
	/// \endif
	ERROR_RF_ACTIVE_AUTHENTICATION_FAILURE,	
	
	/// \ifnot SWIPESDK
	/// Failed to validate signed attributes.
	/// \else
	/// \attention This error code is not used by the \sdk
	/// \endif
	ERROR_RF_SIGNED_ATTRS_FAILED,			
	
	/// \ifnot SWIPESDK
	/// Failed to validate signature.
	/// \else
	/// \attention This error code is not used by the \sdk
	/// \endif
	ERROR_RF_SIGNATURE_FAILED,
	
	/// \ifnot SWIPESDK
	/// The RFID reader is not functioning.
	/// \else
	/// \attention This error code is not used by the \sdk
	/// \endif
	ERROR_RF_READER_DEAD,
	
	/// \ifnot SWIPESDK
	/// RF APDU error: no tag in field.
	/// \else
	/// \attention This error code is not used by the \sdk
	/// \endif
	ERROR_RF_APDU_NO_TAG_IN_FIELD,			
	
	/// \ifnot SWIPESDK
	/// RF APDU error: no response from request.
	/// \else
	/// \attention This error code is not used by the \sdk
	/// \endif
	ERROR_RF_APDU_NO_RESPONSE,
	
	/// \ifnot SWIPESDK
	/// RF APDU error: Cyclic Redundancy Checksum failure.
	/// \else
	/// \attention This error code is not used by the \sdk
	/// \endif
	ERROR_RF_APDU_CRC_FAILURE,
	
	/// \ifnot SWIPESDK
	/// RF APDU error: a general error occurred.
	/// \else
	/// \attention This error code is not used by the \sdk
	/// \endif
	ERROR_RF_APDU_GENERAL_FAILURE,
	
	/// \ifnot SWIPESDK
	/// RF ADPU error: an error response was received.
	/// \else
	/// \attention This error code is not used by the \sdk
	/// \endif
	ERROR_RF_APDU_ERR_RESPONSE,
	
	/// \ifnot SWIPESDK
	/// RF APDU error: unknown command.
	/// \else
	/// \attention This error code is not used by the \sdk
	/// \endif
	ERROR_RF_APDU_UNKNOWN_COMMAND,
	
	/// \ifnot SWIPESDK
	/// RF APDU error: operation mode failure.
	/// \else
	/// \attention This error code is not used by the \sdk
	/// \endif
	ERROR_RF_APDU_OP_MODE_FAILURE,
	
	/// \ifnot SWIPESDK
	/// RF APDU error: parameter out of range.
	/// \else
	/// \attention This error code is not used by the \sdk
	/// \endif
	ERROR_RF_APDU_PARAM_OUT_OF_RANGE,
	
	/// \ifnot SWIPESDK
	/// RF APDU error: access denied.
	/// \else
	/// \attention This error code is not used by the \sdk
	/// \endif
	ERROR_RF_APDU_ACCESS_DENIED,
	
	/// \ifnot SWIPESDK
	/// RF APDU error: an unknown error occurred.
	/// \else
	/// \attention This error code is not used by the \sdk
	/// \endif
	ERROR_RF_APDU_UNKNOWN_ERROR,
	
	/// \ifnot SWIPESDK
	/// Failed to decode the EF.COM file from the RF chip.
	/// \else
	/// \attention This error code is not used by the \sdk
	/// \endif
	ERROR_RF_DECODE_EF_COM_FAILURE,
	
	/// \ifnot SWIPESDK
	/// Failed to decode the EF.SOD file from the RF chip.
	/// \else
	/// \attention This error code is not used by the \sdk
	/// \endif
    ERROR_RF_DECODE_EF_SOD_FAILURE,
    
	/// \ifnot SWIPESDK
	/// Chip Authentication failed.
	/// \else
	/// \attention This error code is not used by the \sdk
	/// \endif
	ERROR_RF_CHIP_AUTHENTICATION_FAILED,
	
	/// \ifnot SWIPESDK
	/// Failed to load certificates required for the RF chip.
	/// \else
	/// \attention This error code is not used by the \sdk
	/// \endif
	ERROR_RF_CERTS_LOAD_FAILED,	
	
	/// \ifnot SWIPESDK
	/// Terminal Authentication failed.
	/// \else
	/// \attention This error code is not used by the \sdk
	/// \endif
	ERROR_RF_TERMINAL_AUTHENICATION_FAILED,	
	
	/// \ifnot SWIPESDK
	/// An operation tried to access a secure data group from the RF chip but was 
	/// unauthorized to do so.
	/// \else
	/// \attention This error code is not used by the \sdk
	/// \endif
	ERROR_RF_UNAUTHORISED_SENSITIVE_DG,
	
	/// \ifnot SWIPESDK
	/// No RF chip has been selected.
	/// \else
	/// \attention This error code is not used by the \sdk
	/// \endif
	ERROR_RF_NO_CHIP_SELECTED,
	
	/// \ifnot SWIPESDK
	/// The current RF read has been aborted and no further actions can be carried out for 
	/// the current document.
	/// \else
	/// \attention This error code is not used by the \sdk
	/// \endif
	ERROR_RF_ABORTED,
	
	/// \ifnot SWIPESDK
	/// Unknown RF reader index.
	/// \else
	/// \attention This error code is not used by the \sdk
	/// \endif
	ERROR_RF_READER_INDEX_UNKNOWN,
	
// Serial port errors
	ERROR_OPENING_COM_PORT,					///< An error occurred opening a COM port.
	ERROR_SETTING_COM_PORT_TIMEOUTS,		///< An error occurred configuring the COM port timeouts.
	ERROR_SETTING_COM_PORT,					///< An error occurred configuring a COM port.
	ERROR_CLOSING_COM_PORT,					///< An error occurred closing a COM port.
	ERROR_READING_COM_PORT,					///< An error occurred reading from a COM port.
	ERROR_WRITING_COM_PORT,					///< An error occurred writing to a COM port.
	
// Device errors
	ERROR_CONNECTING_DEVICE,				///< Failed to connect to a device/hardware component.
	ERROR_DISCONNECTING_DEVICE,				///< Failed to disconnect from a device/hardware component.
	
// Swipe reader errors
	ERROR_SWIPE_READER_NOT_CONNECTED,		///< A swipe reader was expected but is not connected to the host PC.
	ERROR_MSR_NOT_CONNECTED,				///< An MSR was expected but is not connected to the host PC.
	ERROR_SW_UNKNOWN_PROTOCOL,				///< An unrecognised \swipereader protocol was specified.
	ERROR_SW_NO_SERIAL_DATA,				///< No data was returned from a \swipereader.
	ERROR_SW_COMMAND_PARAMETER_INVALID,		///< \internal
	ERROR_SW_UNKNOWN_PROTOCOL_COMMAND,		///< \internal
	ERROR_SW_BAD_CHARACTER,					///< A bad character was found in the data returned from a \swipereader.
	ERROR_SW_CANNOT_PROCESS_COMMANDS,		///< The connected \swipereader has not been configured to accept commands from the \sdk.
	ERROR_SW_INVALID_PROTOCOL_COMMAND,		///< The \swipereader command is not supported by the currently configured \swipereader protocol.
	ERROR_SW_PROTOCOL_COMMAND_FAILED,		///< Failed to create a command for a \swipereader.
	ERROR_SW_COMMAND_NOT_ACKNOWLEDGED,		///< The \swipereader returned a negative-acknowledgement (#NACK) value.
	
// Swipe reader RTE Protocol errors
	ERROR_SW_RTE_SOH_MISSING,				///< RTE swipe protocol error: #SOH byte missing.
	ERROR_SW_RTE_INCORRECT_MESSAGE_LENGTH,	///< RTE swipe protocol error: message length does not match specified length.
	ERROR_SW_RTE_STX_MISSING,				///< RTE swipe protocol error: #STX byte missing.
	ERROR_SW_RTE_ETX_MISSING,				///< RTE swipe protocol error: #ETX byte missing.
	ERROR_SW_RTE_NO_MESSAGE_CONTENT,		///< RTE swipe protocol error: no message content.
	ERROR_SW_RTE_BAD_MESSAGE_CHECKSUM,		///< RTE swipe protocol error: bad message checksum.
	ERROR_SW_RTE_BAD_TRACK_CHECKSUM,		///< RTE swipe protocol error: bad track checksum.
	ERROR_SW_RTE_BAD_BLOCK_CHECKSUM,		///< RTE swipe protocol error: bad block checksum.
	
// Swipe reader RTE QA Protocol errors
	ERROR_SW_RTE_QA_STX_MISSING,			///< RTE QA swipe protocol error: #STX byte missing.
	ERROR_SW_RTE_QA_ETX_MISSING,			///< RTE QA swipe protocol error: #ETX byte missing.
	ERROR_SW_RTE_QA_NO_MESSAGE_CONTENT,		///< RTE QA swipe protocol error: no message content.
	ERROR_SW_RTE_QA_INCORRECT_LINE_COUNT,	///< RTE QA swipe protocol error: incorrect number of lines present.
	ERROR_SW_RTE_QA_INCORRECT_PARAM_COUNT,	///< RTE QA swipe protocol error: incorrect number of parameters present.
	
// Swipe reader MUSE Protocol errors
	ERROR_SW_MUSE_STX_MISSING,				///< MUSE swipe protocol error: #STX byte missing.
	ERROR_SW_MUSE_ETX_MISSING,				///< MUSE swipe protocol error: #ETX byte missing.
	ERROR_SW_MUSE_NO_MESSAGE_CONTENT,		///< MUSE swipe protocol error: no message content.
	ERROR_SW_MUSE_UNKNOWN_DEVICE_TYPE,		///< MUSE swipe protocol error: unknown device type read.
	ERROR_SW_MUSE_BAD_CRC16,				///< MUSE swipe protocol error: bad checksum.
	
// Swipe reader CUTE Protocol errors
	ERROR_SW_CUTE_SOD_MISSING,				///< CUTE swipe protocol error: #CPSB_SOD1 or #CPSB_SOD2 byte missing.
	ERROR_SW_CUTE_STX_MISSING,				///< CUTE swipe protocol error: #STX byte missing.
	ERROR_SW_CUTE_ETX_MISSING,				///< CUTE swipe protocol error: #ETX byte missing.
	ERROR_SW_CUTE_EOD_MISSING,				///< CUTE swipe protocol error: #CPSB_EOD1 or #CPSB_EOD2 byte missing.
	ERROR_SW_CUTE_NO_MESSAGE_CONTENT,		///< CUTE swipe protocol error: no message content.
	ERROR_SW_CUTE_UNKNOWN_DEVICE_TYPE,		///< CUTE swipe protocol error: unknown device type read.
	
// Swipe reader Magtek MSR Protocol errors
	ERROR_SW_MAGMSR_TRACK_ERROR,			///< Magtek swipe protocol error: failed to decode track.

// Swipe reader TECS Protocol errors
	ERROR_SW_TECS_GS_MISSING,				///< TECS swipe protocol error: #GS byte missing.
	ERROR_SW_TECS_EOT_MISSING,				///< TECS swipe protocol error: #EOT byte missing.
	ERROR_SW_TECS_NO_MESSAGE_CONTENT,		///< TECS swipe protocol error: no message content.
	ERROR_SW_TECS_UNKNOWN_DEVICE_TYPE,		///< TECS swipe protocol error: unknown device type read.

// UHF errors
	
	/// \ifnot SWIPESDK
	/// UHF error: The UHF module is not functioning
	/// \else
	/// \attention This error code is not used by the \sdk
	/// \endif
	ERROR_UHF_READER_DEAD,

	/// \ifnot SWIPESDK
	/// UHF error: failed to read the TagID
	/// \else
	/// \attention This error code is not used by the \sdk
	/// \endif
	ERROR_UHF_READ_TAGID,
	
	/// \ifnot SWIPESDK
	/// UHF error: failed to read EPC from tag
	/// \else
	/// \attention This error code is not used by the \sdk
	/// \endif
	ERROR_UHF_READ_EPC,
	
	/// \ifnot SWIPESDK
	/// UHF error: failed to read memory bank from tag
	/// \else
	/// \attention This error code is not used by the \sdk
	/// \endif
	ERROR_UHF_READ_MEMORY_BANK,

	/// \ifnot SWIPESDK
	/// UHF error: An error occurred during a UHF read
	/// \else
	/// \attention This error code is not used by the \sdk
	/// \endif
	ERROR_UHF_READ_ERROR,

	/// \ifnot SWIPESDK
	/// UHF error: A Tag was found, but does not have an entry in the UHF memory map
	/// XML file.
	/// \else
	/// \attention This error code is not used by the \sdk
	/// \endif
	ERROR_UHF_TAG_MEMORY_MAP_UNKNOWN,

	/// \ifnot SWIPESDK
	/// UHF error: A tag was lost mid-way through a read
	/// \else
	/// \attention This error code is not used by the \sdk
	/// \endif
	ERROR_UHF_TAG_LOST,

// Decode errors

	/// \ifnot SWIPESDK
	/// A decoder encountered an error.
	/// \else
	/// \attention This error code is not used by the \sdk
	/// \endif
	ERROR_DECODE_ERROR,
	
// High Level only

	/// \if PAGESDK
	/// Cannot change state in the \hlapi - the current state does not allow the requested 
	/// state change.
	/// \else
	/// \attention This error code is not used by the \sdk
	/// \endif
	ERROR_INVALID_STATE_CHANGE,
	
	/// \if PAGESDK
	/// The \hlapi operation can only be used in Blocking mode.
	/// \else
	/// \attention This error code is not used by the \sdk
	/// \endif
	ERROR_BLOCKING_ONLY,
		
	ERROR_MID_STATECHANGE,					///< \deprecated Not used.

	/// \if PAGESDK
	/// No document is on the window of the \pagereader.	
	/// \else
	/// \attention This error code is not used by the \sdk
	/// \endif
	ERROR_NO_DOC_ON_WINDOW,

// Memory codes
	ERROR_MMMR_SETTNGS_NOT_FOUND,			///< \deprecated Not used.
	ERROR_MEMORY_PARSING_ERROR,				///< \deprecated Not used.
	ERROR_UNINITIALISED_EEPROM,				///< \deprecated Not used.

// twain optical reader (two sided id card reader)
	/// \if PAGESDK
	/// Operation can only be used after a motor feed command has been issued.
	/// \else
	/// \attention This error code is not used by the \sdk
	/// \endif
	ERROR_NO_MOTOR_FEED_CALL_ISSUED, 
	
// Plugin errors

	/// \if PAGESDK
	/// An error occurred within a \sdk Plugin.
	/// \else
	/// \attention This error code is not used by the \sdk
	/// \endif
	ERROR_PLUGIN_ERROR,

//#ifdef DCA
	ERROR_PARAMETER_INCORRECT,				///< Parameter, when parsing, wasn't as expected
	ERROR_LAYOUT_LOAD_FAILURE,				///< Initialisation of the Layout Database module failed. See the log files for more details. TL
	ERROR_UNKNOWN_DATA_ITEM,				///< The requested data item is not available, TL
	ERROR_LAYOUT_DATABASE_ISSUE,			///< There is an issue within the layout database
	ERROR_IMAGE_NOT_AVAILABLE,				///< Image is not available
	ERROR_CANNOT_FIND_LAYOUT_OBJECT,		///< Cannot locate the named layout database object
	ERROR_INVALID_STATE_FOR_REQUEST,		///< Invalid state for request
	ERROR_LICENSE_FILE_DOES_NOT_EXIST,		///< License file can not be found
	ERROR_NO_LICENSE_FILE_LOADED,			///< No license file loaded
	ERROR_LICENSE_FILE_EXPIRED,				///< License file expired
	ERROR_LICENSE_FILE_INVALID,				///< License file is not valid
	ERROR_LICENSE_CUSTOMER_NUMBER_INVALID,	///< License file Customer Number invalid
	ERROR_LICENSE_COMPUTER_NAME_INVALID,	///< License file Computer Name invalid
	ERROR_LICENSE_MACHINE_FINGERPRINT_INVALID,	///< License file Machine Fingerprint invalid
	ERROR_OCR_FAILURE,
	ERROR_GET_DATA_ITEM_FAILURE,			///< The named Data Item can not be retrieved
	ERROR_INSUFFICIENT_PERMISIONS,			///< Insufficient permission for action
	ERROR_PLUGABLE_ERROR,					///< 
	ERROR_BARCODE_PLUGIN_UNAVAILABLE,		///< Barcode plugin is not available
	ERROR_NO_FLIPSIDE_AVAILABLE,			///< No flipside links to this document
	ERROR_INVALID_STATE_FOR_DATA_RETRIEVAL,	///< Invalid state for get item request

	ERROR_READING_REFERENCE_FILE_PROCESS,		///< Process (%s) Reference file could not be loaded from disk - %d - %s
	ERROR_READING_REFERENCE_FILE_IDENTIFIER,	///< Identifier (%s) Reference file could not be loaded from disk - %d - %s
	ERROR_INCORRECT_PIXEL_FORMAT_PROCESS,		///< Process (%s) Image pixel format must be %s
	ERROR_INCORRECT_PIXEL_FORMAT_IDENTIFIER,	///< Identifier (%s) Image pixel format must be %s
	ERROR_REFERENCE_FILE_USAGE,				///< Reference file could not be used %s
	ERROR_CREATING_EIGEN_REFERENCE_FILE,	///< Eigen Reference file could not be created %s
	ERROR_READING_EIGEN_REFERENCE_FILE,		///< Eigen Reference file could not be loaded from disk %s
	ERROR_WRITING_EIGEN_REFERENCE_FILE,		///< Eigen Reference file could not be written to disk %s
	ERROR_MISMATCHING_EIGEN_REFERENCE_FILE,	///< Eigen Reference file does not match to identifier width/height %d, %d
	ERROR_CREATING_EIGEN_MATRIX,			///< Eigen Matrix file could not be created %s
	ERROR_READING_EIGEN_MATRIX,				///< Eigen Matrix file could not be loaded from disk %s
	ERROR_CREATING_EIGEN_COEFFICIENT,		///< Eigen coeffecient could not be created %s
	ERROR_CALCULATING_EIGEN_VECTOR, 		///< Eigen vector can not be calculated %s
	ERROR_LOADING_IMAGE_INTO_OCR_TOOLKIT,	///< Image could not be loaded into OCR toolkit %s
	ERROR_PARAMETER_INVALID_PROCESS,		///< A parameter passed into process (%s) was incorrect.
	ERROR_PARAMETER_INVALID_IDENTIFIER,		///< Identifier (%s) A parameter passed into identifier (%s) was incorrect
	ERROR_PARAMETER_INVALID_VERIFIER,		///< Verifier (%s) A parameter passed into verifier (%s) was incorrect
	ERROR_SEARCH_ZONE_INCORRECT,			///< Identifier (%s) The search zone provide is incorrect (%s)
	ERROR_REFERENCE_FILE_LEARN,				///< Reference file name unavailable (%s - %s)
	ERROR_REFERENCE_FILE_REDUCTION,			///< Reference file width/height after reduction is too small
	ERROR_REFERENCE_PIXEL_FORMAT_PROCESS,		///< Process (%s) Reference image pixel format must be %s
	ERROR_REFERENCE_PIXEL_FORMAT_IDENTIFIER,	///< Identifier (%s) Reference image pixel format must be %s
	ERROR_SUBIMAGE_EXTRACT,					///< Subimage can not be extracted for further processing
//#endif //DCA

	NUM_ERROR_CODES,						///< This is the last "normal" error code, and is used for testing validity.

	// 16-bits of custom error codes should be plenty of space
	CUSTOM_ERROR_CODE_MIN = 0x10000,		///< Minimum value of the range of custom error codes that can be applied.
	CUSTOM_ERROR_CODE_MAX = 0x1ffff,		///< Maximum value of the range of custom error codes that can be applied.

	/// The starting value for the range of operating system exceptions that can occur, for 
	/// example (on Windows&tm;) \c ACCESS_VIOLATION = \c 0xC0000005
	ERROR_EXCEPTION_OCCURRED = 0xC0000000L
	
} MMMReaderErrorCode;
// *** IF YOU ADD TO THIS ENUM PLEASE EDIT C# AND JAVA WRAPPERS ***

/// \cond INTERNAL
#define		FIRST_RF_ERROR_CODE		ERROR_RF_BIOCHIP_DEAD
#define		LAST_RF_ERROR_CODE		ERROR_RF_READER_INDEX_UNKNOWN
/// \endcond	
// INTERNAL

// *** IF YOU ADD TO THIS ENUM PLEASE EDIT C# AND JAVA WRAPPERS ***
// Warning string table defined in ErrorString.cpp must match with this enum.
/// Defines the warning codes that can be sent to the warning callback #MMMReaderWarningCallback.
///
/// \ingroup WarningsModule
typedef enum MMMReaderWarningCode
{
	UNKNOWN_WARNING_OCCURRED,					///< An unknown warning occurred. Check aWarningMsg for further information.

	WARNING_CR5400_ARM_TIMEOUT,					///< CR5400 arm timeout failure.

	NUM_WARNING_CODES
} MMMReaderWarningCode;

/// \cond PAGESDK

/// Defines the different states that the \sdk and \pagereader can be in.
///
/// \ingroup HighLevelInitStateModule
///
/// \note These constants are only relevant if using a \pagereader with the \hlapi.
///
/// \sa MMMReader_GetState(), MMMReader_SetState()
typedef enum ReaderState 
{
	/// Either the \hlapi has not been initialised via MMMReader_Initialise(), or
	/// MMMReader_Shutdown() has been called.
	READER_NOT_INITIALISED, 
	
	/// The \pagereader has been initialised and is currently waiting for a document to
	/// be placed upon the window. When a document is detected, the \sdk will then read
	/// and process the document.
	READER_ENABLED, 
	
	/// The \pagereader has been initialised and is monitoring for new documents. However, it
	/// will not read or process a document when it is placed upon the window.
	///
	/// \remarks While in this state, the #DOC_ON_WINDOW and #DOC_REMOVED events will be
	/// the only Non-Blocking events triggered to alert to the presence/removal of a document.
	/// Alternatively, the MMMReader_IsDocumentOnWindow() API can also be used to detect
	/// the presence of a document while in this state.
	READER_DISABLED, 
	
	/// The \pagereader has been initialised but has been switched into a low power mode.
	/// The \sdk will not monitor for new documents and will not read or process documents.
	READER_ASLEEP, 
	
	/// The \sdk is currently reading a document placed on the \pagereader window.
	///
	/// \remarks While in this state, some operations may not be able to be carried out until
	/// the document read has completed.
	READER_READING, 
	
	/// The \sdk has reported an error. While in this state, the \sdk will automatically
	/// attempt to recover from the error; if successful, it will automatically restore the
	/// previous #ReaderState.
	///
	/// \sa MMMReader_Reset()
	READER_ERRORED, 
	
	/// The \sdk has reported a fatal error that is non-recoverable. The \sdk and/or
	/// \pagereader must be reset.
	READER_FATAL_ERRORED, 
	
	/// The \sdk has been shutdown via MMMReader_Shutdown().
	READER_TERMINATED, 
	
	/// Similar to #READER_ASLEEP, except that the \sdk will disconnect from all physical 
	/// devices that are part of the \pagereader (e.g. RFID, Camera, etc.) This lowers power
	/// and CPU load even further.
	///
	/// \remarks While in this state, it is also possible to access the \pagereader from within
	/// another application. This allows a single \pagereader to be shared between multiple
	/// applications running on the same PC.
	READER_SUSPENDED,

	/// Similar to #READER_ENABLED, except that the \sdk will not connect to any physical 
	/// devices. This state is for the Layout Analysis tool and when the client wishes to feed/process
	/// images to the sdk from an external source (other than a physical 3M reader)
	///
	/// \remarks While in this state, all images/data must be set via the SetItemData api.
	READER_NO_HARDWARE
} ReaderState;


/// Defines the different sound modes the \sdk supports. These values are set to the
/// #SoundSettings::puMode member, which is part of the #MMMReaderSettings::puSoundSettings.
///
/// \ingroup SignalModule
///
/// \note These constants are only relevant if using a \pagereader.
typedef enum MMMReaderSoundMode
{
	/// Use this value to not use any sounds.
	SM_NONE, 
	
	/// Use this value to generate system beeps via the operating system.
	SM_BEEP, 
	
	/// Use this value to play WAV files containing speech samples.
	SM_WAV_SPEECH, 
	
	/// Use this value to play WAV files containing sound effects.
	SM_WAV_EFFECTS 
	
} MMMReaderSoundMode;


// PostProcessingOperations

// Useful union conversion from combined bitflags to itemised bitflags
typedef struct PostProcessOperations
{
	unsigned int puDemosaic : 1;
	unsigned int puNormalise : 1;
	unsigned int puSmall : 1;
	unsigned int puCrop : 1;
	unsigned int puRotate : 1;
	unsigned int puDebarrel : 1;
	unsigned int puRelocateDocument : 1;
	unsigned int puSharpen : 1;
	unsigned int puFullGrey : 1;
	unsigned int puGamma : 1;
	unsigned int puContrast : 1;
	unsigned int puColourHue : 1;
	unsigned int puEdgeEnhance : 1;
	unsigned int puTrueColour : 1;
} PPOPS;

typedef union PostProcessOperationsUnion
{
	PPOPS puBitFlags;
	unsigned int puValue;
} PPOPS_UNION;

/// Bit-flag that for MMMReader_ImagePostProcess indicating no processing should be done
#define PP_NONE				(0x0000)

/// Bit-flag that for post process indicating the image should be demosaic'd (use 
/// for vis light images)
#define PP_DEMOSAIC			(0x0001)

/// Bit-flag that for post process indicating the image should be normalised
#define PP_NORMALISE		(0x0002)

/// Bit-flag that for post process indicating the image should be shrunk to quarter size. 
/// Deprecated - Recommend resizing the image yourself if this is required.
#define PP_SMALL			(0x0004)

/// Bit-flag that for post process indicating the image should be cropped 
/// to the supplied region (as identified by the locate / detect document APIs
#define PP_CROP				(0x0008)

/// Bit-flag that for post process indicating the image should be rotated
/// to the supplied angle (as identified by the locate / detect document APIs
#define PP_ROTATE			(0x0010)


/// Bit-flag that for post process indicating the image should be debarrelled to correct
/// the image from any barrel distortion (not all readers require debarrelling, but if
/// the reader doesn't require it, this flag will be ignored).
#define PP_DEBARREL			(0x0020)

/// Bit-flag for post process indicating the image should be analysed to locate the edges
/// of the document. This is primarily of use when reprocessing images separate to the
/// locate document call.
#define	PP_RELOCATEDOCUMENT	(0x0040)

/// Bit-flag that for post process indicating the image should be sharpened.
#define PP_SHARPEN			(0x0080)

/// Bit-flag that for post process indicating the image should be processed to have a
/// full range of grey-shades, by applying a form of histogram stretch.
/// This is deprecated, and should typically not be used if you wish colour correct images.
#define PP_FULLGREY			(0x0100)

/// Bit-flag that for post process indicating the image should be gamma corrected. 
/// If supplied in combination with PP_TRUE_COLOUR, aGammaVal can be set to value !+ 1.0
/// to set the gamma to a specific value. 
#define PP_GAMMA			(0x0200)

/// Bit-flag that for post process indicating the image should have contrast enhancement
/// applied.
#define PP_CONTRAST			(0x0400)

/// Bit-flag that for post process indicating the image should have colour hue adjustments.
/// This is deprecated, and should typically not be used.
#define PP_COLOURHUE		(0x0800)

/// Bit-flag that for post process indicating the image should have an edge enhancement filter
/// applied.
#define	PP_EDGE_ENHANCE		(0x1000)

/// Bit-flag that for post process indicating the image should have "true colour" applied,
/// to provide the image with better colour rendition. This flag should typically be passed 
/// in combination with aGammaVal being set to the value determined for the reader, 
/// and aColourMatrix set to the correct numeric value.
#define	PP_TRUE_COLOUR		(0x2000)

/// Bit-flag to indicate the image is from a cr5400 - two images front/back on the captured image
/// and needs to be split.
#define PP_SPLIT			(0x4000)

/// Bit-flag to indicate the image is from a cr5400 and needs to have the top/bottom edges aligned 
#define PP_ALIGNTOPBOTTOM	(0x8000)


/// \endcond	
// PAGESDK

/// Callback function definition for a user-defined function that will receive low-level data 
/// from the \sdk.
///
/// \if SWIPESDK
/// \ingroup SwipeDataModule
/// \else
/// \ingroup LowLevelModule
/// \endif
///
/// \if PAGESDK
/// \note This callback is only relevant if using the \llapi. The \hlapi has its own version
/// of this callback, #MMMReaderHLDataCallback, which simplfies how data is returned.
/// \endif
///
/// \param[in] aParam An optional pointer to some user-defined data. This is usually initialised
/// when setting the callback function in question.
///
/// \param[in] aSequenceNum A user-defined value that identifies what data is coming back 
/// through the callback.
///
/// \param[in] aDataType A #MMMReaderDataFormat value specifying what kind of data is being 
/// returned.
///
/// \param[in] aDataLen The length of the returned data, measured in bytes.
///
/// \param[in] aDataPtr A pointer to the returned data.
///
/// \remarks \a aParam can be used to provide some context data back through the callback. One
/// example could be the \c this pointer from a C++ class; in this case, because the callback
/// function must be a C-style function, \a aParam would be able to access elements of its
/// class type.
///
/// \if SWIPESDK
///
/// \remarks \a aSequenceNum will be an integer value from the #MMMReaderSwipeItem enumeration.
/// See #MMMReaderSwipeItem for details on using \a aSequenceNum and casting the \c void*
/// \a aDataPtr to a more usable type.
///
/// \else
///
/// \remarks \a aSequenceNum is a user-defined value that is usually passed into one of the
/// \llapi functions at the point of requesting data back. This parameter is provided so that
/// it is possible to have one data callback defined for multiple data items; the defined
/// function can then filter the \a aSequenceNum as appropriate.
///
/// \remarks \a aDataPtr is returned as a \c void*, so it must be cast to an appropriate type
/// before it can be used. The type to cast to can be determined either by:
///		- Checking the data format via the \a aDataType parameter.
///		- Using the \a aSequenceNum parameter as knowledge of what the data should represent.
///
/// \endif
///
/// \if SWIPESDK
/// \sa #MMMReaderSwipeItem
/// \endif
typedef void (*MMMReaderDataCallback) (
	void *aParam, 
	int aSequenceNum, 
	MMMReaderDataFormat aDataType, 
	int aDataLen,
	void *aDataPtr
);

/// \cond PAGESDK

/// Callback function definition for a user-defined function that is used during RF read
/// operations that require a Basic Access Control (BAC) key. The defined function will
/// receive the last BAC key that was used, and the function should update the provided
/// buffer with a valid key to retry opening the RF chip.
///
/// \ingroup RfidModule
///
/// \param[in] aParam An optional pointer to some user-defined data. This is usually initialised
/// when setting the callback function in question.
///
/// \param[in] aSequenceNum A user-defined value that identifies what data is coming back 
/// through the callback.
///
/// \param[in] aCodelineBufferLen The length of the \a aCodelineBuffer.
///
/// \param[in,out] aCodelineBuffer A writable buffer that initially contains the current
/// BAC key. When the function returns, it should contain an updated valid BAC key.
///
/// \param[out] aChipRemovedFromField A boolean flag that the function should set if the RF
/// chip was removed from the RF field. Setting this to \c true instructs the \llapi to 
/// re-establish communications with the RF chip. This value is initially \c false.
///
/// \remarks \a aParam can be used to provide some context data back through the callback. One
/// example could be the \c this pointer from a C++ class; in this case, because the callback
/// function must be a C-style function, \a aParam would be able to access elements of its
/// class type.
///
/// \remarks \a aSequenceNum is a user-defined value that is usually passed into one of the
/// \llapi functions at the point of requesting data back. This parameter is provided so that
/// it is possible to have one data callback defined for multiple data items; the defined
/// function can then filter the \a aSequenceNum as appropriate.
///
/// \sa MMMReader_RFInitialise()
typedef void (*MMMReaderBACCallback) (
	void *aParam, 
	int aSequenceNum, 
	int aCodelineBufferLen, 
	char *aCodelineBuffer, 
	bool *aChipRemovedFromField
);

typedef enum AccessControlPasswordMethod
{
	PASSWORD_MRZ_REQUIRED_FIELDS_ONLY,
	PASSWORD_MRZ,
	PASSWORD_CAN,
#ifdef GERMAN_ID_CARD
	PASSWORD_PIN,
	PASSWORD_PUK
#endif
} AccessControlPasswordMethod;


typedef struct RFAccessControlPasswords
{
	RTCHAR puFullMRZ[MAX_CODELINE_LENGTH];
	RTCHAR puDocNumber[MAX_CODELINE_FIELD_LENGTH];
	RTCHAR puDateOfBirth[MAX_CODELINE_FIELD_LENGTH];
	RTCHAR puDateOfExpiry[MAX_CODELINE_FIELD_LENGTH];
	RTCHAR puCardAccessNumber[10];
} RFAccessControlPasswords;

/// Callback function definition for a user-defined function that is used during RF read
/// operations that require a Basic Access Control (BAC) or Supplemental Access Control (SAC)
/// key/password. The defined function will receive the last key that was used, and the function 
/// should update the provided buffer with a valid key to retry opening the RF chip.
/// Return true to continue trying and false to cancel.
///
/// \ingroup RfidModule
///
/// \param[in] aParam An optional pointer to some user-defined data. This is usually initialised
/// when setting the callback function in question.
///
/// \param[in] aSequenceNum A user-defined value that identifies what data is coming back 
/// through the callback.
///
/// \param[in,out] aPasswordsInOut A #RFAccessControlPasswords structure used to specify the 
/// key. When the function returns, it should contain an updated valid BAC/SAC key.  If using
/// MRZ, specify the full MRZ (puFullMRZ) or individual MRZ elements (puDocNumber, puDateOfBirth,
/// puDateOfExpiry).  If using the CAN for SAC (aOnlyAcceptsMRZ == false), specify 
/// puCardAccessNumber.
///
/// \param[in] aOnlyAcceptsMRZ A boolean flag that indicates if only MRZ is allowed (true/BAC) or
/// if MRZ and CAN are allowed (false/SAC).  If false, then the chip is attempting SAC and MRZ or CAN
/// are allowed.
///
/// \param[out] aPasswordMethodOut A #AccessControlPasswordMethod enumeration that should be
/// set with the password method used and corresponding values in aPasswordsInOut.
///
/// \param[out] aChipRemovedFromField A boolean flag that the function should set if the RF
/// chip was removed from the RF field. Setting this to \c true instructs the \llapi to 
/// re-establish communications with the RF chip. This value is initially \c false.
///
/// \remarks Return true to continue trying and false to cancel.
///
/// \remarks \a aParam can be used to provide some context data back through the callback. One
/// example could be the \c this pointer from a C++ class; in this case, because the callback
/// function must be a C-style function, \a aParam would be able to access elements of its
/// class type.
///
/// \remarks \a aSequenceNum is a user-defined value that is usually passed into one of the
/// \llapi functions at the point of requesting data back. This parameter is provided so that
/// it is possible to have one data callback defined for multiple data items; the defined
/// function can then filter the \a aSequenceNum as appropriate.
///
/// \sa MMMReader_RFInitialise()
typedef bool (*MMMReaderRFPasswordCallback) (
	void *aParam, 
	int aSequenceNum, 
	RFAccessControlPasswords *aPasswordsInOut,
	bool aOnlyAcceptsMRZ,
	AccessControlPasswordMethod *aPasswordMethodOut,
	bool *aChipRemovedFromFieldOut
);

/// \endcond	
// PAGESDK

/// Callback function definition for a user-defined function that will receive event 
/// notifications that are raised by the \sdk.
///
/// \ingroup EventsModule
///
/// \param[in] aParam An optional pointer to some user-defined data. This is usually initialised
/// when setting the callback function in question.
///
/// \param[in] aEventType Specifies which event has been raised by the \sdk. See 
/// #MMMReaderEventCode for the kinds of events that can be raised.
///
/// \remarks \a aParam can be used to provide some context data back through the callback. One
/// example could be the \c this pointer from a C++ class; in this case, because the callback
/// function must be a C-style function, \a aParam would be able to access elements of its
/// class type.
typedef void (*MMMReaderEventCallback) (
	void *aParam, 
	MMMReaderEventCode aEventType
);

/// Callback function definition for a user-defined function that will receive errors generated
/// by the \sdk.
///
/// \ingroup ErrorsModule
///
/// \param[in] aErrorCode Specifies the code of the generated error. See #MMMReaderErrorCode
/// for the kinds of errors that can be raised.
///
/// \param[in] aErrorMsg A pointer to a string containing a user-friendly message describing
/// the error.
///
/// \param[in] aParam An optional pointer to some user-defined data. This is usually initialised
/// when setting the callback function in question.
///
/// \remarks \a aParam can be used to provide some context data back through the callback. One
/// example could be the \c this pointer from a C++ class; in this case, because the callback
/// function must be a C-style function, \a aParam would be able to access elements of its
/// class type.
///
/// \remarks All errors generated by the \sdk are automatically written to the \sdk log file.
/// therefore it is not necessary to write \a aErrorMsg to an external source, it is simply
/// provided to allow displaying the error visually to the user.
typedef void (*MMMReaderErrorCallback) (
	MMMReaderErrorCode aErrorCode, 
	RTCHAR *aErrorMsg,
	void *aParam
);

/// Callback function definition for a user-defined function that will receive warnings generated
/// by the \sdk.
///
/// \ingroup WarningsModule
///
/// \param[in] aWarningCode Specifies the code of the generated warning. See #MMMReaderWarningCode
/// for all warning types that can be raised.
///
/// \param[in] aWarningMsg A pointer to a string containing a user-friendly message describing
/// the warning.
///
/// \param[in] aCallbackParam An optional pointer to some user-defined data. This is usually
/// initialised when setting the callback function in question.
///
/// \remarks \a aCallbackParam can be used to provide some context data back through the callback.
/// One example could be the \c this pointer from a C++ class; in this case, because the callback
/// function must be a C-style function, \a aCallbackParam would be able to access elements of its
/// class type.
///
/// \remarks All warnings generated by the \sdk will be written to the \sdk log file if logging
/// is enabled on level #LOG_LVL_WARNING or higher. Therefore it is not necessary to write 
/// \a aWarningMsg to an external source, it is simply provided to allow displaying the 
/// error visually to the user.
typedef void (*MMMReaderWarningCallback) (
	MMMReaderWarningCode aWarningCode, 
	const RTCHAR *aWarningMsg,
	void *aCallbackParam
);

/// \cond PAGESDK

/// Callback function definition for a user-defined function that is used during document
/// detection.
///
/// \ingroup CameraModule
///
/// \note This callback is only relevant if using a \pagereader with the \llapi.
///
/// \param[in] aParam An optional pointer to some user-defined data. This is usually initialised
/// when setting the callback function in question.
///
/// \param[in] aImageData The raw data of the image used for document detection.
///
/// \remarks \a aParam can be used to provide some context data back through the callback. One
/// example could be the \c this pointer from a C++ class; in this case, because the callback
/// function must be a C-style function, \a aParam would be able to access elements of its
/// class type.
///
/// \sa MMMReader_CameraDetectDocument()
typedef bool (*MMMReaderValidateDocPositionCallback) (
	void *aParam, 
	unsigned char *aImageData
);

/// Callback function definition for a user-defined function that is used during image decoding
/// via plugins. The defined function can be used to prevent a plugin from decoding an image
/// based on some user-defined criteria, e.g. if a previous plugin decode process has already 
/// returned all the necessary data.
///
/// \ingroup PluginsModule
///
/// \note This callback is only relevant if using a \pagereader with the \llapi. The \hlapi has 
/// its own version of this callback, #MMMReaderHLSkipPluginCallback, which simplfies this 
/// process.
///
/// \param[in] aParam An optional pointer to some user-defined data. This is usually initialised
/// when setting the callback function in question.
///
/// \param[in] aSequenceNum A user-defined value that identifies what data is coming back 
/// through the callback.
///
/// \return Must return \c true if the host application wants to skip this plugin decode 
/// process, or \c false if the host application wants this plugin to continue.
///
/// \remarks \a aParam can be used to provide some context data back through the callback. One
/// example could be the \c this pointer from a C++ class; in this case, because the callback
/// function must be a C-style function, \a aParam would be able to access elements of its
/// class type.
///
/// \remarks \a aSequenceNum is a user-defined value that is usually passed into one of the
/// \llapi functions at the point of requesting data back. This parameter is provided so that
/// it is possible to have one data callback defined for multiple data items; the defined
/// function can then filter the \a aSequenceNum as appropriate.
///
/// \sa MMMReader_ImagePluginDecode()
typedef bool (*MMMReaderSkipPluginCallback)(
	void *aParam,
	int aSequenceNum
);

//#ifdef DCA
/// Callback function definition for a user-defined function that provides feedback on document
/// mrz during ocr processing. This can be useful to correct unrecognized characters on the mrz
/// and improve docuemnt identification.
///
/// \ingroup HighLevelDocProcModule
///
/// \note This callback is only relevant if using a \pagereader and the \hlapi
///
/// \param[in] aParam An optional pointer to some user-defined data. This is usually initialised
/// when setting the callback function in question.
///
/// \param[in] aMrzLine1 The top line of characters found in the mrz zone. 
///
/// \param[in] aMrzLine2 The second line from the top of characters found in the mrz zone. 
///
/// \param[in] aMrzLine3 The top line from the top of characters found in the mrz zone. 
///
/// \param[in] aLineCount The number of lines found in the document mrz zone
///
/// \remarks \a aParam can be used to provide some context data back through the callback. One
/// example could be the \c this pointer from a C++ class; in this case, because the callback
/// function must be a C-style function, \a aParam would be able to access elements of its
/// class type.
///
/// \sa MMMReaderMrzCorrectionCallback()
typedef void (*MMMReaderMrzCorrectionCallback)(
	void *aParam, 
	char *aMrzLine1, 
	char *aMrzLine2, 
	char *aMrzLine3, 
	int aLineCount 
);

//#endif //DCA
/// Callback function definition for a user-defined function that allows the \sdk to notify 
/// the hosting application of some action, for example when an operation/process has finished.
///
/// \ingroup LowLevelModule
///
/// \note This callback is only relevant if using the \llapi.
///
/// \param[in] aParam An optional pointer to some user-defined data. This is usually initialised
/// when setting the callback function in question.
///
/// \remarks \a aParam can be used to provide some context data back through the callback. One
/// example could be the \c this pointer from a C++ class; in this case, because the callback
/// function must be a C-style function, \a aParam would be able to access elements of its
/// class type.
typedef void (*MMMReaderNotifyCallback)(
	void* aParam
);

/// \deprecated North American Drivers Licence (AAMVA) data is returned via the \hlapi 
/// data item #CD_AAMVA_DATA instead.
typedef void (*MMMReaderAAMVADecodeCallback)(
	void* aParam,
	const RTCHAR* aId,
	const RTCHAR* aName,
	const RTCHAR* aValue
);

/// \endcond	
// PAGESDK

/// \cond PAGESDK

/// Defines the different type of certificates that can be used during RF read operations.
///
/// \ingroup RfidModule
typedef enum CERT_TYPE
{
	CT_INVALID = 0,							///< No certificate. Use this value for initialisation.
    CT_DOC_SIGNER_CERT = 1,					///< A Document Signer Certificate (DSC).
    CT_COUNTRY_SIGNER_CERT = 2,				///< A Country Signer Certificate (CSC).
    CT_CERTIFICATE_REVOCATION_LIST = 3,		///< A Certificate Revocation List (CRL).
    CT_CVCA_CERT = 4,						///< A Country Verifying Certification Authority (CVCA) certificate.
    CT_DV_CERT = 5,							///< A Document Verifier (DV) certificate.
    CT_IS_CERT = 6,							///< An Inspection System (IS) certificate.
    CT_IS_PRIVATE_KEY = 7					///< A PKCS8 private key for an IS certificate.
} CERT_TYPE;

/// Defines the different antenna switching modes supported by the \sdk for RF read operations.
///
/// \ingroup RfidModule
///
/// \note The antenna mode can only be set when using one of the following \pagereaders:
///		- \rte8000
///		- \rte8000hs
///
/// \note For all other \pagereaders, the default value used is always 
/// #AM_DUAL_USE_DATA_PAGE_THEN_NON_DATA_PAGE.
///
/// \remarks There are two antennas contained in a \pagereader: one positioned under a document
/// data page, and another positioned on the documents opposite page. In this context, the
/// data page is considered to be the document page that contains the document owner's 
/// information, codeline and image, etc.
///
/// \sa MMMReader_RFOpen()
typedef enum ANTENNA_MODE
{	
	/// \internal
    AM_SINGLE = 0,
    
    /// Only use the antenna positioned under the document data page.
    AM_DUAL_USE_DATA_PAGE = 1,
    
    /// Only use the antenna positioned under the document non-data page.
    AM_DUAL_USE_NON_DATA_PAGE = 2,
    
    /// First try using the data page antenna, then the non-data page antenna.
    AM_DUAL_USE_DATA_PAGE_THEN_NON_DATA_PAGE = 3,
    
    /// First try using the non-data page antenna, then the data page antenna.
    AM_DUAL_USE_NON_DATA_PAGE_THEN_DATA_PAGE = 4,
    
    /// Use the last antenna found.
    AM_DUAL_USE_LAST_FOUND_FIRST = 5
    
} ANTENNA_MODE;

/// Defines the different ways of retrieving a Document Signer Certificate (DSC) in order to 
/// validate the signature in the Document Security Object. The DSC can be optionally stored 
/// on the chip, otherwise it must be provided via some external source.
///
/// \ingroup RfidModule
///
/// \sa #EXTERNAL_CERT_MODE
typedef enum DOC_SIGNER_CERT_MODE
{
	/// Load the DSC from the RF chip only.
    DSCM_ON_CHIP_ONLY = 1,
    
    /// Load DSC from external source.    
    DSCM_EXTERNAL_ONLY = 2,
    
    /// Attempt to load the DSC from the RF chip. If none is loaded, load from an external 
    /// source.
    DSCM_ON_CHIP_THEN_EXTERNAL = 3,
    
    /// Attempt to load the DSC from an external source. If none is loaded, load from the
    /// RF chip.
    DSCM_EXTERNAL_THEN_ON_CHIP = 4,
    
    /// Load the DSC from the RF chip and verify it using a \mmm Inspection System Server.
    /// Contact \ssd for more details.
    DSCM_USE_3M_ISS = 5
    
} DOC_SIGNER_CERT_MODE;

/// Defines the supported external certificate sources supported by the \sdk for RF read
/// operations.
///
/// \ingroup RfidModule
typedef enum EXTERNAL_CERT_MODE
{
	/// Retrieve a certificate from a number of certificate files contained in a local
	/// directory.
	///
	/// \note This mode can be used for the following RF operations:
	///		- Validating the signature in a Document Security Object using a public key
	///		from a Document Signer Certificate (DSC).
	///		- Validating the signature of a DSC using the public key from an external
	///		Country Signer Certificate (CSC).
	///		- \eac Terminal Authentication
	///
	/// \remarks The folder where the certificate files are located can be set via the 
	/// #RFProcessSettings::puCertsDir setting. The #RFProcessSettings structure also
	/// provides settings for including sub-directories and specifying what certificate file
	/// extensions to search for.
	///
	/// \sa #RFProcessSettings
    ECM_CERT_FILE_STORE = 1,
    
    /// Retrieve a certificate by having the \sdk call back to the hosting application
    /// using the #MMMReaderCertificateCallback. In this scenario, the hosting application
    /// is responsible for providing the correct certificate to use for the operation.
    ///
    /// \note This mode can be used for the following RF operations:
    ///		- Validating the signature in a Document Security Object using a public key
    ///		from a Document Signer Certificate (DSC).
    ///		- Validating the signature of a DSC using the public key from an external
    ///		Country Signer Certificate (CSC).
    ///		- \eac Terminal Authentication
	///
	/// \sa #MMMReaderCertificateCallback
    ECM_CERT_CALLBACK = 2,
    
    /// \eac Request that a signature is generated over some data provided by the \sdk to verify
    /// Terminal Authentication.
    ///
    /// \note This mode can be used for the following RF operations:
    ///		- \eac Terminal Authentication
    ///
    /// \remarks This method is provided for systems that do not allow private keys to be
    /// passed around in files or memory, e.g. using HSM or SAM implementations.
    ///
    /// \remarks The \sdk will invoke a #MMMReaderSignRequestCallback, which requires the
    /// host application to generate a signature over a given buffer. The IS certificate will
    /// be included in the callback to provide the host application with all the necessary 
    /// parameters for algorithm and domain parameter selection.
    ///
    /// \sa #MMMReaderSignRequestCallback
    ECM_SIGN_REQUEST = 3,
    
    /// \eac Request that Terminal Authentication be carried out by a remote server using the
    /// \mmm Inspection System Server. Contact \ssd for more details.    
    ///
    /// \note This mode can be used for the following RF operations:
    ///		- \eac Terminal Authentication
    ECM_USE_3M_ISS = 4
    
} EXTERNAL_CERT_MODE;

/// Callback function definition for a user-defined function that is used during RF read
/// operations. This is used when a certificate is required and when the operation is 
/// configured with the #ECM_CERT_CALLBACK external certificate mode. The defined function 
/// will return the requested certificate.
///
/// \ingroup RfidModule
///
/// \param[in] aParam An optional pointer to some user-defined data. This is usually initialised
/// when setting the callback function in question.
/// 
/// \param[in] aCertIdentifier A string containing an identifier for the required certificate.
///
/// \param[in] aCertIdentifierLen The length of \a aCertIdentifier.
///
/// \param[in] aCertType Specifies the type of certificate being requested. Use this in
/// combination with \a aCertIdentifier to uniquely identify the certificate. See the table
/// in the Remarks section for more details.
///
/// \param[out] aCertBuffer A pointer to a writable, pre-allocated buffer for the host 
/// application to write the certificate to.
///
/// \param[in,out] aCertBufferLen Initially will contain the length of the pre-allocated 
/// \a aCertBuffer. When the function returns, it must update this value to be the actual size
/// of the certificate in bytes.
///
/// \return Must return \c true if the certificate was loaded successfully or \c false if no 
/// certificate was loaded.
///
/// \attention This callback function must be used with care to avoid memory corruption.
///
/// \remarks \a aParam can be used to provide some context data back through the callback. One
/// example could be the \c this pointer from a C++ class; in this case, because the callback
/// function must be a C-style function, \a aParam would be able to access elements of its
/// class type.
///
/// \remarks If the pre-allocated \a aCertBuffer is too large for the required certificate, 
/// change \a aCertBufferLen to be the correct value and return \c true. 
///
/// \remarks If the pre-allocated \a aCertBuffer is too small for the required certificate,
/// change \a aCertBufferLen to be the correct value and return \c false. The \sdk will then
/// invoke this callback function again with a new buffer of the correct size.
///
/// \remarks If the function fails to load the certificate, return \c false and do not change
/// the \a aCertBufferLen value.
///
/// \remarks The table below explains which identifiers can be expected:
///	<table>
///	<tr><th>\a aCertType</th><th>Description</th></tr>
///
/// <tr><td>#CT_DOC_SIGNER_CERT</td><td>The \sdk is requesting that an external Document 
///	Signer Certificate (DSC) must be provided. This request will contain an ASN.1 DER encoded
/// SignerIdentifier buffer (see 
/// <a href="https://tools.ietf.org/html/rfc3369#section-5.3" target="_blank">RFC 3369 section 
///	5.3</a>). The function must supply a DER encoded DSC (this can also be Base64 DER encoded).</td></tr>
///
/// <tr><td>#CT_COUNTRY_SIGNER_CERT</td><td>The \sdk is requesting that an external Country 
///	Signer Certificate (CSC) must be provided. This request will contain an ASN.1 DER encoded
/// AuthorityKeyIdentifier buffer (see 
/// <a href="https://tools.ietf.org/html/rfc3280#section-4.2.1.1" target="_blank">RFC 3280 
/// section 4.2.1.1</a>). The function must supply a DER encoded CSC (this can also be Base64 
/// DER encoded).</td></tr>
///
/// <tr><td>#CT_CERTIFICATE_REVOCATION_LIST</td><td>The \sdk is requesting that an external 
/// Certificate Revocation List (CRL) must be provided. This request will contain an ASN.1 DER 
/// encoded IssuerAndSerialNumber buffer (see 
/// <a href="https://tools.ietf.org/html/rfc3369#section-10.2.4" target="_blank">RFC 3369 
/// section 10.2.4</a>). The function must supply a DER encoded CRL (this can also be Base64 
/// DER encoded).</td></tr>
///
/// <tr><td>#CT_CVCA_CERT \eac</td><td>The \sdk is requesting that an external 
/// Country Verifying Certification Authority (CVCA) certificate must be provided. This request
/// will contain a value to match with the Certificate Authority Reference (CAR) field in the 
/// CV certificate. The function must supply a DER encoded certificate (this can also be Base64 
/// DER encoded).</td></tr>
///
/// <tr><td>#CT_DV_CERT \eac</td><td>The \sdk is requesting that an external Document Verifier 
/// (DV) certificate must be provided. This request will contain a value to match with the 
/// Certificate Authority Reference (CAR) field in the CV certificate. The function must supply 
/// a DER encoded certificate (this can also be Base64 DER encoded).</td></tr>
///
/// <tr><td>#CT_IS_CERT \eac</td><td>The \sdk is requesting that an external Inspection System 
/// (IS) certificate must be provided. This request will contain a value to match with the 
/// Certificate Authority Reference (CAR) field in the CV certificate. The function must supply 
/// a DER encoded certificate (this can also be Base64 DER encoded).</td></tr>
///
/// <tr><td>#CT_IS_PRIVATE_KEY \eac</td><td>The \sdk is requesting that an external IS private 
/// key must be provided. This request will contain a value to match with the Certificate 
/// Holder Reference (CHR) field in the IS certificate. The function must supply a DER encoded 
/// PKCS8 PrivateKeyInfo (this can also be Base64 DER encoded).</td></tr>
///
///	</table>
typedef bool (*MMMReaderCertificateCallback)(
	void* aParam, 
	char* aCertIdentifier, 
	int aCertIdentifierLen, 
	CERT_TYPE aCertType, 
	char* aCertBuffer, 
	int* aCertBufferLen
);

/// \eac Callback function definition for a user-defined function that is used during RF read
/// operations. This is used when a signature is required to access the protected DG3 and/or 
/// DG4 data groups on an RF chip, but access to Inspection System private keys is secure. The
/// defined function should therefore generate the signature itself and pass it back to the
/// \sdk to complete the Terminal Authentication protocol. This callback is only used with
/// the #ECM_SIGN_REQUEST external private key mode.
///
/// \ingroup RfidModule
///
/// \param[in] aParam An optional pointer to some user-defined data. This is usually initialised
/// when setting the callback function in question.
///
/// \param[in] aBufferToSign A pointer to a data buffer which the defined function must sign.
///
/// \param[in] aBufferToSignLen The length of \a aBufferToSign in bytes.
///
/// \param[in] aCertBuffer A pointer to a certificate whose public key (and domain parameters)
/// should correspond to the private key used to generate the signature.
///
/// \param[in] aCertBufferLen The length of \a aCertBuffer in bytes.
///
/// \param[in] aCertType Specifies the type of certificate contained in \a aCertBuffer.
///
/// \param[out] aSignature A pointer to a writable, pre-allocated buffer for the defined
/// function to write the signature to.
///
/// \param[in,out] aSignatureLen Initially will contain the length of \a aSignature. When the
/// function returns, it must update this value to be the actual size of the signature in 
/// bytes.
///
/// \return Must return \c true if the signature was generated successfully, or \c false if it
/// failed.
///
/// \remarks \a aParam can be used to provide some context data back through the callback. One
/// example could be the \c this pointer from a C++ class; in this case, because the callback
/// function must be a C-style function, \a aParam would be able to access elements of its
/// class type.
///
/// \remarks If the pre-allocated \a aSignature buffer is too large for the required signature,
/// change \a aSignatureLen to be the correct value and return \c true.
///
/// \remarks If the pre-allocated \a aSignature buffer is too small for the required signature,
/// change \a aSignatureLen to be the correct value and return \c false. The \sdk will then
/// invoke this callback function again with a new buffer of the correct size.
///
/// \remarks If the function fails to generate a signature, return \c false and do not change
/// the \a aSignatureLen value.
typedef bool (*MMMReaderSignRequestCallback)(
    void* aParam,
    char* aBufferToSign, 
	int aBufferToSignLen,
    char* aCertBuffer, 
	int aCertBufferLen, 
	CERT_TYPE aCertType,
    char* aSignature, 
	int* aSignatureLen
);

/// Callback function definition for a user-defined function that is used during RF read
/// operations. This is used when the ePassport supports multiple SAC algorithms.
///
/// \ingroup RfidModule
///
/// \param[in] aParam An optional pointer to some user-defined data. This is usually initialised
/// when setting the callback function in question.
/// 
/// \param[in] SACAlgorithms Semi-colon (;) delimited, NULL terminated list of SAC algorithm
/// OID strings supported by the ePassport.  Use algorithmIndex to specify the desired algorithm.
///
/// \param[out] algorithmIndex allows the caller to specify the zero (0) based index of
/// the desired SAC algorithm from the list of SACAlgorithms. Set algorithmIndex to -1
/// or any other value outside the SACAlgorithms index range to use the default SDK
/// behavior for selecting the SAC algorithm.
///
/// \remarks \a aParam can be used to provide some context data back through the callback. One
/// example could be the \c this pointer from a C++ class; in this case, because the callback
/// function must be a C-style function, \a aParam would be able to access elements of its
/// class type.
typedef void (*MMMReaderSACAlgorithmCallback)(
	void* aParam, 
	char* SACAlgorithms, 
	int* algorithmIndex
);

/// Defines a value that could have one of three possible outcomes. This enumeration is usually
/// used in RF read operations.
///
/// \ingroup RfidModule
typedef enum TRISTATE
{
    TS_FAILURE = -1,			///< The operation failed.
    TS_NOT_PERFORMED = 0,		///< The operation was not performed.
    TS_SUCCESS = 1				///< The operation succeeded.
} TRISTATE;

/// Defines a boxed area, usually used in relation to image processing.
///
/// \ingroup ImageProcModule
///
/// \sa #DOCDETECTSETTINGS::boxes
typedef struct
{
	/// The x-coordinate of the upper left corner of the Box.
	int left;
	
	/// The y-coordinate of the upper left corner of the Box.
	int top;
	
	/// The width of the Box, measured in pixels.
	int wdt;
	
	/// The height of the Box, measure in pixels.
	int hgt;
	
	/// \internal
	float ave;
	
} Box;

/// Defines the data items that can be retrieved from an RF chip.
///
/// \ingroup RfidModule
///
/// \note Depending on which RF chip is being read, not all of the following data groups
/// may be present.
///
/// \sa MMMReader_RFGetFile(), MMMReader_RFValidateDataGroup(), MMMReader_RFDecodeDataGroup()
typedef enum MMMReaderRFItem
{
	RFID_EF_COM,					///< The EF.COM file
    RFID_EF_SOD,					///< The EF.SOD file
	
	RFID_DG1,						///< Data Group 1 (the Machine Readable Zone (MRZ))
    RFID_DG2,						///< Data Group 2 (the photo image)
    RFID_DG3,						///< Data Group 3 (fingerprints)
    RFID_DG4,						///< Data Group 4
    RFID_DG5,						///< Data Group 5
    RFID_DG6,						///< Data Group 6
    RFID_DG7,						///< Data Group 7
    RFID_DG8,						///< Data Group 8
    RFID_DG9,						///< Data Group 9
    RFID_DG10,						///< Data Group 10
    RFID_DG11,						///< Data Group 11
    RFID_DG12,						///< Data Group 12
    RFID_DG13,						///< Data Group 13
    RFID_DG14,						///< Data Group 14
    RFID_DG15,						///< Data Group 15
    RFID_DG16,						///< Data Group 16

	RFID_EF_CVCA_FILE,				///< The EF CVCA file
	RFID_EF_CARD_ACCESS,			///< The EF.CardAccess file

	NUM_RF_ITEMS					///< \internal
} MMMReaderRFItem;

/// Defines the different validation results that can be returned from certain RF read 
/// operations.
///
/// \ingroup RfidModule
typedef enum MMMReaderValidationCode
{
//#ifdef DCA
	/// The operation was not performed.
    RFID_VC_NOT_PERFORMED = -1,		
//#endif

	/// Invalid result
    RFID_VC_INVALID = 0,
    
    /// Valid result
    RFID_VC_VALID = 1,	
    
    /// Valid with revoked certificate (determined from a check against Certificate 
    /// Revocation Lists (CRLs)).
    RFID_VC_VALID_WITH_REVOKED_CERT = 2,
    
    /// No Document Signer Certificate (DSC) loaded.
    RFID_VC_NO_DSC_LOADED = 3,
    
    /// No Country Signer Certificate (CSC) loaded.
    RFID_VC_NO_CSC_LOADED = 4
    
} MMMReaderValidationCode;

typedef enum MMMPluginID
{
	MMM_PID_1DBarcode_Industrial2Of5 = 1,
	MMM_PID_1DBarcode_Interleaved2Of5,
	MMM_PID_1DBarcode_IATA2Of5,
	MMM_PID_1DBarcode_Code3Of9,
	MMM_PID_1DBarcode_Code128,
	MMM_PID_2DBarcode_PDF417,
	MMM_PID_Cheque_CMC7,
	MMM_PID_DrivingLicenceUK,
	MMM_PID_Visa_China,
	MMM_PID_2DBarcode_AztecCode,
	MMM_PID_Visa_US,
	MMM_PID_2DBarcode_QRCode,
	MMM_PID_1DBarcode_Code93,
	MMM_PID_1DBarcode_Codabar,
	MMM_PID_OCRToolKit = 15,
	MMM_PID_1DBarcode_UPC_EAN,
	MMM_PID_2DBarcode_DataMatrix = 20
} MMMPluginID;

typedef enum RT_ORIENTATION
{
    RTO_LR = 1,
    RTO_RL = 2,
    RTO_TB = 4,
    RTO_BT = 8
} RT_ORIENTATION;

/// Defines a collection of information returned from a decoder plugin.
///
/// \ingroup PluginsModule HighLevelDataModule
///
/// \note This structure is only relevant if using a \pagereader.
///
/// \remarks Plugins will decode images into one more more <em>feature regions</em> and one or 
/// more <em>feature parts</em>.
///
/// \remarks A <em>feature region</em> is an area of an image where the decoder plugin found
/// something it recognised. For example, if an image contains more than one barcode, a barcode
/// decoder would return the number of barcodes (feature regions) it found.
///
/// \remarks A <em>feature part</em> is part of a feature region. Feature regions may be split
/// up into separate parts if each part of the decoded data is represented in a different
/// format, e.g. string data, binary data, etc.
///
/// \remarks Therefore, the number of #MMMReaderPluginData structures each plugin will return 
/// will be equal to the total number of feature parts for all feature regions found.
typedef struct MMMReaderPluginData
{
	/// Identifies the type of feature found.
	///
	/// \remarks This member can be used to identify which plugin this data came from. In the 
	/// case that a plugin supports more than one data type, this member will also be used to
	/// identify further what type of data this is.
	MMMPluginID puFeatureTypeID;
		
	/// Provides the coordinates of the four corner points of the region where the decoded
	/// data was found in the image. Measured in pixels.
	MMMRegion puFeatureRegion;
	    
	/// Provides the index of the feature region that this data is for.
	///
	/// \remarks This index is zero based. This value will always be between 0 and 
	/// #puTotalFeatures - 1.
	int puFeatureNum;

	/// Provides the total number of feature regions that the plugin decoder found overall.
	///
	/// \remarks Use this value along with the current #puFeatureNum value to determine when
	/// all the feature regions have been returned.
	int puTotalFeatures;

	/// Provides the index of the feature part of the current feature region that this data
	/// is for.
	///
	/// \remarks This index is zero based. This value will always be between 0 and 
	/// #puTotalParts - 1.
	int puPartNum;

	/// Provides the total number of feature parts for the current feature region.
	///
	/// \remarks Use this value along with the current #puPartNum value to determine when
	/// all the feature parts of the current feature region have been returned.
	int puTotalParts;

	/// Provides the type of data #puData points to.
	///
	/// \remarks If the data type is DF_ECI_DATA #puECINumber can be used to determine
	/// the interpretation of the data.
	MMMReaderDataFormat puDataFormat;

	/// Provides a pointer to the data that was decoded.
	///
	/// \note The format of the data is entirely dependent on the plugin that created
	/// it and how it was encoded into the image. Therefore, always use a combination
	/// of #puDataFormat, #puDataLen and #puECINumber members to make use of the 
	/// pointer correctly.
	///
	/// \remarks Use the #puDataFormat member to determine how to cast this pointer.
	void *puData;

	/// Provides the length of the #puData array.
	///
	/// \note Use this value to determine exactly how much data is available. Although the 
	/// #puData array can resemble a string that is not always the case, so do not assume
	/// that there will be a null-terminating character to mark the end of the data.
	int puDataLen;

	/// Provides an Extended Channel Interpretation (ECI) value, used for barcode decoding.
	///
	/// \note This member is used if the #puDataType member is set to DF_ECI_DATA.
	///
	/// \remarks In the case of barcodes, some symbologies use ECI to interpret data. Examples
	/// of data format include ASCII data, binary data, etc. This value can therefore be
	/// used to determine what format #puData is in.
	///
	/// \remarks The \sdk barcode plugins will return multiple feature parts if sections of a 
	/// decoded barcode change ECI number throughout, thereby representing a change in format.
	///
	/// \remarks This value adhears to the AIM ECI Assignments. For example:
	///		- 0 (ECI 000000) represents plain binary byte encoding.
	///		- 3 (ECI 000003) represents the standard ISO-8859-1 Latin Alphabet character set.
	///		- 20 (ECI(000020) represents the JIS8 and Shift JIS character sets.
	///
	/// \remarks The value returned is dependent on the type of barcode. Please view the
	/// appropriate barcode specification to see which ECI numbers are available.
	int puECINumber;

	/// A friendly name of the feature decoded.
	///
	/// \remarks In the case of barcodes, this will be the name of the symbology.
	///
	/// \remarks In the case of OCR Toolkit plugins, this will be the name of the
	/// table file.
	char *puFeatureName;

	/// Provides the name of the OCR toolkit table file field, when the plugin data
	/// has come from an OCR toolkit plugin.
	char *puFieldName;

	/// Provides the orientation of the decoded data, which is mainly applicable to
	/// some barcodes found.
	RT_ORIENTATION puOrientation;
} MMMReaderPluginData;

/// Defines the image formats the \sdk supports.
///
/// \ingroup ImageProcModule
typedef enum MMMReaderImageFormats
{
	/// An image is represented as a GDI+ Bitmap object.
	MMMR_IF_GDIPLUSBITMAP = 0,
	
	/// An image is in the TIFF format.
	MMMR_IF_TIFF = 3,
	
	/// An image is in the Bitmap format.
	MMMR_IF_BMP = 6,
	
	/// An image is in the JPEG format.
	MMMR_IF_JPEG = 10,
	
	/// An image is in the PNG format.
	MMMR_IF_PNG = 75,
	
	/// An image is in the JPEG 2000 format.
	MMMR_IF_JPEG2000 = 163
	
} MMMReaderImageFormats;

/// \eac Defines the different image compression formats used for storing biometric fingerprint
/// data in an RF chip.
///
/// \ingroup RfidModule
///
/// \sa #DG3FingerprintData::puFIRImgCompresion
typedef enum FIR_IMG_COMPRESSION
{
	/// \todo Find out what this is
    FIC_UNCOMPRESSED_UNPACKED = 0,
    
    /// \todo Find out what this is
    FIC_UNCOMPRESSED_BIT_PACKED = 1,
    
    /// Wavelet Scalar Quantization (WSQ) grayscale image compression.
    FIC_WSQ = 2,
    
    /// JPEG image compression.
    FIC_JPEG = 3,
    
    /// JPEG 2000 image compression.
    FIC_JPEG2000 = 4,
    
    /// PNG image compression.
    FIC_PNG = 5
    
} FIR_IMG_COMPRESSION;

/// \eac Defines the different finger positions available for retrieving biometric fingerprint
/// data from an RF chip.
///
/// \ingroup RfidModule
///
/// \sa #DG3FingerImage::puFingerPosition
typedef enum FIR_FINGER_POSITION
{
    FFP_UNKNOWN = 0,				///< Unknown value. Use this for initialisation.
    FFP_RIGHT_THUMB = 1,			///< Thumb on right hand.
    FFP_RIGHT_INDEX = 2,			///< Index finger on right hand.
    FFP_RIGHT_MIDDLE = 3,			///< Middle finger on right hand.
    FFP_RIGHT_RING = 4,				///< Ring finger on right hand.
    FFP_RIGHT_LITTLE = 5,			///< Little finger on right hand.
    FFP_LEFT_THUMB = 6,				///< Thumb on left hand.
    FFP_LEFT_INDEX = 7,				///< Index finger on left hand.
    FFP_LEFT_MIDDLE = 8,			///< Middle finger on left hand.
    FFP_LEFT_RING = 9,				///< Ring finger on left hand.
    FFP_LEFT_LITTLE = 10,			///< Little finger on left hand.
    FFP_RIGHT_FOUR = 13,			///< All four fingers on right hand.
    FFP_LEFT_FOUR = 14,				///< All four fingers on left hand.
    FFP_THUMBS = 15					///< Both thumbs.
}FIR_FINGER_POSITION;

/// \eac Defines a single fingerprint retrieved from DG3 on an RF chip.
///
/// \ingroup RfidModule
///
/// \sa #DG3FingerprintData::puFingerImages
typedef struct DG3FingerImage
{
	/// The length of the fingerprint image's header and data, measured in bytes.
    int puDataAndHeaderLength;
    
    /// Specifies the finger position this fingerprint represents, i.e. which finger
    /// and on which hand.
    FIR_FINGER_POSITION puFingerPosition;
    
    /// Provides the total number of views of this #puFingerPosition.
    ///
    /// \remarks More than one image may be retrieved from the RF chip for a single finger. Use
    /// this value and #puViewNumber to cycle through each view.
    unsigned char puNumberOfViews;
    
    /// Provides an index to the current view of this finger.
    ///
    /// \remarks This value is zero-based. This value will always be between 0 and 
    /// #puNumberOfViews - 1.
    unsigned char puViewNumber;
    
    /// \todo Find out what this is
    unsigned char puImageQuality;
    
    /// \todo Find out what this is
    unsigned char puImpressionType;
    
    /// \todo Find out what this is
    int puHorizNumPixels;
    
    /// \todo Find out what this is
    int puVertNumPixels;

	/// Provides a pointer to the raw image data of this view of the fingerprint.
	///
	/// \remarks The data will be formatted/compressed according to the 
	/// #DG3FingerprintData::puFIRImgCompresion member.
	///
	/// \sa #FIR_IMG_COMPRESSION
    unsigned char* puDataBuffer;
    
    /// Provides the size of #puDataBuffer in bytes.
    unsigned long puDataBufferSize;
    
} DG3FingerImage;

/// \eac Defines the collection of fingerprints retreived from DG3 on an RF chip.
///
/// \ingroup RfidModule
///
/// \sa #DG3FingerImage
typedef struct DG3FingerprintData
{
	/// \todo Find out what this is
    char puFIRFormatID[4];
    
    /// \todo Find out what this is
    char puFIRVersion[4];
    
    /// \todo Find out what this is
    int puFIRRecordLength;
    
    /// \todo Find out what this is
    char puFIRCBEFFProductID[4];
    
    /// \todo Find out what this is
    char puFIRScannerID[2];
    
    /// \todo Find out what this is
    char puFIRImgAcquisitionLevel[2];
    
    /// Provides the number of fingerprint entries returned.
    /// 
    /// \remarks This value represents the size of the #puFingerImages array.
    int puFIRNumFingers;
    
    /// \todo Find out what this is
    int puFIRScaleUnits;
    
    /// \todo Find out what this is
    int puFIRScanResHoriz;
    
    /// \todo Find out what this is
    int puFIRScanResVert;
    
    /// \todo Find out what this is
    int puFIRImageResHoriz;
    
    /// \todo Find out what this is
    int puFIRImageResVert;
    
    /// \todo Find out what this is    
    int puPixelDepth;
    
    /// Specifies what image compression format is used for all the images referenced in
    /// #puFingerImages.
    FIR_IMG_COMPRESSION puFIRImgCompresion;
    
    /// Provides a pointer to an array of #DG3FingerImage structures which stores all the
    /// fingerprint images.
    ///
    /// \attention This array is dynamically allocated by the \sdk based on the value of 
    /// #puFIRNumFingers; this member represents the size of this array. The \sdk will handle
    /// the memory for this array, it does not need to be manually freed.
	DG3FingerImage *puFingerImages[1];
} DG3FingerprintData;

#define	MAX_READERNAME_LEN	1024

/// Defines the details of an available smart card reader, typically used for RF read 
/// operations.
///
/// \ingroup RfidModule
///
/// \note This structure is only relevant if using the \llapi.
///
/// \remarks This structure is returned through a #MMMReaderDataCallback passed to the
/// MMMReader_RFGetReaderInfo() API.
typedef struct SmartCardReaderInfo
{
	/// Provides the name of the smart card reader.
	RTCHAR puReaderName[MAX_READERNAME_LEN];
	
	/// Determine whether the smart card reader is a contact reader. If \c false, it is
	/// contactless.
	bool puIsContactReader;
	
	/// Provides the internal index of this data structure that the \sdk uses to keep track
	/// of all available smart card readers.
	///
	/// \remarks This index is zero-based. It will always be between 0 and \c ReaderCount - 1,
	/// where \c ReaderCount is the return value from the MMMReader_RFGetReaderCount() API.
	int puReaderIndex;
	
} SmartCardReaderInfo;


//// Defines the different debarrel algorithm available
///
/// \ingroup LowLevelModule
///
/// \sa MMMReader_GetHardwareConfig()
typedef enum DEBARREL_ALGORITHM
{
    DEBARREL_ALGORITHM_DEFAULT = 0,			///< This is the default algorithm - linear with zoom.
    DEBARREL_ALGORITHM_THIRD_ORDER = 1		///< This is a thrid order algorithm - 1 + r^3d.
} DEBARREL_ALGORITHM;

// Defines the details of the currently connected and initialised \pagereader.
///
/// \ingroup LowLevelModule
///
/// \note This structure is only relevant if using a \pagereader with the \llapi.
///
/// \sa MMMReader_GetHardwareConfig()
#define	FIRMWARE_NAME_LEN	20

typedef struct MMMReaderHardwareConfig
{
	/// \todo Find out what this is
	char puLCBPartNum[FIRMWARE_NAME_LEN];
	
	/// \todo Find out what this is
	int puLCBVersion;
	
	/// \todo Find out what this is
	int puLCBIssue;

	/// Provides the width of the camera in pixels.
	int puCamWidth;
	
	/// Provides the height of the camera in pixels.
	int puCamHeight;
	
	/// Provides K constant for debarrel algorithm - this is reader family specific
	float puDebarrelConstant;

	/// Provides Horizonatl Zoom compensation for debarrel algorithm - this is reader family specific
	float puDebarrelZoomHorizontal;

	/// Provides Vertical Zoom compensation for debarrel algorithm - this is reader family specific
	float puDebarrelZoomVertical;

	/// Determine whether the \pagereader has an RF component and is therefore able to use
	/// the RF functionality.
	bool puHasRF;
	
	/// Determine whether the \pagereader has a UV component that is therefore able to capture
	/// UV images.
	bool puHasUV;
	
	/// \todo Find out what this is
	bool puHasCoax;

	/// \todo Find out what this is
	bool puHasBankSwitching;
	
	/// \todo Find out what this is
	bool puHasShortIllumination;
	
	/// \todo Find out what this is
	bool puHasLongIllumination;
	
	/// \todo Find out what this is
	bool puHasRGBIllumination;

	/// Determine whether the \pagereader uses UV tubes or UV LEDs for capturing UV images.
	bool puUsesUVTubes;
	
	/// Determines the colour matrix that is appropriate for this scanner
	int puColourMatrix;

	/// Determines the colour matrix that is appropriate for this scanner, when using the True Colour processing
	int puTrueColourMatrix;

	/// Indicates whether the \pagereader was calibrated via the "AutoCal" tool, or via "CameraSetup"
	bool puCalibratedByAutoCal;

	/// Indicates whether the \pagereader only has the codeline region (an RTE8000 "eViewer")
	bool puCodelineOnly;

	/// Provides the dpi of the \pagereader in use
	int puCamXDPI;
	
	/// Provides the dpi of the \pagereader in use
	int puCamYDPI;
	
	/// Indicates whether the scanner is USB powered. Note that for some scanners that
	/// can work either USB powered or mains powered, this could change on successive calls
	/// if the user has unplugged the scanner.
	bool puIsUSBPowered;

	/// Indicates whether the reader supports the Anti-Glare feature to remove reflections from
	/// documents.
	bool puSupportsAntiGlare;

	/// Indicates whether the scanner has a motor feed
	///
	/// \remarks For scanners with a motor feed MMMReader_CameraFeedDocument() will need to be
	/// called to feed the document.
	bool puHasMotorFeed;

	/// Indicates which rear image types the scanner is capable of capturing.
	///
	/// \remarks The value is a bitwise mask of light types
	///
	/// \sa LIGHT_RED, LIGHT_GREEN, LIGHT_BLUE, LIGHT_IRFULL, LIGHT_IRPART, LIGHT_UV,
	/// LIGHT_COAX_IR, LIGHT_COAX_VIS
	int puRearImageTypes;

	/// Indicates the scanner takes a double sided image of an ID card.
	bool puIsDoubleSidedImage;
	
	/// Indicates whether the reader supports the ImageScaling feature 
	bool puSupportsImageScaling;
	
	/// Indicates which debarrel algorithm to use 
	DEBARREL_ALGORITHM puDebarrelAlgorithm;

} MMMReaderHardwareConfig;


/// \endcond	
// PAGESDK

//-------------------------------------------------------------------------------------------

/// Defines the swipe readers supported by the \sdk.
///
/// \if SWIPESDK
/// \ingroup GeneralModule
/// \else
/// \ingroup SwipeModule
/// \endif
///
/// \note These constants are only relevant if using a \swipereader.
typedef enum MMMSwipeDeviceType
{
	/// No Device provided. Use this for initialisation.
	MSDT_NO_DEVICE = 0,
	
	/// The RTE670X family of swipe readers. This includes RTE6700 hardware plus other variants 
	/// based off it.
	MSDT_RTE670X_FAMILY,
	
	/// The MagTek magnetic swipe reader (MSR) device.
	MSDT_MAGTEK_MSR,
	
	/// The \mmm CR series of document readers.
	MSDT_MMM_CR_SERIES,

	/// The CR100 Keyboard MSR device.
	MSDT_MMM_CR100_KBD,
	
	/// The IDTECH magnetic swipe reader (MSR) device.
	MSDT_IDTECH_MSR,
	
	/// \internal
	MSDT_NUM_DEVICE_TYPES

} MMMSwipeDeviceType;

/// Defines all the serial connection types supported by the \sdk.
///
/// \if SWIPESDK
/// \ingroup GeneralModule
/// \else
/// \ingroup SwipeModule
/// \endif
typedef enum MMMSerialConnectionType
{
	/// Unknown serial connection. Use this for initialisation.
	MSCT_UNKNOWN = 0,
	
	/// A serial device is connected through a hardware serial port.
	MSCT_SERIAL,

	/// A 3M serial device is connected through a USB virtual COM port.
	MSCT_USB_MMM,
	
	/// A serial device is connected through a USB virtual COM port.
	MSCT_USB_VIRTUAL_COM,
	
	/// A serial device is connected through a USB Human Interface Device (HID) connection.
	MSCT_USB_HID,

	/// A serial device is connected through a bluetooth serial port.
	MSCT_BT_SERIAL,
	
	/// \internal
	MSCT_NUM_CONNECTION_TYPES
	
} MMMSerialConnectionType;

/// Defines all the serial port parity types supported by the \sdk.
///
/// \if SWIPESDK
/// \ingroup GeneralModule
/// \else
/// \ingroup SwipeModule
/// \endif
typedef enum MMMSerialParityType
{
	MSPT_NONE = 0,		///< No parity	
	MSPT_ODD = 1,		///< Odd parity
	MSPT_EVEN = 2,		///< Even parity	
	MSPT_MARK = 3,		///< Mark parity	
	MSPT_SPACE = 4		///< Space parity
	
} MMMSerialParityType;

/// Defines all the serial port stop bit types supported by the \sdk.
///
/// \if SWIPESDK
/// \ingroup GeneralModule
/// \else
/// \ingroup SwipeModule
/// \endif
typedef enum MMMSerialStopBitType
{
	MSSBT_ONE = 0,					///< 1 stop bit
	MSSBT_ONE_POINT_FIVE = 1,		///< 1.5 stop bits	
	MSSBT_TWO = 2					///< 2 stop bits
	
} MMMSerialStopBitType;
//------------------------------------------------------------------------------------------

/// Defines a set of constants that are special processing bytes.
///
/// \if SWIPESDK
/// \ingroup GeneralModule
/// \else
/// \ingroup SwipeModule
/// \endif
///
/// \remarks All of the defined constants are in the ASCII range \c 0x00 - \c 0x31. 
///
/// \remarks These values are useful for analysing bytes from data messages received from a 
/// serial device.
typedef enum MMMByteSpecialChars
{	
	SOH = 0x01,					///< Start of Heading byte		
	STX = 0x02,					///< Start of Text byte		
	ETX = 0x03,					///< End of Text byte		
	EOT = 0x04,					///< End of transmit byte
	LF = 0x0a,					///< Line Feed (new line)	
	CR = 0x0d,					///< Carriage Return	
	DLE = 0x10,					///< Data Link Escape	
	ACK = 0x06,					///< Acknowledgement byte	
	NACK = 0x15,				///< Negative Acknowledgement byte
	GS = 0x1d,					///< Group Separator
	RS = 0x1e,					///< Record Separator
	
} MMMByteSpecialChars;

//------------------------------------------------------------------------------------------

/// Defines the different types of devices used for a \swipereader using the RTE protocol.
///
/// \if SWIPESDK
/// \ingroup SwipeRTEProtoModule
/// \else
/// \ingroup SwipeModule
/// \endif
///
/// \note These constants are only relevant if using a \swipereader that has been configured
/// with an RTE protocol variant.
typedef enum RTEProtocolDeviceType
{
	/// The device
	///
	/// \note Constant value is zero, not the letter "O".
	RTE_DT_DEVICE = '0',
	
	/// ATB reader
	RTE_DT_ATB = 'A',
	
	/// MSR reader
	RTE_DT_MSR = 'C',
	
	/// Optical reader
	///
	/// \note Constant value is the letter "O", not zero.
	RTE_DT_OCR = 'O',
	
	/// Serial reader (e.g. barcode)
	RTE_DT_SERIAL = '1',
	
	/// Smartcard reader
	RTE_DT_SMARTCARD = 'S',
	
	/// Reserved
	RTE_DT_RESERVED = '!',
	
	/// Unknown device.
	RTE_DT_UNKNOWN = 0xffff
	
} RTEProtocolDeviceType;

/// Defines the different types of RTE protocol messages.
///
/// \if SWIPESDK
/// \ingroup SwipeRTEProtoModule
/// \else
/// \ingroup SwipeModule
/// \endif
///
/// \note These constants are only relevant if using a \swipereader that has been configured
/// with an RTE protocol variant.
typedef enum RTEProtocolMessageType
{
	/// Message is a command to be sent to the \swipereader.
	RTE_MT_COMMAND = 'C',
	
	/// Message contains data specifically requested by the host application.
	RTE_MT_DATA = 'D',
	
	/// Message contains data sent by the \swipereader.
	RTE_MT_UNSOLICITED_DATA = 'U',
	
	/// Message contains an error response from the \swipereader.
	RTE_MT_ERROR_RESPONSE = 'E'
	
} RTEProtocolMessageType;

/// Defines all the types of RTE protocol commands that can be sent to a \swipereader.
///
/// \if SWIPESDK
/// \ingroup SwipeRTECommandsModule
/// \else
/// \ingroup SwipeModule
/// \endif
///
/// \note These constants are only relevant if using a \swipereader that has been configured
/// with an RTE protocol variant.
typedef enum RTEProtocolCommandType
{
	/// Enable Device command
	///
	/// \sa MMMReader_SwipeRTE_EnableDevice()
	RTE_PCT_ENABLE_DEVICE = 'E',
	
	/// Disable Device command
	///
	/// \sa MMMReader_SwipeRTE_DisableDevice()
	RTE_PCT_DISABLE_DEVICE = 'D',
	
	/// Inquire command
	///
	/// \sa MMMReader_SwipeRTE_Inquire()
	RTE_PCT_INQUIRE = 'I',
	
	/// LED command
	///
	/// \sa MMMReader_SwipeRTE_OperateLED()
	RTE_PCT_LED = 'L',
	
	/// Buzzer command
	///
	/// \sa MMMReader_SwipeRTE_OperateBuzzer()
	RTE_PCT_BUZZER = 'B',
	
	/// Reset command
	///
	/// \sa MMMReader_SwipeRTE_ResetDevice()
	RTE_PCT_RESET = 'R'
	
} RTEProtocolCommandType;

/// Defines all the values for inquiring about tracks for certain devices (ATB and MSR) in
/// the RTE protocol.
///
/// \if SWIPESDK
/// \ingroup SwipeRTECommandsModule
/// \else
/// \ingroup SwipeModule
/// \endif
///
/// \note These constants are only relevant if using a \swipereader that has been configured
/// with an RTE protocol variant.
///
/// \sa MMMReader_SwipeRTE_Inquire()
typedef enum RTEProtocolTrackValue
{
	/// No tracks (for devices which don't have them).
	RTE_TV_NONE = 0,
	
	/// Inquire on the device's default tracks(s) (depends on the device in question).
	RTE_TV_DEVICE_DEFAULT = '0',
	
	/// Inquire on track 1 of the device.
	RTE_TV_TRACK_ONE = '1',
	
	/// Inquire on track 2 of the device.
	RTE_TV_TRACK_TWO = '2',
	
	/// Inquire on track 3 of the device.
	RTE_TV_TRACK_THREE = '3',
	
	/// Inquire on track 4 of the device (ATB only).
	RTE_TV_TRACK_FOUR = '4',
	
	/// Inquire on all the device's tracks.
	RTE_TV_ALL_TRACKS = 'A'
	
} RTEProtocolTrackValue;

/// Defines the values for defining the status of a \swipereader LED in the RTE protocol.
///
/// \if SWIPESDK
/// \ingroup SwipeRTECommandsModule
/// \else
/// \ingroup SwipeModule
/// \endif
///
/// \note These constants are only relevant if using a \swipereader that has been configured
/// with an RTE protocol variant.
///
/// \sa MMMReader_SwipeRTE_OperateLED()
typedef enum RTEProtocolLEDStatus
{	
	RTE_LED_S_OFF = '0',		///< LED should be switched off.	
	RTE_LED_S_RED = 'R',		///< LED should light up red.
	RTE_LED_S_AMBER = 'A',		///< LED should light up amber.	
	RTE_LED_S_GREEN = 'G'		///< LED should light up green.
	
} RTEProtocolLEDStatus;

/// Defines the values for defining an operation to perform on an active \swipereader LED in 
/// the RTE protocol.
///
/// \if SWIPESDK
/// \ingroup SwipeRTECommandsModule
/// \else
/// \ingroup SwipeModule
/// \endif
///
/// \note These constants are only relevant if using a \swipereader that has been configured
/// with an RTE protocol variant.
///
/// \sa MMMReader_SwipeRTE_OperateLED()
typedef enum RTEProtocolLEDOperation
{	
	RTE_LED_O_NONE = 0,			///< No operation.
	RTE_LED_O_ON = 'O',			///< LED should be switched on and should be continuously on.	
	RTE_LED_O_BLINK = 'B'		///< LED should blink on and off.
	
} RTEProtocolLEDOperation;

/// Defines the values for defining different \swipereader buzzer tones in the RTE protocol.
///
/// \if SWIPESDK
/// \ingroup SwipeRTECommandsModule
/// \else
/// \ingroup SwipeModule
/// \endif
///
/// \note These constants are only relevant if using a \swipereader that has been configured
/// with an RTE protocol variant.
///
/// \sa MMMReader_SwipeRTE_OperateBuzzer()
typedef enum RTEProtocolBuzzerTone
{	
	RTE_BUZZER_PLAIN_TONE = 'O',	///< Sound a plain tone.	
	RTE_BUZZER_WARBLE_TONE = 'W'	///< Sound a warbling tone.
	
} RTEProtocolBuzzerTone;

//------------------------------------------------------------------------------------------

/// Defines the different types of devices used for a \swipereader using the MUSE protocol.
///
/// \if SWIPESDK
/// \ingroup SwipeMUSEProtoModule
/// \else
/// \ingroup SwipeModule
/// \endif
///
/// \note These constants are only relevant if using a \swipereader that has been configured
/// with the MUSE protocol.
typedef enum MuseProtocolDeviceType
{
	/// ATB reader
	MUSE_DT_ATB = 'A',
	
	/// MSR reader
	MUSE_DT_MSR = 'C',
	
	/// Optical reader.
	///
	/// \note Constant value is the letter "O", not zero.
	MUSE_DT_OCR = 'O',
	
	/// Unknown device
	MUSE_DT_UNKNOWN = 0xffff
	
} MuseProtocolDeviceType;

//------------------------------------------------------------------------------------------

/// Defines special byte values that can appear in CUTE protocol messages.
///
/// \if SWIPESDK
/// \ingroup SwipeCUTEProtoModule
/// \else
/// \ingroup SwipeModule
/// \endif
///
/// \note These constants are only relevant if using a \swipereader that has been configured
/// with the CUTE protocol.
typedef enum CuteProtocolSpecialBytes
{	
	CPSB_SOD1 = 0x0e,		///< Start delimiter for Magnetic Swipe Reader.	
	CPSB_EOD1 = 0x0f,		///< End delimiter for Magnetic Swipe Reader.	
	CPSB_SOD2 = 0x1c,		///< Start delimiter for OCR reader.
	CPSB_EOD2 = 0x1d		///< End delimiter for OCR reader.
	
} CuteProtocolSpecialBytes;

/// Defines the different types of devices used for a \swipereader using the CUTE protocol.
///
/// \if SWIPESDK
/// \ingroup SwipeCUTEProtoModule
/// \else
/// \ingroup SwipeModule
/// \endif
///
/// \note These constants are only relevant if using a \swipereader that has been configured
/// with the CUTE protocol.
typedef enum CuteProtocolDeviceType
{
	/// ATB reader
	CUTE_DT_ATB = 'A',
	
	/// MSR reader
	CUTE_DT_MSR = 'C',
	
	/// Optical reader.
	///
	/// \note Constant value is the letter "O", not zero.
	CUTE_DT_OCR = 'O',
	
	/// Unknown device.
	CUTE_DT_UNKNOWN = 0xffff
	
} CuteProtocolDeviceType;
//-------------------------------------------------------------------------------------------

/// Defines the different card encoding types recognised by a Magtek MSR swipe reader.
///
/// \if SWIPESDK
/// \ingroup SwipeMagtekProtoModule
/// \else
/// \ingroup SwipeModule
/// \endif
///
/// \note These constants are only relevant if using a Magtek MSR swipe reader.
typedef enum MagtekMsrProtocolCardType
{	
	MAGMSR_CT_ISO_ABA = 0,		///< ISO/ABA encode format	
	MAGMSR_CT_AAMVA = 1,		///< AAMVA encode format	
	MAGMSR_CT_CADL = 2,			///< CADL encode format (returns track 2 only)
	MAGMSR_CT_BLANK = 3,		///< Blank card	
	MAGMSR_CT_OTHER = 4,		///< Other non-standard encode format
	MAGMSR_CT_UNDETERMINED = 5,	///< Could not determine encode format
	MAGMSR_CT_NONE = 6,			///< No decode has occurred
	MAGMSR_CT_JIS_TYPE_2 = 7	///< JIS Type 2 encode format.

} MagtekMsrProtocolCardType;
//------------------------------------------------------------------------------------------

/// Defines the different types of devices used for a \swipereader using the TECS protocol.
///
/// \if SWIPESDK
/// \ingroup SwipeTECSProtoModule
/// \else
/// \ingroup SwipeModule
/// \endif
///
/// \note These constants are only relevant if using a \swipereader that has been configured
/// with the TECS protocol.
typedef enum TecsProtocolDeviceType
{
	/// ATB reader
	TECS_DT_ATB = 'A',
	
	/// MSR reader
	TECS_DT_MSR = 'C',
	
	/// Optical reader.
	///
	/// \note Constant value is the letter "O", not zero.
	TECS_DT_OCR = 'O',
	
	/// Unknown device.
	TECS_DT_UNKNOWN = 0xffff
	
} TecsProtocolDeviceType;
//-------------------------------------------------------------------------------------------

/// Defines all the \swipereader data items that the \sdk can send back to the hosting 
/// application.
///
/// \if SWIPESDK
/// \ingroup SwipeDataModule
/// \else
/// \ingroup SwipeModule
/// \endif
///
/// \note These constants are only relevant if using a \swipereader.
///
/// \remarks Not all \swipereaders can return the same data.
///
/// \remarks These values are returned via the #MMMReaderDataCallback passed into the
/// MMMReader_SwipeInitialise() API. It is returned through the \a aSequenceNum parameter
/// of the callback. To determine exactly which item the data represents, you can use implicit
/// casting or explicitly cast the value to a #MMMReaderSwipeItem type, for example:
///
/// \remarks \include SwipeDataCallbackSequenceNum.cpp
typedef enum MMMReaderSwipeItem
{
	/// The entire data stream received from the \swipereader is being returned.
	///
	/// \remarks The data returned will be a pointer to a byte array. Cast the 
	/// #MMMReaderDataCallback \a aDataPtr parameter to a \c MMMByte* to use the data.
	/// The #MMMReaderDataCallback \a aDataLen parameter will state how big the array is.
	///
	/// \remarks The data will include all control and header values as well as the relevant 
	/// content of the data stream.
	SWIPE_WHOLE_DATA = 0,
	
	/// All relevant data parsed from #SWIPE_WHOLE_DATA. This will include everything except
	/// for header and trailer bytes.
	///
	/// \remarks The data returned will be a pointer to an ASCII string. Cast the 
	/// #MMMReaderDataCallback \a aDataPtr parameter to a \c char* to use the data.
	/// The #MMMReaderDataCallback \a aDataLen parameter will state how big the string is.
	/// The data will include a null-terminating character.
	SWIPE_MESSAGE_CONTENT,
	
	/// A #MMMReaderCodelineData structure containing all parsed data from the OCR data stream. 
	/// The \sdk will do some parsing such as splitting up the lines and extracting some 
	/// common data fields and return the results in this structure.
	///
	/// \remarks The data returned will be a pointer to a #MMMReaderCodelineData structure.
	/// Cast the #MMMReaderDataCallback \a aDataPtr parameter to a \c MMMReaderCodelineData*.
	/// The #MMMReaderDataCallback \a aDataLen parameter will give the size of the structure in
	/// bytes.
	SWIPE_OCR_CODELINE,
	
	/// A #MMMReaderMsrData structure containing all parsed data from the MSR data stream. The
	/// \sdk will do some parsing to split up the tracks and return the results in this 
	/// structure.
	///
	/// \remarks The data returned will be a pointer to a #MMMReaderMsrData structure.
	/// Cast the #MMMReaderDataCallback \a aDataPtr parameter to a \c MMMReaderMsrData*.
	/// The #MMMReaderDataCallback \a aDataLen parameter will give the size of the structure in
	/// bytes.
	SWIPE_MSR_DATA,

	/// A #MMMReaderSwipeBarcodeData structure containing all parsed data from the Barcode data 
	/// stream. The \sdk will do some parsing to split up the tracks and return the results in 
	/// this structure.
	///
	/// \remarks The data returned will be a pointer to a #MMMReaderSwipeBarcodeData structure.
	/// Cast the #MMMReaderDataCallback \a aDataPtr parameter to a \c MMMReaderSwipeBarcodeData*.
	/// The #MMMReaderDataCallback \a aDataLen parameter will give the size of the structure in
	/// bytes.
	SWIPE_BARCODE_PDF417,

	/// A #MMMReaderSwipeBarcodeData structure containing all parsed data from the Barcode data 
	/// stream. The \sdk will do some parsing to split up the tracks and return the results in 
	/// this structure.
	///
	/// \remarks The data returned will be a pointer to a #MMMReaderSwipeBarcodeData structure.
	/// Cast the #MMMReaderDataCallback \a aDataPtr parameter to a \c MMMReaderSwipeBarcodeData*.
	/// The #MMMReaderDataCallback \a aDataLen parameter will give the size of the structure in
	/// bytes.
	SWIPE_BARCODE_1D_128,

	/// A #MMMReaderSwipeBarcodeData structure containing all parsed data from the Barcode data 
	/// stream. The \sdk will do some parsing to split up the tracks and return the results in 
	/// this structure.
	///
	/// \remarks The data returned will be a pointer to a #MMMReaderSwipeBarcodeData structure.
	/// Cast the #MMMReaderDataCallback \a aDataPtr parameter to a \c MMMReaderSwipeBarcodeData*.
	/// The #MMMReaderDataCallback \a aDataLen parameter will give the size of the structure in
	/// bytes.
	SWIPE_BARCODE_1D_3_OF_9,
	
	/// A #MMMReaderAtbData structure containing all parsed data from the ATB data stream. The
	/// \sdk will do some parsing to split up the tracks and blocks and return the results in 
	/// this structure.
	///
	/// \remarks The data returned will be a pointer to a #MMMReaderAtbData structure.
	/// Cast the #MMMReaderDataCallback \a aDataPtr parameter to a \c MMMReaderAtbData*.
	/// The #MMMReaderDataCallback \a aDataLen parameter will give the size of the structure in
	/// bytes.
	SWIPE_ATB_DATA,
	
	/// A #MMMReaderRTEQAData structure containing all parsed data from a RTE6510 swipe reader. 
	/// The \sdk will do some parsing to split up the values and return the results in this 
	/// structure.
	///
	/// \note This data item will only be returned when using a RTE6510 swipe reader.
	///
	/// \remarks The data returned will be a pointer to a #MMMReaderRTEQAData structure.
	/// Cast the #MMMReaderDataCallback \a aDataPtr parameter to a \c MMMReaderRTEQAData*.
	/// The #MMMReaderDataCallback \a aDataLen parameter will give the size of the structure in
	/// bytes.
	SWIPE_RTE_QA_DATA,
		
	/// A #MMMReaderRTESwipeData structure containing control values read from a RTE protocol 
	/// data stream.
	///
	/// \note This data item will only be returned when using a \swipereader configured with
	/// a RTE protocol variant.
	///
	/// \remarks The data returned will be a pointer to a #MMMReaderRTESwipeData structure.
	/// Cast the #MMMReaderDataCallback \a aDataPtr parameter to a \c MMMReaderRTESwipeData*.
	/// The #MMMReaderDataCallback \a aDataLen parameter will give the size of the structure in
	/// bytes.
	SWIPE_RTE_DATA,
	
	/// A #MMMReaderMuseSwipeData structure containing control values read from a MUSE protocol 
	/// data stream.
	///
	/// \note This data item will only be returned when using a \swipereader configured with
	/// the MUSE protocol.
	///
	/// \remarks The data returned will be a pointer to a #MMMReaderMuseSwipeData structure.
	/// Cast the #MMMReaderDataCallback \a aDataPtr parameter to a \c MMMReaderMuseSwipeData*.
	/// The #MMMReaderDataCallback \a aDataLen parameter will give the size of the structure in
	/// bytes.
	SWIPE_MUSE_DATA,
	
	/// A #MMMReaderCuteSwipeData structure containing control values read from a CUTE protocol 
	/// data stream.
	///
	/// \note This data item will only be returned when using a \swipereader configured with
	/// the CUTE protocol.
	///
	/// \remarks The data returned will be a pointer to a #MMMReaderCuteSwipeData structure.
	/// Cast the #MMMReaderDataCallback \a aDataPtr parameter to a \c MMMReaderCuteSwipeData*.
	/// The #MMMReaderDataCallback \a aDataLen parameter will give the size of the structure in
	/// bytes.
	SWIPE_CUTE_DATA,
	
	/// A #MMMReaderMagtekMsrSwipeData structure containing control values read from a
	/// Magtek MSR swipe reader.
	///
	/// \note This data item will only be returned when using a Magtek MSR swipe reader.
	///
	/// \remarks The data returned will be a pointer to a #MMMReaderMagtekMsrSwipeData structure.
	/// Cast the #MMMReaderDataCallback \a aDataPtr parameter to a \c MMMReaderMagtekMsrSwipeData*.
	/// The #MMMReaderDataCallback \a aDataLen parameter will give the size of the structure in
	/// bytes.
	SWIPE_MAGMSR_DATA,	
	
	/// A #MMMReaderTecsSwipeData structure containing control values read from a TECS protocol 
	/// data stream.
	///
	/// \note This data item will only be returned when using a \swipereader configured with
	/// the TECS protocol.
	///
	/// \remarks The data returned will be a pointer to a #MMMReaderTecsSwipeData structure.
	/// Cast the #MMMReaderDataCallback \a aDataPtr parameter to a \c MMMReaderTecsSwipeData*.
	/// The #MMMReaderDataCallback \a aDataLen parameter will give the size of the structure in
	/// bytes.
	SWIPE_TECS_DATA,

	/// An #MMMReaderAAMVAData structure containing AAMVA data parsed from an MSR data stream.
	///
	/// \note This data item will only be returned when MSR data has been successfully
	/// parsed as AAMVA data.
	///
	/// \remarks The data returned will be a pointer to a #MMMReaderAAMVAData structure.
	/// Cast the #MMMReaderDataCallback \a aDataPtr parameter to a \c MMMReaderAAMVAData*.
	/// The #MMMReaderDataCallback \a aDataLen parameter will give the size of the structure in
	/// bytes.
	SWIPE_AAMVA_DATA,

	/// \internal
	NUM_SWIPE_ITEMS
	
} MMMReaderSwipeItem;
//------------------------------------------------------------------------------------------

/// Defines a date that the \sdk has read from some input source.
///
/// \ingroup GeneralModule
///
/// \attention The \sdk will only read what it considers to be a date, but it does not guarantee
/// that it is a valid date. If a date field is invalid, a value of -1 should be used.
typedef struct MMMReaderDate
{
	/// Provides the day part of a date. This will be between 1 - 31.
	int Day;
	
	/// Provides the month part of a date. This will be between 1 - 12.
	int Month;
	
	/// Provides the year part of a date.
	///
	/// \attention Depending on the input source, the #Year may be either a 2-digit year or
	/// a 4-digit year. If it is a 2-digit year, consideration must be taken when interpreting
	/// the value.
	int Year;

} MMMReaderDate;

/// Defines the types of check digit found in an ICAO codeline
typedef enum MMMReaderCheckDigitType
{
	/// Document number check digit
	CDT_DocID,

	/// Date of birth check digit
	CDT_DOB,

	/// Expiry date check digit
	CDT_Expiry,

	/// Optional data check digit
	CDT_OptionalData,

	/// Overall check digit for the whole codeline
	CDT_Overall

} MMMReaderCheckDigitType;

/// Defines the possible result values of a check digit validation
typedef enum MMMReaderCheckDigitResult
{
	/// Check digit is invalid
	CDR_Invalid,

	/// Check digit is valid
	CDR_Valid,

	/// Check digit is not valid according to ICAO rules, but this maybe expected
	CDR_Warning,

	/// Check digit validation has not been carried out
	CDR_NotValidated

} MMMReaderCheckDigitResult;

/// Defines data about a check digit from an OCR ICAO codeline.
///
/// \ingroup GeneralModule HighLevelDataModule SwipeModule ParsingModule SwipeDataModule
typedef struct MMMReaderCodelineCheckDigitData
{
	/// The codeline check digit type.
	MMMReaderCheckDigitType puCheckDigitType;

	/// The codeline number on which the check digit is located.
	///
	/// \remarks This is a 1 based index (1, 2, or 3)
	int puCodelineNumber;

	/// The character position on the line of the codeline on which the check digit
	/// is found.
	///
	/// \remarks This is a 0 based index
	int puCodelinePos;

	/// The expected check digit value, based on the rest of the codeline characters
	/// read.
	char puValueExpected;

	/// The value of the check digit read from the codeline.
	char puValueRead;

	/// The result
	MMMReaderCheckDigitResult puResult;

} MMMReaderCodelineCheckDigitData;

/// Defines a collection of data that has been parsed from an OCR ICAO codeline.
///
/// \ingroup GeneralModule HighLevelDataModule SwipeModule ParsingModule SwipeDataModule
///
/// \remarks The \sdk will parse an OCR codeline and attempt to extract common, useful values
/// from it. For example, if the codeline originated from an identity document, the \sdk will
/// attempt to extract the document owner's name, date or birth, expiry date, etc.
///
/// \remarks If a particular field value could not be found, the corresponding string data 
/// member will have a length of zero. If the data represents a #MMMReaderDate, all data 
/// members of that structure will be set to zero.
///
/// \remarks All dates parsed from the codeline have a 2-digit year.
typedef struct MMMReaderCodelineData
{
	/// Provides the full OCR codeline as a single string.
	char Data[MAX_CODELINE_LENGTH];
	
	/// Provides the number of separate lines found in #Data.
	int LineCount;
	
	/// Provides the first line of the codeline.
	char Line1[MAX_CODELINE_LENGTH];
	
	/// Provides the second line of the codeline.
	char Line2[MAX_CODELINE_LENGTH];
	
	/// Provides the third line of the codeline.
	char Line3[MAX_CODELINE_LENGTH];
	
	/// Provides the identifier of this document, if available.
	///
	/// \remarks If not an empty string, this can include one of the following values:
	///		- \c "SHORTVISA"
	///		- \c "LONGVISA"
	///		- \c "PASSPORT"
	///		- \c "IDTWOLINE"
	///		- \c "IDTHREELINE"
	///		- \c "IDFRANCE"
	///		- \c "USSPECIAL"
	///		- \c "IDHONGKONG"
	///		- \c "RESIDENCECARD"
	///		- \c "SWISSDL"
	///		- \c "UNKNOWN"
	char DocId[MAX_CODELINE_FIELD_LENGTH];
	
	/// Provides the document type extracted from the codeline.
	///
	/// \remarks This can be one of the following values:
	///		- \c "VISA"
	///		- \c "RESIDENCE CARD"
	///		- \c "PASSPORT"
	///		- \c "IDENTITY CARD"
	///		- \c "DRIVING LICENCE"
	///     - \c "VEHICLE REGISTRATION"
	///		- \c "UNKNOWN DOCUMENT"
	char DocType[MAX_CODELINE_FIELD_LENGTH];
	
	/// Provides the surname of the document owner read from the codeline, if available.
	char Surname[MAX_CODELINE_FIELD_LENGTH];
	
	/// Provides the first name of the document owner read from the codeline, if available.
	char Forename[MAX_CODELINE_FIELD_LENGTH];
	
	/// Provides the second name of the document owner read from the codeline, if available.
	char SecondName[MAX_CODELINE_FIELD_LENGTH];
	
	/// Provides the first and second names of the document owner read from the codeline, if 
	/// available.
	char Forenames[MAX_CODELINE_FIELD_LENGTH];
	
	/// Provides the date of birth of the document owner, if available.
	MMMReaderDate DateOfBirth;
	
	/// Provides the expiry date of the document, if available.
	MMMReaderDate ExpiryDate;
	
	/// Provides the three letter ISO 3166-1 country code of the document's issuing state, 
	/// if available.
	char IssuingState[MAX_CODELINE_FIELD_LENGTH];
	
	/// Provides the three letter ISO 3166-1 country code of the document owner's nationality, 
	/// if available.
	char Nationality[MAX_CODELINE_FIELD_LENGTH];
	
	/// Provides the document number, if available.
	char DocNumber[MAX_CODELINE_FIELD_LENGTH];
	
	/// Provides a full texual description of the document owner's gender, if available.
	///
	/// \remarks If not an empty string, this can be one of the following values:
	///		- \c "Male"
	///		- \c "Female"
	///		- \c "Unknown"
	char Sex[MAX_CODELINE_FIELD_LENGTH];
	
	/// Provides a single initial from the #Sex field, if available.
	///
	/// \remarks This will be one of the following values:
	///		- \c 'M' for \c "Male"
	///		- \c 'F' for \c "Female"
	///		- \c 'U' for \c "Unknown"
	char ShortSex;
	
	/// Provides the first optional data field, if available.
	char OptionalData1[MAX_OPTIONAL_DATA_LENGTH];

	/// Provides the second optional data field, if available.
	char OptionalData2[MAX_OPTIONAL_DATA_LENGTH];

	/// A array of all the check digits found in the codeline
	///
	/// \remarks This list will contain CheckDigitDataListCount values. The rest
	/// of the array elements will be unused.
	MMMReaderCodelineCheckDigitData CheckDigitDataList[MAX_CHECKDIGITDATA_COUNT];

	/// The number of used elements in the CheckDigitDataList array
	int CheckDigitDataListCount;

	/// Overall validation result of the codeline, based on the individual results
	/// of all check digits in the codeline.
	MMMReaderCheckDigitResult CodelineValidationResult;

	/// Indicates the mrz was found on the rear side image - when two sided reader is used (ie CR5400) 
	bool MrzOnRearSide;

	/// Indicates the document has expired by comparing the expiry date field
	bool ExpiredDocumentFlag;

	/// Indicates the illumination source where the mrz was extracted from Either LIGHT_IR or LIGHT_WHITE
	int ImageSource;

} MMMReaderCodelineData;

/// \cond PAGESDK

/// Defines a set of option flags that can be used with the MMMReader_ParseCodeline() API.
///
/// \ingroup ParsingModule
///
/// \note These constants are only relevant if using the \llapi.
///
/// \remarks These constants can be bitwise OR'd together to provide multiple options.
///
/// \sa MMMReader_ParseCodeline()
typedef enum MMMParseCodelineFlag
{
	/// No special parsing required.
	MPCF_NONE				= 0x00,
	
	/// Set the #MMMReaderCodelineData::Data member with the value that 
	/// MMMReader_ParseCodeline() has extracted.
	MPCF_SET_DATA_FIELD		= 0x01,
	
	/// Set the #MMMReaderCodelineData::Line1, #MMMReaderCodelineData::Line2 and 
	/// #MMMReaderCodelineData::Line3 members with the values that MMMReader_ParseCodeline()
	/// has extracted.
	MPCF_SET_LINE_FIELDS	= 0x02,
	
	/// Carry out all of the above operations.
	MPCF_ALL = MPCF_SET_DATA_FIELD | MPCF_SET_LINE_FIELDS

} MMMParseCodelineFlag;

/// \endcond	
// PAGESDK
//-------------------------------------------------------------------------------------------

#define LINES 3

/// Defines a structure that contains the results of a QA. 
/// This is a component of the QACodelineMeasurements structure.
typedef struct QALineResults
{
	float puTop;
	float puBottom;
	float puLeft;
	float puRight;
	float puSkew;
	float puWidth;

	char puOCRResults[MAX_CODELINE_LENGTH];
	int puOCRScores[MAX_CODELINE_LENGTH];

	float puAverageStrokeWidth;
	int puBadCharCount;
	int puBrokenCharCount;
} QALineResults;

/// Data item returned by the CD_QAINFO data item, or the low level API
/// CollectQAMeasurements
typedef struct QaCodelineMeasurements
{
	
	float puYOffset;
	float puPixelSize;

	QALineResults puLineData[LINES];

	int puNumberOfSpots;
	bool puUpsideDown;
} QaCodelineMeasurements;

/// Defines a collection of data that has been parsed from a Magnetic Swipe Reader (MSR)
/// device.
///
/// \ingroup HighLevelDataModule SwipeModule SwipeDataModule
///
/// \note This structure is only relevant if using a \swipereader that has MSR capabilities.
///
/// \remarks The \sdk will parse MSR data to split it up into individual tracks.
///
/// \remarks If a particular track is missing from the data, the corresponding string data
/// member will have a length of zero.
typedef struct MMMReaderMsrData
{
	/// Provides the first data track of MSR data, if available.
	char Track1[MAX_DATA_FIELD_LENGTH];
	
	/// Provides the second data track of MSR data, if available.
	char Track2[MAX_DATA_FIELD_LENGTH];
	
	/// Provides the third data track of MSR data, if available.
	char Track3[MAX_DATA_FIELD_LENGTH];

} MMMReaderMsrData;

/// Defines a collection of data that has been parsed from a Barcode Reader 
/// device.
///
/// \ingroup HighLevelDataModule SwipeModule SwipeDataModule
///
/// \note This structure is only relevant if using a \swipereader that has Barcode capabilities.
///
/// \remarks The \sdk will parse Barcode data.
typedef struct MMMReaderSwipeBarcodeData
{
	/// All data read from the barcode.
	char DataField[MAX_BARCODE_DATA_FIELD_LENGTH];
	int DataLength;
}MMMReaderSwipeBarcodeData;

//-------------------------------------------------------------------------------------------

/// Defines a collection of data that has been parsed from an ATB data track.
///
/// \ingroup SwipeModule SwipeDataModule
///
/// \note This structure is only relevant if using a \swipereader that has ATB capabilities.
///
/// \remarks The \sdk will parse ATB track data to split it up into individual blocks.
///
/// \remarks If a particular block is missing from the track, the corresponding string data
/// member will have a length of zero.
///
/// \sa MMMReaderAtbData
typedef struct MMMReaderAtbTrackData
{
	/// Provides the first block of data from the track, if available.
	char Block1[MAX_DATA_FIELD_LENGTH];
	
	/// Provides the second block of data from the track, if available.
	char Block2[MAX_DATA_FIELD_LENGTH];
	
	/// Provides the third block of data from the track, if available.
	char Block3[MAX_DATA_FIELD_LENGTH];
	
} MMMReaderAtbTrackData;

/// Defines a collection of data that has been parsed from an ATB data message.
///
/// \ingroup SwipeModule SwipeDataModule
///
/// \note This structure is only relevant if using a \swipereader that has ATB capabilities.
///
/// \remarks The \sdk will parse ATB data to split it up into individual tracks, which are
/// then split up further into individual blocks.
///
/// \remarks If a particular track is missing from the data, the corresponding string data
/// member will have a length of zero.
typedef struct MMMReaderAtbData
{
	/// Provides the first track of data from the ATB data, if available.
	MMMReaderAtbTrackData Track1;
	
	/// Provides the second track of data from the ATB data, if available.
	MMMReaderAtbTrackData Track2;
	
	/// Provides the third track of data from the ATB data, if available.
	MMMReaderAtbTrackData Track3;
	
	/// Provides the fourth track of data from the ATB data, if available.
	MMMReaderAtbTrackData Track4;

} MMMReaderAtbData;
//-------------------------------------------------------------------------------------------

/// Defines a collection of results parsed from an RTE QA protocol message. This structure will
/// contain a set of results for one line of OCR codeline data.
///
/// \ingroup SwipeModule SwipeDataModule SwipeRTEQAProtoModule
///
/// \note This structure is only relevant if using a RTE6510 swipe reader.
///
/// \sa MMMReaderRTEQAData
typedef struct MMMReaderRTEQALineData
{
	/// Determines whether any results are included in this structure.
	///
	/// \remarks The MMMReaderRTEQAData structure always contains 3 sets of 
	/// MMMReaderRTEQALineData results, one for each possible line of OCR codeline data
	/// found. However, not all lines may be used. Use this data member to determine whether
	/// this OCR line was provided.
	MMMR_BOOL HasData;
	
	/// Provides the total number of characters found in the codeline.
	int CharCount;
	
	/// Provides the lower line boundary. This is the distance from the bottom of the codeline
	/// to the bottom of the document.
	///
	/// \remarks This value is measured in millimeters (mm).
	float LowerLineBoundary;
	
	/// Provides the upper line boundary. This is the distance from the top of the codeline
	/// to the bottom of the document.
	///
	/// \remarks This value is measured in millimeters (mm).
	float UpperLineBoundary;
	
	/// Provides the number of characters that were recognised in the codeline.
	int RecognisedCount;
	
	/// Provides the relative measure of the average stroke width.
	float AverageStrokeWidth;
	
	/// Provides the relative measure of the thinnest stroke width.
	float ThinnestStrokeWidth;
	
	/// Provides the relative measure of the thickest stroke width.
	float ThickestStrokeWidth;
	
	/// Provides the number of characters found that are not continuous.
	int NonContinuousCount;
	
} MMMReaderRTEQALineData;

/// Defines a collection of results from an RTE QA protocol message. This structure will 
/// contain all QA results for an OCR codeline.
///
/// \ingroup SwipeModule SwipeDataModule SwipeRTEQAProtoModule
///
/// \note This structure is only relevant if using a RTE6510 swipe reader.
///
/// \remarks The OCR codeline that these results relate to is always returned as a separate
/// MMMReaderCodelineData structure through the #MMMReaderDataCallback passed to 
/// MMMReader_SwipeInitialise().
typedef struct MMMReaderRTEQAData
{
	/// Provides the number of lines found in the codeline.
	int CodelineCount;
	
	/// Provides the total number of columns found.
	int ColumnCount;
	
	/// Determines whether a clear area is present.
	MMMR_BOOL ClearAreaPresent;
	
	/// Provides the number of spots found.
	int SpotCount;
	
	/// Provides specific results for the first line of the OCR codeline.
	///
	/// \remarks If not provided, #MMMReaderRTEQALineData::HasData will be set to \c FALSE.
	MMMReaderRTEQALineData Line1;
	
	/// Provides specific results for the second line of the OCR codeline.
	///
	/// \remarks If not provided, #MMMReaderRTEQALineData::HasData will be set to \c FALSE.
	MMMReaderRTEQALineData Line2;
	
	/// Provides specific results for the third line of the OCR codeline.
	///
	/// \remarks If not provided, #MMMReaderRTEQALineData::HasData will be set to \c FALSE.
	MMMReaderRTEQALineData Line3;

} MMMReaderRTEQAData;
//-------------------------------------------------------------------------------------------

/// Defines a set of values parsed from an RTE protocol data message.
///
/// \ingroup SwipeModule SwipeDataModule SwipeRTEProtoModule
///
/// \note This structure is only relevant if using a \swipereader configured with a RTE
/// protocol variant.
///
/// \remarks These values are provided for the sake of completeness; in most programming
/// scenarios there is little need for them.
typedef struct MMMReaderRTESwipeData
{
	/// Provides the device type that the RTE protocol message came from. This refers to
	/// which part of the \swipereader was physically used.
	///
	/// \remarks See #RTEProtocolDeviceType for a list of available devices.
	RTEProtocolDeviceType DeviceType;
	
	/// Provides the message type that the RTE protocol message represents.
	///
	/// \remarks See #RTEProtocolMessageType for a list of available message types.
	RTEProtocolMessageType MessageType;
	
	/// Provides the parity check value returned from the RTE protocol message.
	///
	/// \note This value is only returned if the RTE variant protocol has been configured
	/// to return a parity check value.
	MMMByte BCC;
	
	/// Provides the flag byte returned from OCR data messages to provde information regarding
	/// the validity of E13B data.
	///
	/// \remarks The following values may be returned. Note that values represent the ASCII
	/// character, not the numerical value, e.g. "0" represents the character zero 
	/// (ASCII Dec 48, Hex 0x30), not the numerical value zero.
	/// <table>
	/// <tr><th>ASCII Character</th><th>Description</th></tr>
	///
	/// <tr><td>0</td><td>OCRB data without any E13B characters.</td></tr>
	///
	/// <tr><td>1</td><td>E13B data but no ANSI X9.13 routing field identified.</td></tr>
	///
	/// <tr><td>2</td><td>E13B and ANSI X9.13 routing field identified but with incorrect 
	/// check digit.</td></tr>
	///
	/// <tr><td>3</td><td>E13B data with ANSI X9.13 routing field identified and with a valid
	/// checksum.</td></tr>
	///
	/// <tr><td>5</td><td>TAT or barcode or OCR lozenge or OCR target detected.</td></tr>
	///
	/// <tr><td>6</td><td>ATB detected.</td></tr>
	///
	/// </table>
	char OCRFlagByte;
	
} MMMReaderRTESwipeData;
//-------------------------------------------------------------------------------------------

/// Defines a set of values parsed from a MUSE protocol data message.
///
/// \ingroup SwipeModule SwipeDataModule SwipeMUSEProtoModule
///
/// \note This structure is only relevant if using a \swipereader configured with the MUSE
/// protocol.
///
/// \remarks These values are provided for the sake of completeness; in most programming
/// scenarios there is little need for them.
typedef struct MMMReaderMuseSwipeData
{
	/// Provides the device type that the MUSE protocol message came from. This refers to
	/// which part of the \swipereader was physically used.
	///
	/// \remarks See #MuseProtocolDeviceType for a list of available devices.
	MuseProtocolDeviceType DeviceType;
	
	/// Provides the high-order value of the CRC-16 value returned from the MUSE protocol
	/// message.
	MMMByte CrcHigh;
	
	/// Provides the low-order value of the CRC-16 value returned from the MUSE protocol
	/// message.
	MMMByte CrcLow;

} MMMReaderMuseSwipeData;
//-------------------------------------------------------------------------------------------

/// Defines a set of values parsed from a CUTE protocol data message.
///
/// \ingroup SwipeModule SwipeDataModule SwipeCUTEProtoModule
///
/// \note This structure is only relevant if using a \swipereader configured with the CUTE
/// protocol.
///
/// \remarks These values are provided for the sake of completeness; in most programming
/// scenarios there is little need for them.
typedef struct MMMReaderCuteSwipeData
{
	/// Provides the device type that the CUTE protocol message came from. This refers to
	/// which part of the \swipereader was physically used.
	///
	/// \remarks See #CuteProtocolDeviceType for a list of available devices.
	CuteProtocolDeviceType DeviceType;

} MMMReaderCuteSwipeData;
//-------------------------------------------------------------------------------------------

/// Defines a set of values parsed from a Magtek MSR swipe reader.
///
/// \ingroup SwipeModule SwipeDataModule SwipeMagtekProtoModule
///
/// \note This structure is only relevant if using a Magtek MSR swipe reader.
///
/// \remarks These values are provided for the sake of completeness; in most programming
/// scenarios there is little need for them.
typedef struct MMMReaderMagtekMsrSwipeData
{
	/// Provides the encoding type determined from the swiped document.
	///
	/// \remarks See #MagtekMsrProtocolCardType for possible values.
	MagtekMsrProtocolCardType CardEncodeType;
	
} MMMReaderMagtekMsrSwipeData;
//-------------------------------------------------------------------------------------------
	
/// Defines a set of values parsed from a TECS protocol data message.
///
/// \ingroup SwipeModule SwipeDataModule SwipeTECSProtoModule
///
/// \note This structure is only relevant if using a \swipereader configured with the CUTE
/// protocol.
///
/// \remarks These values are provided for the sake of completeness; in most programming
/// scenarios there is little need for them.
typedef struct MMMReaderTecsSwipeData
{
	/// Provides the device type that the TECS protocol message came from. This refers to
	/// which part of the \swipereader was physically used.
	///
	/// \remarks See #TecsProtocolDeviceType for a list of available devices.
	TecsProtocolDeviceType DeviceType;

} MMMReaderTecsSwipeData;
//-------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------

#define MMM_AAMVA_FILE_TYPE_LENGTH		6		// Includes space for null-terminator
#define MMM_AAMVA_IIN_LENGTH			7		// Includes space for null-terminator
#define MMM_AAMVA_MAX_BUFFER_LENGTH		3000
#define MMM_AAMVA_DATA_ID_LENGTH		4		// Space for 3-letter code plus null-terminator
#define MMM_AAMVA_SEX_FIELD_LENGTH		8		// Includes space for null-terminator

/// Defines the measurement format that can be extracted from an \aamva document
typedef enum MMMReaderAAMVAMeasurementFormat
{
	MEASUREMENT_UNKNOWN,
	MEASUREMENT_IN,
	MEASUREMENT_CM,
	MEASUREMENT_LB,
	MEAUSREMENT_KG

} MMMReaderAAMVAMeasurementFormat;

/// Defines the valid \aamva eye colours as stated in ANSI-D20
typedef enum MMMReaderAAMVAEyeColour
{
	EYE_COLOUR_UNK,		//Unknown
	EYE_COLOUR_BLK,		//Black
	EYE_COLOUR_BLU,		//Blue
	EYE_COLOUR_BRO,		//Brown
	EYE_COLOUR_GRY,		//Grey
	EYE_COLOUR_GRN,		//Green
	EYE_COLOUR_HAZ,		//Hazel
	EYE_COLOUR_MAR,		//Maroon
	EYE_COLOUR_PNK,		//Pink
	EYE_COLOUR_DIC		//Dichromatic
} MMMReaderAAMVAEyeColour;

/// Defines the valid \aamva hair colours as stated in ANSI-D20
typedef enum MMMReaderAAMVAHairColour
{
	HAIR_COLOUR_UNK,	//Unknown
	HAIR_COLOUR_BAL,	//Bald
	HAIR_COLOUR_BLK,	//Black
	HAIR_COLOUR_BLN,	//Blond
	HAIR_COLOUR_BRO,	//Brown
	HAIR_COLOUR_GRY,	//Grey
	HAIR_COLOUR_RED,	//Red/Auburn
	HAIR_COLOUR_SDY,	//Sandy
	HAIR_COLOUR_WHI		//White
} MMMReaderAAMVAHairColour;


/// Defines a measurment and the format that the measurement is in
///
/// \ingroup HighLevelDataModule ParsingModule
///
/// \sa MMMReaderAAMVAMeasurement
typedef struct MMMReaderAAMVAMeasurement
{
	unsigned int Measurement;

	MMMReaderAAMVAMeasurementFormat Format;

} MMMReaderAAMVAMeasurement;


/// Defines a collection of data that has been parsed from a North American drivers licence
/// which adhears to the \aamva standard. This structure represents the header values 
/// extracted from the licence data.
///
/// \ingroup HighLevelDataModule ParsingModule
///
/// \sa MMMReaderAAMVAData
typedef struct MMMReaderAAMVAHeader
{
	/// Provides the file type extracted from the licence header.
	char FileType[MMM_AAMVA_FILE_TYPE_LENGTH];
	
	/// Provides the Issuer Identification Number (IIN) that uniquely identifies the issuing
	/// jurisdiction.
	char IIN[MMM_AAMVA_IIN_LENGTH];
	
	/// Provides the name of the issuing jurisdiction, if available.
	char IssuerName[MMM_AAMVA_MAX_BUFFER_LENGTH];
	
	/// Provides the version number of the \aamva standard this data adhears to.
	///
	/// \remarks The following version numbers are accepted, taken from the latest \aamva 
	/// standard document:
	/// <table>
	/// <tr><th>Version</th><th>Description</th></tr>
	///
	/// <tr><td>&lt; 0</td><td>Version number not available or cannot be determined.</td></tr>
	///
	/// <tr><td>0</td><td>Represents licences printed prior to the adoption of the 
	/// \aamva DL/ID-2000 standard.</td></tr>
	///
	/// <tr><td>1</td><td>Represents licences compliant with the \aamva DL/ID-2000 
	/// standard.</td></tr>
	///
	/// <tr><td>2</td><td>Represents licences compliant with the \aamva DL/ID-2003 
	/// standard.</td></tr>
	///
	/// <tr><td>3</td><td>Represents licences compliant with the \aamva DL/ID-2005 
	/// standard.</td></tr>
	/// 
	/// </table>
	int Version;
	
	/// Provides the specific jurisdication version of the licence.
	///
	/// \remarks If this value is less than zero, the jurisdication version is not available
	/// or cannot be determined.
	int JurisdicationVersion;

} MMMReaderAAMVAHeader;

/// Defines a generic data item parsed from a North American drivers licence which adhears
/// to the \aamva standard.
///
/// \ingroup HighLevelDataModule ParsingModule
///
/// \sa MMMReaderAAMVAData
typedef struct MMMReaderAAMVADataItem
{
	/// Provides the 3-letter identifier of this data item.
	///
	/// \remarks The current \aamva standard provides a list of mandatory and optional 
	/// elements, where each element has a unique ID. This data member represents that ID 
	/// value.	
	char ID[MMM_AAMVA_DATA_ID_LENGTH];
	
	/// Provides a descriptive name of this data item.
	///
	/// \remarks This value can be used for display purposes.
	char Name[MMM_AAMVA_MAX_BUFFER_LENGTH];
	
	/// Provides the value of this data item in the form of a null-terminated string.
	///
	/// \remarks The contents and format of this data member depends on what this data item 
	/// represents. For example, dates may be formatted as a single string. To interpret
	/// the data correctly, please refer to the \aamva standard that is specified by the
	/// #MMMReaderAAMVAHeader::Version member.
	char Value[MMM_AAMVA_MAX_BUFFER_LENGTH];

} MMMReaderAAMVADataItem;

/// Defines a collection of values that have been parsed from a North American drivers licence 
/// which adhears to the \aamva standard. This structure contains commonly used data fields.
///
/// \ingroup HighLevelDataModule ParsingModule
///
/// \remarks The \sdk will parse \aamva data and attempt to extract common, useful values from
/// it, e.g. names, address, expiry date, etc. Although the #MMMReaderAAMVAData structure
/// will also contain all data items found in the licence data, this structure can be used
/// to access the commonly used fields.
///
/// \remarks If a particular value could not be found, the corresponding string data member
/// will have a length of zero. If the data represents a MMMReaderDate, all data members of 
/// that structure will be set to zero.
///
/// \remarks All dates parsed are converted from the source format as specified by the
/// particular version of the \aamva standard the licence adhears to.
///
/// \remarks All dates have a 4-digit year.
///
/// \remarks The \sdk will attempt to fill all name fields, such as surname, with the data
/// it has available. For example, if only the #FullName is provided, the \sdk will attempt
/// to split that value up to fill the #Forename and #Surname members.
///
/// \sa MMMReaderAAMVAData
typedef struct MMMReaderAAMVAParsedData
{
	/// Provides the licence number of the licence, if available.
	char LicenceNumber[MMM_AAMVA_MAX_BUFFER_LENGTH];
	
	/// Provides the full name of the licence holder, if available.
	///
	/// \remarks The full name will include the forename, any optional middle names and
	/// the surname.
	char FullName[MMM_AAMVA_MAX_BUFFER_LENGTH];
	
	/// Provides the surname of the licence holder, if available.
	char Surname[MMM_AAMVA_MAX_BUFFER_LENGTH];
	
	/// Provides the forename of the licence holder, if available.
	char Forename[MMM_AAMVA_MAX_BUFFER_LENGTH];
	
	/// Provides the middle name of the licence holder, if available.
	char MiddleName[MMM_AAMVA_MAX_BUFFER_LENGTH];
	
	/// Provides the name suffix of the licence holder, if available (e.g. JR, SR, III, etc).
	char NameSuffix[MMM_AAMVA_DATA_ID_LENGTH];

	/// Provides the forename and middle name of the licence holder, if available.
	char GivenNames[MMM_AAMVA_MAX_BUFFER_LENGTH];
	
	/// Provides a full textual description of the licence holder's gender, if available.
	///
	/// \remarks If not an empty string, this can be one of the following values:
	///		- \c "Male"
	///		- \c "Female"
	///		- \c "Unknown"
	char Sex[MMM_AAMVA_SEX_FIELD_LENGTH];
	
	/// Provides a single initial from the #Sex field, if available.
	///
	/// \remarks This will be one of the following values:
	///		- \c 'M' for \c "Male"
	///		- \c 'F' for \c "Female"
	///		- \c 'U' for \c "Unknown"
	char ShortSex;
	
	/// Provides the date of birth of the licence holder, if available.
	MMMReaderDate DateOfBirth;
	
	/// Provides the date the licence was issued, if available.
	MMMReaderDate IssueDate;
	
	/// Provides the expiry date of the licence, if available.
	MMMReaderDate ExpiryDate;
	
	/// Provides the street name of the licence holder's address, if available.
	char AddressStreet[MMM_AAMVA_MAX_BUFFER_LENGTH];
	
	/// Provides the city of the licence holder's address, if available.
	char AddressCity[MMM_AAMVA_MAX_BUFFER_LENGTH];
	
	/// Provides the state of the licence holder's address, if available.
	char AddressState[MMM_AAMVA_MAX_BUFFER_LENGTH];
	
	/// Provides the postal code of the licence holder's address, if available.
	char AddressPostalCode[MMM_AAMVA_MAX_BUFFER_LENGTH];

	/// Provides the 3 letter country code of the licence holder's address, if available.
	char AddressCountry[MMM_AAMVA_DATA_ID_LENGTH];

	//Provides the height of the licence holder and the measurement format.
	MMMReaderAAMVAMeasurement Height;
	
	//Provides the weight of the licence holder and the measurement format.
	MMMReaderAAMVAMeasurement Weight;
	
	//Provides the ANSI-D20 code of the licence holder's hair colour.
	MMMReaderAAMVAHairColour HairColour;
	
	//Provides the ANSI-D20 code of the licence holder's eye colour.
	MMMReaderAAMVAEyeColour EyeColour;

} MMMReaderAAMVAParsedData;

/// Defines a collection of values that have been parsed from a North American drivers licence 
/// which adhears to the \aamva standard.
///
/// \ingroup HighLevelDataModule ParsingModule
///
/// \remarks The \sdk will parse all header and data items found in the \aamva licence data and
/// store them in this structure.
///
/// \remarks The current \aamva standard defines many data items that could be present on
/// the licence. All data items found will be stored in the #DataItems array. However, the
/// \sdk will also extract the commonly used data items (such as name, address, expiry date, 
/// etc.) and copy them into the #Parsed structure for easier access. 
/// 
/// \remarks If a particular data item is not present in the #Parsed structure, it is still
/// possible to search for it in the #DataItems array, like so:
///
/// \remarks \include AAMVAFindDataItem.cpp
typedef struct MMMReaderAAMVAData
{
	/// Provides the typical/common data items required when handling \aamva licences.
	MMMReaderAAMVAParsedData Parsed;

	/// Provides the header values parsed from an \aamva licence.
	MMMReaderAAMVAHeader Header;

	/// Provides the number of items contained in the #DataItems array.
	int DataItemCount;
	
	/// Provides a pointer to the array of all data items found in the \aamva licence.
	///
	/// \attention This array is dynamically allocated by the \sdk based on the value of
	/// #DataItemCount; this member represents the size of this array. The \sdk will handle
	/// the memory for this array, it does not need to be manually freed.
	MMMReaderAAMVADataItem* DataItems;
	
} MMMReaderAAMVAData;

/// Defines the source type of AAMVA data
///
/// \ingroup ParsingModule
///
/// \sa MMMReader_ParseAAMVA()
typedef enum MMMReaderAAMVADataSource
{
	AAMVA_DS_Barcode,
	AAMVA_DS_MagStripe

} MMMReaderAAMVADataSource;
//-------------------------------------------------------------------------------------------

/// \cond PAGESDK

/// Defines the memory banks that can be read from a UHF tag.
///
/// \ingroup UHFModule
///
/// \note Depending on which UHF tag is being read, not all of the following memory
/// banks may contain data.
///
/// \sa MMMReader_UHFGetMemoryBankData()
typedef enum MMMReaderUHFMemoryBank
{
	UHF_MB_RESERVED,
	UHF_MB_EPC,
	UHF_MB_TID,
	UHF_MB_USER
} MMMReaderUHFMemoryBank;

/// TagID data that has been read from a UHF tag
///
/// \ingroup UHFModule
///
/// \note This structure is only relevant if using a \pagereader that has UHF capabilities.
///
/// \remarks This structure is returned through a #MMMReaderDataCallback passed to the
/// MMMReader_UHFGetTagID() API.
typedef struct MMMReaderUHFTagIDData
{
	///The data read from the TID memory bank of a UHF tag
	unsigned char RawDataBuffer[8];

	///The size of the RawDataBuffer
	unsigned long RawDataBufferSize;

	///The Manufacturer Identifier extracted from the TagID. This will only be
	///present if the tag class-identifier is E0h
	unsigned short ManufacturerID;

	///The 48-bit serial number extracted from the TagID. This will only be
	///present if the tag class-identifier is E0h
	unsigned char SerialNumber[6];

	///The 12-bit mask designer identifier extracted from the TagID. This
	///will only be present if the class-identifier is E2h
	unsigned short MaskDesignerID;

	///The 12-bit vendor defined tag model number, extracted from the
	///TagID. This will only be present if the class-identifier is E2h
	unsigned short ModelNumber;
	
} MMMReaderUHFTagIDData;

/// Data that has been read from a UHF tag memory bank
///
/// \ingroup UHFModule
///
/// \note This structure is only relevant if using a \pagereader that has UHF capabilities.
///
/// \remarks This structure is returned through a #MMMReaderDataCallback passed to the
/// MMMReader_UHFGetMemoryFromMap() API.
typedef struct MMMReaderUHFTagMemoryData
{
	unsigned char UID[100];
	unsigned int UIDLen;
	unsigned char DataBuffer[100];
	unsigned int DataBufferLen;
	MMMReaderUHFMemoryBank MemoryBank;
	unsigned int BankOffset;
} MMMReaderUHFTagMemoryData;

/// Defines the options for detemining document orientation.
///
/// \ingroup HighLevelSettingsModule HLSettingsModule LLSettingsModule
///
/// \note This structure is only relevant if using the \hlapi.
///
/// \sa #MMMReaderSettings::puDataToSend.autoOrientation
typedef enum MMMReaderAutoOrientation
{
	/// Use ICAO MRZ location.
	MRZ = 1,

	/// Use magstripe at top of document.
	MAGSTRIPE_TOP = 2,
	
	/// Use magstripe at bottom of document.
	MAGSTRIPE_BOTTOM = 4,
	
	/// Use the orientation of any barcodes found.
	BARCODE = 8,

	/// Use photo.
	//PHOTO = 16

} MMMReaderAutoOrientation;

//#ifdef DCA

//  These two must match with coresponding values in layoutConstants.h
#define MMM_LDB_MAX_NAME_LENGTH		120    // was 60, doubled to handle textCompare output name (process + item name)  // this is to limit the size of the puData struct to return the data to the client
#define MMM_LDB_MAX_ITEM_LIST_LENGTH	30  // this is to limit the size of the puData struct to return the data to the client
#define MMM_MAX_PATH_LENGTH	3000  // this is to limit the size of the puData struct to return the data to the client


/// Defines the catagory level of a process.
///
/// \ingroup HighLevelDataModule
///
/// \note The category level of a process is directing linked to the license permissions.
///
/// \remarks 

typedef enum MMMReaderProcessCategory
{
	/// Inspection level process.
	PC_INSPECTION = 0,			
	/// Data Capture level process.
	PC_DATA_CAPTURE,			
	/// Authentication level process.
	PC_AUTHENTCIATION			
} MMMReaderProcessCategory;

//  If this enum is changed, also update the enum/string converters in LayoutDatabaseDll.cpp
typedef enum
{
	PT_IcaoMrz = 0, 
	PT_TextZone, 
	PT_SubImage, 
	PT_Anchor, 
	PT_PatternMatch, 
	PT_BlankArea, 
	PT_Confirm, 
	PT_Tamper, 
	PT_TextCompare, 
	PT_Barcode, 
	PT_MagSwipe, 
	PT_RF, 
	PT_UHF, 
	PT_Custom, 
	PT_Plugables, 
	PT_IcaoMrzQA,
	PT_DominantColor,
	numOfProcessTypes
} ProcessType;

typedef enum {SI_Photo = 0, SI_Signature, SI_FingerPrint, SI_InspectionPositive, SI_InspectionNegative, SI_InspectionExamination, numOfSubImageTypes} SubImageType;

/// Defines a structure that describes a bounding box, rotation and light source for retrieved data item 
///
/// \ingroup HighLevelDataModule
///
/// \sa MMMReader_GetDataItem()

typedef struct LocateInfo
{
	/// Stores the left/top and width/height.
	long left, top;
	long width, height;

	/// Stores the rotation of the image area.
	int rotation;

	/// Stores the image source as a MMMReaderDataType enum. (MMMReaderDataType is defined in MMMReaderHighLevelAPI.h) 
	int imageSource;

} LocateInfo;

/// Defines a result structure returned for each verifier in the document
///
/// \ingroup HighLevelDataModule
///
/// \remarks 
typedef struct MMMReaderVerifierResult
{
	///Specific name of the identified document.
	char SpecificVariantName[MMM_LDB_MAX_NAME_LENGTH];

	///Variant name of the identified document. This can be empty.
	char VerifierName[MMM_LDB_MAX_NAME_LENGTH];

	///Mininum confidence threshold for the verifier
	float MininumConfidence;

	///Calculated confidence for the verifier
	float CalculatedConfidence;

	///Verifier Weight
	unsigned long Weight;

	///Verifier Type
	char VerifierType[MMM_LDB_MAX_NAME_LENGTH];

} MMMReaderVerifierResult;

/// Defines a result structure returned on completion of the identification stage for one side
///
/// \ingroup HighLevelDataModule
///
/// \remarks 
typedef struct MMMReaderIdentifyResult
{
	///Group name of the identified document.
	char GroupName[MMM_LDB_MAX_NAME_LENGTH];

	///Specific name of the identified document.
	char SpecificName[MMM_LDB_MAX_NAME_LENGTH];

	///Variant name of the identified document. This can be empty.
	char VariantName[MMM_LDB_MAX_NAME_LENGTH];

	///Display name of the identified document - regional language.
	wchar_t DisplayNameRegional[2*MMM_LDB_MAX_NAME_LENGTH];

	///Risk Alert field for the identified document. This can be empty.
	char RiskAlert[MMM_LDB_MAX_NAME_LENGTH];

	///Mininum confidence threshold for this document identication to be sucessful at a Specific level
	float SpecificMininumIdConfidence;

	///Calculated confidence by the identification stage at a Specific level
	float SpecificIdConfidence;

	///Mininum confidence threshold for this document identication to be sucessful at a Variant level, can be zero when no variant is involved
	float VariantMininumIdConfidence;

	///Calculated confidence by the identification stage at a Variant level, can be zero when no variant is involved
	float VariantIdConfidence;

	///Time in millisecond for the identification stage
	unsigned long TimeTakenToIdentify;  

	///Time in millisecond for the identification stage
	unsigned long VerifierCount;  

	/// ID number for scan
	unsigned long ScanNumber;

	/// Scan directory path
	char ScanDirectory[MMM_MAX_PATH_LENGTH];

	///Flag to indicate the document will be viewed in portrait mode. Images will be rotated 90 degree
	MMMR_BOOL PortraitOrientation;

	///Flag to indicate a flip side document link exists in the layout
	MMMR_BOOL FlipSideAvailable;

	///Flag to indicate the document was placed with back side on the document window. This bool is only 
	///  useful if a FlipSideAvailable is available and two sided scanning is active
	MMMR_BOOL BackSideFlag;

} MMMReaderIdentifyResult;

/// Defines a result structure returned on completion of the document processing for a side.  
///
/// \ingroup HighLevelDataModule
///
/// \remarks 
typedef struct MMMReaderCompletionResult 
{
	///Specific name of the flipSide document.
	char SpecificNameFlipSide[MMM_LDB_MAX_NAME_LENGTH];

	///Variant name of the flipSide document. This can be empty.
	char VariantNameFlipSide[MMM_LDB_MAX_NAME_LENGTH];

	///Mininum confidence threshold for this document authentication to be considered a pass
	float MininumAuthConfidence;

	///Calculated confidence over all the processes with weight > 0
	float AuthConfidence;

	///Time in millisecond for the authentication stage
	unsigned long TimeTakenToProcessDocument; 

	///Time in millisecond for the complete document process
	unsigned long TotalTimeTaken;

	///Number of authentication processes passed
	unsigned long PassedAuthenticationCount;

	///Number of authentication processes failed
	unsigned long FailedAuthenticationCount;

	/// ID number for scan - if a document flip was done, this will be the second side scan
	unsigned long ScanNumber;

	/// Scan directory path - if a document flip was done, this will be the second side scan
	char ScanDirectory[MMM_MAX_PATH_LENGTH];

	///Tristate to indicate the over all authentication result - Pass(True) = (AuthConfidence > MininumAuthConfidence) && MandatoryFailFromProcess != true,   NotPreformed - AuthConfidence == -1 
	TRISTATE AuthenticationResult;

	/// Indicates a document flip was done,
	MMMR_BOOL FlipSideScanAvailable;

	///Flag to indicate a flipside was identified and flipside names will be populated
	MMMR_BOOL FlipSideIdentified;

	///Flag to indicate a process has invoked authentication override fail
	MMMR_BOOL MandatoryFailFromProcess;

} MMMReaderCompletionResult;


/// Defines a general structure used by all data items for that are produced by the document 
///   process steps. These data items will be released on a new document detected (exception 
///   is a flip side for single side reader).  
///
/// \ingroup HighLevelDataModule
///
/// \remarks 
typedef struct MMMDCADataObject
{
	///  item name. This name will match to the layout database output data item list for the process
	char ItemName[MMM_LDB_MAX_NAME_LENGTH];
	///  left, top, width, height of the area on the scan image where the data item was found. Only for images, text zones and barcodes
	LocateInfo LocationInfo;
	///  pointer to the memory buffer conatining the data item value
	void *Data;
	///  length of the memory buffer
	int DataSize;
	///  data format type of the memory buffer
	MMMReaderDataFormat Type;
} MMMDCADataObject;


#define MMMDCATextDataObject MMMDCADataObject
#define MMMDCAImageDataObject MMMDCADataObject


/// Defines a common structure used by other all process result structures 
///
/// \ingroup HighLevelDataModule
///
/// \remarks This structure will be included with all process results
typedef struct ProcessResultHeader
{
	///name of the process to which these results are from.
	char ProcessName[MMM_LDB_MAX_NAME_LENGTH];

	///string name for the process type
	char ProcessTypeStr[MMM_LDB_MAX_NAME_LENGTH];

	///process type enum
	ProcessType ProcessTypeCode;

	/// process category
	MMMReaderProcessCategory ProcessCategory;

	///integer weighting assigned to the process for authentication confidence computing, a zero indicates the process 
	/// will not be included into the total authentication confidence
	int Weighting;

	///calculated confidence for the process
	float Confidence;

	///mininum confidence threshold for a success result
	float ConfidenceSuccessThreshold;

	///Horizontal offset applied to the process bounding box
	double xOffset;

	///Vertical offset applied to the process bounding box
	double yOffset;

	///boolean result for the process execution, 
	MMMR_BOOL Result;

	///boolean result for the process execution, 
	MMMR_BOOL MandatoryFailFromProcess;

} ProcessResultHeader;

/// Defines a general result structure used for process results from process types PT_UHF, PT_Plugables, PT_Custom 
///
/// \ingroup HighLevelDataModule
///
/// \remarks 
typedef struct MMMReaderProcessResultGeneral
{
	///Process Header data
	ProcessResultHeader ProcessResults;

	///Number of data items set by the process, these are from the process outputDataItemList
	unsigned long ItemCount;

	///Data item(s) from the outputDataItemList
	MMMDCADataObject ItemList[MMM_LDB_MAX_ITEM_LIST_LENGTH];

} MMMReaderProcessResultGeneral;


/// Defines a MMMReaderProcessResultIcaoMrz structure used for process results from process type PT_IcaoMrz 
///
/// \ingroup HighLevelDataModule
///
/// \remarks 
typedef struct MMMReaderProcessResultIcaoMrz
{
	///Process Header data
	ProcessResultHeader ProcessResults;

	///mrz lines concatenated together
	MMMDCATextDataObject CodeLine;

	///mrz lines and parse mrz fields, MMMReaderCodelineData structre
	MMMDCADataObject CodeLineData;

} MMMReaderProcessResultIcaoMrz;



typedef struct QaCompareMeasurementsLine
{
	///The TRISTATE results for the various QA checks on a single line
	TRISTATE LeftMarginCheck;
	TRISTATE RightMarginCheck;
	TRISTATE PositionCheck;
	TRISTATE SkewCheck;
	TRISTATE AverageStrokeWidthCheck;
	TRISTATE BadCharsCheck;
	TRISTATE BrokenCharsCheck;
	TRISTATE CharacterCountCheck;
	TRISTATE CharacterHeightCheck;
	TRISTATE CodelineWidthCheck;
	TRISTATE GapBelowCheck;
	TRISTATE OverallLineCheck;

	float LeftMargin;
	float RightMargin;
	float PositionTop;
	float PositionBottom;
	float Skew;
	float AverageStrokeWidth;
	int BadChars;
	int BrokenChars;
	int CharacterCount;
	float CharacterHeight;
	float CodelineWidth;
	float GapBelow;

	float Top;
	float Bottom;
	float Left;
	float Right;

	char OCRResults[MAX_CODELINE_LENGTH];
	int OCRScores[MAX_CODELINE_LENGTH];

} QaCompareMeasurementsLine;


typedef struct QaCompareMeasurements
{
	///The Min/Max values for the various QA checks

	float LeftMarginMin;
	float LeftMarginMax;
	float RightMarginMin;
	float RightMarginMax;
	float PositionTopMin;
	float PositionTopMax;
	float PositionMiddleMin;
	float PositionMiddleMax;
	float PositionBottomMin;
	float PositionBottomMax;
	float SkewMin;
	float SkewMax;
	float AverageStrokeWidthMin;
	float AverageStrokeWidthMax;
	float CharacterHeightMin;
	float CharacterHeightMax;
	float CodelineWidthMin;
	float CodelineWidthMax;
	float GapBelowMin;
	float GapBelowMax;
	int BadChars;
	int BrokenChars;
	int CharacterCount;
	float ThrowoutAmount;

	int NumOfSpots;
	int NumOfLines;

	///The TRISTATE results for the various QA checks
	QaCompareMeasurementsLine LineData[LINES];

	TRISTATE OverallCheck;
	MMMReaderCheckDigitResult CheckDigitsCheck;
	bool UpsideDown;

} QaCompareMeasurements;


/// Defines a MMMReaderProcessResultIcaoMrzQA structure used for process results from process type PT_IcaoMrzQA 
///
/// \ingroup HighLevelDataModule
///
/// \remarks 
typedef struct MMMReaderProcessResultIcaoMrzQA
{
	///Process Header data
	ProcessResultHeader ProcessResults;

	///QaCompareMeasurements structure describing the QA comparison results
	MMMDCADataObject QaCompareMeasurements;

	///Image data item showing the area extarcted from the document
	MMMDCAImageDataObject MrzQAImage;

} MMMReaderProcessResultIcaoMrzQA;

/// Defines a MMMReaderProcessResultTextCompare structure used for process results from process type PT_TextCompare 
///
/// \ingroup HighLevelDataModule
///
/// \remarks 
typedef struct MMMReaderProcessResultTextCompare
{
	///Process Header data
	ProcessResultHeader ProcessResults;

	///String/Date data items used for the comparison
	MMMDCATextDataObject TextCompareSideA;   
	MMMDCATextDataObject TextCompareSideB;

} MMMReaderProcessResultTextCompare;

/// Defines a MMMReaderProcessResultTextZone structure used for process results from process type PT_TextZone 
///
/// \ingroup HighLevelDataModule
///
/// \remarks 
typedef struct MMMReaderProcessResultTextZone
{
	///Process Header data
	ProcessResultHeader ProcessResults;

	///String/Date data item of all the text extracted from the document
	MMMDCATextDataObject TextZoneText;

	///Image data item showing the area used for extraction from the document
	MMMDCAImageDataObject TextZoneImage;
	
	///Number of text fields extracted from the textzone.
	unsigned long TextFieldCount;

	///String/Date data item(s) for each text field defined in the layout database of the docuemnt
	MMMDCADataObject TextFieldList[MMM_LDB_MAX_ITEM_LIST_LENGTH];
} MMMReaderProcessResultTextZone;

/// Defines a MMMReaderProcessResultImageCompare structure used for process results from process types PT_SubImage, PT_PatternMatch, PT_Confirm 
///
/// \ingroup HighLevelDataModule
///
/// \remarks 
typedef struct MMMReaderProcessResultImageCompare
{
	///Process Header data
	ProcessResultHeader ProcessResults;
	
	///SubImage refernce text string
	SubImageType SImageType;
	
	///Image data item showing the area used for comparison from the document
	MMMDCAImageDataObject MatchImage;

	///Image data item retrieved from the reference database directory
	MMMDCAImageDataObject ReferenceImage;

	///SubImage text information
	MMMDCAImageDataObject ReferenceText;

} MMMReaderProcessResultImageCompare;

/// Defines a MMMReaderProcessResultImageZone structure used for process results from process types PT_BlankArea, PT_Tamper, PT_Anchor
///
/// \ingroup HighLevelDataModule
///
/// \remarks 
typedef struct MMMReaderProcessResultImageZone
{
	///Process Header data
	ProcessResultHeader ProcessResults;

	///Image data item showing the area extarcted from the document
	MMMDCAImageDataObject Image;

} MMMReaderProcessResultImageZone;


/// Defines all the different mag and barcode content types the \sdk can recognize.
///
/// \ingroup HighLevelDataModule
///
/// \remarks 
typedef enum MMMReaderBarcodeMagContentType
{
	BMCT_EMPTY,
	BMCT_AAMVA,
	BMCT_ILO,
	BMCT_ANSI_STRING
} MMMReaderBarcodeMagContentType;

/// Defines a structure that describes a barcode 
///
/// \ingroup HighLevelDataModule
///
/// \remarks 
typedef struct MMMReaderBarcodeInformation
{
	/// string name for the barcode
	char FeatureName[MMM_LDB_MAX_NAME_LENGTH];

	/// feature id for the barcode 
	MMMPluginID FeatureTypeID;

	/// content type decoded from the barcode binary data 
	MMMReaderBarcodeMagContentType ContentType;
} MMMReaderBarcodeInformation;

/// Defines a MMMReaderProcessResultBarcode structure used for process results from process type PT_Barcode 
///
/// \ingroup HighLevelDataModule
///
/// \remarks 
typedef struct MMMReaderProcessResultBarcode
{
	///Process Header data
	ProcessResultHeader ProcessResults;

	///Binary data extracted from the barcode
	MMMDCADataObject BarcodeData;

	///MMMReaderBarcodeInformation structure describing the barcode
	MMMDCADataObject BarcodeInformation;

	///Image data item showing the area extarcted from the document
	MMMDCAImageDataObject  BarcodeImage;

	//MMMReaderAAMVAData structure or ansi string parsed from BarcodeData 
	MMMDCADataObject BarcodeContent;

} MMMReaderProcessResultBarcode;

/// Defines a MMMReaderProcessResultMagData structure used for process results from process type PT_MagSwipe 
///
/// \ingroup HighLevelDataModule
///
/// \remarks 
typedef struct MMMReaderProcessResultMagData
{
	///Process Header data
	ProcessResultHeader ProcessResults;

	///MMMReaderMsrData structure extracted from the mag stripe
	MMMDCADataObject MagData;   //  MMMReaderMsrData

	///MMMReaderAAMVAData structure parsed from the MMMReaderMsrData data
	MMMDCADataObject MagContent;

} MMMReaderProcessResultMagData;

//==============================================================
// RFID results
//==============================================================


typedef struct MMMReaderRFResults
{
	///TRISTATE results for the retrieval of the data groups
	TRISTATE DataGroupList[16];
	TRISTATE DataGroup_SOD;
	TRISTATE DataGroup_EFCOM;
	TRISTATE DataGroup_EFCardAccess;

	///MMMReaderValidationCode results for the hash validation on the data groups
	MMMReaderValidationCode DataGroupValidateList[16];
	MMMReaderValidationCode DataGroupValidate_EFCOM;  // future use

	MMMReaderValidationCode SOD_HASH_Result;
	MMMReaderValidationCode SOD_DS_Result;
	MMMReaderValidationCode SOD_DOC_SIGNER_Result;

	///The TRISTATE results for the various rf checks
	TRISTATE BAC_Result;
	TRISTATE SAC_Result;
	TRISTATE AA_Result;
	TRISTATE EAC_CVCA_Result;
	TRISTATE EAC_CHIP_Result;
	TRISTATE EAC_TERM_Result;
	TRISTATE EFCOMtoSOD_Result;
	TRISTATE MRZtoMRZ_Result;

	char ChipID[16];
	char AirBaud[16];

} MMMReaderRFResults;


/// Defines a MMMReaderProcessResultRF structure used for process results from process type PT_RF 
///
/// \ingroup HighLevelDataModule
///
/// \remarks 
typedef struct MMMReaderProcessResultRF
{
	///Process Header data
	ProcessResultHeader ProcessResults;

	///RF Results for various tristate tests
	MMMDCADataObject RFResults;

	///DG1 parsed data
	MMMDCADataObject CodeLineData;  /// MMMReaderCodelineData
	
	///DG2 parsed data
	MMMDCADataObject PhotoData;
	
	///DG3FingerprintData
	MMMDCADataObject FingerPrint;

    ///Number of data groups extracted from rf chip
    unsigned long DataGroupsCount;

    ///Datagroups extracted from the rf chip 
    MMMDCADataObject DataGroups[18];

}
 MMMReaderProcessResultRF;


#define MAX_PERMISSION_FIELD_LENGTH	100
/// Defines a structure that specifies which functionality the current license grants 
/// permission to use.
///
/// \ingroup HighLevelDataModule
///
/// \sa MMMReader_GetLicensePermissions()
typedef struct PermissionList 
{	
	///The maximum number of specific items that can be added to the document data base
	unsigned long MaxAdditionalSpecific;

	///The number of document groups/subgroups that are can be enabled by this license file
	unsigned long ActiveGroupListCount;

	///A flag to indicate if the SDK can be used to gather inspection data
	///(images, ICAO codeline, ICAO 9303 RFID data).
	bool Inspection;

	///A flag to indicate if the SDK can be used without a 3M reader attached
	bool Readerless;

	///A flag to indicate if the SDK can be used to capture data from a document
	bool DataCapture;

	///A flag to indicate if the SDK can be used to help decide if a document is authentic or fake
	bool Authentication;

	///A flag to indicate if the commerical application is allowed to run
	bool CommericalApplication;

	///A flag to indicate if a license file has been successfully loaded
	bool LicenseFileLoaded;

	///Customer Number
	char CustomerNumber[MAX_PERMISSION_FIELD_LENGTH];

	///Computer Name
	char ComputerName[MAX_PERMISSION_FIELD_LENGTH];

	///Expiry Date
	char ExpiryDate[MAX_PERMISSION_FIELD_LENGTH];

} PermissionList;
//#endif //DCA

/// \endcond

//-------------------------------------------------------------------------------------------

#if !(defined(__linux__) && defined(__arm__))
#pragma pack(pop)
#endif

#endif //_MMMReaderDataTypes_H_
