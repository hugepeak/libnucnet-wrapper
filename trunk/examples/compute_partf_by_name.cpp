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
//       Example to demonstrate how to use libnucnet routines to create a
//       libnucnet__Nuc structure of nuclear species from an input xml
//       file, print out the nuclear partition function for a species
//       selected by its name, and clear the structure and free the
//       allocated memory.
//     </abstract>
//   </description>
// </file>
//////////////////////////////////////////////////////////////////////////////*/

#include <wrapper_nuc.h>

int main( int argc, char * argv[] ) {

  int i;
  double d_t9;
  libnucnet::Nuc *p_my_nuclei;
  libnucnet::Species *p_species;

  if ( argc != 3 ) {
       fprintf(
        stderr, "\nUsage: %s filename nucname\n\n", argv[0]
     );
     fprintf(
       stderr, "  filename = input nuclear data xml filename or url\n\n"
     ); 
     fprintf(
       stderr, "  nucname = name of species\n\n"
     ); 
     return EXIT_FAILURE;
   }

  /*============================================================================
  // Get network from input xml file.
  //==========================================================================*/

  p_my_nuclei = new libnucnet::Nuc( argv[1] );

  /*============================================================================
  // Exit if species not in network.
  //==========================================================================*/

  if(
    !(
      p_species = p_my_nuclei->getSpeciesByName( argv[2] )
    )
  )
  {
    fprintf( stderr, "%s not in network!\n", argv[2] );
    return EXIT_FAILURE;
  }

  /*============================================================================
  // Print out data if species present in network.
  //==========================================================================*/

  printf( "\nFor %s\n\n", argv[2] ); 

  printf( "      t9        Partition Function\n" );
  printf( "    ------      ------------------\n\n" );

  for( i = 0; i < 31; i++ ) {
    d_t9 = pow( 10., -1 + ( (float) i ) /10. );
    printf("  %8.4f    %15.4e\n", d_t9,
      p_species->computePartitionFunction( d_t9 )
    );
  }

  /*============================================================================
  // Clean up and exit.
  //==========================================================================*/

  delete p_my_nuclei;

  return EXIT_SUCCESS;

}
