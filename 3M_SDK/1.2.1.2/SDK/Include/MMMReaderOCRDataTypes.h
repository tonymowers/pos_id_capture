#ifndef _MMMReaderOCRDataTypes_H_
#define _MMMReaderOCRDataTypes_H_

#define MAX_CODELINE_LENGTH 200	
#define MAX_CODELINE_FIELD_LENGTH	40
#define MAX_OPTIONAL_DATA_LENGTH	40
#define MAX_DATA_FIELD_LENGTH	1024
#define MAX_CHECKDIGIT_LENGTH 30
#define MAX_CHECKDIGITDATA_COUNT 5

#pragma pack(push, 1)


// idType options - MAKE SURE THESE STAY IN SYNC WITH DTYPES!
typedef enum DTYPESTRPOS {
	STR_PASSPORT, 
	STR_VISA, 
	STR_IDENTITY, 
	STR_RESIDENCE, 
	STR_DL, 
	STR_TRAVEL, 
	STR_VEHREG, 
	STR_CUSTOM, 
	STR_UNKNOWN
} dtypestrpos;

// context 
#define CNONE 0
#define CSOFT 0x01
#define CHARD 0x02
#define CAUSSIE 0x04
#define CFRENCH 0x08
#define CSPANISH 0x10
#define CWAIVERS 0x20
#define CSLV 0x40
#define CRUSVISA 0x80
#define CHONGKONG 0x100
#define CALB 0x200

typedef enum OCRTYPE
{
	ANY,	// A-Z 0-9 <
	ANYZ,	// any but turn all O's to zeros (hard context)
	ANYZ_EXTENDED, // any but turn all O's to zeros if extended checksum (hard context)
	ANYO,	// any but turn all zeros to O's (hard context)
	FALPHA,	// A-Z <
	ALPHA,	// A-Z
	FDIGIT,	// 0-9 <
	DIGIT,	// 0-9
	FGEN,	// MFU
	DGEN,	// MFU 0-9
	TYPE,	// PVICA
	SFALPHA,// Spanish font A-Z<
	SANY,	// Spanish font A-Z 0-9
} ocrtype;

typedef enum DOC_ID
{	
	MISC,   // any
	V2_36,  // 2 x 36 visas + misc 2 x 36 docs
	I3_30,  // 3 line id cards + misc 3 x 30 docs
	PV2_44, // passports + misc 2 x 44 docs
	V2_44,  // 2 x 44 visas
	I2_36,  // 2 line id cards
	X2_30,	// internal
	IRIS1,  // australian visa 1
	IRIS2,  // australian visa 2
	IFRA2_36,  // 2 x 36 french id
	IESP3_30,  // 3 x 30 spanish id
	CS3_30,	   // other 3 line id card starting with CS (ICAO) (used to distinguish CSCHN3_30 like CRHK3_30)
	CSCHN3_30, // Chinese 3x30 starting with CS or C< and non-ICAO
	CCHN3_30,  // 3  30 chinese id
	PESP1_47,  // 1 x 47 spanish passport
	PSLV2_44,  // El salvador passport
	VRUS2_44,  // russian visa
	ISVK2_34,  // 2 x 34 slovakian id card
	IROM2_36,  // 2 x 36 romanian id card
	HK1_8,     // 1 x 8 hong kong id
	PALB2_44,  // Albanian passport
	IALB3_30,  // Albanian id card
	CSGP2_30,  // 2 x 30 Singapore card
	PKOR2_44,  // korean passport
	FCHE2_30,  // swiss DL
	DPOL2_38,  // poslish vehicle reg card
	IEDL3_30,  // enhanced driving license
	PKEN2_44,  // kenyan passport
	PISR2_45,  // israel passport where there is noise on the end
	INUSA3_30, 
	CR3_30,    // other 3 line id card starting with CR
	CT3_30,    // other 3 line id card starting with CT
	CRHK3_30,  // Hong Kong / Macau border entry card
	CTHK3_30,  // Hong Kong / Macau border entry card (CT#)
	XUSA3_30,  // C1 C2 C3 C5 C7 IA USA docs
	DLSVK1_30, // Slovakian driving license
	CS1_30,	   // Chinese 1 x 30 ID card for Mainland to Hongkong and Macao
	CRFRA2_44, // French Vehicle Registration Certificate
	IDMEX3_30, // Mexican voter card IFE (to treat sex as ALPHA instead of ICAO FGEN to handle non-standard H (male) and F (female))
	DLNLD1_30, // Dutch Driver's License
	PK1_10,	// Custom 1 line of 10 (ANNNNNNN<N) from a Kazakstan passport (ex N1462421<7)
	//D1FRA1_30, // French Driver's License
	IDCUSTOM,  // for when it is a custom document as specified in the relevant structure
	IDSIZE     // internal
} doc_id;

typedef struct IcaoFields {
	int line;  // 1, 2 or 3 lines
	int start; // position in line of the first character (starts from zero)
	int len;	// length of the field
	int cs;		// position of the checksum (-1 for not applicable)
	int posnOverallCs; // position in the overall checksum 0,1,2,3,4,5 etc -1 for do not include
} ICAOFIELDS;


// Used for codelinedata splitting of the codeline into fields
typedef struct IcaoFieldPosns {
	//doc_id type; //IDCUSTOM if we are using this structure, any others if defined
	//int nLines;  // number of lines
	//int nChars;	 // number of characters
	ICAOFIELDS name;
	ICAOFIELDS firstname; // for when this is seperate
	ICAOFIELDS nationality;
	ICAOFIELDS docNum;
	ICAOFIELDS issueCtry;
	ICAOFIELDS dob;
	ICAOFIELDS sex;
	ICAOFIELDS doe;
	ICAOFIELDS optional;
	ICAOFIELDS optionalb;
	int overallcs;		// true if the last char is an overall checksum // FIXME bool
	int extendedcs;	// true if the doc number can extend into the optional data // FIXME bool
} ICAOFIELDPOSNS;



typedef struct variants {
	char id[6]; // identifier
	char idText[31]; // document text
	dtypestrpos idType; // standard defined text (see #defines above)
	char extendedCsIncludeFiller; // usually do not include filler
	char csumWarning; // 1 for warning, 0 for fail
} VARIANTS;


typedef struct codelinecontext // known context for a document
{
	doc_id doctype;		// set to IDCUSTOM for this context to apply. another to set that context
	VARIANTS variant[15];	// list all the diffferent variants for this document
	int nvariants;			// number of different id's, max = 15 
	int contextswitch;	// not needed for IDCUSTOM. context that has to be defined for this document e.g. CSLV
	int docnumCdigit;   // document number check digit position
	int cdigitNumChars; // number of characters in the doc number check digit calculation
	int nlines;			// number of lines
	int nchars;			// number of chars per line - 0 means nothing doctype
	ocrtype types[MAX_CODELINE_LENGTH];	// character type
	ICAOFIELDPOSNS datasplit; // field positions for codeline data breakdown - not used for OCR but used for codeline breakdown
} CODELINECONTEXT;

#pragma pack(pop)

#endif //_MMMReaderOCRDataTypes_H_