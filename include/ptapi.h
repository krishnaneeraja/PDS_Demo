typedef char PT_CHAR;//signed byte

typedef unsigned char PT_BYTE; //unsigned byte

typedef short    PT_SHORT;     //signed 2 bytes

typedef unsigned short PT_WORD;//unsigned 2 bytes

typedef long  PT_LONG;         //signed 4 bytes

typedef unsigned long PT_DWORD;//unsigned 4 bytes

typedef unsigned long PT_BOOL; //Boolean value (zero, non-zero)

typedef PT_LONG  PT_STATUS;    //standard result value

typedef PT_DWORD PT_CONNECTION;//connection to FM

typedef PT_DWORD PT_DEVNOTIFY;     //Device notification handle

typedef PT_DWORD PT_SECURITY_LEVEL;//Security level type

typedef PT_DWORD PT_ANTISPOOFING_LEVEL;//Antispoofing level type

typedef PT_DWORD PT_CALLBACK_LEVEL;//GUI callbacks level type

#define PT_SYSTEM_NAME_MAX_LEN	0x100	//user defined
#define PT_GUID_SIZE		0x100	    //user defined

typedef struct pt_extended_info_v1 {
     PT_DWORD Version;
     PT_BYTE GUID[PT_GUID_SIZE];
     PT_DWORD SystemId;
     PT_CHAR SystemName[PT_SYSTEM_NAME_MAX_LEN];
} PT_EXTENDED_INFO_V1;

typedef PT_EXTENDED_INFO_V1     PT_EXTENDED_INFO;

//By default PT_EXTENDED_INFO is defined as version 1

#define DSNSUBSTR_MAXLEN                                   260

#define DISPLAYINFO_MAXLEN                                 128

#define OTHERINFO_MAXLEN                                   96

#define	PT_PURPOSE_ENROLL	                               0x03

#define PT_PURPOSE_VERIFY                                  0x01

#define PT_TRUE			                                   0x01

#define	PT_STATUS_GENERAL_ERROR	                           0xFC17	//-1001

#define PT_STATUS_OK 		                               0

#define PT_FULLBIR_INPUT	                               3

#define	PT_SLOT_INPUT		                               128

#define PT_FALSE                                           -1

typedef struct pt_device_list_item {
     PT_CHAR DisplayInfo[DISPLAYINFO_MAXLEN];
     PT_CHAR DsnSubString[DSNSUBSTR_MAXLEN];
     PT_CHAR OtherInfo[OTHERINFO_MAXLEN];
     PT_DWORD DeviceType;
     PT_BYTE reserved[28];
} PT_DEVICE_LIST_ITEM;

/*
Description
DisplayInfo[DISPLAYINFO_MAXLEN]
String with displayable device info
DsnSubString[DSNSUBSTR_MAXLEN]
String usable as part of DSN for PTOpen() to connect to this device
OtherInfo[OTHERINFO_MAXLEN]
String with other info
DeviceType
Device type
reserved[28]
Reserved for future use
*/

/*
The format of the data returned by PTEnumerateDevices, it contains info about all enumerated devices.
Please note, that the real output parameter from PTEnumerateDevices has variable length - array List[]
has NumDevices items. The PT_DATA structure is used to associate any arbitrary long data block with
the length information.
*/
typedef struct pt_device_list {
     PT_DWORD NumDevices;
     PT_DEVICE_LIST_ITEM List[1];
} PT_DEVICE_LIST;

/*

Description
NumDevices
Number of devices in the list
List[1]
The list of devices, variable length

*/

//The PT_DATA structure is used to associate any arbitrary long data block with the length information.
typedef struct pt_data {
     PT_DWORD Length;
     PT_BYTE Data[1];
} PT_DATA;
/*

Description
Length
Length of the Data field in bytes
Data[1]
The data itself, variable length

*/

typedef struct pt_bir_header {
     PT_DWORD Length;
     PT_BYTE HeaderVersion;
     PT_BYTE Type;
     PT_WORD FormatOwner;
     PT_WORD FormatID;
     PT_CHAR Quality;
     PT_BYTE Purpose;
     PT_DWORD FactorsMask;
} PT_BIR_HEADER;

typedef struct pt_bir {
     PT_BIR_HEADER Header;
     PT_BYTE Data[1];
} PT_BIR;

typedef struct pt_input_bir {
     	PT_BYTE byForm;
  	union {
  		PT_BIR *pBIR; ///< Used when byForm = PT_FULLBIR_INPUT */
  		PT_LONG lSlotNr; ///< Used when byForm = PT_SLOT_INPUT */
  		PT_BYTE abyReserved[20]; /** For future use */
 	} InputBIR;
} PT_INPUT_BIR;

