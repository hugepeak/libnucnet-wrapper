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
//       in a Libnucnet__Nuc structure of nuclear species from data from
//       a URL, print out the data about the species, update the data
//       from another input XML file, print out the data again,
//       and clear the structure and free the allocated memory.
//     </abstract>
//   </description>
// </file>
//////////////////////////////////////////////////////////////////////////////*/
 
#include <wrapper_nuc.h>
#include <user_functions.h>

int
main( int argc, char **argv ) {

  libnucnet::Nuc *p_my_nuclei;

  if ( argc != 3 && argc != 4 ) {
      fprintf(
        stderr, "\nUsage: %s filename new_file xpath\n\n", argv[0]
      );
      fprintf(
        stderr, "  filename = input nuclear data xml file or url.\n\n"
      );
      fprintf(
        stderr,
        "  new_file = input nuclear data xml file or url with new data.\n\n"
      );
      fprintf(
        stderr, "  xpath = xpath expression (optional).\n\n"
      );
      return EXIT_FAILURE;
   }

  /*============================================================================
  // Create nuclear species collection.
  //==========================================================================*/

  if( argc == 3 ) {
    p_my_nuclei = new libnucnet::Nuc( argv[1], NULL );
  } else {
    p_my_nuclei = new libnucnet::Nuc( argv[1], argv[3] );
  }

  /*============================================================================
  // Print the nuclear data.
  //==========================================================================*/

  fprintf( stdout, "\nBefore update of nuclear data:\n\n" );
  libnucnet::user::print_nuclei( p_my_nuclei );

  /*============================================================================
  // Update the data from a local file.
  //==========================================================================*/

  p_my_nuclei->updateFromXml( argv[2] );

  /*============================================================================
  // Sort the nuclei by Z and A.
  //==========================================================================*/

  p_my_nuclei->sortSpecies();
    
  /*============================================================================
  // Print the nuclear data.
  //==========================================================================*/

  fprintf( stdout, "\nAfter update of nuclear data:\n\n" );
  libnucnet::user::print_nuclei( p_my_nuclei );

  /*============================================================================
  // Free the nuclear species collection.
  //==========================================================================*/

  delete p_my_nuclei;
  
  /*============================================================================
  // Done!
  //==========================================================================*/

  return EXIT_SUCCESS;

}

