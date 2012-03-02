/*//////////////////////////////////////////////////////////////////////////////
// <file type="public">
//   <license>
//      Copyright (c) 2006-2011 Clemson University.
//
//      This file is part of the Clemson Webnucleo group's
//      libnucnet module, originally developed by David C. Adams
//      and Bradley S. Meyer.  For more information,
//      please see http://www.webnucleo.org.
//
//      This is free software; you can redistribute it and/or modify it
//      under the terms of the GNU General Public License as published by
//      the Free Software Foundation; either version 2 of the License, or
//      (at your option) any later version.
//
//      This software is distributed in the hope that it will be useful,
//      but WITHOUT ANY WARRANTY; without even the implied warranty of
//      MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//      GNU General Public License for more details.
//
//      You should have received a copy of the GNU General Public License
//      along with this software (please see the "gnu_gpl.txt" file in the doc/
//      directory of this distribution); if not, write to the Free Software
//      Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307
//      USA
//   </license>
//   <description>
//     <abstract>
//        Source code for the Libnucnet__Nuc part of the libnucnet module.
//        For documentation, see the header file Libnucnet__Nuc.h.
//     </abstract>
//   </description>
// </file>
//////////////////////////////////////////////////////////////////////////////*/

/*##############################################################################
// Include header.
//############################################################################*/

#include "Libnucnet__Nuc.h"

/*##############################################################################
// Libnucnet__Nuc__new().
//############################################################################*/

Libnucnet__Nuc *Libnucnet__Nuc__new( ) {

  Libnucnet__Nuc *self;

  /*============================================================================
  // Create network structure.
  //==========================================================================*/

  if(
     !( self = ( Libnucnet__Nuc * ) malloc( sizeof( Libnucnet__Nuc ) ) )
  ) {
    LIBNUCNET__NUC__ERROR( "Virtual memory exhausted" );
  }

  /*============================================================================
  // Initialize Libnucnet__Nuc quantities.
  //==========================================================================*/

  Libnucnet__Nuc__initializeHash( self );

  self->pfSpeciesCompare = 
    (Libnucnet__Species__compare_function)
    Libnucnet__Species__default_compare_function;

  self->iUpdate = 1;
  self->iOwner = 1;

  return self;

}

/*##############################################################################
// Libnucnet__Nuc__new_from_xml().
//############################################################################*/

Libnucnet__Nuc *Libnucnet__Nuc__new_from_xml(
  const char *s_xml_filename, const char *s_xpath_suffix
 ) {

  xmlDocPtr p_doc;
  Libnucnet__Nuc *self;

  /*============================================================================
  // Create document
  //==========================================================================*/

  p_doc = xmlParseFile( s_xml_filename );

  if ( p_doc == NULL ) {

    LIBNUCNET__NUC__ERROR( "Document could not be opened" );

   }

  /*============================================================================
  // Create new structure.
  //==========================================================================*/

  self = Libnucnet__Nuc__new();

  /*============================================================================
  // Get structure from xml document.
  //==========================================================================*/

  Libnucnet__Nuc__updateFromXmlDocument( self, p_doc, s_xpath_suffix );
  
  /*============================================================================
  // Free document.
  //==========================================================================*/
  
  xmlFreeDoc( p_doc );
  xmlCleanupParser();

  /*============================================================================
  // Done.  Return.
  //==========================================================================*/

  return self;

}

/*##############################################################################
// Libnucnet__Nuc__updateFromXmlDocument().
//############################################################################*/

void
Libnucnet__Nuc__updateFromXmlDocument(
  Libnucnet__Nuc *self, xmlDocPtr p_doc, const char *s_xpath_suffix
)
{

  xmlXPathContextPtr p_xpathCtx;
  xmlXPathObjectPtr p_xpathObj;
  xmlChar *sx_str, *sx_xpath, *sx_xpath_suffix;

  int i;

  /*============================================================================
  // Create context.
  //==========================================================================*/

  p_xpathCtx = xmlXPathNewContext( p_doc );

  /*============================================================================
  // Create main xpath expression.
  //==========================================================================*/

  sx_xpath = xmlCharStrdup( DOUBLE_SLASH );

  if( !sx_xpath ) {
     LIBNUCNET__NUC__ERROR( "Couldn't allocate memory for xpath string" );
  }

  sx_str = xmlCharStrdup( XPATH_NUCLEAR_DATA );
  sx_xpath = xmlStrcat( sx_xpath, sx_str );
  xmlFree( sx_str );

  sx_xpath_suffix = xmlCharStrdup( s_xpath_suffix );

  if( sx_xpath_suffix ) {

     sx_xpath = xmlStrcat( sx_xpath, sx_xpath_suffix );

     if( !sx_xpath ) {
        LIBNUCNET__NUC__ERROR( "Couldn't allocate memory for xpath string" );
     }

     xmlFree( sx_xpath_suffix );

  }

  /*============================================================================
  // Nuclide iteration
  //==========================================================================*/

  p_xpathObj =
    xmlXPathEvalExpression( sx_xpath, p_xpathCtx );

  xmlFree( sx_xpath );

  if( !p_xpathObj ) {
    LIBNUCNET__NUC__ERROR( "Invalid xpath expression" );
  }

  if( !p_xpathObj->nodesetval ) {
    LIBNUCNET__NUC__ERROR( "No nuclear data" );
  }

  for( i = 0; i < p_xpathObj->nodesetval->nodeNr; i++ ) {

    if( Libnucnet__Nuc__assignSpecies(
          self, p_xpathObj->nodesetval->nodeTab[i]
        ) != 1 ) {
      LIBNUCNET__NUC__ERROR( "Problem assigning species" );
    }

  }

  xmlXPathFreeObject( p_xpathObj );
  xmlXPathFreeContext( p_xpathCtx );

}

/*##############################################################################
// Libnucnet__Nuc__updateFromXml().
//############################################################################*/

void
Libnucnet__Nuc__updateFromXml(
  Libnucnet__Nuc *self, const char *s_xml_filename, const char *s_xpath_suffix
 ) {

  xmlDocPtr p_doc;

  /*============================================================================
  // Check input structure.
  //==========================================================================*/

  if( !self ) {
    LIBNUCNET__NUC__ERROR( "Non-existent nuclide structure" );
  }

  /*============================================================================
  // Create document
  //==========================================================================*/

  p_doc = xmlParseFile( s_xml_filename );

  if ( p_doc == NULL ) {

    LIBNUCNET__NUC__ERROR( "Document could not be opened" );

   }

  /*============================================================================
  // Update from document.
  //==========================================================================*/

  Libnucnet__Nuc__updateFromXmlDocument( self, p_doc, s_xpath_suffix );

  /*============================================================================
  // Free document.
  //==========================================================================*/
  
  xmlFreeDoc( p_doc );
  xmlCleanupParser();

  /*============================================================================
  // Done.
  //==========================================================================*/

}

/*##############################################################################
// Libnucnet__Nuc__assignSpecies().
//############################################################################*/

int Libnucnet__Nuc__assignSpecies( 
  Libnucnet__Nuc *self, xmlNode *p_nuclide
) {

  xmlXPathContextPtr p_xpathCtx_nuclide;
  xmlXPathObjectPtr p_xpathObj_data;
  xmlNodePtr p_state;
  xmlChar *sx_str, *sx_data;
  unsigned int i, i_z, i_a, i_length;

  /*============================================================================
  // Get nuclide context.
  //==========================================================================*/

  p_xpathCtx_nuclide = xmlXPathNewContext( p_nuclide->doc );
  p_xpathCtx_nuclide->node = p_nuclide;

  /*============================================================================
  // Get Z
  //==========================================================================*/

  sx_str = xmlCharStrdup( ATOMIC_NUMBER );
  p_xpathObj_data =
    xmlXPathEvalExpression( sx_str, p_xpathCtx_nuclide );
  xmlFree( sx_str );

  if( !p_xpathObj_data ) {
    LIBNUCNET__NUC__ERROR( "Invalid xpath expression" );
  }

  if( p_xpathObj_data->nodesetval->nodeNr == 0 ) {
    LIBNUCNET__NUC__ERROR( "mass not found" );
  }

  sx_data = xmlNodeGetContent( p_xpathObj_data->nodesetval->nodeTab[0] );
  i_z = (unsigned int) xmlXPathCastStringToNumber( sx_data );
  xmlXPathFreeObject( p_xpathObj_data );
  xmlFree( sx_data );

  /*============================================================================
  // Get A
  //==========================================================================*/

  sx_str = xmlCharStrdup( MASS_NUMBER );
  p_xpathObj_data =
    xmlXPathEvalExpression( sx_str, p_xpathCtx_nuclide );
  xmlFree( sx_str );

  if( !p_xpathObj_data ) {
    LIBNUCNET__NUC__ERROR( "Invalid xpath expression" );
  }

  if( p_xpathObj_data->nodesetval->nodeNr == 0 ) {
    LIBNUCNET__NUC__ERROR( "mass not found" );
  }

  sx_data = xmlNodeGetContent( p_xpathObj_data->nodesetval->nodeTab[0] );
  i_a = (unsigned int) xmlXPathCastStringToNumber( sx_data );
  xmlXPathFreeObject( p_xpathObj_data );
  xmlFree( sx_data );

  /*============================================================================
  // Get State Data
  //==========================================================================*/

  sx_str = xmlCharStrdup( XPATH_STATE );
  p_xpathObj_data =
    xmlXPathEvalExpression( sx_str, p_xpathCtx_nuclide );
  xmlFree( sx_str );

  if( !p_xpathObj_data ) {
    LIBNUCNET__NUC__ERROR( "Invalid xpath expression" );
  }

  if ( p_xpathObj_data->nodesetval->nodeNr == 0 ) {
  
    Libnucnet__Nuc__getStates(
      self, p_nuclide, i_z, i_a, SINGLE_STATE
    );

    xmlXPathFreeObject( p_xpathObj_data );
    xmlXPathFreeContext( p_xpathCtx_nuclide );
    return 1;

  }

  i_length = (unsigned int) p_xpathObj_data->nodesetval->nodeNr;

  for( i = 0; i < i_length; i++ ) {

    p_state = p_xpathObj_data->nodesetval->nodeTab[i];

    if( i == 0 ) {
    
      if( i_length == 1 ) {
        Libnucnet__Nuc__getStates(
          self, p_state, i_z, i_a, SINGLE_STATE
        );
      } else {
        Libnucnet__Nuc__getStates(
          self, p_state, i_z, i_a, GROUND_STATE
        );
      }

    } else {

      Libnucnet__Nuc__getStates(
        self, p_state, i_z, i_a, EXCITED_STATE
      );

    }

  }

  /*============================================================================
  // Clean up.
  //==========================================================================*/

  xmlXPathFreeObject( p_xpathObj_data );
  xmlXPathFreeContext( p_xpathCtx_nuclide );
 
  return 1;

}