/*
Function-Prototypes
*/
typedef void* (*PT_MALLOC)(PT_DWORD);
typedef void (*PT_FREE)(void *);



typedef struct pt_memory_funcs {
	PT_MALLOC pfnMalloc;
 	PT_FREE pfnFree;
} PT_MEMORY_FUNCS;

//Added here on 041109 by CNU //

/*The global information about this PTAPI library, especially the version info */
typedef struct pt_global_info {
	PT_DWORD ApiVersion;    //Version of PTAPI. Highest byte = major version, second highest byte = minor version, low word = subversions 
	PT_DWORD Functionality; //Bitmask, defining which blocks of functions are supported
	PT_DWORD Flags;         //additional flag bitmask (e.g. encryption strength
} PT_GLOBAL_INFO;

/*A structure used by Info to return status-independentinformation about FM*/
typedef struct pt_info {
	PT_DWORD FwVersion;
	PT_DWORD FwMinNextVersion;
	PT_DWORD FwVariant;
	PT_DWORD FwFunctionality;
	PT_DWORD FwConfig;
	PT_DWORD Id;
	PT_DWORD AuthentifyId;
	PT_DWORD Usage;
	PT_DWORD SensorType;
	PT_WORD ImageWidth;
	PT_WORD ImageHeight;
	PT_DWORD MaxGrabWindow;
	PT_DWORD CompanionVendorCode;
} PT_INFO;


/*Swipe information structure*/
typedef struct pt_swipe_info {
	PT_SHORT RealHeight;
	PT_SHORT BackgroundColor;
	PT_SHORT ROIPos;
	PT_SHORT ROILength;
	PT_WORD RecErrorStates;
	PT_WORD QualityMessage;
	PT_CHAR RecScore;
	PT_CHAR QualityScore;
	PT_CHAR InvertedSwipe;
	PT_BYTE NoiseRobustnessFlags;
	PT_BYTE reserved[8];
} PT_SWIPE_INFO;

/*A structure used by PTExtendedInfo to return additional information about FM (version 2)*/
typedef struct pt_extended_info_v2 {
	PT_DWORD Version;
	PT_BYTE GUID[PT_GUID_SIZE];
	PT_DWORD SystemId;
	PT_CHAR SystemName[PT_SYSTEM_NAME_MAX_LEN];
	PT_DWORD FwFunctionality2;
	PT_DWORD BioLibVersion;
	PT_DWORD TemplateTypeFlags;
	PT_WORD FwUpdateCount;
	PT_BYTE reserve[14];
} PT_EXTENDED_INFO_V2;




///////////////////////////////////

PT_STATUS PTInitialize( PT_MEMORY_FUNCS *pMemoryFuncs );

PT_STATUS PTTerminate(void);


PT_STATUS PTEnumerateDevices(
    PT_CHAR *pszEnumDsn,
    PT_DEVICE_LIST **ppDeviceList
);

PT_STATUS PTClose(
    PT_CONNECTION hConnection	//Handle opened recently
);

PT_STATUS PTOpen(
    PT_CHAR *pszDsn,		//Interface details string i.e serial or usb  
    PT_CONNECTION *phConnection //Connection handle 
);


PT_STATUS PTEnroll(
    PT_CONNECTION hConnection,		
    PT_BYTE byPurpose,			//Purpose of Enrollment
    PT_INPUT_BIR *pStoredTemplate,	//Reserved						( NULL )
    PT_BIR **ppNewTemplate,		//Points to the BIR Template				( NULL if Slot number used)  
    PT_LONG *plSlotNr,			//Slot Number which is stored in FM's template database
    PT_DATA *pPayload,			//Data to be embedded into the template 		( NULL )
    PT_LONG lTimeout,			//Default timeout -1
    PT_BIR **ppAuditData,		//Reserved						( NULL )
    PT_DATA *pSignData,			//Reserved						( NULL )
    PT_DATA **ppSignature		//Reserved						( NULL )
);

PT_STATUS PTVerify(
    PT_CONNECTION hConnection,		
    PT_LONG *plMaxFARRequested,		//False Accept Ratio,ignored
    PT_LONG *plMaxFRRRequested,		//False Reject Ratio,ignored
    PT_BOOL *pboFARPrecedence,		//Depends on the above two parameters, if they are not specified this is also ignored
    PT_INPUT_BIR *pStoredTemplate,	//Points to the Template data/Slot Number
    PT_BIR **ppAdaptedTemplate,		//Reserved ( NULL )
    PT_BOOL *pboResult,			//The result Match (PT_TRUE)/No match (PT_FALSE)
    PT_LONG *plFARAchieved,		//Value of FAR achieved NULL
    PT_LONG *plFRRAchieved,		//Value of FRR achieved NULL
    PT_DATA **ppPayload,		//Points to the payload data to be embedded	( NULL if we do not specify in enroll )
    PT_LONG lTimeout,			//Timeout ( -1 default )
    PT_BOOL boCapture,			//Match with live fingerprint / last good template
    PT_BIR **ppAuditData,		//Reserved ( NULL )
    PT_DATA *pSignData,			//Reserved ( NULL )
    PT_DATA **ppSignature		//Reserved ( NULL )
);


