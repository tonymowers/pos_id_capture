#ifndef _MMMREADERLOWLEVELAPI_H_
#define _MMMREADERLOWLEVELAPI_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "MMMReaderDataTypes.h"
#include "MMMReaderSettings.h"

#ifndef __linux__
#define ENABLE_GDIPLUS
#define HANDLE_DEFINED
#endif

#ifndef HANDLE_DEFINED
typedef void *HANDLE;
#endif

#ifdef DISABLE_GDIPLUS
#undef ENABLE_GDIPLUS
#endif // DISABLE_GDIPLUS

#ifdef ENABLE_GDIPLUS
#include <gdiplus.h>
#endif // ENABLE_GDIPLUS

/// \cond INTERNAL

#ifdef MMMREADERLOWLEVELAPI_EXPORTS
#ifdef __linux__
#define MMM_READER_LOW_LEVEL_API __attribute__ ((visibility("default")))
#else
#define MMM_READER_LOW_LEVEL_API __declspec(dllexport)
#endif
#elif defined (MMMREADERLOWLEVELAPI_STATIC_LINK)
#define MMM_READER_LOW_LEVEL_API
#else
#ifdef __linux__
#define MMM_READER_LOW_LEVEL_API
#else
#define MMM_READER_LOW_LEVEL_API __declspec(dllimport)
#endif
#endif

/// \endcond
// INTERNAL

/// \cond INTERNAL
// This version number can be used to check that the DLL that is loaded is intended for the header file/lib
// that you are using. This will be increased every time compatibility with the previous release is broken.
#define MMM_READER_LOW_LEVEL_API_VERSION	11
/// \endcond

/// \cond PAGESDK

/// Frees all data that has been allocated by the \sdk and returned to the host application.
///
/// \ingroup LowLevelModule
///
/// \note This API is only relevant if using the \llapi.
///
/// \return #NO_ERROR_OCCURRED if successful, otherwise a #MMMReaderErrorCode stating what
/// error occurred.
///
/// \attention The \sdk will manage all memory related to data returned to the host application.
/// Any data returned by the \sdk should not be deleted by the host application, as this can
/// cause issues if the application is not using the same memory heap.
///
/// \remarks This API cannot be called when there are any oustanding non-blocking operations,
/// as this could cause issues with the data being created at that time. This includes calling
/// this API from within a callback function.
///
/// \remarks After this API has been called, any pointers to objects supplied via the
/// callbacks are no longer valid, and should not be used.
///
/// \remarks Typically, this API would be called once a document has been fully processed,
/// before re-entering the document detection logic.
///
/// \sa MMMReader_DestroyCachedObject()
MMM_READER_LOW_LEVEL_API MMMReaderErrorCode MMMReader_DestroyCachedData();

/// Frees a single object that has been supplied from a callback.
///
/// \ingroup LowLevelModule
///
/// \note This API is only relevant if using the \llapi.
///
/// \param[in] aObject A pointer to the object to destroy.
///
/// \return #NO_ERROR_OCCURRED if successful, otherwise a #MMMReaderErrorCode stating what
/// error occurred.
///
/// \remarks This API is useful in that the host application is able to free unused memory
/// as soon as is required, rather than waiting until the document has been fully processed.
/// Therefore, using this API can provide better memory performance than using
/// MMMReader_DestroyCachedData().
///
/// \remarks Unlike MMMReader_DestroyCachedData(), this API may be used at any time, though
/// care should be taken to ensure that the \a aCobject is no longer required.
///
/// \remarks Once destroyed, the pointer to \a aObject is invalid and should not be used
/// anymore.
///
/// \sa MMMReader_DestroyCachedData()
MMM_READER_LOW_LEVEL_API MMMReaderErrorCode MMMReader_DestroyCachedObject(
	void *aObject
);

/// Gets a list of serial numbers of all \pagereaders that are currently connected to the
/// host machine.
///
/// \ingroup LowLevelModule
///
/// \note This API is only relevant if using multiple \pagereaders with the \llapi. The
/// \hlapi has an equivalent API called MMMReader_GetConnectedScanners().
///
/// \param[out] aSerialNumbers A pointer to a string buffer which will contain the list
/// of serial numbers found. This can be \c NULL; see the remarks section for more details.
///
/// \param[in,out] aSerialNumbersLen Initially will contain the length of the \a aSerialNumbers
/// buffer. When the function returns, this value will be the actual length of the string.
///
/// \param[out] aNumScanners A pointer to an integer which will return how many \pagereaders
/// are currently connected to the host machine.
///
/// \return #NO_ERROR_OCCURRED if successful, otherwise a #MMMReaderErrorCode stating what
/// error occurred.
///
/// \remarks This API should be called twice; the first time, \a aSerialNumbers should be
/// \c NULL and \a aSerialNumbersLen should be zero. This will provide the correct length
/// of the \a aSerialNumbers buffer via \a aSerialNumbersLen. The second time should then
/// pass a correctly allocated string buffer to contain the results.
///
/// \remarks The serial numbers returned via \a aSerialNumbers are separated by the semi-colon
/// (\c ";") character.
///
/// \remarks If only one \pagereader is connected, then only one serial number is returned
/// via \a aSerialNumbers.
///
/// \remarks If more than one \pagereader is connected, then this API should be used in
/// conjunction with MMMReader_LL_SelectScanner() to attach to the appropriate \pagereader.
///
/// \sa MMMReader_LL_SelectScanner()
MMM_READER_LOW_LEVEL_API MMMReaderErrorCode MMMReader_LL_GetConnectedScanners(
	RTCHAR *aSerialNumbers,
	int aSerialNumbersLen,
	int *aNumScanners
);

/// Attaches to the indicated \pagereader via its serial number.
///
/// \ingroup LowLevelModule
///
/// \note This API is only relevant if using multiple \pagereaders with the \llapi. The
/// \hlapi has an equivalent API called MMMReader_SelectScanner().
///
/// \param[in] aSerialNumber A null-terminated string specifying the serial number of the
/// \pagereader to attach to.
///
/// \return #NO_ERROR_OCCURRED if successful, otherwise a #MMMReaderErrorCode stating what
/// error occurred.
///
/// \remarks This API allows the host application to operate correctly when there are multiple
/// \pagereaders connected to the same host machine. If there is only one \pagereader connected,
/// then there is no need to call this API.
///
/// \remarks The \a aSerialNumber used is retrieved from the MMMReader_LL_GetConnectedScanners()
/// API.
///
/// \note Special configurations may be required to operate multiple \pagereaders from
/// one host application. Customers wishing to use the \sdk in this way should contact \ssd for
/// assistance.
///
/// \note This API does not allow the use of multiple \pagereaders at the same time within
/// the same host application; one \pagereader must always be selected at any one time. However
/// it is possible for two host applications/instances to control one \pagereader via this
/// API.
///
/// \sa MMMReader_LL_GetConnectedScanners()
MMM_READER_LOW_LEVEL_API MMMReaderErrorCode MMMReader_LL_SelectScanner(
	const char *aSerialNumber
);


/// Gets hardware information about the \pagereader that is currently connected.
///
/// \ingroup LowLevelModule
///
/// \note This API is only relevant if using the \llapi.
///
/// \return A pointer to a MMMReaderHardwareConfig structure containing all the information
/// regarding the currently connected \pagereader.
///
/// \remarks This API should only be used after calling MMMReader_CameraInitialise() to
/// ensure a correctly populated structure.
///
/// \sa MMMReaderHardwareConfig, MMMReader_CameraInitialise()
MMM_READER_LOW_LEVEL_API MMMReaderHardwareConfig *MMMReader_GetHardwareConfig();

/// Reboots the \pagereader camera to attempt to fix certain hardware errors.
///
/// \ingroup LowLevelModule
///
/// \note This API is only relevant if using the \llapi.
///
/// \return #NO_ERROR_OCCURRED if successful, otherwise a #MMMReaderErrorCode stating what
/// error occurred.
///
/// \remarks This API is useful if a hardware error occurs with the \pagereader camera, for
/// example if the \sdk reports an error stating that the camera or a related device is missing
/// yet the \pagereader is connected properly. This API can be used to attempt to fix
/// such errors without resorting to manually disconnecting the hardware.
///
/// \remarks The term "reboot" in this context means that the camera will be disabled at an
/// operating system level and re-enabled after a period of time in the hope that the
/// operating system will recognise it again and allow access to it.
///
/// \note Rebooting the camera may take some time, and should be used as a last resort.
MMM_READER_LOW_LEVEL_API MMMReaderErrorCode MMMReader_Reboot();

/// \endcond
// PAGESDK

///////// Camera APIs

/// \cond PAGESDK

/// Initialises the \pagereader camera and allocates resources to use the camera, LEDs and
/// illumination hardware.
///
/// \ingroup CameraModule
///
/// \note This API is only relevant if using a \pagereader with the \llapi.
///
/// \param[in] aSettings A pointer to a CAMERASET structure which should contain the required
/// settings for the \pagereader.
///
/// \param[in] aReaderDir A null-terminated string containing the directory path to the
/// reference images. See the remarks section below for more details.
///
/// \param[in] aStartSuspended Specify whether the camera is left in a suspended state after
/// initialisation.
///
/// \return #NO_ERROR_OCCURRED if successful, otherwise a #MMMReaderErrorCode stating what
/// error occurred.
///
/// \attention The MMMReader_CameraShutdown() API must be called when finished with the camera
/// to free all resources allocated.
///
/// \remarks \a aSettings would typically be retrieved from the MMMReader_LL_LoadSettings()
/// or MMMReader_LL_LoadCameraImageSettings() APIs.
///
/// \remarks The value to pass into \a aReaderDir depends on which \pagereader is being used:
///		- For \rte8000 and \rte8000hs readers, reference images are stored in the Mass Storage
///		Device (MSD); the directory path for this can be retrieved from
///		#MMMReaderSettings::puReaderDir or the MMMReader_LL_GetReaderDir() API.
///		- For \at9000 and \kioskreader readers, reference images are stored directly in the
///		internal camera memory; these cannot be accessed, so \a aReaderDir can be \c NULL in
///		this case.
///
/// \remarks To cover both cases, it is safe to use #MMMReaderSettings::puReaderDir or the
/// MMMReader_LL_GetReaderDir() API as the value for \a aReaderDir; the \sdk will determine
/// how to proceed.
///
/// \remarks The \a aStartSuspended flag can be used to allow another application to use
/// the camera. If the camera is in use by another application, the camera will automatically
/// be set to the suspended state.
///
/// \sa MMMReader_CameraShutdown(), MMMReader_IsCameraInitialised(), MMMReader_CameraResume(),
/// MMMReader_CameraSuspend(), MMMReader_IsCameraSuspended()
MMM_READER_LOW_LEVEL_API MMMReaderErrorCode MMMReader_CameraInitialise(
	CAMERASET *aSettings,
	RTCHAR *aReaderDir,
	bool aStartSuspended
);

/// Shuts down the \pagereader camera, which will disable all illumination, cleanly
/// disconnect from the camera and free all related resources.
///
/// \ingroup CameraModule
///
/// \note This API is only relevant if using a \pagereader with the \llapi.
///
/// \return #NO_ERROR_OCCURRED if successful, otherwise a #MMMReaderErrorCode stating what
/// error occurred.
///
/// \attention This API must always be called before closing the host application to restore
/// the camera to a consistent state for other applications to use.
///
/// \sa MMMReader_CameraInitialise(), MMMReader_IsCameraInitialised()
MMM_READER_LOW_LEVEL_API MMMReaderErrorCode MMMReader_CameraShutdown();


/// Updates the Camera module settings to use.
///
/// \ingroup CameraModule
///
/// \note This API is only relevant if using the \llapi.
///
/// \param[in] aSettings A pointer to a CAMERASET structure which should contain the required
/// settings for the \pagereader.
///
/// \return #NO_ERROR_OCCURRED if successful, otherwise a #MMMReaderErrorCode stating what
/// error occurred.
///
/// \remarks This API is required to update some settings to compensate for a changed image
/// scale factor.  values stored in DocDetect and DocumentPositioning structure need to be
/// adjusted to match with the scale factor.
///
MMM_READER_LOW_LEVEL_API MMMReaderErrorCode MMMReader_CameraUpdateSettings(
	MMMReaderSettings *aTheSettings
);

/// Reconnects to the \pagereader camera after it has been suspended by either calling
/// MMMReader_CameraSuspend(), or via the MMMReader_CameraInitialise() API with the
/// \a aStartSuspended flag set.
///
/// \ingroup CameraModule
///
/// \note This API is only relevant if using a \pagereader with the \llapi.
///
/// \param[in] aCamSettings A pointer to a CAMERASET structure which should contain the required
/// settings for the \pagereader.
///
/// \return #NO_ERROR_OCCURRED if successful, otherwise a #MMMReaderErrorCode stating what
/// error occurred.
///
/// \remarks \a aCamSettings would typically be retrieved from the MMMReader_LL_LoadSettings()
/// or MMMReader_LL_LoadCameraImageSettings() APIs.
///
/// \remarks The \pagereader camera must not be in use by another application for this API
/// to succeed.
///
/// \sa MMMReader_CameraInitialise(), MMMReader_CameraSuspend(), MMMReader_IsCameraSuspended()
MMM_READER_LOW_LEVEL_API MMMReaderErrorCode MMMReader_CameraResume(
	CAMERASET *aCamSettings
);

/// Releases the \pagereader camera to allow another application to use it.
///
/// \ingroup CameraModule
///
/// \note This API is only relevant if using a \pagereader with the \llapi.
///
/// \return #NO_ERROR_OCCURRED if successful, otherwise a #MMMReaderErrorCode stating what
/// error occurred.
///
/// \attention While the camera is suspended, it it not possible to use any API which requires
/// access to the camera.
///
/// \sa MMMReader_CameraResume(), MMMReader_IsCameraSuspended()
MMM_READER_LOW_LEVEL_API MMMReaderErrorCode MMMReader_CameraSuspend();

/// \todo Find out what this does
MMM_READER_LOW_LEVEL_API void MMMReader_TurnOffIllum();

/// Waits until a document is present on the \pagereader window and notifies when the
/// previous document is removed.
///
/// \ingroup CameraModule
///
/// \note This API is only relevant if using a \pagereader with the \llapi.
///
/// \param[in] aBlocking Specify whether this operation should be blocking or non-blocking;
/// if \c false, then this API will add a command to the queue to be carried out at a later
/// time and return immediately.
///
/// \param[in] aUseUV Specify whether the ultraviolet (UV) light is used for document
/// detection. If \c false, only infra-red (IR) illumination will be used.
///
/// \param[in] aQA \todo Find out what this does
///
/// \param[out] aDocPosition A pointer to a Box structure which will be updated after the
/// \a aDocFoundCallback is invoked.
///
/// \param[in] aCamSettings A pointer to a CAMERASET structure which should contain the required
/// settings for the \pagereader.
///
/// \param[in] aOCRUserSettings A pointer to a OCRUSERSET structure which should contain the
/// required settings for the \pagereader.
///
/// \param[in] aDocDetectSettings A pointer to a DOCDETECTSETTINGS structure which should contain
/// the required document detection settings for the \pagereader.
///
/// \param[in] aDocFoundCallback A user-defined #MMMReaderEventCallback function to receive
/// notification when a new document has been found. This will trigger a #DOC_ON_WINDOW event.
///
/// \param[in] aDocRemovedCallback A user-defined #MMMReaderEventCallback function to receive
/// notification when a document has been removed. This will trigger a #DOC_REMOVED event.
///
/// \param[in] aValidateDocumentPositionCallback A user-defined
/// #MMMReaderValidateDocPositionCallback to receive notifications on when a moving or
/// stationary document has been detected. Can be \c NULL.
///
/// \param[in] aParam An optional pointer to some user-defined data. This is passed on to all
/// callbacks that have been provided.
///
/// \param[in] aEvent If \a aBlocking is \c true, pass in a handle to an OS event which will be
/// set once the operation finishes.
///
/// \param[in] aDocPosSettings A pointer to a DOCPOS structure which should contain the
/// required settings for the \pagereader.
///
/// \param[in] aImgSettings A pointer to an IMGSET structure which should contain the required
/// settings for the \pagereader.
///
/// \param[in] aFlashIllum \todo Find out what this does
///
/// \return #NO_ERROR_OCCURRED if successful, otherwise a #MMMReaderErrorCode stating what
/// error occurred.
///
/// \attention Document detection is performed by taking images with the camera and
/// performing processing on those images. Therefore, this API can consume CPU and USB
/// resources.
///
/// \remarks \a aParam can be used to provide some context data back through the callbacks. One
/// example could be the \c this pointer from a C++ class; in this case, because the callback
/// functions must be C-style functions, \a aParam would be able to access elements of its
/// class type.
///
/// \remarks Typically, the required settings structures are taken from #MMMReaderSettings by
/// using MMMReader_LL_LoadSettings().
///
/// \remarks If there is already a document on the \pagereader window, it will not be detected
/// as a new document if it is the same as in a previous call to
/// MMMReader_CameraDetectDocument() or MMMReader_LocateDocument(). When this document is
/// removed, the \a aDocRemoveCallback will be invoked.
///
/// \remarks If supplied, \a aValidateDocumentPositionCallback will be invoked when a moving
/// or stationary document has been detected to allow the host application to override document
/// detection. This callback is simply provided with the raw image data that is used for the
/// document detection. Applications may use this in conjunction with the
/// MMMReader_CameraCheckDetectBoxes() API to check specific regions to determine if they are
/// covered by the document.
///
/// \remarks When a non-moving document has been detected, the \a aDocFoundCallback will be
/// invoked. Once this callback has completed, the \a aDocPosition Box will be updated with
/// the location of the document.
///
/// \sa MMMReader_LocateDocument()
MMM_READER_LOW_LEVEL_API MMMReaderErrorCode MMMReader_CameraDetectDocument(
	bool aBlocking,
	bool aUseUV,
	bool aQA,
	Box *aDocPosition,
	CAMERASET *aCamSettings,
	OCRUSERSET *aOCRUserSettings,
	DOCDETECTSETTINGS *aDocDetectSettings,
	MMMReaderEventCallback aDocFoundCallback,
	MMMReaderEventCallback aDocRemovedCallback,
	MMMReaderValidateDocPositionCallback aValidateDocumentPositionCallback,
	void *aParam,
	HANDLE aEvent,
	DOCPOS *aDocPosSettings,
	IMGSET *aImgSettings,
	int aFlashIllum
);

/// Waits until a document is present on the \pagereader window and notifies when the
/// previous document is removed.
///
/// This Ex version has additional parameters image and locateDocPos.  See param comments for details.
///
/// \ingroup CameraModule
///
/// \note This API is only relevant if using a \pagereader with the \llapi.
///
/// \param[in] aBlocking Specify whether this operation should be blocking or non-blocking;
/// if \c false, then this API will add a command to the queue to be carried out at a later
/// time and return immediately.
///
/// \param[in] aUseUV Specify whether the ultraviolet (UV) light is used for document
/// detection. If \c false, only infra-red (IR) illumination will be used.
///
/// \param[in] aQA \todo Find out what this does
///
/// \param[out] aDocPosition A pointer to a Box structure which will be updated after the
/// \a aDocFoundCallback is invoked.
///
/// \param[in] aCamSettings A pointer to a CAMERASET structure which should contain the required
/// settings for the \pagereader.
///
/// \param[in] aOCRUserSettings A pointer to a OCRUSERSET structure which should contain the
/// required settings for the \pagereader.
///
/// \param[in] aDocDetectSettings A pointer to a DOCDETECTSETTINGS structure which should contain
/// the required document detection settings for the \pagereader.
///
/// \param[in] aDocFoundCallback A user-defined #MMMReaderEventCallback function to receive
/// notification when a new document has been found. This will trigger a #DOC_ON_WINDOW event.
///
/// \param[in] aDocRemovedCallback A user-defined #MMMReaderEventCallback function to receive
/// notification when a document has been removed. This will trigger a #DOC_REMOVED event.
///
/// \param[in] aValidateDocumentPositionCallback A user-defined
/// #MMMReaderValidateDocPositionCallback to receive notifications on when a moving or
/// stationary document has been detected. Can be \c NULL.
///
/// \param[in] aParam An optional pointer to some user-defined data. This is passed on to all
/// callbacks that have been provided.
///
/// \param[in] aEvent If \a aBlocking is \c true, pass in a handle to an OS event which will be
/// set once the operation finishes.
///
/// \param[in] aDocPosSettings A pointer to a DOCPOS structure which should contain the
/// required settings for the \pagereader.
///
/// \param[in] aImgSettings A pointer to an IMGSET structure which should contain the required
/// settings for the \pagereader.
///
/// \param[in] aFlashIllum \todo Find out what this does
///
/// \param[out] image A pointer to the address of a Bitmap to be populated with the image used for location.
/// The caller is responsible for deleting.  Can be used with DocDetectSettings.useIRForDetect = TRUE as it is the 
/// standard IR image to immediately post process instead of taking another IR capture.  useIRForDetect Only for readers that
/// support hardware ambient removal, including AT9000 MK2 and CR5400.
///
/// \param[in] locateDosPos Indicates whether to take a snapshot and locate the document.  Can be used with 
/// DocDetectSettings.useFirstImageCapturedForLocate = TRUE to bypass taking a snapshot here to locate and 
/// the first post processed image call will locate the document instead.  For readers such as the CR5400 that
/// use ActiveReader firmware detection to save time, not for ActiveVideo detection as the snapshot is already taken.
/// Only use if taking and processing an IR image first.
///
/// \param[out] aDarkObjectFound A pointer to a boolean flag which will be true if a dark object is found (cell phone).
///
/// \return #NO_ERROR_OCCURRED if successful, otherwise a #MMMReaderErrorCode stating what
/// error occurred.
///
/// \attention Document detection is performed by taking images with the camera and
/// performing processing on those images. Therefore, this API can consume CPU and USB
/// resources.
///
/// \remarks \a aParam can be used to provide some context data back through the callbacks. One
/// example could be the \c this pointer from a C++ class; in this case, because the callback
/// functions must be C-style functions, \a aParam would be able to access elements of its
/// class type.
///
/// \remarks Typically, the required settings structures are taken from #MMMReaderSettings by
/// using MMMReader_LL_LoadSettings().
///
/// \remarks If there is already a document on the \pagereader window, it will not be detected
/// as a new document if it is the same as in a previous call to
/// MMMReader_CameraDetectDocument() or MMMReader_LocateDocument(). When this document is
/// removed, the \a aDocRemoveCallback will be invoked.
///
/// \remarks If supplied, \a aValidateDocumentPositionCallback will be invoked when a moving
/// or stationary document has been detected to allow the host application to override document
/// detection. This callback is simply provided with the raw image data that is used for the
/// document detection. Applications may use this in conjunction with the
/// MMMReader_CameraCheckDetectBoxes() API to check specific regions to determine if they are
/// covered by the document.
///
/// \remarks When a non-moving document has been detected, the \a aDocFoundCallback will be
/// invoked. Once this callback has completed, the \a aDocPosition Box will be updated with
/// the location of the document.
///
/// \sa MMMReader_LocateDocument()
#ifdef ENABLE_GDIPLUS
MMM_READER_LOW_LEVEL_API MMMReaderErrorCode MMMReader_CameraDetectDocumentEx(
	bool aBlocking,
	bool aUseUV,
	bool aQA,
	Box *aDocPosition,
	CAMERASET *aCamSettings,
	OCRUSERSET *aOCRUserSettings,
	DOCDETECTSETTINGS *aDocDetectSettings,
	MMMReaderEventCallback aDocFoundCallback,
	MMMReaderEventCallback aDocRemovedCallback,
	MMMReaderValidateDocPositionCallback aValidateDocumentPositionCallback,
	void *aParam,
	HANDLE aEvent,
	DOCPOS *aDocPosSettings,
	IMGSET *aImgSettings,
	int aFlashIllum,
	Gdiplus::Bitmap **image,
	bool locateDocPos,
	bool *aDarkObjectFound
);
#endif // ENABLE_GDIPLUS

/// Waits until a document is present on the \pagereader window and notifies when the
/// previous document is removed. This function will support a two sided reader like cr54000
///
/// \ingroup CameraModule
///
/// \note This API is only relevant if using a \pagereader with the \llapi.
///
/// \param[in] aBlocking Specify whether this operation should be blocking or non-blocking;
/// if \c false, then this API will add a command to the queue to be carried out at a later
/// time and return immediately.
///
/// \param[in] aUseUV Specify whether the ultraviolet (UV) light is used for document
/// detection. If \c false, only infra-red (IR) illumination will be used.
///
/// \param[in] aQA \todo Find out what this does
///
/// \param[out] aDocPosition A pointer to a Box structure which will be updated after the
/// \a aDocFoundCallback is invoked.
///
/// \param[out] aDocPositionSide2 A pointer to a Box structure which will be updated after the
/// \a aDocFoundCallback is invoked. Only when the image is split two sides.
///
/// \param[in] aCamSettings A pointer to a CAMERASET structure which should contain the required
/// settings for the \pagereader.
///
/// \param[in] aOCRUserSettings A pointer to a OCRUSERSET structure which should contain the
/// required settings for the \pagereader.
///
/// \param[in] aDocDetectSettings A pointer to a DOCDETECTSETTINGS structure which should contain
/// the required document detection settings for the \pagereader.
///
/// \param[in] aDocFoundCallback A user-defined #MMMReaderEventCallback function to receive
/// notification when a new document has been found. This will trigger a #DOC_ON_WINDOW event.
///
/// \param[in] aDocRemovedCallback A user-defined #MMMReaderEventCallback function to receive
/// notification when a document has been removed. This will trigger a #DOC_REMOVED event.
///
/// \param[in] aValidateDocumentPositionCallback A user-defined
/// #MMMReaderValidateDocPositionCallback to receive notifications on when a moving or
/// stationary document has been detected. Can be \c NULL.
///
/// \param[in] aParam An optional pointer to some user-defined data. This is passed on to all
/// callbacks that have been provided.
///
/// \param[in] aEvent If \a aBlocking is \c true, pass in a handle to an OS event which will be
/// set once the operation finishes.
///
/// \param[in] aDocPosSettings A pointer to a DOCPOS structure which should contain the
/// required settings for the \pagereader.
///
/// \param[in] aDocPosSettingsSide2 A pointer to a DOCPOS structure which should contain the
/// required settings for the \pagereader when the image is split two sides.
///
/// \param[in] aImgSettings A pointer to an IMGSET structure which should contain the required
/// settings for the \pagereader.
///
/// \param[in] aFlashIllum \todo Find out what this does
///
/// \return #NO_ERROR_OCCURRED if successful, otherwise a #MMMReaderErrorCode stating what
/// error occurred.
///
/// \attention Document detection is performed by taking images with the camera and
/// performing processing on those images. Therefore, this API can consume CPU and USB
/// resources.
///
/// \remarks \a aParam can be used to provide some context data back through the callbacks. One
/// example could be the \c this pointer from a C++ class; in this case, because the callback
/// functions must be C-style functions, \a aParam would be able to access elements of its
/// class type.
///
/// \remarks Typically, the required settings structures are taken from #MMMReaderSettings by
/// using MMMReader_LL_LoadSettings().
///
/// \remarks If there is already a document on the \pagereader window, it will not be detected
/// as a new document if it is the same as in a previous call to
/// MMMReader_CameraDetectDocument() or MMMReader_LocateDocument(). When this document is
/// removed, the \a aDocRemoveCallback will be invoked.
///
/// \remarks If supplied, \a aValidateDocumentPositionCallback will be invoked when a moving
/// or stationary document has been detected to allow the host application to override document
/// detection. This callback is simply provided with the raw image data that is used for the
/// document detection. Applications may use this in conjunction with the
/// MMMReader_CameraCheckDetectBoxes() API to check specific regions to determine if they are
/// covered by the document.
///
/// \remarks When a non-moving document has been detected, the \a aDocFoundCallback will be
/// invoked. Once this callback has completed, the \a aDocPosition Box will be updated with
/// the location of the document.
///
/// \sa MMMReader_LocateDocument()
MMM_READER_LOW_LEVEL_API MMMReaderErrorCode MMMReader_CameraDetectDocumentTwoSided(
	bool aBlocking,
	bool aUseUV,
	bool aQA,
	Box *aDocPosition,
	Box *aDocPositionSide2,
	CAMERASET *aCamSettings,
	OCRUSERSET *aOCRUserSettings,
	DOCDETECTSETTINGS *aDocDetectSettings,
	MMMReaderEventCallback aDocFoundCallback,
	MMMReaderEventCallback aDocRemovedCallback,
	MMMReaderValidateDocPositionCallback aValidateDocumentPositionCallback,
	void *aParam,
	HANDLE aEvent,
	DOCPOS *aDocPosSettings,
	DOCPOS *aDocPosSettingsSide2,
	IMGSET *aImgSettings,
	int aFlashIllum
);

/// Waits until a document is present on the \pagereader window and notifies when the
/// previous document is removed. This function will support a two sided reader like cr54000
///
/// This Ex version has additional parameters image and locateDocPos.  See param comments for details.
///
/// \ingroup CameraModule
///
/// \note This API is only relevant if using a \pagereader with the \llapi.
///
/// \param[in] aBlocking Specify whether this operation should be blocking or non-blocking;
/// if \c false, then this API will add a command to the queue to be carried out at a later
/// time and return immediately.
///
/// \param[in] aUseUV Specify whether the ultraviolet (UV) light is used for document
/// detection. If \c false, only infra-red (IR) illumination will be used.
///
/// \param[in] aQA \todo Find out what this does
///
/// \param[out] aDocPosition A pointer to a Box structure which will be updated after the
/// \a aDocFoundCallback is invoked.
///
/// \param[out] aDocPositionSide2 A pointer to a Box structure which will be updated after the
/// \a aDocFoundCallback is invoked. Only when the image is split two sides.
///
/// \param[in] aCamSettings A pointer to a CAMERASET structure which should contain the required
/// settings for the \pagereader.
///
/// \param[in] aOCRUserSettings A pointer to a OCRUSERSET structure which should contain the
/// required settings for the \pagereader.
///
/// \param[in] aDocDetectSettings A pointer to a DOCDETECTSETTINGS structure which should contain
/// the required document detection settings for the \pagereader.
///
/// \param[in] aDocFoundCallback A user-defined #MMMReaderEventCallback function to receive
/// notification when a new document has been found. This will trigger a #DOC_ON_WINDOW event.
///
/// \param[in] aDocRemovedCallback A user-defined #MMMReaderEventCallback function to receive
/// notification when a document has been removed. This will trigger a #DOC_REMOVED event.
///
/// \param[in] aValidateDocumentPositionCallback A user-defined
/// #MMMReaderValidateDocPositionCallback to receive notifications on when a moving or
/// stationary document has been detected. Can be \c NULL.
///
/// \param[in] aParam An optional pointer to some user-defined data. This is passed on to all
/// callbacks that have been provided.
///
/// \param[in] aEvent If \a aBlocking is \c true, pass in a handle to an OS event which will be
/// set once the operation finishes.
///
/// \param[in] aDocPosSettings A pointer to a DOCPOS structure which should contain the
/// required settings for the \pagereader.
///
/// \param[in] aDocPosSettingsSide2 A pointer to a DOCPOS structure which should contain the
/// required settings for the \pagereader when the image is split two sides.
///
/// \param[in] aImgSettings A pointer to an IMGSET structure which should contain the required
/// settings for the \pagereader.
///
/// \param[in] aFlashIllum \todo Find out what this does
///
/// \param[out] image A pointer to the address of a Bitmap to be populated with the image used for location.
/// The caller is responsible for deleting.  Can be used with DocDetectSettings.useIRForDetect = TRUE as it is the 
/// standard IR image to immediately post process instead of taking another IR capture.  useIRForDetect Only for readers that
/// support hardware ambient removal, including AT9000 MK2 and CR5400.
///
/// \param[in] locateDosPos Indicates whether to take a snapshot and locate the document.  Can be used with 
/// DocDetectSettings.useFirstImageCapturedForLocate = TRUE to bypass taking a snapshot here to locate and 
/// the first post processed image call will locate the document instead.  For readers such as the CR5400 that
/// use ActiveReader firmware detection to save time, not for ActiveVideo detection as the snapshot is already taken.
/// Only use if taking and processing an IR image first.
///
/// \param[in] insertDoc Indicates whether to insert the document in the reader if in ready position.  Only applies to CR5400 at this time.
///
/// \param[out] aDarkObjectFound A pointer to a boolean flag which will be true if a dark object is found (cell phone).
///
/// \return #NO_ERROR_OCCURRED if successful, otherwise a #MMMReaderErrorCode stating what
/// error occurred.
///
/// \attention Document detection is performed by taking images with the camera and
/// performing processing on those images. Therefore, this API can consume CPU and USB
/// resources.
///
/// \remarks \a aParam can be used to provide some context data back through the callbacks. One
/// example could be the \c this pointer from a C++ class; in this case, because the callback
/// functions must be C-style functions, \a aParam would be able to access elements of its
/// class type.
///
/// \remarks Typically, the required settings structures are taken from #MMMReaderSettings by
/// using MMMReader_LL_LoadSettings().
///
/// \remarks If there is already a document on the \pagereader window, it will not be detected
/// as a new document if it is the same as in a previous call to
/// MMMReader_CameraDetectDocument() or MMMReader_LocateDocument(). When this document is
/// removed, the \a aDocRemoveCallback will be invoked.
///
/// \remarks If supplied, \a aValidateDocumentPositionCallback will be invoked when a moving
/// or stationary document has been detected to allow the host application to override document
/// detection. This callback is simply provided with the raw image data that is used for the
/// document detection. Applications may use this in conjunction with the
/// MMMReader_CameraCheckDetectBoxes() API to check specific regions to determine if they are
/// covered by the document.
///
/// \remarks When a non-moving document has been detected, the \a aDocFoundCallback will be
/// invoked. Once this callback has completed, the \a aDocPosition Box will be updated with
/// the location of the document.
///
/// \sa MMMReader_LocateDocument()
#ifdef ENABLE_GDIPLUS
MMM_READER_LOW_LEVEL_API MMMReaderErrorCode MMMReader_CameraDetectDocumentTwoSidedEx(
	bool aBlocking,
	bool aUseUV,
	bool aQA,
	Box *aDocPosition,
	Box *aDocPositionSide2,
	CAMERASET *aCamSettings,
	OCRUSERSET *aOCRUserSettings,
	DOCDETECTSETTINGS *aDocDetectSettings,
	MMMReaderEventCallback aDocFoundCallback,
	MMMReaderEventCallback aDocRemovedCallback,
	MMMReaderValidateDocPositionCallback aValidateDocumentPositionCallback,
	void *aParam,
	HANDLE aEvent,
	DOCPOS *aDocPosSettings,
	DOCPOS *aDocPosSettingsSide2,
	IMGSET *aImgSettings,
	int aFlashIllum,
	Gdiplus::Bitmap **image,
	bool locateDocPos,
	bool insertDoc,
	bool *darkObjectFound
);
#endif // ENABLE_GDIPLUS

/// Performs a single check for a document on the \pagereader window, determining whether
/// one is present and if so at which position.
///
/// \ingroup CameraModule
///
/// \note This API is only relevant if using a \pagereader with the \llapi.
///
/// \param[in] aUseUV Specify whether the ultraviolet (UV) light is used for document
/// detection. If \c false, only infra-red (IR) illumination will be used.
///
/// \param[in] aQA \todo Find out what this does
///
/// \param[out] aDocPosition A pointer to a Box structure which will be updated after a
/// document has been located.
///
/// \param[out] aDocFound A pointer to a boolean flag which will be updated when a document
/// is found.
///
/// \param[in] aCamSettings A pointer to a CAMERASET structure which should contain the required
/// settings for the \pagereader.
///
/// \param[in] prOCRUserSettings A pointer to a OCRUSERSET structure which should contain the
/// required settings for the \pagereader.
///
/// \param[in] aDocDetectSettings A pointer to a DOCDETECTSETTINGS structure which should contain
/// the required document detection settings for the \pagereader.
///
/// \param[in] aBlocking Specify whether this operation should be blocking or non-blocking;
/// if \c false, then this API will add a command to the queue to be carried out at a later
/// time and return immediately.
///
/// \param[in] aEvent If \a aBlocking is \c true, pass in a handle to an OS event which will be
/// set once the operation finishes.
///
/// \param[in] aDocPosSettings A pointer to a DOCPOS structure which should contain the
/// required settings for the \pagereader.
///
/// \param[in] aImgSettings A pointer to an IMGSET structure which should contain the required
/// settings for the \pagereader.
///
/// \return #NO_ERROR_OCCURRED if successful, otherwise a #MMMReaderErrorCode stating what
/// error occurred.
///
/// \remarks This API is similar to MMMReader_CameraDetectDocument() and uses the same settings
/// for determining whether the document is present. The only difference is that this API
/// does not use callback functions to notify when a document has been found, it will update
/// the \a aDocFound boolean flag instead.
///
/// \remarks This API is often called with \a aBlocking set to \c true, as typically the reason
/// for calling this API is to obtain the value of \a aDocFound and \a aDocPosition.
///
/// \sa MMMReader_CameraDetectDocument()
MMM_READER_LOW_LEVEL_API MMMReaderErrorCode MMMReader_LocateDocument(
	bool aUseUV,
	bool aQA,
	Box *aDocPosition,
	bool *aDocFound,
	CAMERASET *aCamSettings,
	OCRUSERSET *prOCRUserSettings,
	DOCDETECTSETTINGS *aDocDetectSettings,
	bool aBlocking,
	HANDLE aEvent,
	DOCPOS *aDocPosSettings,
	ImgSettings *aImgSettings
);