/*##############################################################################
// Libnucnet__Nuc__getStates().
//############################################################################*/

void Libnucnet__Nuc__getStates( 
  Libnucnet__Nuc *self,
  xmlNode *p_state,
  unsigned int i_z,
  unsigned int i_a,
  int i_append
)
{

  Libnucnet__Species *p_species;
  xmlXPathContextPtr p_xpathCtx_state;
  xmlXPathObjectPtr p_xpathObj_data;
  xmlChar *sx_str, *sx_data;
  xmlChar *sx_source;

  size_t i_count;
  double d_spin;
  double d_mass_excess;
  gsl_vector *p_t9 = NULL, *p_log10_partf = NULL;
  xmlChar *sx_state;

  /*============================================================================
  // Get state context.
  //==========================================================================*/

  p_xpathCtx_state = xmlXPathNewContext( p_state->doc );
  p_xpathCtx_state->node = p_state;

  /*============================================================================
  // Get Source
  //==========================================================================*/

  sx_str = xmlCharStrdup( SOURCE );
  p_xpathObj_data =
    xmlXPathEvalExpression( sx_str, p_xpathCtx_state );
  xmlFree( sx_str );

  if( !p_xpathObj_data ) {
    LIBNUCNET__NUC__ERROR( "Invalid xpath expression" );
  }

  if( p_xpathObj_data->nodesetval->nodeNr == 0 ) {
    sx_source = xmlCharStrdup( NUC_EMPTY_STRING );
  }
  else if( p_xpathObj_data->nodesetval->nodeNr == 1 ) {
    sx_source = xmlNodeGetContent( p_xpathObj_data->nodesetval->nodeTab[0] );
  } else {
    LIBNUCNET__NUC__ERROR( "More than one data source in input" );
  }

  xmlXPathFreeObject( p_xpathObj_data );

  /*============================================================================
  // Get Mass 
  //==========================================================================*/

  sx_str = xmlCharStrdup( MASS_EXCESS );
  p_xpathObj_data =
    xmlXPathEvalExpression( sx_str, p_xpathCtx_state );
  xmlFree( sx_str );

  if( p_xpathObj_data == NULL ) {
    LIBNUCNET__NUC__ERROR( "Invalid xpath expression" );
  }

  if( p_xpathObj_data->nodesetval->nodeNr == 0 ) {
    LIBNUCNET__NUC__ERROR( "mass not found" );
  }

  sx_data = xmlNodeGetContent( p_xpathObj_data->nodesetval->nodeTab[0] );

  d_mass_excess =
    xmlXPathCastStringToNumber( sx_data );
  xmlXPathFreeObject( p_xpathObj_data );
  xmlFree( sx_data );

  /*============================================================================
  // Get Spin  
  //==========================================================================*/

  sx_str = xmlCharStrdup( SPIN );
  p_xpathObj_data =
    xmlXPathEvalExpression( sx_str, p_xpathCtx_state );
  xmlFree( sx_str );

  if( !p_xpathObj_data ) {
    LIBNUCNET__NUC__ERROR( "Invalid xpath expression" );
  }

  if( p_xpathObj_data->nodesetval->nodeNr == 0 ) {
    LIBNUCNET__NUC__ERROR( "spin not found" );
  }

  sx_data = xmlNodeGetContent( p_xpathObj_data->nodesetval->nodeTab[0] );
  d_spin = xmlXPathCastStringToNumber( sx_data );
  xmlXPathFreeObject( p_xpathObj_data );
  xmlFree( sx_data );

  /*============================================================================
  // Count t9 array
  //==========================================================================*/

  sx_str = xmlCharStrdup( XPATH_T9 );
  p_xpathObj_data =
    xmlXPathEvalExpression( sx_str, p_xpathCtx_state );
  xmlFree( sx_str );

  if( !p_xpathObj_data ) {
    LIBNUCNET__NUC__ERROR( "Invalid xpath expression" );
  }

  if( p_xpathObj_data->nodesetval->nodeNr != 0 )
  {

  /*============================================================================
  // Allocate memory for t9 array.
  //==========================================================================*/

    if( 
        !(
           p_t9 =
             gsl_vector_calloc( (size_t) p_xpathObj_data->nodesetval->nodeNr )
        )
    )
      LIBNUCNET__NUC__ERROR( "Virtual memory exhausted" );

  /*============================================================================
  // Get t9 array
  //==========================================================================*/

    for(
      i_count = 0;
      i_count < WnMatrix__get_gsl_vector_size( p_t9 );
      i_count++
    )
    {
 
      sx_data =
        xmlNodeGetContent( p_xpathObj_data->nodesetval->nodeTab[i_count] );
      gsl_vector_set(
        p_t9,
        i_count,
        xmlXPathCastStringToNumber( sx_data )
      );
      xmlFree( sx_data );

    }

  }

  xmlXPathFreeObject( p_xpathObj_data );

  /*============================================================================
  // Count partf array
  //==========================================================================*/

  sx_str = xmlCharStrdup( PARTF );
  p_xpathObj_data =
    xmlXPathEvalExpression( sx_str, p_xpathCtx_state );
  xmlFree( sx_str );

  if( !p_xpathObj_data ) {
    LIBNUCNET__NUC__ERROR( "Invalid xpath expression" );
  }

  if( p_xpathObj_data->nodesetval->nodeNr != 0 )
  {

    if( p_t9 )
      if(
          (size_t) p_xpathObj_data->nodesetval->nodeNr !=
          WnMatrix__get_gsl_vector_size( p_t9 )
      )
        LIBNUCNET__NUC__ERROR( "t9 and partf arrays not equal in length" );

    if( !p_t9 )
      LIBNUCNET__NUC__ERROR( "t9 and partf arrays not equal in length" );

  /*============================================================================
  // Allocate memory for arrays.
  //==========================================================================*/

    if( 
        !( 
           p_log10_partf =
             gsl_vector_calloc( (size_t) p_xpathObj_data->nodesetval->nodeNr )
        )
    ) {
      LIBNUCNET__NUC__ERROR( "Virtual memory exhausted" );
    }

  /*============================================================================
  // Get log10 partf array
  //==========================================================================*/

    for(
      i_count = 0;
      i_count < WnMatrix__get_gsl_vector_size( p_log10_partf );
      i_count++
    )
    {

      sx_data =
        xmlNodeGetContent( p_xpathObj_data->nodesetval->nodeTab[i_count] );
      gsl_vector_set(
        p_log10_partf,
        i_count,
        xmlXPathCastStringToNumber( sx_data )
      );
      xmlFree( sx_data );

    }

  }

  xmlXPathFreeObject( p_xpathObj_data );


  /*============================================================================
  // Assign state.
  //==========================================================================*/

  sx_str = xmlCharStrdup( ID );
  sx_state = xmlGetProp( p_state, sx_str );
  xmlFree( sx_str );

  /*============================================================================
  // Add species.
  //==========================================================================*/

  p_species =
    Libnucnet__Species__new(
      i_z, i_a, (char *) sx_source, i_append, (char *) sx_state,
      d_mass_excess, d_spin, p_t9, p_log10_partf
    );

  if( !Libnucnet__Nuc__updateSpecies( self, p_species ) )
    LIBNUCNET__NUC__ERROR( "Couldn't update species.\n" );

  /*============================================================================
  // Clean up.
  //==========================================================================*/

  xmlXPathFreeContext( p_xpathCtx_state );
  if( p_t9 ) gsl_vector_free( p_t9 );
  if( p_log10_partf ) gsl_vector_free( p_log10_partf );
  xmlFree( sx_source );
  xmlFree( sx_state );

  return;

}

/*##############################################################################
// Libnucnet__Species__new().
//############################################################################*/

Libnucnet__Species *
Libnucnet__Species__new(
  unsigned int i_z,
  unsigned int i_a,
  const char *s_source,
  int i_state,
  const char *s_state,
  double d_mass_excess,
  double d_spin,
  gsl_vector *p_t9,
  gsl_vector *p_log10_partf
)
{

  Libnucnet__Species *p_species;

  /*============================================================================
  // Allocate memory for species.
  //==========================================================================*/

  p_species = ( Libnucnet__Species * ) malloc ( sizeof( Libnucnet__Species ) );

  if( !p_species ) {
    LIBNUCNET__NUC__ERROR( "Virtual memory exhausted" );
  }

  p_species->sxName = NULL;
  p_species->sxBaseName = NULL;
  p_species->sxState = NULL;
  p_species->sxSource = NULL;
  p_species->pT9 = NULL;
  p_species->pLog10Partf = NULL;

  /*============================================================================
  // Get nuclide string
  //==========================================================================*/

  p_species->sxName = Libnucnet__Nuc__create_species_name( i_z, i_a );
  p_species->sxBaseName = xmlStrdup( p_species->sxName );

  if( i_state != SINGLE_STATE )
    p_species->sxState = xmlCharStrdup( s_state );

  p_species->sxName =
    xmlStrcat( p_species->sxName, p_species->sxState );
  
  /*============================================================================
  // Assign quantities.
  //==========================================================================*/

  p_species->iZ = i_z;
  p_species->iA = i_a;

  Libnucnet__Species__updateMassExcess( p_species, d_mass_excess );
  Libnucnet__Species__updateSpin( p_species, d_spin );
  Libnucnet__Species__updateSource( p_species, s_source );

  /*============================================================================
  // Add partition function data.
  //==========================================================================*/

  Libnucnet__Species__updatePartitionFunctionData(
    p_species, p_t9, p_log10_partf
  );

  /*============================================================================
  // Done. Return.
  //==========================================================================*/

  return p_species;

}

/*##############################################################################
// Libnucnet__Nuc__addSpecies().
//############################################################################*/

int
Libnucnet__Nuc__addSpecies(
  Libnucnet__Nuc *self,
  Libnucnet__Species *p_species
)
{

  /*============================================================================
  // Assign species to hash.
  //==========================================================================*/

  if(
      xmlHashAddEntry(
        self->pSpeciesHash,
        p_species->sxName,
        p_species
      )
  )
    return 0;

  /*============================================================================
  // Assign index.
  //==========================================================================*/

  p_species->iIndex = Libnucnet__Nuc__getNumberOfSpecies( self ) - 1;

  /*============================================================================
  // Change update flag.
  //==========================================================================*/

  self->iUpdate++;

  return 1;

}

/*##############################################################################
// Libnucnet__Nuc__updateSpecies().
//############################################################################*/

