/**
 * @file bclib.h
 *
 * @brief Biodata Conversion Library interface
 * 
 * BCLib provides the interface for biometric data conversions
 * between UPEK proprietary formats, BioAPI and ANSI/ISO formats
 * 
 * Copyright (c) UPEK 2007-2009
 *
 * @author Petr Tahal
 */

#ifndef BCLIB_H
#define BCLIB_H

//------------------------------------------------------------------------------
// Definitions
//------------------------------------------------------------------------------

/** @cond */

#if (defined(_WIN32) || defined(_WIN32_WCE)) && !defined(WIN32)
#define WIN32
#endif

#if defined(BC_PLATFORM_TBX16) && !defined(_WIN16)
#define _WIN16
#endif

#if defined(_WIN16) && !defined(WIN16)
#define WIN16
#endif

#if defined(WIN16)

// Disable some level 4 warnings wor MSVC 1.5

#pragma warning (disable: 4001)
#pragma warning (disable: 4705)

#endif //#if defined(WIN16)

// Macro used for compile time checking

#define BC_COMPILE_TIME_ASSERT(cond, msg) typedef char msg[(cond) ? 1 : -1]

// This is a definition which has sole purpose of helping readability.
// It indicates that formal parameter is an input parameter.

#ifndef IN
#define IN
#endif

// This is a definition which has sole purpose of helping readability.
// It indicates that formal parameter is an output parameter.

#ifndef OUT
#define OUT
#endif

// This is a definition which has sole purpose of helping readability.
// It indicates that formal parameter is both input and output parameter.

#ifndef INOUT 
#define INOUT
#endif

//------------------------------------------------------------------------------
// Macros
//------------------------------------------------------------------------------

#ifdef __cplusplus

    #define BEGIN_EXTERNC extern "C" {
    #define END_EXTERNC   }

#else  // #ifdef __cplusplus

    #define BEGIN_EXTERNC
    #define END_EXTERNC

#endif // #ifdef __cplusplus
   
// Macros for declaration and definition of API functions 

#if defined(__SYMBIAN32__) && !defined(BC_FORCE_DYNAMIC)

#define BC_DECL_DLLEXP(ReturnType, Name, Params) ReturnType Name Params
#define BC_DECL_DLLIMP(ReturnType, Name, Params) ReturnType Name Params

#else

#if defined(_WIN64) && defined(BC_FORCE_DYNAMIC)
#define BC_DECL_DLLEXP(ReturnType, Name, Params) __declspec (dllexport) ReturnType __stdcall Name Params
#define BC_DECL_DLLIMP(ReturnType, Name, Params) __declspec (dllimport) ReturnType __stdcall Name Params
#endif //#if defined(WIN32) && defined(BC_FORCE_DYNAMIC)      

#if defined(WIN32) && defined(BC_FORCE_DYNAMIC)
#define BC_DECL_DLLEXP(ReturnType, Name, Params) __declspec (dllexport) ReturnType __stdcall Name Params
#define BC_DECL_DLLIMP(ReturnType, Name, Params) __declspec (dllimport) ReturnType __stdcall Name Params
#endif //#if defined(WIN32) && defined(BC_FORCE_DYNAMIC)      

#if defined(WIN16) && defined(BC_FORCE_DYNAMIC)
#define BC_DECL_DLLEXP(ReturnType, Name, Params) ReturnType __export Name Params
#define BC_DECL_DLLIMP(ReturnType, Name, Params) ReturnType Name Params
#endif //#if defined(WIN16) && defined(BC_FORCE_DYNAMIC)

#if defined(WIN32) && !defined(BC_FORCE_DYNAMIC)
#if defined(BC_PLATFORM_WIN32CE)
#define BC_DECL_DLLEXP(ReturnType, Name, Params) ReturnType __cdecl Name Params
#define BC_DECL_DLLIMP(ReturnType, Name, Params) ReturnType __cdecl Name Params
#else
#define BC_DECL_DLLEXP(ReturnType, Name, Params) ReturnType __stdcall Name Params
#define BC_DECL_DLLIMP(ReturnType, Name, Params) ReturnType __stdcall Name Params
#endif 
#endif // static lib under win32/win32ce

#if defined(WIN16) && !defined(FORCE_DYNAMIC)
#define BC_DECL_DLLEXP(ReturnType, Name, Params) ReturnType Name Params
#define BC_DECL_DLLIMP(ReturnType, Name, Params) ReturnType Name Params
#endif

#endif