/// Performs a single check for a document on the \pagereader window, determining whether
/// one is present and if so at which position.
///
/// This Ex version has additional parameters image and locateDocPos.  See param comments for details.
///
/// \ingroup CameraModule
///
/// \note This API is only relevant if using a \pagereader with the \llapi.
///
/// \param[in] aUseUV Specify whether the ultraviolet (UV) light is used for document
/// detection. If \c false, only infra-red (IR) illumination will be used.
///
/// \param[in] aQA \todo Find out what this does
///
/// \param[out] aDocPosition A pointer to a Box structure which will be updated after a
/// document has been located.
///
/// \param[out] aDocFound A pointer to a boolean flag which will be updated when a document
/// is found.
///
/// \param[in] aCamSettings A pointer to a CAMERASET structure which should contain the required
/// settings for the \pagereader.
///
/// \param[in] prOCRUserSettings A pointer to a OCRUSERSET structure which should contain the
/// required settings for the \pagereader.
///
/// \param[in] aDocDetectSettings A pointer to a DOCDETECTSETTINGS structure which should contain
/// the required document detection settings for the \pagereader.
///
/// \param[in] aBlocking Specify whether this operation should be blocking or non-blocking;
/// if \c false, then this API will add a command to the queue to be carried out at a later
/// time and return immediately.
///
/// \param[in] aEvent If \a aBlocking is \c true, pass in a handle to an OS event which will be
/// set once the operation finishes.
///
/// \param[in] aDocPosSettings A pointer to a DOCPOS structure which should contain the
/// required settings for the \pagereader.
///
/// \param[in] aImgSettings A pointer to an IMGSET structure which should contain the required
/// settings for the \pagereader.
///
/// \param[out] image A pointer to the address of a Bitmap to be populated with the image used for location.
/// The caller is responsible for deleting.  Can be used with DocDetectSettings.useIRForDetect = TRUE as it is the 
/// standard IR image to immediately post process instead of taking another IR capture.  useIRForDetect Only for readers that
/// support hardware ambient removal, including AT9000 MK2 and CR5400.
///
/// \param[in] locateDosPos Indicates whether to take a snapshot and locate the document.  Can be used with 
/// DocDetectSettings.useFirstImageCapturedForLocate = TRUE to bypass taking a snapshot here to locate and 
/// the first post processed image call will locate the document instead.  For readers such as the CR5400 that
/// use ActiveReader firmware detection to save time, not for ActiveVideo detection as the snapshot is already taken.
/// Only use if taking and processing an IR image first.
///
/// \param[out] aDarkObjectFound A pointer to a boolean flag which will be true if a dark object is found (cell phone).
///
/// \return #NO_ERROR_OCCURRED if successful, otherwise a #MMMReaderErrorCode stating what
/// error occurred.
///
/// \remarks This API is similar to MMMReader_CameraDetectDocument() and uses the same settings
/// for determining whether the document is present. The only difference is that this API
/// does not use callback functions to notify when a document has been found, it will update
/// the \a aDocFound boolean flag instead.
///
/// \remarks This API is often called with \a aBlocking set to \c true, as typically the reason
/// for calling this API is to obtain the value of \a aDocFound and \a aDocPosition.
///
/// \sa MMMReader_CameraDetectDocument()
#ifdef ENABLE_GDIPLUS
MMM_READER_LOW_LEVEL_API MMMReaderErrorCode MMMReader_LocateDocumentEx(
	bool aUseUV,
	bool aQA,
	Box *aDocPosition,
	bool *aDocFound,
	CAMERASET *aCamSettings,
	OCRUSERSET *prOCRUserSettings,
	DOCDETECTSETTINGS *aDocDetectSettings,
	bool aBlocking,
	HANDLE aEvent,
	DOCPOS *aDocPosSettings,
	ImgSettings *aImgSettings,
	Gdiplus::Bitmap **image,
	bool locateDocPos,
	bool *aDarkObjectFound
);
#endif // ENABLE_GDIPLUS

/// Performs a single check for a document on the \pagereader window, determining whether
/// one is present and if so at which position.
///
/// \ingroup CameraModule
///
/// \note This API is only relevant if using a \pagereader with the \llapi.
///
/// \param[in] aUseUV Specify whether the ultraviolet (UV) light is used for document
/// detection. If \c false, only infra-red (IR) illumination will be used.
///
/// \param[in] aQA \todo Find out what this does
///
/// \param[out] aDocPosition A pointer to a Box structure which will be updated after a
/// document has been located.
///
/// \param[out] aDocPositionSide2 A pointer to a Box structure which will be updated after a
/// document side2 has been located.
///
/// \param[out] aDocFound A pointer to a boolean flag which will be updated when a document
/// is found.
///
/// \param[in] aCamSettings A pointer to a CAMERASET structure which should contain the required
/// settings for the \pagereader.
///
/// \param[in] prOCRUserSettings A pointer to a OCRUSERSET structure which should contain the
/// required settings for the \pagereader.
///
/// \param[in] aDocDetectSettings A pointer to a DOCDETECTSETTINGS structure which should contain
/// the required document detection settings for the \pagereader.
///
/// \param[in] aBlocking Specify whether this operation should be blocking or non-blocking;
/// if \c false, then this API will add a command to the queue to be carried out at a later
/// time and return immediately.
///
/// \param[in] aEvent If \a aBlocking is \c true, pass in a handle to an OS event which will be
/// set once the operation finishes.
///
/// \param[in] aDocPosSettings A pointer to a DOCPOS structure which should contain the
/// required settings for the \pagereader.
///
/// \param[in] aDocPosSettingsSide2 A pointer to a DOCPOS structure which should contain the
/// required settings for the \pagereader when the image is split two sides.
///
/// \param[in] aImgSettings A pointer to an IMGSET structure which should contain the required
/// settings for the \pagereader.
///
/// \return #NO_ERROR_OCCURRED if successful, otherwise a #MMMReaderErrorCode stating what
/// error occurred.
///
/// \remarks This API is similar to MMMReader_CameraDetectDocument() and uses the same settings
/// for determining whether the document is present. The only difference is that this API
/// does not use callback functions to notify when a document has been found, it will update
/// the \a aDocFound boolean flag instead.
///
/// \remarks This API is often called with \a aBlocking set to \c true, as typically the reason
/// for calling this API is to obtain the value of \a aDocFound and \a aDocPosition.
///
/// \sa MMMReader_CameraDetectDocument()
MMM_READER_LOW_LEVEL_API MMMReaderErrorCode MMMReader_LocateDocumentTwoSided(
	bool aUseUV,
	bool aQA,
	Box *aDocPosition,
	Box *aDocPositionSide2,
	bool *aDocFound,
	CAMERASET *aCamSettings,
	OCRUSERSET *prOCRUserSettings,
	DOCDETECTSETTINGS *aDocDetectSettings,
	bool aBlocking,
	HANDLE aEvent,
	DOCPOS *aDocPosSettings,
	DOCPOS *aDocPosSettingsSide2,
	ImgSettings *aImgSettings
);

/// Performs a single check for a document on the \pagereader window, determining whether
/// one is present and if so at which position.
///
/// This Ex version has additional parameters image and locateDocPos.  See param comments for details.
///
/// \ingroup CameraModule
///
/// \note This API is only relevant if using a \pagereader with the \llapi.
///
/// \param[in] aUseUV Specify whether the ultraviolet (UV) light is used for document
/// detection. If \c false, only infra-red (IR) illumination will be used.
///
/// \param[in] aQA \todo Find out what this does
///
/// \param[out] aDocPosition A pointer to a Box structure which will be updated after a
/// document has been located.
///
/// \param[out] aDocPositionSide2 A pointer to a Box structure which will be updated after a
/// document side2 has been located.
///
/// \param[out] aDocFound A pointer to a boolean flag which will be updated when a document
/// is found.
///
/// \param[in] aCamSettings A pointer to a CAMERASET structure which should contain the required
/// settings for the \pagereader.
///
/// \param[in] prOCRUserSettings A pointer to a OCRUSERSET structure which should contain the
/// required settings for the \pagereader.
///
/// \param[in] aDocDetectSettings A pointer to a DOCDETECTSETTINGS structure which should contain
/// the required document detection settings for the \pagereader.
///
/// \param[in] aBlocking Specify whether this operation should be blocking or non-blocking;
/// if \c false, then this API will add a command to the queue to be carried out at a later
/// time and return immediately.
///
/// \param[in] aEvent If \a aBlocking is \c true, pass in a handle to an OS event which will be
/// set once the operation finishes.
///
/// \param[in] aDocPosSettings A pointer to a DOCPOS structure which should contain the
/// required settings for the \pagereader.
///
/// \param[in] aDocPosSettingsSide2 A pointer to a DOCPOS structure which should contain the
/// required settings for the \pagereader when the image is split two sides.
///
/// \param[in] aImgSettings A pointer to an IMGSET structure which should contain the required
/// settings for the \pagereader.
///
/// \param[out] image A pointer to the address of a Bitmap to be populated with the image used for location.
/// The caller is responsible for deleting.  Can be used with DocDetectSettings.useIRForDetect = TRUE as it is the 
/// standard IR image to immediately post process instead of taking another IR capture.  useIRForDetect Only for readers that
/// support hardware ambient removal, including AT9000 MK2 and CR5400.
///
/// \param[in] locateDosPos Indicates whether to take a snapshot and locate the document.  Can be used with 
/// DocDetectSettings.useFirstImageCapturedForLocate = TRUE to bypass taking a snapshot here to locate and 
/// the first post processed image call will locate the document instead.  For readers such as the CR5400 that
/// use ActiveReader firmware detection to save time, not for ActiveVideo detection as the snapshot is already taken.
/// Only use if taking and processing an IR image first.
///
/// \param[in] insertDoc Indicates whether to insert the document in the reader if in ready position.  Only applies to CR5400 at this time.
///
/// \param[out] aDarkObjectFound A pointer to a boolean flag which will be true if a dark object is found (cell phone).
///
/// \return #NO_ERROR_OCCURRED if successful, otherwise a #MMMReaderErrorCode stating what
/// error occurred.
///
/// \remarks This API is similar to MMMReader_CameraDetectDocument() and uses the same settings
/// for determining whether the document is present. The only difference is that this API
/// does not use callback functions to notify when a document has been found, it will update
/// the \a aDocFound boolean flag instead.
///
/// \remarks This API is often called with \a aBlocking set to \c true, as typically the reason
/// for calling this API is to obtain the value of \a aDocFound and \a aDocPosition.
///
/// \sa MMMReader_CameraDetectDocument()
#ifdef ENABLE_GDIPLUS
MMM_READER_LOW_LEVEL_API MMMReaderErrorCode MMMReader_LocateDocumentTwoSidedEx(
	bool aUseUV,
	bool aQA,
	Box *aDocPosition,
	Box *aDocPositionSide2,
	bool *aDocFound,
	CAMERASET *aCamSettings,
	OCRUSERSET *prOCRUserSettings,
	DOCDETECTSETTINGS *aDocDetectSettings,
	bool aBlocking,
	HANDLE aEvent,
	DOCPOS *aDocPosSettings,
	DOCPOS *aDocPosSettingsSide2,
	ImgSettings *aImgSettings,
	Gdiplus::Bitmap **image,
	bool locateDocPos,
	bool insertDoc,
	bool *aDarkObjectFound
);
#endif // ENABLE_GDIPLUS

/// Takes an individual snapshot with the \pagereader camera.
///
/// \ingroup CameraModule
///
/// \note This API is only relevant if using a \pagereader with the \llapi.
///
/// \param[in] aBlocking Specify whether this operation should be blocking or non-blocking;
/// if \c false, then this API will add a command to the queue to be carried out at a later
/// time and return immediately.
///
/// \param[in] aLight A bitwise flag specifying which lights to use when taking the image, e.g.
/// #LIGHT_IRFULL.
///
/// \param[in] aLightingBanks A bitwise flag specifying which lighting banks to use when
/// taking the image, e.g. #LIGHT_BANK_ALL.
///
/// \param[in] aBrightnessSettings A pointer to a BRIGHTNESSSETTINGS structure which should
/// contain the required settings for the \pagereader.
///
/// \param[in] aSettings A pointer to a CAMERASET structure which should contain the required
/// settings for the \pagereader.
///
/// \param[in] aDataCallback A user-defined #MMMReaderDataCallback function which will receive
/// the image as a GDI+ Bitmap.
///
/// \param[in] aParam An optional pointer to some user-defined data. This is passed on to the
/// callback provided.
///
/// \param[in] aSequenceNum A user-defined value that identifies what data is coming back
/// through the \a aDataCallback.
///
/// \param[in] aEvent If \a aBlocking is \c true, pass in a handle to an OS event which will be
/// set once the operation finishes.
///
/// \return #NO_ERROR_OCCURRED if successful, otherwise a #MMMReaderErrorCode stating what
/// error occurred.
///
/// \remarks \a aParam can be used to provide some context data back through the callbacks. One
/// example could be the \c this pointer from a C++ class; in this case, because the callback
/// functions must be C-style functions, \a aParam would be able to access elements of its
/// class type.
///
/// \remarks \a aSequenceNum is a user-defined value which is passed straight into
/// \a aDataCallback. This parameter is provided so that it is possible to have one data
/// callback defined for multiple data items; the defined function can then filter the
/// \a aSequenceNum as appropriate.
///
/// \remarks Typically, the required settings structures are taken from #MMMReaderSettings by
/// using MMMReader_LL_LoadSettings(). However, in some applications it may be desirable
/// to adjust the settings to get optimal images.
///
/// \remarks If \a aLight is #LIGHT_UV and the ultraviolet (UV) light has not yet been
/// enabled, the image will not be captured until the UV warmup time has been reached, as
/// specified by the values in the CAMERASET structure.
///
/// \remarks \a aLightBanks indicates the lighting banks that should be used. Typically this
/// should be #LIGHT_BANK_ALL, however when attempting to suppress or highlight Optically
/// Variable Devices (OVDs), using a different combination of light banks may be appropriate.
/// This value is a bitmask indicating whether the left/right/front/back light banks should be
/// illuminated.
///
/// \remarks The data returned from the camera is not generally in a format that is directly
/// usable for most purposes. Generally the image must be passed through
/// MMMReader_ImagePostProcessImage() prior to using it. This is split into two separate APIs
/// to allow the camera to continue taking images while image post-processing is performed.
///
/// \sa MMMReader_ImagePostProcessImage()
MMM_READER_LOW_LEVEL_API MMMReaderErrorCode MMMReader_CameraTakeSnapshot(
	bool aBlocking,
	int aLight,
	int aLightingBanks,
	BRIGHTNESSSETTINGS *aBrightnessSettings,
	CAMERASET *aSettings,
	MMMReaderDataCallback aDataCallback,
	void *aParam,
	int aSequenceNum,
	HANDLE aEvent
);

/// Informs the low level \pagereader camera all document processing has been completed and any post actions can be done.
///
/// \ingroup CameraModule
///
/// \note This API is only relevant if using a \pagereader with the \llapi.
///
/// \return \c no return information.
///
MMM_READER_LOW_LEVEL_API void MMMReader_DocumentProcessingComplete();

/// Determines whether the \pagereader camera has been initialised.
///
/// \ingroup CameraModule
///
/// \note This API is only relevant if using a \pagereader with the \llapi.
///
/// \return \c true if MMMReader_CameraInitialise() has been called, or \c false otherwise.
///
/// \sa MMMReader_CameraInitialise()
MMM_READER_LOW_LEVEL_API bool MMMReader_IsCameraInitialised();

/// Determines whether the \pagereader camera is in a suspended state.
///
/// \ingroup CameraModule
///
/// \note This API is only relevant if using a \pagereader with the \llapi.
///
/// \return \c true if the camera is suspended, or \c false otherwise.
///
/// \sa MMMReader_CameraSuspend()
MMM_READER_LOW_LEVEL_API bool MMMReader_IsCameraSuspended();

/// Determines whether any operation is currently active in the camera module.
///
/// \ingroup CameraModule
///
/// \note This API is only relevant if using a \pagereader with the \llapi.
///
/// \return \c true if any non-blocking operation is currently being executed or is queued, or
/// \c false otherwise.
///
/// \sa MMMReader_CameraCancelQueue()
MMM_READER_LOW_LEVEL_API bool MMMReader_IsCameraActive();

/// Cancels any pending camera operations that have been queued by non-blocking API calls
/// within the camera module, and attempts to cancel any currently executing operation.
///
/// \ingroup CameraModule
///
/// \note This API is only relevant if using a \pagereader with the \llapi.
///
/// \return #NO_ERROR_OCCURRED if successful, otherwise a #MMMReaderErrorCode stating what
/// error occurred.
///
/// \remarks Any operations that are currently in progress may complete or not be aborted
/// immediately.
///
/// \remarks This API will return as soon as it has indicated that the operations are to be
/// cancelled, not when any currently in-progress operations are complete. Therefore, if the
/// host application needs to wait for completion, the MMMReader_IsCameraActive() API
/// can be used to check for completion.
///
/// \sa MMMReader_IsCameraActive()
MMM_READER_LOW_LEVEL_API MMMReaderErrorCode MMMReader_CameraCancelQueue();

/// Enables or disables the ultraviolet (UV) light in the \pagereader camera.
///
/// \ingroup CameraModule
///
/// \note This API is only relevant if using a \pagereader with the \llapi.
///
/// \param[in] aBlocking Specify whether this operation should be blocking or non-blocking;
/// if \c false, then this API will add a command to the queue to be carried out at a later
/// time and return immediately.
///
/// \param[in] aEnabled Specify whether to enable the UV light.
///
/// \param[in] aEvent If \a aBlocking is \c true, pass in a handle to an OS event which will be
/// set once the operation finishes.
///
/// \return #NO_ERROR_OCCURRED if successful, otherwise a #MMMReaderErrorCode stating what
/// error occurred.
///
/// \remarks Since any images currently being taken are affected by illumination, this
/// operation will be added to the queue (if \a aBlocking is \c false) and the illumination
/// will be changed after all previously queued operations are complete.
///
/// \sa MMMReader_CameraTakeSnapshot()
MMM_READER_LOW_LEVEL_API MMMReaderErrorCode MMMReader_EnableUV(
	bool aBlocking,
	bool aEnabled,
	HANDLE aEvent
);

/// Determines how long it will take for the ultraviolet (UV) lamp to warm-up to produce
/// effective UV images.
///
/// \ingroup CameraModule
///
/// \note This API is only relevant if using certain \pagereaders with the \llapi.
///
/// \param[in] aSettings A pointer to a CAMERASET structure which should contain the required
/// settings for the \pagereader.
///
/// \param[out] aWarmupTime A pointer to an integer which will be updated to contain the
/// warm-up time, in milliseconds (ms).
///
/// \return #NO_ERROR_OCCURRED if successful, otherwise a #MMMReaderErrorCode stating what
/// error occurred.
///
/// \note Some \pagereaders use UV tubes to produce UV images, whilst others use UV LEDs which
/// operate immediately. Therefore this API only applies to some \pagereaders. Consult \ssd
/// for more details.
///
/// \remarks The warm-up time is specified by the values in the \a aSettings structure.
/// Typically, the required settings structures are taken from #MMMReaderSettings by
/// using MMMReader_LL_LoadSettings().
///
/// \sa MMMReader_GetUVEnabledTime()
MMM_READER_LOW_LEVEL_API MMMReaderErrorCode MMMReader_GetUVWarmupTime(
	CAMERASET *aSettings,
	int *aWarmupTime
);

/// Determines how long the ultraviolet (UV) lamp has currently been enabled for. This may
/// have been via MMMReader_EnableUV() or MMMReader_CameraDetectDocument().
///
/// \ingroup CameraModule
///
/// \note This API is only relevant if using certain \pagereaders with the \llapi.
///
/// \return The time the UV lamp has been enabled for, in milliseconds (ms).
///
/// \note Some \pagereaders use UV tubes to produce UV images, whilst others use UV LEDs which
/// operate immediately. Therefore this API only applies to some \pagereaders. Consult \ssd
/// for more details.
///
/// \sa MMMReader_GetUVWarmupTime()
MMM_READER_LOW_LEVEL_API int MMMReader_GetUVEnabledTime();

/// Checks a number of boxes within a supplied document image and indicates whether each box
/// has an average value greater than the document detection threshold.
///
/// \ingroup CameraModule
///
/// \note This API is only relevant if using a \pagereader with the \llapi.
///
/// \param[in] aCamSettings A pointer to a CAMERASET structure which should contain the required
/// settings for the \pagereader.
///
/// \param[in] docDetSettings A pointer to a DOCDETECTSETTINGS structure which should contain
/// the required settings for the \pagereader.
///
/// \param[in] aImgData A pointer to the raw image data to inspect.
///
/// \param[in] aBoxCount The number of boxes in the \a aBoxes array.
///
/// \param[in] aBoxes An array of Box structures which specify the regions to inspect in
/// \a aImgData.
///
/// \param[out] aResults A pointer to an array of boolean flags which will provide the results
/// for each Box in \a aBoxes.
///
/// \return #NO_ERROR_OCCURRED if successful, otherwise a #MMMReaderErrorCode stating what
/// error occurred.
///
/// \remarks Typically, the required settings structures are taken from #MMMReaderSettings by
/// using MMMReader_LL_LoadSettings().
///
/// \remarks Each of the \a aBoxes must have a width and height greater than zero. Also, the
/// positions of the \a aBoxes must not exceed the dimensions of the camera resolution.
///
/// \remarks A typical use of this API is within the #MMMReaderValidateDocPositionCallback
/// supplied to MMMReader_CameraDetectDocument(), to provide feedback to the user of the
/// position of the document during document detection.
MMM_READER_LOW_LEVEL_API MMMReaderErrorCode MMMReader_CameraCheckDetectBoxes(
	CAMERASET *aCamSettings,
	DOCDETECTSETTINGS * docDetSettings,
	unsigned char *aImgData,
	int aBoxCount,
	Box aBoxes[],
	bool aResults[]
);

/// \endcond
// PAGESDK

////////// Image Processing APIs

/// \cond PAGESDK

/// Initialises the \sdk image processing module and allocates resources required for image
/// processing.
///
/// \ingroup ImageProcModule
///
/// \note This API is only relevant if using a \pagereader with the \llapi.
///
/// \param[in] aCamSettings A pointer to a CAMERASET structure which should contain the required
/// settings for the \pagereader.
///
/// \param[in] aReaderDir A null-terminated string containing the directory path to the
/// reference images. See the remarks section below for more details.
///
/// \return #NO_ERROR_OCCURRED if successful, otherwise a #MMMReaderErrorCode stating what
/// error occurred.
///
/// \attention The MMMReader_ImageProcessingShutdown() API must be called when finished with
/// the image processing module to free all resources allocated.
///
/// \remarks \a aCamSettings would typically be retrieved from the MMMReader_LL_LoadSettings()
/// or MMMReader_LL_LoadCameraImageSettings() APIs. A number of the image processing APIs
/// depend upon the resolution and settings of the \pagereader camera, hence why the
/// CAMERASET structure is required.
///
/// \remarks The value to pass into \a aReaderDir depends on which \pagereader is being used:
///		- For \rte8000 and \rte8000hs readers, reference images are stored in the Mass Storage
///		Device (MSD); the directory path for this can be retrieved from
///		#MMMReaderSettings::puReaderDir or the MMMReader_LL_GetReaderDir() API.
///		- For \at9000 and \kioskreader readers, reference images are stored directly in the
///		internal camera memory; these cannot be accessed, so \a aReaderDir can be \c NULL in
///		this case.
///
/// \remarks To cover both cases, it is safe to use #MMMReaderSettings::puReaderDir or the
/// MMMReader_LL_GetReaderDir() API as the value for \a aReaderDir; the \sdk will determine
/// how to proceed.
///
/// \sa MMMReader_ImageProcessingShutdown(), MMMReader_IsImageProcessingInitialised()
MMM_READER_LOW_LEVEL_API MMMReaderErrorCode MMMReader_ImageProcessingInitialise(
	CAMERASET *aCamSettings,
	RTCHAR *aReaderDir,
	DIRTDETECTSET *aDirtDetectSettings
);

/// Shuts down the \sdk image processing module and frees all related resources.
///
/// \ingroup ImageProcModule
///
/// \note This API is only relevant if using a \pagereader with the \llapi.
///
/// \return #NO_ERROR_OCCURRED if successful, otherwise a #MMMReaderErrorCode stating what
/// error occurred.
///
/// \sa MMMReader_ImageProcessingInitialise(), MMMReader_IsImageProcessingInitialised()
MMM_READER_LOW_LEVEL_API MMMReaderErrorCode MMMReader_ImageProcessingShutdown();

/// Determines whether the \sdk image processing module has been initialised.
///
/// \ingroup ImageProcModule
///
/// \note This API is only relevant if using a \pagereader with the \llapi.
///
/// \return \c true if MMMReader_ImageProcessingInitialise() has been called, or \c false
/// otherwise.
///
/// \sa MMMReader_ImageProcessingInitialise()
MMM_READER_LOW_LEVEL_API bool MMMReader_IsImageProcessingInitialised();

/// Determines whether any operation is currently active in the image processing module.
///
/// \ingroup ImageProcModule
///
/// \note This API is only relevant if using a \pagereader with the \llapi.
///
/// \return \c true if any non-blocking operation is currently being executed or is queued, or
/// \c false otherwise.
///
/// \sa MMMReader_ImageProcessingCancelQueue()
MMM_READER_LOW_LEVEL_API bool MMMReader_IsImageProcessingActive();

/// Cancels any pending image processing operations that have been queued by non-blocking API
/// calls within the image processing module, and attempts to cancel any currently executing
/// operation.
///
/// \ingroup ImageProcModule
///
/// \note This API is only relevant if using a \pagereader with the \llapi.
///
/// \return #NO_ERROR_OCCURRED if successful, otherwise a #MMMReaderErrorCode stating what
/// error occurred.
///
/// \remarks Any operations that are currently in progress may complete or not be aborted
/// immediately.
///
/// \remarks This API will return as soon as it has indicated that the operations are to be
/// cancelled, not when any currently in-progress operations are complete. Therefore, if the
/// host application needs to wait for completion, the MMMReader_IsImageProcessingActive() API
/// can be used to check for completion.
///
/// \sa MMMReader_IsImageProcessingActive()
MMM_READER_LOW_LEVEL_API MMMReaderErrorCode MMMReader_ImageProcessingCancelQueue();

/// Crops an image to display only the codeline region of a document image.
///
/// \ingroup ImageProcModule
///
/// \note This API is only relevant if using a \pagereader with the \llapi.
///
/// \param[in] aInputImage A GDI+ Bitmap object which is the source image to crop.
///
/// \param[out] aOutputImage A pointer to a GDI+ Bitmap object which will be updated to
/// point to the cropped image result.
///
/// \return #NO_ERROR_OCCURRED if successful, otherwise a #MMMReaderErrorCode stating what
/// error occurred.
///
/// \attention The cropped result will be cached by the \sdk and a pointer to the resulting
/// image will be returned via \a aOutputImage. Because of this, the host application should
/// not manually free any memory related to \a aOutputImage, the \sdk will automatically
/// handle this task.
///
/// \remarks \a aInputImage is assumed to be an image that has been cropped to the document
/// based on MMMReader_ImagePostProcessImage().
///
/// \sa MMMReader_ImagePostProcessImage(), MMMReader_ImageReadCodeline()
#ifdef ENABLE_GDIPLUS
MMM_READER_LOW_LEVEL_API MMMReaderErrorCode MMMReader_ImageCropToCodeline(
	Gdiplus::Bitmap *aInputImage,
	Gdiplus::Bitmap **aOutputImage
);
#endif // ENABLE_GDIPLUS

/// Crops an image to display only the codeline region of a document image.
/// MMMReader_ImageCropToCodelineEx or MMMReader_ImageCropToCodelineTwoSidedEx
/// can be used to get the aCodelineLocation for cropping.
///
/// \ingroup ImageProcModule
///
/// \note This API is only relevant if using a \pagereader with the \llapi.
///
/// \param[in] aInputImage A GDI+ Bitmap object which is the source image to crop.
///
/// \param[out] aOutputImage A pointer to a GDI+ Bitmap object which will be updated to
/// point to the cropped image result.
///
/// \param[in] aCodelineLocation location of codeline area to crop to or NULL if unknown.
///
/// \return #NO_ERROR_OCCURRED if successful, otherwise a #MMMReaderErrorCode stating what
/// error occurred.
///
/// \attention The cropped result will be cached by the \sdk and a pointer to the resulting
/// image will be returned via \a aOutputImage. Because of this, the host application should
/// not manually free any memory related to \a aOutputImage, the \sdk will automatically
/// handle this task.
///
/// \remarks \a aInputImage is assumed to be an image that has been cropped to the document
/// based on MMMReader_ImagePostProcessImage().
/// MMMReader_ImageCropToCodelineEx or MMMReader_ImageCropToCodelineTwoSidedEx
/// can be used to get the aCodelineLocation for cropping.
///
/// \sa MMMReader_ImagePostProcessImage(), MMMReader_ImageReadCodeline()
#ifdef ENABLE_GDIPLUS
MMM_READER_LOW_LEVEL_API MMMReaderErrorCode MMMReader_ImageCropToCodelineEx(
	Gdiplus::Bitmap *aInputImage,
	Gdiplus::Bitmap **aOutputImage,
	Box * aCodelineLocation
);
#endif // ENABLE_GDIPLUS

/// Crops an image to display only the photo region of a document image.
///
/// \ingroup ImageProcModule
///
/// \note This API is only relevant if using a \pagereader with the \llapi.
///
/// \param[in] aInputImage A GDI+ Bitmap object which is the source image to crop.
///
/// \param[out] aOutputImage A pointer to a GDI+ Bitmap object which will be updated to
/// point to the cropped image result.
///
/// \param[in] aSettings A pointer to a PHOTOSET structure which should contain the required
/// settings for the \pagereader.
///
/// \return #NO_ERROR_OCCURRED if successful, otherwise a #MMMReaderErrorCode stating what
/// error occurred.
///
/// \attention The cropped result will be cached by the \sdk and a pointer to the resulting
/// image will be returned via \a aOutputImage. Because of this, the host application should
/// not manually free any memory related to \a aOutputImage, the \sdk will automatically
/// handle this task.
///
/// \remarks \a aSettings should define where the photo is located in the image. Typically,
/// the required settings structures are taken from #MMMReaderSettings by using
/// MMMReader_LL_LoadSettings().
///
/// \remarks \a aInputImage is assumed to be an image that has been cropped to the document
/// based on MMMReader_ImagePostProcessImage().
///
/// \sa MMMReader_ImagePostProcessImage()
#ifdef ENABLE_GDIPLUS
MMM_READER_LOW_LEVEL_API MMMReaderErrorCode MMMReader_ImageCropToPhoto(
	Gdiplus::Bitmap *aInputImage,
	Gdiplus::Bitmap **aOutputImage,
	PHOTOSET *aSettings
);
#endif // ENABLE_GDIPLUS

/// Performs Optical Character Recognition (OCR) on a document image to read the Machine
/// Readable Zone (MRZ) codeline from travel documents.
///
/// \ingroup ImageProcModule
///
/// \note This API is only relevant if using a \pagereader with the \llapi.
///
/// \param[in] aBlocking Specify whether this operation should be blocking or non-blocking;
/// if \c false, then this API will add a command to the queue to be carried out at a later
/// time and return immediately.
///
/// \param[in] aInputImage A GDI+ Bitmap object which is the source image to read from.
///
/// \param[in] aVisibleLight \todo Find out what this does
///
/// \param[in] aOCRSettings A pointer to a OCRSET structure which should contain the
/// required settings for the \pagereader.
///
/// \param[in] aOCRUserSettings A pointer to a OCRUSERSET structure which should contain the
/// required settings for the \pagereader.
///
/// \param[in] aDocPosition \todo Find out what this does
///
/// \param[out] aTooLight \todo Find out what this does
///
/// \param[out] aDocUpsideDown A pointer to a boolean flag which will be updated to state
/// whether the document image was determined to be upside down.
///
/// \param[out] thresholdUsed \todo Find out what this does
///
/// \param[in] aDataCallback A user-defined #MMMReaderDataCallback function which will receive
/// the codeline that was read as a string.
///
/// \param[in] aParam An optional pointer to some user-defined data. This is passed on to the
/// callback provided.
///
/// \param[in] aSequenceNum A user-defined value that identifies what data is coming back
/// through the \a aDataCallback.
///
/// \param[in] aEvent If \a aBlocking is \c true, pass in a handle to an OS event which will be
/// set once the operation finishes.
///
/// \param[in] aImgSettings A pointer to a IMGSET structure which should contain the
/// required settings for the \pagereader.
///
/// \param[in] aDocPosSettings A pointer to a DOCPOS structure which should contain the
/// required settings for the \pagereader.
///
/// \param[in] aDirtPosn A pointer to an integer which obtains the dirt position information
/// if there is dirt on the scanner window
///
/// \return #NO_ERROR_OCCURRED if successful, otherwise a #MMMReaderErrorCode stating what
/// error occurred.
///
/// \remarks \a aParam can be used to provide some context data back through the callbacks. One
/// example could be the \c this pointer from a C++ class; in this case, because the callback
/// functions must be C-style functions, \a aParam would be able to access elements of its
/// class type.
///
/// \remarks \a aSequenceNum is a user-defined value which is passed straight into
/// \a aDataCallback. This parameter is provided so that it is possible to have one data
/// callback defined for multiple data items; the defined function can then filter the
/// \a aSequenceNum as appropriate.
///
/// \remarks Typically, the required settings structures are taken from #MMMReaderSettings by
/// using MMMReader_LL_LoadSettings().
///
/// \sa MMMReader_ImagePerformSecurityCheck()
#ifdef ENABLE_GDIPLUS
MMM_READER_LOW_LEVEL_API MMMReaderErrorCode MMMReader_ImageReadCodeline(
	bool aBlocking,
	Gdiplus::Bitmap *aInputImage,
	bool aVisibleLight,
	OCRSET *aOCRSettings,
	OCRUSERSET *aOCRUserSettings,
	Box *aDocPosition,
	bool *aTooLight,
	bool *aDocUpsideDown,
	int *thresholdUsed,
	MMMReaderDataCallback aDataCallback,
	void *aParam,
	int aSequenceNum,
	HANDLE aEvent,
	IMGSET *aImgSettings,
	DOCPOS *aDocPosSettings,
	int *aDirtPosn
);
#endif // ENABLE_GDIPLUS

/// Performs Optical Character Recognition (OCR) on a document image to read the Machine
/// Readable Zone (MRZ) codeline from travel documents.
///
/// \ingroup ImageProcModule
///
/// \note This API is only relevant if using a \pagereader with the \llapi.
///
/// \param[in] aBlocking Specify whether this operation should be blocking or non-blocking;
/// if \c false, then this API will add a command to the queue to be carried out at a later
/// time and return immediately.
///
/// \param[in] aInputImage A GDI+ Bitmap object which is the source image to read from.
///
/// \param[in] aVisibleLight \todo Find out what this does
///
/// \param[in] aOCRSettings A pointer to a OCRSET structure which should contain the
/// required settings for the \pagereader.
///
/// \param[in] aOCRUserSettings A pointer to a OCRUSERSET structure which should contain the
/// required settings for the \pagereader.
///
/// \param[in] aDocPosition \todo Find out what this does
///
/// \param[out] aTooLight \todo Find out what this does
///
/// \param[out] aDocUpsideDown A pointer to a boolean flag which will be updated to state
/// whether the document image was determined to be upside down.
///
/// \param[out] thresholdUsed \todo Find out what this does
///
/// \param[in] aDataCallback A user-defined #MMMReaderDataCallback function which will receive
/// the codeline that was read as a string.
///
/// \param[in] aParam An optional pointer to some user-defined data. This is passed on to the
/// callback provided.
///
/// \param[in] aSequenceNum A user-defined value that identifies what data is coming back
/// through the \a aDataCallback.
///
/// \param[in] aEvent If \a aBlocking is \c true, pass in a handle to an OS event which will be
/// set once the operation finishes.
///
/// \param[in] aImgSettings A pointer to a IMGSET structure which should contain the
/// required settings for the \pagereader.
///
/// \param[in] aDocPosSettings A pointer to a DOCPOS structure which should contain the
/// required settings for the \pagereader.
///
/// \param[in] aDirtPosn A pointer to an integer which obtains the dirt position information
/// if there is dirt on the scanner window
///
/// \param[out] aCodelineLocation A pointer to a Box which will be updated to the location of
/// the codeline.
///
/// \return #NO_ERROR_OCCURRED if successful, otherwise a #MMMReaderErrorCode stating what
/// error occurred.
///
/// \remarks \a aParam can be used to provide some context data back through the callbacks. One
/// example could be the \c this pointer from a C++ class; in this case, because the callback
/// functions must be C-style functions, \a aParam would be able to access elements of its
/// class type.
///
/// \remarks \a aSequenceNum is a user-defined value which is passed straight into
/// \a aDataCallback. This parameter is provided so that it is possible to have one data
/// callback defined for multiple data items; the defined function can then filter the
/// \a aSequenceNum as appropriate.
///
/// \remarks Typically, the required settings structures are taken from #MMMReaderSettings by
/// using MMMReader_LL_LoadSettings().
///
/// \sa MMMReader_ImagePerformSecurityCheck()
#ifdef ENABLE_GDIPLUS
MMM_READER_LOW_LEVEL_API MMMReaderErrorCode MMMReader_ImageReadCodelineEx(
	bool aBlocking,
	Gdiplus::Bitmap *aInputImage,
	bool aVisibleLight,
	OCRSET *aOCRSettings,
	OCRUSERSET *aOCRUserSettings,
	Box *aDocPosition,
	bool *aTooLight,
	bool *aDocUpsideDown,
	int *thresholdUsed,
	MMMReaderDataCallback aDataCallback,
	void *aParam,
	int aSequenceNum,
	HANDLE aEvent,
	IMGSET *aImgSettings,
	DOCPOS *aDocPosSettings,
	int *aDirtPosn,
	Box *aCodelineLocation
);
#endif // ENABLE_GDIPLUS

/// Performs Optical Character Recognition (OCR) on a two document images to read the Machine
/// Readable Zone (MRZ) codeline from travel documents.
///
/// \ingroup ImageProcModule
///
/// \note This API is only relevant if using a \pagereader with the \llapi.
///
/// \param[in] aBlocking Specify whether this operation should be blocking or non-blocking;
/// if \c false, then this API will add a command to the queue to be carried out at a later
/// time and return immediately.
///
/// \param[in] aInputImage A GDI+ Bitmap object which is the source image to read from.
///
/// \param[in] aVisibleLight \todo Find out what this does
///
/// \param[in] aOCRSettings A pointer to a OCRSET structure which should contain the
/// required settings for the \pagereader.
///
/// \param[in] aOCRUserSettings A pointer to a OCRUSERSET structure which should contain the
/// required settings for the \pagereader.
///
/// \param[in] aDocPosition \todo Find out what this does
///
/// \param[out] aTooLight \todo Find out what this does
///
/// \param[out] aDocUpsideDown A pointer to a boolean flag which will be updated to state
/// whether the document image was determined to be upside down.
///
/// \param[out] thresholdUsed \todo Find out what this does
///
/// \param[in] aDataCallback A user-defined #MMMReaderDataCallback function which will receive
/// the codeline that was read as a string.
///
/// \param[in] aParam An optional pointer to some user-defined data. This is passed on to the
/// callback provided.
///
/// \param[in] aSequenceNum A user-defined value that identifies what data is coming back
/// through the \a aDataCallback.
///
/// \param[in] aEvent If \a aBlocking is \c true, pass in a handle to an OS event which will be
/// set once the operation finishes.
///
/// \param[in] aImgSettings A pointer to a IMGSET structure which should contain the
/// required settings for the \pagereader.
///
/// \param[in] aDocPosSettings A pointer to a DOCPOS structure which should contain the
/// required settings for the \pagereader.
///
/// \param[in] aDirtPosn A pointer to an integer which obtains the dirt position information
/// if there is dirt on the scanner window
///
/// \param[in] aInputRearImage A GDI+ Bitmap object which is the source image rear to read from as a secondary source.
///
/// \param[out] prMrzOnRearSide A pointer to a boolean flag which will be updated to state
/// whether the ocr was extracted from the rear image.
///
/// \return #NO_ERROR_OCCURRED if successful, otherwise a #MMMReaderErrorCode stating what
/// error occurred.
///
/// \remarks \a aParam can be used to provide some context data back through the callbacks. One
/// example could be the \c this pointer from a C++ class; in this case, because the callback
/// functions must be C-style functions, \a aParam would be able to access elements of its
/// class type.
///
/// \remarks \a aSequenceNum is a user-defined value which is passed straight into
/// \a aDataCallback. This parameter is provided so that it is possible to have one data
/// callback defined for multiple data items; the defined function can then filter the
/// \a aSequenceNum as appropriate.
///
/// \remarks Typically, the required settings structures are taken from #MMMReaderSettings by
/// using MMMReader_LL_LoadSettings().
///
/// \sa MMMReader_ImagePerformSecurityCheck()
#ifdef ENABLE_GDIPLUS

