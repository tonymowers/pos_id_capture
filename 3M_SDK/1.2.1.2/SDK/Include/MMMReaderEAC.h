#ifndef _MMMREADEREAC_H_
#define _MMMREADEREAC_H_

#ifdef __cplusplus
extern "C" {
#endif


#define		ERROR_RF_RESERVED1						ERROR_RF_CHIP_AUTHENTICATION_FAILED
#define		ERROR_RF_RESERVED2						ERROR_RF_CERTS_LOAD_FAILED
#define		ERROR_RF_RESERVED3						ERROR_RF_TERMINAL_AUTHENICATION_FAILED
#define		ERROR_RF_RESERVED4						ERROR_RF_UNAUTHORISED_SENSITIVE_DG

#define		CT_RESERVED1							CT_CVCA_CERT
#define		CT_RESERVED2							CT_DV_CERT
#define		CT_RESERVED3							CT_IS_CERT
#define		CT_RESERVED4							CT_IS_PRIVATE_KEY

#define     ECM_RESERVED1							ECM_SIGN_REQUEST
#define     ECM_RESERVED2							ECM_USE_3M_ISS

#define		RFID_ITEM_RESERVED1						RFID_EF_CVCA_FILE

#define		CD_RF_RESERVED1							CD_SCEF_CVCA_FILE
#define		CD_RF_RESERVED2							CD_SCTERMINAL_AUTHENTICATION_STATUS
#define		CD_RF_RESERVED3							CD_SCCHIP_AUTHENTICATION_STATUS

#define		puRFIDDataToSendReserved1				puGetTerminalAuthStatus
#define		puRFIDDataToSendReserved2				puGetChipAuthStatus
#define		puRFIDDataToSendReserved3				puEFCVCAFile

#define     puRFProcessReserved1					puEACEnabled
#define     puRFProcessReserved2					puExternalCVCertsMode
#define     puRFProcessReserved3					puExternalPrivateKeyMode
#define     puRFProcessReserved4					puEACVersion


#ifdef __cplusplus
 } 
#endif //__cplusplus

#endif //_MMMREADEREAC_H_