// Trivial definitions of DECL_DLLEXP and DECL_DLLIMP, if not defined

#if !defined(BC_DECL_DLLEXP)
#define BC_DECL_DLLEXP(ReturnType, Name, Params) ReturnType Name Params
#endif 

#if !defined(BC_DECL_DLLIMP)
#define BC_DECL_DLLIMP(ReturnType, Name, Params) ReturnType Name Params
#endif 

// Defining and declaring macros for functions

#if defined(__SYMBIAN32__)

#define BC_DEFN_API_FUNC(ReturnType, Name, Params) ReturnType Name Params

#if defined(__cplusplus)

#if defined(BCLIB_EXPORT)
#define BC_DECL_API_FUNC(ReturnType, Name, Params) __declspec (dllexport) ReturnType Name Params
#else
#define BC_DECL_API_FUNC(ReturnType, Name, Params) __declspec (dllexport) static ReturnType Name Params
#endif

#else

#if defined(BCLIB_EXPORT)
#define BC_DECL_API_FUNC(ReturnType, Name, Params) __declspec (dllexport) ReturnType Name Params
#else
#define BC_DECL_API_FUNC(ReturnType, Name, Params) __declspec (dllexport) ReturnType Name Params
#endif

#endif

#else   // #ifdef __SYMBIAN32__

#ifdef BCLIB_EXPORT
#define BC_DEFN_API_FUNC(ReturnType, Name, Params) BC_DECL_DLLEXP(ReturnType, Name, Params)
#define BC_DECL_API_FUNC(ReturnType, Name, Params) BC_DECL_DLLEXP(ReturnType, Name, Params)
#else
#define BC_DEFN_API_FUNC(ReturnType, Name, Params) BC_DECL_DLLIMP(ReturnType, Name, Params)
#define BC_DECL_API_FUNC(ReturnType, Name, Params) BC_DECL_DLLIMP(ReturnType, Name, Params)
#endif  // #ifdef BCLIB_EXPORT

#endif  // #ifdef __SYMBIAN32__

/** @endcond */

//------------------------------------------------------------------------------
// Error codes
//------------------------------------------------------------------------------

/** @defgroup errorcodes Error Codes
 *
 *  @brief BCLib error codes that can be returned by any API function.
 *  
 *  All functions of the API are designed the way that they return 
 *  a sint32 value as a return code. In all the cases the return code
 *  signals only the success or failure of the operation. All eventual 
 *  outputs of the function are passed via output arguments.
 *  
 *  In order to allow maximum flexibility the error codes of BCLib 
 *  are offseted. Hopefully this allows the caller to better fit it 
 *  into its own error code space.
 *
 * @{
 */

/** @brief This is the standard offset of all codes.
 *
 *  This symbol allows to eventually change the offset of BCLib error 
 *  codes if required.
 */
#define BCERR_STD_OFFSET                                    (-30000)

/** @brief OK status
 *
 *  Signals a successful completion of the operation. This is the only
 *  code which is fixed and not affected by #BCERR_STD_OFFSET value.
 */
#define BCERR_OK                                            (0)

/** @brief Generic error
 *
 *  A code for an unknown or unexpected error. It should not be returned in 
 *  a non-experimental version of the library. If it is not true, please contact 
 *  the authors.
 */
#define BCERR_ERROR                                         (BCERR_STD_OFFSET - 1)

/** @brief Memory allocation failure
 *
 * This error code signals inability to allocate a block of memory. Specifically
 * the code can be returned in the following situations:
 * - The library was not able to allocate some of the memory pools, which are used
 *   for the internal dynamic memory allocation during the execution of the given
 *   API function call.
 * - The internal memory allocation exceeded the space available in the given 
 *   internal memory pool.
 * - Library was forced to allocate a memory block of size zero.
 */
#define BCERR_MEMORY_ALLOCATION                             (BCERR_STD_OFFSET - 2)

/** @brief Bad parameter was provided
 *
 * This code indicates an incorrect input was provided to the API function.
 * Most usually it is returned when some argument of the function is passed
 * as NULL, when it is not allowed.
 */
#define BCERR_BAD_PARAMETER                                 (BCERR_STD_OFFSET - 3)

/** @brief Functionality is not supported by this library version
 *
 * Functionality or type of information is not supported by the given version or
 * variant of the library. 
 */
#define BCERR_NOT_SUPPORTED                                 (BCERR_STD_OFFSET - 5)

