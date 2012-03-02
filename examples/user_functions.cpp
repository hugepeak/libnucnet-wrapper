#include <user_functions.h>

namespace libnucnet
{

namespace user
{

/*##############################################################################
// print_species()
//############################################################################*/

int
print_species(
  Species *p_species,
  void *p_data
)
{

  if( p_data ) {
    fprintf( stderr, "No extra data for this routine.\n" );
    exit( EXIT_FAILURE );
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

/*##############################################################################
// print_nuclei()
//############################################################################*/

void print_nuclei( libnucnet::Nuc * self ) {

  /*============================================================================
  // Print out header.
  //==========================================================================*/

  fprintf(
    stdout,
    "\n  Index  Z    A    Name    Mass Excess (MeV)    Spin   Data Source\n"
  );
  fprintf(
    stdout,
    "  _____ ___  ___  ______  ___________________   ____   ___________\n\n"
  );

  /*============================================================================
  // Print out species data.
  //==========================================================================*/

  self->iterateSpecies(
    (libnucnet::species::iterateFunction) libnucnet::user::print_species
  );

  /*============================================================================
  // Print out the number of species.
  //==========================================================================*/

  fprintf( stdout, "\nThe collection has a total of %lu species.\n\n",
    (unsigned long) self->getNumberOfSpecies()
  );

  return;

}

} // namespace user

} // namespace libnucnet

