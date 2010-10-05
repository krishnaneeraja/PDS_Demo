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

  "\nUsage: %s <input-template-file> <output-template-file>";

//------------------------------------------------------------------------------
// Suplementary info containing data not present in UPEK template
  
{
  
    // Structure version, must be 1.
    
    // Capture system CBEFF product identifier.
    
    // Compliance bits according to ANSI standard paragraph 6.4.5.
    
    // Identificaiton of equipment assigned by its manufacturer
    
    // Original image width (in pixels) - present in UPEK template
    
    // Original image height (in pixels) - present in UPEK template
    
    // Original image horizontal resolution (in dpi) - present in UPEK template
    
    // Original image vertical resolution (in dpi) - present in UPEK template
    
    // Finger position as in ANSI/ISO (2 = right index finger)



//------------------------------------------------------------------------------
// Allocate input buffer and load data from file
  
			    uint32 * pLenData) 
{
  
  
  
    // Open file
    
  
    
    {
      
      
	// Get file length
	
      
      
	
	{
	  
	    // Allocate buffer for data
	    
	  
	  
	    // Seek back to the beginning of file
	    
	  
	    // Read data from file
	    
	
      
      else
	
	{
	  
	    // File length zero, return error
	    
	
      
	// Close the file
	
    
  
  else
    
    {
      
	// File open error
	
    
  



//------------------------------------------------------------------------------
// Store data to file
  
			   uint32 lenData) 
{
  
  
  
    // Open the file
    
  
    
    {
      
	// File opened, write data
	
      
	// Close the file
	
    
  
  else
    
    {
      
	// File open error
	
    
  



//------------------------------------------------------------------------------
#ifdef MAIN
  int
main (int argc, char *argv[]) 
{
  
    
    {
      
    
  
  else
    
    {
      convert (argv[1], argv[2]);
    
}
#endif

int
convert (char *inFile, char *outFile) 
{
  sint32 retVal = 0;
  
  
  
  
  
  
  
  
    // Get library version and print it
    printf
    ("\nBiodata Conversion Library version %d.%d convert template sample\n",
     
  
    // Check command line parameters count
    
    // Read template from file
    
  
    
    {
      
    
  
  else
    
    {
      
	// Get output data langth
	
	BCConvertTemplate (BCTE_NONE, BCT_ISO_FMR, 
			   
			   
      
	
	{
	  
	    // Cannot get output lenght, report error
	    
	
      
      else
	
	{
	  
	    // Allocate memory 
	    
	  
	    // Call conversion
	    
	    BCConvertTemplate (BCTE_NONE, BCT_ISO_FMR, 
			       lenInput, 
			       
	  
	    
	    {
	      
		// Success, store data to file
		
	      
	    
	  
	  else
	    
	    {
	      
		// Conversion failed, report error
		
			 retVal);
	    
	
    
  
    // Cleanup
    
    
    {
      
    
  
    
    {
      
    
  



//------------------------------------------------------------------------------