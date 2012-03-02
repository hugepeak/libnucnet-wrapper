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
//       in a Libnucnet__Nuc structure of nuclear species from an input
//       xml file, create a view and modify data included in the view,
//       and clear the structures and free the allocated memory.
//     </abstract>
//   </description>
// </file>
//////////////////////////////////////////////////////////////////////////////*/
 
#include <wrapper_nuc.h>
#include <user_functions.h>

int modify_species( libnucnet::Species *, double * );

int
main( int argc, char **argv ) {

  libnucnet::Nuc *p_my_nuclei;
  libnucnet::NucView *p_view;
  double d_addend;

  if ( argc != 4 ) {
      fprintf(
        stderr, "\nUsage: %s filename xpath addend\n\n", argv[0]
      );
      fprintf(
        stderr, "  filename = input nuclear data xml file or url.\n\n"
      );
      fprintf(
        stderr, "  xpath = xpath expression for view nuclides.\n\n"
      );
      fprintf(
        stderr, "  addend = addend to mass excess.\n\n"
      );
      return EXIT_FAILURE;
   }

  /*============================================================================
  // Create nuclear species collection.
  //==========================================================================*/

  p_my_nuclei = new libnucnet::Nuc( argv[1] );
    
  /*============================================================================
  // Get view.
  //==========================================================================*/

  p_view = new libnucnet::NucView( p_my_nuclei, argv[2] );

  /*============================================================================
  // Modify source info in view nuclides.
  //==========================================================================*/

  d_addend = atof( argv[3] );

  p_view->getNuc()->iterateSpecies(
    (libnucnet::species::iterateFunction) modify_species,
    &d_addend
  );

  /*============================================================================
  // Print the nuclear data.
  //==========================================================================*/

  libnucnet::user::print_nuclei( p_my_nuclei );

  /*============================================================================
  // Print the number of nuclei whose data were modified.
  //==========================================================================*/

  fprintf(
    stdout,
    "The number of nuclei with modified data (those in view \"%s\"): %lu\n\n",
    p_view->getXPath(),
    (unsigned long) p_view->getNuc()->getNumberOfSpecies()
  );

  /*============================================================================
  // Clean up.  Done.
  //==========================================================================*/

  delete p_my_nuclei;

  delete p_view;

  /*============================================================================
  // Done!
  //==========================================================================*/

  return EXIT_SUCCESS;

}

/*##############################################################################
// modify_source().
//############################################################################*/

int
modify_species( libnucnet::Species * p_species, double * p_addend )
{

  p_species->updateMassExcess(
    p_species->getMassExcess() + *p_addend
  );

  p_species->updateSource(
    "Modified mass excess"
  );

  return 1;

}