MMM_READER_LOW_LEVEL_API MMMReaderErrorCode MMMReader_ImageReadCodelineTwoSided(
	bool aBlocking,
	Gdiplus::Bitmap *aInputImage,
	bool aVisibleLight,
	OCRSET *aOCRSettings,
	OCRUSERSET *aOCRUserSettings,
	Box *aDocPosition,
	bool *aTooLight,
	bool *aDocUpsideDown,
	int *thresholdUsed,
	MMMReaderDataCallback aDataCallback,
	void *aParam,
	int aSequenceNum,
	HANDLE aEvent,
	IMGSET *aImgSettings,
	DOCPOS *aDocPosSettings,
	int *aDirtPosn,
	Gdiplus::Bitmap *aInputRearImage,
	bool *prMrzOnRearSide
);
#endif // ENABLE_GDIPLUS

/// Performs Optical Character Recognition (OCR) on a two document images to read the Machine
/// Readable Zone (MRZ) codeline from travel documents.
///
/// \ingroup ImageProcModule
///
/// \note This API is only relevant if using a \pagereader with the \llapi.
///
/// \param[in] aBlocking Specify whether this operation should be blocking or non-blocking;
/// if \c false, then this API will add a command to the queue to be carried out at a later
/// time and return immediately.
///
/// \param[in] aInputImage A GDI+ Bitmap object which is the source image to read from.
///
/// \param[in] aVisibleLight \todo Find out what this does
///
/// \param[in] aOCRSettings A pointer to a OCRSET structure which should contain the
/// required settings for the \pagereader.
///
/// \param[in] aOCRUserSettings A pointer to a OCRUSERSET structure which should contain the
/// required settings for the \pagereader.
///
/// \param[in] aDocPosition \todo Find out what this does
///
/// \param[out] aTooLight \todo Find out what this does
///
/// \param[out] aDocUpsideDown A pointer to a boolean flag which will be updated to state
/// whether the document image was determined to be upside down.
///
/// \param[out] thresholdUsed \todo Find out what this does
///
/// \param[in] aDataCallback A user-defined #MMMReaderDataCallback function which will receive
/// the codeline that was read as a string.
///
/// \param[in] aParam An optional pointer to some user-defined data. This is passed on to the
/// callback provided.
///
/// \param[in] aSequenceNum A user-defined value that identifies what data is coming back
/// through the \a aDataCallback.
///
/// \param[in] aEvent If \a aBlocking is \c true, pass in a handle to an OS event which will be
/// set once the operation finishes.
///
/// \param[in] aImgSettings A pointer to a IMGSET structure which should contain the
/// required settings for the \pagereader.
///
/// \param[in] aDocPosSettings A pointer to a DOCPOS structure which should contain the
/// required settings for the \pagereader.
///
/// \param[in] aDirtPosn A pointer to an integer which obtains the dirt position information
/// if there is dirt on the scanner window
///
/// \param[in] aInputRearImage A GDI+ Bitmap object which is the source image rear to read from as a secondary source.
///
/// \param[out] prMrzOnRearSide A pointer to a boolean flag which will be updated to state
/// whether the ocr was extracted from the rear image.
///
/// \param[out] aCodelineLocation A pointer to a Box which will be updated to the location of
/// the codeline.
///
/// \return #NO_ERROR_OCCURRED if successful, otherwise a #MMMReaderErrorCode stating what
/// error occurred.
///
/// \remarks \a aParam can be used to provide some context data back through the callbacks. One
/// example could be the \c this pointer from a C++ class; in this case, because the callback
/// functions must be C-style functions, \a aParam would be able to access elements of its
/// class type.
///
/// \remarks \a aSequenceNum is a user-defined value which is passed straight into
/// \a aDataCallback. This parameter is provided so that it is possible to have one data
/// callback defined for multiple data items; the defined function can then filter the
/// \a aSequenceNum as appropriate.
///
/// \remarks Typically, the required settings structures are taken from #MMMReaderSettings by
/// using MMMReader_LL_LoadSettings().
///
/// \sa MMMReader_ImagePerformSecurityCheck()
#ifdef ENABLE_GDIPLUS

MMM_READER_LOW_LEVEL_API MMMReaderErrorCode MMMReader_ImageReadCodelineTwoSidedEx(
	bool aBlocking,
	Gdiplus::Bitmap *aInputImage,
	bool aVisibleLight,
	OCRSET *aOCRSettings,
	OCRUSERSET *aOCRUserSettings,
	Box *aDocPosition,
	bool *aTooLight,
	bool *aDocUpsideDown,
	int *thresholdUsed,
	MMMReaderDataCallback aDataCallback,
	void *aParam,
	int aSequenceNum,
	HANDLE aEvent,
	IMGSET *aImgSettings,
	DOCPOS *aDocPosSettings,
	int *aDirtPosn,
	Gdiplus::Bitmap *aInputRearImage,
	bool *prMrzOnRearSide,
	Box *aCodelineLocation
);
#endif // ENABLE_GDIPLUS

/// Performs a security check to indicate whether a UV document image passes or fails, based
/// on the codeline read from the document.
///
/// \ingroup ImageProcModule
///
/// \note This API is only relevant if using a \pagereader with the \llapi.
///
/// \param[in] aBitmap A GDI+ Bitmap object which is the source image to check. This must be
/// a UV image.
///
/// \param[in] aCodeline A null-terminated string containing the codeline read from the
/// document.
///
/// \param[out] aResult A pointer to an integer which will be updated to store the result.
///
/// \return #NO_ERROR_OCCURRED if successful, otherwise a #MMMReaderErrorCode stating what
/// error occurred.
///
/// \remarks If bit 0 is set in \a aResult, this indicates that the codeline check failed.
/// If bit 1 is set in \a aResult, this indicates that the UV check failed.
///
/// \sa MMMReader_ImageReadCodeline()
#ifdef ENABLE_GDIPLUS
MMM_READER_LOW_LEVEL_API MMMReaderErrorCode MMMReader_ImagePerformSecurityCheck(
	Gdiplus::Bitmap *aBitmap,
	char *aCodeline,
	int *aResult
#ifdef USE_NEW_UV_SC_ALGORITHM
/* This alternate method does not appear to work for all documents
   - old algorithm has been changed so that it work with AT9000 */
	, int aThreshLevel,
	int aMaxWhitePixelPct
#endif
);
#endif // ENABLE_GDIPLUS

/// Confirms the check digits within a codeline determine its valid.
///
/// \ingroup ImageProcModule
///
/// \note This API is only relevant if using the \llapi.
///
/// \param[in] aCodeline A null-terminated string containing the codeline read from the
/// document.
///
/// \param[out] aResult A pointer to an integer which will be updated to store the result.
///
/// \return #NO_ERROR_OCCURRED if successful, otherwise a #MMMReaderErrorCode stating what
/// error occurred.
///
/// \remarks \a aResult can be one of the following values:
/// <table>
/// <tr><th>Value</th><th>Description</th></tr>
///
/// <tr><td>0</td><td>Checksum not checked</td></tr>
///
/// <tr><td>&gt; 0</td><td>Checksum OK</td></tr>
///
/// <tr><td>-1</td><td>Checksum error</td></tr>
///
/// <tr><td>&lt; -1</td><td>Checksum warning</td></tr>
///
/// </table>
///
/// \remarks The checksum warning signifies that the checksum did not compute but that
/// the document is not an ICAO document, so it could use different checksum rules. Warnings
/// can be produced from some three-line US documents.
///
/// \sa MMMReader_ImageReadCodeline(), MMMReader_ImageReadCodelineTwoSided
MMM_READER_LOW_LEVEL_API MMMReaderErrorCode MMMReader_ValidateCheckDigits(
	char *aCodeline,
	int *aResult
);

/// Validates all the check digits within a codeline and returns detailed
/// results.
///
/// \ingroup ImageProcModule
///
/// \note This API is only relevant if using the \llapi.
///
/// \param[in] aCodeline A null-terminated string containing the codeline read from the
/// document, including an end-of-line character at the end of each line.
///
/// \param[out] aResultString A pointer to an integer array which will be updated to
/// store the results. This array should be at least 26 elements in length to
/// handle all possible check digits.
///
///	\param[in/out] aResultStringLength A pointer to an integer specifying the
/// length of aResultString. The value of this integer will be changed to specify
/// the length of the results.
///
/// \return #NO_ERROR_OCCURRED if successful, otherwise a #MMMReaderErrorCode stating what
/// error occurred. If the buffer passed into aResultString is too small or null
/// #ERROR_STRING_BUFFER_TOO_SMALL will be returned.
///
/// \remarks \a aResult will be an array in the following format:
/// <table>
/// <tr><th>Index</th><th>Description</th></tr>
///
/// <tr><td>0</td><td>Overall result
/// <li>'U' = Checksums not checked</li>
/// <li>'F' = Checksums error</li>
/// <li>'N' = Checksums warning</li>
/// <li>'P' = Checksums OK</li>
/// </td></tr>
///
/// <tr><td>1</td><td>Check Digit Type ID
/// <li>'B' = DOB_ID</li>
/// <li>'E' = EXPIRY_ID</li>
/// <li>'D' = DOCNUM_ID</li>
/// <li>'O' = OPTIONAL_ID</li>
/// <li>'T' = OVERALL_ID</li>
/// </td></tr>
///
/// <tr><td>2</td><td>Codeline number check digit is located on (1, 2, or 3)</td></tr>
///
/// <tr><td>3</td><td>Character position on codeline of the check digit (1 based index)</td></tr>
///
/// <tr><td>4</td><td>Expected check digit value</td></tr>
///
/// <tr><td>5</td><td>Value of check digit read</td></tr>
///
/// </table>
///
/// \remarks The checksum warning signifies that the checksum did not compute but that
/// the document is not an ICAO document, so it could use different checksum rules. Warnings
/// can be produced from some three-line US documents.
///
/// \sa MMMReader_ImageReadCodeline(), MMMReader_ImageReadCodelineTwoSided
MMM_READER_LOW_LEVEL_API MMMReaderErrorCode MMMReader_ValidateExtendedCheckDigits(
	char *aCodeline,
	int *aResultString,
	int *aResultStringLength
);

/// Converts an image from one format into another.
///
/// \ingroup ImageProcModule
///
/// \note This API is only relevant if using a \pagereader with the \llapi.
///
/// \param[in] aDestinationFormat Specify which format the source image should be converted to.
/// Use one of the #MMMReaderImageFormats values.
///
/// \param[in] aInBuffer A pointer to the raw image data to convert from.
///
/// \param[in] aInBufferLen The size of \a aInBuffer in bytes.
///
/// \param[out] aOutBuffer A pointer to a writable buffer which will be updated to store the
/// converted image. This can be \c NULL; see the remarks section below for details.
///
/// \param[in,out] aOutBufferLen Initially will contain the size of the \a aOutBuffer.
/// When the function returns, this value will be the actual size of the resulting image.
///
/// \return #NO_ERROR_OCCURRED if successful, otherwise a #MMMReaderErrorCode stating what
/// error occurred.
///
/// \remarks \a aInBuffer should point to an image which can be in any of the following
/// image formats:
///		- JPEG
///		- JPEG 2000
///		- PNG
///		- BMP
///		- TIF
///
/// \remarks This API should be called twice; the first time, \a aOutBuffer should be
/// \c NULL and \a aOutBufferLen should be zero. This will provide the correct length
/// of the \a aOutBuffer via \a aOutBufferLen. The second time should then pass a correctly
/// allocated buffer to contain the result.
MMM_READER_LOW_LEVEL_API MMMReaderErrorCode MMMReader_ImageConvertFormat(
	int aDestinationFormat,
	unsigned char *aInBuffer,
	int aInBufferLen,
	char *aOutBuffer,
	int *aOutBufferLen
);

/// Performs a number of image processing operations to improve a raw image captured by
/// the \pagereader camera via the MMMReader_CameraTakeSnapshot() API.
///
/// \ingroup ImageProcModule
///
/// \note This API is only relevant if using a \pagereader with the \llapi.
///
/// \param[in] aBlocking Specify whether this operation should be blocking or non-blocking;
/// if \c false, then this API will add a command to the queue to be carried out at a later
/// time and return immediately.
///
/// \param[in] aInputImage A GDI+ Bitmap object which is the source image to post-process.
///
/// \param[in] aLight A bitwise flag specifying which lights where used when taking the image,
/// e.g. #LIGHT_IRFULL. Only required if \a aOperations includes the #PP_NORMALISE flag.
///
/// \param[in] aLightingBanks A bitwise flag specifying which lighting banks were used when
/// taking the image, e.g. #LIGHT_BANK_ALL. Only required if \a aOperations includes the
/// #PP_NORMALISE flag.
///
/// \param[in] aColour Specify whether the image is a colour image. If \c false, then
/// \a aInputImage is considered to be grayscale.
///
/// \param[in] aOperations A bitmask value of all the post-processing operations to perform.
/// See the remarks section below for more details.
///
/// \param[in] aDMQuality \todo Find out what this does
///
/// \param[in] aCorrectionMatrix Specify the colour correction matrix to use.
///
/// \param[in] aSettings A pointer to a CAMERASET structure which should contain the
/// required settings for the \pagereader.
///
/// \param[in] aDocDetectSettings A pointer to a DOCDETECTSETTINGS structure which should
/// contain the required settings for the \pagereader.
///
/// \param[in] aDocPosition The position of the document as received from
/// MMMReader_CameraDetectDocument() or MMMReader_LocateDocument(). Only required if
/// \a aOperations includes the #PP_CROP and/or #PP_ROTATE flags.
///
/// \param[in] aDataCallback A user-defined #MMMReaderDataCallback function which will receive
/// the post-processed image result as a GDI+ Bitmap.
///
/// \param[in] aParam An optional pointer to some user-defined data. This is passed on to the
/// callback provided.
///
/// \param[in] aSequenceNum A user-defined value that identifies what data is coming back
/// through the \a aDataCallback.
///
/// \param[in] aEvent If \a aBlocking is \c true, pass in a handle to an OS event which will be
/// set once the operation finishes.
///
/// \param[in] aDocPosSettings A pointer to a DOCPOS structure which should
/// contain the required settings for the \pagereader.
///
/// \return #NO_ERROR_OCCURRED if successful, otherwise a #MMMReaderErrorCode stating what
/// error occurred.
///
/// \remarks \a aParam can be used to provide some context data back through the callbacks. One
/// example could be the \c this pointer from a C++ class; in this case, because the callback
/// functions must be C-style functions, \a aParam would be able to access elements of its
/// class type.
///
/// \remarks \a aSequenceNum is a user-defined value which is passed straight into
/// \a aDataCallback. This parameter is provided so that it is possible to have one data
/// callback defined for multiple data items; the defined function can then filter the
/// \a aSequenceNum as appropriate.
///
/// \remarks Typically, the required settings structures are taken from #MMMReaderSettings by
/// using MMMReader_LL_LoadSettings(). The \a aCorrectionMatrix value should be taken from
/// the #BRIGHTNESSSETTINGS::cmatrix data member.
///
/// \remarks This API can perform any number of the following post-processing operations,
/// specified by the bitmask \a aOperations:
///		- #PP_NONE
///		- #PP_DEMOSAIC
///		- #PP_NORMALISE
///		- #PP_SMALL
///		- #PP_CROP
///		- #PP_ROTATE
///		- #PP_DEBARREL
///		- #PP_RELOCATEDOCUMENT
///		- #PP_SHARPEN
///		- #PP_FULLGREY
///		- #PP_GAMMA
///		- #PP_CONTRAST
///		- #PP_COLOURHUE
///		- #PP_EDGE_ENHANCE
///
/// \remarks Although this API is part of the image processing module, it requires that
/// the \pagereader camera be initialised (via MMMReader_CameraInitialise()) to perform
/// the de-mosaicing (#PP_DEMOSAIC) functionality.
///
/// \remarks Once the post-processing has completed, \a aDataCallback will return the
/// resulting image as a GDI+ Bitmap.
///
/// \remarks When normalising \a aInputImage (#PP_NORMALISE), the \a aLight and
/// \a aLightingBanks values are required, which will be the same as those passed into
/// MMMReader_CameraTakeSnapshot().
///
/// \remarks When cropping (#PP_CROP) and/or rotating (#PP_ROTATE) \a aInputImage, the
/// \a aDocPosition must be provided, which is received from the
/// MMMReader_CameraDetectDocument() or MMMReader_LocateDocument() APIs.
#ifdef ENABLE_GDIPLUS
MMM_READER_LOW_LEVEL_API MMMReaderErrorCode MMMReader_ImagePostProcessImage(
	bool aBlocking,
	Gdiplus::Bitmap *aInputImage,
	int aLight,
	int aLightingBanks,
	bool aColour,
	int aOperations,
	int aDMQuality,
	int aCorrectionMatrix,
	float aGammaValue,
	CAMERASET *aSettings,
	DOCDETECTSETTINGS *aDocDetectSettings,
	Box *aDocPosition,
	MMMReaderDataCallback aDataCallback,
	void *aParam,
	int aSequenceNum,
	HANDLE aEvent,
	DOCPOS *aDocPosSettings
);
#endif // ENABLE_GDIPLUS

/// Performs a number of image processing operations to improve a raw image captured by
/// the \pagereader camera via the MMMReader_CameraTakeSnapshot() API. This function can 
/// also process the CR54000 two sided image then split the input into two separate output images
///
/// \ingroup ImageProcModule
///
/// \note This API is only relevant if using a \pagereader with the \llapi.
///
/// \param[in] aBlocking Specify whether this operation should be blocking or non-blocking;
/// if \c false, then this API will add a command to the queue to be carried out at a later
/// time and return immediately.
///
/// \param[in] aInputImage A GDI+ Bitmap object which is the source image to post-process.
///
/// \param[in] aLight A bitwise flag specifying which lights where used when taking the image,
/// e.g. #LIGHT_IRFULL. Only required if \a aOperations includes the #PP_NORMALISE flag.
///
/// \param[in] aLightingBanks A bitwise flag specifying which lighting banks were used when
/// taking the image, e.g. #LIGHT_BANK_ALL. Only required if \a aOperations includes the
/// #PP_NORMALISE flag.
///
/// \param[in] aColour Specify whether the image is a colour image. If \c false, then
/// \a aInputImage is considered to be grayscale.
///
/// \param[in] aOperations A bitmask value of all the post-processing operations to perform.
/// See the remarks section below for more details.
///
/// \param[in] aDMQuality \todo Find out what this does
///
/// \param[in] aCorrectionMatrix Specify the colour correction matrix to use.
///
/// \param[in] aSettings A pointer to a CAMERASET structure which should contain the
/// required settings for the \pagereader.
///
/// \param[in] aDocDetectSettings A pointer to a DOCDETECTSETTINGS structure which should
/// contain the required settings for the \pagereader.
///
/// \param[in] aDocPosition The position of the document as received from
/// MMMReader_CameraDetectDocument() or MMMReader_LocateDocument(). Only required if
/// \a aOperations includes the #PP_CROP and/or #PP_ROTATE flags.
///
/// \param[in] aDocPositionSide2 The position of the document as received from
/// MMMReader_CameraDetectDocument() or MMMReader_LocateDocument(). Only required if
/// the image is split two sides.
///
/// \param[in] aDataCallback A user-defined #MMMReaderDataCallback function which will receive
/// the post-processed image result as a GDI+ Bitmap.
///
/// \param[in] aParam An optional pointer to some user-defined data. This is passed on to the
/// callback provided.
///
/// \param[in] aSequenceNum A user-defined value that identifies what data is coming back
/// through the \a aDataCallback.
///
/// \param[in] aEvent If \a aBlocking is \c true, pass in a handle to an OS event which will be
/// set once the operation finishes.
///
/// \param[in] aDocPosSettings A pointer to a DOCPOS structure which should
/// contain the required settings for the \pagereader.
///
/// \param[in] aDocPosSettingsSide2 A pointer to a DOCPOS structure which should
/// contain the required settings for the \pagereader when the image is split two sides.
///
/// \return #NO_ERROR_OCCURRED if successful, otherwise a #MMMReaderErrorCode stating what
/// error occurred.
///
/// \remarks \a aParam can be used to provide some context data back through the callbacks. One
/// example could be the \c this pointer from a C++ class; in this case, because the callback
/// functions must be C-style functions, \a aParam would be able to access elements of its
/// class type.
///
/// \remarks \a aSequenceNum is a user-defined value which is passed straight into
/// \a aDataCallback. This parameter is provided so that it is possible to have one data
/// callback defined for multiple data items; the defined function can then filter the
/// \a aSequenceNum as appropriate.
///
/// \remarks Typically, the required settings structures are taken from #MMMReaderSettings by
/// using MMMReader_LL_LoadSettings(). The \a aCorrectionMatrix value should be taken from
/// the #BRIGHTNESSSETTINGS::cmatrix data member.
///
/// \remarks This API can perform any number of the following post-processing operations,
/// specified by the bitmask \a aOperations:
///		- #PP_NONE
///		- #PP_DEMOSAIC
///		- #PP_NORMALISE
///		- #PP_SMALL
///		- #PP_CROP
///		- #PP_ROTATE
///		- #PP_DEBARREL
///		- #PP_RELOCATEDOCUMENT
///		- #PP_SHARPEN
///		- #PP_FULLGREY
///		- #PP_GAMMA
///		- #PP_CONTRAST
///		- #PP_COLOURHUE
///		- #PP_EDGE_ENHANCE
///		- #PP_SPLIT
///
/// \remarks Although this API is part of the image processing module, it requires that
/// the \pagereader camera be initialised (via MMMReader_CameraInitialise()) to perform
/// the de-mosaicing (#PP_DEMOSAIC) functionality.
///
/// \remarks Once the post-processing has completed, \a aDataCallback will return the
/// resulting image as a GDI+ Bitmap.
///
/// \remarks When normalising \a aInputImage (#PP_NORMALISE), the \a aLight and
/// \a aLightingBanks values are required, which will be the same as those passed into
/// MMMReader_CameraTakeSnapshot().
///
/// \remarks When cropping (#PP_CROP) and/or rotating (#PP_ROTATE) \a aInputImage, the
/// \a aDocPosition must be provided, which is received from the
/// MMMReader_CameraDetectDocument() or MMMReader_LocateDocument() APIs.
#ifdef ENABLE_GDIPLUS
MMM_READER_LOW_LEVEL_API MMMReaderErrorCode MMMReader_ImagePostProcessImageTwoSided(
	bool aBlocking,
	Gdiplus::Bitmap *aInputImage,
	int aLight,
	int aLightingBanks,
	bool aColour,
	int aOperations,
	int aDMQuality,
	int aCorrectionMatrix,
	float aGammaValue,
	CAMERASET *aSettings,
	DOCDETECTSETTINGS *aDocDetectSettings,
	Box *aDocPosition,
	Box *aDocPositionSide2,
	MMMReaderDataCallback aDataCallback,
	void *aParam,
	int aSequenceNum,
	HANDLE aEvent,
	DOCPOS *aDocPosSettings,
	DOCPOS *aDocPosSettingsSide2
);

#endif // ENABLE_GDIPLUS

/// \endcond
// PAGESDK


///////////  Signalling Interface

/// \cond PAGESDK

/// Initialises the \sdk signalling module and allocates resources required to use the
/// sound and LED interfaces.
///
/// \ingroup SignalModule
///
/// \note This API is only relevant if using a \pagereader with the \llapi.
///
/// \param[in] aFileLocations A null-terminated string providing the folder location of
/// any LED configuration files.
///
/// \param[in] wavFileLocation A null-terminated string providing the folder location of
/// any WAV sound files.
///
/// \return #NO_ERROR_OCCURRED if successful, otherwise a #MMMReaderErrorCode stating what
/// error occurred.
///
/// \attention The MMMReader_SignalShutdown() API must be called when finished with
/// the signalling module to free all resources allocated.
///
/// \remarks LEDs will not be used for signalling until the \pagereader camera has been
/// initialised via MMMReader_CameraInitialise(). However, there is no requirement to
/// initialise the camera prior to calling this API.
///
/// \remarks \a aFileLocations should be the folder location where the \c Led.ini file can
/// be located; this can be used to configure the LED scheme for the \rte8000 and \rte8000hs
/// readers. Typically this value should be set to the value provided by
/// #MMMReaderSettings::puReaderDir or the MMMReader_LL_GetReaderDir() API.
///
/// \remarks \a wabFileLocation should be the folder location where any WAV (*.wav) files
/// can be located if the \sdk has been configured to play WAV sounds (either using the
/// #SM_WAV_SPEECH or #SM_WAV_EFFECTS #MMMReaderSoundMode). Typically this value should be set
/// to the value provided by #MMMReaderSettings::puCfgDir or the MMMReader_LL_GetConfigDir()
/// API.
///
/// \sa MMMReader_SignalShutdown(), MMMReader_IsSignalInitialised(), MMMReader_SignalEvent()
MMM_READER_LOW_LEVEL_API MMMReaderErrorCode MMMReader_SignalInitialise(
	RTCHAR *aFileLocations,
	RTCHAR *wavFileLocation
);

/// Shuts down the \sdk signalling module and frees all related resources.
///
/// \ingroup SignalModule
///
/// \note This API is only relevant if using a \pagereader with the \llapi.
///
/// \return #NO_ERROR_OCCURRED if successful, otherwise a #MMMReaderErrorCode stating what
/// error occurred.
///
/// \sa MMMReader_SignalInitialise(), MMMReader_IsSignalInitialised()
MMM_READER_LOW_LEVEL_API MMMReaderErrorCode MMMReader_SignalShutdown();

/// Determines whether the \sdk signalling module has been initialised.
///
/// \ingroup SignalModule
///
/// \note This API is only relevant if using a \pagereader with the \llapi.
///
/// \return \c true if MMMReader_SignalInitialise() has been called, or \c false otherwise.
///
/// \sa MMMReader_SignalInitialise()
MMM_READER_LOW_LEVEL_API bool MMMReader_IsSignalInitialised();

/// Signal an event has occurred with potentially a sound and the LED indicators.
///
/// \ingroup SignalModule
///
/// \note This API is only relevant if using a \pagereader with the \llapi.
///
/// \param[in] aSoundSettings A pointer to a SoundSettings structure which should
/// contain the required settings for the \pagereader.
///
/// \param[in] aEvent Specify which #MMMReaderSignal to indicate.
///
/// \return #NO_ERROR_OCCURRED if successful, otherwise a #MMMReaderErrorCode stating what
/// error occurred.
///
/// \remarks Typically, the required settings structures are taken from #MMMReaderSettings by
/// using MMMReader_LL_LoadSettings().
///
/// \remarks Some of the #MMMReaderSignal values can only be used for either signalling the
/// LEDs or signalling a sound, while most can be used for both.
///
/// \sa MMMReader_LedEvent(), MMMReader_SoundEvent()
MMM_READER_LOW_LEVEL_API MMMReaderErrorCode MMMReader_SignalEvent(
	SoundSettings *aSoundSettings,
	MMMReaderSignal aEvent
);

/// Signal an event has occurred with only the LED indicators.
///
/// \ingroup SignalModule
///
/// \note This API is only relevant if using a \pagereader with the \llapi.
///
/// \param[in] aEvent Specify which #MMMReaderSignal to indicate.
///
/// \return #NO_ERROR_OCCURRED if successful, otherwise a #MMMReaderErrorCode stating what
/// error occurred.
///
/// \remarks Some of the #MMMReaderSignal values can only be used for either signalling the
/// LEDs or signalling a sound, while most can be used for both.
///
/// \sa MMMReader_SignalEvent(), MMMReader_SoundEvent()
MMM_READER_LOW_LEVEL_API MMMReaderErrorCode MMMReader_LedEvent(
	MMMReaderSignal aEvent
);

/// Signal an event has occurred with only a sound.
///
/// \ingroup SignalModule
///
/// \note This API is only relevant if using a \pagereader with the \llapi.
///
/// \param[in] aSoundSettings A pointer to a SoundSettings structure which should
/// contain the required settings for the \pagereader.
///
/// \param[in] aEvent Specify which #MMMReaderSignal to indicate.
///
/// \return #NO_ERROR_OCCURRED if successful, otherwise a #MMMReaderErrorCode stating what
/// error occurred.
///
/// \remarks Typically, the required settings structures are taken from #MMMReaderSettings by
/// using MMMReader_LL_LoadSettings().
///
/// \remarks Some of the #MMMReaderSignal values can only be used for either signalling the
/// LEDs or signalling a sound, while most can be used for both.
///
/// \sa MMMReader_SignalEvent(), MMMReader_LedEvent()
MMM_READER_LOW_LEVEL_API MMMReaderErrorCode MMMReader_SoundEvent(
	SoundSettings *aSoundSettings,
	MMMReaderSignal aEvent
);

/// \deprecated This API is no longer used.
MMM_READER_LOW_LEVEL_API MMMReaderErrorCode MMMReader_BGR_SoundBuzzer(
	bool aTurnOn
);

/// \endcond
// PAGESDK

///////////  Error handling APIs

/// Assigns a user-defined #MMMReaderErrorCallback function to receive error messages
/// when an \sdk error occurs.
///
/// \if SWIPESDK
/// \ingroup ErrorsModule
/// \else
/// \ingroup LLErrorsLoggingModule
/// \endif
///
/// \if PAGESDK
/// \note This API is only relevant if using the \llapi.
/// \endif
///
/// \param[in] aCallback A pointer to a user-defined function that will receive error
/// messages generated by the \sdk.
///
/// \param[in] aCallbackParam An optional pointer to some user-defined data. This is passed on
/// to \a aCallback
///
/// \remarks \a aParam can be used to provide some context data back through the callbacks. One
/// example could be the \c this pointer from a C++ class; in this case, because the callback
/// functions must be C-style functions, \a aParam would be able to access elements of its
/// class type.
///
/// \note The \a aCallback will be called from within the scope of the thread that produced
/// the error, so that thread will not continue until the error has completed but other
/// threads may continue during this call. Therefore, callers should ensure that any
/// re-initialisation logic performed within the \a aCallback is thread-safe.
///
/// \sa MMMReader_InitialiseLogging()
MMM_READER_LOW_LEVEL_API void MMMReader_SetErrorHandler(
	MMMReaderErrorCallback aCallback,
	void *aCallbackParam
);

/// Gets the error string and error code associated with the last \sdk error that occurred.
///
/// \if SWIPESDK
/// \ingroup ErrorsModule
/// \else
/// \ingroup LLErrorsLoggingModule
/// \endif
///
/// \if PAGESDK
/// \note This API is only relevant if using the \llapi. The \hlapi has an equivalent API
/// called MMMReader_GetLastError().
/// \endif
///
/// \param[out] aErrorCode A pointer to a #MMMReaderErrorCode which will contain the last
/// error code recorded.
///
/// \param[out] aErrorString A pointer to a string buffer which will contain the last error
/// string recorded. This can be \c NULL; see the remarks section below for details.
///
/// \param[in,out] aStrLen Initially will contain the length of the \a aErrorString
/// buffer. When the function returns, this value will be the actual length of the string.
///
/// \return #NO_ERROR_OCCURRED if successful, otherwise a #MMMReaderErrorCode stating what
/// error occurred.
///
/// \remarks This API should be called twice; the first time, \a aErrorString should be
/// \c NULL and \a aStrLen should be zero. This will provide the correct length of the
/// \a aErrorString buffer via \a aStrLen. The second time should then pass a correctly
/// allocated string buffer to contain the result.
///
/// \remarks This API will only return the last error recorded. If an #MMMReaderErrorCallback
/// was supplied to MMMReader_SetErrorHandler(), all errors will also be received via that
/// callback function.
///
/// \sa MMMReader_LL_GetErrorMessage()
MMM_READER_LOW_LEVEL_API MMMReaderErrorCode MMMReader_LL_GetLastError(
	MMMReaderErrorCode *aErrorCode,
	RTCHAR *aErrorString,
	int *aStrLen
);

/// Gets the error string associated with a given #MMMReaderErrorCode.
///
/// \if SWIPESDK
/// \ingroup ErrorsModule
/// \else
/// \ingroup LLErrorsLoggingModule
/// \endif
///
/// \if PAGESDK
/// \note This API is only relevant if using the \llapi. The \hlapi has an equivalent API
/// called MMMReader_GetErrorMessage().
/// \endif
///
/// \param[in] aErrorCode The #MMMReaderErrorCode to get the error message for.
///
/// \param[out] aErrorString A pointer to a string buffer which will contain the error
/// string. This can be \c NULL; see the remarks section below for details.
///
/// \param[in,out] aStrLen Initially will contain the length of the \a aErrorString
/// buffer. When the function returns, this value will be the actual length of the string.
///
/// \return #NO_ERROR_OCCURRED if successful, otherwise a #MMMReaderErrorCode stating what
/// error occurred.
///
/// \remarks This API should be called twice; the first time, \a aErrorString should be
/// \c NULL and \a aStrLen should be zero. This will provide the correct length of the
/// \a aErrorString buffer via \a aStrLen. The second time should then pass a correctly
/// allocated string buffer to contain the result.
///
/// \note The error string returned may include formatting flags (e.g. \c "%d", \c "%s", etc.)
/// which would typically be populated with context dependent data when \a aErrorCode
/// occurs.
///
/// \sa MMMReader_LL_GetLastError()
MMM_READER_LOW_LEVEL_API MMMReaderErrorCode MMMReader_LL_GetErrorMessage(
	MMMReaderErrorCode aErrorCode,
	RTCHAR *aErrorString,
	int *aStrLen
);

/// Assigns a user-defined #MMMReaderWarningCallback function to receive warning messages
/// when an \sdk warning occurs.
///
/// \if SWIPESDK
/// \ingroup WarningsModule
/// \else
/// \ingroup LLErrorsLoggingModule
/// \endif
///
/// \if PAGESDK
/// \note This API is only relevant if using the \llapi.
/// \endif
///
/// \param[in] aCallback A pointer to a user-defined function that will receive warning
/// messages generated by the \sdk.
///
/// \param[in] aCallbackParam An optional pointer to some user-defined data. This is passed on
/// to \a aCallback
///
/// \remarks \a aParam can be used to provide some context data back through the callbacks. One
/// example could be the \c this pointer from a C++ class; in this case, because the callback
/// functions must be C-style functions, \a aParam would be able to access elements of its
/// class type.
///
/// \note The \a aCallback will be called from within the scope of the thread that produced
/// the warning, so that thread will not continue until the warning has completed but other
/// threads may continue during this call. Therefore, callers should ensure that any
/// logic performed within the \a aCallback is thread-safe.
///
/// \sa MMMReader_InitialiseLogging()
MMM_READER_LOW_LEVEL_API void MMMReader_LL_SetWarningHandler(
	MMMReaderWarningCallback aCallback,
	void *aCallbackParam
);

/// Enables or disables the \sdk logging feature.
///
/// \if SWIPESDK
/// \ingroup LoggingModule
/// \else
/// \ingroup LLErrorsLoggingModule
/// \endif
///
/// \if PAGESDK
/// \note This API is only relevant if using the \llapi. The \hlapi has an equivalent API
/// called MMMReader_EnableLogging().
/// \endif
///
/// \param[in] aEnabled Specify whether logging should be enabled or disabled.
///
/// \param[in] aLogLevel Specify how detailed the logging should be. If \a aEnabled is
/// \c false, this value can be ignored (set to -1).
///
/// \param[in] aLogMask Specify which areas of the \sdk should be logged to file by providing
/// a bitmask of flags. Typically, this value should be #LOGMASK_ALL.
///
/// \param[in] aFilename Specify the name of the file to write log messages to. If
/// \a aEnabled is \c false, this value can be ignored (set to \c NULL).
///
/// \return #NO_ERROR_OCCURRED if successful, otherwise a #MMMReaderErrorCode stating what
/// error occurred.
///
/// \attention The MMMReader_ShutdownLogging() API must be called when finished with
/// error logging to free all resources allocated.
///
/// \remarks This API is useful for generating log files for diagnosing problems; \ssd may
/// ask you to enable logging for such a scenario.
///
/// \remarks Altering the \a aLogLevel will produce more or less detailed log files. Zero (0)
/// represents logging of errors only, while 5 represents the highest, most detailed logging
/// possible. \ssd will typically ask for log level 5 files to assist them in diagnosing
/// problems.
///
/// \remarks Once logging has been enabled, the host application may use the
/// MMMReader_LL_LogMessage() and MMMReader_LL_LogFormatted() APIs to add its own messages
/// into the main \sdk log file.
///
/// \sa MMMReader_ShutdownLogging(), MMMReader_SetLoggingOptions(), MMMReader_LL_LogMessage(),
/// MMMReader_LL_LogFormatted(), MMMReader_LL_GetLastError()
MMM_READER_LOW_LEVEL_API MMMReaderErrorCode MMMReader_InitialiseLogging(
	bool aEnabled,
	int aLogLevel,
	int aLogMask,
	const RTCHAR *aFilename
);

/// Shuts down the \sdk logging features and frees all related resources.
///
/// \if SWIPESDK
/// \ingroup LoggingModule
/// \else
/// \ingroup LLErrorsLoggingModule
/// \endif
///
/// \if PAGESDK
/// \note This API is only relevant if using the \llapi.
/// \endif
///
/// \return #NO_ERROR_OCCURRED if successful, otherwise a #MMMReaderErrorCode stating what
/// error occurred.
///
/// \sa MMMReader_InitialiseLogging()
MMM_READER_LOW_LEVEL_API MMMReaderErrorCode MMMReader_ShutdownLogging();

