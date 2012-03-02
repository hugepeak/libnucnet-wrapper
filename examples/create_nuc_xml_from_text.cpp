/*//////////////////////////////////////////////////////////////////////////////
// <file type="public">
//   <description>
//     <abstract>
//       Example to demonstrate how to use libnucnet routines to create a
//       Libnucnet__Nuc structure of nuclear species, add species to the
//       structure, write the data to an xml file, and clear the structure
//       and free the allocated memory.
//     </abstract>
//   </description>
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
// </file>
//////////////////////////////////////////////////////////////////////////////*/
 
#include <wrapper_nuc.h>

int main( int argc, char **argv ) {

  libnucnet::Nuc *p_my_nuclei;
  libnucnet::Species *p_species;
  FILE *p_input_file;
  size_t i, i_partf_count;
  unsigned int i_z, i_a;
  int i_state;
  double d_mass_excess;
  float f_spin;
  char s_state[2];
  double d_t9, d_log10_partf;
  gsl_vector *p_t9, *p_log10_partf;

  /*============================================================================
  // Check input.
  //==========================================================================*/

  if ( argc != 3 ) {
      fprintf(
        stderr, "\nUsage: %s input_file output_file \n", argv[0]
      );
      fprintf(
        stderr, "\n  input_file: name of input text file\n"
      );
      fprintf(
        stderr, "\n  output_file: name of output xml file\n\n"
      );
      return EXIT_FAILURE;
   }

  /*============================================================================
  // Create the collection.
  //==========================================================================*/

  p_my_nuclei = new libnucnet::Nuc;

  /*============================================================================
  // Open input file.
  //==========================================================================*/

  if( ( p_input_file = fopen( argv[1], "r" ) ) == NULL ) {
    fprintf( stderr, "Could not open file.\n" );
    return EXIT_FAILURE;
  }

  /*============================================================================
  // Read in input.
  //==========================================================================*/

  while( !feof( p_input_file ) ) {

    fscanf(
      p_input_file,
      "%u%u%lf%f\n",
      (unsigned int *) &i_z,
      (unsigned int *) &i_a,
      &d_mass_excess,
      &f_spin
    );

    fscanf( p_input_file, "%s%d\n", s_state, &i_state );

    fscanf( p_input_file, "%lu\n", (unsigned long *) &i_partf_count );

    p_t9 = gsl_vector_alloc( i_partf_count );
    p_log10_partf = gsl_vector_alloc( i_partf_count );

    for( i = 0; i < i_partf_count; i++ ) {

      fscanf(
        p_input_file,
        "%lf%lf\n",
        &d_t9,
        &d_log10_partf
      );

      gsl_vector_set( p_t9, i, d_t9 );
      gsl_vector_set( p_log10_partf, i, d_log10_partf );

    }

    p_species =
      new libnucnet::Species(
        i_z,
        i_a,
        "example",
        i_state,
        s_state,
        d_mass_excess,
        f_spin,
        p_t9,
        p_log10_partf
      );

    p_my_nuclei->addSpecies( p_species );

    gsl_vector_free( p_t9 );
    gsl_vector_free( p_log10_partf );

  }
  
  /*============================================================================
  // Close input file.
  //==========================================================================*/

  fclose( p_input_file );

  /*============================================================================
  // Write out nuclear data to xml file.
  //==========================================================================*/

  p_my_nuclei->writeToXmlFile( argv[2] );

  /*============================================================================
  // Clean up
  //==========================================================================*/

  delete p_my_nuclei;

  /*============================================================================
  // Done!
  //==========================================================================*/

  return EXIT_SUCCESS;

}