int
Libnucnet__Nuc__updateSpecies(
  Libnucnet__Nuc *self,
  Libnucnet__Species *p_species
)
{

  xmlListPtr p_list;
  Libnucnet__Species *p_old;

  /*============================================================================
  // Get old species.
  //==========================================================================*/

  p_old =
    ( Libnucnet__Species * )
    xmlHashLookup( self->pSpeciesHash, p_species->sxName );

  /*============================================================================
  // Check not updating same species.
  //==========================================================================*/

  if( p_old == p_species )
    return 0;

  /*============================================================================
  // If old species exists, remove it.  Otherwise, check for two possibilities.
  // 1)  Old species does not have states but new one does (e.g., old species
  // is al26 and new one is al26g).  2)  Old species does include states but
  // new one does not (e.g., old species is al26g and m, new one is al26).
  // In either case, remove all old species (and states).
  //==========================================================================*/

  if( p_old )
    Libnucnet__Nuc__removeSpecies( self, p_old );
  else if( p_species->sxState )
    Libnucnet__Nuc__removeSpecies(
      self,
      Libnucnet__Nuc__getSpeciesByName(
        self,
        (const char *) p_species->sxBaseName
      )
    );
  else
  {
    p_list =
      Libnucnet__Nuc__createSpeciesStateList(
        self,
        p_species->sxBaseName
      );

    xmlListWalk(
      p_list,
      (xmlListWalker) Libnucnet__Species__removeStates,
      self
    );

    xmlListDelete( p_list );
  }

  /*============================================================================
  // Add new species.
  //==========================================================================*/

  return
    Libnucnet__Nuc__addSpecies( self, p_species );

}


/*##############################################################################
// Libnucnet__Nuc__create_species_name().
//############################################################################*/

xmlChar *Libnucnet__Nuc__create_species_name( 
  unsigned int i_z, unsigned int i_a
)
{

  xmlChar *sx_species_name, *sx_str, *sx_str2;
  const char *s_zname[] =
                    {"n"  ,
                     "h"  , "he" , "li" , "be" , "b"  ,
                     "c"  , "n"  , "o"  , "f"  , "ne" ,
                     "na" , "mg" , "al" , "si" , "p"  ,
                     "s"  , "cl" , "ar" , "k"  , "ca" ,
                     "sc" , "ti" , "v"  , "cr" , "mn" ,
                     "fe" , "co" , "ni" , "cu" , "zn" ,
                     "ga" , "ge" , "as" , "se" , "br" ,
                     "kr" , "rb" , "sr" , "y"  , "zr" ,
                     "nb" , "mo" , "tc" , "ru" , "rh" ,
                     "pd" , "ag" , "cd" , "in" , "sn" ,
                     "sb" , "te" , "i"  , "xe" , "cs" ,
                     "ba" , "la" , "ce" , "pr" , "nd" ,
                     "pm" , "sm" , "eu" , "gd" , "tb" ,
                     "dy" , "ho" , "er" , "tm" , "yb" ,
                     "lu" , "hf" , "ta" , "w"  , "re" ,
                     "os" , "ir" , "pt" , "au" , "hg" ,
                     "tl" , "pb" , "bi" , "po" , "at" ,
                     "rn" , "fr" , "ra" , "ac" , "th" ,
                     "pa" , "u"  , "np" , "pu" , "am" ,
                     "cm" , "bk" , "cf" , "es" , "fm" ,
                     "md" , "no" , "lr" , "rf" , "db" ,
                     "sg" , "bh" , "hs" , "mt" , "ds" ,
                     "rg" , "cn"
                    };

  if( i_a == 0 ) return NULL;

  if( i_z > sizeof( s_zname ) / sizeof( s_zname[0] ) - 1 )
    sx_species_name = Libnucnet__Nuc__create_unassigned_element_name( i_z );
  else
    sx_species_name = xmlCharStrdup( s_zname[i_z] );

  if( !sx_species_name ) return NULL;

  sx_str = (xmlChar *) malloc( sizeof( xmlChar ) * NUC_BUF_SIZE );
  xmlStrPrintf( sx_str, NUC_BUF_SIZE, (const xmlChar *) "%d", i_a );

  if ( i_z == 0 ) {

    if( i_a == 2 )
    {
      sx_str2 = xmlStrdup( sx_species_name );
      sx_species_name = xmlStrcat( sx_species_name, sx_str2 );
      xmlFree( sx_str2 );
    }

    if( i_a > 2 )
      LIBNUCNET__NUC__ERROR( "Invalid neutron!" );

  }
  else
    sx_species_name = xmlStrcat( sx_species_name, sx_str );

  xmlFree( sx_str );

  return sx_species_name;

}

/*##############################################################################
// Libnucnet__Nuc__create_unassigned_element_name().
//############################################################################*/

xmlChar *
Libnucnet__Nuc__create_unassigned_element_name(
  unsigned int i_z
)
{

  const xmlChar *sx_unassigned_str[] =
    {
      (const xmlChar *) "n",
      (const xmlChar *) "u",
      (const xmlChar *) "b",
      (const xmlChar *) "t",
      (const xmlChar *) "q",
      (const xmlChar *) "p",
      (const xmlChar *) "h",
      (const xmlChar *) "s",
      (const xmlChar *) "o",
      (const xmlChar *) "e"
    };

  xmlChar *sx_str, *sx_name, *sx_sub;
  int i;

  sx_str = xmlCharStrdup( NUC_EMPTY_STRING );
  while( i_z != 0 )
  {
    sx_str =
      xmlStrcat(
        sx_str,
        sx_unassigned_str[i_z % 10]
      );
    i_z /= 10;
  }

  sx_name = xmlCharStrdup( NUC_EMPTY_STRING );
  for( i = xmlStrlen( sx_str ) - 1; i >= 0; i-- )
  {
    sx_sub = xmlStrsub( sx_str, i, 1 );
    sx_name = xmlStrcat( sx_name, sx_sub );
    xmlFree( sx_sub );
  }

  xmlFree( sx_str );

  return sx_name;

}

/*##############################################################################
// Libnucnet__Nuc__getSpeciesByName().
//############################################################################*/

Libnucnet__Species
*Libnucnet__Nuc__getSpeciesByName( 
  const Libnucnet__Nuc *self,
  const char *s_nucname 
) {

  Libnucnet__Species *p_species;
  xmlChar *sx_species;

  if( !self ) {
    LIBNUCNET__NUC__ERROR( "Invalid nuclei collection" );
  }

  sx_species = xmlCharStrdup( s_nucname );

  p_species =
    ( Libnucnet__Species * )
    xmlHashLookup(
      self->pSpeciesHash, sx_species
    );

  xmlFree( sx_species );

  return p_species;

}

/*##############################################################################
// Libnucnet__Nuc__getNumberOfSpecies().
//############################################################################*/

size_t
Libnucnet__Nuc__getNumberOfSpecies( const Libnucnet__Nuc *self ) {

  if( !self ) {
    LIBNUCNET__NUC__ERROR( "Invalid Network" );
  }

  return (size_t) xmlHashSize( self->pSpeciesHash );

}

/*##############################################################################
// Libnucnet__Species__getZ().
//############################################################################*/

unsigned int
Libnucnet__Species__getZ( const Libnucnet__Species *self )
{

  if( !self ) {
    LIBNUCNET__NUC__ERROR( "Invalid species" );
  } else {
    return self->iZ;
  }

}

/*##############################################################################
// Libnucnet__Species__getA().
//############################################################################*/

unsigned int
Libnucnet__Species__getA(
  const Libnucnet__Species *self
) {

  if( !self ) {
    LIBNUCNET__NUC__ERROR( "Invalid species" );
  } else {
    return self->iA;
  }

}

/*##############################################################################
// Libnucnet__Species__getMassExcess().
//############################################################################*/

double
Libnucnet__Species__getMassExcess(
  const Libnucnet__Species *self
) {

  if( !self ) {
    LIBNUCNET__NUC__ERROR( "Invalid species" );
  } else {
    return self->dMassExcess;
  }

}

/*##############################################################################
// Libnucnet__Species__getSpin().
//############################################################################*/

double
Libnucnet__Species__getSpin(
  const Libnucnet__Species *self
) {

  if( !self ) {
    LIBNUCNET__NUC__ERROR( "Invalid species" );
  } else {
    return self->dSpin;
  }

}

/*##############################################################################
// Libnucnet__Species__getName().
//############################################################################*/

const char *
Libnucnet__Species__getName(
  const Libnucnet__Species *self
) {

  if( !self ) {
    LIBNUCNET__NUC__ERROR( "Invalid species" );
  } else {
    return (const char *) self->sxName;
  }

}

/*##############################################################################
// Libnucnet__Species__getSource().
//############################################################################*/

const char *
Libnucnet__Species__getSource(
  const Libnucnet__Species *self
)
{

  if( !self ) {
    LIBNUCNET__NUC__ERROR( "Invalid species" );
  } else {
    return (const char *) self->sxSource;
  }

}

/*##############################################################################
// Libnucnet__Nuc__getSpeciesByZA().
//############################################################################*/

Libnucnet__Species *
Libnucnet__Nuc__getSpeciesByZA(
  const Libnucnet__Nuc *self,
  unsigned int i_z,
  unsigned int i_a,
  const char *s_state
) {

  Libnucnet__Species *p_species;
  char *s_nucname;
  xmlChar *sx_name;

  sx_name = Libnucnet__Nuc__create_species_name( i_z, i_a );

  if( !sx_name ) return NULL;

  s_nucname =
    (char *) malloc( NUC_BUF_SIZE * sizeof( char ) );

  strcpy(
    s_nucname,
    (char *) sx_name
  );

  xmlFree( sx_name );

  if( !self ) {
    LIBNUCNET__NUC__ERROR( "Invalid input structure" );
  }

  if( s_state ) {
    strcat( s_nucname, s_state );
  }

  p_species = Libnucnet__Nuc__getSpeciesByName( self, s_nucname );
  free( s_nucname );

  return p_species;

}

/*##############################################################################
// Libnucnet__Species__computePartitionFunction().
//############################################################################*/

