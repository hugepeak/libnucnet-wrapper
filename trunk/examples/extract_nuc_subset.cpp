/*//////////////////////////////////////////////////////////////////////////////
// <file type="public">
//   <license>
//     This file was originally written by Bradley S. Meyer.
//
//     This is free software; you can redistribute it and/or modify it
//     under the terms of the GNU General Public License as published by
//     the Free Software Foundation; either version 2 of the License, or
//     (at your option) any later version.
//
//     This software is distributed in the hope that it will be useful,
//     but WITHOUT ANY WARRANTY; without even the implied warranty of
//     MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//     GNU General Public License for more details.
//
//     Please see the src/README.txt file in this distribution for more
//     information.
//   </license>
//   <description>
//     <abstract>
//       Example to demonstrate how to use libnucnet routines to parse
//       in a libnucnet__Nuc structure of nuclear species from an input
//       xml file, extract a subset of those data by an xpath expression,
//       save the subset to a new file,
//       and clear the structure and free the allocated memory.
//     </abstract>
//   </description>
// </file>
//////////////////////////////////////////////////////////////////////////////*/
 
#include <wrapper_nuc.h>

int
main( int argc, char **argv ) {

  libnucnet::Nuc *p_my_nuclei, *p_subset;

  if ( argc != 4 ) {
      fprintf(
        stderr, "\nUsage: %s filename xpath new_filename\n\n", argv[0]
      );
      fprintf(
        stderr, "  filename = input nuclear data xml file or url.\n\n"
      );
      fprintf(
        stderr, "  xpath = xpath expression for subset.\n\n"
      );
      fprintf(
        stderr, "  new_filename = name of file to dump subset.\n\n"
      );
      return EXIT_FAILURE;
   }

  /*============================================================================
  // Create nuclear species collection.
  //==========================================================================*/

  p_my_nuclei = new libnucnet::Nuc( argv[1] );
    
  p_subset = p_my_nuclei->extractSubset( argv[2] );

  /*============================================================================
  // Free the original nuclear species collection.
  //==========================================================================*/

  delete p_my_nuclei;
  
  /*============================================================================
  // Write the subset to a new xml file.
  //==========================================================================*/

  p_subset->writeToXmlFile( argv[3] );

  /*============================================================================
  // Free the subset.
  //==========================================================================*/

  delete p_subset;
  
  /*============================================================================
  // Done!
  //==========================================================================*/

  return EXIT_SUCCESS;

}

