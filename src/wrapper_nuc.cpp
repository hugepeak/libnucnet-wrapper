#include "wrapper_nuc.h"

namespace libnucnet
{

NucView::NucView(
  Nuc *p_nuc,
  const char *s_nuc_xpath
)
{

  Nuc *p_nuc_new;

  pNucnetNucView =
    Libnucnet__NucView__new(
      p_nuc->getNucnetNuc(),
      s_nuc_xpath
    );

  p_nuc_new = 
    new Nuc( 
      Libnucnet__NucView__getNuc(
        pNucnetNucView
      )
    );

  pNuc = p_nuc_new;

}

NucView::~NucView()
{

  Libnucnet__NucView__free( pNucnetNucView );

}

Nuc *
NucView::getNuc()
{
  
  return pNuc;

}

const char *
NucView::getXPath()
{

  return 
    Libnucnet__NucView__getXPath(
      pNucnetNucView
    ); 

}

int
Nuc::addSpecies(
  Species * p_species
)
{

  vSpeciesVector.push_back( p_species );

  return
    Libnucnet__Nuc__addSpecies(
      pNucnetNuc,
      p_species->getNucnetSpecies()
    ); 

}

void
Nuc::clearSpeciesCompareFunction()
{

  Libnucnet__Nuc__clearSpeciesCompareFunction(
    pNucnetNuc
  );

} 

double
Nuc::computeSpeciesBindingEnergy(
  Species * p_species
)
{

  return
    Libnucnet__Nuc__computeSpeciesBindingEnergy(
      pNucnetNuc,
      (const Libnucnet__Species *) p_species->getNucnetSpecies()
    );

}

Nuc *
Nuc::extractSubset(
  const char * s_xpath
)
{

  Nuc * p_new_nuc;

  p_new_nuc = 
    new Nuc(
      Libnucnet__Nuc__extractSubset(
        pNucnetNuc,
        s_xpath
      )
    );

  return p_new_nuc;

}

Nuc::~Nuc()
{

  Libnucnet__Nuc__free( pNucnetNuc );

  for( size_t i = 0; i < vSpeciesVector.size(); i++ )
    delete vSpeciesVector[i];
 
}

unsigned int
Nuc::getLargestNucleonNumber(
  const char * s_nucleon_type
)
{

  return
    Libnucnet__Nuc__getLargestNucleonNumber(
      pNucnetNuc,
      s_nucleon_type
    );

}

size_t
Nuc::getNumberOfSpecies()
{

  return
    Libnucnet__Nuc__getNumberOfSpecies( pNucnetNuc );

}

Species *
Nuc::getSpeciesByIndex(
  size_t i_index
)
{

  return vSpeciesVector[i_index]; 

}

Species *
Nuc::getSpeciesByName(
  const char * s_species_name
)
{

  return
    getSpeciesByIndex(
      Libnucnet__Species__getIndex(
        Libnucnet__Nuc__getSpeciesByName( 
          pNucnetNuc,
          s_species_name 
        )
      )
    );

}

Species *
Nuc::getSpeciesByZA(
  unsigned int i_z,
  unsigned int i_a
)
{

  return
    getSpeciesByZA( i_z, i_a, NULL );

}

Species *
Nuc::getSpeciesByZA(
  unsigned int i_z,
  unsigned int i_a,
  const char *s_state
)
{

  Libnucnet__Species *p_nucnet_species;
  Species *p_species = NULL;

  p_nucnet_species =
    Libnucnet__Nuc__getSpeciesByZA(
      pNucnetNuc,
      i_z,
      i_a,
      s_state 
    );

  if( p_nucnet_species )
    return
      getSpeciesByIndex(
        Libnucnet__Species__getIndex( p_nucnet_species )
      );

  return p_species;

}

int
nuc::is_valid_input_xml(
  const char *s_xml_filename
)
{

  return
    Libnucnet__Nuc__is_valid_input_xml(
      s_xml_filename
    );

}

void
Nuc::iterateSpecies(
  species::iterateFunction pfFunc
)
{

  iterateSpecies( pfFunc, NULL );

}
  
void
Nuc::iterateSpecies(
  species::iterateFunction pfFunc,
  void *p_user_data
)
{

  size_t i;

  for( i = 0; i < vSpeciesVector.size(); i++ )
    if( 
      pfFunc( vSpeciesVector[i], p_user_data ) == 0
    ) break;
    
}
  
Nuc::Nuc() 
{

  Libnucnet__Nuc *p_nucnet_nuc = Libnucnet__Nuc__new();

  pNucnetNuc = p_nucnet_nuc;

  Libnucnet__Nuc__iterateSpecies(
    pNucnetNuc,
    (Libnucnet__Species__iterateFunction) nuc::addNucnetSpecies,
    &vSpeciesVector
  );

}

Nuc::Nuc(
  const char *s_xml_filename
)
{

  Libnucnet__Nuc *p_nucnet_nuc;

  p_nucnet_nuc = 
    Libnucnet__Nuc__new_from_xml(
      s_xml_filename,
      NULL
    );

  pNucnetNuc = p_nucnet_nuc;

  Libnucnet__Nuc__iterateSpecies(
    pNucnetNuc,
    (Libnucnet__Species__iterateFunction) nuc::addNucnetSpecies,
    &vSpeciesVector
  );

} 

Nuc::Nuc(
  const char *s_xml_filename,
  const char *s_xpath
)
{

  Libnucnet__Nuc *p_nucnet_nuc;

  p_nucnet_nuc = 
    Libnucnet__Nuc__new_from_xml(
      s_xml_filename,
      s_xpath 
    );

  pNucnetNuc = p_nucnet_nuc;

  Libnucnet__Nuc__iterateSpecies(
    pNucnetNuc,
    (Libnucnet__Species__iterateFunction) nuc::addNucnetSpecies,
    &vSpeciesVector
  );

} 

Nuc::Nuc(
  Libnucnet__Nuc *p_nucnet_nuc
)
{

  pNucnetNuc = p_nucnet_nuc;

  Libnucnet__Nuc__iterateSpecies(
    pNucnetNuc,
    (Libnucnet__Species__iterateFunction) nuc::addNucnetSpecies,
    &vSpeciesVector
  );

}

int
nuc::addNucnetSpecies(
  Libnucnet__Species *self,
  void *p_data 
)
{

  nuc::SpeciesContainer *p_vector = (nuc::SpeciesContainer *) p_data;

  Species *p_species = new Species( self );
 
  p_vector->push_back( p_species );

  return 1;

}
 
int
Nuc::removeSpecies(
  Species *p_species
)
{

  size_t i_index;
  
  i_index =
    p_species->getVectorIndex();

  if( i_index >= vSpeciesVector.size() )
    LIBNUCNET__NUC__ERROR( "No such species to remove!" );

  vSpeciesVector.erase( 
    vSpeciesVector.begin() + i_index 
  );

  if(
    !Libnucnet__Nuc__removeSpecies(
      pNucnetNuc,
      p_species->getNucnetSpecies()
    )
  )
    LIBNUCNET__NUC__ERROR( "Couldn't remove species!" );

  delete p_species;
  
  return 1;

}

void
Nuc::setSpeciesCompareFunction(
  species::compare_function pfFunc
)
{

  Libnucnet__Nuc__setSpeciesCompareFunction(
    pNucnetNuc,
    pfFunc
  );

}

void
Nuc::sortSpecies()
{

  Libnucnet__Nuc__sortSpecies( pNucnetNuc );

}

void
Nuc::updateFromXml(
  const char *s_xml_filename
)
{

  updateFromXml(
    s_xml_filename,
    NULL
  );

}

void
Nuc::updateFromXml(
  const char *s_xml_filename,
  const char *s_xpath
)
{

  Libnucnet__Nuc__updateFromXml(
    pNucnetNuc,
    s_xml_filename,
    s_xpath
  );

  for( size_t i = 0; i < vSpeciesVector.size(); i++ )
    delete vSpeciesVector[i];

  vSpeciesVector.clear();

  Libnucnet__Nuc__iterateSpecies(
    pNucnetNuc,
    (Libnucnet__Species__iterateFunction) nuc::addNucnetSpecies,
    &vSpeciesVector
  );

}

int
Nuc::updateSpecies(
  Species *p_species
)
{

  // may need to consider different states here.

  Species *p_old;

  size_t i_index;

  p_old = getSpeciesByName( p_species->getName() );

  i_index = p_old->getVectorIndex();
  
  if( 
    !Libnucnet__Nuc__updateSpecies(
      pNucnetNuc,
      p_species->getNucnetSpecies()
    )
  )
    return 0;

  vSpeciesVector.erase(
    vSpeciesVector.begin() + i_index 
  );

  delete p_old;

  vSpeciesVector.push_back( p_species );

  return 1;

}

void
Nuc::writeToXmlFile(
  const char *s_xml_filename
)
{

  Libnucnet__Nuc__writeToXmlFile(
    pNucnetNuc,
    s_xml_filename
  );

}

Libnucnet__Nuc *
Nuc::getNucnetNuc()
{

  return pNucnetNuc;

}

double
Species::computePartitionFunction( 
  double d_t9
)
{

  return
    Libnucnet__Species__computePartitionFunction(
      pNucnetSpecies,
      d_t9
    );

}

double
Species::computeQuantumAbundance(
  double d_t9,
  double d_rho
)
{

  return
    Libnucnet__Species__computeQuantumAbundance(
      pNucnetSpecies,
      d_t9,
      d_rho
    );

}

Species *
Species::copy()
{

  Species *p_species;

  Libnucnet__Species *p_nucnet_species;

  p_nucnet_species = Libnucnet__Species__copy( pNucnetSpecies );

  p_species = new Species( p_nucnet_species );

  return p_species;

}

Species::Species() {}

Species::~Species() {}

void
Species::freeNucnetSpecies()
{

  Libnucnet__Species__free( pNucnetSpecies );

}

unsigned int
Species::getA() const
{

  return
    Libnucnet__Species__getA( pNucnetSpecies );

}

size_t
Species::getIndex()
{

  return
    Libnucnet__Species__getIndex( pNucnetSpecies );

}

double
Species::getMassExcess()
{

  return
    Libnucnet__Species__getMassExcess( pNucnetSpecies );

}

const char *
Species::getName() const
{

  return
    Libnucnet__Species__getName( pNucnetSpecies );

}

gsl_vector *
Species::getPartitionFunctionLog10()
{

  return
    Libnucnet__Species__getPartitionFunctionLog10( pNucnetSpecies );

}

gsl_vector *
Species::getPartitionFunctionT9()
{

  return
    Libnucnet__Species__getPartitionFunctionT9( pNucnetSpecies );

}

const char *
Species::getSource()
{

  return
    Libnucnet__Species__getSource( pNucnetSpecies );

}

double
Species::getSpin()
{

  return 
    Libnucnet__Species__getSpin( pNucnetSpecies );

}

unsigned int
Species::getZ() const
{

  return
    Libnucnet__Species__getZ( pNucnetSpecies );

}

Species::Species(
  Libnucnet__Species *p_nucnet_species
)
{

  pNucnetSpecies = p_nucnet_species;

}

Species::Species(
  unsigned int i_z,
  unsigned int i_a,
  const char *s_source,
  int i_state_flag,
  const char *s_state,
  double d_mass_excess,
  double d_spin,
  gsl_vector *p_t9,
  gsl_vector *p_log10_partf 
)
{

  Libnucnet__Species *p_nucnet_species;

  p_nucnet_species = 
    Libnucnet__Species__new(
      i_z,
      i_a,
      s_source,
      i_state_flag,
      s_state,
      d_mass_excess,
      d_spin,
      p_t9,
      p_log10_partf
    );

  pNucnetSpecies = p_nucnet_species;

}

void
Species::updateMassExcess( 
  double d_new_mass_excess
)
{

  Libnucnet__Species__updateMassExcess(
    pNucnetSpecies,
    d_new_mass_excess
  );

}

void
Species::updatePartitionFunctionData(
  gsl_vector *p_new_t9_array,
  gsl_vector *p_new_log10_partf_array
)
{

  Libnucnet__Species__updatePartitionFunctionData(
    pNucnetSpecies,
    p_new_t9_array,
    p_new_log10_partf_array
  );

}

void
Species::updateSource(
  const char *s_source
)
{

  Libnucnet__Species__updateSource(
    pNucnetSpecies,
    s_source
  );

}

void
Species::updateSpin(
  double d_new_spin
)
{

  Libnucnet__Species__updateSpin(
    pNucnetSpecies,
    d_new_spin
  );

}

size_t 
Species::getVectorIndex()
{

  return
    Libnucnet__Species__getIndex( pNucnetSpecies ); 

}

Libnucnet__Species *
Species::getNucnetSpecies()
{

  return pNucnetSpecies;

}

} // namespace libnucnet
 