double
Libnucnet__Species__computePartitionFunction(
  const Libnucnet__Species *self,
  double d_t9
) {

  double d_exp = 0;
  gsl_interp_accel *acc;
  gsl_spline *spline;

  /*============================================================================
  // Check that species present in network.
  //==========================================================================*/

  if( !self ) {
    LIBNUCNET__NUC__ERROR( "Invalid species" );
  }

  /*============================================================================
  // Check temperature.
  //==========================================================================*/

  if( d_t9 < 0 ) {
    LIBNUCNET__NUC__ERROR( "Invalid temperature" );
  }

  /*============================================================================
  // If size = 0, no table so return g.s. partition function.
  //==========================================================================*/

  if( !self->pT9 )
    return ( ( 2 * self->dSpin ) + 1 );

  /*============================================================================
  // Check if t9 inside table.  If so, interpolate.  Else use table endpoints.
  // If table size is only 2, can't use spline.  Do simple linear interpolation
  // inside table.
  //==========================================================================*/

  if( d_t9 <= self->pT9->data[0] )
    d_exp = self->pLog10Partf->data[0];
  else if( d_t9 >= self->pT9->data[self->pT9->size - 1] )
    d_exp = self->pLog10Partf->data[self->pLog10Partf->size - 1];
  else if( self->pT9->size == 2 )
  {
    d_exp =
      self->pLog10Partf->data[0] + 
      ( d_t9 - self->pT9->data[0] ) *
      ( self->pLog10Partf->data[1] - self->pLog10Partf->data[0] ) /
      ( self->pT9->data[1] - self->pT9->data[0] );
  }
  else
  {
    acc = gsl_interp_accel_alloc();
    spline =
      gsl_spline_alloc( gsl_interp_cspline, self->pT9->size );
    gsl_spline_init(
      spline,
      self->pT9->data,
      self->pLog10Partf->data,
      self->pT9->size
    );

    d_exp = gsl_spline_eval( spline, d_t9, acc );

    gsl_spline_free( spline );
    gsl_interp_accel_free( acc );

  }
    
  /*============================================================================
  // Return partition function.
  //==========================================================================*/

  return ( ( 2 * self->dSpin ) + 1 ) * pow( 10., d_exp );

}

/*##############################################################################
// Libnucnet__Nuc__computeSpeciesNseFactor().
//############################################################################*/

double
Libnucnet__Nuc__computeSpeciesNseFactor(
  const Libnucnet__Nuc *self,
  const Libnucnet__Species *p_species,
  double d_t9,
  double d_rho
) {

  /*============================================================================
  // Check input.
  //==========================================================================*/

  if( !self ) {
    LIBNUCNET__NUC__ERROR( "Invalid collection of nuclei" );
  }

  /*============================================================================
  // Get neutron and proton data.
  //==========================================================================*/

  if(
      !Libnucnet__Nuc__getSpeciesByName( self, NEUT )
  )
    LIBNUCNET__NUC__ERROR( "Neutron not present in network" );

  if(
      !Libnucnet__Nuc__getSpeciesByName( self, PROT )
  )
    LIBNUCNET__NUC__ERROR( "Proton not present in network" );

  /*============================================================================
  // Return NSE factor.
  //==========================================================================*/

  return
    log(
      Libnucnet__Species__computeQuantumAbundance(
        p_species,
        d_t9,
        d_rho
      )
    )
    +
    (
      Libnucnet__Nuc__computeSpeciesBindingEnergy( self, p_species ) /
      (
        GSL_CONST_CGSM_BOLTZMANN *
        WN_ERGS_TO_MEV *
        GSL_CONST_NUM_GIGA *
        d_t9
      )
    );
    
}

/*##############################################################################
// Libnucnet__Species__computeQuantumAbundance().
//############################################################################*/

double
Libnucnet__Species__computeQuantumAbundance(
  const Libnucnet__Species *self,
  double d_t9,
  double d_rho
)
{

  double d_xmnu;

  /*============================================================================
  // Check input.
  //==========================================================================*/

  if( !self ) {
    LIBNUCNET__NUC__ERROR( "Invalid species" );
  }

  if( d_t9 <= 0. ) {
    LIBNUCNET__NUC__ERROR( "Invalid temperature" );
  }

  if( d_rho <= 0. ) {
    LIBNUCNET__NUC__ERROR( "Invalid density" );
  }

  /*============================================================================
  // Compute species mass.
  //==========================================================================*/

  d_xmnu =
    WN_AMU_TO_MEV *
    Libnucnet__Species__getA( self ) +
    Libnucnet__Species__getMassExcess( self );

  /*============================================================================
  // Return quantum abundance.
  //==========================================================================*/

  return
    (
      Libnucnet__Species__computePartitionFunction( self, d_t9 ) / 
      ( d_rho * GSL_CONST_NUM_AVOGADRO )
    )
    *
    pow(
      ( 
        d_xmnu *
        WN_MEV_TO_ERGS *
        GSL_CONST_CGSM_BOLTZMANN * 
        d_t9 *
        GSL_CONST_NUM_GIGA
      )
      /
      (
        2 * M_PI *
        gsl_pow_2(
          GSL_CONST_CGSM_PLANCKS_CONSTANT_HBAR *
          GSL_CONST_CGSM_SPEED_OF_LIGHT
        )
      ),
      1.5
    );

}

/*##############################################################################
// Libnucnet__Species__getIndex().
//############################################################################*/

size_t
Libnucnet__Species__getIndex( const Libnucnet__Species *self )
{

  if( !self ) {

    LIBNUCNET__NUC__ERROR( "Invalid species" );

  } else {

    return self->iIndex;

  }

}

/*##############################################################################
// Libnucnet__Nuc__initializeHash()
//############################################################################*/

void Libnucnet__Nuc__initializeHash( Libnucnet__Nuc *self ) {

  self->pSpeciesHash = xmlHashCreate( 0 );

  return;

}
  
/*##############################################################################
// Libnucnet__Species__hashFree()
//############################################################################*/

void
Libnucnet__Species__hashFree(
  Libnucnet__Species *self,
  xmlChar * sx_name 
) {

  if( !sx_name ) {
    LIBNUCNET__NUC__ERROR( "Cannot free non-existent species" );
  }

  Libnucnet__Species__free( self );

}

/*##############################################################################
// Libnucnet__Species__free()
//############################################################################*/

void
Libnucnet__Species__free( Libnucnet__Species *self )
{

  xmlFree( self->sxName );
  xmlFree( self->sxBaseName );
  xmlFree( self->sxState );
  xmlFree( self->sxSource );
  if( self->pT9 ) gsl_vector_free( self->pT9 );
  if( self->pLog10Partf ) gsl_vector_free( self->pLog10Partf );
  free( self );

  return;

}

/*##############################################################################
// Libnucnet__Nuc__removeSpecies()
//############################################################################*/

int
Libnucnet__Nuc__removeSpecies(
  Libnucnet__Nuc * self, Libnucnet__Species * p_species
){

   size_t i;
   Libnucnet__Species **p_species_array;

   if( !self )
     LIBNUCNET__NUC__ERROR( "Invalid input structure" );

   if( !p_species ) return 0;

   if(
     xmlHashRemoveEntry(
        self->pSpeciesHash,
        p_species->sxName,
        (xmlHashDeallocator) Libnucnet__Species__hashFree
     )
   )
     return 0;

   p_species_array = Libnucnet__Nuc__createSpeciesArray( self );

   for( i = 0; i < Libnucnet__Nuc__getNumberOfSpecies( self ); i++ )
     p_species_array[i]->iIndex = i;

   free( p_species_array );

   self->iUpdate++;

   return 1;

}

/*##############################################################################
// Libnucnet__Nuc__free()
//############################################################################*/

void
Libnucnet__Nuc__free(
  Libnucnet__Nuc * self
){

   if( !self ) return;

   if( self->iOwner )
   {
     xmlHashFree(
       self->pSpeciesHash,
       (xmlHashDeallocator) Libnucnet__Species__hashFree
     );
   } 
   else
   {
     xmlHashFree(
       self->pSpeciesHash,
       NULL
     );
   }

   free( self );

}

/*##############################################################################
// Libnucnet__Nuc__is_valid_input_xml()
//############################################################################*/

int Libnucnet__Nuc__is_valid_input_xml( const char *s_filename ) {

  xmlDocPtr p_doc;
  xmlSchemaParserCtxtPtr p_parser_ctxt;
  xmlSchemaValidCtxtPtr p_valid_ctxt;
  xmlSchemaPtr p_schema;
  int i_valid;

  p_parser_ctxt = xmlSchemaNewParserCtxt( LIBNUCNET__NUC__SCHEMA );

  if( !(
        p_schema = xmlSchemaParse( p_parser_ctxt )
      )
  ) {
      LIBNUCNET__NUC__ERROR( "Schema not found or invalid" );
  }

  p_valid_ctxt = xmlSchemaNewValidCtxt( p_schema );

  p_doc = xmlParseFile( s_filename );

  if( xmlSchemaValidateDoc( p_valid_ctxt, p_doc ) == 0 ) {
     i_valid = 1;
  } else {
     i_valid = 0;
  }

  xmlFreeDoc( p_doc );
  xmlSchemaFreeValidCtxt( p_valid_ctxt );
  xmlSchemaFree( p_schema );
  xmlSchemaFreeParserCtxt( p_parser_ctxt );

  xmlCleanupParser();

  return i_valid;

}

/*##############################################################################
// Libnucnet__Nuc__setSpeciesCompareFunction().
//############################################################################*/

void
Libnucnet__Nuc__setSpeciesCompareFunction(
  Libnucnet__Nuc *self,
  Libnucnet__Species__compare_function pf_func
)
{

  self->pfSpeciesCompare = pf_func;

}

/*##############################################################################
// Libnucnet__Nuc__clearSpeciesCompareFunction().
//############################################################################*/

void
Libnucnet__Nuc__clearSpeciesCompareFunction(
  Libnucnet__Nuc *self
)
{

  self->pfSpeciesCompare = 
    (Libnucnet__Species__compare_function)
    Libnucnet__Species__default_compare_function;

}

/*##############################################################################
// Libnucnet__Species__index_compare_function().
//############################################################################*/

int
Libnucnet__Species__index_compare_function(
  const Libnucnet__Species *p_species_1,
  const Libnucnet__Species *p_species_2
) {

  if( p_species_1->iIndex < p_species_2->iIndex )
    return -1;
  else
    return 1;

}
   
/*##############################################################################
// Libnucnet__Species__default_compare_function().
//############################################################################*/

int
Libnucnet__Species__default_compare_function(
  const Libnucnet__Species *p_species_1,
  const Libnucnet__Species *p_species_2
) {

  int i;

  if( p_species_1->iZ < p_species_2->iZ ) {
    return -1;
  } else if( p_species_1->iZ > p_species_2->iZ ) {
    return 1;
  }

  if( p_species_1->iA < p_species_2->iA ) {
    return -1;
  } else if( p_species_1->iA > p_species_2->iA ) {
    return 1;
  }

  i = xmlStrcmp( p_species_1->sxName, p_species_2->sxName );

  if( i == 0 ) {
    return 0;
  } else {
    return GSL_SIGN( i );
  }

}
   
