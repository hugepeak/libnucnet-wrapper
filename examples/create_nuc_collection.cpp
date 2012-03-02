#include <wrapper_nuc.h>

void print_nuclei( libnucnet::Nuc * );

int print_species( libnucnet::Species *, void * );

int
main( int argc, char **argv ) {

  libnucnet::Nuc *p_my_nuclei;
  libnucnet::Species *p_species, *p_copy1, *p_copy2;
  size_t i, i_partf_count = 10;
  unsigned i_z, i_a;
  int i_state;
  double d_mass_excess, d_spin;
  double a_t9[10] = {0.1, 0.2, 0.3, 0.5, 1.0, 2.0, 3.0, 5.0, 7.0, 10.};
  double a_log10_partf[10] = {1.,2.,3.,4.,5.,6.,7.,8.,9.,10.};
  gsl_vector *p_t9, *p_log10_partf;

  /*============================================================================
  // Check input.
  //==========================================================================*/

   if ( argc != 1 ) {
      fprintf(
        stderr, "\nUsage: %s\n\n", argv[0]
      );
      return EXIT_FAILURE;
   }

  /*============================================================================
  // Allocate and assign gsl vectors.
  //==========================================================================*/

  p_t9 = gsl_vector_alloc( i_partf_count );
  p_log10_partf = gsl_vector_alloc( i_partf_count );

  for( i = 0; i < i_partf_count; i++ ) {
    gsl_vector_set( p_t9, i, a_t9[i] );
    gsl_vector_set( p_log10_partf, i, a_log10_partf[i] );
  }

  /*============================================================================
  // Create collection of nuclear species.
  //==========================================================================*/

  p_my_nuclei = new libnucnet::Nuc;

  print_nuclei( p_my_nuclei );

  /*============================================================================
  // Add neutrons.
  //==========================================================================*/

  fprintf( stdout, "\nAdd neutrons:\n\n" );

  i_z = 0; 
  i_a = 1;
  i_state = 0;
  d_mass_excess = 8.071;
  d_spin = 0.5;

  p_species =
    new libnucnet::Species(
      i_z,
      i_a,
      argv[0],
      i_state,
      NULL,
      d_mass_excess,
      d_spin,
      p_t9,
      p_log10_partf
  );

  if( !p_my_nuclei->addSpecies( p_species ) )
  {
    fprintf( stderr, "Couldn't add species.\n" );
    return EXIT_FAILURE;
  }

  print_nuclei( p_my_nuclei );

  /*============================================================================
  // Add di-neutrons.
  //==========================================================================*/

  fprintf( stdout, "\nAdd di-neutrons:\n\n" );

  i_z = 0; 
  i_a = 2;
  i_state = 0;
  d_mass_excess = 0.3;
  d_spin = 0.;

  p_species =
    new libnucnet::Species(
      i_z,
      i_a,
      "Made up data",
      i_state,
      NULL,
      d_mass_excess,
      d_spin,
      p_t9,
      p_log10_partf
  );

  if( !p_my_nuclei->addSpecies( p_species ) )
  {
    fprintf( stderr, "Couldn't add species.\n" );
    return EXIT_FAILURE;
  }

  print_nuclei( p_my_nuclei );

  /*============================================================================
  // Add protons.
  //==========================================================================*/

  fprintf( stdout, "\nAdd protons:\n\n" );

  i_z = 1; 
  i_a = 1;
  i_state = 0;
  d_mass_excess = 7.289;
  d_spin = 0.5;

  p_species =
    new libnucnet::Species(
      i_z,
      i_a,
      argv[0],
      i_state,
      NULL,
      d_mass_excess,
      d_spin,
      p_t9,
      p_log10_partf
    );

  if( !p_my_nuclei->addSpecies( p_species ) )
  {
    fprintf( stderr, "Couldn't add species.\n" );
    return EXIT_FAILURE;
  }

  /*========================================================================
  // Get two copies of protons.  One for an update.  One for later.
  //==========================================================================*/

  p_copy1 = p_species->copy();
  p_copy2 = p_species->copy();

  /*========================================================================
  // Update collection for illustration.
  //==========================================================================*/

  if( !p_my_nuclei->updateSpecies( p_copy1 ) )
  {
    fprintf( stderr, "Couldn't update species.\n" );
    return EXIT_FAILURE;
  }

  /*========================================================================
  // Print out data about the species.
  //==========================================================================*/

  print_nuclei( p_my_nuclei );

  /*============================================================================
  // Remove neutrons
  //==========================================================================*/

  fprintf( stdout, "\nRemove neutrons:\n\n" );

  p_my_nuclei->removeSpecies(
    p_my_nuclei->getSpeciesByName( "n" )
  );

  print_nuclei( p_my_nuclei );

  /*============================================================================
  // Free the collection.
  //==========================================================================*/

  printf( "\nFree the collection:\n\n" );

  delete p_my_nuclei;
  
  /*============================================================================
  // Recreate the collection.
  //==========================================================================*/

  p_my_nuclei = new libnucnet::Nuc;

  /*============================================================================
  // Add protons and print out data.
  //==========================================================================*/

  fprintf( stdout, "\nAdd protons again:\n\n" );

  if( !p_my_nuclei->addSpecies( p_copy2 ) )
  {
    fprintf( stderr, "Couldn't add species.\n" );
    return EXIT_FAILURE;
  }

  print_nuclei( p_my_nuclei );

  /*============================================================================
  // Final clean up
  //==========================================================================*/

  gsl_vector_free( p_t9 );
  gsl_vector_free( p_log10_partf );

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
  // Print out header information.
  //==========================================================================*/

  fprintf(
    stdout,
    "\n  Index  Z    A    Name    Mass Excess (MeV)    Spin   Data Source\n"
  );
  fprintf(
    stdout,
    "  _____ ___  ___  ______  ___________________   ____   __________\n\n"
  );

  /*============================================================================
  // Iterate to print species.
  //==========================================================================*/

  self->iterateSpecies(
    (libnucnet::species::iterateFunction) print_species
  );

  /*============================================================================
  // Print out the number of species.
  //==========================================================================*/

  fprintf(
    stdout,
    "\nThe collection has a total of %lu species.\n\n",
    (unsigned long) self->getNumberOfSpecies()
  );

  return;

}

/*##############################################################################
// print_species()
//############################################################################*/

int
print_species(
  libnucnet::Species *p_species, void *p_user_data
)
{

  if( p_user_data ) {
    fprintf( stderr, "No user data should be passed to this function.\n" );
    return 0;
  }

  fprintf(
    stdout,
    "%5lu %4u %4u  %5s  %13.4f  %13.2f   %s\n",
    (unsigned long) p_species->getIndex(),
    p_species->getZ(),
    p_species->getA(),
    p_species->getName(),
    p_species->getMassExcess(),
    p_species->getSpin(),
    p_species->getSource()
  );

  return 1;

}