/// Logs a message from the host application to the main \sdk log file.
///
/// \if SWIPESDK
/// \ingroup LoggingModule
/// \else
/// \ingroup LLErrorsLoggingModule
/// \endif
///
/// \if PAGESDK
/// \note This API is only relevant if using the \llapi when logging has been enabled. The
/// \hlapi has an equivalent API called MMMReader_LogMessage().
/// \endif
///
/// \param[in] aLevel Specify the level of the log message. The higher the number, the less
/// important the message is.
///
/// \param[in] aMask Typically for host applications, this should be #LOGMASK_ALL.
///
/// \param[in] aLocation A null-terminated string specifying the location where the message
/// was generated. Typically this is the code location in the host application where the
/// message was generated to assist in tracing messages in the log file.
///
/// \param[in] aMessage A null-terminated string providing the message to write to the log
/// file.
///
/// \return #NO_ERROR_OCCURRED if successful, otherwise a #MMMReaderErrorCode stating what
/// error occurred.
///
/// \remarks This API is useful for host applications to add their own messages to the main
/// \sdk log file. It is recommended that this API be only used to add messages related to the
/// \pagereader, \swipereader and/or \sdk, and not general application activity.
///
/// \remarks If logging has been disabled via the MMMReader_InitialiseLogging() API, this API
/// will have no effect.
///
/// \remarks Only messages that have a \a aLogLevel less than the value provided to
/// MMMReader_InitialiseLogging() will be written to the \sdk log file.
///
/// \sa MMMReader_InitialiseLogging(), MMMReader_LL_LogFormatted()
MMM_READER_LOW_LEVEL_API MMMReaderErrorCode MMMReader_LL_LogMessage(
	int aLevel,
	int aMask,
	RTCHAR *aLocation,
	RTCHAR *aMessage
);

/// Logs a formatted message from the host application to the main \sdk log file.
///
/// \if SWIPESDK
/// \ingroup LoggingModule
/// \else
/// \ingroup LLErrorsLoggingModule
/// \endif
///
/// \if PAGESDK
/// \note This API is only relevant if using the \llapi when logging has been enabled. The
/// \hlapi has an equivalent API called MMMReader_LogFormatted().
/// \endif
///
/// \param[in] aLevel Specify the level of the log message. The higher the number, the less
/// important the message is.
///
/// \param[in] aMask Typically for host applications, this should be #LOGMASK_ALL.
///
/// \param[in] aLocation A null-terminated string specifying the location where the message
/// was generated. Typically this is the code location in the host application where the
/// message was generated to assist in tracing messages in the log file.
///
/// \param[in] aFormat A null-terminated string providing the format of the message to write
/// to the log file. This can contain formatting flags as used in C functions such as
/// \c printf(), \c sprintf(), etc.
///
/// \param[in] ... A variable list of parameters which match up with the formatting flags
/// in \a aFormat to create the literal message.
///
/// \return #NO_ERROR_OCCURRED if successful, otherwise a #MMMReaderErrorCode stating what
/// error occurred.
///
/// \remarks This API is useful for host applications to add their own messages to the main
/// \sdk log file. It is recommended that this API be only used to add messages related to the
/// \pagereader and/or \sdk, and not general application activity.
///
/// \remarks The difference between this API and MMMReader_LL_LogMessage() is that this API
/// acts in a similar way to the C functions such as \c printf(), \c sprintf(), etc.
/// Formatting flags (e.g. \c "%d", \c "%s", etc.) can be provided in \a aFormat, where they
/// will be replaced by the variable list of parameters after \a aFormat. This API is useful
/// when the host application needs to log non-string values to the log file.
///
/// \remarks If logging has been disabled via the MMMReader_InitialiseLogging() API, this API
/// will have no effect.
///
/// \remarks Only messages that have a \a aLogLevel less than the value provided to
/// MMMReader_InitialiseLogging() will be written to the \sdk log file.
///
/// \sa MMMReader_InitialiseLogging(), MMMReader_LL_LogMessage()
MMM_READER_LOW_LEVEL_API MMMReaderErrorCode MMMReader_LL_LogFormatted(
	int aLevel,
	int aMask,
	RTCHAR *aLocation,
	RTCHAR *aFormat,
	...
);

/// Configure the \sdk logging features with the provided LoggingSettings.
///
/// \if SWIPESDK
/// \ingroup LoggingModule
/// \else
/// \ingroup LLErrorsLoggingModule
/// \endif
///
/// \if PAGESDK
/// \note This API is only relevant if using the \llapi.
/// \endif
///
/// \param[in] aOptions A pointer to a LoggingSettings structure which should
/// contain the required settings for logging.
///
/// \return #NO_ERROR_OCCURRED if successful, otherwise a #MMMReaderErrorCode stating what
/// error occurred.
///
/// \if PAGESDK
///
/// \remarks Typically, the required settings structures are taken from #MMMReaderSettings by
/// using MMMReader_LL_LoadSettings().
///
/// \else
///
/// \remarks Typically, the required settings structures are loaded via
/// MMMReader_LL_LoadLoggingSettings().
///
/// \endif
///
/// \sa MMMReader_InitialiseLogging()
MMM_READER_LOW_LEVEL_API MMMReaderErrorCode MMMReader_SetLoggingOptions(
	LoggingSettings* aOptions
);


///////////  Settings APIs

/// \cond PAGESDK

/// \internal
MMM_READER_LOW_LEVEL_API MMMReaderErrorCode MMMReader_LL_ResetSettingsData();

/// Loads the \sdk configuration settings from files located in the default location.
///
/// \ingroup LLSettingsModule
///
/// \note This API is only relevant if using the \llapi. The \hlapi has an equivalent
/// API called MMMReader_GetSettings().
///
/// \param[out] theSettings A pointer to a MMMReaderSettings structure which will be populated
/// with the \sdk settings loaded.
///
/// \return #NO_ERROR_OCCURRED if successful, otherwise a #MMMReaderErrorCode stating what
/// error occurred.
///
/// \remarks The \sdk configuration settings are defined under two categories:
///		- Calibration - these settings are loaded from a \pagereader or protected configuration
///		files. Calibration settings should not be modified unless absolutely necessary.
///		- Application - these settings are loaded from standard configuration files and can be
///		freely modified.
///
/// \remarks Application (and some calibration) configuration files are stored in the
/// \sdk Config folder. This API assumes that the Config folder is located as directed by
/// the \c MMMReader.ini file that must be present in the \sdk binaries folder.
///
/// \remarks For \pagereaders, calibration settings are loaded from the \pagereader itself.
/// For \pagereaders which have a Mass Storage Device (MSD), such as the \rte8000, calibration
/// setting files are located here. For all other \pagereaders, calibration settings are stored
/// directly into the \pagereader camera memory; in this scenario, these calibration settings
/// can only be accessed through the \sdk.
///
/// \remarks For \swipereaders, calibration settings are stored in protected configuration
/// files under the \sdk Config folder.
///
/// \sa MMMReader_LL_LoadSettingsFromIniFile(), MMMReader_LL_SaveSettings()
MMM_READER_LOW_LEVEL_API MMMReaderErrorCode MMMReader_LL_LoadSettings(
	MMMReaderSettings *theSettings
);

/// Loads the \sdk configuration settings from files located in as specified by a customised
/// location file.
///
/// \ingroup LLSettingsModule
///
/// \note This API is only relevant if using the \llapi.
///
/// \param[out] aTheSettings A pointer to a MMMReaderSettings structure which will be populated
/// with the \sdk settings loaded.
///
/// \param[in] aMMMReaderIniFilePath A null-terminated string providing the file path of the
/// location configuration file to use instead of the standard \sdk \c MMMReader.ini file.
///
/// \return #NO_ERROR_OCCURRED if successful, otherwise a #MMMReaderErrorCode stating what
/// error occurred.
///
/// \remarks This API differs from MMMReader_LL_LoadSettings() in that this API must be
/// explicitly instructed where the \sdk folders are located. This is done by the customised
/// location configuration file provided by \a aMMMReaderIniFilePath.
///
/// \remarks This API is not recommended for most customers; the preferred, general way of
/// loading \sdk configuration settings is by the MMMReader_LL_LoadSettings() API.
///
/// \sa MMMReader_LL_LoadSettings(), MMMReader_LL_SaveSettings()
MMM_READER_LOW_LEVEL_API MMMReaderErrorCode MMMReader_LL_LoadSettingsFromIniFile(
	MMMReaderSettings *aTheSettings,
	RTCHAR *aMMMReaderIniFilePath
);

/// Initialises a MMMReaderSettings structure with default values for all settings.
///
/// \ingroup LLSettingsModule
///
/// \note This API is only relevant if using the \llapi.
///
/// \param[out] aSettings A pointer to a MMMReaderSettings structure which will be populated
/// with the default \sdk settings.
///
/// \return #NO_ERROR_OCCURRED if successful, otherwise a #MMMReaderErrorCode stating what
/// error occurred.
///
/// \remarks Although the default values chosen represent "sensible" values, some settings
/// within the configuration files are \pagereader calibration specific and so should be
/// tuned to each \pagereader specifically.
MMM_READER_LOW_LEVEL_API MMMReaderErrorCode MMMReader_LL_InitialiseSettings(
	MMMReaderSettings *aSettings
);

/// Commits the provided MMMReaderSettings structure to the \sdk configuration
/// files, thereby persisting the settings for future sessions to use.
///
/// \ingroup LLSettingsModule
///
/// \note This API is only relevant if using the \llapi. The \hlapi has an equivalent
/// API called MMMReader_SaveSettings().
///
/// \param[in] aSettings A pointer to the MMMReaderSettings structure to commit to the
/// configuration files.
///
/// \return #NO_ERROR_OCCURRED if successful, otherwise a #MMMReaderErrorCode stating what
/// error occurred.
///
/// \attention This API will overwrite the contents of the \sdk configuration files with the
/// the values in \a aSettings. Ensure that this is what is required before using this API.
///
/// \remarks For efficiency, only changes that have been made since the settings were loaded
/// will be saved, so manual edits made to the configuration files while the host application
/// is running will not be overridden by this API.
///
/// \sa MMMReader_LL_LoadSettings()
MMM_READER_LOW_LEVEL_API MMMReaderErrorCode MMMReader_LL_SaveSettings(
	MMMReaderSettings *aSettings
);

/// Reads the value of the specified setting from the supplied setting structure.
///
/// \ingroup LLSettingsModule
///
/// \note This API is only relevant if using the \llapi. The \hlapi has an equivalent
/// API called MMMReader_GetSettingValue().
///
/// \param[in] aTheSettings A pointer to the MMMReaderSettings structure to read the
/// setting value from.
///
/// \param[in] aSectionName The name of the section in which the setting is located.
///
/// \param[in] aSettingName The name of the setting.
///
/// \param[out] aSettingValue Pointer to memory where the value will be written.
/// It is assumed that the data type of the memory pointed to will be the correct
/// type for the setting requested.
///
/// \param[in/out] aSettingValueSize The size of the memory aSettingValue points to.
/// The value of this integer will be changed to specify the size of memory required
/// if aSettingValue is NULL or the size of memory used if aSettingValueSize was larger.
///
/// \return #NO_ERROR_OCCURRED if successful, otherwise a #MMMReaderErrorCode stating what
/// error occurred.
MMM_READER_LOW_LEVEL_API MMMReaderErrorCode MMMReader_LL_GetSettingValue(
	MMMReaderSettings *aTheSettings,
	const RTCHAR *aSectionName,
	const RTCHAR *aSettingName,
	void *aSettingValue,
	int *aSettingValueSize
);

/// \internal
MMM_READER_LOW_LEVEL_API MMMReaderErrorCode MMMReader_LL_SetRootIniFile(
	const RTCHAR* aRootIniFilePath
);

/// Gets the location of the \pagereader Mass Storage Device (MSD) file directory.
///
/// \ingroup LLSettingsModule
///
/// \note This API is only relevant if using the \llapi.
///
/// \return A null-terminated string which contains the folder location in question.
///
/// \note Only the \rte8000 and \rte8000hs have a Mass Storage Device. For all other
/// \pagereaders, the value returned will be the same as that from MMMReader_LL_GetExeDir().
///
/// \sa #MMMReaderSettings::puReaderDir
MMM_READER_LOW_LEVEL_API const RTCHAR* MMMReader_LL_GetReaderDir();

/// \endcond
// PAGESDK

/// Gets the location where the current host application is running from.
///
/// \ingroup LLSettingsModule
///
/// \if PAGESDK
/// \note This API is only relevant if using the \llapi.
/// \endif
///
/// \return A null-terminated string which contains the folder location in question.
///
/// \if PAGESDK
/// \sa #MMMReaderSettings::puExeDir
/// \endif
MMM_READER_LOW_LEVEL_API const RTCHAR* MMMReader_LL_GetExeDir();

/// Gets the location of the \sdk Binary ("Bin") folder.
///
/// \ingroup LLSettingsModule
///
/// \if PAGESDK
/// \note This API is only relevant if using the \llapi.
/// \endif
///
/// \return A null-terminated string which contains the folder location in question.
///
/// \remarks The value returned could be the same as that from MMMReader_LL_GetExeDir() but
/// not necessarily.
///
/// \if PAGESDK
/// \sa #MMMReaderSettings::puBinDir
/// \endif
MMM_READER_LOW_LEVEL_API const RTCHAR* MMMReader_LL_GetBinDir();

/// Gets the location of the \sdk Configuration ("Config") folder.
///
/// \ingroup LLSettingsModule
///
/// \if PAGESDK
/// \note This API is only relevant if using the \llapi.
/// \endif
///
/// \return A null-terminated string which contains the folder location in question.
///
/// \if PAGESDK
/// \sa #MMMReaderSettings::puCfgDir
/// \endif
MMM_READER_LOW_LEVEL_API const RTCHAR* MMMReader_LL_GetConfigDir();

/// \cond PAGESDK

/// Gets the location of the \sdk Plugins folder.
///
/// \ingroup LLSettingsModule
///
/// \note This API is only relevant if using the \llapi.
///
/// \return A null-terminated string which contains the folder location in question.
///
/// \sa #MMMReaderSettings::puPluginDir
MMM_READER_LOW_LEVEL_API const RTCHAR* MMMReader_LL_GetPluginDir();

/// Gets the location of the \sdk Data folder.
///
/// \ingroup LLSettingsModule
///
/// \note This API is only relevant if using the \llapi.
///
/// \return A null-terminated string which contains the folder location in question.
///
/// \sa #MMMReaderSettings::puDataDir
MMM_READER_LOW_LEVEL_API const RTCHAR* MMMReader_LL_GetDataDir();

/// Gets the location of the \sdk Log folder.
///
/// \ingroup LLSettingsModule
///
/// \note This API is only relevant if using the \llapi.
///
/// \return A null-terminated string which contains the folder location in question.
///
/// \sa #MMMReaderSettings::puLogDir
MMM_READER_LOW_LEVEL_API const RTCHAR* MMMReader_LL_GetLogDir();

// Gets the location of the \sdk Layout Database folder.
///
/// \ingroup LLSettingsModule
///
/// \note This API is only relevant if using the \llapi.
///
/// \return A null-terminated string which contains the folder location in question.
///
/// \sa #MMMReaderSettings::puLayoutDataBaseDir
MMM_READER_LOW_LEVEL_API const RTCHAR* MMMReader_LL_GetLayoutDatabaseDir();

/// \endcond
// PAGESDK

/// \cond PAGESDK

/// Loads Camera and Image Processing related settings from the \sdk configuration files.
///
/// \ingroup LLSettingsModule
///
/// \note This API is only relevant if using the \llapi.
///
/// \param[out] aCameraSettings A pointer to a CAMERASET structure which will be populated
/// with the \sdk settings loaded.
///
/// \param[out] aDocDetectSettings A pointer to a DOCDETECTSETTINGS structure which will be
/// populated with the \sdk settings loaded.
///
/// \param[out] aDocPosSettings A pointer to a DOCPOS structure which will be populated
/// with the \sdk settings loaded.
///
/// \param[out] aImageSettings A pointer to a IMGSET structure which will be populated
/// with the \sdk settings loaded.
///
/// \param[out] aPhotoSettings A pointer to a PHOTOSET structure which will be populated
/// with the \sdk settings loaded.
///
/// \param[out] aOcrSettings A pointer to a OCRSET structure which will be populated
/// with the \sdk settings loaded.
///
/// \param[out] aOcrUserSettings A pointer to a OCRUSERSET structure which will be populated
/// with the \sdk settings loaded.
///
/// \return #NO_ERROR_OCCURRED if successful, otherwise a #MMMReaderErrorCode stating what
/// error occurred.
///
/// \remarks This API is an alternative to MMMReader_LL_LoadSettings() if only the Camera
/// and Image Processing modules are required.
///
/// \sa MMMReader_LL_LoadSettings()
MMM_READER_LOW_LEVEL_API MMMReaderErrorCode MMMReader_LL_LoadCameraImageSettings(
	CAMERASET* aCameraSettings,
	DOCDETECTSETTINGS* aDocDetectSettings,
	DOCPOS* aDocPosSettings,
	IMGSET* aImageSettings,
	PHOTOSET* aPhotoSettings,
	OCRSET* aOcrSettings,
	OCRUSERSET* aOcrUserSettings,
	DIRTDETECTSET* aDirtDetectSettings
);

/// Loads Camera and Image Processing related settings from the \sdk configuration files with TwoSided docPosition.
///
/// \ingroup LLSettingsModule
///
/// \note This API is only relevant if using the \llapi.
///
/// \param[out] aCameraSettings A pointer to a CAMERASET structure which will be populated
/// with the \sdk settings loaded.
///
/// \param[out] aDocDetectSettings A pointer to a DOCDETECTSETTINGS structure which will be
/// populated with the \sdk settings loaded.
///
/// \param[out] aDocPosSettings A pointer to a DOCPOS structure which will be populated
/// with the \sdk settings loaded.
///
/// \param[out] aImageSettings A pointer to a IMGSET structure which will be populated
/// with the \sdk settings loaded.
///
/// \param[out] aPhotoSettings A pointer to a PHOTOSET structure which will be populated
/// with the \sdk settings loaded.
///
/// \param[out] aOcrSettings A pointer to a OCRSET structure which will be populated
/// with the \sdk settings loaded.
///
/// \param[out] aOcrUserSettings A pointer to a OCRUSERSET structure which will be populated
/// with the \sdk settings loaded.
///
/// \return #NO_ERROR_OCCURRED if successful, otherwise a #MMMReaderErrorCode stating what
/// error occurred.
///
/// \remarks This API is an alternative to MMMReader_LL_LoadSettings() if only the Camera
/// and Image Processing modules are required.
///
/// \sa MMMReader_LL_LoadSettings()
MMM_READER_LOW_LEVEL_API MMMReaderErrorCode MMMReader_LL_LoadCameraImageSettingsTwoSided(
	CAMERASET* aCameraSettings,
	DOCDETECTSETTINGS* aDocDetectSettings,
	DOCPOS* aDocPosSettings,
	DOCPOS* aDocPosSettingsSide2,
	IMGSET* aImageSettings,
	PHOTOSET* aPhotoSettings,
	OCRSET* aOcrSettings,
	OCRUSERSET* aOcrUserSettings,
	DIRTDETECTSET* aDirtDetectSettings
);

/// Loads Signalling related settings from the \sdk configuration files.
///
/// \ingroup LLSettingsModule
///
/// \note This API is only relevant if using the \llapi.
///
/// \param[out] aSoundSettings A pointer to a SoundSettings structure which will be populated
/// with the \sdk settings loaded.
///
/// \return #NO_ERROR_OCCURRED if successful, otherwise a #MMMReaderErrorCode stating what
/// error occurred.
///
/// \remarks This API is an alternative to MMMReader_LL_LoadSettings() if only the
/// Signalling module is required.
///
/// \sa MMMReader_LL_LoadSettings()
MMM_READER_LOW_LEVEL_API MMMReaderErrorCode MMMReader_LL_LoadSignalSettings(
	SoundSettings* aSoundSettings
);

/// Loads RF related settings from the \sdk configuration files.
///
/// \ingroup LLSettingsModule
///
/// \note This API is only relevant if using the \llapi.
///
/// \param[out] aRFSettings A pointer to a RFIDSettings structure which will be populated
/// with the \sdk settings loaded.
///
/// \return #NO_ERROR_OCCURRED if successful, otherwise a #MMMReaderErrorCode stating what
/// error occurred.
///
/// \remarks This API is an alternative to MMMReader_LL_LoadSettings() if only the
/// RFID module is required.
///
/// \sa MMMReader_LL_LoadSettings()
MMM_READER_LOW_LEVEL_API MMMReaderErrorCode MMMReader_LL_LoadRFSettings(
	RFIDSettings* aRFSettings
);

/// Loads Debug related settings from the \sdk configuration files.
///
/// \ingroup LLSettingsModule
///
/// \note This API is only relevant if using the \llapi.
///
/// \param[out] aDebugSettings A pointer to a IMGDEBUG structure which will be populated
/// with the \sdk settings loaded.
///
/// \return #NO_ERROR_OCCURRED if successful, otherwise a #MMMReaderErrorCode stating what
/// error occurred.
///
/// \remarks This API is an alternative to MMMReader_LL_LoadSettings() if only the
/// debug settings are required.
///
/// \sa MMMReader_LL_LoadSettings()
MMM_READER_LOW_LEVEL_API MMMReaderErrorCode MMMReader_LL_LoadDebugSettings(
	IMGDEBUG* aDebugSettings
);

/// Loads \hlapi Data To Send settings from the \sdk configuration files.
///
/// \ingroup LLSettingsModule
///
/// \note This API is only relevant if using the \llapi.
///
/// \param[out] aDataToSendSettings A pointer to a DATASENDSET structure which will be populated
/// with the \sdk settings loaded.
///
/// \return #NO_ERROR_OCCURRED if successful, otherwise a #MMMReaderErrorCode stating what
/// error occurred.
///
/// \remarks This API is an alternative to MMMReader_LL_LoadSettings() if only the
/// \hlapi Data To Send settings are required.
///
/// \sa MMMReader_LL_LoadSettings()
MMM_READER_LOW_LEVEL_API MMMReaderErrorCode MMMReader_LL_LoadDataToSendSettings(
	DATASENDSET* aDataToSendSettings
);

/// \endcond
// PAGESDK

/// Load \swipereader configuration settings from the \sdk configuration files.
///
/// \ingroup LLSettingsModule
///
/// \if PAGESDK
/// \note This API is only relevant if using the \llapi.
/// \endif
///
/// \param[out] aSwipeSettings A pointer to a MMMSwipeSettings structure which will be populated
/// with the \sdk settings loaded.
///
/// \return #NO_ERROR_OCCURRED if successful, otherwise a #MMMReaderErrorCode stating what
/// error occurred.
///
/// \if PAGESDK
///
/// \remarks This API is an alternative to MMMReader_LL_LoadSettings() if only a
/// \swipereader is used.
///
/// \sa MMMReader_LL_LoadSettings()
///
/// \endif
MMM_READER_LOW_LEVEL_API MMMReaderErrorCode MMMReader_LL_LoadSwipeSettings(
	MMMSwipeSettings* aSwipeSettings
);

/// Loads logging settings from the \sdk configuration files.
///
/// \ingroup LLSettingsModule
///
/// \if PAGESDK
/// \note This API is only relevant if using the \llapi.
/// \endif
///
/// \param[out] aLoggingSettings A pointer to a LoggingSettings structure which will be
/// populated with the \sdk settings loaded.
///
/// \return #NO_ERROR_OCCURRED if successful, otherwise a #MMMReaderErrorCode stating what
/// error occurred.
///
/// \if PAGESDK
///
/// \remarks This API is an alternative to MMMReader_LL_LoadSettings() if only the
/// logging settings are required.
///
/// \sa MMMReader_LL_LoadSettings()
///
/// \endif
MMM_READER_LOW_LEVEL_API MMMReaderErrorCode MMMReader_LL_LoadLoggingSettings(
	LoggingSettings* aLoggingSettings
);

/// \cond PAGESDK

/// Loads UHF related settings from the \sdk configuration files.
///
/// \ingroup LLSettingsModule
///
/// \note This API is only relevant if using the \llapi.
///
/// \param[out] aUHFSettings A pointer to a #UHFSettings structure which will be populated
/// with the \sdk settings loaded.
///
/// \return #NO_ERROR_OCCURRED if successful, otherwise a #MMMReaderErrorCode stating what
/// error occurred.
///
/// \remarks This API is an alternative to MMMReader_LL_LoadSettings() if only the
/// UHFmodule is required.
///
/// \sa MMMReader_LL_LoadSettings()
MMM_READER_LOW_LEVEL_API MMMReaderErrorCode MMMReader_LL_LoadUHFSettings(
	UHFSettings* aUHFSettings
);

/// \cond PAGESDK

/// Initialises Camera and Image Processing related settings with default values.
///
/// \ingroup LLSettingsModule
///
/// \note This API is only relevant if using the \llapi.
///
/// \param[out] aCameraSettings A pointer to a CAMERASET structure which will be populated
/// with the default \sdk settings.
///
/// \param[out] aDocDetectSettings A pointer to a DOCDETECTSETTINGS structure which will be
/// populated with the default \sdk settings.
///
/// \param[out] aDocPosSettings A pointer to a DOCPOS structure which will be populated
/// with the default \sdk settings.
///
/// \param[out] aImageSettings A pointer to a IMGSET structure which will be populated
/// with the default \sdk settings.
///
/// \param[out] aPhotoSettings A pointer to a PHOTOSET structure which will be populated
/// with the default \sdk settings.
///
/// \param[out] aOcrSettings A pointer to a OCRSET structure which will be populated
/// with the default \sdk settings.
///
/// \param[out] aOcrUserSettings A pointer to a OCRUSERSET structure which will be populated
/// with the default \sdk settings.
///
/// \return #NO_ERROR_OCCURRED if successful, otherwise a #MMMReaderErrorCode stating what
/// error occurred.
///
/// \remarks This API is an alternative to MMMReader_LL_InitialiseSettings() if only the Camera
/// and Image Processing modules are required.
///
/// \sa MMMReader_LL_InitialiseSettings()
MMM_READER_LOW_LEVEL_API MMMReaderErrorCode MMMReader_LL_InitialiseCameraImageSettings(
	CAMERASET* aCameraSettings,
	DOCDETECTSETTINGS* aDocDetectSettings,
	DOCPOS* aDocPosSettings,
	IMGSET* aImageSettings,
	PHOTOSET* aPhotoSettings,
	OCRSET* aOcrSettings,
	OCRUSERSET* aOcrUserSettings
);

/// Initialises Signalling related settings with default values.
///
/// \ingroup LLSettingsModule
///
/// \note This API is only relevant if using the \llapi.
///
/// \param[out] aSoundSettings A pointer to a SoundSettings structure which will be populated
/// with the default \sdk settings.
///
/// \return #NO_ERROR_OCCURRED if successful, otherwise a #MMMReaderErrorCode stating what
/// error occurred.
///
/// \remarks This API is an alternative to MMMReader_LL_InitialiseSettings() if only the
/// Signalling module is required.
///
/// \sa MMMReader_LL_InitialiseSettings()
MMM_READER_LOW_LEVEL_API MMMReaderErrorCode MMMReader_LL_InitialiseSignalSettings(
	SoundSettings* aSoundSettings
);

/// Initialises RF related settings with default values.
///
/// \ingroup LLSettingsModule
///
/// \note This API is only relevant if using the \llapi.
///
/// \param[out] aRFSettings A pointer to a RFIDSettings structure which will be populated
/// with the default \sdk settings.
///
/// \return #NO_ERROR_OCCURRED if successful, otherwise a #MMMReaderErrorCode stating what
/// error occurred.
///
/// \remarks This API is an alternative to MMMReader_LL_InitialiseSettings() if only the
/// RFID module is required.
///
/// \sa MMMReader_LL_InitialiseSettings()
MMM_READER_LOW_LEVEL_API MMMReaderErrorCode MMMReader_LL_InitialiseRFSettings(
	RFIDSettings* aRFSettings
);

/// Initialises debug settings with default values.
///
/// \ingroup LLSettingsModule
///
/// \note This API is only relevant if using the \llapi.
///
/// \param[out] aDebugSettings A pointer to a IMGDEBUG structure which will be populated
/// with the default \sdk settings.
///
/// \return #NO_ERROR_OCCURRED if successful, otherwise a #MMMReaderErrorCode stating what
/// error occurred.
///
/// \remarks This API is an alternative to MMMReader_LL_InitialiseSettings() if only debug
/// settings are required.
///
/// \sa MMMReader_LL_InitialiseSettings()
MMM_READER_LOW_LEVEL_API MMMReaderErrorCode MMMReader_LL_InitialiseDebugSettings(
	IMGDEBUG* aDebugSettings
);

/// Initialises \hlapi Data To Send settings with default values.
///
/// \ingroup LLSettingsModule
///
/// \note This API is only relevant if using the \llapi.
///
/// \param[out] aDataToSendSettings A pointer to a DATASENDSET structure which will be
/// populated with the default \sdk settings.
///
/// \return #NO_ERROR_OCCURRED if successful, otherwise a #MMMReaderErrorCode stating what
/// error occurred.
///
/// \remarks This API is an alternative to MMMReader_LL_InitialiseSettings() if only the
/// \hlapi Data To Send settings are required.
///
/// \sa MMMReader_LL_InitialiseSettings()
MMM_READER_LOW_LEVEL_API MMMReaderErrorCode MMMReader_LL_InitialiseDataToSendSettings(
	DATASENDSET* aDataToSendSettings
);

/// \endcond
// PAGESDK

/// Initialises \swipereader configuration settings with default values.
///
/// \ingroup LLSettingsModule
///
/// \if PAGESDK
/// \note This API is only relevant if using the \llapi.
/// \endif
///
/// \param[out] aSwipeSettings A pointer to a MMMSwipeSettings structure which will be populated
/// with the default \sdk settings.
///
/// \return #NO_ERROR_OCCURRED if successful, otherwise a #MMMReaderErrorCode stating what
/// error occurred.
///
/// \if PAGESDK
///
/// \remarks This API is an alternative to MMMReader_LL_InitialiseSettings() if only a
/// \swipereader is used.
///
/// \sa MMMReader_LL_InitialiseSettings()
///
/// \endif
MMM_READER_LOW_LEVEL_API MMMReaderErrorCode MMMReader_LL_InitialiseSwipeSettings(
	MMMSwipeSettings* aSwipeSettings
);

/// Initialises logging settings with default values.
///
/// \ingroup LLSettingsModule
///
/// \if PAGESDK
/// \note This API is only relevant if using the \llapi.
/// \endif
///
/// \param[out] aLoggingSettings A pointer to a LoggingSettings structure which will be
/// populated with the default \sdk settings.
///
/// \return #NO_ERROR_OCCURRED if successful, otherwise a #MMMReaderErrorCode stating what
/// error occurred.
///
/// \if PAGESDK
///
/// \remarks This API is an alternative to MMMReader_LL_InitialiseSettings() if only the
/// logging settings are required.
///
/// \sa MMMReader_LL_InitialiseSettings()
///
/// \endif
MMM_READER_LOW_LEVEL_API MMMReaderErrorCode MMMReader_LL_InitialiseLoggingSettings(
	LoggingSettings* aLoggingSettings
);

/// \cond PAGESDK

/// Initialises UHF related settings with default values.
///
/// \ingroup LLSettingsModule
///
/// \note This API is only relevant if using the \llapi.
///
/// \param[out] aUHFSettings A pointer to a UHFSettings structure which will be populated
/// with the default \sdk settings.
///
/// \return #NO_ERROR_OCCURRED if successful, otherwise a #MMMReaderErrorCode stating what
/// error occurred.
///
/// \remarks This API is an alternative to MMMReader_LL_InitialiseSettings() if only the
/// UHF module is required.
///
/// \sa MMMReader_LL_InitialiseSettings() MMMReader_LL_LoadUHFSettings()
MMM_READER_LOW_LEVEL_API MMMReaderErrorCode MMMReader_LL_InitialiseUHFSettings(
	UHFSettings* aUHFSettings
);

/// \cond PAGESDK

/// Saves Camera and Image Processing related settings to the \sdk configuration files.
///
/// \ingroup LLSettingsModule
///
/// \note This API is only relevant if using the \llapi.
///
/// \param[in] aCameraSettings A pointer to a CAMERASET structure which contains the settings
/// to save.
///
/// \param[in] aDocDetectSettings A pointer to a DOCDETECTSETTINGS structure which contains
/// the settings to save.
///
/// \param[in] aDocPosSettings A pointer to a DOCPOS structure which contains the settings
/// to save.
///
/// \param[in] aImageSettings A pointer to a IMGSET structure which contains the settings
/// to save.
///
/// \param[in] aPhotoSettings A pointer to a PHOTOSET structure which contains the settings
/// to save.
///
/// \param[in] aOcrSettings A pointer to a OCRSET structure which contains the settings
/// to save.
///
/// \param[in] aOcrUserSettings A pointer to a OCRUSERSET structure which contains the settings
/// to save.
///
/// \return #NO_ERROR_OCCURRED if successful, otherwise a #MMMReaderErrorCode stating what
/// error occurred.
///
/// \remarks This API is an alternative to MMMReader_LL_SaveSettings() if only the Camera
/// and Image Processing modules are required.
///
/// \sa MMMReader_LL_SaveSettings()
MMM_READER_LOW_LEVEL_API MMMReaderErrorCode MMMReader_LL_SaveCameraImageSettings(
	CAMERASET* aCameraSettings,
	DOCDETECTSETTINGS* aDocDetectSettings,
	DOCPOS* aDocPosSettings,
	IMGSET* aImageSettings,
	PHOTOSET* aPhotoSettings,
	OCRSET* aOcrSettings,
	OCRUSERSET* aOcrUserSettings
);

/// Saves Signalling related settings to the \sdk configuration files.
///
/// \ingroup LLSettingsModule
///
/// \note This API is only relevant if using the \llapi.
///
/// \param[in] aSoundSettings A pointer to a SoundSettings structure which contains the settings
/// to save.
///
/// \return #NO_ERROR_OCCURRED if successful, otherwise a #MMMReaderErrorCode stating what
/// error occurred.
///
/// \remarks This API is an alternative to MMMReader_LL_SaveSettings() if only the
/// Signalling module is required.
///
/// \sa MMMReader_LL_SaveSettings()
MMM_READER_LOW_LEVEL_API MMMReaderErrorCode MMMReader_LL_SaveSignalSettings(
	SoundSettings* aSoundSettings
);

/// Saves RF related settings to the \sdk configuration files.
///
/// \ingroup LLSettingsModule
///
/// \note This API is only relevant if using the \llapi.
///
/// \param[in] aRFSettings A pointer to a RFIDSettings structure which contains the settings
/// to save.
///
/// \return #NO_ERROR_OCCURRED if successful, otherwise a #MMMReaderErrorCode stating what
/// error occurred.
///
/// \remarks This API is an alternative to MMMReader_LL_SaveSettings() if only the
/// RFID module is required.
///
/// \sa MMMReader_LL_SaveSettings()
MMM_READER_LOW_LEVEL_API MMMReaderErrorCode MMMReader_LL_SaveRFSettings(
	RFIDSettings* aRFSettings
);

/// Saves debug related settings to the \sdk configuration files.
///
/// \ingroup LLSettingsModule
///
/// \note This API is only relevant if using the \llapi.
///
/// \param[in] aDebugSettings A pointer to a IMGDEBUG structure which contains the settings
/// to save.
///
/// \return #NO_ERROR_OCCURRED if successful, otherwise a #MMMReaderErrorCode stating what
/// error occurred.
///
/// \remarks This API is an alternative to MMMReader_LL_SaveSettings() if only the
/// debug settings are required.
///
/// \sa MMMReader_LL_SaveSettings()
MMM_READER_LOW_LEVEL_API MMMReaderErrorCode MMMReader_LL_SaveDebugSettings(
	IMGDEBUG* aDebugSettings
);

/// Saves \hlapi Data To Send settings to the \sdk configuration files.
///
/// \ingroup LLSettingsModule
///
/// \note This API is only relevant if using the \llapi.
///
/// \param[in] aDataToSendSettings A pointer to a DATASENDSET structure which contains the
/// settings to save.
///
/// \return #NO_ERROR_OCCURRED if successful, otherwise a #MMMReaderErrorCode stating what
/// error occurred.
///
/// \remarks This API is an alternative to MMMReader_LL_SaveSettings() if only the
/// \hlapi Data To Send settings are required.
///
/// \sa MMMReader_LL_SaveSettings()
MMM_READER_LOW_LEVEL_API MMMReaderErrorCode MMMReader_LL_SaveDataToSendSettings(
	DATASENDSET* aDataToSendSettings
);

/// \endcond
// PAGESDK

/// Saves \swipereader configuration settings to the \sdk configuration files.
///
/// \ingroup LLSettingsModule
///
/// \if PAGESDK
/// \note This API is only relevant if using the \llapi.
/// \endif
///
/// \param[in] aSwipeSettings A pointer to a MMMSwipeSettings structure which contains the
/// settings to save.
///
/// \return #NO_ERROR_OCCURRED if successful, otherwise a #MMMReaderErrorCode stating what
/// error occurred.
///
/// \if PAGESDK
///
/// \remarks This API is an alternative to MMMReader_LL_SaveSettings() if only a
/// \swipereader is used.
///
/// \sa MMMReader_LL_SaveSettings()
///
/// \endif
MMM_READER_LOW_LEVEL_API MMMReaderErrorCode MMMReader_LL_SaveSwipeSettings(
	MMMSwipeSettings* aSwipeSettings
);

/// Saves logging settings to the \sdk configuration files.
///
/// \ingroup LLSettingsModule
///
/// \if PAGESDK
/// \note This API is only relevant if using the \llapi.
/// \endif
///
/// \param[in] aLoggingSettings A pointer to a LoggingSettings structure which contains the
/// settings to save.
///
/// \return #NO_ERROR_OCCURRED if successful, otherwise a #MMMReaderErrorCode stating what
/// error occurred.
///
/// \if PAGESDK
///
/// \remarks This API is an alternative to MMMReader_LL_SaveSettings() if only the
/// logging settings are required.
///
/// \sa MMMReader_LL_SaveSettings()
///
/// \endif
MMM_READER_LOW_LEVEL_API MMMReaderErrorCode MMMReader_LL_SaveLoggingSettings(
	LoggingSettings* aLoggingSettings
);

/// \cond PAGESDK

/// Saves UHF related settings to the \sdk configuration files.
///
/// \ingroup LLSettingsModule
///
/// \note This API is only relevant if using the \llapi.
///
/// \param[in] aUHFSettings A pointer to a UHFSettings structure which contains the settings
/// to save.
///
/// \return #NO_ERROR_OCCURRED if successful, otherwise a #MMMReaderErrorCode stating what
/// error occurred.
///
/// \remarks This API is an alternative to MMMReader_LL_SaveSettings() if only the
/// UHF module is required.
///
/// \sa MMMReader_LL_SaveSettings()
MMM_READER_LOW_LEVEL_API MMMReaderErrorCode MMMReader_LL_SaveUHFSettings(
	UHFSettings* aUHFSettings
);