/*##############################################################################
// Libnucnet__Nuc__computeSpeciesBindingEnergy( )
//############################################################################*/

double
Libnucnet__Nuc__computeSpeciesBindingEnergy(
  const Libnucnet__Nuc *self,
  const Libnucnet__Species *p_species
) {

  if( !self || !p_species ) {
    LIBNUCNET__NUC__ERROR( "Invalid input" );
  }

  return
    p_species->iZ *
      Libnucnet__Species__getMassExcess( 
        Libnucnet__Nuc__getSpeciesByName( self, PROT )
      )
    + ( p_species->iA - p_species->iZ ) *
      Libnucnet__Species__getMassExcess(
        Libnucnet__Nuc__getSpeciesByName( self, NEUT )
      )
    - Libnucnet__Species__getMassExcess( p_species );

}

/*##############################################################################
// Libnucnet__Nuc__makeXmlDocument()
//############################################################################*/

xmlDocPtr
Libnucnet__Nuc__makeXmlDocument(
  const Libnucnet__Nuc *self
)
{

  typedef struct {
    const Libnucnet__Nuc *pNuc;
    xmlNodePtr pNode;
  } user_data;


  xmlDocPtr p_doc;
  xmlNodePtr p_root;
  xmlListPtr p_list;
  xmlChar *sx_str;
  user_data *p_user_data;

  /*============================================================================
  // Check input structure.
  //==========================================================================*/

  if( !self ) {
     LIBNUCNET__NUC__ERROR( "Invalid input structure" );
  }

  /*============================================================================
  // Get document.
  //==========================================================================*/

  sx_str = xmlCharStrdup( XML_VERSION );
  p_doc = xmlNewDoc( sx_str );
  xmlFree( sx_str );

  if (p_doc == NULL) {
    LIBNUCNET__NUC__ERROR( "DOMImplementation.createDocument: failed" );
  }

  /*============================================================================
  // Set root.
  //==========================================================================*/

  sx_str = xmlCharStrdup( NUCLEAR_DATA );
  p_root = xmlNewNode( NULL, sx_str );
  xmlFree( sx_str );
  xmlDocSetRootElement( p_doc, p_root );

  /*============================================================================
  // User data.
  //==========================================================================*/

  p_user_data = ( user_data * ) malloc( sizeof( user_data ) );

  if( !p_user_data ) LIBNUCNET__NUC__ERROR( "Couldn't allocate memory" );

  p_user_data->pNuc = self;
  p_user_data->pNode = p_root;

  /*============================================================================
  // Loop on species to make name list.
  //==========================================================================*/

  p_list =
    Libnucnet__Nuc__createSpeciesBaseNameList( self );

  /*============================================================================
  // Loop on species.
  //==========================================================================*/

  xmlListWalk(
    p_list,
    (xmlListWalker) Libnucnet__Species__make_xml,
    p_user_data
  );

  /*============================================================================
  // Clean up.
  //==========================================================================*/

  xmlListDelete( p_list );
  free( p_user_data );

  /*============================================================================
  // Done.
  //==========================================================================*/

  return p_doc;

}

/*##############################################################################
// Libnucnet__Species__make_xml()
//############################################################################*/

int
Libnucnet__Species__make_xml(
  xmlChar *sx_name,
  void *p_data
)
{

  typedef struct {
    const Libnucnet__Nuc *pNuc;
    xmlNodePtr pNode;
  } user_data;

  xmlNodePtr p_comment, p_nuclide, p_states;
  xmlChar *sx_str, *sx_str2;
  Libnucnet__Species *p_species;
  xmlListPtr p_list;

  user_data *p_user_data = ( user_data * ) p_data;

  p_list =
    Libnucnet__Nuc__createSpeciesStateList( p_user_data->pNuc, sx_name );

  /*---------------------------------------------------------------------------
  // Get species.
  //--------------------------------------------------------------------------*/

  p_species =
    (Libnucnet__Species *)
    xmlLinkGetData(
      xmlListFront( p_list )
    );

  /*---------------------------------------------------------------------------
  // Create comment and add.
  //--------------------------------------------------------------------------*/

  p_comment =
    xmlNewComment( p_species->sxBaseName );

  xmlAddChild( p_user_data->pNode, p_comment );

  /*---------------------------------------------------------------------------
  // Get nuclide element.
  //--------------------------------------------------------------------------*/

  sx_str = xmlCharStrdup( NUCLIDE );
  p_nuclide = xmlNewNode( NULL, sx_str );
  xmlFree( sx_str );

  if( p_nuclide == NULL ) {
    LIBNUCNET__NUC__ERROR( "Document.createElement: NULL\n\tException" );
  }

  /*---------------------------------------------------------------------------
  // Insert Z.
  //--------------------------------------------------------------------------*/

  sx_str = xmlCharStrdup( ATOMIC_NUMBER );
  sx_str2 = (xmlChar *) malloc( sizeof( xmlChar ) * NUC_BUF_SIZE );
  xmlStrPrintf(
    sx_str2,
    NUC_BUF_SIZE,
    (const xmlChar *) "%d",
    Libnucnet__Species__getZ( p_species )
  );
  xmlNewChild( p_nuclide, NULL, sx_str, sx_str2 );
  xmlFree( sx_str2 );
  xmlFree( sx_str );

  /*---------------------------------------------------------------------------
  // Insert A.
  //--------------------------------------------------------------------------*/

  sx_str = xmlCharStrdup( MASS_NUMBER );
  sx_str2 = (xmlChar *) malloc( sizeof( xmlChar ) * NUC_BUF_SIZE );
  xmlStrPrintf(
    sx_str2,
    NUC_BUF_SIZE,
    (const xmlChar *) "%d",
    Libnucnet__Species__getA( p_species )
  );
  xmlNewChild( p_nuclide, NULL, sx_str, sx_str2 );
  xmlFree( sx_str2 );
  xmlFree( sx_str );

  if( xmlListSize( p_list ) == 1 )
    Libnucnet__Species__addStateXmlData( p_species, p_nuclide );
  else
  {
    sx_str = xmlCharStrdup( STATES );
    p_states = xmlNewNode( NULL, sx_str );
    xmlFree( sx_str );

    xmlListWalk(
      p_list,
      (xmlListWalker) Libnucnet__Species__makeStateXml,
      p_states
    );

    xmlAddChild( p_nuclide, p_states );
  }

  xmlAddChild( p_user_data->pNode, p_nuclide );

  xmlListDelete( p_list );

  return 1;

}

/*##############################################################################
// Libnucnet__Species__makeStateXml()
//############################################################################*/

int
Libnucnet__Species__makeStateXml(
  Libnucnet__Species *self,
  xmlNodePtr p_node
)
{

  xmlNodePtr p_state;
  xmlChar *sx_str;

  sx_str = xmlCharStrdup( STATE );
  p_state = xmlNewNode( NULL, sx_str );
  xmlFree( sx_str );

  sx_str = xmlCharStrdup( ID );
  xmlNewProp( p_state, sx_str, self->sxState );
  xmlFree( sx_str );

  Libnucnet__Species__addStateXmlData( self, p_state );

  xmlAddChild( p_node, p_state );

  return 1;

}

/*##############################################################################
// Libnucnet__Species__addStateXmlData()
//############################################################################*/

void
Libnucnet__Species__addStateXmlData(
  Libnucnet__Species *self,
  xmlNodePtr p_node
)
{

  xmlNodePtr p_partf, p_point;
  size_t i;
  xmlChar *sx_str, *sx_str2;

  /*---------------------------------------------------------------------------
  // Insert data source string if not empty.
  //--------------------------------------------------------------------------*/

  if( strlen( Libnucnet__Species__getSource( self ) ) != 0 ) {

    sx_str = xmlCharStrdup( SOURCE );
    sx_str2 =
      xmlCharStrdup( Libnucnet__Species__getSource ( self ) );

    xmlNewChild(
      p_node,
      NULL,
      sx_str,
      sx_str2
    );

    xmlFree( sx_str );
    xmlFree( sx_str2 );

  }

  /*---------------------------------------------------------------------------
  // Insert mass excess.
  //--------------------------------------------------------------------------*/

  sx_str = xmlCharStrdup( MASS_EXCESS );

  sx_str2 = (xmlChar *) malloc( sizeof( xmlChar ) * NUC_BUF_SIZE );
  xmlStrPrintf(
    sx_str2,
    NUC_BUF_SIZE,
    (const xmlChar *) "%g",
    Libnucnet__Species__getMassExcess( self )
  );

  xmlNewChild(
     p_node,
     NULL,
     sx_str,
     sx_str2
  );

  xmlFree( sx_str2 );
  xmlFree( sx_str );

  /*---------------------------------------------------------------------------
  // Insert spin.
  //--------------------------------------------------------------------------*/

  sx_str = xmlCharStrdup( SPIN );

  sx_str2 = (xmlChar *) malloc( sizeof( xmlChar ) * NUC_BUF_SIZE );
  xmlStrPrintf(
    sx_str2,
    NUC_BUF_SIZE,
    (const xmlChar *) "%g",
    Libnucnet__Species__getSpin( self )
  );

  xmlNewChild(
     p_node,
     NULL,
     sx_str,
     sx_str2
  );

  xmlFree( sx_str2 );
  xmlFree( sx_str );

  /*---------------------------------------------------------------------------
  // Get partf element.
  //--------------------------------------------------------------------------*/

  sx_str = xmlCharStrdup( PARTF_TABLE );
  p_partf = xmlNewChild( p_node, NULL, sx_str, NULL );
  xmlFree( sx_str );

  /*---------------------------------------------------------------------------
  // If no partition function data, return.
  //--------------------------------------------------------------------------*/

  if( !self->pT9 && !self->pLog10Partf ) return;

  /*---------------------------------------------------------------------------
  // Insert partf points.
  //--------------------------------------------------------------------------*/

  for(
    i = 0;
    i < WnMatrix__get_gsl_vector_size( self->pT9 );
    i++
  )
  {

    /*........................................................................
    // Get point element.
    //......................................................................*/

    sx_str = xmlCharStrdup( PARTF_POINT );
    p_point = xmlNewChild( p_partf, NULL, sx_str, NULL );
    xmlFree( sx_str );

    /*........................................................................
    // Append t9 value to point.
    //......................................................................*/

    sx_str = xmlCharStrdup( PARTF_T9 );

    sx_str2 = (xmlChar *) malloc( sizeof( xmlChar ) * NUC_BUF_SIZE );
    xmlStrPrintf(
      sx_str2,
      NUC_BUF_SIZE,
      (const xmlChar *) "%g",
      gsl_vector_get( self->pT9, i )
    );

    xmlNewChild(
      p_point,
      NULL,
      sx_str,
      sx_str2
    );

    xmlFree( sx_str2 );
    xmlFree( sx_str );

    /*........................................................................
    // Append partf value to point.
    //......................................................................*/

    sx_str = xmlCharStrdup( PARTF_ENTRY );

    sx_str2 = (xmlChar *) malloc( sizeof( xmlChar ) * NUC_BUF_SIZE );
    xmlStrPrintf(
      sx_str2,
      NUC_BUF_SIZE,
      (const xmlChar *) "%g",
      gsl_vector_get( self->pLog10Partf, i )
    );

    xmlNewChild(
      p_point,
      NULL,
      sx_str,
      sx_str2
    );

    xmlFree( sx_str2 );
    xmlFree( sx_str );

  }

}