/** @brief Provided buffer is too small 
 *
 * This code signals when too small output buffer is provided to the function
 * so the potential result of the operation cannot fit it. 
 */
#define BCERR_BUFFER_TOO_SMALL                              (BCERR_STD_OFFSET - 6)

/** @brief Unknown envelope type 
 *
 * Unknown envelope type was privided or requested
 */
#define BCERR_UNKNOWN_ENVELOPE                              (BCERR_STD_OFFSET - 7)

/** @brief Unknown data type 
 *
 * Unknown data type was provided or requested
 */
#define BCERR_UNKNOWN_DATA                                  (BCERR_STD_OFFSET - 8)

/** @brief Error in input decoding
 *
 */
#define BCERR_DECODING_INPUT                                (BCERR_STD_OFFSET - 9)

/** @brief Error in output encoding
 *
 */
#define BCERR_ENCODING_OUTPUT                               (BCERR_STD_OFFSET - 10)

/** @brief Error in data format
 *
 */
#define BCERR_BAD_DATA_FORMAT                               (BCERR_STD_OFFSET - 11)

/** @brief Missing information 
 *
 * Some necessary information is missing in input data and suplementary 
 * structure was not provided.
 */
#define BCERR_MISSING_INFORMATION                           (BCERR_STD_OFFSET - 12)

/** @brief Internal error
 *
 */
#define BCERR_INTERNAL                                      (BCERR_STD_OFFSET - 99)

/**  @} */

//------------------------------------------------------------------------------
// Types
//------------------------------------------------------------------------------

#ifndef BC_USE_EXTERNAL_TYPES

// Define necessary types

#if defined(__GNUC__)

// GNU compiler used

typedef signed int          sint32;
typedef unsigned int        uint32;

#else

// Other platforms

typedef signed long         sint32;
typedef unsigned long       uint32;

#endif

typedef signed short        sint16;
typedef unsigned short      uint16;

#endif

// Checking proper size of types

BC_COMPILE_TIME_ASSERT(sizeof(uint32) == 4, bc_uint32IsNot32bit);
BC_COMPILE_TIME_ASSERT(sizeof(sint32) == 4, bc_sint32IsNot32bit);
BC_COMPILE_TIME_ASSERT(sizeof(uint16) == 2, bc_uint16IsNot16bit);
BC_COMPILE_TIME_ASSERT(sizeof(sint16) == 2, bc_sint16IsNot16bit);

//------------------------------------------------------------------------------

/**
 * @defgroup bctemplateenvelope_enum Template Envelopes
 * @{
 * @brief Envelope types for biometric templates
 *
 */

typedef enum BCTemplateEnvelopeTag
{
    /// Not specified

    BCTE_NOT_SPECIFIED = -1,

    /// No envelope

    BCTE_NONE = 0,

    /// PT_BIR

    BCTE_PT_BIR,

    /// ABS_BIR

    BCTE_ABS_BIR,

    /// BioAPI_BIR

    BCTE_BIOAPI_BIR,

    /// Last value mark
    
    BCTE_LAST
    
} BCTemplateEnvelopeType;

/*@}*/

//------------------------------------------------------------------------------

/**
 * @defgroup bcimageenvelope_enum Image Envelopes
 * @{
 * @brief Envelope types for images
 *
 */

typedef enum BCImageEnvelopeTypeTag
{
    /// Not specified

    BCIE_NOT_SPECIFIED = -1,

    /// No envelope

    BCIE_NONE = 0,

    /// ABS_SAMPLE_IMAGE

    BCIE_ABS_IMAGE,

    /// BioAPI_GUI_BITMAP

    BCIE_BIOAPI_GUI_BITMAP,

    /// PT_DIF

    BCIE_PT_DIF,

    /// Last value mark
    
    BCIE_LAST
    
} BCImageEnvelopeType;

/*@}*/

//------------------------------------------------------------------------------

/**
 * @defgroup bctemplatedata_enum Image Envelopes
 * @{
 * @brief Inner data types for biometric template
 *
 */

typedef enum BCTemplateDataTypeTag
{
    /// Not specified

    BCT_NOT_SPECIFIED = -1,

    /// None - no data

    BCT_NONE = 0,

    /// UPEK legacy

    BCT_UPEK_LEGACY,

    /// UPEK alpha

    BCT_UPEK_ALPHA,

    /// UPEK beta

    BCT_UPEK_BETA,

    /// UPEK automatic - on input format is detected,
    /// on output the best format is chosen (alpha or beta)

    BCT_UPEK_AUTO,

    /// ANSI template

    BCT_ANSI,

    /// ISO template, Finger Minutiae Record format

    BCT_ISO_FMR,

    /// ISO template, Finger Minutiae Card format (normal)

    BCT_ISO_FMC_NORMAL,

    /// ISO template, Finger Minutiae Card format (compact)

    BCT_ISO_FMC_COMPACT,

    /// Last value mark
    
    BCT_LAST
    
} BCTemplateDataType;