///////////  Version APIs

/// Gets the file version number of a \sdk module.
///
/// \if SWIPESDK
/// \ingroup SwipeMiscModule
/// \else
/// \ingroup LowLevelModule
/// \endif
///
/// \if PAGESDK
/// \note This API is only relevant if using the \llapi.
/// \endif
///
/// \param[in] aModule A null-terminated string providing the file name of the module
/// to get information from.
///
/// \param[out] aVersionStr A pointer to a writable string buffer which will hold the
/// result. This can be \c NULL; see the remarks section below for details.
///
/// \param[in,out] aVersionLen Initially will contain the length of the \a aVersionStr
/// buffer. When the function returns, this value will be the actual length of the string.
///
/// \return #NO_ERROR_OCCURRED if successful, otherwise a #MMMReaderErrorCode stating what
/// error occurred.
///
/// \remarks This API should be called twice; the first time, \a aVersionStr should be
/// \c NULL and \a aVersionLen should be zero. This will provide the correct length
/// of the \a aVersionStr buffer via \a aVersionLen. The second time should then
/// pass a correctly allocated string buffer to contain the results.
///
/// \remarks Only the file name of the module should be provided in \a aModule; it will
/// automatically be located in the same directory as the other \sdk modules.
///
/// \sa MMMReader_LL_GetMMMReaderVersions()
MMM_READER_LOW_LEVEL_API MMMReaderErrorCode MMMReader_LL_GetFileVersion(
	RTCHAR *aModule,
	RTCHAR *aVersionStr,
	int *aVersionLen
);

/// Gets the file version information of all \sdk components.
///
/// \if SWIPESDK
/// \ingroup SwipeMiscModule
/// \else
/// \ingroup LowLevelModule
/// \endif
///
/// \if PAGESDK
/// \note This API is only relevant if using the \llapi.
/// \endif
///
/// \param[out] aVersionStr A pointer to a writable string buffer which will hold the
/// result. This can be \c NULL; see the remarks section below for details.
///
/// \param[in,out] aVersionLen Initially will contain the length of the \a aVersionStr
/// buffer. When the function returns, this value will be the actual length of the string.
///
/// \return #NO_ERROR_OCCURRED if successful, otherwise a #MMMReaderErrorCode stating what
/// error occurred.
///
/// \remarks This API should be called twice; the first time, \a aVersionStr should be
/// \c NULL and \a aVersionLen should be zero. This will provide the correct length
/// of the \a aVersionStr buffer via \a aVersionLen. The second time should then
/// pass a correctly allocated string buffer to contain the results.
///
/// \remarks \a aVersionStr will be updated to contain a string in a format that can be
/// easily output to provide information to \ssd. The files listed are those located in the
/// same directory as the \sdk modules.
///
/// \sa MMMReader_LL_GetFileVersion()
MMM_READER_LOW_LEVEL_API MMMReaderErrorCode MMMReader_LL_GetMMMReaderVersions(
	RTCHAR *aVersionStr,
	int *aVersionLen
);

/// \cond PAGESDK

/// Gets the serial number of a \pagereader.
///
/// \ingroup LowLevelModule
///
/// \note This API is only relevant if using a \pagereader with the \llapi.
///
/// \param[in] aReaderDir A null-terminated string providing the directory path of the
/// \sdk Reader folder. See below for more details.
///
/// \param[out] aSerialStr A pointer to a writable string buffer which will hold the
/// result. This can be \c NULL; see the remarks section below for details.
///
/// \param[in,out] aSerialLen Initially will contain the length of the \a aSerialStr
/// buffer. When the function returns, this value will be the actual length of the string.
///
/// \return #NO_ERROR_OCCURRED if successful, otherwise a #MMMReaderErrorCode stating what
/// error occurred.
///
/// \remarks This API should be called twice; the first time, \a aSerialStr should be
/// \c NULL and \a aSerialLen should be zero. This will provide the correct length
/// of the \a aSerialStr buffer via \a aSerialLen. The second time should then
/// pass a correctly allocated string buffer to contain the results.
///
/// \remarks \a aReaderDir should be the same value as provided by the
/// #MMMReaderSettings::puReaderDir value (via the MMMReader_LL_LoadSettings() API) or
/// from the MMMReader_LL_GetReaderDir() API.
///
/// \sa MMMReader_LL_GetCameraSerialNumber()
MMM_READER_LOW_LEVEL_API MMMReaderErrorCode MMMReader_LL_GetSerialNumber(
	RTCHAR *aReaderDir,
	RTCHAR *aSerialStr,
	int *aSerialLen
);

/// Gets the serial number of the \pagereader camera.
///
/// \ingroup LowLevelModule
///
/// \note This API is only relevant if using a \pagereader with the \llapi.
///
/// \param[out] aSerialNumber A pointer to an integer which will be updated with the
/// camera serial number.
///
/// \return #NO_ERROR_OCCURRED if successful, otherwise a #MMMReaderErrorCode stating what
/// error occurred.
///
/// \sa MMMReader_LL_GetSerialNumber()
MMM_READER_LOW_LEVEL_API MMMReaderErrorCode MMMReader_LL_GetCameraSerialNumber(
	int *aSerialNumber
);

/// \endcond
// PAGESDK

/// \cond INTERNAL

/// Gets the API version number of the \llapi.
///
/// \if SWIPESDK
/// \ingroup SwipeMiscModule
/// \else
/// \ingroup LowLevelModule
/// \endif
///
/// \if PAGESDK
/// \note This API is only relevant if using the \llapi.
/// \endif
///
/// \return The current version of the \llapi.
///
/// \remarks The API version will not be incremented for every release but only with releases
/// which alter the arguments of existing APIs or make other changes which will force the
/// host application to be recompiled.
///
MMM_READER_LOW_LEVEL_API int MMMReader_LL_GetAPIVersion();

/// \endcond

/// Gets the date and time a \sdk module was compiled.
///
/// \if SWIPESDK
/// \ingroup SwipeMiscModule
/// \else
/// \ingroup LowLevelModule
/// \endif
///
/// \if PAGESDK
/// \note This API is only relevant if using the \llapi.
/// \endif
///
/// \param[in] aModule A null-terminated string providing the file name of the module
/// to get information from.
///
/// \param[out] aDate A pointer to a writable string buffer which will hold the
/// date result. This can be \c NULL; see the remarks section below for details.
///
/// \param[in,out] aDateLength Initially will contain the length of the \a aDate
/// buffer. When the function returns, this value will be the actual length of the string.
///
/// \param[out] aTime A pointer to a writable string buffer which will hold the
/// time result. This can be \c NULL; see the remarks section below for details.
///
/// \param[in,out] aTimeLength Initially will contain the length of the \a aTime
/// buffer. When the function returns, this value will be the actual length of the string.
///
/// \return #NO_ERROR_OCCURRED if successful, otherwise a #MMMReaderErrorCode stating what
/// error occurred.
///
/// \remarks This API can be used as a more reliable timestamp than the file timestamp.
///
/// \remarks This API should be called twice; the first time, \a aDate and \a aTime should be
/// \c NULL and \a aDateLength and \a aTimeLength should be zero. This will provide the
/// correct length of the buffers via their length parameters. The second time should then
/// pass a correctly allocated string buffers to contain the results.
///
/// \remarks Only the file name of the module should be provided in \a aModule; it will
/// automatically be located in the same directory as the other \sdk modules.
///
/// \sa MMMReader_LL_GetModuleInfo()
MMM_READER_LOW_LEVEL_API MMMReaderErrorCode MMMReader_LL_GetLastCompile(
	RTCHAR* aModule,
	RTCHAR* aDate,
	int* aDateLength,
	RTCHAR* aTime,
	int* aTimeLength
);

/// Extracts information from the \c VersionInfo resource of a \sdk module.
///
/// \if SWIPESDK
/// \ingroup SwipeMiscModule
/// \else
/// \ingroup LowLevelModule
/// \endif
///
/// \if PAGESDK
/// \note This API is only relevant if using the \llapi.
/// \endif
///
/// \param[in] aModule A null-terminated string providing the file name of the module
/// to get information from.
///
/// \param[in] aQuery A null-terminated string describing the information to be extracted.
/// See the remarks section below for more details.
///
/// \param[out] aInfo A pointer to a writable string buffer which will hold the
/// result. This can be \c NULL; see the remarks section below for details.
///
/// \param[in,out] aInfoLength Initially will contain the length of the \a aInfo
/// buffer. When the function returns, this value will be the actual length of the string.
///
/// \return #NO_ERROR_OCCURRED if successful, otherwise a #MMMReaderErrorCode stating what
/// error occurred.
///
/// \remarks This API should be called twice; the first time, \a aInfo should be
/// \c NULL and \a aInfoLength should be zero. This will provide the correct length
/// of the \a aInfo buffer via \a aInfoLength. The second time should then
/// pass a correctly allocated string buffer to contain the results.
///
/// \remarks \a aQuery can be any of the following:
///		- \c "OriginalFilename"
///		- \c "ProductName"
///		- \c "CompanyName"
///		- \c "FileVersion"
///		- \c "ProductVersion"
///		- \c "FileDescription"
///
/// \sa MMMReader_LL_GetLastCompile()
MMM_READER_LOW_LEVEL_API MMMReaderErrorCode MMMReader_LL_GetModuleInfo(
	RTCHAR* aModule,
	RTCHAR* aQuery,
	RTCHAR* aInfo,
	int* aInfoLength
);


///////////// QA

/// \cond PAGESDK

/// Performs Optical Character Recognition (OCR) on a document image to read the Machine
/// Readable Zone (MRZ) codeline from travel documents. This API performs this operation
/// in Quality Assurance (QA) mode and collects further measurements about the codeline.
///
/// \ingroup QaModule
///
/// \note This API is only relevant if using a \pagereader with the \llapi.
///
/// \param[in] aBlocking Specify whether this operation should be blocking or non-blocking;
/// if \c false, then this API will add a command to the queue to be carried out at a later
/// time and return immediately.
///
/// \param[in] aInputImage A GDI+ Bitmap object which is the source image to read from.
///
/// \param[in] aOCRSettings A pointer to a OCRSET structure which should contain the
/// required settings for the \pagereader.
///
/// \param[in] aOCRUserSettings A pointer to a OCRUSERSET structure which should contain the
/// required settings for the \pagereader.
///
/// \param[in] aDocPosition \todo Find out what this does
///
/// \param[in] aDataCallback A user-defined #MMMReaderDataCallback function which will receive
/// the QA measurements as a QaCodelineMeasurements structure.
///
/// \param[in] aParam An optional pointer to some user-defined data. This is passed on to the
/// callback provided.
///
/// \param[in] aSequenceNum A user-defined value that identifies what data is coming back
/// through the \a aDataCallback.
///
/// \param[in] aEvent If \a aBlocking is \c true, pass in a handle to an OS event which will be
/// set once the operation finishes.
///
/// \param[in] aImgSettings A pointer to a IMGSET structure which should contain the
/// required settings for the \pagereader.
///
/// \return #NO_ERROR_OCCURRED if successful, otherwise a #MMMReaderErrorCode stating what
/// error occurred.
///
/// \attention This API is generally not intended for use by customers. Customers wishing to
/// use QA functionality should generally use the specific QA API. Contact \ssd for more
/// details.
///
/// \remarks \a aParam can be used to provide some context data back through the callbacks. One
/// example could be the \c this pointer from a C++ class; in this case, because the callback
/// functions must be C-style functions, \a aParam would be able to access elements of its
/// class type.
///
/// \remarks \a aSequenceNum is a user-defined value which is passed straight into
/// \a aDataCallback. This parameter is provided so that it is possible to have one data
/// callback defined for multiple data items; the defined function can then filter the
/// \a aSequenceNum as appropriate.
///
/// \remarks Typically, the required settings structures are taken from #MMMReaderSettings by
/// using MMMReader_LL_LoadSettings().
///
/// \sa MMMReader_ImageReadCodeline()
#ifdef ENABLE_GDIPLUS
MMM_READER_LOW_LEVEL_API MMMReaderErrorCode MMMReader_CollectQAMeasurements(
	bool aBlocking,
	Gdiplus::Bitmap *aInputImage,
	OCRSET *aOCRSettings,
	OCRUSERSET *aOCRUserSettings,
	Box *aDocPosition,
	MMMReaderDataCallback aDataCallback,
	void *aParam,
	int aSequenceNum,
	HANDLE aEvent,
	IMGSET *aImgSettings
);
#endif // ENABLE_GDIPLUS

/*
/// Confirms that the codeline had no unreadable characters, and is of the expected format
MMM_READER_LOW_LEVEL_API MMMReaderErrorCode MMMReader_QAValidateCodeline(char *aCodeline, OCRUserSet *aOCRSettings,
																  int *aCodelineErrorCode, char *aCodelineErrorStr, int *aCodelineErrorStrLen);


/// Confirms that the codeline is positioned correctly within the document
MMM_READER_LOW_LEVEL_API MMMReaderErrorCode MMMReader_QAValidateCodelinePosition(QaCodelineMeasurements *aQaInfo, QASettings *aSettings,
																		  int *aCodelineErrorCode, char *aCodelineErrorStr, int *aCodelineErrorStrLen);

*/

/// \endcond
// PAGESDK

//////////// Image Processing Plugins

/// \cond PAGESDK

/// Loads and initialises a \sdk plugin.
///
/// \ingroup PluginsModule
///
/// \note This API is only relevant if using a \pagereader with the \llapi.
///
/// \param[in] aFilename A null-terminated string providing the full file path of the
/// plugin to load.
///
/// \param[in] aINIFileDir A null-terminated string providing the path to the plugin's
/// configuration file (if present). See the remarks section below for more details.
///
/// \param[out] aDecoderHandle A pointer which will be updated to be the library handle to
/// the loaded plugin.
///
/// \param[in] aMSDPath A null-terminated string providing the path where \pagereader settings
/// are stored. See the remarks section below for more details.
///
/// \return #NO_ERROR_OCCURRED if successful, otherwise a #MMMReaderErrorCode stating what
/// error occurred.
///
/// \attention The MMMReader_ImagePluginUnLoad() API must be called when finished with the
/// plugin referred to by \a aDecoderHandler to free all resources allocated.
///
/// \remarks All plugin libraries used by the \sdk follow the file name convention
/// <tt>RTDECODE_[name].dll</tt> where <tt>[name]</tt> is the name of the plugin. Some plugins
/// use configuration files, which are located in the folder given by \a aINIFileDir, and
/// follow the file name convention <tt>\#RTDECODE_[name].ini</tt>.
///
/// \remarks Typical file paths to use are:
///		- \a aINIFileDir - the #MMMReaderSettings::puCfgDir value, or use the
///		MMMReader_LL_GetConfigDir() API.
///		- \a aMSDPath - the #MMMReaderSettings::puReaderDir value, or use the
///		MMMReader_LL_GetReaderDir().
///
/// \remarks Use the returned \a aDecoderHandle for all image plugin APIs.
///
/// \sa MMMReader_ImagePluginUnLoad(), MMMReader_ImagePluginDecode()
MMM_READER_LOW_LEVEL_API MMMReaderErrorCode MMMReader_ImagePluginLoad(
	RTCHAR *aFilename,
	RTCHAR *aINIFileDir,
	HANDLE *aDecoderHandle,
	RTCHAR *aMSDPath
);

/// Unloads a \sdk plugin and frees all related resources.
///
/// \ingroup PluginsModule
///
/// \note This API is only relevant if using a \pagereader with the \llapi.
///
/// \param[in] aDecoderHandle The handle to the plugin to unload.
///
/// \return #NO_ERROR_OCCURRED if successful, otherwise a #MMMReaderErrorCode stating what
/// error occurred.
///
/// \remarks \a aDecoderHandler must be a valid pointer that was returned via the
/// MMMReader_ImagePluginLoad() API.
///
/// \attention Once the plugin has been unloaded, \a aDecoderHandle becomes invalid and
/// should not be used again.
///
/// \sa MMMReader_ImagePluginLoad()
MMM_READER_LOW_LEVEL_API MMMReaderErrorCode MMMReader_ImagePluginUnLoad(
	HANDLE aDecoderHandle
);

/// Uses a \sdk plugin to decode information from an image.
///
/// \ingroup PluginsModule
///
/// \note This API is only relevant if using a \pagereader with the \llapi.
///
/// \param[in] aBlocking Specify whether this operation should be blocking or non-blocking;
/// if \c false, then this API will add a command to the queue to be carried out at a later
/// time and return immediately.
///
/// \param[in] aDecoderHandle The handle to the plugin to use.
///
/// \param[in] aInputImage A GDI+ Bitmap object which is the source image to read from.
///
/// \param[in] aDocumentCodeline A null-terminated string providing codeline as read from the
/// MMMReader_ImageReadCodeline() API.
///
/// \param[in] aImageId The current image ID in use. See the remarks section below for more
/// details.
///
/// \param[in] aDecoderInfoName A null-terminated string providing a specific decoder
/// information set to use if the plugin supports more than one data type (e.g. multiple
/// barcode symbologies). This can be an empty string if not required.
///
/// \param[in] aDataCallback A user-defined #MMMReaderDataCallback function which will receive
/// the result as a MMMReaderPluginData structure.
///
/// \param[in] aParam An optional pointer to some user-defined data. This is passed on to the
/// callback provided.
///
/// \param[in] aSequenceNum A user-defined value that identifies what data is coming back
/// through the \a aDataCallback.
///
/// \param[in] aEvent If \a aBlocking is \c true, pass in a handle to an OS event which will be
/// set once the operation finishes.
///
/// \param[in] aSkipCallback A pointer to a user-defined #MMMReaderSkipPluginCallback function
/// which determines whether the decode operation should continue. This can be \c NULL.
///
/// \return #NO_ERROR_OCCURRED if successful, otherwise a #MMMReaderErrorCode stating what
/// error occurred.
///
/// \remarks \a aParam can be used to provide some context data back through the callbacks. One
/// example could be the \c this pointer from a C++ class; in this case, because the callback
/// functions must be C-style functions, \a aParam would be able to access elements of its
/// class type.
///
/// \remarks \a aSequenceNum is a user-defined value which is passed straight into
/// \a aDataCallback. This parameter is provided so that it is possible to have one data
/// callback defined for multiple data items; the defined function can then filter the
/// \a aSequenceNum as appropriate.
///
/// \remarks Some \sdk plugins use a shared library for keeping track of decoded information.
/// The \a aImageId parameter is used by plugins to check whether the same image is being
/// decoded by another decoder. If \a aImageId is the same since the last call to this API,
/// then the shared decoded information can be retrieved and used again, speeding up
/// decoding performance. Update the \a aImageId value for each new image to refresh the
/// shared decoded information, e.g. increment \a aImageId by 1 each time.
///
/// \remarks If the image contains multiple features (e.g. more than 1 barcode) or multiple
/// parts per feature, then multiple MMMReaderPluginData structures will be returned through
/// \a aDataCalback. See MMMReaderPluginData for an explanation on plugin features and parts.
///
/// \remarks The \a aSkipCallback can be optionally provided. If provided, it will be invoked
/// immediately before decoding \a aInputImage to check that the operation should continue.
/// This can be useful if the host application has determined it is no longer necessary
/// to perform the decode operation, for example if another plugin has already successfully
/// decoded the image.
///
/// \sa MMMReader_ImagePluginLoad(), MMMReaderPluginData
#ifdef ENABLE_GDIPLUS
MMM_READER_LOW_LEVEL_API MMMReaderErrorCode MMMReader_ImagePluginDecode(
	bool aBlocking,
	HANDLE aDecoderHandle,
	Gdiplus::Bitmap *aInputImage,
	char *aDocumentCodeline,
	int aImageId,
	char *aDecoderInfoName,
	MMMReaderDataCallback aDataCallback,
	void *aParam,
	int aSequenceNum,
	HANDLE aEvent,
	MMMReaderSkipPluginCallback aSkipCallback
);
#endif // ENABLE_GDIPLUS

/// Gets the image settings required by a \sdk plugin.
///
/// \ingroup PluginsModule
///
/// \note This API is only relevant if using a \pagereader with the \llapi.
///
/// \param[in] aDecoderHandle The handle to the plugin to use.
///
/// \param[out] aImageSettings A pointer to a MMMReaderImageSettings which will be updated to
/// contain the image setting values required by the plugin.
///
/// \param[in] aIndex Specify which image definition to get settings for. See the remarks
/// section below for more details.
///
/// \return #NO_ERROR_OCCURRED if successful, otherwise a #MMMReaderErrorCode stating what
/// error occurred.
///
/// \remarks To obtain all image definitions from a plugin, call this API repeatedly with
/// increasing values of \a aIndex (starting from zero) until an error is returned.
///
/// \sa MMMReader_ImagePluginGetRequiredImages()
MMM_READER_LOW_LEVEL_API MMMReaderErrorCode MMMReader_ImagePluginGetImageSettings(
	HANDLE aDecoderHandle,
	MMMReaderImageSettings *aImageSettings,
	int aIndex
);

/// Gets the image types that are required by a \sdk plugin.
///
/// \ingroup PluginsModule
///
/// \note This API is only relevant if using a \pagereader with the \llapi.
///
/// \param[in] aDecoderHandle The handle to the plugin to use.
///
/// \param[in] aDecoderInfoName A null-terminated string providing a specific decoder
/// information set to use if the plugin supports more than one data type (e.g. multiple
/// barcode symbologies). This can be an empty string if not required.
///
/// \param[out] aImageNames A pointer to a writable string buffer which will hold the
/// result. This can be \c NULL; see the remarks section below for details.
///
/// \param[in,out] aLen Initially will contain the length of the \a aImageNames
/// buffer. When the function returns, this value will be the actual length of the string.
///
/// \return #NO_ERROR_OCCURRED if successful, otherwise a #MMMReaderErrorCode stating what
/// error occurred.
///
/// \remarks This API should be called twice; the first time, \a aImageNames should be
/// \c NULL and \a aLen should be zero. This will provide the correct length
/// of the \a aImageNames buffer via \a aLen. The second time should then
/// pass a correctly allocated string buffer to contain the results.
///
/// \remarks \a aImageNames will contain a comma separated list of image names. These refer
/// to the standard \sdk images used:
///		- \c IMAGE_IR
///		- \c IMAGE_VIS
///		- \c IMAGE_UV
///		- \c IMAGE_COAX
///		- \c IMAGE_COAXIR
///
/// \remarks Alternatively, an image name could be defined as one of the image definitions
/// returned from MMMReader_ImagePluginGetImageSettings().
///
/// \sa MMMReader_ImagePluginGetImageSettings()
MMM_READER_LOW_LEVEL_API MMMReaderErrorCode MMMReader_ImagePluginGetRequiredImages(
	HANDLE aDecoderHandle,
	char *aDecoderInfoName,
	char *aImageNames,
	int aLen
);

/// \endcond
// PAGESDK

/////////// RFID APIs

/// \cond PAGESDK

/// Initialises the RFID module and allocates resources required for RF read operations.
///
/// \ingroup RfidModule
///
/// \note This API is only relevant if using the \llapi.
///
/// \param[in] aTheSettings A pointer to the RFIDSettings structure which contains all the
/// necessary RFID settings to use.
///
/// \param[in] aBACStringCallback A pointer to a user-defined #MMMReaderBACCallback
/// function which is invoked when a Basic Access Control (BAC) string is required. This
/// can be \c NULL. See the remarks section below for more details.
///
/// \param[in] aRFProgressCallback A pointer to a user-defined #MMMReaderDataCallback which
/// will receive progress reports when the RF chip is being read. This can be \c NULL. See
/// the remarks section below for more details.
///
/// \param[in] aMRZDataRequiredFieldsOnly Specifies whether all Basic Access Control (BAC)
/// keys are the full codeline or just the 3 required fields concatenated together.
///
/// \param[in] aStartSuspended Specify whether the RFID device is left in a suspended state
/// after initialisation.
///
/// \param[in] aParam An optional pointer to some user-defined data. This is passed on to all
/// callbacks that will be provided.
///
/// \return #NO_ERROR_OCCURRED if successful, otherwise a #MMMReaderErrorCode stating what
/// error occurred.
///
/// \attention The MMMReader_RFShutdown() API must be called when finished with the RFID device
/// to free all resources allocated.
///
/// \remarks \a aParam can be used to provide some context data back through the callbacks. One
/// example could be the \c this pointer from a C++ class; in this case, because the callback
/// functions must be C-style functions, \a aParam would be able to access elements of its
/// class type. \a aParam will be passed to the #MMMReaderCertificateCallback and
/// #MMMReaderSignRequestCallback functions, set by MMMReader_RFSetCertificateCallback() and
/// MMMReader_RFSetSignRequestCallback() APIs respectively.
///
/// \remarks The \a aStartSuspended flag can be used to allow another application to use
/// the RFID device.
///
/// \remarks The \a aBACStringCallback is optional. If provided, it will be invoked when a
/// Basic Access Control (BAC) key is required. If not provided, the BAC key must be provided
/// via MMMReader_RFOpen(). See #MMMReaderBACCallback for more details.
///
/// \remarks The \a aRFProgressCallback is optional and can be used to receive progress updates
/// on the current RF chip reading operation. The host application should avoid performing
/// any significant processing within the callback function to avoid slowing down
/// communications.
///
/// \sa MMMReader_RFShutdown(), MMMReader_IsRFInitialised(), MMMReader_IsRFSuspended(),
/// MMMReader_RFOpen()
MMM_READER_LOW_LEVEL_API MMMReaderErrorCode MMMReader_RFInitialise(
	RFIDSettings *aTheSettings,
	MMMReaderBACCallback aBACStringCallback,
	MMMReaderDataCallback aRFProgressCallback,
	bool aMRZDataRequiredFieldsOnly,
	bool aStartSuspended,
	void *aParam
);

/// Shuts down the RFID module and frees all related resources.
///
/// \ingroup RfidModule
///
/// \note This API is only relevant if using the \llapi.
///
/// \return #NO_ERROR_OCCURRED if successful, otherwise a #MMMReaderErrorCode stating what
/// error occurred.
///
/// \sa MMMReader_RFInitialise(), MMMReader_IsRFInitialised()
MMM_READER_LOW_LEVEL_API MMMReaderErrorCode MMMReader_RFShutdown();

/// Switches off the RF antenna. On readers that cannot use the RFID and camera at the same time,
/// it is important to call this when the RF read process has been completed to enable the camera
/// to be used again.
///
/// \ingroup RfidModule
///
/// \note This API is only relevant if using the \llapi.
///
/// \param[in] aBlocking Specify whether this operation should be blocking or non-blocking;
/// if \c false, then this API will add a command to the queue to be carried out at a later
/// time and return immediately.
///
/// \param[in] aAntennaPoweredOffCallback A user-defined MMMReaderEvenCallback function which will
/// receive a notification when the RF antenna has been powered off.
///
/// \param[in] aParam An optional pointer to some user-defined data. This is passed on to the
/// callback that has been provided.
///
/// \param[in] aEvent If \a aBlocking is \c true, pass in a handle to an OS event which will be
/// set once the operation finishes.
///
/// \return #NO_ERROR_OCCURRED if successful, otherwise a #MMMReaderErrorCode stating what
/// error occurred.
MMM_READER_LOW_LEVEL_API MMMReaderErrorCode MMMReader_RFPowerOff(
	bool aBlocking,
	MMMReaderEventCallback aAntennaPoweredOffCallback,
	void *aParam,
	HANDLE aEvent);

/// Cancels any pending RFID operations that have been queued by non-blocking API calls
/// within the RFID module, and attempts to cancel any currently executing operation.
///
/// \ingroup RfidModule
///
/// \note This API is only relevant if using the \llapi.
///
/// \return #NO_ERROR_OCCURRED if successful, otherwise a #MMMReaderErrorCode stating what
/// error occurred.
///
/// \remarks Any operations that are currently in progress may complete or not be aborted
/// immediately.
///
/// \remarks This API will return as soon as it has indicated that the operations are to be
/// cancelled, not when any currently in-progress operations are complete. Therefore, if the
/// host application needs to wait for completion, the MMMReader_IsRFActive() API can be used
/// to check for completion.
///
/// \sa MMMReader_IsRFActive()
MMM_READER_LOW_LEVEL_API MMMReaderErrorCode MMMReader_RFIDCancelQueue();

/// Aborts all RF chip reading operations whilst currently reading a document and prevents
/// any further operations from being added to the queue until MMMReader_RFOpen() is called
/// again.
///
/// \ingroup RfidModule
///
/// \note This API is only relevant if using the \llapi. The \hlapi has an equivalent
/// API called MMMReader_RFAbort().
///
/// \return #NO_ERROR_OCCURRED if successful, otherwise a #MMMReaderErrorCode stating what
/// error occurred.
///
/// \remarks This API is useful in the case where the host application determines that it will
/// take too long to complete an RF read, and it would be quicker to abort the operation than
/// wait for all RF reading operations to fail one at a time.
///
/// \remarks If there are no current RF operations waiting to be carried out or RF has
/// been disabled, this function has no effect.
///
/// \sa MMMReader_RFOpen(), MMMReader_LL_IsRFAborted()
MMM_READER_LOW_LEVEL_API MMMReaderErrorCode MMMReader_LL_RFAbort();

/// Determines whether the RFID module has been initialised.
///
/// \ingroup RfidModule
///
/// \note This API is only relevant if using the \llapi.
///
/// \return \c true if MMMReader_RFInitialise() has been called, or \c false otherwise.
///
/// \sa MMMReader_RFInitialise()
MMM_READER_LOW_LEVEL_API bool MMMReader_IsRFInitialised();

/// Determines whether the RFID module is in a suspended state.
///
/// \ingroup RfidModule
///
/// \note This API is only relevant if using the \llapi.
///
/// \return \c true if the RFID module is suspended, or \c false otherwise.
///
/// \sa MMMReader_RFSuspend()
MMM_READER_LOW_LEVEL_API bool MMMReader_IsRFSuspended();

/// Determines if the current RF read has been aborted using MMMReader_LL_RFAbort().
///
/// \ingroup RfidModule
///
/// \note This API is only relevant if using the \llapi.
///
/// \return \c true if MMMReader_LL_RFAbort() has been called, or \c false otherwise.
///
/// \sa MMMReader_LL_RFAbort()
MMM_READER_LOW_LEVEL_API bool MMMReader_LL_IsRFAborted();

/// Reconnects to the RFID device after it has been suspended by either calling
/// MMMReader_RFSuspend(), or via the MMMReader_RFInitialise() API with the \a aStartSuspended
/// flag set.
///
/// \ingroup RfidModule
///
/// \note This API is only relevant if using the \llapi.
///
/// \return #NO_ERROR_OCCURRED if successful, otherwise a #MMMReaderErrorCode stating what
/// error occurred.
///
/// \sa MMMReader_RFInitialise(), MMMReader_RFSuspend(), MMMReader_IsRFSuspended()
MMM_READER_LOW_LEVEL_API MMMReaderErrorCode MMMReader_RFResume();

/// Releases the RFID device to allow another application to use it.
///
/// \ingroup RfidModule
///
/// \note This API is only relevant if using the \llapi.
///
/// \return #NO_ERROR_OCCURRED if successful, otherwise a #MMMReaderErrorCode stating what
/// error occurred.
///
/// \attention While the RFID device is suspended, it is not possible to use any API which
/// requires access to the RFID device.
///
/// \sa MMMReader_RFResume(), MMMReader_IsRFSuspended()
MMM_READER_LOW_LEVEL_API MMMReaderErrorCode MMMReader_RFSuspend();

/// Gets the number of smart card readers available.
///
/// \ingroup RfidModule
///
/// \note This API is only relevant if using the \llapi.
///
/// \return The number of smart card reader devices available.
///
/// \remarks Smart card readers can either be contact or contact-less.
///
/// \remarks Use this API in conjunction with MMMReader_RFGetReaderInfo() to get information
/// regarding all the available smart card readers.
///
/// \remarks If multiple smart card readers are available, then it can be specified which
/// reader to use via the MMMReader_RFOpen() API.
///
/// \sa MMMReader_RFGetReaderInfo(), MMMReader_RFOpen()
MMM_READER_LOW_LEVEL_API int MMMReader_RFGetReaderCount();

/// Gets information regarding a particular smart card reader.
///
/// \ingroup RfidModule
///
/// \note This API is only relevant if using the \llapi.
///
/// \param[in] aBlocking Specify whether this operation should be blocking or non-blocking;
/// if \c false, then this API will add a command to the queue to be carried out at a later
/// time and return immediately.
///
/// \param[in] aReaderIndex Specify which smart card reader to get information for. This is
/// a zero-based index.
///
/// \param[in] aDataCallback A user-defined #MMMReaderDataCallback function which will receive
/// the information as a SmartCardReaderInfo structure.
///
/// \param[in] aCallbackParam An optional pointer to some user-defined data. This is passed on
/// to the callback provided.
///
/// \param[in] aSequenceNum A user-defined value that identifies what data is coming back
/// through the \a aDataCallback.
///
/// \param[in] aEvent If \a aBlocking is \c true, pass in a handle to an OS event which will be
/// set once the operation finishes.
///
/// \return #NO_ERROR_OCCURRED if successful, otherwise a #MMMReaderErrorCode stating what
/// error occurred.
///
/// \remarks \a aCallbackParam can be used to provide some context data back through the
/// callbacks. One example could be the \c this pointer from a C++ class; in this case, because
/// the callback functions must be C-style functions, \a aCallbackParam would be able to access
/// elements of its class type.
///
/// \remarks \a aSequenceNum is a user-defined value which is passed straight into
/// \a aDataCallback. This parameter is provided so that it is possible to have one data
/// callback defined for multiple data items; the defined function can then filter the
/// \a aSequenceNum as appropriate.
///
/// \remarks The index to the first smart card reader is zero (0). Increment \a aReaderIndex
/// up until MMMReader_RFGetReaderCount() - \c 1 to get information for all available smart
/// card readers. All data is returned via the \a aDataCallback as a SmartCardReaderInfo
/// structure.
///
/// \remarks This API can provide information such as the name of the smart card reader and
/// whether it is contact or contact-less. See SmartCardReaderInfo for more details.
///
/// \sa MMMReader_RFGetReaderCount(), SmartCardReaderInfo
MMM_READER_LOW_LEVEL_API MMMReaderErrorCode MMMReader_RFGetReaderInfo(
	bool aBlocking,
	int aReaderIndex,
	MMMReaderDataCallback aDataCallback,
	void *aCallbackParam,
	int aSequenceNum,
	HANDLE aEvent
);

/// Determines whether any operation is currently active in the RFID module.
///
/// \ingroup RfidModule
///
/// \note This API is only relevant if using the \llapi.
///
/// \return \c true if any non-blocking operation is current being executed or is queued, or
/// \c false otherwise.
///
/// \sa MMMReader_RFIDCancelQueue()
MMM_READER_LOW_LEVEL_API bool MMMReader_IsRFActive();

/// Updates the RFID module settings to use.
///
/// \ingroup RfidModule
///
/// \note This API is only relevant if using the \llapi.
///
/// \param[in] aRFSettings A pointer to a RFProcessSettings structure which contains the
/// setting values to update.
///
/// \return #NO_ERROR_OCCURRED if successful, otherwise a #MMMReaderErrorCode stating what
/// error occurred.
///
/// \remarks This API is required to update some settings as further processing is required
/// for some settings. Specifically, if any of the certificate modes (defined by the
/// #EXTERNAL_CERT_MODE values) have changed, or settings related to a local certificate
/// filestore have been modified, then the certificates must be reloaded from disk.
///
/// \note Reloading certificate files may be take some time depending on the size of the
/// certificate file store.
///
/// \sa RFProcessSettings
MMM_READER_LOW_LEVEL_API MMMReaderErrorCode MMMReader_RFUpdateSettings(
	RFProcessSettings *aRFSettings
);

// Obtains the list of all RF chip ids that are present. Currently the API
// doesnt support multiple chips - always pass NULL or an empty string for chipid.
//MMMReaderErrorCode MMMReader_RFGetChipIDs(
//	bool aBlocking,
//	RFITEM aItem,
//	MMMReaderDataCallback aDataCallback,
//	void *aParam,
//	int aSequenceNum,
//	HANDLE aEvent
//);

/// Opens an RFID chip for reading from a given smart card reader.
///
/// \ingroup RfidModule
///
/// \note This API is only relevant if using the \llapi.
///
/// \param[in] aBlocking Specify whether this operation should be blocking or non-blocking;
/// if \c false, then this API will add a command to the queue to be carried out at a later
/// time and return immediately.
///
/// \param[in] aBACString A null-terminated string providing the Basic Access Control (BAC)
/// key required to allow access to BAC chips. This can be \c NULL; see the remarks section
/// below for more details.
///
/// \param[in] aAntennaMode Specify the antenna switching mode to use when searching for an
/// RFID chip. Typically this is taken from the #RFProcessSettings::puAntennaMode setting.
///
/// \param[in] aMaxAPDUAttempts Specify the maximum number of times any single APDU will be
/// attempted to be sent if not successful. This must be at least 2 as the establishment of BAC
/// is triggered by an APDU error. Typically this is taken from the
/// #RFProcessSettings::puMaxAPDUAttempts setting.
///
/// \param[in] aDefaultChipBaudRate Specify the first (and highest) over-air baud rate that
/// the smart card reader will use to communicate to the RFID chip. Typically this is taken from the
/// #RFProcessSettings::puDefaultChipBaudRate setting.
///
/// \param[in] aSelectLDSApplication This should be \c true for most e-Passport reading
/// applications. Typically this is taken from the #RFProcessSettings::puSelectLDSApplication
/// setting.
///
/// \param[in] aChipOpenedCallback A user-defined #MMMReaderEventCallback function to receive
/// notification when a RFID chip has been opened or fails to open, triggering a
/// #RF_CHIP_OPENED_SUCCESSFULLY or a #RF_CHIP_OPEN_FAILED event respectively.
///
/// \param[in] aParam An optional pointer to some user-defined data. This is passed on to the
/// callback that has been provided.
///
/// \param[in] aEvent If \a aBlocking is \c true, pass in a handle to an OS event which will be
/// set once the operation finishes.
///
/// \param[in] aReaderIndex Specify which smart card reader to use for communicating to the
/// RFID chip. See the remarks section below for more details.
///
/// \return #NO_ERROR_OCCURRED if successful, otherwise a #MMMReaderErrorCode stating what
/// error occurred.
///
/// \remarks \a aParam can be used to provide some context data back through the callbacks. One
/// example could be the \c this pointer from a C++ class; in this case, because the callback
/// functions must be C-style functions, \a aParam would be able to access elements of its
/// class type.
///
/// \remarks Typically, the required settings are taken from the #RFProcessSettings structure,
/// which is typically loaded into the #MMMReaderSettings by using MMMReader_LL_LoadSettings().
///
/// \remarks A BAC key is required to allow access to BAC chips. If a key is known in advance,
/// provide it via the \a aBACString parameter. However, the BAC key can also be obtained
/// whilst opening the RFID chip by using the a #MMMReaderBACCallback function which can
/// be provided via MMMReader_RFInitialise(). If \a aBACString is \c NULL, then a user-defined
/// #MMMReaderBACCallback function must be provided.
///
/// \remarks \a aReaderIndex can be used to open an RFID chip from a specific smart card
/// reader; this is useful if multiple smart card readers are available. The \a aReaderIndex
/// is a zero-based index. If only one smart card reader is available, set this value to zero.
/// The number of smart card readers available can be determined by using the
/// MMMReader_RFGetReaderCount() API.
///
/// \remarks This API will attempt to open an RFID chip immediately, meaning if the RFID chip
/// is not yet present around the smart card reader device then this API will fail. Consider
/// using the MMMReader_RFWaitForOpen() API instead for such scenarios.
///
/// \remarks Once an RFID chip has been successfully opened (upon receiving the
/// #RF_APPLICATION_OPENED_SUCCESSFULLY event), you can use the other RFID APIs to perform other
/// operations on the chip, such as getting and validating data. See the <b>See also</b>
/// section for a list of the APIs that can be used.
///
/// \sa RFProcessSettings, MMMReader_RFWaitForOpen(), MMMReader_RFGetReaderCount(),
/// MMMReader_RFGetFile(), MMMReader_RFValidateDataGroup(), MMMReader_RFDecodeDataGroup(),
/// MMMReader_RFValidateSignature(), MMMReader_RFValidateSignedAttributes(),
/// MMMReader_RFCheckActiveAuthentication(), MMMReader_RFCheckPassiveAuthentication(),
/// MMMReader_RFValidateDocSignerCert(), MMMReader_RFGetChipAuthenticationStatus(), 
/// MMMReader_RFGetTerminalAuthenticationStatus()
MMM_READER_LOW_LEVEL_API MMMReaderErrorCode MMMReader_RFOpen(
	bool aBlocking,
	char *aBACString,
	ANTENNA_MODE aAntennaMode,
	int aMaxAPDUAttempts,
	int aDefaultChipBaudRate,
	bool aSelectLDSApplication,
	MMMReaderEventCallback aChipOpenedCallback,
	void *aParam,
	HANDLE aEvent,
	int aReaderIndex
);