/*##############################################################################
// Libnucnet__Nuc__writeToXmlFile()
//############################################################################*/

void
Libnucnet__Nuc__writeToXmlFile(
  const Libnucnet__Nuc *self,
  const char *s_output_xml_filename
)
{

  xmlDocPtr p_doc = NULL;
  xmlChar *sx_str, *sx_str2;

  /*============================================================================
  // Check input structure.
  //==========================================================================*/

  if( !self ) {
     LIBNUCNET__NUC__ERROR( "Invalid input structure" );
  }

  /*============================================================================
  // Get document.
  //==========================================================================*/
 
  p_doc = Libnucnet__Nuc__makeXmlDocument( self );

  /*============================================================================
  // Add namespaces.
  //==========================================================================*/

  sx_str = xmlCharStrdup( W3C__NAMESPACE );
  sx_str2 = xmlCharStrdup( XSI );
  xmlNewNs( xmlDocGetRootElement( p_doc ), sx_str, sx_str2 );
  xmlFree( sx_str2 );
  xmlFree( sx_str );

  sx_str = xmlCharStrdup( XSI_SCHEMA_LOCATION );
  sx_str2 = xmlCharStrdup( LIBNUCNET__NUC__SCHEMALOCATION );
  xmlNewProp( xmlDocGetRootElement( p_doc ), sx_str, sx_str2 );
  xmlFree( sx_str2 );
  xmlFree( sx_str );

  /*============================================================================
  // Write file.
  //==========================================================================*/

  if(
       xmlSaveFormatFileEnc( s_output_xml_filename, p_doc, "UTF-8", 1 ) == -1
  ) {
      LIBNUCNET__NUC__ERROR(
        "DOMImplementation.saveDocToFile: failed\n"
      );
  }

  /*============================================================================
  // Clean up.
  //==========================================================================*/

  xmlFreeDoc( p_doc );
  xmlCleanupParser();

}

/*##############################################################################
// Libnucnet__Nuc__extractSubset().
//############################################################################*/

Libnucnet__Nuc *
Libnucnet__Nuc__extractSubset(
  const Libnucnet__Nuc *self,
  const char *s_xpath_suffix
)
{

  xmlDocPtr p_doc;
  Libnucnet__Nuc *p_nuc;

  /*============================================================================
  // Check input.
  //==========================================================================*/

  if( !self )
    LIBNUCNET__NUC__ERROR( "Invalid input" );

  /*============================================================================
  // Create new structure.
  //==========================================================================*/

  p_nuc = Libnucnet__Nuc__new();

  /*============================================================================
  // Get xml document.
  //==========================================================================*/

  p_doc = Libnucnet__Nuc__makeXmlDocument( self );

  /*============================================================================
  // Get structure from xml document.
  //==========================================================================*/

  Libnucnet__Nuc__updateFromXmlDocument( p_nuc, p_doc, s_xpath_suffix );
  
  /*============================================================================
  // Free document.
  //==========================================================================*/
  
  xmlFreeDoc( p_doc );
  xmlCleanupParser();

  /*============================================================================
  // Done.  Return.
  //==========================================================================*/

  return p_nuc;

}
  
/*##############################################################################
// Libnucnet__Species__updateMassExcess().
//############################################################################*/

void
Libnucnet__Species__updateMassExcess(
  Libnucnet__Species *self,
  double d_new_mass_excess
)
{

  if( !self) LIBNUCNET__NUC__ERROR( "Invalid input" );

  self->dMassExcess = d_new_mass_excess;

}

/*##############################################################################
// Libnucnet__Species__updateSpin().
//############################################################################*/

void
Libnucnet__Species__updateSpin(
  Libnucnet__Species *self,
  double d_new_spin
)
{

  if( !self) LIBNUCNET__NUC__ERROR( "Invalid input" );

  self->dSpin = d_new_spin;

}

/*##############################################################################
// Libnucnet__Species__updateSource().
//############################################################################*/

void
Libnucnet__Species__updateSource(
  Libnucnet__Species *self,
  const char *s_new_source
)
{

  if( !self) LIBNUCNET__NUC__ERROR( "Invalid input" );

  if( self->sxSource ) xmlFree( self->sxSource );

  self->sxSource = xmlCharStrdup( s_new_source );

  if( !self->sxSource )
    LIBNUCNET__NUC__ERROR( "Couldn't allocate memory for source string" );

}

/*##############################################################################
// Libnucnet__Species__updatePartitionFunctionData().
//############################################################################*/

void
Libnucnet__Species__updatePartitionFunctionData(
  Libnucnet__Species *self,
  gsl_vector *p_new_t9_array,
  gsl_vector *p_new_log10_partf_array
)
{

  size_t i;
  gsl_permutation *p_perm;

  /*============================================================================
  // Check input.
  //==========================================================================*/

  if( !self )
    LIBNUCNET__NUC__ERROR( "Invalid input" );

  if( !p_new_t9_array && !p_new_log10_partf_array ) return;

  if( p_new_t9_array->size != p_new_log10_partf_array->size )
    LIBNUCNET__NUC__ERROR( "Input arrays not same size" );

  /*============================================================================
  // If old data exist, free them.
  //==========================================================================*/

  if( self->pT9 ) gsl_vector_free( self->pT9 );
  if( self->pLog10Partf ) gsl_vector_free( self->pLog10Partf );

  /*============================================================================
  // Allocate new memory.
  //==========================================================================*/

  self->pT9 = gsl_vector_alloc( p_new_t9_array->size );
  if( !self->pT9 )
    LIBNUCNET__NUC__ERROR( "Couldn't allocate memory for T9 array.\n" );

  self->pLog10Partf = gsl_vector_alloc( p_new_log10_partf_array->size );
  if( !self->pLog10Partf )
    LIBNUCNET__NUC__ERROR( "Couldn't allocate memory for Log10 Partf array" );

  /*============================================================================
  // Sort t9 array.
  //==========================================================================*/

  p_perm = gsl_permutation_alloc( p_new_t9_array->size );

  gsl_sort_vector_index( p_perm, p_new_t9_array );

  /*============================================================================
  // Assign vectors.
  //==========================================================================*/

  for( i = 0; i < p_new_t9_array->size; i++ )
  {
    gsl_vector_set(
      self->pT9,
      i,
      p_new_t9_array->data[p_perm->data[i]]
    );
    gsl_vector_set(
      self->pLog10Partf,
      i,
      p_new_log10_partf_array->data[p_perm->data[i]]
    );
  }

  /*============================================================================
  // Clean up.
  //==========================================================================*/

  gsl_permutation_free( p_perm );

}

/*##############################################################################
// Libnucnet__Species__getPartitionFunctionT9().
//############################################################################*/

gsl_vector *
Libnucnet__Species__getPartitionFunctionT9( const Libnucnet__Species *self )
{

  if( !self )
    LIBNUCNET__NUC__ERROR( "Invalid input" );

  return self->pT9;

}

/*##############################################################################
// Libnucnet__Species__getPartitionFunctionLog10().
//############################################################################*/

gsl_vector *
Libnucnet__Species__getPartitionFunctionLog10( const Libnucnet__Species *self )
{

  if( !self )
    LIBNUCNET__NUC__ERROR( "Invalid input" );

  return self->pLog10Partf;

}

/*##############################################################################
// Libnucnet__Species__copy().
//############################################################################*/

Libnucnet__Species *
Libnucnet__Species__copy( const Libnucnet__Species *self )
{

  Libnucnet__Species *p_copy;

  if( !self )
    LIBNUCNET__NUC__ERROR( "Invalid input" );

  p_copy = ( Libnucnet__Species * ) malloc( sizeof( Libnucnet__Species ) );

  if( !p_copy )
    LIBNUCNET__NUC__ERROR( "Couldn't create new species" );

  p_copy->iZ = self->iZ;
  p_copy->iA = self->iA;
  p_copy->dMassExcess = self->dMassExcess;
  p_copy->dSpin = self->dSpin;
  p_copy->sxName = xmlStrdup( self->sxName );
  p_copy->sxBaseName = xmlStrdup( self->sxBaseName );
  p_copy->sxState = xmlStrdup( self->sxState );
  p_copy->pT9 = NULL;
  p_copy->pLog10Partf = NULL;

  if( !p_copy->sxName )
    LIBNUCNET__NUC__ERROR( "Couldn't create species names" ); 

  if( self->sxSource )
  {
    p_copy->sxSource = xmlStrdup( self->sxSource );
    if( !p_copy->sxSource )
      LIBNUCNET__NUC__ERROR( "Couldn't create species source string" );
  }

  if( self->pT9 )
  {
    p_copy->pT9 =
      gsl_vector_alloc( WnMatrix__get_gsl_vector_size( self->pT9 ) );
 
    if( !p_copy->pT9 )
      LIBNUCNET__NUC__ERROR( "Couldn't allocate partf vector" );

    gsl_vector_memcpy(
      p_copy->pT9,
      Libnucnet__Species__getPartitionFunctionT9( self )
    );
  }

  if( self->pLog10Partf )
  {
    p_copy->pLog10Partf =
      gsl_vector_alloc( WnMatrix__get_gsl_vector_size( self->pLog10Partf ) );
 
    if( !p_copy->pLog10Partf )
      LIBNUCNET__NUC__ERROR( "Couldn't allocate partf vector" );

    gsl_vector_memcpy(
      p_copy->pLog10Partf,
      Libnucnet__Species__getPartitionFunctionLog10( self )
    );
  }

  return p_copy;

}

/*##############################################################################
// Libnucnet__Nuc__createSpeciesStateList().
//############################################################################*/