/////////////////////////////////
PT_STATUS PTCapture(
    PT_CONNECTION hConnection,
    PT_BYTE byPurpose,           //
    PT_BIR **ppCapturedTemplate, //
    PT_LONG lTimeout,            //Timeout ( -1 default )
    PT_BIR **ppAuditData,        //Reserved ( NULL )
    PT_DATA *pSignData,          //Reserved ( NULL )
    PT_DATA **ppSignature        //Reserved ( NULL )
);

PT_STATUS PTVerifyMatch(
	PT_CONNECTION hConnection,
	PT_LONG *plMaxFARRequested,
	PT_LONG *plMaxFRRRequested,
	PT_BOOL *pboFARPrecedence,
	PT_INPUT_BIR *pCapturedTemplate,
	PT_INPUT_BIR *pStoredTemplate,
	PT_BIR **ppAdaptedTemplate,
	PT_BOOL *pboResult,
	PT_LONG *plFARAchieved,
	PT_LONG *plFRRAchieved,
	PT_DATA **ppPayload
);

PT_STATUS PTGlobalInfo(
	PT_GLOBAL_INFO **ppGlobalInfo
);

PT_STATUS PTOpenDevice(
	PT_CHAR *pszDsn,
	PT_CONNECTION *phConnection
);

PT_STATUS PTOpenSession(
	PT_CONNECTION hConnection,
	PT_CHAR *pszDsn
);


//Not supported //
PT_STATUS PTScanQuality(
	PT_CONNECTION hConnection,
	PT_DWORD *pdwScanQuality
);

//Not supported//
PT_STATUS PTGetSwipeInfo(
	PT_CONNECTION hConnection,
	PT_SWIPE_INFO **ppSwipeInfo,
	PT_DATA **ppSkewInfo,
	PT_DATA **ppSpeedInfo
);

//NOt Supported//
PT_STATUS PTExtendedInfo(
    PT_CONNECTION hConnection,
	PT_EXTENDED_INFO **ppExtInfo
);

PT_STATUS PTInfo(
    PT_CONNECTION hConnection,
	PT_INFO **ppInfo
);

PT_STATUS PT_IDLE_CALLBACK(
	void *pIdleCallbackCtx,
	PT_BYTE *pbyResponse
);

PT_STATUS PTDetectFingerEx(
	PT_CONNECTION hConnection,
	PT_LONG lTimeout,
	PT_DWORD dwFlags
);


PT_STATUS PTVerifyMatch(
	PT_CONNECTION hConnection,
	PT_LONG *plMaxFARRequested,
	PT_LONG *plMaxFRRRequested,
	PT_BOOL *pboFARPrecedence,
	PT_INPUT_BIR *pCapturedTemplate,
	PT_INPUT_BIR *pStoredTemplate,
	PT_BIR **ppAdaptedTemplate,
	PT_BOOL *pboResult,
	PT_LONG *plFARAchieved,
	PT_LONG *plFRRAchieved,
	PT_DATA **ppPayload
);


PT_STATUS PTVerifyEx(
PT_CONNECTION hConnection,
PT_LONG *plMaxFARRequested,
PT_LONG *plMaxFRRRequested,
PT_BOOL *pboFARPrecedence,
PT_INPUT_BIR *pStoredTemplates,
PT_BYTE byNrTemplates,
PT_BIR **ppAdaptedTemplate,
PT_SHORT *pshResult,
PT_LONG *plFARAchieved,
PT_LONG *plFRRAchieved,
PT_DATA **ppPayload,
PT_LONG lTimeout,
PT_BOOL boCapture,
PT_BIR **ppAuditData,
PT_DATA *pSignData,
PT_DATA **ppSignature
);


PT_STATUS PT_STD_GUI_STATE_CALLBACK (
   void *pGuiStateCallbackCtx,
  PT_DWORD dwGuiState,
  PT_BYTE *pbyResponse,
  PT_DWORD dwMessage,
 PT_BYTE byProgress,
  void *pSampleBuffer,
  PT_DATA *pData
);


PT_LONG LoadDataFromFile( const char *pFileName, unsigned char **ppBufer, PT_LONG *pLenData );