/// Opens an RFID chip for reading from a given smart card reader. This API will wait for a
/// period of time for the RFID chip to be present before opening it.
///
/// \ingroup RfidModule
///
/// \note This API is only relevant if using the \llapi.
///
/// \param[in] aBlocking Specify whether this operation should be blocking or non-blocking;
/// if \c false, then this API will add a command to the queue to be carried out at a later
/// time and return immediately.
///
/// \param[in] aBACString A null-terminated string providing the Basic Access Control (BAC)
/// key required to allow access to BAC chips. This can be \c NULL; see the remarks section
/// below for more details.
///
/// \param[in] aAntennaMode Specify the antenna switching mode to use when searching for an
/// RFID chip. Typically this is taken from the #RFProcessSettings::puAntennaMode setting.
///
/// \param[in] aMaxAPDUAttempts Specify the maximum number of times any single APDU will be
/// attempted to be sent if not successful. This must be at least 2 as the establishment of BAC
/// is triggered by an APDU error. Typically this is taken from the
/// #RFProcessSettings::puMaxAPDUAttempts setting.
///
/// \param[in] aDefaultChipBaudRate Specify the first (and highest) over-air baud rate that
/// the smart card reader will use to communicate to the RFID chip. Typically this is taken from the
/// #RFProcessSettings::puDefaultChipBaudRate setting.
///
/// \param[in] aSelectLDSApplication This should be \c true for most e-Passport reading
/// applications. Typically this is taken from the #RFProcessSettings::puSelectLDSApplication
/// setting.
///
/// \param[in] aTimeoutMillisecs Specify the maximum amount of time to wait for the RFID
/// chip to be present. This value is measured in milliseconds (ms).
///
/// \param[in] aChipOpenedCallback A user-defined #MMMReaderEventCallback function to receive
/// notification when a RFID chip has been opened or the operation takes too long, triggering a
/// #RF_CHIP_OPENED_SUCCESSFULLY or a #RF_CHIP_OPEN_TIMEOUT event respectively.
///
/// \param[in] aParam An optional pointer to some user-defined data. This is passed on to the
/// callback that has been provided.
///
/// \param[in] aEvent If \a aBlocking is \c true, pass in a handle to an OS event which will be
/// set once the operation finishes.
///
/// \param[in] aReaderIndex Specify which smart card reader to use for communicating to the
/// RFID chip. See the remarks section below for more details.
///
/// \return #NO_ERROR_OCCURRED if successful, otherwise a #MMMReaderErrorCode stating what
/// error occurred.
///
/// \remarks This API differs from MMMReader_RFOpen() in that it will wait for a period of
/// time (determined by \a aTimeoutMillisecs) until either an RFID chip has been opened or
/// the timeout expires, which would trigger a #RF_CHIP_OPEN_TIMEOUT event. This is useful
/// for scenarios where a smart card reader is positioned away from the device which can
/// capture the Basic Access Control (BAC) key, allowing the user time to move the RF document
/// from one place to another.
///
/// \remarks \a aParam can be used to provide some context data back through the callbacks. One
/// example could be the \c this pointer from a C++ class; in this case, because the callback
/// functions must be C-style functions, \a aParam would be able to access elements of its
/// class type.
///
/// \remarks Typically, the required settings are taken from the #RFProcessSettings structure,
/// which is typically loaded into the #MMMReaderSettings by using MMMReader_LL_LoadSettings().
///
/// \remarks A BAC key is required to allow access to BAC chips. If a key is known in advance,
/// provide it via the \a aBACString parameter. However, the BAC key can also be obtained
/// whilst opening the RFID chip by using the a #MMMReaderBACCallback function which can
/// be provided via MMMReader_RFInitialise(). If \a aBACString is \c NULL, then a user-defined
/// #MMMReaderBACCallback function must be provided.
///
/// \remarks \a aReaderIndex can be used to open an RFID chip from a specific smart card
/// reader; this is useful if multiple smart card readers are available. The \a aReaderIndex
/// is a zero-based index. If only one smart card reader is available, set this value to zero.
/// The number of smart card readers available can be determined by using the
/// MMMReader_RFGetReaderCount() API.
///
/// \remarks Once an RFID chip has been successfully opened (upon receiving the
/// #RF_APPLICATION_OPENED_SUCCESSFULLY event), you can use the other RFID APIs to perform other
/// operations on the chip, such as getting and validating data. See the <b>See also</b>
/// section for a list of the APIs that can be used.
///
/// \sa RFProcessSettings, MMMReader_RFOpen(), MMMReader_RFWaitForRemoval(),
/// MMMReader_RFGetReaderCount(), MMMReader_RFGetFile(), MMMReader_RFValidateDataGroup(),
/// MMMReader_RFDecodeDataGroup(), MMMReader_RFValidateSignature(),
/// MMMReader_RFValidateSignedAttributes(), MMMReader_RFCheckActiveAuthentication(),
/// MMMReader_RFValidateDocSignerCert(), MMMReader_RFGetChipAuthenticationStatus(),
/// MMMReader_RFGetTerminalAuthenticationStatus(), MMMReader_RFCheckPassiveAuthentication()
MMM_READER_LOW_LEVEL_API MMMReaderErrorCode MMMReader_RFWaitForOpen(
	bool aBlocking,
	char *aBACString,
	ANTENNA_MODE aAntennaMode,
	int aMaxAPDUAttempts,
	int aDefaultChipBaudRate,
	bool aSelectLDSApplication,
	int aTimeoutMillisecs,
	MMMReaderEventCallback aChipOpenedCallback,
	void *aParam,
	HANDLE aEvent,
	int aReaderIndex
);

/// Waits for no RFID chips to be present in a smart card reader.
///
/// \ingroup RfidModule
///
/// \note This API is only relevant if using the \llapi.
///
/// \param[in] aBlocking Specify whether this operation should be blocking or non-blocking;
/// if \c false, then this API will add a command to the queue to be carried out at a later
/// time and return immediately.
///
/// \param[in] aTimeoutMillisecs Specify the maximum amount of time to wait for the RFID
/// chip to be removed. This value is measured in milliseconds (ms).
///
/// \param[in] aChipOpenedCallback A user-defined #MMMReaderEventCallback function to receive
/// notification when a RFID chip has been removed or the operation takes too long, triggering
/// a #RF_CHIP_REMOVAL_SUCCESS or a #RF_CHIP_REMOVAL_TIMEOUT event respectively.
///
/// \param[in] aParam An optional pointer to some user-defined data. This is passed on to the
/// callback that has been provided.
///
/// \param[in] aEvent If \a aBlocking is \c true, pass in a handle to an OS event which will be
/// set once the operation finishes.
///
/// \param[in] aReaderIndex Specify which smart card reader to use for communicating to the
/// RFID chip. See the remarks section below for more details.
///
/// \return #NO_ERROR_OCCURRED if successful, otherwise a #MMMReaderErrorCode stating what
/// error occurred.
///
/// \remarks \a aParam can be used to provide some context data back through the callbacks. One
/// example could be the \c this pointer from a C++ class; in this case, because the callback
/// functions must be C-style functions, \a aParam would be able to access elements of its
/// class type.
///
/// \remarks \a aReaderIndex can be used to open an RFID chip from a specific smart card
/// reader; this is useful if multiple smart card readers are available. The \a aReaderIndex
/// is a zero-based index. If only one smart card reader is available, set this value to zero.
/// The number of smart card readers available can be determined by using the
/// MMMReader_RFGetReaderCount() API.
///
/// \sa MMMReader_RFWaitForOpen()
MMM_READER_LOW_LEVEL_API MMMReaderErrorCode MMMReader_RFWaitForRemoval(
	bool aBlocking,
	int aTimeoutMillisecs,
	MMMReaderEventCallback aChipOpenedCallback,
	void *aParam,
	HANDLE aEvent,
	int aReaderIndex
);

/// Gets the requested raw data file from the currently open RFID chip.
///
/// \ingroup RfidModule
///
/// \note This API is only relevant if using the \llapi.
///
/// \param[in] aBlocking Specify whether this operation should be blocking or non-blocking;
/// if \c false, then this API will add a command to the queue to be carried out at a later
/// time and return immediately.
///
/// \param[in] aItem Specify which data file to return from the RFID chip. See #MMMReaderRFItem
/// for all available items.
///
/// \param[in] aDataCallback A user-defined #MMMReaderDataCallback function which will receive
/// the data that was read as a byte array.
///
/// \param[in] aParam An optional pointer to some user-defined data. This is passed on to the
/// callback provided.
///
/// \param[in] aSequenceNum A user-defined value that identifies what data is coming back
/// through the \a aDataCallback.
///
/// \param[in] aEvent If \a aBlocking is \c true, pass in a handle to an OS event which will be
/// set once the operation finishes.
///
/// \return #NO_ERROR_OCCURRED if successful, otherwise a #MMMReaderErrorCode stating what
/// error occurred.
///
/// \attention This API will only return data if an RFID chip is currently open for reading,
/// achieved via either MMMReader_RFOpen() or MMMReader_RFWaitForOpen().
///
/// \remarks This API can be used to retrieve any of the data groups (DG1 to DG16) and
/// EF.COM and EF.SOD files. Note that data will only be returned if the requested file is
/// present on the RFID chip.
///
/// \remarks \a aParam can be used to provide some context data back through the callbacks. One
/// example could be the \c this pointer from a C++ class; in this case, because the callback
/// functions must be C-style functions, \a aParam would be able to access elements of its
/// class type.
///
/// \remarks \a aSequenceNum is a user-defined value which is passed straight into
/// \a aDataCallback. This parameter is provided so that it is possible to have one data
/// callback defined for multiple data items; the defined function can then filter the
/// \a aSequenceNum as appropriate.
///
/// \remarks For DG1, DG2 and DG3, the data file can be decoded by the \sdk to provide more
/// relevant data by using the MMMReader_RFDecodeDataGroup() API.
///
/// \sa MMMReader_RFOpen(), MMMReader_RFWaitForOpen(), MMMReader_RFDecodeDataGroup()
MMM_READER_LOW_LEVEL_API MMMReaderErrorCode MMMReader_RFGetFile(
	bool aBlocking,
	MMMReaderRFItem aItem,
	MMMReaderDataCallback aDataCallback,
	void *aParam,
	int aSequenceNum,
	HANDLE aEvent
);

/// Validates the data within a data group on the currently opened RFID chip.
///
/// \ingroup RfidModule
///
/// \note This API is only relevant if using the \llapi.
///
/// \param[in] aBlocking Specify whether this operation should be blocking or non-blocking;
/// if \c false, then this API will add a command to the queue to be carried out at a later
/// time and return immediately.
///
/// \param[in] aItem Specify which data file to return from the RFID chip. See #MMMReaderRFItem
/// for all available items.
///
/// \param[in] aDataCallback A user-defined #MMMReaderDataCallback function which will receive
/// the validation result as a #MMMReaderValidationCode value.
///
/// \param[in] aParam An optional pointer to some user-defined data. This is passed on to the
/// callback provided.
///
/// \param[in] aSequenceNum A user-defined value that identifies what data is coming back
/// through the \a aDataCallback.
///
/// \param[in] aEvent If \a aBlocking is \c true, pass in a handle to an OS event which will be
/// set once the operation finishes.
///
/// \return #NO_ERROR_OCCURRED if successful, otherwise a #MMMReaderErrorCode stating what
/// error occurred.
///
/// \attention This API will only return data if an RFID chip is currently open for reading,
/// achieved via either MMMReader_RFOpen() or MMMReader_RFWaitForOpen().
///
/// \remarks This API can be used to validate any of the data groups (DG1 to DG16); only
/// #RFID_DG1 to #RFID_DG16 are acceptable values for \a aItem. The result will be returned
/// via the \a aDataCallback as a #MMMReaderValidationCode value.
///
/// \remarks \a aParam can be used to provide some context data back through the callbacks. One
/// example could be the \c this pointer from a C++ class; in this case, because the callback
/// functions must be C-style functions, \a aParam would be able to access elements of its
/// class type.
///
/// \remarks \a aSequenceNum is a user-defined value which is passed straight into
/// \a aDataCallback. This parameter is provided so that it is possible to have one data
/// callback defined for multiple data items; the defined function can then filter the
/// \a aSequenceNum as appropriate.
///
/// \sa MMMReader_RFOpen(), MMMReader_RFWaitForOpen()
MMM_READER_LOW_LEVEL_API MMMReaderErrorCode MMMReader_RFValidateDataGroup(
	bool aBlocking,
	MMMReaderRFItem aItem,
	MMMReaderDataCallback aDataCallback,
	void *aParam,
	int aSequenceNum,
	HANDLE aEvent
);

/// Gets decoded data from certain data groups on the currently opened RFID chip.
///
/// \ingroup RfidModule
///
/// \note This API is only relevant if using the \llapi.
///
/// \param[in] aBlocking Specify whether this operation should be blocking or non-blocking;
/// if \c false, then this API will add a command to the queue to be carried out at a later
/// time and return immediately.
///
/// \param[in] aItem Specify which data file to return from the RFID chip. See #MMMReaderRFItem
/// for all available items.
///
/// \param[in] aDataCallback A user-defined #MMMReaderDataCallback function which will receive
/// the data that was read. See the remarks section below for more details.
///
/// \param[in] aParam An optional pointer to some user-defined data. This is passed on to the
/// callback provided.
///
/// \param[in] aSequenceNum A user-defined value that identifies what data is coming back
/// through the \a aDataCallback.
///
/// \param[in] aEvent If \a aBlocking is \c true, pass in a handle to an OS event which will be
/// set once the operation finishes.
///
/// \return #NO_ERROR_OCCURRED if successful, otherwise a #MMMReaderErrorCode stating what
/// error occurred.
///
/// \attention This API will only return data if an RFID chip is currently open for reading,
/// achieved via either MMMReader_RFOpen() or MMMReader_RFWaitForOpen().
///
/// \remarks This API can be used to decode data from the data groups DG1, DG2 and DG3;
/// therefore, only #RFID_DG1, #RFID_DG2, #RFID_DG3 and #RFID_EF_COM are acceptable values for \a aItem.
///
/// \remarks \a aParam can be used to provide some context data back through the callbacks. One
/// example could be the \c this pointer from a C++ class; in this case, because the callback
/// functions must be C-style functions, \a aParam would be able to access elements of its
/// class type.
///
/// \remarks \a aSequenceNum is a user-defined value which is passed straight into
/// \a aDataCallback. This parameter is provided so that it is possible to have one data
/// callback defined for multiple data items; the defined function can then filter the
/// \a aSequenceNum as appropriate.
///
/// \remarks If \a aItem is #RFID_DG1, this will extract the codeline from the data group
/// and will return it as a null-terminated string (\c char*). Note that unlike the
/// data returned by MMMReader_ImageReadCodeline(), this will not contain any return
/// characters at the end of each line within the data.
///
/// \remarks If \a aItem is #RFID_DG2, this will extract the photo image of the document and
/// will return it a a raw byte array. Note that the image format may be either JPEG or
/// JPEG 2000.
///
/// \remarks \eac If \a aItem is #RFID_DG3, this will extract the biometric fingerprint data
/// from the RFID chip and return it as a DG3FingerprintData structure.
///
/// \remarks If \a aItem is #RFID_EF_COM, retrieve a 2 byte bit map of data groups (DG1-DG16) present
/// in the EF.COM.
///
/// \sa MMMReader_RFOpen(), MMMReader_RFWaitForOpen()
MMM_READER_LOW_LEVEL_API MMMReaderErrorCode MMMReader_RFDecodeDataGroup(
	bool aBlocking,
	MMMReaderRFItem aItem,
	MMMReaderDataCallback aDataCallback,
	void *aParam,
	int aSequenceNum,
	HANDLE aEvent
);

/// Validates the signature on the currently opened RFID chip.
///
/// \ingroup RfidModule
///
/// \note This API is only relevant if using the \llapi.
///
/// \param[in] aBlocking Specify whether this operation should be blocking or non-blocking;
/// if \c false, then this API will add a command to the queue to be carried out at a later
/// time and return immediately.
///
/// \param[in] aDataCallback A user-defined #MMMReaderDataCallback function which will receive
/// the validation result as a #MMMReaderValidationCode value.
///
/// \param[in] aParam An optional pointer to some user-defined data. This is passed on to the
/// callback provided.
///
/// \param[in] aSequenceNum A user-defined value that identifies what data is coming back
/// through the \a aDataCallback.
///
/// \param[in] aEvent If \a aBlocking is \c true, pass in a handle to an OS event which will be
/// set once the operation finishes.
///
/// \return #NO_ERROR_OCCURRED if successful, otherwise a #MMMReaderErrorCode stating what
/// error occurred.
///
/// \attention This API will only return data if an RFID chip is currently open for reading,
/// achieved via either MMMReader_RFOpen() or MMMReader_RFWaitForOpen().
///
/// \remarks \a aParam can be used to provide some context data back through the callbacks. One
/// example could be the \c this pointer from a C++ class; in this case, because the callback
/// functions must be C-style functions, \a aParam would be able to access elements of its
/// class type.
///
/// \remarks \a aSequenceNum is a user-defined value which is passed straight into
/// \a aDataCallback. This parameter is provided so that it is possible to have one data
/// callback defined for multiple data items; the defined function can then filter the
/// \a aSequenceNum as appropriate.
///
/// \sa MMMReader_RFOpen(), MMMReader_RFWaitForOpen()
MMM_READER_LOW_LEVEL_API MMMReaderErrorCode MMMReader_RFValidateSignature(
	bool aBlocking,
	MMMReaderDataCallback aDataCallback,
	void *aParam,
	int aSequenceNum,
	HANDLE aEvent
);

/// Validates the signed attributes on the currently opened RFID chip.
///
/// \ingroup RfidModule
///
/// \note This API is only relevant if using the \llapi.
///
/// \param[in] aBlocking Specify whether this operation should be blocking or non-blocking;
/// if \c false, then this API will add a command to the queue to be carried out at a later
/// time and return immediately.
///
/// \param[in] aDataCallback A user-defined #MMMReaderDataCallback function which will receive
/// the validation result as a #MMMReaderValidationCode value.
///
/// \param[in] aParam An optional pointer to some user-defined data. This is passed on to the
/// callback provided.
///
/// \param[in] aSequenceNum A user-defined value that identifies what data is coming back
/// through the \a aDataCallback.
///
/// \param[in] aEvent If \a aBlocking is \c true, pass in a handle to an OS event which will be
/// set once the operation finishes.
///
/// \return #NO_ERROR_OCCURRED if successful, otherwise a #MMMReaderErrorCode stating what
/// error occurred.
///
/// \attention This API will only return data if an RFID chip is currently open for reading,
/// achieved via either MMMReader_RFOpen() or MMMReader_RFWaitForOpen().
///
/// \remarks \a aParam can be used to provide some context data back through the callbacks. One
/// example could be the \c this pointer from a C++ class; in this case, because the callback
/// functions must be C-style functions, \a aParam would be able to access elements of its
/// class type.
///
/// \remarks \a aSequenceNum is a user-defined value which is passed straight into
/// \a aDataCallback. This parameter is provided so that it is possible to have one data
/// callback defined for multiple data items; the defined function can then filter the
/// \a aSequenceNum as appropriate.
///
/// \sa MMMReader_RFOpen(), MMMReader_RFWaitForOpen()
MMM_READER_LOW_LEVEL_API MMMReaderErrorCode MMMReader_RFValidateSignedAttributes(
	bool aBlocking,
	MMMReaderDataCallback aDataCallback,
	void *aParam,
	int aSequenceNum,
	HANDLE aEvent
);

/// Validates that EF.COM and EF.SOD agree on the datagroups present on the currently opened RFID chip.
///
/// \ingroup RfidModule
///
/// \note This API is only relevant if using the \llapi.
///
/// \param[in] aBlocking Specify whether this operation should be blocking or non-blocking;
/// if \c false, then this API will add a command to the queue to be carried out at a later
/// time and return immediately.
///
/// \param[in] aDataCallback A user-defined #MMMReaderDataCallback function which will receive
/// the validation result as a #TRISTATE value.
///
/// \param[in] aParam An optional pointer to some user-defined data. This is passed on to the
/// callback provided.
///
/// \param[in] aSequenceNum A user-defined value that identifies what data is coming back
/// through the \a aDataCallback.
///
/// \param[in] aEvent If \a aBlocking is \c true, pass in a handle to an OS event which will be
/// set once the operation finishes.
///
/// \return #NO_ERROR_OCCURRED if successful, otherwise a #MMMReaderErrorCode stating what
/// error occurred.
///
/// \attention This API will only return data if an RFID chip is currently open for reading,
/// achieved via either MMMReader_RFOpen() or MMMReader_RFWaitForOpen().
///
/// \remarks \a aParam can be used to provide some context data back through the callbacks. One
/// example could be the \c this pointer from a C++ class; in this case, because the callback
/// functions must be C-style functions, \a aParam would be able to access elements of its
/// class type.
///
/// \remarks \a aSequenceNum is a user-defined value which is passed straight into
/// \a aDataCallback. This parameter is provided so that it is possible to have one data
/// callback defined for multiple data items; the defined function can then filter the
/// \a aSequenceNum as appropriate.
///
/// \sa MMMReader_RFOpen(), MMMReader_RFWaitForOpen()
MMM_READER_LOW_LEVEL_API MMMReaderErrorCode MMMReader_CrosscheckEFComEFSod(
	bool aBlocking,
	MMMReaderDataCallback aDataCallback,
	void *aParam,
	int aSequenceNum,
	HANDLE aEvent
);


/// Gets the air baud rate that is being used for the currently open RFID chip.
///
/// \ingroup RfidModule
///
/// \note This API is only relevant if using the \llapi.
///
/// \param[in] aBlocking Specify whether this operation should be blocking or non-blocking;
/// if \c false, then this API will add a command to the queue to be carried out at a later
/// time and return immediately.
///
/// \param[in] aDataCallback A user-defined #MMMReaderDataCallback function which will receive
/// the data that was read as a null-terminated string (\c char*).
///
/// \param[in] aParam An optional pointer to some user-defined data. This is passed on to the
/// callback provided.
///
/// \param[in] aSequenceNum A user-defined value that identifies what data is coming back
/// through the \a aDataCallback.
///
/// \param[in] aEvent If \a aBlocking is \c true, pass in a handle to an OS event which will be
/// set once the operation finishes.
///
/// \return #NO_ERROR_OCCURRED if successful, otherwise a #MMMReaderErrorCode stating what
/// error occurred.
///
/// \attention This API will only return data if an RFID chip is currently open for reading,
/// achieved via either MMMReader_RFOpen() or MMMReader_RFWaitForOpen().
///
/// \remarks \a aParam can be used to provide some context data back through the callbacks. One
/// example could be the \c this pointer from a C++ class; in this case, because the callback
/// functions must be C-style functions, \a aParam would be able to access elements of its
/// class type.
///
/// \remarks \a aSequenceNum is a user-defined value which is passed straight into
/// \a aDataCallback. This parameter is provided so that it is possible to have one data
/// callback defined for multiple data items; the defined function can then filter the
/// \a aSequenceNum as appropriate.
///
/// \sa MMMReader_RFOpen(), MMMReader_RFWaitForOpen()
MMM_READER_LOW_LEVEL_API MMMReaderErrorCode MMMReader_RFGetAirBaudRate(
	bool aBlocking,
	MMMReaderDataCallback aDataCallback,
	void *aParam,
	int aSequenceNum,
	HANDLE aEvent
);

/// Determines whether Basic Access Control (BAC) has been used to open the currently open
/// RFID chip.
///
/// \ingroup RfidModule
///
/// \note This API is only relevant if using the \llapi.
///
/// \param[in] aBlocking Specify whether this operation should be blocking or non-blocking;
/// if \c false, then this API will add a command to the queue to be carried out at a later
/// time and return immediately.
///
/// \param[in] aDataCallback A user-defined #MMMReaderDataCallback function which will receive
/// the result as a #TRISTATE value.
///
/// \param[in] aParam An optional pointer to some user-defined data. This is passed on to the
/// callback provided.
///
/// \param[in] aSequenceNum A user-defined value that identifies what data is coming back
/// through the \a aDataCallback.
///
/// \param[in] aEvent If \a aBlocking is \c true, pass in a handle to an OS event which will be
/// set once the operation finishes.
///
/// \return #NO_ERROR_OCCURRED if successful, otherwise a #MMMReaderErrorCode stating what
/// error occurred.
///
/// \attention This API will only return data if an RFID chip is currently open for reading,
/// achieved via either MMMReader_RFOpen() or MMMReader_RFWaitForOpen().
///
/// \remarks When using a Basic Access Control (BAC) chip, BAC will not be established until
/// the first data item is requested. Therefore, using this API immediately after a call
/// to MMMReader_RFOpen() will return #TS_NOT_PERFORMED, even if the RFID chip is a BAC chip.
///
/// \remarks \a aParam can be used to provide some context data back through the callbacks. One
/// example could be the \c this pointer from a C++ class; in this case, because the callback
/// functions must be C-style functions, \a aParam would be able to access elements of its
/// class type.
///
/// \remarks \a aSequenceNum is a user-defined value which is passed straight into
/// \a aDataCallback. This parameter is provided so that it is possible to have one data
/// callback defined for multiple data items; the defined function can then filter the
/// \a aSequenceNum as appropriate.
///
/// \sa MMMReader_RFOpen(), MMMReader_RFWaitForOpen()
MMM_READER_LOW_LEVEL_API MMMReaderErrorCode MMMReader_RFGetBACStatus(
	bool aBlocking,
	MMMReaderDataCallback aDataCallback,
	void *aParam,
	int aSequenceNum,
	HANDLE aEvent
);

/// Determines whether Supplemental Access Control (SAC) has been used to open the currently open
/// RFID chip.
///
/// \ingroup RfidModule
///
/// \note This API is only relevant if using the \llapi.
///
/// \param[in] aBlocking Specify whether this operation should be blocking or non-blocking;
/// if \c false, then this API will add a command to the queue to be carried out at a later
/// time and return immediately.
///
/// \param[in] aDataCallback A user-defined #MMMReaderDataCallback function which will receive
/// the result as a #TRISTATE value.
///
/// \param[in] aParam An optional pointer to some user-defined data. This is passed on to the
/// callback provided.
///
/// \param[in] aSequenceNum A user-defined value that identifies what data is coming back
/// through the \a aDataCallback.
///
/// \param[in] aEvent If \a aBlocking is \c true, pass in a handle to an OS event which will be
/// set once the operation finishes.
///
/// \return #NO_ERROR_OCCURRED if successful, otherwise a #MMMReaderErrorCode stating what
/// error occurred.
///
/// \attention This API will only return data if an RFID chip is currently open for reading,
/// achieved via either MMMReader_RFOpen() or MMMReader_RFWaitForOpen().
///
/// \remarks \a aParam can be used to provide some context data back through the callbacks. One
/// example could be the \c this pointer from a C++ class; in this case, because the callback
/// functions must be C-style functions, \a aParam would be able to access elements of its
/// class type.
///
/// \remarks \a aSequenceNum is a user-defined value which is passed straight into
/// \a aDataCallback. This parameter is provided so that it is possible to have one data
/// callback defined for multiple data items; the defined function can then filter the
/// \a aSequenceNum as appropriate.
///
/// \sa MMMReader_RFOpen(), MMMReader_RFWaitForOpen()
MMM_READER_LOW_LEVEL_API MMMReaderErrorCode MMMReader_RFGetSACStatus(
	bool aBlocking, 
	MMMReaderDataCallback aDataCallback, 
	void *aParam, 
	int aSequenceNum, 
	HANDLE aEvent
);

/// Gets the chip ID of the currently open RFID chip.
///
/// \ingroup RfidModule
///
/// \note This API is only relevant if using the \llapi.
///
/// \param[in] aBlocking Specify whether this operation should be blocking or non-blocking;
/// if \c false, then this API will add a command to the queue to be carried out at a later
/// time and return immediately.
///
/// \param[in] aDataCallback A user-defined #MMMReaderDataCallback function which will receive
/// the data that was read as a null-terminated string (\c char*).
///
/// \param[in] aParam An optional pointer to some user-defined data. This is passed on to the
/// callback provided.
///
/// \param[in] aSequenceNum A user-defined value that identifies what data is coming back
/// through the \a aDataCallback.
///
/// \param[in] aEvent If \a aBlocking is \c true, pass in a handle to an OS event which will be
/// set once the operation finishes.
///
/// \return #NO_ERROR_OCCURRED if successful, otherwise a #MMMReaderErrorCode stating what
/// error occurred.
///
/// \attention This API will only return data if an RFID chip is currently open for reading,
/// achieved via either MMMReader_RFOpen() or MMMReader_RFWaitForOpen().
///
/// \remarks \a aParam can be used to provide some context data back through the callbacks. One
/// example could be the \c this pointer from a C++ class; in this case, because the callback
/// functions must be C-style functions, \a aParam would be able to access elements of its
/// class type.
///
/// \remarks \a aSequenceNum is a user-defined value which is passed straight into
/// \a aDataCallback. This parameter is provided so that it is possible to have one data
/// callback defined for multiple data items; the defined function can then filter the
/// \a aSequenceNum as appropriate.
///
/// \sa MMMReader_RFOpen(), MMMReader_RFWaitForOpen()
MMM_READER_LOW_LEVEL_API MMMReaderErrorCode MMMReader_RFGetChipId(
	bool aBlocking,
	MMMReaderDataCallback aDataCallback,
	void *aParam,
	int aSequenceNum,
	HANDLE aEvent
);

/// Resets the \sdk to clear all RF data that has been cached and forces an RFID chip to be
/// re-opened if further processing is required.
///
/// \ingroup RfidModule
///
/// \note This API is only relevant if using the \llapi.
///
/// \return #NO_ERROR_OCCURRED if successful, otherwise a #MMMReaderErrorCode stating what
/// error occurred.
MMM_READER_LOW_LEVEL_API MMMReaderErrorCode MMMReader_RFResetPassport();

/// Sends an APDU directly to the currently open RFID chip.
///
/// \ingroup RfidModule
///
/// \note This API is only relevant if using the \llapi.
///
/// \param[in] aBlocking Specify whether this operation should be blocking or non-blocking;
/// if \c false, then this API will add a command to the queue to be carried out at a later
/// time and return immediately.
///
/// \param[in] aAPDU A pointer to a binary buffer which contains the APDU to send.
///
/// \param[in] aAPDULen The size of the \a aAPDU buffer.
///
/// \param[out] aResponseBuffer A pointer to a writable binary buffer which will contain the
/// response returned from the RFID chip.
///
/// \param[in,out] aResponseBufferLen Initially will contain the length of the \a aResponseBuffer.
/// When the function returns, this value will be the actual length of the buffer.
///
/// \param[in] aEvent If \a aBlocking is \c true, pass in a handle to an OS event which will be
/// set once the operation finishes.
///
/// \return #NO_ERROR_OCCURRED if successful, otherwise a #MMMReaderErrorCode stating what
/// error occurred.
///
/// \attention It is recommended that the host application does not use this API unless there
/// is a specific need to send raw APDU data to the RFID chip; most data can be returned
/// via the RF APIs provided by the \sdk.
///
/// \attention If using this API, care must be taken to avoid mixing this API call with other
/// APIs that send APDUs (such as MMMReader_RFGetFile()).
///
/// \sa MMMReader_RFGetFile()
MMM_READER_LOW_LEVEL_API MMMReaderErrorCode MMMReader_RFSendAPDU(
	bool aBlocking,
	unsigned char* aAPDU,
	int aAPDULen,
	unsigned char* aResponseBuffer,
	int *aResponseBufferLen,
	HANDLE aEvent
);

/// Verifies that the currently opened RFID chip passes Active Authentication.
///
/// \ingroup RfidModule
///
/// \note This API is only relevant if using the \llapi.
///
/// \param[in] aBlocking Specify whether this operation should be blocking or non-blocking;
/// if \c false, then this API will add a command to the queue to be carried out at a later
/// time and return immediately.
///
/// \param[in] aDataCallback A user-defined #MMMReaderDataCallback function which will receive
/// the validation result as a #MMMReaderValidationCode value.
///
/// \param[in] aParam An optional pointer to some user-defined data. This is passed on to the
/// callback provided.
///
/// \param[in] aSequenceNum A user-defined value that identifies what data is coming back
/// through the \a aDataCallback.
///
/// \param[in] aEvent If \a aBlocking is \c true, pass in a handle to an OS event which will be
/// set once the operation finishes.
///
/// \return #NO_ERROR_OCCURRED if successful, otherwise a #MMMReaderErrorCode stating what
/// error occurred.
///
/// \attention This API will only return data if an RFID chip is currently open for reading,
/// achieved via either MMMReader_RFOpen() or MMMReader_RFWaitForOpen().
///
/// \remarks \a aParam can be used to provide some context data back through the callbacks. One
/// example could be the \c this pointer from a C++ class; in this case, because the callback
/// functions must be C-style functions, \a aParam would be able to access elements of its
/// class type.
///
/// \remarks \a aSequenceNum is a user-defined value which is passed straight into
/// \a aDataCallback. This parameter is provided so that it is possible to have one data
/// callback defined for multiple data items; the defined function can then filter the
/// \a aSequenceNum as appropriate.
///
/// \sa MMMReader_RFOpen(), MMMReader_RFWaitForOpen()
MMM_READER_LOW_LEVEL_API MMMReaderErrorCode MMMReader_RFCheckActiveAuthentication(
	bool aBlocking,
	MMMReaderDataCallback aDataCallback,
	void *aParam,
	int aSequenceNum,
	HANDLE aEvent
);

/// Verifies that the currently opened RFID chip passes Passive Authentication.
///
/// \ingroup RfidModule
///
/// \note This API is only relevant if using the \llapi.
///
/// \param[in] aBlocking Specify whether this operation should be blocking or non-blocking;
/// if \c false, then this API will add a command to the queue to be carried out at a later
/// time and return immediately.
///
/// \param[in] aDataCallback A user-defined #MMMReaderDataCallback function which will receive
/// the validation result as a #MMMReaderValidationCode value.
///
/// \param[in] aParam An optional pointer to some user-defined data. This is passed on to the
/// callback provided.
///
/// \param[in] aSequenceNum A user-defined value that identifies what data is coming back
/// through the \a aDataCallback.
///
/// \param[in] aEvent If \a aBlocking is \c true, pass in a handle to an OS event which will be
/// set once the operation finishes.
///
/// \return #NO_ERROR_OCCURRED if successful, otherwise a #MMMReaderErrorCode stating what
/// error occurred.
///
/// \attention This API will only return data if an RFID chip is currently open for reading,
/// achieved via either MMMReader_RFOpen() or MMMReader_RFWaitForOpen().
///
/// \remarks \a aParam can be used to provide some context data back through the callbacks. One
/// example could be the \c this pointer from a C++ class; in this case, because the callback
/// functions must be C-style functions, \a aParam would be able to access elements of its
/// class type.
///
/// \remarks \a aSequenceNum is a user-defined value which is passed straight into
/// \a aDataCallback. This parameter is provided so that it is possible to have one data
/// callback defined for multiple data items; the defined function can then filter the
/// \a aSequenceNum as appropriate.
///
/// \sa MMMReader_RFOpen(), MMMReader_RFWaitForOpen()
MMM_READER_LOW_LEVEL_API MMMReaderErrorCode MMMReader_RFCheckPassiveAuthentication(
	bool aBlocking, 
	MMMReaderDataCallback aDataCallback, 
	void *aParam, 
	int aSequenceNum, 
	HANDLE aEvent
);