xmlListPtr
Libnucnet__Nuc__createSpeciesStateList(
  const Libnucnet__Nuc *self,
  xmlChar *sxBaseName
)
{

  typedef struct {
    xmlListPtr pList;
    xmlChar *sxBaseName;
  } user_data;

  xmlListPtr p_list;

  user_data *p_user_data = ( user_data * ) malloc( sizeof( user_data ) );

  if( !p_user_data ) LIBNUCNET__NUC__ERROR( "Couldn't create data structure" );

  p_user_data->pList =
    xmlListCreate(
      NULL,
      (xmlListDataCompare) self->pfSpeciesCompare
    );

  p_user_data->sxBaseName = sxBaseName;

  xmlHashScan(
    self->pSpeciesHash,
    (xmlHashScanner)
      Libnucnet__Nuc__createSpeciesStateListCallback,
    p_user_data
  );

  p_list = p_user_data->pList;

  free( p_user_data );

  return p_list;

}

/*##############################################################################
// Libnucnet__Nuc__createSpeciesStateListCallback().
//############################################################################*/

void
Libnucnet__Nuc__createSpeciesStateListCallback(
  Libnucnet__Species *p_species,
  void *p_data,
  xmlChar *sx_name
)
{

  typedef struct {
    xmlListPtr pList;
    xmlChar *sxBaseName;
  } user_data;

  user_data *p_user_data = ( user_data * ) p_data;

  if( !sx_name ) LIBNUCNET__NUC__ERROR( "Invalid input" );

  if( xmlStrcmp( p_species->sxBaseName, p_user_data->sxBaseName ) == 0 )
    xmlListInsert( p_user_data->pList, p_species );

}

/*##############################################################################
// Libnucnet__Species__removeStates().
//############################################################################*/

int
Libnucnet__Species__removeStates(
  Libnucnet__Species *self,
  Libnucnet__Nuc *p_nuc
)
{

  return
    Libnucnet__Nuc__removeSpecies( p_nuc, self );

}

/*##############################################################################
// Libnucnet__Nuc__createSpeciesBaseNameList().
//############################################################################*/

xmlListPtr
Libnucnet__Nuc__createSpeciesBaseNameList(
  const Libnucnet__Nuc *self
)
{

  typedef struct {
    xmlListPtr pList;
    xmlHashTablePtr pHash;
  } user_data;

  xmlListPtr p_list;

  user_data *p_user_data = ( user_data * ) malloc( sizeof( user_data ) );

  if( !p_user_data ) LIBNUCNET__NUC__ERROR( "Couldn't create data structure" );

  p_user_data->pList = xmlListCreate( NULL, NULL );
  p_user_data->pHash = xmlHashCreate( 0 );

  Libnucnet__Nuc__iterateSpecies(
    self,
    (Libnucnet__Species__iterateFunction)
      Libnucnet__Nuc__createSpeciesBaseNameListIterator,
    p_user_data
  );

  p_list = p_user_data->pList;

  xmlHashFree( p_user_data->pHash, NULL );

  free( p_user_data );

  return p_list;

}

/*##############################################################################
// Libnucnet__Nuc__createSpeciesBaseNameListIterator().
//############################################################################*/

int
Libnucnet__Nuc__createSpeciesBaseNameListIterator(
  Libnucnet__Species *p_species,
  void *p_data
)
{

  typedef struct {
    xmlListPtr pList;
    xmlHashTablePtr pHash;
  } user_data;

  user_data *p_user_data = ( user_data * ) p_data;

  if( xmlHashLookup( p_user_data->pHash, p_species->sxBaseName ) )
    return 1;

  xmlListPushBack( p_user_data->pList, p_species->sxBaseName );

  xmlHashAddEntry(
    p_user_data->pHash,
    p_species->sxBaseName,
    p_species
  );

  return 1;

}

/*##############################################################################
// Libnucnet__Nuc__createSpeciesArray().
//############################################################################*/

Libnucnet__Species **
Libnucnet__Nuc__createSpeciesArray(
  const Libnucnet__Nuc *self
)
{

  typedef struct {
    size_t iIndex;
    Libnucnet__Species **pSpeciesArray;
  } user_data;
  
  Libnucnet__Species **p_species_array;

  user_data *p_user_data;

  if( !self )
    LIBNUCNET__NUC__ERROR( "Invalid input" );

  p_user_data =
    ( user_data * )
    malloc( sizeof( user_data ) );

  if( !p_user_data ) LIBNUCNET__NUC__ERROR( "Couldn't allocate memory" );

  p_user_data->pSpeciesArray =
    ( Libnucnet__Species ** )
    malloc(
      sizeof( Libnucnet__Species * ) *
      Libnucnet__Nuc__getNumberOfSpecies( self )
    );

  if( !p_user_data->pSpeciesArray )
    LIBNUCNET__NUC__ERROR( "Couldn't allocate memory" );

  p_user_data->iIndex = 0;

  xmlHashScan(
    self->pSpeciesHash,
    (xmlHashScanner) Libnucnet__Nuc__createSpeciesArrayCallback,
    p_user_data
  );

  p_species_array = p_user_data->pSpeciesArray;

  free( p_user_data );

  Libnucnet__Nuc__sortSpeciesArray(
    self,
    p_species_array,
    (Libnucnet__Species__compare_function)
      Libnucnet__Species__index_compare_function
  );
        
  return p_species_array;

}
  
/*##############################################################################
// Libnucnet__Nuc__createSpeciesArrayCallback().
//############################################################################*/

void
Libnucnet__Nuc__createSpeciesArrayCallback(
  Libnucnet__Species *p_species,
  void *p_data,
  const xmlChar *sx_species
)
{

  typedef struct {
    size_t iIndex;
    Libnucnet__Species **pSpeciesArray;
  } user_data;

  user_data *p_user_data = ( user_data * ) p_data;
  
  if( !sx_species )
    LIBNUCNET__NUC__ERROR( "No such species" );

  p_user_data->pSpeciesArray[p_user_data->iIndex++] = p_species;

}

/*##############################################################################
// Libnucnet__Nuc__sortSpecies()
//############################################################################*/

void
Libnucnet__Nuc__sortSpecies(
  Libnucnet__Nuc *self
)
{

  Libnucnet__Species **p_species_array;
  size_t i;

  if( !self ) LIBNUCNET__NUC__ERROR( "Invalid input" );

  p_species_array =
    Libnucnet__Nuc__createSpeciesArray( self );

  Libnucnet__Nuc__sortSpeciesArray(
    self,
    p_species_array,
    (Libnucnet__Species__compare_function) self->pfSpeciesCompare
  );

  for( i = 0; i < Libnucnet__Nuc__getNumberOfSpecies( self ); i++ )
    p_species_array[i]->iIndex = i;

  free( p_species_array );

}

/*##############################################################################
// Libnucnet__Nuc__sortSpeciesArray()
//############################################################################*/

void
Libnucnet__Nuc__sortSpeciesArray(
  const Libnucnet__Nuc *self,
  Libnucnet__Species **p_species_array,
  Libnucnet__Species__compare_function pf_func
)
{

  typedef struct {
    Libnucnet__Species *pSpecies;
    Libnucnet__Species__compare_function pfFunc;
  } user_data;

  user_data **p_user_data;
  size_t i;

  if( !self ) LIBNUCNET__NUC__ERROR( "Invalid input" );

  p_user_data =
    ( user_data ** )
    malloc(
      sizeof( user_data * ) *
      Libnucnet__Nuc__getNumberOfSpecies( self )
    );

  if( !p_user_data ) LIBNUCNET__NUC__ERROR( "Couldn't allocate memory" );

  for( i = 0; i < Libnucnet__Nuc__getNumberOfSpecies( self ); i++ )
  {
    p_user_data[i] =
      ( user_data * ) malloc( sizeof( user_data ) );
    p_user_data[i]->pSpecies = p_species_array[i];
    p_user_data[i]->pfFunc = pf_func;
  }

  qsort(
    p_user_data,
    Libnucnet__Nuc__getNumberOfSpecies( self ),
    sizeof( user_data * ),
    Libnucnet__Nuc__sort_helper
  );

  for( i = 0; i < Libnucnet__Nuc__getNumberOfSpecies( self ); i++ )
    p_species_array[i] = p_user_data[i]->pSpecies;

  for( i = 0; i < Libnucnet__Nuc__getNumberOfSpecies( self ); i++ )
    free( p_user_data[i] );

  free( p_user_data );

}

/*##############################################################################
// Libnucnet__Nuc__sort_helper().
//############################################################################*/

int
Libnucnet__Nuc__sort_helper(
  const void *p_1,
  const void *p_2
)
{

  typedef struct {
    Libnucnet__Species *pSpecies;
    Libnucnet__Species__compare_function pfFunc;
  } user_data;

  user_data *p_data1 = *( user_data * const * ) p_1;
  user_data *p_data2 = *( user_data * const * ) p_2;

  return
    p_data1->pfFunc(
      p_data1->pSpecies,
      p_data2->pSpecies
    );

}

/*##############################################################################
// Libnucnet__Nuc__iterateSpecies()
//############################################################################*/

void
Libnucnet__Nuc__iterateSpecies(
  const Libnucnet__Nuc *self,
  Libnucnet__Species__iterateFunction pf_iterator,
  void *p_user_data
)
{

  Libnucnet__Species **p_species_array;
  size_t i, i_species;

  if( !self ) LIBNUCNET__NUC__ERROR( "Invalid input" );

  p_species_array =
    Libnucnet__Nuc__createSpeciesArray( self );

  i_species = Libnucnet__Nuc__getNumberOfSpecies( self );

  for( i = 0; i < i_species; i++ )
    if( pf_iterator( p_species_array[i], p_user_data ) == 0 ) break;

  free( p_species_array );

}

/*##############################################################################
// Libnucnet__NucView__new().
//############################################################################*/

Libnucnet__NucView *
Libnucnet__NucView__new(
  const Libnucnet__Nuc * self,
  const char * s_nuc_xpath
)
{

  xmlDocPtr p_doc;
  Libnucnet__NucView * p_view;

  p_view =
    ( Libnucnet__NucView * ) malloc( sizeof( Libnucnet__NucView ) );

  if( !p_view ) LIBNUCNET__NUC__ERROR( "Couldn't allocate memory for view" );

  if( s_nuc_xpath )
    p_view->sxXPath = xmlCharStrdup( s_nuc_xpath );
  else
    p_view->sxXPath = xmlCharStrdup( NUC_EMPTY_STRING );

  p_view->pNuc = Libnucnet__Nuc__createNewNucForView( self );

  p_doc = Libnucnet__Nuc__makeXmlDocument( self );

  Libnucnet__Nuc__addSpeciesToViewFromXml(
    self,
    p_view->pNuc->pSpeciesHash,
    p_doc,
    s_nuc_xpath
  );

  xmlFreeDoc( p_doc );

  return p_view;

}

