/**
 * @file convert-template.c
 *
 * BCLib Sample
 *
 * Copyright (c) UPEK 2009
 */

#include "errno.h"
#include "stdio.h"
#include "stdlib.h"
#include<math.h>
#include "bclib.h"

//------------------------------------------------------------------------------
// Temporary definitions for quick test

char gUsageInfo[] = "\nUsage: %s <input-template-file> <output-template-file>";

//------------------------------------------------------------------------------
// Suplementary info containing data not present in UPEK template

BCTemplateInfoType_1 gSuplementInfoTpl = 
{
    // Structure version, must be 1.

    1,

    // Capture system CBEFF product identifier.

    0x12,
        
    // Compliance bits according to ANSI standard paragraph 6.4.5.

    0,

    // Identificaiton of equipment assigned by its manufacturer

    0,

    // Original image width (in pixels) - present in UPEK template

    0,

    // Original image height (in pixels) - present in UPEK template

    0,
    
    // Original image horizontal resolution (in dpi) - present in UPEK template

    0,

    // Original image vertical resolution (in dpi) - present in UPEK template

    0,

    // Finger position as in ANSI/ISO (2 = right index finger)

    2 
};

//------------------------------------------------------------------------------
// Allocate input buffer and load data from file

sint32 LoadDataFromFile( const char *pFileName, unsigned char **ppBufer, uint32 *pLenData )
{
    sint32 retVal = 0;
    FILE *fil = NULL;

    // Open file
    
    fil = fopen( pFileName, "rb" );
    if ( fil != NULL )
    {
        sint32 fileLength = 0;

        // Get file length
        
        fseek( fil, 0, SEEK_END );
        fileLength = ftell( fil );

        if ( fileLength > 0 )
        {
            // Allocate buffer for data
            
            *ppBufer = (unsigned char *) malloc( fileLength );
            *pLenData = (uint32) fileLength;

            // Seek back to the beginning of file

            fseek( fil, 0, SEEK_SET );

            // Read data from file
            
            fread( *ppBufer, 1, fileLength, fil );
        }
        else
        {
            // File length zero, return error
            
            retVal = -2;
        }

        // Close the file
        
        fclose( fil );
    }
    else
    {
        // File open error
        
        retVal = -1;
    }

    return retVal;
}

//------------------------------------------------------------------------------
// Store data to file

sint32 StoreDataToFile( const char *pFileName, const unsigned char *pBuf, uint32 lenData )
{
    sint32 retVal = 0;
    FILE *fil = NULL;

    // Open the file
    
    fil = fopen( pFileName, "wb" );
    if ( fil != NULL )
    {
        // File opened, write data
        
        fwrite( pBuf, 1, lenData, fil );

        // Close the file
        
        fclose( fil );
    }
    else
    {
        // File open error
        
        retVal = -1;
    }

    return retVal;
}

//------------------------------------------------------------------------------

int main( int argc, char* argv[] )
{

    if ( argc <= 2 )
    {
        printf( gUsageInfo, argv[0] );
    }
    else
    {
	convert(argv[1],argv[2]);
    }
}

int convert(char *inFile, char *outFile)    
{
    sint32 retVal = 0;

    uint32 versionMajor = 0;
    uint32 versionMinor = 0;

    unsigned char *pInputBuffer = NULL;
    unsigned char *pOutputBuffer = NULL;
    uint32 lenInput = 0;
    uint32 lenOutput = 0;

    retVal = BCGetVersion( &versionMajor, &versionMinor );
    // Get library version and print it
    printf( "\nBiodata Conversion Library version %d.%d convert template sample\n",
        versionMajor, versionMinor );

    // Check command line parameters count
    

        // Read template from file

        retVal = LoadDataFromFile( inFile, &pInputBuffer, &lenInput );
        if ( retVal < 0 )
        {
            printf( "Error reading file '%s' (code %d)\n", inFile, retVal );
        }
        else
        {
            // Get output data langth
    
            retVal = BCConvertTemplate( BCTE_NONE, BCT_ISO_FMR, 
                pInputBuffer, lenInput,
                BCTE_PT_BIR, BCT_UPEK_LEGACY,
                NULL, &lenOutput,
                NULL );

            if ( retVal != BCERR_OK )
            {
                // Cannot get output lenght, report error
                
                printf( "Error getting output length (code %d)\n",
                    retVal );
            }
            else
            {
                // Allocate memory 

                pOutputBuffer = (unsigned char *) malloc( lenOutput );

                // Call conversion
    
                retVal = BCConvertTemplate( BCTE_NONE, BCT_ISO_FMR, 
                    pInputBuffer, lenInput,
                    BCTE_PT_BIR, BCT_UPEK_LEGACY,
                    pOutputBuffer, &lenOutput,
                    NULL );
            
                if ( retVal == BCERR_OK )
                {
                    // Success, store data to file
                
                    StoreDataToFile( outFile, pOutputBuffer, lenOutput );
                    printf( "File '%s' converted to '%s'\n",
                        inFile, outFile );
                }
                else
                {
                    // Conversion failed, report error
                
                    printf( "Error converting file '%s' (code %d)\n",
                        inFile, retVal );
                }
            }
        }

    // Cleanup
    
    if ( pInputBuffer != NULL )
    {
        free( pInputBuffer );
    }

    if ( pOutputBuffer != NULL )
    {
        free( pOutputBuffer );
    }

    return retVal;
}

//------------------------------------------------------------------------------