/*@}*/

//------------------------------------------------------------------------------

/**
 * @defgroup bcimagedata_enum Image Envelopes
 * @{
 * @brief Inner data types for image
 *
 */

typedef enum BCImageDataTypeTag
{
    /// Not specified

    BCI_NOT_SPECIFIED = -1,

    /// None - value used when envelope is not specified

    BCI_NONE = 0,

    /// Raw image

    BCI_RAW,

    /// ANSI image

    BCI_ANSI,

    /// ISO image 

    BCI_ISO,

    /// Last value mark
    
    BCI_LAST
    
} BCImageDataType;

/*@}*/

//------------------------------------------------------------------------------

/** @brief Inner data types for image
 *
 */

typedef enum BCImageCompressionTypeTag
{
    /// Not specified

    BCIC_NOT_SPECIFIED = -1,

    /// Uncompressed

    BCIC_UNCOMPRESSED = 0,

    /// Uncompressed, bit packed

    BCIC_UNCOMPRESSED_BITPACKED = 1,

    /// Compressed via WSQ
    
    BCIC_COMPRESSED_WSQ = 2,

    /// Compressed via JPEG
    
    BCIC_COMPRESSED_JPEG = 3,

    /// Compressed via JPEG2000
    
    BCIC_COMPRESSED_JPEG2000 = 4,

    /// Compressed via PNG
    
    BCIC_COMPRESSED_PNG = 5

} BCImageCompressionType;
    
//------------------------------------------------------------------------------

/** @brief Image info type
 *
 */

typedef enum BCImageTypeTag
{
    BCIT_NOT_SPECIFIED = -1,
    BCIT_UNKNOWN = 0,
    BCIT_SWIPE_SENSOR = 1,
    BCIT_AREA_SENSOR = 2
    
} BCImageType;

//------------------------------------------------------------------------------

/** @brief Image info type.
 *
 * This structure serves as a supplementary source of information for conversion,
 * if some information requested by output format is not contained in input format.
 */

typedef struct
{
    /** @brief Structure version, must be 0x101.
     */
    sint32 version;
    
    /** @brief Capture system CBEFF product identifier.
     */
    uint32 captureCBEFFPid;

    /** @brief Compliance bits according to ANSI standard paragraph 6.4.5.
     */
    uint32 captureEquipmentCompliance;

    /** @brief Identificaiton of equipment assigned by its manufacturer
     */
    uint32 captureEquipmentId;

    /** @brief Image width. 
     */
    sint32  width;

    /** @brief Image height.
     */
    sint32  height;

    /** @brief Horizontal scan resolution (dpi) of the image. 
     */
    sint32  horizontalScanResolution;

    /** @brief Vertical scan resolution (dpi) of the image.
     */
    sint32  verticalScanResolution;

    /** @brief Horizontal image resolution (dpi). 
     */
    sint32  horizontalImageResolution;

    /** @brief Vertical image resolution (dpi).
     */
    sint32  verticalImageResolution;

    /** @brief Color bit depth of the image.
     */
    sint32  colorDepth;

    /** @brief The background color of the image.
     */
    sint32  bgColor;

    /** @brief Type of the sensor used for image acquisition.
     */
    BCImageType imageType;

    /** @brief Compression algorithm
     */
    BCImageCompressionType compression;

    /** @brief Image quality (0..255)
     */
    sint32 quality;

} BCImageInfoType_1;

//------------------------------------------------------------------------------

/** @brief Template info type
 *
 * This structure serves as a supplementary source of information for conversion,
 * if some information requested by output format is not contained in input format.
 */

typedef struct
{
    /** @brief Structure version, must be 1.
     */
    uint32 version;

    /** @brief Capture system CBEFF product identifier.
     */
    uint32 captureCBEFFPid;

    /** @brief Compliance bits according to ANSI standard paragraph 6.4.5.
     */
    uint32 captureEquipmentCompliance;

    /** @brief Identificaiton of equipment assigned by its manufacturer
     */
    uint32 captureEquipmentId;

    /** @brief Original image width (in pixels)
     */
    uint16 imageWidth;

    /** @brief Original image height (in pixels)
     */
    uint16 imageHeight;
    
    /** @brief Original image horizontal resolution (in dpi)
     */
    uint16 imageHorizontalResolution;

    /** @brief Original image vertical resolution (in dpi)
     */
    uint16 imageVerticalResolution;

    /** @brief Finger position as in ANSI/ISO
     */
    uint32 fingerPosition;

} BCTemplateInfoType_1;