/*##############################################################################
// Libnucnet__Nuc__createNewNucForView().
//############################################################################*/

Libnucnet__Nuc *
Libnucnet__Nuc__createNewNucForView(
  const Libnucnet__Nuc *self
)
{

  Libnucnet__Nuc *p_new_nuc;

  if( !self ) LIBNUCNET__NUC__ERROR( "Invalid input nuc structure" );
  
  p_new_nuc = Libnucnet__Nuc__new();

  p_new_nuc->iUpdate = self->iUpdate;
  p_new_nuc->iOwner = 0;
  p_new_nuc->pfSpeciesCompare = self->pfSpeciesCompare;

  return p_new_nuc;

}

/*##############################################################################
// Libnucnet__Nuc__addSpeciesToViewFromXml().
//############################################################################*/

void
Libnucnet__Nuc__addSpeciesToViewFromXml(
  const Libnucnet__Nuc *self,
  xmlHashTablePtr p_hash,
  xmlDocPtr p_doc,
  const char *s_xpath_suffix
)
{

  xmlXPathContextPtr p_xpathCtx;
  xmlXPathObjectPtr p_xpathObj;
  xmlChar *sx_str, *sx_xpath, *sx_xpath_suffix;

  int i;

  /*============================================================================
  // Create context.
  //==========================================================================*/

  p_xpathCtx = xmlXPathNewContext( p_doc );

  /*============================================================================
  // Create main xpath expression.
  //==========================================================================*/

  sx_xpath = xmlCharStrdup( DOUBLE_SLASH );

  if( !sx_xpath ) {
     LIBNUCNET__NUC__ERROR( "Couldn't allocate memory for xpath string" );
  }

  sx_str = xmlCharStrdup( XPATH_NUCLEAR_DATA );
  sx_xpath = xmlStrcat( sx_xpath, sx_str );
  xmlFree( sx_str );

  sx_xpath_suffix = xmlCharStrdup( s_xpath_suffix );

  if( sx_xpath_suffix ) {

     sx_xpath = xmlStrcat( sx_xpath, sx_xpath_suffix );

     if( !sx_xpath ) {
        LIBNUCNET__NUC__ERROR( "Couldn't allocate memory for xpath string" );
     }

     xmlFree( sx_xpath_suffix );

  }

  /*============================================================================
  // Nuclide iteration
  //==========================================================================*/

  p_xpathObj =
    xmlXPathEvalExpression( sx_xpath, p_xpathCtx );

  xmlFree( sx_xpath );

  if( !p_xpathObj ) {
    LIBNUCNET__NUC__ERROR( "Invalid xpath expression" );
  }

  if( !p_xpathObj->nodesetval ) {
    LIBNUCNET__NUC__ERROR( "No nuclear data" );
  }

  for( i = 0; i < p_xpathObj->nodesetval->nodeNr; i++ ) {

    if( Libnucnet__Nuc__addSpeciesToView(
          self,
          p_xpathObj->nodesetval->nodeTab[i],
          p_hash
        ) != 1
    ) {
      LIBNUCNET__NUC__ERROR( "Problem assigning species" );
    }

  }

  xmlXPathFreeObject( p_xpathObj );
  xmlXPathFreeContext( p_xpathCtx );

}

/*##############################################################################
// Libnucnet__Nuc__addSpeciesToView().
//############################################################################*/

int
Libnucnet__Nuc__addSpeciesToView(
  const Libnucnet__Nuc *self,
  xmlNode *p_nuclide,
  xmlHashTablePtr p_hash
) {

  xmlXPathContextPtr p_xpathCtx_nuclide;
  xmlXPathObjectPtr p_xpathObj_data;
  xmlChar *sx_str, *sx_data;
  unsigned int i_z, i_a;
  Libnucnet__Species * p_species;
  int i;

  /*============================================================================
  // Get nuclide context.
  //==========================================================================*/

  p_xpathCtx_nuclide = xmlXPathNewContext( p_nuclide->doc );
  p_xpathCtx_nuclide->node = p_nuclide;

  /*============================================================================
  // Get Z
  //==========================================================================*/

  sx_str = xmlCharStrdup( ATOMIC_NUMBER );
  p_xpathObj_data =
    xmlXPathEvalExpression( sx_str, p_xpathCtx_nuclide );
  xmlFree( sx_str );

  if( !p_xpathObj_data ) {
    LIBNUCNET__NUC__ERROR( "Invalid xpath expression" );
  }

  if( p_xpathObj_data->nodesetval->nodeNr == 0 ) {
    LIBNUCNET__NUC__ERROR( "mass not found" );
  }

  sx_data = xmlNodeGetContent( p_xpathObj_data->nodesetval->nodeTab[0] );
  i_z = (unsigned int) xmlXPathCastStringToNumber( sx_data );
  xmlXPathFreeObject( p_xpathObj_data );
  xmlFree( sx_data );

  /*============================================================================
  // Get A
  //==========================================================================*/

  sx_str = xmlCharStrdup( MASS_NUMBER );
  p_xpathObj_data =
    xmlXPathEvalExpression( sx_str, p_xpathCtx_nuclide );
  xmlFree( sx_str );

  if( !p_xpathObj_data ) {
    LIBNUCNET__NUC__ERROR( "Invalid xpath expression" );
  }

  if( p_xpathObj_data->nodesetval->nodeNr == 0 ) {
    LIBNUCNET__NUC__ERROR( "mass not found" );
  }

  sx_data = xmlNodeGetContent( p_xpathObj_data->nodesetval->nodeTab[0] );
  i_a = (unsigned int) xmlXPathCastStringToNumber( sx_data );
  xmlXPathFreeObject( p_xpathObj_data );
  xmlFree( sx_data );

  /*============================================================================
  // Get State Data
  //==========================================================================*/

  sx_str = xmlCharStrdup( XPATH_STATE );
  p_xpathObj_data =
    xmlXPathEvalExpression( sx_str, p_xpathCtx_nuclide );
  xmlFree( sx_str );

  if( !p_xpathObj_data ) {
    LIBNUCNET__NUC__ERROR( "Invalid xpath expression" );
  }

  if( p_xpathObj_data->nodesetval->nodeNr == 0 ) {
  
    p_species = 
      Libnucnet__Nuc__getSpeciesByZA(
        self,
        i_z,
        i_a,
        NULL
      );

    xmlHashAddEntry(
      p_hash,
      p_species->sxName,
      p_species
    );      

    xmlXPathFreeObject( p_xpathObj_data );
    xmlXPathFreeContext( p_xpathCtx_nuclide );
    return 1;

  }

  sx_str = xmlCharStrdup( ID );

  for( i = 0; i < p_xpathObj_data->nodesetval->nodeNr; i++ )
  {

    sx_data = xmlGetProp( p_xpathObj_data->nodesetval->nodeTab[i], sx_str );

    p_species =
      Libnucnet__Nuc__getSpeciesByZA(
        self,
        i_z,
        i_a,
        (const char *) sx_data
      );

    xmlHashAddEntry(
      p_hash,
      p_species->sxName,
      p_species
    );      

    if( sx_data ) xmlFree( sx_data );

  }

  xmlFree( sx_str );

  /*============================================================================
  // Clean up.
  //==========================================================================*/

  xmlXPathFreeObject( p_xpathObj_data );
  xmlXPathFreeContext( p_xpathCtx_nuclide );
 
  return 1;

}

/*##############################################################################
// Libnucnet__NucView__free().
//############################################################################*/

void
Libnucnet__NucView__free(
  Libnucnet__NucView * self
)
{

  if( !self ) return;

  Libnucnet__Nuc__free( self->pNuc );
  xmlFree( self->sxXPath );

  free( self );

}

/*##############################################################################
// Libnucnet__NucView__getNuc().
//############################################################################*/

Libnucnet__Nuc *
Libnucnet__NucView__getNuc(
  Libnucnet__NucView * self
)
{

  if( !self ) LIBNUCNET__NUC__ERROR( "Invalid view" );

  return self->pNuc;

}

/*##############################################################################
// Libnucnet__NucView__getXPath().
//############################################################################*/

const char *
Libnucnet__NucView__getXPath(
  Libnucnet__NucView * self
)
{

  if( !self ) LIBNUCNET__NUC__ERROR( "Invalid view" );

  return (const char *) self->sxXPath;

}

/*##############################################################################
// Libnucnet__Nuc__getLargestNucleonNumber().
//############################################################################*/

unsigned int
Libnucnet__Nuc__getLargestNucleonNumber(
  Libnucnet__Nuc * self,
  const char * s_nucleon_type
)
{

  typedef struct {
    unsigned int iMax;
    xmlChar * sxNucleonType;
  } work;

  work * p_work;
  unsigned int i_result;

  p_work = ( work * ) malloc( sizeof( work ) );

  if( !p_work ) LIBNUCNET__NUC__ERROR( "Couldn't allocate memory" );

  p_work->iMax = 0;
  p_work->sxNucleonType = xmlCharStrdup( s_nucleon_type );

  xmlHashScan(
    self->pSpeciesHash,
    (xmlHashScanner) Libnucnet__Nuc__getLargestNucleonNumberHelper,
    p_work
  );

  i_result = (unsigned int) p_work->iMax;

  xmlFree( p_work->sxNucleonType );
  free( p_work );

  return i_result;

}

/*##############################################################################
// Libnucnet__Nuc__getLargestNucleonNumberIterator().
//############################################################################*/

void
Libnucnet__Nuc__getLargestNucleonNumberHelper(
  Libnucnet__Species * p_species,
  void * p_data,
  xmlChar * sx_name
)
{

  typedef struct {
    unsigned int iMax;
    xmlChar * sxNucleonType;
  } work;

  unsigned int i_test;
  work * p_work = ( work * ) p_data;

  if( !sx_name ) LIBNUCNET__NUC__ERROR( "Invalid species" );

  if( strcmp( (const char *) p_work->sxNucleonType, ATOMIC_NUMBER ) == 0 )
    i_test = Libnucnet__Species__getZ( p_species );
  else if( strcmp( (const char *) p_work->sxNucleonType, MASS_NUMBER ) == 0 )
    i_test = Libnucnet__Species__getA( p_species );
  else if( strcmp( (const char *) p_work->sxNucleonType, NEUTRON_NUMBER ) == 0 )
  {
    i_test = Libnucnet__Species__getA( p_species ) -
       Libnucnet__Species__getZ( p_species ); 
  }
  else
    LIBNUCNET__NUC__ERROR( "Invalid nucleon type" );

  if( i_test > p_work->iMax ) p_work->iMax = i_test;

}
  