/// Validates the Document Signer Certificates (DSCs) on the currently opened RFID chip.
///
/// \ingroup RfidModule
///
/// \note This API is only relevant if using the \llapi.
///
/// \param[in] aBlocking Specify whether this operation should be blocking or non-blocking;
/// if \c false, then this API will add a command to the queue to be carried out at a later
/// time and return immediately.
///
/// \param[in] aDataCallback A user-defined #MMMReaderDataCallback function which will receive
/// the validation result as a #MMMReaderValidationCode value.
///
/// \param[in] aParam An optional pointer to some user-defined data. This is passed on to the
/// callback provided.
///
/// \param[in] aSequenceNum A user-defined value that identifies what data is coming back
/// through the \a aDataCallback.
///
/// \param[in] aEvent If \a aBlocking is \c true, pass in a handle to an OS event which will be
/// set once the operation finishes.
///
/// \return #NO_ERROR_OCCURRED if successful, otherwise a #MMMReaderErrorCode stating what
/// error occurred.
///
/// \attention This API will only return data if an RFID chip is currently open for reading,
/// achieved via either MMMReader_RFOpen() or MMMReader_RFWaitForOpen().
///
/// \remarks When the certificates are validated, depending on the configuration settings
/// from RFProcessSettings the #MMMReaderCertificateCallback (set via
/// MMMReader_RFSetCertificateCallback()) may be invoked to supply the required certificates.
///
/// \remarks \a aParam can be used to provide some context data back through the callbacks. One
/// example could be the \c this pointer from a C++ class; in this case, because the callback
/// functions must be C-style functions, \a aParam would be able to access elements of its
/// class type.
///
/// \remarks \a aSequenceNum is a user-defined value which is passed straight into
/// \a aDataCallback. This parameter is provided so that it is possible to have one data
/// callback defined for multiple data items; the defined function can then filter the
/// \a aSequenceNum as appropriate.
///
/// \sa MMMReader_RFOpen(), MMMReader_RFWaitForOpen(), MMMReader_RFSetCertificateCallback()
MMM_READER_LOW_LEVEL_API MMMReaderErrorCode MMMReader_RFValidateDocSignerCert(
	bool aBlocking,
	MMMReaderDataCallback aDataCallback,
	void *aParam,
	int aSequenceNum,
	HANDLE aEvent
);

/// Sets a user-defined #MMMReaderCertificateCallback to be called when a certificate is
/// required for RF read operations.
///
/// \ingroup RfidModule
///
/// \note This API is only relevant if using the \llapi.
///
/// \param[in] aCertCallback A pointer to a user-defined #MMMReaderCertificateCallback function
/// to return the required certificate to use.
///
/// \return #NO_ERROR_OCCURRED if successful, otherwise a #MMMReaderErrorCode stating what
/// error occurred.
///
/// \remarks The #MMMReaderCertificateCallback is used only when an operation has been set
/// to #ECM_CERT_CALLBACK, such as the following:
///		- #RFProcessSettings::puDocSignerCertMode
///		- #RFProcessSettings::puExternalDSCMode
///		- #RFProcessSettings::puExternalCSCMode
///		- \eac #RFProcessSettings::puExternalCVCertsMode
///		- \eac #RFProcessSettings::puExternalPrivateKeyMode
///
/// \remarks An optional \c void* parameter will be returned through \a aCertCallback, which
/// will have been set via MMMReader_RFInitialise(). This parameter can be used to provide some
/// context data back through the callback. One example could be the \c this pointer from a
/// C++ class; in this case, because the callback functions must be C-style functions,
/// the parameter would be able to access elements of its class type.
///
/// \remarks For a detailed explanation of what the callback will return and expect from the
/// host application, see #MMMReaderCertificateCallback for more details.
///
/// \sa MMMReader_RFValidateDocSignerCert(), #MMMReaderCertificateCallback
MMM_READER_LOW_LEVEL_API MMMReaderErrorCode MMMReader_RFSetCertificateCallback(
	MMMReaderCertificateCallback aCertCallback
);

/// Sets a user-defined #MMMReaderSACAlgorithmCallback to be called when an ePassport supports
/// multiple SAC algorithms, which allows the caller to specify the desired algorithm.
///
/// \ingroup RfidModule
///
/// \note This API is only relevant if using the \llapi.
///
/// \param[in] aSACAlgorithmCallback A pointer to a user-defined #MMMReaderSACAlgorithmCallback function
/// to return the SAC algorithm to use.
///
/// \return #NO_ERROR_OCCURRED if successful, otherwise a #MMMReaderErrorCode stating what
/// error occurred.
///
/// \remarks For a detailed explanation of what the callback will return and expect from the
/// host application, see #MMMReaderSACAlgorithmCallback for more details.
///
/// \sa MMMReader_RFValidateDocSignerCert(), #MMMReaderCertificateCallback
MMM_READER_LOW_LEVEL_API MMMReaderErrorCode MMMReader_RFSetSACAlgorithmCallback(
	MMMReaderSACAlgorithmCallback aSACAlgorithmCallback
);

/// Sets a user-defined #MMMReaderNotifyCallback function to receive notifications when
/// the RF operations queue is empty.
///
/// \ingroup RfidModule
///
/// \note This API is only relevant if using the \llapi.
///
/// \param[in] aNotifyCallback A pointer to a user-defined #MMMReaderNotifyCallback function
/// which will be invoked when the RF operations queue is empty after completing all
/// pending operations.
///
/// \param[in] aParam An optional pointer to some user-defined data. This is passed on to the
/// callback provided.
///
/// \return #NO_ERROR_OCCURRED if successful, otherwise a #MMMReaderErrorCode stating what
/// error occurred.
///
/// \remarks Setting this callback function is useful as the host application need not
/// track whether all RF data items have been returned individually. It is particularly useful
/// in the case where errors occur when retrieving data items, so that individual errors need
/// not be tracked during the RF read.
///
/// \remarks \a aParam can be used to provide some context data back through the callbacks. One
/// example could be the \c this pointer from a C++ class; in this case, because the callback
/// functions must be C-style functions, \a aParam would be able to access elements of its
/// class type.
MMM_READER_LOW_LEVEL_API MMMReaderErrorCode MMMReader_RFSetQueueFinishedNotification(
	MMMReaderNotifyCallback aNotifyCallback,
	void* aParam
);

/// Gets the total amount of time spent waiting for the APDU response from the currently open
/// RFID chip.
///
/// \ingroup RfidModule
///
/// \note This API is only relevant if using the \llapi.
///
/// \param[in] aBlocking Specify whether this operation should be blocking or non-blocking;
/// if \c false, then this API will add a command to the queue to be carried out at a later
/// time and return immediately.
///
/// \param[in] aDataCallback A user-defined #MMMReaderDataCallback function which will receive
/// the data that was read as an integer.
///
/// \param[in] aParam An optional pointer to some user-defined data. This is passed on to the
/// callback provided.
///
/// \param[in] aSequenceNum A user-defined value that identifies what data is coming back
/// through the \a aDataCallback.
///
/// \param[in] aEvent If \a aBlocking is \c true, pass in a handle to an OS event which will be
/// set once the operation finishes.
///
/// \return #NO_ERROR_OCCURRED if successful, otherwise a #MMMReaderErrorCode stating what
/// error occurred.
///
/// \attention This API will only return data if an RFID chip is currently open for reading,
/// achieved via either MMMReader_RFOpen() or MMMReader_RFWaitForOpen().
///
/// \remarks The time is returned via \a aDataCallback as an integer (\c int*). The time is
/// measured in milliseconds (ms).
///
/// \remarks \a aParam can be used to provide some context data back through the callbacks. One
/// example could be the \c this pointer from a C++ class; in this case, because the callback
/// functions must be C-style functions, \a aParam would be able to access elements of its
/// class type.
///
/// \remarks \a aSequenceNum is a user-defined value which is passed straight into
/// \a aDataCallback. This parameter is provided so that it is possible to have one data
/// callback defined for multiple data items; the defined function can then filter the
/// \a aSequenceNum as appropriate.
///
/// \sa MMMReader_RFOpen(), MMMReader_RFWaitForOpen()
MMM_READER_LOW_LEVEL_API MMMReaderErrorCode MMMReader_RFGetChipApduTime(
	bool aBlocking,
	MMMReaderDataCallback aDataCallback,
	void *aParam,
	int aSequenceNum,
	HANDLE aEvent
);

/// Gets the total number of bytes read from the currently open RFID chip.
///
/// \ingroup RfidModule
///
/// \note This API is only relevant if using the \llapi.
///
/// \param[in] aBlocking Specify whether this operation should be blocking or non-blocking;
/// if \c false, then this API will add a command to the queue to be carried out at a later
/// time and return immediately.
///
/// \param[in] aDataCallback A user-defined #MMMReaderDataCallback function which will receive
/// the data that was read as an integer.
///
/// \param[in] aParam An optional pointer to some user-defined data. This is passed on to the
/// callback provided.
///
/// \param[in] aSequenceNum A user-defined value that identifies what data is coming back
/// through the \a aDataCallback.
///
/// \param[in] aEvent If \a aBlocking is \c true, pass in a handle to an OS event which will be
/// set once the operation finishes.
///
/// \return #NO_ERROR_OCCURRED if successful, otherwise a #MMMReaderErrorCode stating what
/// error occurred.
///
/// \attention This API will only return data if an RFID chip is currently open for reading,
/// achieved via either MMMReader_RFOpen() or MMMReader_RFWaitForOpen().
///
/// \remarks The value is returned via \a aDataCallback as an integer (\c int*).
///
/// \remarks \a aParam can be used to provide some context data back through the callbacks. One
/// example could be the \c this pointer from a C++ class; in this case, because the callback
/// functions must be C-style functions, \a aParam would be able to access elements of its
/// class type.
///
/// \remarks \a aSequenceNum is a user-defined value which is passed straight into
/// \a aDataCallback. This parameter is provided so that it is possible to have one data
/// callback defined for multiple data items; the defined function can then filter the
/// \a aSequenceNum as appropriate.
///
/// \sa MMMReader_RFOpen(), MMMReader_RFWaitForOpen()
MMM_READER_LOW_LEVEL_API MMMReaderErrorCode MMMReader_RFGetChipBytesRead(
	bool aBlocking,
	MMMReaderDataCallback aDataCallback,
	void *aParam,
	int aSequenceNum,
	HANDLE aEvent
);

/// \eac Verifies that Chip Authentication has been successfully established with the currently
/// open RFID chip.
///
/// \ingroup RfidModule
///
/// \note This API is only relevant if using the \llapi.
///
/// \param[in] aBlocking Specify whether this operation should be blocking or non-blocking;
/// if \c false, then this API will add a command to the queue to be carried out at a later
/// time and return immediately.
///
/// \param[in] aDataCallback A user-defined #MMMReaderDataCallback function which will receive
/// the validation result as a #TRISTATE value.
///
/// \param[in] aParam An optional pointer to some user-defined data. This is passed on to the
/// callback provided.
///
/// \param[in] aSequenceNum A user-defined value that identifies what data is coming back
/// through the \a aDataCallback.
///
/// \param[in] aEvent If \a aBlocking is \c true, pass in a handle to an OS event which will be
/// set once the operation finishes.
///
/// \return #NO_ERROR_OCCURRED if successful, otherwise a #MMMReaderErrorCode stating what
/// error occurred.
///
/// \attention This API will only return data if an RFID chip is currently open for reading,
/// achieved via either MMMReader_RFOpen() or MMMReader_RFWaitForOpen().
///
/// \attention This API will only work in the EAC-enabled \sdk; the standard \sdk will return
/// #ERROR_FEATURE_NOT_ENABLED. Contact \ssd for more details.
///
/// \remarks Chip Authentication will not be established until the first data item is requested.
/// Therefore, using this API immediately after a call to MMMReader_RFOpen() will
/// return #TS_NOT_PERFORMED, even if the RFID chip supports Chip Authentication.
///
/// \remarks \a aParam can be used to provide some context data back through the callbacks. One
/// example could be the \c this pointer from a C++ class; in this case, because the callback
/// functions must be C-style functions, \a aParam would be able to access elements of its
/// class type.
///
/// \remarks \a aSequenceNum is a user-defined value which is passed straight into
/// \a aDataCallback. This parameter is provided so that it is possible to have one data
/// callback defined for multiple data items; the defined function can then filter the
/// \a aSequenceNum as appropriate.
///
/// \sa MMMReader_RFOpen(), MMMReader_RFWaitForOpen()
MMM_READER_LOW_LEVEL_API MMMReaderErrorCode MMMReader_RFGetChipAuthenticationStatus(
	bool aBlocking,
	MMMReaderDataCallback aDataCallback,
	void *aParam,
	int aSequenceNum,
	HANDLE aEvent
);

/// \eac Verifies that Terminal Authentication has been successfully established with the
/// currently open RFID chip.
///
/// \ingroup RfidModule
///
/// \note This API is only relevant if using the \llapi.
///
/// \param[in] aBlocking Specify whether this operation should be blocking or non-blocking;
/// if \c false, then this API will add a command to the queue to be carried out at a later
/// time and return immediately.
///
/// \param[in] aDataCallback A user-defined #MMMReaderDataCallback function which will receive
/// the validation result as a #TRISTATE value.
///
/// \param[in] aParam An optional pointer to some user-defined data. This is passed on to the
/// callback provided.
///
/// \param[in] aSequenceNum A user-defined value that identifies what data is coming back
/// through the \a aDataCallback.
///
/// \param[in] aEvent If \a aBlocking is \c true, pass in a handle to an OS event which will be
/// set once the operation finishes.
///
/// \return #NO_ERROR_OCCURRED if successful, otherwise a #MMMReaderErrorCode stating what
/// error occurred.
///
/// \attention This API will only return data if an RFID chip is currently open for reading,
/// achieved via either MMMReader_RFOpen() or MMMReader_RFWaitForOpen().
///
/// \attention This API will only work in the EAC-enabled \sdk; the standard \sdk will return
/// #ERROR_FEATURE_NOT_ENABLED. Contact \ssd for more details.
///
/// \remarks As Terminal Authentication is only established during reading of protected
/// data groups (DG3 and DG4). Therefore the result #TS_NOT_PERFORMED will be returned
/// when called before attempting to read these data items. Therefore, it is recommended
/// this API is called after reading the protected data items.
///
/// \remarks During Terminal Authentication, data may need to be signed to complete the
/// operation if #RFProcessSettings::puExternalPrivateKeyMode is set to #ECM_SIGN_REQUEST. If
/// so, a #MMMReaderSignRequestCallback must be set. See MMMReader_RFSetSignRequestCallback()
/// for more details.
///
/// \remarks \a aParam can be used to provide some context data back through the callbacks. One
/// example could be the \c this pointer from a C++ class; in this case, because the callback
/// functions must be C-style functions, \a aParam would be able to access elements of its
/// class type.
///
/// \remarks \a aSequenceNum is a user-defined value which is passed straight into
/// \a aDataCallback. This parameter is provided so that it is possible to have one data
/// callback defined for multiple data items; the defined function can then filter the
/// \a aSequenceNum as appropriate.
///
/// \sa MMMReader_RFOpen(), MMMReader_RFWaitForOpen(), MMMReader_RFSetSignRequestCallback()
MMM_READER_LOW_LEVEL_API MMMReaderErrorCode MMMReader_RFGetTerminalAuthenticationStatus(
	bool aBlocking,
	MMMReaderDataCallback aDataCallback,
	void *aParam,
	int aSequenceNum,
	HANDLE aEvent
);

/// \eac Sets a user-defined #MMMReaderSignRequestCallback to be called whenever data needs
/// to be signed for Terminal Authentication on the currently open RFID chip.
///
/// \ingroup RfidModule
///
/// \note This API is only relevant if using the \llapi.
///
/// \param[in] aSignRequestCallback A pointer to a user-defined #MMMReaderSignRequestCallback
/// function to return the signed data.
///
/// \return #NO_ERROR_OCCURRED if successful, otherwise a #MMMReaderErrorCode stating what
/// error occurred.
///
/// \attention This API will only work in the EAC-enabled \sdk; the standard \sdk will return
/// #ERROR_FEATURE_NOT_ENABLED. Contact \ssd for more details.
///
/// \remarks This callback function is only used if
/// #RFProcessSettings::puExternalPrivateKeyMode is set to #ECM_SIGN_REQUEST.
///
/// \remarks An optional \c void* parameter will be returned through \a aCertCallback, which
/// will have been set via MMMReader_RFInitialise(). This parameter can be used to provide some
/// context data back through the callback. One example could be the \c this pointer from a
/// C++ class; in this case, because the callback functions must be C-style functions,
/// the parameter would be able to access elements of its class type.
///
/// \remarks For a detailed explanation of what the callback will return and expect from the
/// host application, see #MMMReaderSignRequestCallback for more details.
///
/// \sa MMMReader_RFGetTerminalAuthenticationStatus(), #MMMReaderSignRequestCallback
MMM_READER_LOW_LEVEL_API MMMReaderErrorCode MMMReader_RFSetSignRequestCallback(
	MMMReaderSignRequestCallback aSignRequestCallback
);


MMM_READER_LOW_LEVEL_API MMMReaderErrorCode MMMReader_LL_SetRFPasswordCallback(
	MMMReaderRFPasswordCallback aPasswordCallback
);

/// Gets a map of datagroup hashes present in the EF.SOD file from the currently open RFID chip.
///
/// \ingroup RfidModule
///
/// \note This API is only relevant if using the \llapi.
///
/// \param[in] aBlocking Specify whether this operation should be blocking or non-blocking;
/// if \c false, then this API will add a command to the queue to be carried out at a later
/// time and return immediately.
///
/// \param[in] aDataCallback A user-defined #MMMReaderDataCallback function which will receive
/// the data that was read as a byte array.
///
/// \param[in] aParam An optional pointer to some user-defined data. This is passed on to the
/// callback provided.
///
/// \param[in] aSequenceNum A user-defined value that identifies what data is coming back
/// through the \a aDataCallback.
///
/// \param[in] aEvent If \a aBlocking is \c true, pass in a handle to an OS event which will be
/// set once the operation finishes.
///
/// \return #NO_ERROR_OCCURRED if successful, otherwise a #MMMReaderErrorCode stating what
/// error occurred.
///
/// \attention This API will only return data if an RFID chip is currently open for reading,
/// achieved via either MMMReader_RFOpen() or MMMReader_RFWaitForOpen().
///
/// \remarks This API can be used to retrieve a 2 byte bit map of data group hashes (DG1-DG16) present 
/// in the EF.SOD.
///
/// \remarks \a aParam can be used to provide some context data back through the callbacks. One
/// example could be the \c this pointer from a C++ class; in this case, because the callback
/// functions must be C-style functions, \a aParam would be able to access elements of its
/// class type.
///
/// \remarks \a aSequenceNum is a user-defined value which is passed straight into
/// \a aDataCallback. This parameter is provided so that it is possible to have one data
/// callback defined for multiple data items; the defined function can then filter the
/// \a aSequenceNum as appropriate.
///
/// \sa MMMReader_RFOpen(), MMMReader_RFWaitForOpen()
MMM_READER_LOW_LEVEL_API MMMReaderErrorCode MMMReader_RFGetEFSodHashMap(
	bool aBlocking,
	MMMReaderDataCallback aDataCallback,
	void *aParam,
	int aSequenceNum,
	HANDLE aEvent
);

/// Gets the Document Signer Certificate (DSC), if any, in the EF.SOD file from the currently open RFID chip.
///
/// \ingroup RfidModule
///
/// \note This API is only relevant if using the \llapi.
///
/// \param[in] aBlocking Specify whether this operation should be blocking or non-blocking;
/// if \c false, then this API will add a command to the queue to be carried out at a later
/// time and return immediately.
///
/// \param[in] aDataCallback A user-defined #MMMReaderDataCallback function which will receive
/// the data that was read as a byte array.
///
/// \param[in] aParam An optional pointer to some user-defined data. This is passed on to the
/// callback provided.
///
/// \param[in] aSequenceNum A user-defined value that identifies what data is coming back
/// through the \a aDataCallback.
///
/// \param[in] aEvent If \a aBlocking is \c true, pass in a handle to an OS event which will be
/// set once the operation finishes.
///
/// \return #NO_ERROR_OCCURRED if successful, otherwise a #MMMReaderErrorCode stating what
/// error occurred.
///
/// \attention This API will only return data if an RFID chip is currently open for reading,
/// achieved via either MMMReader_RFOpen() or MMMReader_RFWaitForOpen().
///
/// \remarks This API can be used to retrieve the Document Signer Certificate (DSC) from the 
/// EF.SOD, if present.
///
/// \remarks \a aParam can be used to provide some context data back through the callbacks. One
/// example could be the \c this pointer from a C++ class; in this case, because the callback
/// functions must be C-style functions, \a aParam would be able to access elements of its
/// class type.
///
/// \remarks \a aSequenceNum is a user-defined value which is passed straight into
/// \a aDataCallback. This parameter is provided so that it is possible to have one data
/// callback defined for multiple data items; the defined function can then filter the
/// \a aSequenceNum as appropriate.
///
/// \sa MMMReader_RFOpen(), MMMReader_RFWaitForOpen()
MMM_READER_LOW_LEVEL_API MMMReaderErrorCode MMMReader_RFGetDocSignerCert(
	bool aBlocking,
	MMMReaderDataCallback aDataCallback,
	void *aParam,
	int aSequenceNum,
	HANDLE aEvent
);

/// \endcond
// PAGESDK

/////////// Swipe Reader APIs

/// Initialises the \swipereader and allocates all necessary resources to use it.
///
/// \if SWIPESDK
/// \ingroup SwipeInitStateModule
/// \else
/// \ingroup SwipeModule
/// \endif
///
/// \if PAGESDK
/// \note This API is only relevant if using a \swipereader with the \llapi.
/// \endif
///
/// \param[in] aSwipeSettings A pointer to a MMMSwipeSettings structure which contains
/// the required settings to use.
///
/// \param[in] aDataCallback A pointer to a user-defined #MMMReaderDataCallback function that
/// will receive data from the \swipereader and \sdk.
///
/// \param[in] aEventCallback A pointer to a user-defined #MMMReaderEventCallback function
/// that will receive event triggers from the \swipereader and \sdk. This can be \c NULL.
///
/// \param[in] aParam An optional pointer to some user-defined data. This is passed on to all
/// callbacks that have been set.
///
/// \return #NO_ERROR_OCCURRED if successful, otherwise a #MMMReaderErrorCode stating what
/// error occurred.
///
/// \attention The MMMReader_SwipeShutdown() API must be called when finished to free all
/// resources allocated.
///
/// \if PAGESDK
///
/// \remarks This API can be used to either use a \swipereader on its own, or one can be
/// used in conjunction with a \pagereader.
///
/// \endif
///
/// \remarks Typically \a aSwipeSettings is loaded using the MMMReader_LL_LoadSwipeSettings()
/// API.
///
/// \if PAGESDK
///
/// \remarks If used in conjunction with a \pagereader, it could also be taken from the
/// #MMMReaderSettings::puSwipeSettings structure (loaded via MMMReader_LL_LoadSettings()).
///
/// \endif
///
/// \remarks \a aParam can be used to provide some context data back through the callbacks. One
/// example could be the \c this pointer from a C++ class; in this case, because the callback
/// functions must be C-style functions, \a aParam would be able to access elements of its
/// class type.
///
/// \remarks Data from the \swipereader will be returned via the \a aDataCallback. Note that
/// depending on the protocol the \swipereader is configured with, not all data may be
/// returned. See #MMMReaderSwipeItem for more details.
///
/// \note Because data could be returned from the \swipereader at any time, \a aDataCallback
/// will usually be invoked from another thread. Care must be taken to ensure that the host
/// application performs actions in a thread-safe manner when receiving data.
///
/// \sa MMMReader_SwipeShutdown(), MMMReader_IsSwipeInitialised(),
/// #MMMReaderSwipeItem
MMM_READER_LOW_LEVEL_API MMMReaderErrorCode MMMReader_SwipeInitialise(
	MMMSwipeSettings* aSwipeSettings,
	MMMReaderDataCallback aDataCallback,
	MMMReaderEventCallback aEventCallback,
	void* aParam
);

/// Shuts down the \swipereader and frees all related resources.
///
/// \if SWIPESDK
/// \ingroup SwipeInitStateModule
/// \else
/// \ingroup SwipeModule
/// \endif
///
/// \if PAGESDK
/// \note This API is only relevant if using a \swipereader with the \llapi.
/// \endif
///
/// \return #NO_ERROR_OCCURRED if successful, otherwise a #MMMReaderErrorCode stating what
/// error occurred.
///
/// \sa MMMReader_SwipeInitialise(), MMMReader_IsSwipeInitialised()
MMM_READER_LOW_LEVEL_API MMMReaderErrorCode MMMReader_SwipeShutdown();

/// Determines whether the \swipereader has been initialised.
///
/// \if SWIPESDK
/// \ingroup SwipeInitStateModule
/// \else
/// \ingroup SwipeModule
/// \endif
///
/// \if PAGESDK
/// \note This API is only relevant if using a \swipereader with the \llapi.
/// \endif
///
/// \return \c true if MMMReader_SwipeInitialise() has been called, or \c false otherwise.
///
/// \sa MMMReader_SwipeInitialise()
MMM_READER_LOW_LEVEL_API bool MMMReader_IsSwipeInitialised();

/// Updates the \swipereader settings to use.
///
/// \if SWIPESDK
/// \ingroup SwipeInitStateModule
/// \else
/// \ingroup SwipeModule
/// \endif
///
/// \if PAGESDK
/// \note This API is only relevant if using a \swipereader with the \llapi.
/// \endif
///
/// \param[in] aNewSettings A pointer to a MMMSwipeSettings structure which contains the
/// setting values to update.
///
/// \return #NO_ERROR_OCCURRED if successful, otherwise a #MMMReaderErrorCode stating what
/// error occurred.
///
/// \attention Generally the required settings should be determined by the \swipeconfigwizard
/// supplied with the \sdk, and should therefore not require modification in most cases.
///
/// \attention This API only updates the \swipereader settings for the current session only; it
/// does not persist setting changes. To save the changes to the \sdk configuration files,
/// use the MMMReader_LL_SaveSwipeSettings() API. Before saving settings, make sure that
/// this is what is required, as the previous settings will be overwritten.
///
/// \sa MMMSwipeSettings, MMMReader_LL_LoadSwipeSettings(), MMMReader_LL_SaveSwipeSettings()
MMM_READER_LOW_LEVEL_API MMMReaderErrorCode MMMReader_SwipeUpdateSettings(
	MMMSwipeSettings* aNewSettings
);

/// Sends an \ref SendingCommandsPageRteEnableSubSec "Enable Device" command to the
/// \swipereader.
///
/// \if SWIPESDK
/// \ingroup SwipeRTECommandsModule
/// \else
/// \ingroup SwipeModule
/// \endif
///
/// \deprecated This API is provided for backwards compatibility with some \swipereaders
/// which accept commands; generally, this API is not required in most scenarios.
///
/// \if PAGESDK
/// \note This API is only relevant if using a \swipereader (configured with either the
/// \c RTE_INTERRUPT or \c RTE_POLLED protocols) with the \llapi.
/// \endif
///
/// \param[in] aDevice Specifies the device to enable.
///
/// \return #NO_ERROR_OCCURRED if successful, otherwise a #MMMReaderErrorCode stating what
/// error occurred.
///
/// \attention The setting #MMMSwipeProtocolSettings_RTE::AutoSendEnableDevice found in
/// the #MMMSwipeSettings structure allows the \sdk to automatically handle enabling the
/// device, thereby negating the need for this API in most situations.
MMM_READER_LOW_LEVEL_API MMMReaderErrorCode MMMReader_SwipeRTE_EnableDevice(
	RTEProtocolDeviceType aDevice
);

/// Sends a \ref SendingCommandsPageRteDisableSubSec "Disable Device" command to the
/// \swipereader.
///
/// \if SWIPESDK
/// \ingroup SwipeRTECommandsModule
/// \else
/// \ingroup SwipeModule
/// \endif
///
/// \deprecated This API is provided for backwards compatibility with some \swipereaders
/// which accept commands; generally, this API is not required in most scenarios.
///
/// \if PAGESDK
/// \note This API is only relevant if using a \swipereader (configured with either the
/// \c RTE_INTERRUPT or \c RTE_POLLED protocols) with the \llapi.
/// \endif
///
/// \param[in] aDevice Specifies the device to disable.
///
/// \return #NO_ERROR_OCCURRED if successful, otherwise a #MMMReaderErrorCode stating what
/// error occurred.
MMM_READER_LOW_LEVEL_API MMMReaderErrorCode MMMReader_SwipeRTE_DisableDevice(
	RTEProtocolDeviceType aDevice
);

/// Sends an \ref SendingCommandsPageRteInquireSubSec "Inquire" command to the \swipereader.
///
/// \if SWIPESDK
/// \ingroup SwipeRTECommandsModule
/// \else
/// \ingroup SwipeModule
/// \endif
///
/// \deprecated This API is provided for backwards compatibility with some \swipereaders
/// which accept commands; generally, this API is not required in most scenarios.
///
/// \if PAGESDK
/// \note This API is only relevant if using a \swipereader (configured with either the
/// \c RTE_INTERRUPT or \c RTE_POLLED protocols) with the \llapi.
/// \endif
///
/// \param[in] aDevice Specifies the device to inquire on.
///
/// \param[in] aTracks Specifies which tracks to return. See #RTEProtocolTrackValue for
/// more details.
///
/// \return #NO_ERROR_OCCURRED if successful, otherwise a #MMMReaderErrorCode stating what
/// error occurred.
MMM_READER_LOW_LEVEL_API MMMReaderErrorCode MMMReader_SwipeRTE_Inquire(
	RTEProtocolDeviceType aDevice,
	RTEProtocolTrackValue aTracks
);

/// Sends an \ref SendingCommandsPageRteLedSubSec "Operate LED" command to the \swipereader.
///
/// \if SWIPESDK
/// \ingroup SwipeRTECommandsModule
/// \else
/// \ingroup SwipeModule
/// \endif
///
/// \deprecated This API is provided for backwards compatibility with some \swipereaders
/// which accept commands; generally, this API is not required in most scenarios.
///
/// \if PAGESDK
/// \note This API is only relevant if using a \swipereader (configured with either the
/// \c RTE_INTERRUPT or \c RTE_POLLED protocols) with the \llapi.
/// \endif
///
/// \param[in] aDevice Specifies the device to use.
///
/// \param[in] aStatus Specifies the LED status to set. See #RTEProtocolLEDStatus for more
/// details.
///
/// \param[in] aOperation Specified the LED operation to set. See #RTEProtocolLEDOperation
/// for more details.
///
/// \return #NO_ERROR_OCCURRED if successful, otherwise a #MMMReaderErrorCode stating what
/// error occurred.
MMM_READER_LOW_LEVEL_API MMMReaderErrorCode MMMReader_SwipeRTE_OperateLED(
	RTEProtocolDeviceType aDevice,
	RTEProtocolLEDStatus aStatus,
	RTEProtocolLEDOperation aOperation
);

/// Sends an \ref SendingCommandsPageRteBuzzSubSec "Operate Buzzer" command to the \swipereader.
///
/// \if SWIPESDK
/// \ingroup SwipeRTECommandsModule
/// \else
/// \ingroup SwipeModule
/// \endif
///
/// \deprecated This API is provided for backwards compatibility with some \swipereaders
/// which accept commands; generally, this API is not required in most scenarios.
///
/// \if PAGESDK
/// \note This API is only relevant if using a \swipereader (configured with either the
/// \c RTE_INTERRUPT or \c RTE_POLLED protocols) with the \llapi.
/// \endif
///
/// \param[in] aTone Specifies the buzzer tone to use. See #RTEProtocolBuzzerTone for more
/// details.
///
/// \param[in] aTime Specifies the duration of the buzzer tone in seconds.
///
/// \return #NO_ERROR_OCCURRED if successful, otherwise a #MMMReaderErrorCode stating what
/// error occurred.
MMM_READER_LOW_LEVEL_API MMMReaderErrorCode MMMReader_SwipeRTE_OperateBuzzer(
	RTEProtocolBuzzerTone aTone,
	unsigned int aTime
);

/// Sends a \ref SendingCommandsPageRteResetSubSec "Reset Device" command to the \swipereader.
///
/// \if SWIPESDK
/// \ingroup SwipeRTECommandsModule
/// \else
/// \ingroup SwipeModule
/// \endif
///
/// \deprecated This API is provided for backwards compatibility with some \swipereaders
/// which accept commands; generally, this API is not required in most scenarios.
///
/// \if PAGESDK
/// \note This API is only relevant if using a \swipereader (configured with either the
/// \c RTE_INTERRUPT or \c RTE_POLLED protocols) with the \llapi.
/// \endif
///
/// \param[in] aDevice Specifies the device to reset.
///
/// \return #NO_ERROR_OCCURRED if successful, otherwise a #MMMReaderErrorCode stating what
/// error occurred.
MMM_READER_LOW_LEVEL_API MMMReaderErrorCode MMMReader_SwipeRTE_ResetDevice(
	RTEProtocolDeviceType aDevice
);

/// Sends a custom message or command to the \swipereader.
///
/// \if SWIPESDK
/// \ingroup SwipeRTECommandsModule
/// \else
/// \ingroup SwipeModule
/// \endif
///
/// \param[in] aData A pointer to an array of bytes to send to the \swipereader.
///
/// \param[in] aDataLength The length of the \a aData array.
///
/// \param[in] aWaitForResponse Specify whether the \llapi should wait for a response from
/// the \swipereader.
///
/// \return #NO_ERROR_OCCURRED if successful, otherwise a #MMMReaderErrorCode stating what
/// error occurred.
///
/// \attention Do not use this API if you are not sure what you are sending; this API could
/// have serious implications if misused.
MMM_READER_LOW_LEVEL_API MMMReaderErrorCode MMMReader_SwipeSendMessage(
	const MMMByte* aData,
	int aDataLength,
	bool aWaitForResponse
);

/////////// Parsing APIs

/// \cond PAGESDK

/// Initialises the \sdk parser module to assist in some data parsing functionality, and
/// allocates all necessary resources.
///
/// \ingroup ParsingModule
///
/// \note This API is only relevant if using the \llapi.
///
/// \return #NO_ERROR_OCCURRED if successful, otherwise a #MMMReaderErrorCode stating what
/// error occurred.
///
/// \attention The MMMReader_ParseShutdown() API must be called when finished to free all
/// resources allocated.
///
/// \remarks The Parser module is required for all operations except for
/// MMMReader_ParseCodeline(); if codeline parsing is only required, then it is safe to not
/// call MMMReader_ParseInitialise() and MMMReader_ParseShutdown().
///
/// \sa MMMReader_ParseShutdown(), MMMReader_IsParsingInitialised(), MMMReader_ParseCodeline(),
/// MMMReader_ParseAAMVA()
MMM_READER_LOW_LEVEL_API MMMReaderErrorCode MMMReader_ParseInitialise();

/// Shuts down the \sdk parser module and frees all related resouces.
///
/// \ingroup ParsingModule
///
/// \note This API is only relevant if using the \llapi.
///
/// \return #NO_ERROR_OCCURRED if successful, otherwise a #MMMReaderErrorCode stating what
/// error occurred.
///
/// \sa MMMReader_ParseInitialise(), MMMReader_IsParsingInitialised()
MMM_READER_LOW_LEVEL_API MMMReaderErrorCode MMMReader_ParseShutdown();

/// Determines whether the \sdk parser module has been initialised.
///
/// \ingroup ParsingModule
///
/// \note This API is only relevant if using the \llapi.
///
/// \return \c true if MMMReader_ParseInitialise() has been called, or \c false otherwise.
///
/// \sa MMMReader_ParseInitialise()
MMM_READER_LOW_LEVEL_API bool MMMReader_IsParsingInitialised();

/// Parses an ICAO codeline string into separate field values.
///
/// \ingroup ParsingModule
///
/// \note This API is only relevant if using the \llapi.
///
/// \param[in] aCodeline A null-terminated string providing the codeline to parse.
///
/// \param[in] aFlags A bitwise flag set specifying certain options used to configure this
/// API. See #MMMParseCodelineFlag for more details. ICAOFIELDOPOSNS is for when you know
/// the identity of the document and can specify where all the field positions are. e.g.
/// for when it doesn't conform to the ICAO standard
///
/// \param[in] aPosns context rule to use or NULL for no context
///
/// \param[out] aCodelineData A pointer to a MMMReaderCodelineData structure which will contain
/// the results.
///
/// \return #NO_ERROR_OCCURRED if successful, otherwise a #MMMReaderErrorCode stating what
/// error occurred.
///
/// \attention This API does \b not require that the \sdk parser module be initialised first
/// via MMMReader_ParseInitialise().
///
/// \remarks The codeline may come from the following sources:
///		- From MMMReader_ImageReadCodeline() or MMMReader_ImageReadCodelineTwoSided
///		- From DG1 of an RFID chip, which can be accessed via the MMMReader_RFDecodeDataGroup()
///
/// \remarks This API will not fail if \a aCodeline is not a valid codeline. If \a aCodeline
/// is invalid or cannot be parsed, all values in \a aCodelineData will be set to empty strings
/// and zero values.
///
/// \sa MMMReaderCodelineData
MMM_READER_LOW_LEVEL_API MMMReaderErrorCode MMMReader_ParseCodeline(
	const char* aCodeline,
	MMMParseCodelineFlag aFlags,
	MMMReaderCodelineData* aCodelineData,
	CODELINECONTEXT *posns
);

/// Parses a data stream read from a North American drivers licence which adheres to the \aamva
/// standard.
///
/// \ingroup ParsingModule
///
/// \note This API is only relevant if using the \llapi.
///
/// \param[in] aDataStr A null-terminated string providing the data to parse.
///
/// \param[in] aDataLength The length of \a aDataStr.
///
/// \param[out] aDataStructPtr A pointer to an MMMReaderAAMVAData pointer which must have a value
/// of null.
///
///	\param[in] aDataSource The source of \a aDataStr. See #MMMReaderAAMVADataSource
///
/// \return #NO_ERROR_OCCURRED if successful, otherwise a #MMMReaderErrorCode stating what
/// error occurred.
///
/// \attention This API requires that the \sdk parser module be initialised first via
/// MMMReader_ParseInitialise().
///
/// \remarks The \sdk will attempt to parse appropriate values from \a aDataStr. If \a aDataStr
/// is invalid or cannot be parsed, all values in \a aDataStructPtr point to an MMMReaderAAMVAData
/// structure containing empty strings and zero values.
///
/// \remarks To free the data created call MMMReader_DestroyCachedObject() passing in
/// the pointer to an MMMReaderAAMVAData returned.
/// e.g.
/// \code
/// MMMReaderAAMVAData *lAAMVADataPtr = 0;
///
/// MMMReader_ParseAAMVA(
///	  lDataStr,
///	  lDataLength,
///	  &lAAMVADataPtr,
///	  AAMVA_DS_MagStripe
/// );
///
/// //... Use returned data
///
/// MMMReader_DestroyCachedObject(lAAMVADataPtr);
/// \endcode
///
/// \remarks See MMMReaderAAMVAData for more details.
///
/// \sa MMMReader_ParseInitialise(), MMMReaderAAMVAData, MMMReader_DestroyCachedObject()
MMM_READER_LOW_LEVEL_API MMMReaderErrorCode MMMReader_ParseAAMVA(
	const char* aDataStr,
	int aDataLength,
	MMMReaderAAMVAData** aDataStructPtr,
	MMMReaderAAMVADataSource aDataSource
);

