#ifndef WRAPPER_NUC_H
#define WRAPPER_NUC_H

#include <iostream>
#include <vector>
#include "Libnucnet__Nuc.h"

namespace libnucnet
{

class Species;

namespace species
{

typedef int ( *compare_function ) (
  const Libnucnet__Species *,
  const Libnucnet__Species *
);

typedef int ( *iterateFunction ) (
  Species *, void *
);

typedef double ( *nseCorrectionFactorFunction ) (
  Species *, double, double, double, void *
);

} // namespace species

namespace nuc
{

typedef std::vector<Species *> SpeciesContainer;

int is_valid_input_xml( const char * );

int
addNucnetSpecies(
  Libnucnet__Species *,
  void * 
);

} // namespace nuc

class Species {

  public:

    Species();
    Species( Libnucnet__Species * );
    Species(
      unsigned int,
      unsigned int,
      const char *,
      int,
      const char *,
      double,
      double,
      gsl_vector *,
      gsl_vector *   
    );
    ~Species();

    double computePartitionFunction( double );
    double computeQuantumAbundance( double, double );
    Species * copy();
    unsigned int getA() const;
    size_t getIndex();
    double getMassExcess();
    const char * getName() const;
    gsl_vector * getPartitionFunctionLog10();
    gsl_vector * getPartitionFunctionT9();
    const char * getSource();
    double getSpin();
    unsigned int getZ() const;
    void updateMassExcess( double );
    void updatePartitionFunctionData( gsl_vector *, gsl_vector * );
    void updateSource( const char * );
    void updateSpin( double );

    Libnucnet__Species * getNucnetSpecies();
    void freeNucnetSpecies();
    size_t getVectorIndex();

  private:

    Libnucnet__Species * pNucnetSpecies;
  
};

class Nuc {

  public:

    Nuc();
    Nuc( Libnucnet__Nuc * );
    Nuc( const char * );
    Nuc( const char *, const char * );
    ~Nuc();

    int addSpecies( Species * );
    void clearSpeciesCompareFunction();
    double computeSpeciesBindingEnergy( Species * );
    Nuc * extractSubset( const char * );
    unsigned int getLargestNucleonNumber( const char * );
    size_t getNumberOfSpecies();
    Species * getSpeciesByIndex( size_t );
    Species * getSpeciesByName( const char * );
    Species * getSpeciesByZA( unsigned int, unsigned int );
    Species * getSpeciesByZA( unsigned int, unsigned int, const char * );
    void iterateSpecies( species::iterateFunction );
    void iterateSpecies( species::iterateFunction, void * );
    int removeSpecies( Species * );
    void setSpeciesCompareFunction( species::compare_function );
    void sortSpecies();
    void updateFromXml( const char * );
    void updateFromXml( const char *, const char * );
    int updateSpecies( Species * );
    void writeToXmlFile( const char * );

    Libnucnet__Nuc * getNucnetNuc();
    
  private:

    Libnucnet__Nuc * pNucnetNuc;
    nuc::SpeciesContainer vSpeciesVector;

};

class NucView {

  public:

    NucView( Nuc *, const char * );
    ~NucView();

    Nuc * getNuc();
    const char * getXPath();
    Libnucnet__NucView * getNucnetNucView();

  private:

    Nuc * pNuc;
    Libnucnet__NucView * pNucnetNucView;

};

} // namespace libnucnet

#endif
