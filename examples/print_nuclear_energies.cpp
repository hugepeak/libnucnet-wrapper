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
//       xml file, compute and print out nuclear energies,
//       and clear the structure and free the allocated memory.
//     </abstract>
//   </description>
// </file>
//////////////////////////////////////////////////////////////////////////////*/
 
#include <wrapper_nuc.h>

#define MY_BUFFER   32

/*##############################################################################
// Prototypes.
//############################################################################*/

double compute_species_mass( libnucnet::Species * );

void
print_nuclei(
  libnucnet::Nuc *
);

int
print_species(
  libnucnet::Species *,
  libnucnet::Nuc *
);

char *
compute_species_neutron_separation_energy(
  libnucnet::Nuc *,
  libnucnet::Species *
);

char *
compute_species_proton_separation_energy(
  libnucnet::Nuc *,
  libnucnet::Species *
);

/*##############################################################################
// main().
//############################################################################*/

int
main( int argc, char **argv ) {

  libnucnet::Nuc *p_my_nuclei;

  if ( argc != 2 && argc != 3 ) {
      fprintf(
        stderr, "\nUsage: %s filename xpath\n\n", argv[0]
      );
      fprintf(
        stderr, "  filename = input nuclear data xml file or url.\n\n"
      );
      fprintf(
        stderr, "  xpath = xpath expression (optional).\n\n"
      );
      return EXIT_FAILURE;
   }

  /*============================================================================
  // Create nuclear species collection.
  //==========================================================================*/

  if( argc == 2 ) {
    p_my_nuclei = new libnucnet::Nuc( argv[1], NULL );
  } else {
    p_my_nuclei = new libnucnet::Nuc( argv[1], argv[2] );
  }
    
  /*============================================================================
  // Check that neutron and protons present.
  //==========================================================================*/
  
  if(
    !p_my_nuclei->getSpeciesByName( "h1" ) ||
    !p_my_nuclei->getSpeciesByName( "n" )
  )
  {
    fprintf( stderr, "Neutrons and protons must be in nuclear collection!\n" );
    return EXIT_FAILURE;
  }

  /*============================================================================
  // Print the nuclear data.
  //==========================================================================*/

  print_nuclei( p_my_nuclei );

  /*============================================================================
  // Free the nuclear species collection.
  //==========================================================================*/

  delete p_my_nuclei;
  
  /*============================================================================
  // Done!
  //==========================================================================*/

  return EXIT_SUCCESS;

}

/*##############################################################################
// print_nuclei()
//############################################################################*/

void print_nuclei( libnucnet::Nuc * self ) {

  /*============================================================================
  // Print out header.
  //==========================================================================*/

  fprintf(
    stdout,
    "\n  Z    A    Name  Mass (MeV)   B / A (MeV)   S_n (MeV)  S_p (MeV)\n"
  );
  fprintf(
    stdout,
    " ___  ___   ____  __________   ___________   _________  _________\n\n"
  );

  /*============================================================================
  // Print out species data.
  //==========================================================================*/

  self->iterateSpecies(
    (libnucnet::species::iterateFunction) print_species,
    self
  );

}

/*##############################################################################
// print_species()
//############################################################################*/

int
print_species(
  libnucnet::Species *p_species,
  libnucnet::Nuc *p_nuc
)
{

  double d_mass;
  char *s_sn, *s_sp;

  d_mass = compute_species_mass( p_species );

  s_sn =
    compute_species_neutron_separation_energy( p_nuc, p_species );

  s_sp =
    compute_species_proton_separation_energy( p_nuc, p_species );

  fprintf(
    stdout,
    "%4u %4u  %5s  %10.4f %10.4f       %s    %s\n",
    p_species->getZ(),
    p_species->getA(),
    p_species->getName(),
    d_mass,
    p_nuc->computeSpeciesBindingEnergy(
      p_species
    ) /
      p_species->getA(),
    s_sn,
    s_sp
  );

  free( s_sn );
  free( s_sp );

  return 1;

}

/*##############################################################################
// compute_species_mass().
//############################################################################*/

double compute_species_mass( libnucnet::Species *p_species )
{

  return
    GSL_CONST_CGSM_UNIFIED_ATOMIC_MASS * 
    gsl_pow_2( GSL_CONST_CGSM_SPEED_OF_LIGHT ) *
    ( GSL_CONST_NUM_MICRO / GSL_CONST_CGSM_ELECTRON_VOLT ) *
    p_species->getA() +
    p_species->getMassExcess();

}

/*##############################################################################
// compute_species_neutron_separation_energy().
//############################################################################*/

char *
compute_species_neutron_separation_energy(
  libnucnet::Nuc *p_nuc,
  libnucnet::Species *p_species
)
{

  libnucnet::Species *p_species_2 = NULL;
  char *s_separation_energy;

  p_species_2 =
    p_nuc->getSpeciesByZA(
      p_species->getZ(),
      p_species->getA() - 1
    );

  if( !p_species_2 )
    p_species_2 =
      p_nuc->getSpeciesByZA(
        p_species->getZ(),
        p_species->getA() - 1,
        "g"
      );

  /*============================================================================
  // Return as string to allow for case when separation energy cannot be
  // computed.
  //==========================================================================*/

  s_separation_energy = ( char * ) malloc( sizeof( char ) * MY_BUFFER );

  if( p_species_2 )
    sprintf(
      s_separation_energy,
      "%.4f",
      p_nuc->computeSpeciesBindingEnergy( p_species ) -
        p_nuc->computeSpeciesBindingEnergy( p_species_2 )
    );
  else
    sprintf(
      s_separation_energy,
      "-------"
    );

  return s_separation_energy;

}
/*##############################################################################
// compute_species_proton_separation_energy().
//############################################################################*/

char *
compute_species_proton_separation_energy(
  libnucnet::Nuc *p_nuc,
  libnucnet::Species *p_species
)
{

  libnucnet::Species *p_species_2;
  char *s_separation_energy;

  p_species_2 =
    p_nuc->getSpeciesByZA(
      p_species->getZ() - 1,
      p_species->getA() - 1
    );

  if( !p_species_2 )
    p_species_2 =
      p_nuc->getSpeciesByZA(
        p_species->getZ() - 1,
        p_species->getA() - 1,
        "g"
      );

  /*============================================================================
  // Return as string to allow for case when separation energy cannot be
  // computed.
  //==========================================================================*/

  s_separation_energy = ( char * ) malloc( sizeof( char ) * MY_BUFFER );

  if( p_species_2 )
    sprintf(
      s_separation_energy,
      "%.4f",
      p_nuc->computeSpeciesBindingEnergy( p_species ) -
        p_nuc->computeSpeciesBindingEnergy( p_species_2 )
    );
  else
    sprintf(
      s_separation_energy,
      "-------"
    );

  return s_separation_energy;

}