/// Checks a data stream for a North American drivers licence \aamva
/// standard protocol.
///
/// \ingroup ParsingModule
///
/// \note This API is only relevant if using the \llapi.
///
/// \param[in] aDataStr A null-terminated string providing the data to parse.
///
/// \param[in] aDataLength The length of \a aDataStr.
///
/// \return #NO_ERROR_OCCURRED if successful check, otherwise a #ERROR_UNKNOWN_DATA_FORMAT
///
/// \remarks The \sdk will attempt to check for specific feature from \a aDataStr. If \a aDataStr
/// is invalid the #ERROR_UNKNOWN_DATA_FORMAT will be returned.
///
/// \sa MMMReader_ParseInitialise()
MMM_READER_LOW_LEVEL_API MMMReaderErrorCode MMMReader_CheckForAAMVA(
	const char* aDataStr,
	int aDataLength
);


/// \endcond
// PAGESDK

/////////// UHF APIs

/// \cond PAGESDK

/// Initialises the UHF module and allocates resources required for UHF read operations.
///
/// \ingroup UHFModule
///
/// \note This API is only relevant if using the \llapi.
///
/// \param[in] aTheSettings A pointer to a #UHFSettings structure that contains all the
/// values to initialise the UHFModule with.
///
/// \return #NO_ERROR_OCCURRED if successful, otherwise a #MMMReaderErrorCode stating the
/// error that occurred.
///
/// \attention The MMMReader_UHFShutdown() API must be called when finished with the
/// UHF device to free all resources allocated.
///
/// \sa MMMReader_UHFShutdown(), MMMReader_IsUHFInitialised(), MMMReader_IsUHFOpen(),
/// MMMReader_UHFOpen()
MMM_READER_LOW_LEVEL_API MMMReaderErrorCode MMMReader_UHFInitialise(
	UHFSettings *aTheSettings
);

/// Determines whether any operation is currently active in the UHF module.
///
/// \ingroup UHFModule
///
/// \note This API is only relevant if using the \llapi.
///
/// \return \c true if any non-blocking operation is current being executed or is queued, or
/// \c false otherwise.
///
/// \sa MMMReader_UHFCancelQueue()
MMM_READER_LOW_LEVEL_API bool MMMReader_IsUHFActive();

/// Cancels any pending UHF operations that have been queued by non-blocking API calls
/// within the UHF module, and attempts to cancel any currently executing operation.
///
/// \ingroup UHFModule
///
/// \note This API is only relevant if using the \llapi.
///
/// \return #NO_ERROR_OCCURRED if successful, otherwise a #MMMReaderErrorCode stating what
/// error occurred.
///
/// \remarks Any operations that are currently in progress may complete or not be aborted
/// immediately.
///
/// \remarks This API will return as soon as it has indicated that the operations are to be
/// cancelled, not when any currently in-progress operations are complete. Therefore, if the
/// host application needs to wait for completion, the MMMReader_IsUHFActive() API can be used
/// to check for completion.
///
/// \sa MMMReader_IsUHFActive()
MMM_READER_LOW_LEVEL_API MMMReaderErrorCode MMMReader_UHFCancelQueue();

/// Shuts down the UHF module and frees all related resources.
///
/// \ingroup UHFModule
///
/// \note This API is only relevant if using the \llapi.
///
/// \return #NO_ERROR_OCCURRED if successful, otherwise a #MMMReaderErrorCode stating the
/// error that occurred.
///
/// \sa MMMReader_UHFInitialise(), MMMReader_IsUHFInitialised()
MMM_READER_LOW_LEVEL_API MMMReaderErrorCode MMMReader_UHFShutdown();

/// Determines whether the UHF module has been initialised.
///
/// \ingroup UHFModule
///
/// \note This API is only relevant if using the \llapi.
///
/// \return \c true if MMMReader_UHFInitialise() has been called, or \c false otherwise.
///
/// \sa MMMReader_UHFInitialise()
MMM_READER_LOW_LEVEL_API bool MMMReader_IsUHFInitialised();

/// Reads the TagID from a UHF tag that is present in the field of the reader.
///
/// \ingroup UHFModule
///
/// \note This API is only relevant if using the \llapi.
///
/// \param[in] aBlocking Specify whether this operation should be blocking or non-blocking;
/// if \c false, then this API will add a command to the queue to be carried out at a later
/// time and return immediately. If \c true the command will be carried out before this
/// function returns
///
/// \param[in] aDataCallback A user-defined #MMMReaderDataCallback function which will receive
/// the data that was read as a #MMMReaderUHFTagIDData struct.
///
/// \param[in] aParam An optional pointer to some user-defined data. This is passed on to the
/// callback provided.
///
/// \param[in] aSequenceNum A user-defined value that identifies what data is coming back
/// through the \a aDataCallback.
///
/// \param[in] aEvent If \a aBlocking is \c true, pass in a handle to an OS event which will be
/// set once the operation finishes.
///
/// \param[in] aEventCallback A user-defined #MMMReaderEventCallback function which will
/// receive UHF event #UHF_READ_COMPLETE or #UHF_READ_TIMEOUT
///
/// \param[in] aUID An optional Tag UID, to specify which Tag to read the data from. If set
/// to NULL, data will be read from the first Tag found in the field.
///
/// \param[in] aUIDLen The length of the data pointed to by the optional aUID parameter.
///
/// \return #NO_ERROR_OCCURRED if successful, otherwise a #MMMReaderErrorCode stating what
/// error occurred.
///
/// \attention This API will only return data if a UHF tag is found in the reader field
///
/// \remarks \a aParam can be used to provide some context data back through the callbacks.
/// One example could be the \c this pointer from a C++ class; in this case, because the
/// callback functions must be C-style functions, \a aParam would be able to access elements
/// of its class type.
///
/// \remarks \a aSequenceNum is a user-defined value which is passed straight into
/// \a aDataCallback. This parameter is provided so that it is possible to have one data
/// callback defined for multiple data items; the defined function can then filter the
/// \a aSequenceNum as appropriate.
///
/// \sa MMMReader_UHFInitialise()
MMM_READER_LOW_LEVEL_API MMMReaderErrorCode MMMReader_UHFGetTagID(
	bool  aBlocking,
	MMMReaderDataCallback  aDataCallback,
	void *  aParam,
	int aSequenceNum,
	HANDLE  aEvent,
	MMMReaderEventCallback aEventCallback,
	unsigned char *aUID,
	unsigned int aUIDLen
);

/// Waits until a UHF tag is present in the field of the reader and then reads the TagID
/// value from the tag.
///
/// \ingroup UHFModule
///
/// \note This API is only relevant if using the \llapi.
///
/// \param[in] aBlocking Specify whether this operation should be blocking or non-blocking;
/// if \c false, then this API will add a command to the queue to be carried out at a later
/// time and return immediately. If \c true the command will be carried out before this
/// function returns
///
/// \param[in] aTimeoutMillisecs The maximum amount of time to wait for a tag to enter the
/// field of the reader. 0 = infinite.
///
/// \param[in] aDataCallback A user-defined #MMMReaderDataCallback function which will receive
/// the data that was read as a #MMMReaderUHFTagIDData struct.
///
/// \param[in] aParam An optional pointer to some user-defined data. This is passed on to the
/// callback provided.
///
/// \param[in] aSequenceNum A user-defined value that identifies what data is coming back
/// through the \a aDataCallback.
///
/// \param[in] aEvent If \a aBlocking is \c true, pass in a handle to an OS event which will be
/// set once the operation finishes.
///
/// \param[in] aEventCallback A user-defined #MMMReaderEventCallback function which will
/// receive UHF events #UHF_READ_COMPLETE and #UHF_READ_TIMEOUT
///
/// \param[in] aUID An optional Tag UID, to specify which Tag to read the data from. If set
/// to NULL, data will be read from the first Tag found in the field.
///
/// \param[in] aUIDLen The length of the data pointed to by the optional aUID parameter.
///
/// \return #NO_ERROR_OCCURRED if successful, otherwise a #MMMReaderErrorCode stating what
/// error occurred.
///
/// \attention This API will only return data if a UHF tag is found in the reader field
///
/// \remarks \a aParam can be used to provide some context data back through the callbacks.
/// One example could be the \c this pointer from a C++ class; in this case, because the
/// callback functions must be C-style functions, \a aParam would be able to access elements
/// of its class type.
///
/// \remarks \a aSequenceNum is a user-defined value which is passed straight into
/// \a aDataCallback. This parameter is provided so that it is possible to have one data
/// callback defined for multiple data items; the defined function can then filter the
/// \a aSequenceNum as appropriate.
///
/// \sa MMMReader_UHFInitialise()
MMM_READER_LOW_LEVEL_API MMMReaderErrorCode MMMReader_UHFWaitForTagID(
	bool  aBlocking,
	int aTimeoutMillisecs,
	MMMReaderDataCallback  aDataCallback,
	void *  aParam,
	int aSequenceNum,
	HANDLE  aEvent,
	MMMReaderEventCallback aEventCallback,
	unsigned char *aUID,
	unsigned int aUIDLen
);

/// Reads the EPC (Electronic Product Code) section from the EPC memory bank of a UHF tag
/// that is present in the field of the reader.
///
/// \ingroup UHFModule
///
/// \note This API is only relevant if using the \llapi.
///
/// \param[in] aBlocking Specify whether this operation should be blocking or non-blocking;
/// if \c false, then this API will add a command to the queue to be carried out at a later
/// time and return immediately. If \c true the command will be carried out before this
/// function returns
///
/// \param[in] aDataCallback A user-defined #MMMReaderDataCallback function which will receive
/// the data that was read as an unsigned char array.
///
/// \param[in] aParam An optional pointer to some user-defined data. This is passed on to the
/// callback provided.
///
/// \param[in] aSequenceNum A user-defined value that identifies what data is coming back
/// through the \a aDataCallback.
///
/// \param[in] aEvent If \a aBlocking is \c true, pass in a handle to an OS event which will be
/// set once the operation finishes.
///
/// \param[in] aEventCallback A user-defined #MMMReaderEventCallback function which will
/// receive UHF event #UHF_READ_COMPLETE or #UHF_READ_TIMEOUT
///
/// \return #NO_ERROR_OCCURRED if successful, otherwise a #MMMReaderErrorCode stating what
/// error occurred.
///
/// \attention This API will only return data if a UHF tag is found in the reader field
///
/// \remarks \a aParam can be used to provide some context data back through the callbacks.
/// One example could be the \c this pointer from a C++ class; in this case, because the
/// callback functions must be C-style functions, \a aParam would be able to access elements
/// of its class type.
///
/// \remarks \a aSequenceNum is a user-defined value which is passed straight into
/// \a aDataCallback. This parameter is provided so that it is possible to have one data
/// callback defined for multiple data items; the defined function can then filter the
/// \a aSequenceNum as appropriate.
///
/// \sa MMMReader_UHFInitialise()
MMM_READER_LOW_LEVEL_API MMMReaderErrorCode MMMReader_UHFGetEPC(
	bool  aBlocking,
	MMMReaderDataCallback  aDataCallback,
	void *  aParam,
	int aSequenceNum,
	HANDLE  aEvent,
	MMMReaderEventCallback aEventCallback
);

/// Waits until a UHF tag is present in the field of the reader and then reads the
/// EPC (Electronic Product Code) section from the EPC memory bank of the tag.
///
/// \ingroup UHFModule
///
/// \note This API is only relevant if using the \llapi.
///
/// \param[in] aBlocking Specify whether this operation should be blocking or non-blocking;
/// if \c false, then this API will add a command to the queue to be carried out at a later
/// time and return immediately. If \c true the command will be carried out before this
/// function returns
///
/// \param[in] aTimeoutMillisecs The maximum amount of time to wait for a tag to enter the
/// field of the reader. 0 = infinite.
///
/// \param[in] aDataCallback A user-defined #MMMReaderDataCallback function which will receive
/// the data that was read as an unsigned char array.
///
/// \param[in] aParam An optional pointer to some user-defined data. This is passed on to the
/// callback provided.
///
/// \param[in] aSequenceNum A user-defined value that identifies what data is coming back
/// through the \a aDataCallback.
///
/// \param[in] aEvent If \a aBlocking is \c true, pass in a handle to an OS event which will be
/// set once the operation finishes.
///
/// \param[in] aEventCallback A user-defined #MMMReaderEventCallback function which will
/// receive UHF events #UHF_READ_COMPLETE and #UHF_READ_TIMEOUT
///
/// \return #NO_ERROR_OCCURRED if successful, otherwise a #MMMReaderErrorCode stating what
/// error occurred.
///
/// \attention This API will only return data if a UHF tag is found in the reader field
///
/// \remarks \a aParam can be used to provide some context data back through the callbacks.
/// One example could be the \c this pointer from a C++ class; in this case, because the
/// callback functions must be C-style functions, \a aParam would be able to access elements
/// of its class type.
///
/// \remarks \a aSequenceNum is a user-defined value which is passed straight into
/// \a aDataCallback. This parameter is provided so that it is possible to have one data
/// callback defined for multiple data items; the defined function can then filter the
/// \a aSequenceNum as appropriate.
///
/// \sa MMMReader_UHFInitialise()
MMM_READER_LOW_LEVEL_API MMMReaderErrorCode MMMReader_UHFWaitForEPC(
	bool  aBlocking,
	int aTimeoutMillisecs,
	MMMReaderDataCallback  aDataCallback,
	void *  aParam,
	int aSequenceNum,
	HANDLE  aEvent,
	MMMReaderEventCallback aEventCallback
);

/// Reads the contents of the desired memory bank from a UHF tag that is present
/// in the field of the reader.
///
/// \ingroup UHFModule
///
/// \note This API is only relevant if using the \llapi.
///
/// \param[in] aBlocking Specify whether this operation should be blocking or non-blocking;
/// if \c false, then this API will add a command to the queue to be carried out at a later
/// time and return immediately. If \c true the command will be carried out before this
/// function returns
///
/// \param[in] aMemoryBank The tag memory bank to read the data from.
///
///	\param[in] aOffset The memory bank offset, in bytes, to start the read from.
///
///	\param[in] aByteCount The number of bytes to read from the memory bank.
///
/// \param[in] aDataCallback A user-defined #MMMReaderDataCallback function which will receive
/// the data that was read as an unsigned char array.
///
/// \param[in] aParam An optional pointer to some user-defined data. This is passed on to the
/// callback provided.
///
/// \param[in] aSequenceNum A user-defined value that identifies what data is coming back
/// through the \a aDataCallback.
///
/// \param[in] aEvent If \a aBlocking is \c true, pass in a handle to an OS event which will be
/// set once the operation finishes.
///
/// \param[in] aEventCallback A user-defined #MMMReaderEventCallback function which will
/// receive UHF event UHF_READ_COMPLETE
///
/// \param[in] aUID An optional Tag UID, to specify which Tag to read the data from. If set
/// to NULL, data will be read from the first Tag found in the field.
///
/// \param[in] aUIDLen The length of the data pointed to by the optional aUID parameter.
///
/// \return #NO_ERROR_OCCURRED if successful, otherwise a #MMMReaderErrorCode stating what
/// error occurred.
///
/// \attention This API will only return data if a UHF tag is found in the reader field
/// and the memory bank specified contains data.
///
/// \remarks \a aParam can be used to provide some context data back through the callbacks.
/// One example could be the \c this pointer from a C++ class; in this case, because the
/// callback functions must be C-style functions, \a aParam would be able to access elements
/// of its class type.
///
/// \remarks \a aSequenceNum is a user-defined value which is passed straight into
/// \a aDataCallback. This parameter is provided so that it is possible to have one data
/// callback defined for multiple data items; the defined function can then filter the
/// \a aSequenceNum as appropriate.
///
/// \sa MMMReader_UHFInitialise()
MMM_READER_LOW_LEVEL_API MMMReaderErrorCode MMMReader_UHFGetMemoryBankData(
	bool  aBlocking,
	MMMReaderUHFMemoryBank aMemoryBank,
	unsigned long aOffset,
	unsigned long aByteCount,
	MMMReaderDataCallback  aDataCallback,
	void *  aParam,
	int aSequenceNum,
	HANDLE  aEvent,
	MMMReaderEventCallback aEventCallback,
	unsigned char *aUID,
	unsigned int aUIDLen
);

/// Waits until a UHF tag is present in the field of the reader and then reads the
/// contents of the desired memory bank from the UHF tag.
///
/// \ingroup UHFModule
///
/// \note This API is only relevant if using the \llapi.
///
/// \param[in] aBlocking Specify whether this operation should be blocking or non-blocking;
/// if \c false, then this API will add a command to the queue to be carried out at a later
/// time and return immediately. If \c true the command will be carried out before this
/// function returns
///
/// \param[in] aTimeoutMillisecs The maximum amount of time to wait for a tag to enter the
/// field of the reader. 0 = infinite.
///
/// \param[in] aMemoryBank The tag memory bank to read the data from.
///
///	\param[in] aOffset The memory bank offset, in bytes, to start the read from.
///
///	\param[in] aByteCount The number of bytes to read from the memory bank.
///
/// \param[in] aDataCallback A user-defined #MMMReaderDataCallback function which will receive
/// the data that was read as an unsigned char array.
///
/// \param[in] aParam An optional pointer to some user-defined data. This is passed on to the
/// callback provided.
///
/// \param[in] aSequenceNum A user-defined value that identifies what data is coming back
/// through the \a aDataCallback.
///
/// \param[in] aEvent If \a aBlocking is \c true, pass in a handle to an OS event which will be
/// set once the operation finishes.
///
/// \param[in] aEventCallback A user-defined #MMMReaderEventCallback function which will
/// receive UHF events #UHF_READ_COMPLETE and #UHF_READ_TIMEOUT
///
/// \param[in] aUID An optional Tag UID, to specify which Tag to read the data from. If set
/// to NULL, data will be read from the first Tag found in the field.
///
/// \param[in] aUIDLen The length of the data pointed to by the optional aUID parameter.
///
/// \return #NO_ERROR_OCCURRED if successful, otherwise a #MMMReaderErrorCode stating what
/// error occurred.
///
/// \attention This API will only return data if a UHF tag is found in the reader field
/// and the memory bank specified contains data.
///
/// \remarks \a aParam can be used to provide some context data back through the callbacks.
/// One example could be the \c this pointer from a C++ class; in this case, because the
/// callback functions must be C-style functions, \a aParam would be able to access elements
/// of its class type.
///
/// \remarks \a aSequenceNum is a user-defined value which is passed straight into
/// \a aDataCallback. This parameter is provided so that it is possible to have one data
/// callback defined for multiple data items; the defined function can then filter the
/// \a aSequenceNum as appropriate.
///
/// \sa MMMReader_UHFInitialise()
MMM_READER_LOW_LEVEL_API MMMReaderErrorCode MMMReader_UHFWaitForMemoryBankData(
	bool  aBlocking,
	int aTimeoutMillisecs,
	MMMReaderUHFMemoryBank aMemoryBank,
	unsigned long aOffset,
	unsigned long aByteCount,
	MMMReaderDataCallback  aDataCallback,
	void *  aParam,
	int aSequenceNum,
	HANDLE  aEvent,
	MMMReaderEventCallback aEventCallback,
	unsigned char *aUID,
	unsigned int aUIDLen
);

/// Reads the UID from every tag that is present in the field of the reader.
///
/// \ingroup UHFModule
///
/// \note This API is only relevant if using the \llapi.
///
/// \param[in] aBlocking Specify whether this operation should be blocking or non-blocking;
/// if \c false, then this API will add a command to the queue to be carried out at a later
/// time and return immediately. If \c true the command will be carried out before this
/// function returns
///
/// \param[in] aDataCallback A user-defined #MMMReaderDataCallback function which will receive
/// the data that was read as a #MMMReaderUHFTagIDData struct.
///
/// \param[in] aParam An optional pointer to some user-defined data. This is passed on to the
/// callback provided.
///
/// \param[in] aSequenceNum A user-defined value that identifies what data is coming back
/// through the \a aDataCallback.
///
/// \param[in] aEvent If \a aBlocking is \c true, pass in a handle to an OS event which will be
/// set once the operation finishes.
///
/// \param[in] aEventCallback A user-defined #MMMReaderEventCallback function which will
/// receive UHF event UHF_READ_COMPLETE
///
/// \return #NO_ERROR_OCCURRED if successful, otherwise a #MMMReaderErrorCode stating what
/// error occurred.
///
/// \attention This API will only return data if a UHF tag is found in the reader field
///
/// \remarks \a aParam can be used to provide some context data back through the callbacks.
/// One example could be the \c this pointer from a C++ class; in this case, because the
/// callback functions must be C-style functions, \a aParam would be able to access elements
/// of its class type.
///
/// \remarks \a aSequenceNum is a user-defined value which is passed straight into
/// \a aDataCallback. This parameter is provided so that it is possible to have one data
/// callback defined for multiple data items; the defined function can then filter the
/// \a aSequenceNum as appropriate.
///
/// \sa MMMReader_UHFInitialise()
MMM_READER_LOW_LEVEL_API MMMReaderErrorCode MMMReader_UHFGetTagsInField(
	bool  aBlocking,
	MMMReaderDataCallback  aDataCallback,
	void *  aParam,
	int aSequenceNum,
	HANDLE  aEvent,
	MMMReaderEventCallback aEventCallback
);

/// Reads the UID from every tag that is present in the field of the reader. If there are
/// no tags currently present, this command will continue to look for tags until either
/// some tags enter the field, or the timeout value is hit.
///
/// \ingroup UHFModule
///
/// \note This API is only relevant if using the \llapi.
///
/// \param[in] aBlocking Specify whether this operation should be blocking or non-blocking;
/// if \c false, then this API will add a command to the queue to be carried out at a later
/// time and return immediately. If \c true the command will be carried out before this
/// function returns
///
/// \param[in] aTimeoutMillisecs The maximum amount of time to wait for a tag to enter the
/// field of the reader. 0 = infinite, in this istance use MMMReader_CameraCancelQueue() to
/// abort the wait.
///
/// \param[in] aDataCallback A user-defined #MMMReaderDataCallback function which will receive
/// the data that was read as a #MMMReaderUHFTagIDData struct.
///
/// \param[in] aParam An optional pointer to some user-defined data. This is passed on to the
/// callback provided.
///
/// \param[in] aSequenceNum A user-defined value that identifies what data is coming back
/// through the \a aDataCallback.
///
/// \param[in] aEvent If \a aBlocking is \c true, pass in a handle to an OS event which will be
/// set once the operation finishes.
///
/// \param[in] aEventCallback A user-defined #MMMReaderEventCallback function which will
/// receive UHF event UHF_READ_COMPLETE
///
/// \return #NO_ERROR_OCCURRED if successful, otherwise a #MMMReaderErrorCode stating what
/// error occurred.
///
/// \attention This API will only return data if a UHF tag is found in the reader field
///
/// \remarks \a aParam can be used to provide some context data back through the callbacks.
/// One example could be the \c this pointer from a C++ class; in this case, because the
/// callback functions must be C-style functions, \a aParam would be able to access elements
/// of its class type.
///
/// \remarks \a aSequenceNum is a user-defined value which is passed straight into
/// \a aDataCallback. This parameter is provided so that it is possible to have one data
/// callback defined for multiple data items; the defined function can then filter the
/// \a aSequenceNum as appropriate.
///
/// \sa MMMReader_UHFInitialise()
MMM_READER_LOW_LEVEL_API MMMReaderErrorCode MMMReader_UHFWaitForTagsInField(
	bool  aBlocking,
	int aTimeoutMillisecs,
	MMMReaderDataCallback  aDataCallback,
	void *  aParam,
	int aSequenceNum,
	HANDLE  aEvent,
	MMMReaderEventCallback aEventCallback
);

/// Reads the all the memory from a Tag based on the memory map config file.
///
/// \ingroup UHFModule
///
/// \note This API is only relevant if using the \llapi.
///
/// \param[in] aBlocking Specify whether this operation should be blocking or non-blocking;
/// if \c false, then this API will add a command to the queue to be carried out at a later
/// time and return immediately. If \c true the command will be carried out before this
/// function returns
///
/// \param[in] aUID An optional Tag UID, to specify which Tag to read the data from. If set
/// to NULL, data will be read from the first Tag found in the field.
///
/// \param[in] aUIDLen The length of the data pointed to by the optional aUID parameter.
///
/// \param[in] aDataCallback A user-defined #MMMReaderDataCallback function which will receive
/// all the data that is read as a #MMMReaderUHFTagData structs.
///
/// \param[in] aParam An optional pointer to some user-defined data. This is passed on to the
/// callback provided.
///
/// \param[in] aSequenceNum A user-defined value that identifies what data is coming back
/// through the \a aDataCallback.
///
/// \param[in] aEvent If \a aBlocking is \c true, pass in a handle to an OS event which will be
/// set once the operation finishes.
///
/// \param[in] aEventCallback A user-defined #MMMReaderEventCallback function which will
/// receive UHF event #UHF_READ_COMPLETE or #UHF_READ_TIMEOUT
///
/// \return #NO_ERROR_OCCURRED if successful, otherwise a #MMMReaderErrorCode stating what
/// error occurred.
///
/// \attention This API will only return data if a UHF tag is found in the reader field
///
/// \remarks \a aParam can be used to provide some context data back through the callbacks.
/// One example could be the \c this pointer from a C++ class; in this case, because the
/// callback functions must be C-style functions, \a aParam would be able to access elements
/// of its class type.
///
/// \remarks \a aSequenceNum is a user-defined value which is passed straight into
/// \a aDataCallback. This parameter is provided so that it is possible to have one data
/// callback defined for multiple data items; the defined function can then filter the
/// \a aSequenceNum as appropriate.
///
/// \sa MMMReader_UHFInitialise()
MMM_READER_LOW_LEVEL_API MMMReaderErrorCode MMMReader_UHFGetMemoryFromMap(
	bool  aBlocking,
	unsigned char *aUID,
	unsigned int aUIDLen,
	MMMReaderDataCallback  aDataCallback,
	void *  aParam,
	int aSequenceNum,
	HANDLE  aEvent,
	MMMReaderEventCallback aEventCallback
);

/// \cond PAGESDK

/// Gets scanner hardware type about reader that is currently connected.
///
/// \ingroup LowLevelModule
///
/// \note This API is only relevant if using the \llapi.
///
/// \return The HardwareDeviceType value for the currently connected reader.
///
/// \remarks This API should only be used after calling MMMReader_CameraInitialise() to
/// ensure a correctly populated parameter.
///
/// \sa MMMReader_LL_GetConnectedScanners()

MMM_READER_LOW_LEVEL_API const RTCHAR *MMMReader_LL_GetScannerHardwareDeviceType();

/// Feeds a document using the motor feed of \pagereader.
///
/// \ingroup CameraModule
///
/// \note This API is only relevant if using a \pagereader with the \llapi.
///
/// \param[in] aBlocking Specify whether this operation should be blocking or non-blocking;
/// if \c false, then this API will add a command to the queue to be carried out at a later
/// time and return immediately.
///
/// \param[in] aSettings A pointer to a CAMERASET structure which should contain the required
/// settings for the \pagereader.
///
/// \param[in] aFeedCompleteEventCallback A user-defined #MMMReaderEventCallback function
/// which will receive a DOC_FEED_COMPLETE event when a feed is successfully completed.
///
/// \param[in] aCallbackParam An optional pointer to some user-defined data. This is passed
/// on to the callback provided.
///
/// \param[in] aCommandCompleteEvent If \a aBlocking is \c true, pass in a handle to an OS
/// event which will be set once the operation finishes.
///
/// \return #NO_ERROR_OCCURRED if successful, otherwise a #MMMReaderErrorCode stating what
/// error occurred.
///
/// \remarks \a aCommandCompleteEvent can be used to provide some context data back through
/// the callbacks. One example could be the \c this pointer from a C++ class; in this case,
/// because the callback functions must be C-style functions, \a aCommandCompleteEvent would
/// be able to access elements of its class type.
///
/// \remarks Typically, the required settings structures are taken from #MMMReaderSettings by
/// using MMMReader_LL_LoadSettings(). However, in some applications it may be desirable
/// to adjust the settings.
///
MMM_READER_LOW_LEVEL_API MMMReaderErrorCode MMMReader_CameraFeedDocument(
	bool aBlocking,
	CameraSettings *aSettings,
	MMMReaderEventCallback aFeedCompleteEventCallback,
	void *aCallbackParam,
	HANDLE aCommandCompleteEvent
);

/// Read the magnetic stripe from a document using a \pagereader.
///
/// \ingroup CameraModule
///
/// \note This API is only relevant if using a \pagereader with the \llapi.
///
/// \param[in] aBlocking Specify whether this operation should be blocking or non-blocking;
/// if \c false, then this API will add a command to the queue to be carried out at a later
/// time and return immediately.
///
/// \param[in] aDataCallback A user-defined #MMMReaderDataCallback function which will receive
/// the magnetic data as an #MMMReaderMsrData structure containing all parsed data.
///
/// \param[in] aParam An optional pointer to some user-defined data. This is passed on to the
/// callback provided.
///
/// \param[in] aSequenceNum A user-defined value that identifies what data is coming back
/// through the \a aDataCallback.
///
/// \param[in] aEvent If \a aBlocking is \c true, pass in a handle to an OS event which will be
/// set once the operation finishes.
///
/// \return #NO_ERROR_OCCURRED if successful, otherwise a #MMMReaderErrorCode stating what
/// error occurred.
///
/// \remarks The magnetic data will be available after a document has been fed using
/// the motor feed. The #MMMReaderHardwareConfig::puHasMotorFeed property can be used
/// to determine whether the \pagereader has a motor feed.
///
/// \sa MMMReader_CameraFeedDocument()
MMM_READER_LOW_LEVEL_API MMMReaderErrorCode MMMReader_CameraReadMagStripe(
	bool aBlocking,
	MMMReaderDataCallback aDataCallback,
	void *aParam,
	int aSequenceNum,
	HANDLE aEvent
);

/// Converts a colour bmp into a grayscale bmp
///
/// \ingroup ImageProcModule
///
/// \note This API is only relevant if using a \pagereader with the \llapi.
///
/// \param[in] aSrcBitmap Specify the input colour bmp.
///
/// \param[out] aDstBitmap Specify the resulting grayscale bmp.
///
/// \return #NO_ERROR_OCCURRED if successful, otherwise a #MMMReaderErrorCode stating what
/// error occurred.
///
#ifdef ENABLE_GDIPLUS
MMM_READER_LOW_LEVEL_API MMMReaderErrorCode MMMReader_ConvertToGreyscale(
	Gdiplus::Bitmap *aSrcBitmap,
	Gdiplus::Bitmap **aDstBitmap
);
#endif // ENABLE_GDIPLUS

// Adjust Ocr and DocPos settings based on image dpi of 400 and scaled for aDPI.
/// \cond PAGESDK

/// Adjust Ocr and DocPos settings based on image dpi of 400 and scaled for aDPI..
///
/// \ingroup LLSettingsModule
///
/// \note This API is only relevant if using the \llapi.
///
/// \param[in] aDPI Image dpi to adjust values to
/// to save.
/// \param[in] aDocPosSettings A pointer to a DocPosSettings structure which contains the settings
/// to save.
/// \param[in] aDocPosSettingsSide2 A pointer to a DocPosSettings structure which contains the settings
/// to save.
/// \param[in] aOcrSettings A pointer to a OcrSettings structure which contains the settings
/// to save.
///
/// \return #NO_ERROR_OCCURRED if successful, otherwise a #MMMReaderErrorCode stating what
/// error occurred.
//////
/// \sa MMMReader_LL_InitialiseSettings()
MMM_READER_LOW_LEVEL_API MMMReaderErrorCode MMMReader_LL_AdjustSettingsByDPI(
	int aDPI,
	DOCPOS* aDocPosSettings,
	DOCPOS* aDocPosSettingsSide2,
	OCRSET* aOcrSettings
);

/// Parses an ICAO codeline string into separate field values, includes the old style checksum results
///
/// \ingroup ParsingModule
///
/// \note This API is only relevant if using the \llapi.
///
/// \param[in] aCodeline A null-terminated string providing the codeline to parse.
///
/// \param[in] aFlags A bitwise flag set specifying certain options used to configure this
/// API. See #MMMParseCodelineFlag for more details. ICAOFIELDOPOSNS is for when you know
/// the identity of the document and can specify where all the field positions are. e.g.
/// for when it doesn't conform to the ICAO standard
///
/// \param[out] aCsResult Gets filled in with the old style overall checksum result
///
/// \param[out] aCsArrayString Gets filled in with the old style overall checksum string
///
/// \param[in] aCsArrayStringLength Gets filled in with the size of aCsArrayString. This should
/// be set to MAX_CHECKDIGIT_LENGTH or it might fail with too small
///
/// \param[in] aPosns context rule to use or NULL for no context
///
/// \param[out] aCodelineData A pointer to a MMMReaderCodelineData structure which will contain
/// the results.
///
/// \return #NO_ERROR_OCCURRED if successful, otherwise a #MMMReaderErrorCode stating what
/// error occurred.
///
/// \attention This API does \b not require that the \sdk parser module be initialised first
/// via MMMReader_ParseInitialise().
///
/// \remarks The codeline may come from the following sources:
///		- From MMMReader_ImageReadCodeline() or MMMReader_ImageReadCodelineTwoSided
///		- From DG1 of an RFID chip, which can be accessed via the MMMReader_RFDecodeDataGroup()
///
/// \remarks This API will not fail if \a aCodeline is not a valid codeline. If \a aCodeline
/// is invalid or cannot be parsed, all values in \a aCodelineData will be set to empty strings
/// and zero values.
///
/// \sa MMMReaderCodelineData
MMM_READER_LOW_LEVEL_API MMMReaderErrorCode MMMReader_ParseCodelineWithCsums(
	const char* aCodeline,
	MMMParseCodelineFlag aFlags,
	MMMReaderCodelineData* aCodelineData,
	int *aCsResult,
	int *aCsArrayString,
	int *aCsArrayStringLength,
	CODELINECONTEXT *aPosns
);


/// Enable or disable dirt detection depending on the parameters passed in
///
/// \ingroup LLCameraAPI
///
/// \note This API is only relevant if using the \llapi.
///
/// \param[in] aDirtDetectSettings - the dirt detection settings
///
/// \return #NO_ERROR_OCCURRED if successful, otherwise a #MMMReaderErrorCode stating what
/// error occurred.
//////
MMM_READER_LOW_LEVEL_API MMMReaderErrorCode MMMReader_EnableDisableDirtDetection(
	DIRTDETECTSET *aDirtDetectSettings
);


/// Find out if the dirt detection is running or not
///
/// \ingroup LLCameraAPI
///
/// \note This API is only relevant if using the \llapi.
///
/// true if dirt detection is running and false otherwise
//////
MMM_READER_LOW_LEVEL_API bool MMMReader_IsDirtDetectionRunning(
);

/// Enable or disable support for TWAIN scanners (currently only the ID150). Default is enabled.
///
/// \ingroup LLCameraAPI
///
/// \note This API is only relevant if using the \llapi.
///
/// \param[in] aEnabled - indicates whether the TWAIN scanners should be supported or not.
///
/// \return #NO_ERROR_OCCURRED if successful, otherwise a #MMMReaderErrorCode stating what
/// error occurred.
//////
MMM_READER_LOW_LEVEL_API MMMReaderErrorCode MMMReader_EnableTwainScanners(bool aEnabled);

/// Ejects the document from readers supporting this action, such as the CR5400.
///
/// \ingroup CameraModule
///
/// \note This API is only relevant if using a \pagereader with the \llapi.
///
/// \return \c no return information.
///
MMM_READER_LOW_LEVEL_API void MMMReader_EjectDocument();

/// Inserts the document into readers supporting this action, such as the CR5400.
///
/// \ingroup CameraModule
///
/// \note This API is only relevant if using a \pagereader with the \llapi.
///
/// \return \c no return information.
///
MMM_READER_LOW_LEVEL_API void MMMReader_InsertDocument();

/// Clears the document counter on readers supporting this action, such as the CR5400.
/// Call MMMReader_DocumentProcessingComplete to increment counter.
///
/// \ingroup CameraModule
///
/// \note This API is only relevant if using a \pagereader with the \llapi.
///
/// \return \c no return information.
///
MMM_READER_LOW_LEVEL_API void MMMReader_ClearDocumentCounter();

/// Gets the current document counter value on readers supporting this action, such as the CR5400.
/// Call MMMReader_DocumentProcessingComplete to increment counter.
///
/// \ingroup CameraModule
///
/// \note This API is only relevant if using a \pagereader with the \llapi.
///
/// \return document count.
///
MMM_READER_LOW_LEVEL_API int MMMReader_GetDocumentCount();

/// Informs caller if the document is ready to insert on motorized readers, such as the CR5400.
///
/// \ingroup CameraModule
///
/// \note This API is only relevant if using a \pagereader with the \llapi.
///
/// \return TRUE if the document is ready to insert.
///
MMM_READER_LOW_LEVEL_API bool MMMReader_DocumentReadyToInsert();

/// Dump all members within the MMMReaderSettings data struct to a text file.
///
/// \ingroup LLSettingsModule
///
/// \note This API is only relevant if using the \llapi.
///
/// \param[in] aTheSettings - pointer to MMMReaderSettings struct to dump.
///
/// \param[in] aPathFileName - full path directory/filename to write the text data to.
///
/// \return #NO_ERROR_OCCURRED if successful, otherwise a #MMMReaderErrorCode stating what
/// error occurred.
//////
MMM_READER_LOW_LEVEL_API MMMReaderErrorCode MMMReader_LL_WriteSettingsTextfile(MMMReaderSettings *aTheSettings, char *aPathFileName);

/// Return the gDocDetectData.docState member.
///
/// \ingroup CameraModule
///
/// \note This API is only relevant if using a \pagereader with the \llapi.
///
/// \return value of gDocDetectData.docState (MOVEDOC=-2, NODOC=-1, SAMEDOC=0, NEWDOC=1)
///
MMM_READER_LOW_LEVEL_API int MMMReader_GetDetectDocumentState();

/// Is the reader locked.
///
/// \ingroup CameraModule
///
/// \note This API is only relevant if using the \llapi.
///
/// \return TRUE if the reader is locked
//////
MMM_READER_LOW_LEVEL_API bool MMMReader_LL_IsReaderLocked();

/// Unlocks a locked reader.
///
/// \ingroup LLCameraModule
///
/// \note This API is only relevant if using the \llapi.
///
/// \param[in] key - The value of the key used to lock the reader.
///
/// \return #NO_ERROR_OCCURRED if successful, otherwise a #MMMReaderErrorCode stating what
/// error occurred.
//////
MMM_READER_LOW_LEVEL_API bool MMMReader_LL_UnlockReader(const char *key);

/// \endcond
// PAGESDK
//-------------------------------------------------------------------------------------------

#ifdef __cplusplus
 }
#endif

#endif