//------------------------------------------------------------------------------
// Function prototypes
//------------------------------------------------------------------------------

BEGIN_EXTERNC

/** @defgroup apioverview API Overview
 *
 *  @brief Conversion operations as performed by BCLib API functions.
 *  
 * @{
 */
    
/** @brief Returns the version of the library.
 *  
 * This function provides the basic information about the
 * library version. It is the only function that can be called
 * outside of the BCLib session (i.e. without prior call to BCInit()).
 *
 * @param pMajor major version number
 * @param pMinor minor version number
 *
 * @return BCERR_OK upon success
 */

BC_DECL_API_FUNC(sint32, BCGetVersion,
(
    OUT   uint32  *pMajor, 
    OUT   uint32  *pMinor
));

/** @brief Converts biometric template data.
 *  
 * Converts template data from input type/envelope to output type/envelope.
 * Note: Conversion cannot use the same buffer both for input and output.
 *
 * @param inputEnvelope Input envelope type.
 * @param inputDataType Input data type.
 * @param pInput Pointer to input buffer.
 * @param inputLength Length of input buffer.
 * @param outputEnvelope Requested output envelope type.
 * @param outputDataType Requested output data type.
 * @param pOutput Pointer to output buffer allocated by caller. Can be NULL, if just
 *   information on length of necessary output buffer is requested.
 * @param pOutputLength Pointer to variable containing on input length of output buffer
 *   allocated by caller, on output it contains length of data. Can be used to determine
 *   necessary length of output buffer (if pOutput is set to NULL).
 * @param pSuplementaryInfo Pointer to structure containing suplementary information,
 *   missing in input data. Note: In order to allow to change the structure type
 *   without changing function signature, void pointer is used.
 *
 * @return BCERR_OK upon success
 */

BC_DECL_API_FUNC(sint32, BCConvertTemplate,
(
    IN     BCTemplateEnvelopeType inputEnvelope,
    IN     BCTemplateDataType inputDataType,
    IN     const void *pInput,
    IN     uint32 inputLength,
    IN     BCTemplateEnvelopeType outputEnvelope,
    IN     BCTemplateDataType outputDataType,
    INOUT  void *pOutput,
    INOUT  uint32 *pOutputLength,
    IN     const void *pSuplementaryInfo
));

/** @brief Converts biometric image data.
 *  
 * Converts image data/envelope from input type to output type/envelope.
 * Note: Conversion cannot use the same buffer both for input and output.
 *
 * @param inputEnvelope Input envelope type.
 * @param inputDataType Input data type.
 * @param pInput Pointer to input buffer.
 * @param inputLength Length of input buffer.
 * @param outputEnvelope Requested output envelope type.
 * @param outputDataType Requested output data type.
 * @param outputCompression Requested output compression. In current version
 *   this parameter is ignored and no compression/decompression is made
 * @param pOutput Pointer to output buffer allocated by caller. Can be NULL, if just
 *   information on length of necessary output buffer is requested.
 * @param pOutputLength Pointer to variable containing on input length of output buffer
 *   allocated by caller, on output it contains length of data. Can be used to determine
 *   necessary length of output buffer (if pOutput is set to NULL).
 * @param pSuplementaryInfo Pointer to structure containing suplementary information,
 *   missing in input data. Note: In order to allow to change the structure type
 *   without changing function signature, void pointer is used.
 *   
 * @return BCERR_OK upon success
 */

BC_DECL_API_FUNC(sint32, BCConvertImage,
(
    IN     BCImageEnvelopeType inputEnvelope,
    IN     BCImageDataType inputDataType,
    IN     const void *pInput,
    IN     uint32 inputLength,
    IN     BCImageEnvelopeType outputEnvelope,
    IN     BCImageDataType outputDataType,
    IN     BCImageCompressionType outputCompression,
    INOUT  void *pOutput,
    INOUT  uint32 *pOutputLength,
    IN     const void *pSuplementaryInfo
));

/** @} */

END_EXTERNC

#endif //#ifndef BCLIB_H
