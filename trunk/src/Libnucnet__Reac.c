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
//        Source code for the Libnucnet__Reac part of the libnucnet module.
//        For documentation, see the header file Libnucnet__Reac.h.
//     </abstract>
//   </description>
// </file>
//////////////////////////////////////////////////////////////////////////////*/

/*##############################################################################
// Standard library #include's.
//############################################################################*/

#include "Libnucnet__Reac.h"

/*##############################################################################
// Libnucnet__Reac__new().
//############################################################################*/

Libnucnet__Reac *Libnucnet__Reac__new( ) {

  Libnucnet__Reac *self;

  /*============================================================================
  // Create reaction structure.
  //==========================================================================*/

  if(
     !( self = ( Libnucnet__Reac * ) malloc( sizeof( Libnucnet__Reac ) ) )
  )
    LIBNUCNET__REAC__ERROR( "Virtual memory exhausted" );

  /*============================================================================
  // Initialize Libnucnet__Reac quantities.
  //==========================================================================*/

  self->pReactionHash = xmlHashCreate( 0 );
  self->pFDHash = xmlHashCreate( 0 );

  self->iUpdate = 1;
  self->iOwner = 1;

  self->pfReactionCompare = NULL;

  /*============================================================================
  // Set standard reactions.
  //==========================================================================*/

  Libnucnet__Reac__registerRateFunction(
    self,
    SINGLE_RATE_STRING,
    (Libnucnet__Reaction__rateFunction) SINGLE_RATE_FUNCTION
  );

  Libnucnet__Reac__registerRateFunction(
    self,
    NON_SMOKER_STRING,
    (Libnucnet__Reaction__rateFunction) NON_SMOKER_FUNCTION
  );

  Libnucnet__Reac__registerRateFunction(
    self,
    RATE_TABLE_STRING,
    (Libnucnet__Reaction__rateFunction) RATE_TABLE_FUNCTION
  );

  /*============================================================================
  // Return.
  //==========================================================================*/

  return self;

}

/*##############################################################################
// Libnucnet__Reac__new_from_xml().
//############################################################################*/

Libnucnet__Reac *Libnucnet__Reac__new_from_xml( 
  const char *s_xml_filename, const char *s_xpath_suffix
) {

  xmlDocPtr p_doc;
  Libnucnet__Reac *self;

  /*============================================================================
  // Create document.
  //==========================================================================*/

  p_doc = xmlParseFile( s_xml_filename );

  if ( p_doc == NULL ) {

    LIBNUCNET__REAC__ERROR( "Could not create document" );
    
  }

  /*============================================================================
  // Get Libnucnet__Reac structure.
  //==========================================================================*/

  self = Libnucnet__Reac__new();

  /*============================================================================
  // Update data from xml document.
  //==========================================================================*/

  Libnucnet__Reac__updateFromXmlDocument( self, p_doc, s_xpath_suffix );

  /*============================================================================
  // Clean up and return.
  //==========================================================================*/

  xmlFreeDoc( p_doc );
  xmlCleanupParser();

  return self;

}

/*##############################################################################
// Libnucnet__Reac__updateFromXmlDocument().
//############################################################################*/

void
Libnucnet__Reac__updateFromXmlDocument(
  Libnucnet__Reac *self, xmlDocPtr p_doc, const char *s_xpath_suffix
)
{

  xmlXPathContextPtr p_xpathCtx = NULL;
  xmlXPathObjectPtr p_xpathObj = NULL;
  xmlChar *sx_xpath, *sx_xpath_suffix;
  int i;

  /*============================================================================
  // Reaction iteration
  //==========================================================================*/

  sx_xpath = xmlCharStrdup( XPATH_REAC );

  if( !sx_xpath ) {
     LIBNUCNET__REAC__ERROR( "Couldn't allocate memory for xpath string" );
  }

  if( s_xpath_suffix ) {

     sx_xpath_suffix = xmlCharStrdup( s_xpath_suffix );

     sx_xpath = xmlStrcat( sx_xpath, sx_xpath_suffix );
  
     if( !sx_xpath ) {
        LIBNUCNET__REAC__ERROR( "Couldn't allocate memory for xpath string" );
     }

     xmlFree( sx_xpath_suffix );

  }

  p_xpathCtx = xmlXPathNewContext( p_doc );
  p_xpathObj = xmlXPathEvalExpression( sx_xpath, p_xpathCtx );
  xmlFree( sx_xpath );

  if( !p_xpathObj ) {
    LIBNUCNET__REAC__ERROR( "Invalid xpath expression" );
  }

  if( !xmlXPathNodeSetIsEmpty( p_xpathObj->nodesetval ) ) {

    for( i = 0; i < p_xpathObj->nodesetval->nodeNr; i++ ) {

      if( 
          ( Libnucnet__Reac__assignReaction(
               self, p_xpathObj->nodesetval->nodeTab[i]
            )
          ) != 1
      ) {

        LIBNUCNET__REAC__ERROR( "Problem assigning reaction" );

      }

    }

  }

  /*============================================================================
  // Clean up
  //==========================================================================*/

  xmlXPathFreeObject( p_xpathObj );
  xmlXPathFreeContext( p_xpathCtx );

}

/*##############################################################################
// Libnucnet__Reac__updateFromXml().
//############################################################################*/

void
Libnucnet__Reac__updateFromXml( 
  Libnucnet__Reac *self, const char *s_xml_filename, const char *s_xpath_suffix
) {

  xmlDocPtr p_doc;

  /*============================================================================
  // Create document.
  //==========================================================================*/

  p_doc = xmlParseFile( s_xml_filename );

  if ( p_doc == NULL ) {

    LIBNUCNET__REAC__ERROR( "Could not create document" );
    
  }

  /*============================================================================
  // Update from xml document.
  //==========================================================================*/

  Libnucnet__Reac__updateFromXmlDocument( self, p_doc, s_xpath_suffix );

  /*============================================================================
  // Clean up.
  //==========================================================================*/

  xmlFreeDoc( p_doc );
  xmlCleanupParser();

}

/*##############################################################################
// Libnucnet__Reac__assignReaction().
//############################################################################*/

int
Libnucnet__Reac__assignReaction(
  Libnucnet__Reac *self, xmlNode *p_reaction_node
) {

  Libnucnet__Reaction *p_reaction;

  /*============================================================================
  // Create reaction.
  //==========================================================================*/

  p_reaction = Libnucnet__Reac__new_reaction_from_xml_node( p_reaction_node );

  /*============================================================================
  // Get rate data structure.
  //==========================================================================*/

  Libnucnet__Reaction__addRateDataFromXml( p_reaction, p_reaction_node );

  if( !p_reaction->pRd ) return 0;

  /*============================================================================
  // Add or update reaction.
  //==========================================================================*/

  if( !Libnucnet__Reac__updateReaction( self, p_reaction ) )
    LIBNUCNET__REAC__ERROR( "Couldn't update reaction!" );

  /*============================================================================
  // Return.
  //==========================================================================*/

  return 1;

}

/*##############################################################################
// Libnucnet__Reac__new_reaction_from_xml_node().
//############################################################################*/

Libnucnet__Reaction *
Libnucnet__Reac__new_reaction_from_xml_node(
  xmlNode *p_reaction_node
)
{

  xmlXPathContextPtr p_xpathCtx_reaction;
  xmlXPathObjectPtr p_xpathObj_data;
  int i;
  xmlChar *sx_str;
  Libnucnet__Reaction *p_reaction;

  /*============================================================================
  // Create reaction.
  //==========================================================================*/

  p_reaction = Libnucnet__Reaction__new();

  /*============================================================================
  // Get context.
  //==========================================================================*/

  p_xpathCtx_reaction = xmlXPathNewContext( p_reaction_node->doc );
  p_xpathCtx_reaction->node = p_reaction_node;

  /*============================================================================
  // Get source data.
  //==========================================================================*/

  sx_str = xmlCharStrdup( REACTION_SOURCE );
  p_xpathObj_data =
    xmlXPathEvalExpression( sx_str, p_xpathCtx_reaction );
  xmlFree( sx_str );

  if( !p_xpathObj_data ) {
    LIBNUCNET__REAC__ERROR( "Invalid xpath expression" );
  }

  if( p_xpathObj_data->nodesetval->nodeNr == 0 ) {
    p_reaction->sxSource = xmlCharStrdup( REAC_EMPTY_STRING );
  } else if ( p_xpathObj_data->nodesetval->nodeNr == 1 ) {
    p_reaction->sxSource =
      xmlNodeGetContent( p_xpathObj_data->nodesetval->nodeTab[0] );
  } else {
    LIBNUCNET__REAC__ERROR( "Too many instances of source" );
  }

  xmlXPathFreeObject( p_xpathObj_data );
    
  /*============================================================================
  // Get reactants xpath.
  //==========================================================================*/

  sx_str = xmlCharStrdup( REACTANT );
  p_xpathObj_data =
    xmlXPathEvalExpression( sx_str, p_xpathCtx_reaction );
  xmlFree( sx_str );

  if( !p_xpathObj_data ) {
    LIBNUCNET__REAC__ERROR( "Invalid xpath expression" );
  }

  /*============================================================================
  // Get reactants.
  //==========================================================================*/

  for( i = 0; i < p_xpathObj_data->nodesetval->nodeNr; i++ )
  {
    sx_str = xmlNodeGetContent( p_xpathObj_data->nodesetval->nodeTab[i] );
    Libnucnet__Reaction__addReactant( p_reaction, (const char *) sx_str );
    xmlFree( sx_str );
  }

  xmlXPathFreeObject( p_xpathObj_data );

  /*============================================================================
  // Get products xpath
  //==========================================================================*/

  sx_str = xmlCharStrdup( PRODUCT );
  p_xpathObj_data =
    xmlXPathEvalExpression( sx_str, p_xpathCtx_reaction );
  xmlFree( sx_str );

  if( !p_xpathObj_data ) {
    LIBNUCNET__REAC__ERROR( "Invalid xpath expression" );
  }

  /*============================================================================
  // Get products. 
  //==========================================================================*/

  for( i = 0; i < p_xpathObj_data->nodesetval->nodeNr; i++ ) {

    sx_str = xmlNodeGetContent( p_xpathObj_data->nodesetval->nodeTab[i] );
    Libnucnet__Reaction__addProduct( p_reaction, (char *) sx_str );
    xmlFree( sx_str );

  }

  xmlXPathFreeObject( p_xpathObj_data );
  xmlXPathFreeContext( p_xpathCtx_reaction );

  /*============================================================================
  // Return.
  //==========================================================================*/

  return p_reaction;

}

/*##############################################################################
// Libnucnet__Reaction__addRateDataFromXml().
//############################################################################*/

void
Libnucnet__Reaction__addRateDataFromXml(
  Libnucnet__Reaction *self,
  xmlNode *p_reaction_node
) {

  xmlXPathContextPtr p_xpathCtx_reaction;
  xmlXPathObjectPtr p_xpathObj_data;
  xmlChar *sx_str;

  /*============================================================================
  // Get context.
  //==========================================================================*/

  p_xpathCtx_reaction = xmlXPathNewContext( p_reaction_node->doc );
  p_xpathCtx_reaction->node = p_reaction_node;

  /*============================================================================
  // Assign rate table, if it exists
  //==========================================================================*/

  sx_str = xmlCharStrdup( RATE_TABLE_STRING );
  p_xpathObj_data =
    xmlXPathEvalExpression( sx_str, p_xpathCtx_reaction );
  xmlFree( sx_str );

  if( !p_xpathObj_data ) {
    LIBNUCNET__REAC__ERROR( "Invalid xpath expression" );
  }

  if( p_xpathObj_data->nodesetval->nodeNr == 1 ) {

    Libnucnet__Reaction__updateRateTableFromXml(
      self,
      p_xpathObj_data->nodesetval->nodeTab[0]
    );

    xmlXPathFreeObject( p_xpathObj_data );
    xmlXPathFreeContext( p_xpathCtx_reaction );

    return;

  }
  xmlXPathFreeObject( p_xpathObj_data );

  /*============================================================================
  // Get non smoker, if it exists
  //==========================================================================*/

  sx_str = xmlCharStrdup( NON_SMOKER_STRING );
  p_xpathObj_data =
    xmlXPathEvalExpression( sx_str, p_xpathCtx_reaction );
  xmlFree( sx_str );

  if( !p_xpathObj_data ) {
    LIBNUCNET__REAC__ERROR( "Invalid xpath expression" );
  }

  if( p_xpathObj_data->nodesetval->nodeNr == 1 ) {

    Libnucnet__Reaction__addNonSmokerFromXml(
      self,
      p_xpathObj_data->nodesetval->nodeTab[0]
    ); 

    xmlXPathFreeObject( p_xpathObj_data );
    xmlXPathFreeContext( p_xpathCtx_reaction );

    return;

  } 
  xmlXPathFreeObject( p_xpathObj_data );

  /*============================================================================
  // Get single rate, if it exists
  //==========================================================================*/

  sx_str = xmlCharStrdup( SINGLE_RATE_STRING );
  p_xpathObj_data =
    xmlXPathEvalExpression( sx_str, p_xpathCtx_reaction );
  xmlFree( sx_str );

  if( !p_xpathObj_data ) {
    LIBNUCNET__REAC__ERROR( "Invalid xpath expression" );
  }

  if( p_xpathObj_data->nodesetval->nodeNr == 1 ) {

    sx_str = xmlNodeGetContent( p_xpathObj_data->nodesetval->nodeTab[0] );
    Libnucnet__Reaction__updateSingleRate(
      self,
      xmlXPathCastStringToNumber( sx_str )
    );
    xmlFree( sx_str );

    xmlXPathFreeObject( p_xpathObj_data );
    xmlXPathFreeContext( p_xpathCtx_reaction );

    return;

  }
  xmlXPathFreeObject( p_xpathObj_data );

  /*============================================================================
  // Get user rate, if it exists
  //==========================================================================*/

  sx_str = xmlCharStrdup( USER_RATE_STRING );
  p_xpathObj_data =
    xmlXPathEvalExpression( sx_str, p_xpathCtx_reaction );
  xmlFree( sx_str );

  if( !p_xpathObj_data ) {
    LIBNUCNET__REAC__ERROR( "Invalid xpath expression" );
  }

  if( p_xpathObj_data->nodesetval->nodeNr == 1 ) {

    Libnucnet__Reaction__updateUserRateFromXml(
      self,
      p_xpathObj_data->nodesetval->nodeTab[0]
    ); 

    xmlXPathFreeObject( p_xpathObj_data );
    xmlXPathFreeContext( p_xpathCtx_reaction );

    return;

  }

  xmlXPathFreeObject( p_xpathObj_data );
  xmlXPathFreeContext( p_xpathCtx_reaction );

  return;

}

/*##############################################################################
// Libnucnet__Reaction__RateData__new().
//############################################################################*/

Libnucnet__Reaction__RateData *
Libnucnet__Reaction__RateData__new( void )
{

  Libnucnet__Reaction__RateData *self;

  self =
    ( Libnucnet__Reaction__RateData * )
    malloc( sizeof( Libnucnet__Reaction__RateData ) );
 
  if( !self )
    LIBNUCNET__REAC__ERROR( "Couldn't allocate memory for rate data" );

  self->pSingle = NULL;
  self->pNsfHash = NULL;
  self->pRt = NULL;

  return self;

}

/*##############################################################################
// Libnucnet__Reaction__updateRateTableFromXml().
//############################################################################*/

void
Libnucnet__Reaction__updateRateTableFromXml(
  Libnucnet__Reaction *self,
  xmlNode *p_table
)
{

  xmlXPathContextPtr p_xpathCtx_table;
  xmlXPathObjectPtr p_xpathObj_data;
  xmlChar *sx_data, *sx_str;
  size_t i;
  gsl_vector *p_t9, *p_rate, *p_sef;

  /*============================================================================
  // Get context.
  //==========================================================================*/

  p_xpathCtx_table = xmlXPathNewContext( p_table->doc );
  p_xpathCtx_table->node = p_table;

  /*============================================================================
  // Count entries
  //==========================================================================*/

  sx_str = xmlCharStrdup( RATE_TABLE_T9 );
  p_xpathObj_data =
    xmlXPathEvalExpression( sx_str, p_xpathCtx_table );
  xmlFree( sx_str );
  
  if( !p_xpathObj_data ) {
    LIBNUCNET__REAC__ERROR( "Invalid xpath expression" );
  }

  /*============================================================================
  // Allocate memory.
  //==========================================================================*/

  p_t9 =
    gsl_vector_alloc( (size_t) p_xpathObj_data->nodesetval->nodeNr );
  p_rate =
    gsl_vector_alloc( (size_t) p_xpathObj_data->nodesetval->nodeNr );
  p_sef =
    gsl_vector_alloc( (size_t) p_xpathObj_data->nodesetval->nodeNr );

  /*============================================================================
  // Get t9
  //==========================================================================*/

  for( i = 0; i < p_t9->size; i++ ) {

    sx_data = xmlNodeGetContent( p_xpathObj_data->nodesetval->nodeTab[i] );
    gsl_vector_set(
      p_t9,
      i,
      xmlXPathCastStringToNumber( sx_data )
    );
    xmlFree( sx_data );

  }

  xmlXPathFreeObject( p_xpathObj_data );

  /*============================================================================
  // Get rate
  //==========================================================================*/

  sx_str = xmlCharStrdup( RATE_TABLE_ENTRY );
  p_xpathObj_data =
    xmlXPathEvalExpression( sx_str, p_xpathCtx_table );
  xmlFree( sx_str );

  if( !p_xpathObj_data ) {
    LIBNUCNET__REAC__ERROR( "Invalid xpath expression" );
  }

  for( i = 0; i < p_rate->size; i++ ) {

    sx_data = xmlNodeGetContent( p_xpathObj_data->nodesetval->nodeTab[i] );
    gsl_vector_set(
      p_rate,
      i,
      xmlXPathCastStringToNumber( sx_data )
    );
    xmlFree( sx_data );

  }

  xmlXPathFreeObject( p_xpathObj_data );

  /*============================================================================
  // Get sef
  //==========================================================================*/

  gsl_vector_set_all( p_sef, 1. );

  sx_str = xmlCharStrdup( RATE_TABLE_SEF );
  p_xpathObj_data =
    xmlXPathEvalExpression( sx_str, p_xpathCtx_table );
  xmlFree( sx_str );

  if( !p_xpathObj_data ) {
    LIBNUCNET__REAC__ERROR( "Invalid xpath expression" );
  }

  for( i = 0; i < p_sef->size; i++ ) {

    sx_data = xmlNodeGetContent( p_xpathObj_data->nodesetval->nodeTab[i] );
    gsl_vector_set(
      p_sef,
      i,
      xmlXPathCastStringToNumber( sx_data )
    );
    xmlFree( sx_data );

  }

  /*============================================================================
  // Free xpath.
  //==========================================================================*/

  xmlXPathFreeObject( p_xpathObj_data );
  xmlXPathFreeContext( p_xpathCtx_table );

  /*============================================================================
  // Add table to reaction.
  //==========================================================================*/

  Libnucnet__Reaction__updateRateTable(
    self,
    p_t9,
    p_rate,
    p_sef
  );

  /*============================================================================
  // Free vectors.
  //==========================================================================*/

  gsl_vector_free( p_t9 );
  gsl_vector_free( p_rate );
  gsl_vector_free( p_sef );

}

/*##############################################################################
// Libnucnet__Reaction__updateUserRateFromXml().
//############################################################################*/

void
Libnucnet__Reaction__updateUserRateFromXml(
  Libnucnet__Reaction *self,
  xmlNode *p_user_rate
)
{

  xmlXPathContextPtr p_xpathCtx_user_rate;
  xmlXPathObjectPtr p_xpathObj_data;
  xmlChar *sx_str, *sx_str2, *sx_name, *sx_tag1, *sx_tag2, *sx_value;
  int i;

  /*============================================================================
  // Get context.
  //==========================================================================*/

  p_xpathCtx_user_rate = xmlXPathNewContext( p_user_rate->doc );
  p_xpathCtx_user_rate->node = p_user_rate;

  /*============================================================================
  // Set user function.
  //==========================================================================*/

  sx_str = xmlCharStrdup( FUNCTION_KEY );
  sx_str2 = xmlGetProp( p_user_rate, sx_str );
  Libnucnet__Reaction__setUserRateFunctionKey(
    self,
    (const char *) sx_str2
  );
  xmlFree( sx_str2 );
  xmlFree( sx_str );
  
  /*============================================================================
  // Get rate properties.
  //==========================================================================*/

  sx_str = xmlCharStrdup( USER_RATE_PROPERTY_XPATH );
  p_xpathObj_data =
    xmlXPathEvalExpression( sx_str, p_xpathCtx_user_rate );
  xmlFree( sx_str );

  if( !p_xpathObj_data ) {
    LIBNUCNET__REAC__ERROR( "Invalid xpath expression" );
  }

  for( i = 0; i < p_xpathObj_data->nodesetval->nodeNr; i++ )
  {

    sx_name =
      xmlGetProp(
        p_xpathObj_data->nodesetval->nodeTab[i],
        (const xmlChar *) PROPERTY_NAME
      );

    sx_tag1 =
      xmlGetProp(
        p_xpathObj_data->nodesetval->nodeTab[i],
        (const xmlChar *) PROPERTY_TAG1
      );

    sx_tag2 =
      xmlGetProp(
        p_xpathObj_data->nodesetval->nodeTab[i],
        (const xmlChar *) PROPERTY_TAG2
      );

    sx_value =
      xmlNodeGetContent( p_xpathObj_data->nodesetval->nodeTab[i] );

    Libnucnet__Reaction__updateUserRateFunctionProperty(
      self,
      (const char *) sx_name,
      (const char *) sx_tag1,
      (const char *) sx_tag2,
      (const char *) sx_value
    );

    xmlFree( sx_name ); 
    xmlFree( sx_tag1 );
    xmlFree( sx_tag2 );
    xmlFree( sx_value );

  }

  /*============================================================================
  // Free xpath.
  //==========================================================================*/

  xmlXPathFreeObject( p_xpathObj_data );
  xmlXPathFreeContext( p_xpathCtx_user_rate );

}

/*##############################################################################
// Libnucnet__Reaction__addNonSmokerFromXml().
//############################################################################*/

void
Libnucnet__Reaction__addNonSmokerFromXml(
  Libnucnet__Reaction *self,
  xmlNode *p_non_smoker
)
{

  xmlXPathContextPtr p_xpathCtx_nsfs;
  xmlXPathObjectPtr p_xpathObj_data;
  xmlChar *sx_str;
  int i;

  /*============================================================================
  // Get context.
  //==========================================================================*/

  p_xpathCtx_nsfs = xmlXPathNewContext( p_non_smoker->doc );
  p_xpathCtx_nsfs->node = p_non_smoker;

  /*============================================================================
  // Get fits.
  //==========================================================================*/

  sx_str = xmlCharStrdup( FIT );
  p_xpathObj_data =
    xmlXPathEvalExpression( sx_str, p_xpathCtx_nsfs );
  xmlFree( sx_str );

  if( !p_xpathObj_data ) {
    LIBNUCNET__REAC__ERROR( "Invalid xpath expression" );
  }

  if( p_xpathObj_data->nodesetval->nodeNr == 0 ) {
    Libnucnet__Reaction__addNonSmokerFitFromXml(
      self,
      p_non_smoker
    );
  }
  else {

    for(
      i = 0;
      i < p_xpathObj_data->nodesetval->nodeNr;
      i++
    ) 
    {

      Libnucnet__Reaction__addNonSmokerFitFromXml(
        self,
        p_xpathObj_data->nodesetval->nodeTab[i]
      );

    }

  }

  /*============================================================================
  // Clean up and return.
  //==========================================================================*/

  xmlXPathFreeObject( p_xpathObj_data );
  xmlXPathFreeContext( p_xpathCtx_nsfs );

  return;

}

/*##############################################################################
// Libnucnet__Reac__addNonSmokeFitFromXml().
//############################################################################*/

void
Libnucnet__Reaction__addNonSmokerFitFromXml(
  Libnucnet__Reaction *self,
  xmlNode *p_non_smoker_fit 
)
{

  xmlXPathContextPtr p_xpathCtx_nsf;
  xmlXPathObjectPtr p_xpathObj_data;
  xmlChar *sx_data, *sx_str, *sx_note = NULL;

  double
    d_spint = 0.,
    d_spinf = 0.,
    d_acc = 0.,
    d_tlowfit = 0.,
    d_thighfit = D_THIGHFIT_DEFAULT,
    d_tlowhf = 0.,
    *a_a;

  int i_count;

  /*============================================================================
  // Get context.
  //==========================================================================*/

  p_xpathCtx_nsf = xmlXPathNewContext( p_non_smoker_fit->doc );
  p_xpathCtx_nsf->node = p_non_smoker_fit;

  /*============================================================================
  // Get note.
  //==========================================================================*/

  sx_str = xmlCharStrdup( NOTE );
  sx_note =
    xmlGetProp( p_non_smoker_fit, sx_str );
  xmlFree( sx_str );

  /*============================================================================
  // Get spint
  //==========================================================================*/

  sx_str = xmlCharStrdup( SPINT );
  p_xpathObj_data =
    xmlXPathEvalExpression( sx_str, p_xpathCtx_nsf );
  xmlFree( sx_str );

  if( !p_xpathObj_data ) {
    LIBNUCNET__REAC__ERROR( "Invalid xpath expression" );
  }

  if( p_xpathObj_data->nodesetval->nodeNr == 1 ) {
    sx_data = xmlNodeGetContent( p_xpathObj_data->nodesetval->nodeTab[0] );
    d_spint = xmlXPathCastStringToNumber( sx_data );
    xmlFree( sx_data );
  }

  xmlXPathFreeObject( p_xpathObj_data );

  /*============================================================================
  // Get spinf
  //==========================================================================*/

  sx_str = xmlCharStrdup( SPINF );
  p_xpathObj_data =
    xmlXPathEvalExpression( sx_str, p_xpathCtx_nsf );
  xmlFree( sx_str );

  if( !p_xpathObj_data ) {
    LIBNUCNET__REAC__ERROR( "Invalid xpath expression" );
  }

  if( p_xpathObj_data->nodesetval->nodeNr == 1 ) {
    sx_data = xmlNodeGetContent( p_xpathObj_data->nodesetval->nodeTab[0] );
    d_spinf = xmlXPathCastStringToNumber( sx_data );
    xmlFree( sx_data );
  }

  xmlXPathFreeObject( p_xpathObj_data );

  /*============================================================================
  // Get TlowHf
  //==========================================================================*/

  sx_str = xmlCharStrdup( TLOWHF );
  p_xpathObj_data =
    xmlXPathEvalExpression( sx_str, p_xpathCtx_nsf );
  xmlFree( sx_str );

  if( !p_xpathObj_data ) {
    LIBNUCNET__REAC__ERROR( "Invalid xpath expression" );
  }

  if( p_xpathObj_data->nodesetval->nodeNr == 1 ) {
    sx_data = xmlNodeGetContent( p_xpathObj_data->nodesetval->nodeTab[0] );
    d_tlowhf = xmlXPathCastStringToNumber( sx_data );
    xmlFree( sx_data );
  }

  xmlXPathFreeObject( p_xpathObj_data );

  /*============================================================================
  // Get Tlowfit
  //==========================================================================*/

  sx_str = xmlCharStrdup( TLOWFIT );
  p_xpathObj_data =
    xmlXPathEvalExpression( sx_str, p_xpathCtx_nsf );
  xmlFree( sx_str );

  if( !p_xpathObj_data ) {
    LIBNUCNET__REAC__ERROR( "Invalid xpath expression" );
  }

  sx_data = xmlNodeGetContent( p_xpathObj_data->nodesetval->nodeTab[0] );
  if( !sx_data )
    LIBNUCNET__REAC__ERROR( "No TlowFit data" );
  d_tlowfit = xmlXPathCastStringToNumber( sx_data );
  xmlFree( sx_data );

  xmlXPathFreeObject( p_xpathObj_data );

  /*============================================================================
  // Get Thighfit.  Default set to D_THIGHFIT_DEFAULT.
  //==========================================================================*/

  sx_str = xmlCharStrdup( THIGHFIT );
  p_xpathObj_data =
    xmlXPathEvalExpression( sx_str, p_xpathCtx_nsf );
  xmlFree( sx_str );

  if( !p_xpathObj_data ) {
    LIBNUCNET__REAC__ERROR( "Invalid xpath expression" );
  }

  if( p_xpathObj_data->nodesetval->nodeNr == 1 ) {
    sx_data = xmlNodeGetContent( p_xpathObj_data->nodesetval->nodeTab[0] );
    d_thighfit = xmlXPathCastStringToNumber( sx_data );
    xmlFree( sx_data );
  }

  xmlXPathFreeObject( p_xpathObj_data );

  /*============================================================================
  // Get acc
  //==========================================================================*/

  sx_str = xmlCharStrdup( ACC );
  p_xpathObj_data =
    xmlXPathEvalExpression( sx_str, p_xpathCtx_nsf );
  xmlFree( sx_str );

  if( !p_xpathObj_data ) {
    LIBNUCNET__REAC__ERROR( "Invalid xpath expression" );
  }

  if( p_xpathObj_data->nodesetval->nodeNr == 1 ) {
    sx_data = xmlNodeGetContent( p_xpathObj_data->nodesetval->nodeTab[0] );
    d_acc = xmlXPathCastStringToNumber( sx_data );
    xmlFree( sx_data );
  }

  xmlXPathFreeObject( p_xpathObj_data );

  /*============================================================================
  // Get a1-a8
  //==========================================================================*/

  a_a = ( double * ) calloc( (size_t) I_NSF, sizeof( double ) );

  for ( i_count = 1; i_count <= I_NSF; i_count++ ) { 

    sx_str = (xmlChar *) malloc( sizeof( xmlChar ) * REAC_BUF_SIZE );
    xmlStrPrintf( sx_str, REAC_BUF_SIZE, (const xmlChar *) "a%d", i_count );
    p_xpathObj_data =
      xmlXPathEvalExpression( sx_str, p_xpathCtx_nsf );
    xmlFree( sx_str );

    if( !p_xpathObj_data && i_count != I_NSF ) {
       LIBNUCNET__REAC__ERROR( "Invalid xpath expression" );
    }

    if( p_xpathObj_data ) {
      if( p_xpathObj_data->nodesetval->nodeNr == 1 ) {
        sx_data = xmlNodeGetContent( p_xpathObj_data->nodesetval->nodeTab[0] );
        a_a[i_count - 1] = xmlXPathCastStringToNumber( sx_data );
        xmlFree( sx_data );
      }
      xmlXPathFreeObject( p_xpathObj_data );
    }

  }

  /*============================================================================
  // Clean up xpath context.
  //==========================================================================*/

  xmlXPathFreeContext( p_xpathCtx_nsf );

  /*============================================================================
  // Add fit.
  //==========================================================================*/

  Libnucnet__Reaction__addNonSmokerFit( 
    self,
    (char *) sx_note,
    a_a,
    d_spint,
    d_spinf,
    d_tlowhf, 
    d_tlowfit,
    d_thighfit,
    d_acc
  );

  /*============================================================================
  // Clean up.
  //==========================================================================*/

  xmlFree( sx_note );

}

/*##############################################################################
// Libnucnet__Reaction__computeNonSmokerRate().
//############################################################################*/

double
Libnucnet__Reaction__computeNonSmokerRate(
  Libnucnet__Reaction *self,
  double d_t9,
  void *p_data
)
{

  struct {
    double dRate, dT9;
  } user_data;

  if( p_data ) LIBNUCNET__REAC__ERROR( "No extra data to this routine" );

  user_data.dRate = 0.;
  user_data.dT9 = d_t9;

  xmlHashScan(
    self->pRd->pNsfHash,
    (xmlHashScanner) Libnucnet__Reaction__computeNonSmokerRateCallback,
    &user_data
  );  

  return user_data.dRate;

}

/*##############################################################################
// Libnucnet__Reaction__computeNonSmokerRateCallback().
//############################################################################*/

void
Libnucnet__Reaction__computeNonSmokerRateCallback(
  Libnucnet__Reaction__NonSmokerFit *p_nsf,
  void *p_data,
  xmlChar *sx_note
)
{

  struct user_data {
    double dRate, dT9;
  };
  double d_t9 = 0;

  struct user_data *p_user_data = ( struct user_data * ) p_data;

  if( !sx_note )
    LIBNUCNET__REAC__ERROR( "Invalid Non-Smoker fit" );

  /*============================================================================
  // Check temperature.
  //==========================================================================*/

  if( p_user_data->dT9 < *p_nsf->pTlowfit )
    d_t9 = *p_nsf->pTlowfit;
  else if( p_user_data->dT9 > *p_nsf->pThighfit )
    d_t9 = *p_nsf->pThighfit;
  else
    d_t9 = p_user_data->dT9;

  /*============================================================================
  // Calculate Non_Smoker forward rate
  //==========================================================================*/

  p_user_data->dRate +=
    exp( 
      ( p_nsf->a[0] ) +
      ( p_nsf->a[1] / d_t9 ) +
      ( p_nsf->a[2] / pow( d_t9, ( 1. / 3. ) ) ) +
      ( p_nsf->a[3] * pow( d_t9, ( 1. / 3. ) ) ) +
      ( p_nsf->a[4] * d_t9 ) +
      ( p_nsf->a[5] * pow( d_t9, ( 5. / 3. ) ) ) +
      ( p_nsf->a[6] * log( d_t9 ) )
    );

}

/*##############################################################################
// Libnucnet__Reaction__updateSingleRate().
//############################################################################*/

void
Libnucnet__Reaction__updateSingleRate( 
  Libnucnet__Reaction *self, double d_forward 
)
{

  /*============================================================================
  // If old data exist, free and reallocate.
  //==========================================================================*/

  if( self->pRd )
    Libnucnet__Reaction__freeRateData( self );

  /*============================================================================
  // Now allocate memory.
  //==========================================================================*/

  self->pRd = Libnucnet__Reaction__RateData__new( );

  /*============================================================================
  // Assign function key.
  //==========================================================================*/

  xmlFree( self->sxFunctionKey );
  self->sxFunctionKey = xmlCharStrdup( SINGLE_RATE_STRING );

  /*============================================================================
  // Allocate memory.
  //==========================================================================*/

  self->pRd->pSingle =
    ( Libnucnet__Reaction__Single * )
    malloc( sizeof( Libnucnet__Reaction__Single ) );

  /*============================================================================
  // Assign rate.
  //==========================================================================*/

  self->pRd->pSingle->dSingleRate = d_forward;

}

/*##############################################################################
// Libnucnet__Reaction__updateRateTable().
//############################################################################*/

void
Libnucnet__Reaction__updateRateTable( 
  Libnucnet__Reaction *self,
  gsl_vector *p_t9,
  gsl_vector *p_rate,
  gsl_vector *p_sef
)
{

  size_t i;
  gsl_permutation *p_perm;

  /*============================================================================
  // If old data exist, free.
  //==========================================================================*/

  if( self->pRd )
    Libnucnet__Reaction__freeRateData( self );

  /*============================================================================
  // Now allocate memory.
  //==========================================================================*/

  self->pRd = Libnucnet__Reaction__RateData__new( );

  /*============================================================================
  // Assign function key.
  //==========================================================================*/

  xmlFree( self->sxFunctionKey );
  self->sxFunctionKey = xmlCharStrdup( RATE_TABLE_STRING );

  /*============================================================================
  // Allocate memory for rate table.
  //==========================================================================*/

  self->pRd->pRt =
    ( Libnucnet__Reaction__RateTable * )
    malloc( sizeof( Libnucnet__Reaction__RateTable ) );

  /*============================================================================
  // Check array sizes.
  //==========================================================================*/

  if(
     p_t9->size != p_rate->size ||
     p_t9->size != p_sef->size 
  )
     LIBNUCNET__REAC__ERROR( "Arrays do not have the same size" );

  /*============================================================================
  // Allocate arrays.
  //==========================================================================*/

  self->pRd->pRt->pT9 = gsl_vector_alloc( p_t9->size );
  self->pRd->pRt->pRate = gsl_vector_alloc( p_rate->size );
  self->pRd->pRt->pSef = gsl_vector_alloc( p_sef->size );
  
  /*============================================================================
  // Sort t9 array.
  //==========================================================================*/

  p_perm = gsl_permutation_alloc( p_t9->size );
  gsl_sort_vector_index( p_perm, p_t9 );

  /*============================================================================
  // Assign rate table.
  //==========================================================================*/

  for( i = 0; i < p_perm->size; i++ )
  {
    gsl_vector_set(
      self->pRd->pRt->pT9,
      i,
      p_t9->data[p_perm->data[i]]
    );
    gsl_vector_set(
      self->pRd->pRt->pRate,
      i,
      p_rate->data[p_perm->data[i]]
    );
    gsl_vector_set(
      self->pRd->pRt->pSef,
      i,
      p_sef->data[p_perm->data[i]]
    );
  }

  /*============================================================================
  // Clean up.
  //==========================================================================*/

  gsl_permutation_free( p_perm );

}

/*##############################################################################
// Libnucnet__Reaction__addNonSmokerFit().
//############################################################################*/

void
Libnucnet__Reaction__addNonSmokerFit( 
  Libnucnet__Reaction *self,
  const char *s_note,
  double a[],
  double d_spint,
  double d_spinf,
  double d_tlowhf, 
  double d_tlowfit,
  double d_thighfit,
  double d_acc
) {

  Libnucnet__Reaction__NonSmokerFit *p_nsf;
  xmlChar sx_note[REAC_BUF_SIZE];

  /*============================================================================
  // Allocate memory for rate data if necessary.
  //==========================================================================*/

  if( !self->pRd )
    self->pRd = Libnucnet__Reaction__RateData__new( );

  /*============================================================================
  // Assign function key.
  //==========================================================================*/

  xmlFree( self->sxFunctionKey );
  self->sxFunctionKey = xmlCharStrdup( NON_SMOKER_STRING );

  /*============================================================================
  // Allocate hash.
  //==========================================================================*/

  if( !self->pRd->pNsfHash )
    self->pRd->pNsfHash = xmlHashCreate( 0 );

  /*============================================================================
  // Allocate memory.
  //==========================================================================*/

  p_nsf = 
    ( Libnucnet__Reaction__NonSmokerFit * )
    malloc( sizeof( Libnucnet__Reaction__NonSmokerFit ) );

  if( !p_nsf )
    LIBNUCNET__REAC__ERROR( "Couldn't allocate memory" );

  /*============================================================================
  // Allocate memory for various structure elements.
  //==========================================================================*/

  p_nsf->pSpint = ( double * ) malloc( sizeof( double ) );
  p_nsf->pSpinf = ( double * ) malloc( sizeof( double ) );
  p_nsf->pTlowfit = ( double * ) malloc( sizeof( double ) );
  p_nsf->pThighfit = ( double * ) malloc( sizeof( double ) );
  p_nsf->pTlowHf = ( double * ) malloc( sizeof( double ) );
  p_nsf->pAcc = ( double * ) malloc( sizeof( double ) );

  /*============================================================================
  // Assign non-smoker.
  //==========================================================================*/

  *p_nsf->pSpint = d_spint;
  *p_nsf->pSpinf = d_spinf;
  *p_nsf->pTlowHf = d_tlowhf;
  *p_nsf->pTlowfit = d_tlowfit;
  *p_nsf->pThighfit = d_thighfit;
  *p_nsf->pAcc = d_acc;

  p_nsf->a = a;

  /*============================================================================
  // Get note string.
  //==========================================================================*/

  if( s_note )
    xmlStrPrintf( sx_note, REAC_BUF_SIZE, (const xmlChar *) "%s", s_note );
  else
    xmlStrPrintf( 
      sx_note,
      REAC_BUF_SIZE,
      (const xmlChar *) "%d",
      xmlHashSize( self->pRd->pNsfHash ) 
    );

  p_nsf->sxNote = xmlStrdup( sx_note );
  
  /*============================================================================
  // Add to hash.
  //==========================================================================*/

  if(
    xmlHashAddEntry(
      self->pRd->pNsfHash,
      p_nsf->sxNote,
      p_nsf
    )
    != 0
  )
    LIBNUCNET__REAC__ERROR( "Couldn't add hash entry" );

   return;

}

/*##############################################################################
// Libnucnet__Reaction__updateSource().
//############################################################################*/

void
Libnucnet__Reaction__updateSource(
  Libnucnet__Reaction *self, const char *s_str
) {

  if( !self )
    LIBNUCNET__REAC__ERROR( "Invalid input" );

  if( self->sxSource ) xmlFree( self->sxSource );

  self->sxSource = xmlCharStrdup( s_str );

  if( !self->sxSource )
    LIBNUCNET__REAC__ERROR( "Couldn't allocate memory" );

}

/*##############################################################################
// Libnucnet__Reaction__addReactant().
//############################################################################*/

void
Libnucnet__Reaction__addReactant(
  Libnucnet__Reaction *self, const char *s_str
) {

  Libnucnet__Reaction__Element *p_element;
  xmlListPtr p_list;
  double d_number_duplicates;

  if( !self )
    LIBNUCNET__REAC__ERROR( "Invalid input" );

  p_element =
    ( Libnucnet__Reaction__Element * )
    malloc( sizeof( Libnucnet__Reaction__Element ) );

  if( !p_element )
    LIBNUCNET__REAC__ERROR( "Couldn't allocate memory" );

  p_element->sxName = xmlCharStrdup( s_str );

  if( !p_element->sxName )
    LIBNUCNET__REAC__ERROR( "Couldn't allocate memory" );

  if( Libnucnet__Reaction__Element__isNuclide( p_element ) ) {
    if( !xmlListPushBack( self->pReactantList, p_element ) )
      LIBNUCNET__REAC__ERROR( "Couldn't add reactant to list" );
  } else {
    if( !xmlListPushBack( self->pOtherReactantList, p_element ) )
      LIBNUCNET__REAC__ERROR( "Couldn't add reactant to list" );
  }

  /*============================================================================
  // Update string.
  //==========================================================================*/

  Libnucnet__Reaction__updateString( self );
   
  /*============================================================================
  // Update duplicate factor.
  //==========================================================================*/

  if( Libnucnet__Reaction__Element__isNuclide( p_element ) ) {

    p_list = xmlListDup( self->pReactantList );
  
    d_number_duplicates = (double) xmlListRemoveAll( p_list, p_element );

    xmlListDelete( p_list );

    self->dDuplicateReactantFactor *= d_number_duplicates;

  }

}

/*##############################################################################
// Libnucnet__Reaction__addProduct().
//############################################################################*/

void
Libnucnet__Reaction__addProduct(
  Libnucnet__Reaction *self, const char *s_str
) {

  Libnucnet__Reaction__Element *p_element;
  xmlListPtr p_list;
  double d_number_duplicates;

  if( !self )
    LIBNUCNET__REAC__ERROR( "Invalid input" );

  p_element =
    ( Libnucnet__Reaction__Element * )
    malloc( sizeof( Libnucnet__Reaction__Element ) );

  if( !p_element )
    LIBNUCNET__REAC__ERROR( "Couldn't allocate memory" );

  p_element->sxName = xmlCharStrdup( s_str );

  if( !p_element->sxName )
    LIBNUCNET__REAC__ERROR( "Couldn't allocate memory" );

  if( Libnucnet__Reaction__Element__isNuclide( p_element ) ) {
    if( !xmlListPushBack( self->pProductList, p_element ) )
      LIBNUCNET__REAC__ERROR( "Couldn't add reactant to list" );
  } else {
    if( !xmlListPushBack( self->pOtherProductList, p_element ) )
      LIBNUCNET__REAC__ERROR( "Couldn't add reactant to list" );
  }

  /*============================================================================
  // Update string.
  //==========================================================================*/

  Libnucnet__Reaction__updateString( self );
   
  /*============================================================================
  // Update duplicate factor.
  //==========================================================================*/

  if( Libnucnet__Reaction__Element__isNuclide( p_element ) ) {

    p_list = xmlListDup( self->pProductList );
  
    d_number_duplicates = (double) xmlListRemoveAll( p_list, p_element );

    xmlListDelete( p_list );

    self->dDuplicateProductFactor *= d_number_duplicates;

  }

}

/*##############################################################################
// Libnucnet__Reaction__getString().
//############################################################################*/

const char *
Libnucnet__Reaction__getString( const Libnucnet__Reaction *self ) {

  if ( self )
    return (const char *) self->sxReaction;
  else
    return NULL;

}

/*##############################################################################
// Libnucnet__Reaction__getSource().
//############################################################################*/

const char *
Libnucnet__Reaction__getSource( const Libnucnet__Reaction *self ) {

  if ( self )
    if( self->sxSource )
      return (const char *) self->sxSource;
    else
      return REAC_EMPTY_STRING;
  else
    LIBNUCNET__REAC__ERROR( "Invalid reaction" );

}

/*##############################################################################
// Libnucnet__Reaction__printRateData().
//############################################################################*/

void
Libnucnet__Reaction__printRateData( Libnucnet__Reaction *self ) {

  xmlListPtr p_list;
  size_t i_index;

  if( !self ) {
     LIBNUCNET__REAC__ERROR( "Invalid reaction" );
  }

  fprintf(
    stdout,
    "\n%s\n",
    Libnucnet__Reaction__getString( self )
  );

  fprintf(
    stdout,
    "\nData source: %s\n\n",
    Libnucnet__Reaction__getSource( self )
  );

  if(
    Libnucnet__Reaction__hasUserDefinedRateFunction( self )
  )
  {

    fprintf(
      stdout,
      "\nUser-defined function key: %s\n\n",
      (char *) self->sxFunctionKey
    );

    p_list =
      Libnucnet__Reaction__createRateFunctionPropertyList( self );

    xmlListWalk(
      p_list,
      (xmlListWalker) Libnucnet__Reaction__print_rate_function_property_walker,
      NULL
    );

    xmlListDelete( p_list );

    fprintf( stdout, "\n" );

    return;

  }
       
  if( !xmlStrcmp( self->sxFunctionKey, (const xmlChar *) RATE_TABLE_STRING ) )
  {

    fprintf( stdout, "   T9   \t  Rate  \t   Sef\n" );
    fprintf( stdout, "=========\t=========\t=========\n" );

    for (
      i_index = 0;
      i_index < self->pRd->pRt->pT9->size;
      i_index++
    ) {

      fprintf(
        stdout,
        "%1.3e\t%1.3e\t%1.3e\n", 
        self->pRd->pRt->pT9->data[i_index],
        self->pRd->pRt->pRate->data[i_index],
        self->pRd->pRt->pSef->data[i_index] 
      );

    }

  }
  else if(
    !xmlStrcmp( self->sxFunctionKey, (const xmlChar *) NON_SMOKER_STRING )
  )
  {

    xmlHashScan(
      self->pRd->pNsfHash,
      (xmlHashScanner) Libnucnet__Reaction__printNonSmokerFitCallback,
      NULL
    );

  }
  else if(
    !xmlStrcmp( self->sxFunctionKey, (const xmlChar *) SINGLE_RATE_STRING )
  )
    fprintf( stdout, "Rate: %e\n", self->pRd->pSingle->dSingleRate );
  else
    LIBNUCNET__REAC__ERROR( "No such reaction" );

  fprintf( stdout, "\n" );

}

/*##############################################################################
// Libnucnet__Reaction__createRateFunctionPropertyList().
//############################################################################*/

xmlListPtr
Libnucnet__Reaction__createRateFunctionPropertyList(
  Libnucnet__Reaction *self
)
{

  xmlListPtr p_list;

  p_list =
    xmlListCreate(
      (xmlListDeallocator)
        Libnucnet__Reaction__rate_function_property_data_deallocator,
      (xmlListDataCompare)
        Libnucnet__Reaction__rate_function_property_data_compare
    );

  xmlHashScanFull(
    self->pRd->pRateFunctionPropertyHash,
    (xmlHashScannerFull)
      Libnucnet__Reaction__rate_function_property_list_maker,
    p_list
  );

  return p_list;

}

/*##############################################################################
// Libnucnet__Reaction__rate_function_property_list_maker().
//############################################################################*/

void
Libnucnet__Reaction__rate_function_property_list_maker(
  xmlChar *sx_value,
  xmlListPtr p_list,
  xmlChar *sx_name,
  xmlChar *sx_tag1,
  xmlChar *sx_tag2
)
{

  typedef struct {
    xmlChar *sxName;
    xmlChar *sxTag1;
    xmlChar *sxTag2;
    xmlChar *sxValue;
  } rate_property;

  rate_property *p_property =
    ( rate_property * ) malloc( sizeof( rate_property ) );

  if( !p_property ) LIBNUCNET__REAC__ERROR( "Couldn't allocate memory" );

  p_property->sxName = sx_name;
  p_property->sxTag1 = sx_tag1;
  p_property->sxTag2 = sx_tag2;
  p_property->sxValue = sx_value;
  
  xmlListInsert( p_list, p_property );

}
  
/*##############################################################################
// Libnucnet__Reaction__rate_function_property_data_compare().
//############################################################################*/

int
Libnucnet__Reaction__rate_function_property_data_compare(
  void *p_data1,
  void *p_data2
)
{

  typedef struct {
    xmlChar *sxName;
    xmlChar *sxTag1;
    xmlChar *sxTag2;
    xmlChar *sxValue;
  } property_data;

  int i_compare;

  property_data *p_property1 = ( property_data * ) p_data1; 
  property_data *p_property2 = ( property_data * ) p_data2; 

  i_compare = xmlStrcmp( p_property1->sxName, p_property2->sxName );

  if( i_compare ) return i_compare;

  i_compare = xmlStrcmp( p_property1->sxTag1, p_property2->sxTag1 );

  if( i_compare ) return i_compare;

  i_compare = xmlStrcmp( p_property1->sxTag2, p_property2->sxTag2 );

  if( i_compare )
    return i_compare;
  else
    return 0;

}

/*##############################################################################
// Libnucnet__Reaction__rate_function_property_data_deallocator().
//############################################################################*/

void
Libnucnet__Reaction__rate_function_property_data_deallocator(
  xmlLinkPtr p_link
)
{

  free( xmlLinkGetData( p_link ) );

}

/*##############################################################################
// Libnucnet__Reaction__print_rate_function_property_walker().
//############################################################################*/

int
Libnucnet__Reaction__print_rate_function_property_walker(
  void *p_data,
  void *p_extra_data
)
{

  typedef struct {
    xmlChar *sxName;
    xmlChar *sxTag1;
    xmlChar *sxTag2;
    xmlChar *sxValue;
  } property_data;

  property_data *p_property = ( property_data * ) p_data;

  if( p_extra_data ) LIBNUCNET__REAC__ERROR( "No extra data to this routine" );

  if( !p_property->sxTag2 )
  {
    if( !p_property->sxTag1 )
      fprintf(
        stdout,
        "Property name: %s  Value: %s\n",
        (char *) p_property->sxName,
        (char *) p_property->sxValue
      );
    else
      fprintf(
        stdout,
        "Property name: %s  Tag1: %s Value: %s\n",
        (char *) p_property->sxName,
        (char *) p_property->sxTag1,
        (char *) p_property->sxValue
      );
  }
  else
      fprintf(
        stdout,
        "Property name: %s  Tag1: %s Tag2: %s Value: %s\n",
        (char *) p_property->sxName,
        (char *) p_property->sxTag1,
        (char *) p_property->sxTag2,
        (char *) p_property->sxValue
      );

  return 1;

}

/*##############################################################################
// Libnucnet__Reaction__printNonSmokerFitCallback().
//############################################################################*/

void
Libnucnet__Reaction__printNonSmokerFitCallback(
  Libnucnet__Reaction__NonSmokerFit *p_nsf,
  void *p_data,
  xmlChar *sx_note
)
{
 
  int i_index; 

  if( p_data )
    LIBNUCNET__REAC__ERROR( "Shouldn't have extra data for this routine" );

  fprintf(
    stdout,
    "\nNote: %s\n\n",
    (char *) sx_note
  );

  if( p_nsf->pAcc ) fprintf( stdout, "Acc: %f\n", *(p_nsf->pAcc) );
  if( p_nsf->pSpint ) fprintf( stdout, "Spint: %f\n", *(p_nsf->pSpint) );
  if( p_nsf->pSpinf ) fprintf( stdout, "Spinf: %f\n", *(p_nsf->pSpinf) );
  if( p_nsf->pTlowHf ) fprintf( stdout, "TlowHf: %f\n", *(p_nsf->pTlowHf) );
  if( p_nsf->pTlowfit ) fprintf( stdout, "Tlowfit: %f\n", *(p_nsf->pTlowfit) );
  
  for ( i_index = 1; i_index <= I_NSF; i_index++ )
    printf( "a%d: %f\n", i_index, p_nsf->a[i_index-1] );

}

/*##############################################################################
// Libnucnet__Reac__getNumberOfReactions().
//############################################################################*/

size_t
Libnucnet__Reac__getNumberOfReactions( const Libnucnet__Reac *self ) {

  return (size_t) xmlHashSize( self->pReactionHash );

}

/*##############################################################################
// Libnucnet__Reaction__computeRate().
//############################################################################*/

double
Libnucnet__Reaction__computeRate(
  Libnucnet__Reaction *self,
  double d_t9,
  void *p_data
)
{

  double d_rate;
  Libnucnet__Reac__FD *p_func;

  if( !self ) {

    LIBNUCNET__REAC__ERROR( "Reaction not found" );

  }

  if( GSL_SIGN( d_t9 ) != 1 ) {
    LIBNUCNET__REAC__ERROR( "Invalid temperature" );
  }

  p_func =
    ( Libnucnet__Reac__FD * )
    xmlHashLookup(
      self->pReac->pFDHash,
      self->sxFunctionKey
    );

  if( !p_func )
  {
    fprintf(
      stderr,
      "\n%s: %s\n",
      (char *) self->sxReaction,
      (char *) self->sxFunctionKey
    );
    LIBNUCNET__REAC__ERROR( "No rate function" );
  }

  d_rate =
    p_func->pfFunc(
      self,
      d_t9,
      p_data
    );
  
  if( !gsl_finite( d_rate ) ) {

    fprintf(
      stderr,
      "Reaction: %s\n", Libnucnet__Reaction__getString( self ) 
    );

    fprintf(
      stderr,
      "Out of bounds for t9 = %e\n", d_t9
    );

    LIBNUCNET__REAC__ERROR( "Invalid rate" );

  } else {

    return d_rate;

  } 

}

/*##############################################################################
// Libnucnet__Reaction__getSingleRate()
//############################################################################*/

double
Libnucnet__Reaction__getSingleRate(
  const Libnucnet__Reaction *self,
  double d_t9,
  void *p_data
)
{

  if( d_t9 < 0 || p_data ) LIBNUCNET__REAC__ERROR( "Invalid input" );

  return self->pRd->pSingle->dSingleRate;

}

/*##############################################################################
// Libnucnet__Reaction__computeRateFromTable()
//############################################################################*/

double
Libnucnet__Reaction__computeRateFromTable(
  Libnucnet__Reaction *self,
  double d_t9,
  void *p_data
)
{

  double d_result, d_rate;
  double *a_y;
  gsl_interp_accel *acc;
  gsl_spline *spline;
  size_t i;

  if( p_data ) LIBNUCNET__REAC__ERROR( "No extra data to this routine" );

  if( d_t9 <= gsl_vector_get( self->pRd->pRt->pT9, 0L ) )
  {
    d_rate = gsl_vector_get( self->pRd->pRt->pRate, 0L );
  }
  else if(
    d_t9 >=
      gsl_vector_get( self->pRd->pRt->pT9, self->pRd->pRt->pT9->size - 1 )
  )
  {
    d_rate =
      gsl_vector_get(
        self->pRd->pRt->pRate, self->pRd->pRt->pRate->size - 1
      );
  }
  else
  {
    a_y =
      ( double * ) malloc( sizeof( double ) * self->pRd->pRt->pT9->size );
    for( i = 0; i < self->pRd->pRt->pRate->size; i++ ) {
      a_y[i] =
        log10( gsl_vector_get( self->pRd->pRt->pRate, i ) + TINY ) +
        log10( gsl_vector_get( self->pRd->pRt->pSef, i ) + TINY );
    }

    if( self->pRd->pRt->pT9->size == 2 )
    {
      d_result =
        a_y[0] +
        ( d_t9 - self->pRd->pRt->pT9->data[0] ) * ( a_y[1] - a_y[0] ) /
        ( self->pRd->pRt->pT9->data[1] - self->pRd->pRt->pT9->data[0] );
    }

    else
    {

      acc = gsl_interp_accel_alloc();
      spline =
        gsl_spline_alloc( gsl_interp_cspline, self->pRd->pRt->pT9->size );
      gsl_spline_init(
        spline,
        self->pRd->pRt->pT9->data,
        a_y,
        self->pRd->pRt->pT9->size
      );

      d_result = gsl_spline_eval( spline, d_t9, acc );

      gsl_spline_free( spline );
      gsl_interp_accel_free( acc );
    }

    free( a_y );
    d_rate = pow( 10., d_result );

  }

  return d_rate;

}

/*##############################################################################
// Libnucnet__Reac__removeReaction()
//############################################################################*/

int
Libnucnet__Reac__removeReaction(
  Libnucnet__Reac * self, Libnucnet__Reaction * p_reaction
){


   if(
     xmlHashRemoveEntry(
       self->pReactionHash,
       p_reaction->sxReaction,
       (xmlHashDeallocator) Libnucnet__Reaction__hashFree
     )
   )
     return 0;

   self->iUpdate++;

   return 1;

}

/*##############################################################################
// Libnucnet__Reaction__Element__free()
//############################################################################*/

void
Libnucnet__Reaction__Element__free(
  xmlLinkPtr p_link
)
{

  Libnucnet__Reaction__Element *p_element;

  p_element =
    ( Libnucnet__Reaction__Element *)
    xmlLinkGetData( p_link );
  xmlFree( p_element->sxName );
  free( p_element );

}

/*##############################################################################
// Libnucnet__Reaction__free()
//############################################################################*/

void
Libnucnet__Reaction__free(
  Libnucnet__Reaction *self
) {

  if( !self ) {
    LIBNUCNET__REAC__ERROR( "Cannot free non-existent reaction" );
  }

  if( self->pRd ) Libnucnet__Reaction__freeRateData( self );

  xmlFree( self->sxReaction );
  xmlFree( self->sxSource );
  xmlFree( self->sxFunctionKey );

  xmlListDelete( self->pReactantList );
  xmlListDelete( self->pOtherReactantList );
  xmlListDelete( self->pProductList );
  xmlListDelete( self->pOtherProductList );

  free( self );

  return;

}

/*##############################################################################
// Libnucnet__Reaction__hashFree()
//############################################################################*/

void Libnucnet__Reaction__hashFree(
  Libnucnet__Reaction *self, xmlChar * sx_name
) {

  if( !sx_name ) {
    LIBNUCNET__REAC__ERROR( "Cannot free non-existent reaction" );
  }

  Libnucnet__Reaction__free( self );

  return;

}

/*##############################################################################
// Libnucnet__Reaction__freeRateData()
//############################################################################*/

void
Libnucnet__Reaction__freeRateData(
  Libnucnet__Reaction *self
)
{

  if( !self->sxFunctionKey )
    LIBNUCNET__REAC__ERROR( "Trying to free non-existent rate data type" );

  else if(
    !xmlStrcmp( self->sxFunctionKey, (const xmlChar *) RATE_TABLE_STRING )
  )
  {
    gsl_vector_free( self->pRd->pRt->pT9 );
    gsl_vector_free( self->pRd->pRt->pRate );
    gsl_vector_free( self->pRd->pRt->pSef );
    free( self->pRd->pRt );
  }

  else if(
    !xmlStrcmp( self->sxFunctionKey, (const xmlChar *) NON_SMOKER_STRING )
  )
    xmlHashFree(
      self->pRd->pNsfHash,
      (xmlHashDeallocator) Libnucnet__Reaction__NonSmokerFit__free
    );

  else if(
    !xmlStrcmp( self->sxFunctionKey, (const xmlChar *) SINGLE_RATE_STRING )
  )
    free( self->pRd->pSingle );

  else
    xmlHashFree(
      self->pRd->pRateFunctionPropertyHash,
      (xmlHashDeallocator) xmlFree
    );

  free( self->pRd );

}

/*##############################################################################
// Libnucnet__Reac__free()
//############################################################################*/

void
Libnucnet__Reac__free( Libnucnet__Reac *self ) {

  if( !self ) return;

  if( self->iOwner )
  {

    xmlHashFree(
      self->pReactionHash,
      (xmlHashDeallocator) Libnucnet__Reaction__hashFree
    );

    if( self->pFDHash )
    {
      xmlHashFree(
        self->pFDHash,
        (xmlHashDeallocator) free
      );
    }

  }
  else
  {

    xmlHashFree(
      self->pReactionHash,
      NULL
    );

  }

  free( self );

}

/*##############################################################################
// Libnucnet__Reac__getReactionByString( )
//############################################################################*/

Libnucnet__Reaction
*Libnucnet__Reac__getReactionByString(
  const Libnucnet__Reac *self, const char * s_reaction
) {

  return
    ( Libnucnet__Reaction * )
    xmlHashLookup( self->pReactionHash, 
      (const xmlChar *) s_reaction
    );

}

/*##############################################################################
// Libnucnet__Reaction__getDuplicateReactantFactor().
//############################################################################*/

double
Libnucnet__Reaction__getDuplicateReactantFactor(
  const Libnucnet__Reaction * self
) {

  if( !self ) {
    LIBNUCNET__REAC__ERROR( "Reaction not found" );
  }

  return self->dDuplicateReactantFactor;

}

/*##############################################################################
// Libnucnet__Reaction__getDuplicateProductFactor().
//############################################################################*/

double
Libnucnet__Reaction__getDuplicateProductFactor(
  const Libnucnet__Reaction * self
) {

  if( !self ) {
    LIBNUCNET__REAC__ERROR( "Reaction not found" );
  }

  return self->dDuplicateProductFactor;

}

/*##############################################################################
// Libnucnet__Reac__is_valid_input_xml()
//############################################################################*/

int Libnucnet__Reac__is_valid_input_xml( const char *s_filename ) {

  xmlDocPtr p_doc;
  xmlSchemaParserCtxtPtr p_parser_ctxt;
  xmlSchemaValidCtxtPtr p_valid_ctxt;
  xmlSchemaPtr p_schema;
  int i_valid;

  p_parser_ctxt = xmlSchemaNewParserCtxt( LIBNUCNET__REAC__SCHEMA );

  p_schema = xmlSchemaParse( p_parser_ctxt );

  if( !p_schema ) {
     LIBNUCNET__REAC__ERROR( "Schema not found or invalid" );
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
// Libnucnet__Reaction__updateString().
//############################################################################*/

void
Libnucnet__Reaction__updateString(
  Libnucnet__Reaction *self
)
{

  xmlChar *sx_arrow;

  if( self->sxReaction ) xmlFree( self->sxReaction );

  sx_arrow = xmlCharStrdup( " -> " );

  self->sxReaction = xmlCharStrdup( NULL );

  /*============================================================================
  // Construct reactant string.
  //==========================================================================*/

  xmlListWalk(
    self->pReactantList,
    (xmlListWalker) Libnucnet__Reaction__updateStringWalker,
    self
  ); 

  xmlListWalk(
    self->pOtherReactantList,
    (xmlListWalker) Libnucnet__Reaction__updateStringWalker,
    self
  ); 

  /*============================================================================
  // Add arrow.
  //==========================================================================*/

  self->sxReaction = xmlStrcat( self->sxReaction, sx_arrow );

  /*============================================================================
  // Construct product string.
  //==========================================================================*/

  xmlListWalk(
    self->pProductList,
    (xmlListWalker) Libnucnet__Reaction__updateStringWalker,
    self
  );

  xmlListWalk(
    self->pOtherProductList,
    (xmlListWalker) Libnucnet__Reaction__updateStringWalker,
    self
  );

  xmlFree( sx_arrow );

}

/*##############################################################################
// Libnucnet__Reaction__updateStringWalker1().
//############################################################################*/

int
Libnucnet__Reaction__updateStringWalker(
  Libnucnet__Reaction__Element *p_element,
  Libnucnet__Reaction *p_reaction
)
{

  xmlChar *sx_plus, *sx_arrow, *sx_str;

  sx_arrow = xmlCharStrdup( " -> " );

  if( !p_reaction->sxReaction )
    p_reaction->sxReaction = xmlStrdup( p_element->sxName );
  else {
    sx_str =
      xmlStrsub(
        p_reaction->sxReaction,
        xmlStrlen( p_reaction->sxReaction ) - xmlStrlen( sx_arrow ),
        xmlStrlen( sx_arrow )
      );
    if( !sx_str || xmlStrcmp( sx_str, sx_arrow ) ) {
      sx_plus = xmlCharStrdup( " + " );
      p_reaction->sxReaction =
        xmlStrcat( p_reaction->sxReaction, sx_plus );
      xmlFree( sx_plus );
    }
    if( sx_str ) xmlFree( sx_str );
    p_reaction->sxReaction =
      xmlStrcat( p_reaction->sxReaction, p_element->sxName );
  }

  xmlFree( sx_arrow );

  return 1;

}
    
/*##############################################################################
// Libnucnet__Reaction__new().
//############################################################################*/

Libnucnet__Reaction
*Libnucnet__Reaction__new( void )
{

  Libnucnet__Reaction *self;

  /*============================================================================
  // Allocate memory.
  //==========================================================================*/

  self = (Libnucnet__Reaction *) malloc( sizeof( Libnucnet__Reaction ) );

  self->pReactantList =
    xmlListCreate( 
      (xmlListDeallocator) Libnucnet__Reaction__Element__free,
      (xmlListDataCompare) Libnucnet__Reaction__Element__data_compare
    );

  self->pOtherReactantList =
    xmlListCreate( 
      (xmlListDeallocator) Libnucnet__Reaction__Element__free,
      (xmlListDataCompare) Libnucnet__Reaction__Element__data_compare
    );

  self->pProductList =
    xmlListCreate( 
      (xmlListDeallocator) Libnucnet__Reaction__Element__free,
      (xmlListDataCompare) Libnucnet__Reaction__Element__data_compare
    );

  self->pOtherProductList =
    xmlListCreate( 
      (xmlListDeallocator) Libnucnet__Reaction__Element__free,
      (xmlListDataCompare) Libnucnet__Reaction__Element__data_compare
    );

  if(
      !self ||
      !self->pReactantList ||
      !self->pProductList
  ) {
    LIBNUCNET__REAC__ERROR( "Couldn't allocate memory for reaction" );
  }

  /*============================================================================
  // Initialize source, function, function name, and string.
  //==========================================================================*/

  self->sxSource = NULL;
  self->sxReaction = NULL;
  self->sxFunctionKey = NULL;

  /*============================================================================
  // Initialize rate data.
  //==========================================================================*/

  self->pRd = NULL;

  /*============================================================================
  // Initialize duplicate data.
  //==========================================================================*/

  self->dDuplicateReactantFactor = 1.;
  self->dDuplicateProductFactor = 1.;
  self->pParentDuplicate = NULL;

  /*============================================================================
  // Done. Return.
  //==========================================================================*/

  return self;

}

/*##############################################################################
// Libnucnet__Reac__addReaction().
//############################################################################*/

int
Libnucnet__Reac__addReaction(
  Libnucnet__Reac *self,
  Libnucnet__Reaction *p_reaction
)
{

  /*============================================================================
  // Set Reac for reaction.
  //==========================================================================*/

  p_reaction->pReac = self;

  /*============================================================================
  // Add reaction to hash and set update flag.
  //==========================================================================*/

  if(
    xmlHashAddEntry(
      self->pReactionHash,
      p_reaction->sxReaction,
      p_reaction
    )
  )
    return 0;

  self->iUpdate++;

  /*============================================================================
  // Done.
  //==========================================================================*/

  return 1;

}

/*##############################################################################
// Libnucnet__Reac__updateReaction().
//############################################################################*/

int
Libnucnet__Reac__updateReaction(
  Libnucnet__Reac *self,
  Libnucnet__Reaction *p_reaction
)
{

  /*============================================================================
  // Set Reac for reaction.
  //==========================================================================*/

  p_reaction->pReac = self;

  /*============================================================================
  // Check whether trying to update with same reaction.
  //==========================================================================*/

  if(
    (Libnucnet__Reaction *)
    xmlHashLookup(
      self->pReactionHash,
      p_reaction->sxReaction
    )
    == p_reaction
  )
    return 0;

  /*============================================================================
  // Replace reaction in hash and set update flag.
  //==========================================================================*/

  if(
    xmlHashUpdateEntry(
      self->pReactionHash,
      p_reaction->sxReaction,
      p_reaction,
      (xmlHashDeallocator) Libnucnet__Reaction__hashFree
    )
  )
    return 0;

  self->iUpdate++;

  /*============================================================================
  // Done.
  //==========================================================================*/

  return 1;

}

/*##############################################################################
// Libnucnet__Reac__getDuplicateReactions().
//############################################################################*/

Libnucnet__Reac *
Libnucnet__Reac__getDuplicateReactions(
  const Libnucnet__Reac *self
)
{

  typedef struct extra_data {
    xmlHashTablePtr pHash;
    Libnucnet__Reac *pDuplicates;
  } extra_data;

  extra_data *p_data;
  Libnucnet__Reac *p_duplicates;

  if( !self ) LIBNUCNET__REAC__ERROR( "Invalid Libnucnet__Reac" );

  p_data = ( extra_data * ) malloc( sizeof( extra_data ) );

  p_data->pHash = xmlHashCreate( 0 );
  p_data->pDuplicates = Libnucnet__Reac__new();

  xmlHashScan(
    self->pReactionHash,
    (xmlHashScanner) Libnucnet__Reac__getDuplicateReactionsCallback,
    p_data
  );

  p_duplicates = p_data->pDuplicates;
  xmlHashFree( p_data->pHash, NULL );
  free( p_data );

  return p_duplicates;

}

/*##############################################################################
// Libnucnet__Reaction__getDuplicateReactionsCallback().
//############################################################################*/

void
Libnucnet__Reac__getDuplicateReactionsCallback(
  Libnucnet__Reaction *p_reaction, void *p_data, xmlChar *sx_reaction
)
{
      
  typedef struct extra_data {
    xmlHashTablePtr pHash;
    Libnucnet__Reac *pDuplicates;
  } extra_data;

  Libnucnet__Reaction *p_new_reaction = NULL, *p_old_reaction = NULL;
  Libnucnet__Reaction__Element *p_element;
  xmlListPtr p_list1, p_list2;
  xmlChar *sx_str, *sx_reactants, *sx_products;
  extra_data *p_extra_data;

  if( !sx_reaction )
    LIBNUCNET__REAC__ERROR( "No such reaction " );

  /*============================================================================
  // Get extra data.
  //==========================================================================*/

  p_extra_data = ( extra_data * ) p_data;

/*==============================================================================
// Reactant string.
//============================================================================*/

  p_list1 = xmlListDup( p_reaction->pReactantList );

  p_list2 = xmlListDup( p_reaction->pOtherReactantList );

  xmlListMerge( p_list1, p_list2 );

  xmlListDelete( p_list2 );

  sx_reactants = xmlCharStrdup( REAC_EMPTY_STRING );

  while( xmlListSize( p_list1 ) != 0 )
  {
    p_element =
      ( Libnucnet__Reaction__Element * )
      xmlLinkGetData( xmlListFront( p_list1 ) );
    sx_reactants =
      xmlStrcat(
        sx_reactants,
        p_element->sxName
      ); 
    xmlListPopFront( p_list1 );
  }

  xmlListDelete( p_list1 );

  /*============================================================================
  // Product string.
  //==========================================================================*/

  p_list1 = xmlListDup( p_reaction->pProductList );

  p_list2 = xmlListDup( p_reaction->pOtherProductList );

  xmlListMerge( p_list1, p_list2 );

  xmlListDelete( p_list2 );

  sx_products = xmlCharStrdup( REAC_EMPTY_STRING );

  while( xmlListSize( p_list1 ) != 0 )
  {
    p_element =
      ( Libnucnet__Reaction__Element * )
      xmlLinkGetData( xmlListFront( p_list1 ) );
    sx_products =
      xmlStrcat(
        sx_products,
        p_element->sxName
      ); 
    xmlListPopFront( p_list1 );
  }

  xmlListDelete( p_list1 );

  /*============================================================================
  // Create full string.
  //==========================================================================*/

  sx_str = xmlCharStrdup( "" );

  if( xmlStrcmp( sx_reactants, sx_products ) < 0 )
  {
    sx_str = xmlStrcat( sx_str, sx_reactants );
    sx_str = xmlStrcat( sx_str, sx_products );
  }
  else if( xmlStrcmp( sx_reactants, sx_products ) > 0 )
  {
    sx_str = xmlStrcat( sx_str, sx_products );
    sx_str = xmlStrcat( sx_str, sx_reactants );
  }
  else
    LIBNUCNET__REAC__ERROR( "Reactant and product strings the same" );

  xmlFree( sx_reactants );
  xmlFree( sx_products );

  /*============================================================================
  // Check whether reaction already present in hash.  If so, add to
  // duplicates.
  //==========================================================================*/

  if(
    xmlHashAddEntry( p_extra_data->pHash, sx_str, p_reaction )
  )
  {
    p_new_reaction = Libnucnet__Reaction__copy( p_reaction );
    p_old_reaction =
      (Libnucnet__Reaction *) xmlHashLookup( p_extra_data->pHash, sx_str );
    p_new_reaction->pParentDuplicate = p_old_reaction;
    Libnucnet__Reac__addReaction( p_extra_data->pDuplicates, p_new_reaction );
  }

  /*============================================================================
  // Done with the string.
  //==========================================================================*/

  xmlFree( sx_str );

}

/*##############################################################################
// Libnucnet__Reac__data_compare().
//############################################################################*/

int
Libnucnet__Reac__data_compare(
  xmlChar *sx_1, xmlChar *sx_2
)
{

  return xmlStrcmp( sx_1, sx_2 );

}

/*##############################################################################
// Libnucnet__Reaction__Element__data_compare().
//############################################################################*/

int
Libnucnet__Reaction__Element__data_compare(
  Libnucnet__Reaction__Element *p_element1,
  Libnucnet__Reaction__Element *p_element2
)
{

  return xmlStrcmp( p_element1->sxName, p_element2->sxName );

}

/*##############################################################################
// Libnucnet__Reaction__copy().
//############################################################################*/

Libnucnet__Reaction *
Libnucnet__Reaction__copy( const Libnucnet__Reaction *self )
{

  Libnucnet__Reaction *p_reaction;

  p_reaction = Libnucnet__Reaction__new(); 

  p_reaction->pReac = self->pReac;

  p_reaction->sxReaction = xmlStrdup( self->sxReaction );

  p_reaction->sxSource = xmlStrdup( self->sxSource );

  Libnucnet__Reaction__copy_element_list(
    p_reaction->pReactantList, self->pReactantList
  );

  Libnucnet__Reaction__copy_element_list(
    p_reaction->pOtherReactantList, self->pOtherReactantList
  );

  Libnucnet__Reaction__copy_element_list(
    p_reaction->pProductList, self->pProductList
  );

  Libnucnet__Reaction__copy_element_list(
    p_reaction->pOtherProductList, self->pOtherProductList
  );

  p_reaction->pRd = Libnucnet__Reaction__RateData__new( );

  p_reaction->sxFunctionKey = xmlStrdup( self->sxFunctionKey );

  if( !self->sxFunctionKey)
    LIBNUCNET__REAC__ERROR( "No such reaction" );
  else if(
    !xmlStrcmp( self->sxFunctionKey, (const xmlChar *) RATE_TABLE_STRING )
  )
  {

    p_reaction->pRd->pRt =
      ( Libnucnet__Reaction__RateTable * )
      malloc( sizeof( Libnucnet__Reaction__RateTable ) );
    if( !p_reaction->pRd->pRt )
       LIBNUCNET__REAC__ERROR( "Couldn't allocate memory" );

    p_reaction->pRd->pRt->pT9 =
      gsl_vector_alloc( self->pRd->pRt->pT9->size );
    p_reaction->pRd->pRt->pRate =
      gsl_vector_alloc( self->pRd->pRt->pRate->size );
    p_reaction->pRd->pRt->pSef =
      gsl_vector_alloc( self->pRd->pRt->pSef->size );
    if(
      !p_reaction->pRd->pRt->pT9 ||
      !p_reaction->pRd->pRt->pRate ||
      !p_reaction->pRd->pRt->pSef
    )
      LIBNUCNET__REAC__ERROR( "Couldn't allocate memory" );

    gsl_vector_memcpy(
      p_reaction->pRd->pRt->pT9, self->pRd->pRt->pT9
    );

    gsl_vector_memcpy(
      p_reaction->pRd->pRt->pRate, self->pRd->pRt->pRate
    );

    gsl_vector_memcpy(
      p_reaction->pRd->pRt->pSef, self->pRd->pRt->pSef
    );

    return p_reaction;

  }
  else if(
    !xmlStrcmp( self->sxFunctionKey, (const xmlChar *) NON_SMOKER_STRING )
  )
  {

    p_reaction->pRd->pNsfHash =
      xmlHashCopy(
        self->pRd->pNsfHash,
        (xmlHashCopier) Libnucnet__Reaction__NonSmokerFit__copier
      );

    return p_reaction;

  }
  else if(
    !xmlStrcmp( self->sxFunctionKey, (const xmlChar *) SINGLE_RATE_STRING )
  )
  {

    p_reaction->pRd->pSingle =
      ( Libnucnet__Reaction__Single * )
      malloc( sizeof( Libnucnet__Reaction__Single ) );
    p_reaction->pRd->pSingle->dSingleRate =
      self->pRd->pSingle->dSingleRate;
    return p_reaction;

  }
  else
  {
    p_reaction->pRd->pRateFunctionPropertyHash =
      xmlHashCopy(
        self->pRd->pRateFunctionPropertyHash,
        (xmlHashCopier) xmlStrdup
      );
    return p_reaction;
  }

}

/*##############################################################################
// Libnucnet__Reaction__copy_element_list().
//############################################################################*/

void
Libnucnet__Reaction__copy_element_list(
  xmlListPtr p_destination_list, xmlListPtr p_source_list
)
{

  xmlListWalk(
    p_source_list,
    (xmlListWalker)
       Libnucnet__Reaction__copy_element_list_walker,
    p_destination_list
  );

}

/*##############################################################################
// Libnucnet__Reaction__copy_element_list_walker().
//############################################################################*/

int
Libnucnet__Reaction__copy_element_list_walker(
  Libnucnet__Reaction__Element *p_element, xmlListPtr p_list
)
{

  Libnucnet__Reaction__Element *p_new_element;

  p_new_element =
    ( Libnucnet__Reaction__Element * )
    malloc( sizeof( Libnucnet__Reaction__Element ) );

  if( !p_new_element )
    LIBNUCNET__REAC__ERROR( "Couldn't allocate memeory" );

  p_new_element->sxName = xmlStrdup( p_element->sxName );

  if( !p_new_element->sxName )
    LIBNUCNET__REAC__ERROR( "Couldn't allocate memeory" );

  if( !xmlListPushBack( p_list, p_new_element ) )
    LIBNUCNET__REAC__ERROR( "Couldn't push back list" );

  return 1;

}

/*##############################################################################
// Libnucnet__Reaction__duplicate_element_list().
//############################################################################*/

xmlListPtr
Libnucnet__Reaction__duplicate_element_list( xmlListPtr p_list )
{

  xmlListPtr p_new_list;

  p_new_list =
    xmlListCreate(
      NULL,
      (xmlListDataCompare) Libnucnet__Reaction__Element__data_compare
    );

  xmlListWalk(
    p_list,
    (xmlListWalker)
       Libnucnet__Reaction__duplicate_element_list_walker,
    p_new_list
  );

  return p_new_list;

}
    
/*##############################################################################
// Libnucnet__Reaction__duplicate_element_list_walker().
//############################################################################*/

int
Libnucnet__Reaction__duplicate_element_list_walker(
  Libnucnet__Reaction__Element *p_element, xmlListPtr p_list
)
{

  if( !xmlListPushBack( p_list, p_element ) )
    LIBNUCNET__REAC__ERROR( "Couldn't push back list" );

  return 1;

}

/*##############################################################################
// Libnucnet__Reaction__Element__isNuclide().
//############################################################################*/

int
Libnucnet__Reaction__Element__isNuclide(
  const Libnucnet__Reaction__Element *self
)
{

  if(
    !strcmp( GAMMA, (char *) self->sxName ) || 
    !strcmp( ELECTRON_ANTINEUTRINO, (char *) self->sxName ) ||
    !strcmp( ELECTRON_NEUTRINO, (char *) self->sxName ) ||
    !strcmp( MU_ANTINEUTRINO, (char *) self->sxName ) ||
    !strcmp( MU_NEUTRINO, (char *) self->sxName ) ||
    !strcmp( TAU_ANTINEUTRINO, (char *) self->sxName ) ||
    !strcmp( TAU_NEUTRINO, (char *) self->sxName ) ||
    !strcmp( ELECTRON, (char *) self->sxName ) ||
    !strcmp( POSITRON, (char *) self->sxName ) ||
    !strcmp( MU, (char *) self->sxName ) ||
    !strcmp( ANTIMU, (char *) self->sxName ) ||
    !strcmp( TAU, (char *) self->sxName ) ||
    !strcmp( ANTITAU, (char *) self->sxName )
  )
    return 0;
  else
    return 1;

}

/*##############################################################################
// Libnucnet__Reaction__NonSmokerFit__free().
//############################################################################*/

void
Libnucnet__Reaction__NonSmokerFit__free(
  Libnucnet__Reaction__NonSmokerFit *self, xmlChar *sx_note
)
{

  if( !sx_note )
    LIBNUCNET__REAC__ERROR( "Cannot free non-existent fit" );

  xmlFree( self->sxNote );

  if( self->pSpint ) free( self->pSpint );
  if( self->pSpinf ) free( self->pSpinf );
  if( self->pTlowHf ) free( self->pTlowHf );
  if( self->pAcc ) free( self->pAcc );

  free( self->pTlowfit );
  free( self->pThighfit );
  free( self->a );
  free( self );

}

/*##############################################################################
// Libnucnet__Reaction__NonSmokerFit__copier().
//############################################################################*/

Libnucnet__Reaction__NonSmokerFit *
Libnucnet__Reaction__NonSmokerFit__copier(
  Libnucnet__Reaction__NonSmokerFit * self, xmlChar * sx_note
)
{

  int i;
  Libnucnet__Reaction__NonSmokerFit *p_nsf;

  if( !sx_note )
    LIBNUCNET__REAC__ERROR( "Invalid note" );

  p_nsf =
    ( Libnucnet__Reaction__NonSmokerFit * )
    malloc( sizeof( Libnucnet__Reaction__NonSmokerFit ) );

  if( !p_nsf )
    LIBNUCNET__REAC__ERROR( "Couldn't allocate memory" );

  p_nsf->pSpint = NULL;
  p_nsf->pSpinf = NULL;
  p_nsf->pTlowHf = NULL;
  p_nsf->pTlowfit = NULL;
  p_nsf->pAcc = NULL;

  p_nsf->sxNote = xmlStrdup( self->sxNote );

  if( self->pSpint ) {
    p_nsf->pSpint = ( double * ) malloc( sizeof( double ) );
    *p_nsf->pSpint = *self->pSpint;
  }

  if( self->pSpinf ) {
    p_nsf->pSpinf = ( double * ) malloc( sizeof( double ) );
    *p_nsf->pSpinf = *self->pSpinf;
  }

  if( self->pTlowHf ) {
    p_nsf->pTlowHf = ( double * ) malloc( sizeof( double ) );
    *p_nsf->pTlowHf = *self->pTlowHf;
  }

  if( self->pAcc ) {
    p_nsf->pAcc = ( double * ) malloc( sizeof( double ) );
    *p_nsf->pAcc = *self->pAcc;
  }

  p_nsf->pTlowfit = ( double * ) malloc( sizeof( double ) );
  *p_nsf->pTlowfit = *self->pTlowfit;

  p_nsf->pThighfit = ( double * ) malloc( sizeof( double ) );
  *p_nsf->pThighfit = *self->pThighfit;

  p_nsf->a = ( double * ) malloc( sizeof( double ) * I_NSF );
  if( !p_nsf->a )
     LIBNUCNET__REAC__ERROR( "Couldn't allocate memory" );
  for( i = 0; i < I_NSF; i++ )
    p_nsf->a[i] = self->a[i];

  return p_nsf;

}

/*##############################################################################
// Libnucnet__Reaction__NonSmokerFit__makeXmlCallback().
//############################################################################*/

void
Libnucnet__Reaction__NonSmokerFit__makeXmlCallback(
  Libnucnet__Reaction__NonSmokerFit *self,
  void *p_data,
  xmlChar *sx_note
)
{
  
  struct nsf_user_data {
    xmlNodePtr pRateNode;
    xmlHashTablePtr pHash;
  };
  xmlChar *sx_str1, *sx_str2;
  xmlNodePtr p_node;

  struct nsf_user_data *p_nsf_user_data =
    (struct nsf_user_data *) p_data;

  sx_str1 = xmlCharStrdup( "0" );
  if(
     xmlHashSize( p_nsf_user_data->pHash ) == 1 && 
     xmlStrcmp( sx_note, sx_str1 ) == 0
  )
  {
    Libnucnet__Reaction__NonSmokerFit__makeXmlCallbackHelper(
      self, p_nsf_user_data->pRateNode
    );
  }
  else
  {
    sx_str2 = xmlCharStrdup( FIT );
    p_node =
      xmlNewChild(
        p_nsf_user_data->pRateNode,
        NULL,
        sx_str2,
        NULL
      );
    xmlFree( sx_str2 );
    sx_str2 = xmlCharStrdup( NOTE );
    xmlNewProp(
       p_node,
       sx_str2,
       sx_note
    );
    xmlFree( sx_str2 );
    Libnucnet__Reaction__NonSmokerFit__makeXmlCallbackHelper(
      self, p_node
    );
  }

  xmlFree( sx_str1 );

}

/*##############################################################################
// Libnucnet__Reaction__NonSmokerFit__makeXmlCallbackHelper().
//############################################################################*/

void
Libnucnet__Reaction__NonSmokerFit__makeXmlCallbackHelper(
  Libnucnet__Reaction__NonSmokerFit *self,
  xmlNodePtr p_node
)
{

  xmlChar *sx_str1, *sx_str2;
  int i;

  if( self->pSpint ) {

    sx_str2 = (xmlChar *) malloc( sizeof( xmlChar ) * REAC_BUF_SIZE );
    xmlStrPrintf(
      sx_str2,
      REAC_BUF_SIZE,
      (const xmlChar *) "%g",
      *(self->pSpint)
    );

    sx_str1 = xmlCharStrdup( SPINT );

    xmlNewChild(
      p_node,
      NULL,
      sx_str1,
      sx_str2
    );

    xmlFree( sx_str1 );
    xmlFree( sx_str2 );

  }


  if( self->pSpinf ) {

    sx_str2 = (xmlChar *) malloc( sizeof( xmlChar ) * REAC_BUF_SIZE );
    xmlStrPrintf(
      sx_str2,
      REAC_BUF_SIZE,
      (const xmlChar *) "%g",
      *(self->pSpinf)
    );

    sx_str1 = xmlCharStrdup( SPINF );
    xmlNewChild(
      p_node,
      NULL,
      sx_str1,
      sx_str2
    );

    xmlFree( sx_str1 );
    xmlFree( sx_str2 );

  }

  if( self->pTlowHf ) {

    sx_str2 = (xmlChar *) malloc( sizeof( xmlChar ) * REAC_BUF_SIZE );
    xmlStrPrintf(
      sx_str2,
      REAC_BUF_SIZE,
      (const xmlChar *) "%g",
      *(self->pTlowHf)
    );

    sx_str1 = xmlCharStrdup( TLOWHF );
    xmlNewChild(
      p_node,
      NULL,
      sx_str1,
      sx_str2
    );

    xmlFree( sx_str1 );
    xmlFree( sx_str2 );

  }

  sx_str2 = (xmlChar *) malloc( sizeof( xmlChar ) * REAC_BUF_SIZE );
  xmlStrPrintf(
    sx_str2,
    REAC_BUF_SIZE,
    (const xmlChar *) "%g",
    *(self->pTlowfit)
  );

  sx_str1 = xmlCharStrdup( TLOWFIT );
  xmlNewChild(
    p_node,
    NULL,
    sx_str1,
    sx_str2
  );

  xmlFree( sx_str1 );
  xmlFree( sx_str2 );

  sx_str2 = (xmlChar *) malloc( sizeof( xmlChar ) * REAC_BUF_SIZE );
  xmlStrPrintf(
    sx_str2,
    REAC_BUF_SIZE,
    (const xmlChar *) "%g",
    *(self->pThighfit)
  );

  sx_str1 = xmlCharStrdup( THIGHFIT );
  xmlNewChild(
    p_node,
    NULL,
    sx_str1,
    sx_str2
  );

  xmlFree( sx_str1 );
  xmlFree( sx_str2 );


  if( self->pAcc ) {

    sx_str2 = (xmlChar *) malloc( sizeof( xmlChar ) * REAC_BUF_SIZE );
    xmlStrPrintf(
      sx_str2,
      REAC_BUF_SIZE,
      (const xmlChar *) "%g",
      *(self->pAcc)
    );

    sx_str1 = xmlCharStrdup( ACC );
    xmlNewChild(
      p_node,
      NULL,
      sx_str1,
      sx_str2
    );

    xmlFree( sx_str1 );
    xmlFree( sx_str2 );

  }

  sx_str1 = (xmlChar *) malloc( sizeof( xmlChar *) * REAC_BUF_SIZE ); 

  for( i = 1; i <= I_NSF; i++ ) { 
 
    xmlStrPrintf( sx_str1, REAC_BUF_SIZE, (const xmlChar *) "a%d", i );

    sx_str2 = (xmlChar *) malloc( sizeof( xmlChar ) * REAC_BUF_SIZE );
    xmlStrPrintf(
      sx_str2,
      REAC_BUF_SIZE,
      (const xmlChar *) "%g",
      self->a[i-1]
    );

    xmlNewChild(
      p_node,
      NULL,
      sx_str1,
      sx_str2
    );

    xmlFree( sx_str2 );

  }

  xmlFree( sx_str1 );

}

/*##############################################################################
// Libnucnet__Reac__iterateReactions()
//############################################################################*/

void
Libnucnet__Reac__iterateReactions(
  const Libnucnet__Reac *self,
  Libnucnet__Reaction__iterateFunction pf_iterator,
  void *p_user_data
)
{

  Libnucnet__Reaction **p_reactions;
  size_t i, i_reactions;

  if( !self ) LIBNUCNET__REAC__ERROR( "Invalid input" );

  p_reactions =
    Libnucnet__Reac__createReactionArray( self );

  i_reactions = Libnucnet__Reac__getNumberOfReactions( self );

  for( i = 0; i < i_reactions; i++ )
    if( pf_iterator( p_reactions[i], p_user_data ) == 0 ) break;

  free( p_reactions );

}

/*##############################################################################
// Libnucnet__Reac__writeToXmlFile()
//############################################################################*/

void
Libnucnet__Reac__writeToXmlFile(
  const Libnucnet__Reac *self, const char *s_output_xml_filename
)
{

  xmlDocPtr p_doc;
  xmlChar *sx_str, *sx_str2;

  /*============================================================================
  // Check input structure.
  //==========================================================================*/

  if( !self ) {
     LIBNUCNET__REAC__ERROR( "Invalid input structure" );
  }

  /*============================================================================
  // Create the xml document.
  //==========================================================================*/

  p_doc = Libnucnet__Reac__makeXmlDocument( self );

  /*============================================================================
  // Create namespace attributes and add.
  //==========================================================================*/

  sx_str = xmlCharStrdup( W3C__NAMESPACE );
  sx_str2 = xmlCharStrdup( XSI );
  xmlNewNs( xmlDocGetRootElement( p_doc ), sx_str, sx_str2 );
  xmlFree( sx_str );
  xmlFree( sx_str2 );

  sx_str = xmlCharStrdup( XSI_SCHEMA_LOCATION );
  sx_str2 = xmlCharStrdup( LIBNUCNET__REAC__SCHEMALOCATION );
  xmlNewProp( xmlDocGetRootElement( p_doc ), sx_str, sx_str2 );
  xmlFree( sx_str );
  xmlFree( sx_str2 );

  /*============================================================================
  // Write file.
  //==========================================================================*/

  if(
       xmlSaveFormatFileEnc( s_output_xml_filename, p_doc, "UTF-8", 1 ) == -1
  ) {
      LIBNUCNET__REAC__ERROR(
        "xmlSaveFormatFileEnc: failed\n\tException #%d\n"
      );
  }

  /*============================================================================
  // Clean up.
  //==========================================================================*/

  xmlFreeDoc( p_doc );
  xmlCleanupParser();

}

/*##############################################################################
// Libnucnet__Reac__makeXmlDocument().
//############################################################################*/

xmlDocPtr
Libnucnet__Reac__makeXmlDocument(
  const Libnucnet__Reac *self
)
{

  xmlDocPtr p_doc;
  xmlNodePtr p_root;
  xmlChar *sx_str;

  /*============================================================================
  // Get document.
  //==========================================================================*/

  sx_str = xmlCharStrdup( XML_VERSION );
  p_doc = xmlNewDoc( sx_str );
  xmlFree( sx_str );

  if (p_doc == NULL) {
    LIBNUCNET__REAC__ERROR( "DOMImplementation.createDocument: failed" );
  }

  /*============================================================================
  // Add root.
  //==========================================================================*/

  sx_str = xmlCharStrdup( REACTION_DATA );
  p_root = xmlNewNode( NULL, sx_str );
  xmlFree( sx_str );

  xmlDocSetRootElement( p_doc, p_root );

  /*============================================================================
  // Loop on reactions.
  //==========================================================================*/

  Libnucnet__Reac__iterateReactions(
    self,
    (Libnucnet__Reaction__iterateFunction)
       Libnucnet__Reaction__makeXmlDocumentIterator,
    p_root
  );

  /*============================================================================
  // Return.
  //==========================================================================*/

  return p_doc;

}

/*##############################################################################
// Libnucnet__Reac__makeXmlDocumentIterator().
//############################################################################*/

int
Libnucnet__Reaction__makeXmlDocumentIterator(
  Libnucnet__Reaction *self, xmlNodePtr p_root
)
{

  struct {
    xmlNodePtr pReactionNode;
    xmlChar *sxStr;
  } reaction_element_user_data;

  struct {
    xmlNodePtr pRateNode;
    xmlHashTablePtr pHash;
  } nsf_user_data;

  xmlListPtr p_list;
  xmlNodePtr p_comment, p_reaction_node, p_rate_node, p_point, p_properties;
  xmlChar *sx_str, *sx_str2;
  size_t i;

  /*---------------------------------------------------------------------------
  // Create comment and add.
  //--------------------------------------------------------------------------*/

  p_comment = xmlNewComment( self->sxReaction );

  xmlAddChild( p_root, p_comment );

  /*---------------------------------------------------------------------------
  // Get reaction node.
  //--------------------------------------------------------------------------*/

  sx_str = xmlCharStrdup( REACTION );
  p_reaction_node = xmlNewChild( p_root, NULL, sx_str, NULL );
  xmlFree( sx_str );

  if( p_reaction_node == NULL )
    LIBNUCNET__REAC__ERROR( "Document.createElement: NULL\n\tException" );

  /*---------------------------------------------------------------------------
  // Insert data source string if not empty.
  //--------------------------------------------------------------------------*/

  if( strlen( Libnucnet__Reaction__getSource( self ) ) != 0 ) {
   
    sx_str = xmlCharStrdup( REACTION_SOURCE );
    xmlNewChild(
      p_reaction_node,
      NULL,
      sx_str,
      self->sxSource
    );
    xmlFree( sx_str );

  }

  /*---------------------------------------------------------------------------
  // Insert reactants.
  //--------------------------------------------------------------------------*/

  reaction_element_user_data.pReactionNode = p_reaction_node;
  reaction_element_user_data.sxStr = xmlCharStrdup( REACTANT );

  if( !reaction_element_user_data.sxStr )
    LIBNUCNET__REAC__ERROR( "Couldn't allocate memory" );

  xmlListWalk(
    self->pReactantList,
    (xmlListWalker) Libnucnet__Reac__makeXmlDocumentReactionElementWalker,
    &reaction_element_user_data
  );

  xmlListWalk(
    self->pOtherReactantList,
    (xmlListWalker) Libnucnet__Reac__makeXmlDocumentReactionElementWalker,
    &reaction_element_user_data
  );

  xmlFree( reaction_element_user_data.sxStr );

  /*---------------------------------------------------------------------------
  // Insert products.
  //--------------------------------------------------------------------------*/

  reaction_element_user_data.pReactionNode = p_reaction_node;
  reaction_element_user_data.sxStr = xmlCharStrdup( PRODUCT );

  if( !reaction_element_user_data.sxStr )
    LIBNUCNET__REAC__ERROR( "Couldn't allocate memory" );

  xmlListWalk(
    self->pProductList,
    (xmlListWalker) Libnucnet__Reac__makeXmlDocumentReactionElementWalker,
    &reaction_element_user_data
  );

  xmlListWalk(
    self->pOtherProductList,
    (xmlListWalker) Libnucnet__Reac__makeXmlDocumentReactionElementWalker,
    &reaction_element_user_data
  );

  xmlFree( reaction_element_user_data.sxStr );

  /*============================================================================
  // Insert rate data.
  //==========================================================================*/

  if( !self->sxFunctionKey )
    LIBNUCNET__REAC__ERROR( "No rate function name" );

  else if(
    !xmlStrcmp( self->sxFunctionKey, (const xmlChar *) SINGLE_RATE_STRING )
  )
  {

    /*-------------------------------------------------------------------------
    // Insert single rate.
    //------------------------------------------------------------------------*/

    sx_str2 = (xmlChar *) malloc( sizeof( xmlChar ) * REAC_BUF_SIZE );
    xmlStrPrintf(
      sx_str2,
      REAC_BUF_SIZE,
      (const xmlChar *) "%g",
      self->pRd->pSingle->dSingleRate
    );

    sx_str = xmlCharStrdup( SINGLE_RATE_STRING );

    p_rate_node =
      xmlNewChild(
        p_reaction_node,
        NULL,
        sx_str,
        sx_str2
      );

    xmlFree( sx_str );
    xmlFree( sx_str2 );

  }

    /*-------------------------------------------------------------------------
    // Insert rate table.
    //------------------------------------------------------------------------*/

  else if(
    !xmlStrcmp( self->sxFunctionKey, (const xmlChar *) RATE_TABLE_STRING )
  )
  {

    sx_str = xmlCharStrdup( RATE_TABLE_STRING );
    p_rate_node =
      xmlNewChild(
        p_reaction_node,
        NULL,
        sx_str,
        NULL
      );
    xmlFree( sx_str );

    for( i = 0; i < self->pRd->pRt->pT9->size; i++ ) {

      sx_str = xmlCharStrdup( REAC_POINT );
      p_point =
        xmlNewChild(
          p_rate_node,
          NULL,
          sx_str,
          NULL
        );
      xmlFree( sx_str );

      sx_str2 = (xmlChar *) malloc( sizeof( xmlChar ) * REAC_BUF_SIZE );
      xmlStrPrintf(
        sx_str2,
        REAC_BUF_SIZE,
        (const xmlChar *) "%g",
        gsl_vector_get( self->pRd->pRt->pT9, i )
      );

      sx_str = xmlCharStrdup( RATE_TABLE_T9_NODE );

      xmlNewChild(
         p_point,
         NULL,
         sx_str,
         sx_str2
      );

      xmlFree( sx_str );

      xmlFree( sx_str2 );

      sx_str2 = (xmlChar *) malloc( sizeof( xmlChar ) * REAC_BUF_SIZE );
      xmlStrPrintf(
        sx_str2,
        REAC_BUF_SIZE,
        (const xmlChar *) "%g",
        gsl_vector_get( self->pRd->pRt->pRate, i )
      );

      sx_str = xmlCharStrdup( RATE_TABLE_ENTRY_NODE );

      xmlNewChild(
         p_point,
         NULL,
         sx_str,
         sx_str2
      );

      xmlFree( sx_str );

      xmlFree( sx_str2 );

      sx_str2 = (xmlChar *) malloc( sizeof( xmlChar ) * REAC_BUF_SIZE );
      xmlStrPrintf(
        sx_str2,
        REAC_BUF_SIZE,
        (const xmlChar *) "%g",
        gsl_vector_get( self->pRd->pRt->pSef, i )
      );

      sx_str = xmlCharStrdup( RATE_TABLE_SEF_NODE );

      xmlNewChild(
        p_point,
        NULL,
        sx_str,
        sx_str2
      );

      xmlFree( sx_str );
      xmlFree( sx_str2 );

    }

  }

    /*-------------------------------------------------------------------------
    // Insert non smoker rate data.
    //------------------------------------------------------------------------*/

  else if(
    !xmlStrcmp( self->sxFunctionKey, (const xmlChar *) NON_SMOKER_STRING )
  )
  {

    sx_str = xmlCharStrdup( NON_SMOKER_STRING );

    p_rate_node =
      xmlNewChild(
        p_reaction_node,
        NULL,
        sx_str,
        NULL
      );
    xmlFree( sx_str );

    nsf_user_data.pHash = self->pRd->pNsfHash;
    nsf_user_data.pRateNode = p_rate_node;

    xmlHashScan(
      self->pRd->pNsfHash,
      (xmlHashScanner) Libnucnet__Reaction__NonSmokerFit__makeXmlCallback,
      &nsf_user_data
    );

  }

    /*-------------------------------------------------------------------------
    // Otherwise user rate.
    //------------------------------------------------------------------------*/

  else
  {

    sx_str = xmlCharStrdup( USER_RATE_STRING );  
    p_rate_node =
      xmlNewChild(
        p_reaction_node,
        NULL,
        sx_str,
        NULL
      );
    xmlFree( sx_str );

    sx_str = xmlCharStrdup( FUNCTION_KEY );
    xmlNewProp(
       p_rate_node,
       sx_str,
       self->sxFunctionKey
    );
    xmlFree( sx_str );

    if( self->pRd )
    {
      if( xmlHashSize( self->pRd->pRateFunctionPropertyHash ) != 0 )
      {

        sx_str = xmlCharStrdup( PROPERTIES );
        p_properties =
          xmlNewChild(
            p_rate_node,
            NULL,
            sx_str,
            NULL
          ); 
        xmlFree( sx_str );

        p_list =
          Libnucnet__Reaction__createRateFunctionPropertyList( self );

        xmlListWalk(
          p_list,
          (xmlListWalker)
             Libnucnet__Reaction__xml_rate_function_property_walker,
          p_properties
        );

        xmlListDelete( p_list );

      }

    }

  }
       
  /*============================================================================
  // Done.  Return.
  //==========================================================================*/

  return 1;

}

/*##############################################################################
// Libnucnet__Reaction__xml_rate_function_property_walker().
//############################################################################*/

int
Libnucnet__Reaction__xml_rate_function_property_walker(
  void *p_data,
  xmlNodePtr p_node
)
{

  typedef struct {
    xmlChar *sxName;
    xmlChar *sxTag1;
    xmlChar *sxTag2;
    xmlChar *sxValue;
  } property_data;

  xmlChar *sx_str;
  xmlNodePtr p_property_node;

  property_data *p_property = ( property_data * ) p_data;

  sx_str = xmlCharStrdup( USER_RATE_PROPERTY );

  p_property_node =
    xmlNewChild(
      p_node,
      NULL,
      sx_str,
      p_property->sxValue
    );
  xmlFree( sx_str );

  sx_str = xmlCharStrdup( PROPERTY_NAME );
  xmlNewProp(
    p_property_node,
    sx_str,
    p_property->sxName
  );
  xmlFree( sx_str );

  if( p_property->sxTag1 )
  {
    sx_str = xmlCharStrdup( PROPERTY_TAG1 );
    xmlNewProp(
      p_property_node,
      sx_str,
      p_property->sxTag1
    );
    xmlFree( sx_str );
  }

  if( p_property->sxTag2 )
  {
    sx_str = xmlCharStrdup( PROPERTY_TAG2 );
    xmlNewProp(
      p_property_node,
      sx_str,
      p_property->sxTag2
    );
    xmlFree( sx_str );
  }

  return 1;

}

/*##############################################################################
// Libnucnet__Reac__makeXmlDocumentReactionElementWalker().
//############################################################################*/

int
Libnucnet__Reac__makeXmlDocumentReactionElementWalker(
  Libnucnet__Reaction__Element *p_element, void *p_data
)
{

  struct user_data {
    xmlNodePtr pReactionNode;
    xmlChar *sxStr;
  };

  struct user_data *p_user_data = ( struct user_data * ) p_data;

  xmlNewChild(
    p_user_data->pReactionNode, 
    NULL,
    p_user_data->sxStr,
    p_element->sxName
  );

  return 1;

}

/*##############################################################################
// Libnucnet__Reaction__getParentDuplicate().
//############################################################################*/

Libnucnet__Reaction *
Libnucnet__Reaction__getParentDuplicate(
  const Libnucnet__Reaction *self
)
{

  return self->pParentDuplicate;

}

/*##############################################################################
// Libnucnet__Reaction__isWeak().
//############################################################################*/

int
Libnucnet__Reaction__isWeak( const Libnucnet__Reaction *self )
{

  int i_weak = 0;

  xmlListWalk(
    self->pOtherReactantList,
    (xmlListWalker) Libnucnet__Reaction__isWeakWalker,
    &i_weak
  );

  if( i_weak == 1 ) return i_weak;

  xmlListWalk(
    self->pOtherProductList,
    (xmlListWalker) Libnucnet__Reaction__isWeakWalker,
    &i_weak
  );

  return i_weak;

}

/*##############################################################################
// Libnucnet__Reaction__isWeakWalker().
//############################################################################*/

int
Libnucnet__Reaction__isWeakWalker(
  Libnucnet__Reaction__Element *p_element, int *p_weak
)
{
 
  if(
      strcmp( (char *) p_element->sxName, ELECTRON ) == 0 ||
      strcmp( (char *) p_element->sxName, POSITRON ) == 0 ||
      strcmp( (char *) p_element->sxName, MU ) == 0 ||
      strcmp( (char *) p_element->sxName, ANTIMU ) == 0 ||
      strcmp( (char *) p_element->sxName, TAU ) == 0 ||
      strcmp( (char *) p_element->sxName, ANTITAU ) == 0 ||
      strcmp( (char *) p_element->sxName, ELECTRON_NEUTRINO ) == 0 ||
      strcmp( (char *) p_element->sxName, ELECTRON_ANTINEUTRINO ) == 0 ||
      strcmp( (char *) p_element->sxName, MU_NEUTRINO ) == 0 ||
      strcmp( (char *) p_element->sxName, MU_ANTINEUTRINO ) == 0 ||
      strcmp( (char *) p_element->sxName, TAU_NEUTRINO ) == 0 ||
      strcmp( (char *) p_element->sxName, TAU_ANTINEUTRINO ) == 0
  )
  {
    *p_weak = 1;
    return 0;
  }

  return 1;

}

/*##############################################################################
// Libnucnet__Reaction__isBetaPlus().
//############################################################################*/

int
Libnucnet__Reaction__isBetaPlus( const Libnucnet__Reaction *self )
{

  int i_beta_plus_reactants = 0;

  xmlListWalk(
    self->pOtherProductList,
    (xmlListWalker) Libnucnet__Reaction__isBetaPlusWalker,
    &i_beta_plus_reactants
  );

  if( i_beta_plus_reactants == 2 )
    return 1;
  else
    return 0;

}

/*##############################################################################
// Libnucnet__Reaction__isBetaPlusWalker().
//############################################################################*/

int
Libnucnet__Reaction__isBetaPlusWalker(
  Libnucnet__Reaction__Element *p_element, int *p_beta_plus_reactants
)
{

  if( !strcmp( (char *) p_element->sxName, ELECTRON_NEUTRINO ) )
    (*p_beta_plus_reactants)++;

  if( !strcmp( (char *) p_element->sxName, POSITRON ) )
    (*p_beta_plus_reactants)++;

  return 1;

}
  
/*##############################################################################
// Libnucnet__Reaction__isPositronCapture().
//############################################################################*/

int
Libnucnet__Reaction__isPositronCapture( const Libnucnet__Reaction *self )
{

  int i_positron_capture_reactants = 0, i_positron_capture_products = 0;

  xmlListWalk(
    self->pOtherReactantList,
    (xmlListWalker) Libnucnet__Reaction__isPositronCaptureWalker,
    &i_positron_capture_reactants
  );

  if( i_positron_capture_reactants != 1 )
    return 0;

  xmlListWalk(
    self->pOtherProductList,
    (xmlListWalker) Libnucnet__Reaction__isPositronCaptureWalker,
    &i_positron_capture_products
  );

  if( i_positron_capture_products == 1 )
    return 1;
  else
    return 0;

}

/*##############################################################################
// Libnucnet__Reaction__isPositronCaptureWalker().
//############################################################################*/

int
Libnucnet__Reaction__isPositronCaptureWalker(
  Libnucnet__Reaction__Element *p_element, int *p_positron_capture_reactants
)
{

  if( !strcmp( (char *) p_element->sxName, ELECTRON_ANTINEUTRINO ) )
    (*p_positron_capture_reactants)++;

  if( !strcmp( (char *) p_element->sxName, POSITRON ) )
    (*p_positron_capture_reactants)++;

  return 1;

}
  
/*##############################################################################
// Libnucnet__Reaction__iterateReactants().
//############################################################################*/

void
Libnucnet__Reaction__iterateReactants(
  const Libnucnet__Reaction *self,
  Libnucnet__Reaction__Element__iterateFunction pf_func,
  void *p_user_data
)
{

  typedef struct {
    Libnucnet__Reaction__Element__iterateFunction pfFunc;
    void *pUserData;
  } extra_data;

  extra_data *p_extra_data;

  if( !self || !pf_func )
    LIBNUCNET__REAC__ERROR( "Invalid input" );

  p_extra_data = ( extra_data * ) malloc( sizeof( extra_data ) );

  if( !p_extra_data )
    LIBNUCNET__REAC__ERROR( "Couldn't allocate memory" );

  p_extra_data->pfFunc = pf_func;
  p_extra_data->pUserData = p_user_data;

  xmlListWalk(
    self->pReactantList,
    (xmlListWalker) Libnucnet__Reaction__Element__walker,
    p_extra_data
  );

  xmlListWalk(
    self->pOtherReactantList,
    (xmlListWalker) Libnucnet__Reaction__Element__walker,
    p_extra_data
  );

  free( p_extra_data );

}

/*##############################################################################
// Libnucnet__Reaction__iterateProducts().
//############################################################################*/

void
Libnucnet__Reaction__iterateProducts(
  const Libnucnet__Reaction *self,
  Libnucnet__Reaction__Element__iterateFunction pf_func,
  void *p_user_data
)
{

  typedef struct {
    Libnucnet__Reaction__Element__iterateFunction pfFunc;
    void *pUserData;
  } extra_data;

  extra_data *p_extra_data;

  if( !self || !pf_func )
    LIBNUCNET__REAC__ERROR( "Invalid input" );

  p_extra_data = ( extra_data * ) malloc( sizeof( extra_data ) );

  if( !p_extra_data )
    LIBNUCNET__REAC__ERROR( "Couldn't allocate memory" );

  p_extra_data->pfFunc = pf_func;
  p_extra_data->pUserData = p_user_data;

  xmlListWalk(
    self->pProductList,
    (xmlListWalker) Libnucnet__Reaction__Element__walker,
    p_extra_data
  );

  xmlListWalk(
    self->pOtherProductList,
    (xmlListWalker) Libnucnet__Reaction__Element__walker,
    p_extra_data
  );

  free( p_extra_data );

}

/*##############################################################################
// Libnucnet__Reaction__Element__walker().
//############################################################################*/

int
Libnucnet__Reaction__Element__walker(
  Libnucnet__Reaction__Element *self,
  void *p_data
)
{

  typedef struct {
    Libnucnet__Reaction__Element__iterateFunction pfFunc;
    void *pUserData;
  } extra_data;

  extra_data *p_extra_data = ( extra_data * ) p_data;

  return
    p_extra_data->pfFunc(
      self,
      p_extra_data->pUserData
  );

}

/*##############################################################################
// Libnucnet__Reaction__Element__getName().
//############################################################################*/

const char *
Libnucnet__Reaction__Element__getName(
  const Libnucnet__Reaction__Element *self
)
{

  if( !self ) LIBNUCNET__REAC__ERROR( "Invalid input" );

  return
    ( const char * ) self->sxName;

}
  
/*##############################################################################
// Libnucnet__Reac__extractSubset().
//############################################################################*/

Libnucnet__Reac *
Libnucnet__Reac__extractSubset(
  const Libnucnet__Reac *self,
  const char *s_xpath
)
{

  Libnucnet__Reac *p_reac;
  xmlDocPtr p_doc;

  /*============================================================================
  // Check input.
  //==========================================================================*/

  if( !self )
    LIBNUCNET__REAC__ERROR( "Invalid input" );

  /*============================================================================
  // Create structure.
  //==========================================================================*/

  p_reac = Libnucnet__Reac__new();

  /*============================================================================
  // Create document.
  //==========================================================================*/

  p_doc = Libnucnet__Reac__makeXmlDocument( self );

  /*============================================================================
  // Extract subset.
  //==========================================================================*/

  Libnucnet__Reac__updateFromXmlDocument( p_reac, p_doc, s_xpath );

  /*============================================================================
  // Free document and return.
  //==========================================================================*/

  xmlFreeDoc( p_doc );
  xmlCleanupParser();

  return p_reac;

}

/*##############################################################################
// Libnucnet__Reac__createReactionArray().
//############################################################################*/

Libnucnet__Reaction **
Libnucnet__Reac__createReactionArray(
  const Libnucnet__Reac *self
)
{

  struct {
    size_t iIndex;
    Libnucnet__Reaction **pReactions;
  } work_data;

  if( !self )
    LIBNUCNET__REAC__ERROR( "Invalid input" );

  work_data.pReactions =
    ( Libnucnet__Reaction ** )
    malloc(
      sizeof( Libnucnet__Reaction ) *
      Libnucnet__Reac__getNumberOfReactions( self )
    );

  if( !work_data.pReactions )
    LIBNUCNET__REAC__ERROR( "Couldn't allocate memory" );

  work_data.iIndex = 0;

  xmlHashScan(
    self->pReactionHash,
    (xmlHashScanner) Libnucnet__Reac__createReactionArrayCallback,
    &work_data
  );

  if( self->pfReactionCompare )
    Libnucnet__Reac__sortReactionArray( self, work_data.pReactions );

  return work_data.pReactions;

}
  
/*##############################################################################
// Libnucnet__Reac__sort_helper().
//############################################################################*/

int
Libnucnet__Reac__sort_helper(
  const void *p_1, const void *p_2
)
{

  typedef struct {
    Libnucnet__Reaction *pReaction;
    Libnucnet__Reaction__compare_function pfFunc;
  } sort_data;

  sort_data *p_sort_1 = *( sort_data * const * ) p_1;
  sort_data *p_sort_2 = *( sort_data * const * ) p_2;

  return
    p_sort_1->pfFunc(
      p_sort_1->pReaction,
      p_sort_2->pReaction
    );

}

/*##############################################################################
// Libnucnet__Reac__sortReactionArray().
//############################################################################*/

void
Libnucnet__Reac__sortReactionArray(
  const Libnucnet__Reac *self,
  Libnucnet__Reaction **p_reactions
)
{

  typedef struct {
    Libnucnet__Reaction *pReaction;
    Libnucnet__Reaction__compare_function pfFunc;
  } sort_data;
 
  size_t i;
  sort_data **p_sort_array;

  p_sort_array =
    (sort_data **)
    malloc(
      sizeof( sort_data * ) * Libnucnet__Reac__getNumberOfReactions( self )
    );

  for( i = 0; i < Libnucnet__Reac__getNumberOfReactions( self ); i++ )
  {
    p_sort_array[i] = (sort_data *) malloc( sizeof( sort_data ) );
    p_sort_array[i]->pReaction = p_reactions[i];
    p_sort_array[i]->pfFunc = self->pfReactionCompare;
  }

  qsort(
    p_sort_array,
    Libnucnet__Reac__getNumberOfReactions( self ),
    sizeof( sort_data * ),
    Libnucnet__Reac__sort_helper
  );

  for( i = 0; i < Libnucnet__Reac__getNumberOfReactions( self ); i++ )
  {
    p_reactions[i] = p_sort_array[i]->pReaction;
    free( p_sort_array[i] );
  }

  free( p_sort_array );

}

/*##############################################################################
// Libnucnet__Reac__createReactionArrayCallback().
//############################################################################*/

void
Libnucnet__Reac__createReactionArrayCallback(
  Libnucnet__Reaction *p_reaction,
  void *p_data,
  const xmlChar *sx_reaction
)
{

  typedef struct {
    size_t iIndex;
    Libnucnet__Reaction **pReactions;
  } work_data;

  work_data *p_work_data = ( work_data * ) p_data;

  if( !sx_reaction )
    LIBNUCNET__REAC__ERROR( "No such reaction" );

  p_work_data->pReactions[p_work_data->iIndex++] = p_reaction;

}

/*##############################################################################
// Libnucnet__Reac__setReactionCompareFunction().
//############################################################################*/

void
Libnucnet__Reac__setReactionCompareFunction(
  Libnucnet__Reac *self,
  Libnucnet__Reaction__compare_function pf_func
)
{

  self->pfReactionCompare = pf_func;

}

/*##############################################################################
// Libnucnet__Reac__clearReactionCompareFunction().
//############################################################################*/

void
Libnucnet__Reac__clearReactionCompareFunction(
  Libnucnet__Reac *self
)
{

  self->pfReactionCompare = NULL;

}

/*##############################################################################
// Libnucnet__Reac__registerRateFunction().
//############################################################################*/

int
Libnucnet__Reac__registerRateFunction(
  Libnucnet__Reac *self,
  const char *s_function_key,
  Libnucnet__Reaction__userRateFunction pf_func
)
{

  int i_result;
  Libnucnet__Reac__FD *p_func;

  if( !self ) LIBNUCNET__REAC__ERROR( "Invalid input" );

  p_func = ( Libnucnet__Reac__FD * ) malloc( sizeof( Libnucnet__Reac__FD ) );

  if( !p_func ) LIBNUCNET__REAC__ERROR( "Couldn't allocate memory" );

  p_func->pfFunc = pf_func;

  p_func->pfDeallocator = NULL;

  i_result =
    xmlHashUpdateEntry(
      self->pFDHash,
      (const xmlChar *) s_function_key,
      p_func,
      (xmlHashDeallocator) free
    ) + 1;

  if( i_result ) self->iUpdate++;

  return i_result;

}

/*##############################################################################
// Libnucnet__Reac__registerUserRateFunction().
//############################################################################*/

int
Libnucnet__Reac__registerUserRateFunction(
  Libnucnet__Reac *self,
  const char *s_function_key,
  Libnucnet__Reaction__userRateFunction pf_func
)
{

  return
    Libnucnet__Reac__registerRateFunction(
      self,
      s_function_key,
      pf_func
    );

}

/*##############################################################################
// Libnucnet__Reac__setUserRateFunctionDataDeallocator().
//############################################################################*/

int
Libnucnet__Reac__setUserRateFunctionDataDeallocator(
  Libnucnet__Reac * self,
  const char * s_function_key,
  Libnucnet__Reaction__user_rate_function_data_deallocator pf_deallocator
)
{

  Libnucnet__Reac__FD *p_fd;

  if( !self ) LIBNUCNET__REAC__ERROR( "Invalid reaction collection" );

  p_fd =
    ( Libnucnet__Reac__FD * )
    xmlHashLookup(
      self->pFDHash,
      (const xmlChar *) s_function_key
    );

  if( !p_fd ) return 0;
  
  p_fd->pfDeallocator = pf_deallocator;

  return 1;

}

/*##############################################################################
// Libnucnet__Reaction__setUserRateFunctionKey().
//############################################################################*/

void
Libnucnet__Reaction__setUserRateFunctionKey(
  Libnucnet__Reaction *self,
  const char *s_function_key
)
{

  if( !self ) LIBNUCNET__REAC__ERROR( "No such reaction" );

  if( self->sxFunctionKey )
    LIBNUCNET__REAC__ERROR( "Function already set" );

  self->sxFunctionKey = xmlCharStrdup( s_function_key );

  if( !self->sxFunctionKey )
    LIBNUCNET__REAC__ERROR( "Couldn't allocate memory" );

  self->pRd =
    (Libnucnet__Reaction__RateData *)
    malloc( sizeof( Libnucnet__Reaction__RateData ) );

  if( !self->pRd ) LIBNUCNET__REAC__ERROR( "Couldn't allocate memory" );

  self->pRd->pRateFunctionPropertyHash = xmlHashCreate( 0 );

  if( !self->pRd->pRateFunctionPropertyHash )
    LIBNUCNET__REAC__ERROR( "Couldn't allocate hash" );

}
    
/*##############################################################################
// Libnucnet__Reaction__updateUserRateFunctionProperty().
//############################################################################*/

int
Libnucnet__Reaction__updateUserRateFunctionProperty(
  Libnucnet__Reaction *self,
  const char *s_property_name,
  const char *s_tag1,
  const char *s_tag2,
  const char *s_value
)
{

  xmlChar *sx_value;

  if( !self ) LIBNUCNET__REAC__ERROR( "Invalid input" );

  if( !Libnucnet__Reaction__hasUserDefinedRateFunction( self ) )
    return 0;

  sx_value = xmlCharStrdup( s_value );

  if( !sx_value ) LIBNUCNET__REAC__ERROR( "Couldn't allocate memory" );

  if( !self->pRd )
    LIBNUCNET__REAC__ERROR( "User rate function name must first be set" );

  if(
     (
       xmlHashUpdateEntry3(
         self->pRd->pRateFunctionPropertyHash,
         (const xmlChar *) s_property_name,
         (const xmlChar *) s_tag1,
         (const xmlChar *) s_tag2,
         sx_value,
         (xmlHashDeallocator) xmlFree
       ) == 0
     )
  )
    return 1;
  else
  {
    xmlFree( sx_value );
    return 0;
  }
    
}

/*##############################################################################
// Libnucnet__Reaction__removeUserRateFunctionProperty().
//############################################################################*/

int
Libnucnet__Reaction__removeUserRateFunctionProperty(
  Libnucnet__Reaction *self,
  const char *s_property_name,
  const char *s_tag1,
  const char *s_tag2
)
{

  if( !self ) LIBNUCNET__REAC__ERROR( "Invalid input" );

  if( !self->pRd )
    LIBNUCNET__REAC__ERROR( "User rate function name must first be set" );

  return
    xmlHashRemoveEntry3(
      self->pRd->pRateFunctionPropertyHash,
      (const xmlChar *) s_property_name,
      (const xmlChar *) s_tag1,
      (const xmlChar *) s_tag2,
      (xmlHashDeallocator) xmlFree
    ) + 1;
    
}

/*##############################################################################
// Libnucnet__Reaction__getUserRateFunctionProperty()
//############################################################################*/

const char *
Libnucnet__Reaction__getUserRateFunctionProperty(
  const Libnucnet__Reaction *self,
  const char *s_name,
  const char *s_tag1,
  const char *s_tag2
)
{

  if( !Libnucnet__Reaction__hasUserDefinedRateFunction( self ) )
    LIBNUCNET__REAC__ERROR(
      "Reaction does not have a user-defined rate function"
    );

  return
    (const char *)
    xmlHashLookup3(
      self->pRd->pRateFunctionPropertyHash,
      (const xmlChar *) s_name,
      (const xmlChar *) s_tag1,
      (const xmlChar *) s_tag2
    );

}
      
/*##############################################################################
// Libnucnet__Reaction__iterateUserRateFunctionProperties().
//############################################################################*/

void
Libnucnet__Reaction__iterateUserRateFunctionProperties(
  const Libnucnet__Reaction *self,
  const char *s_name,
  const char *s_tag1,
  const char *s_tag2,
  Libnucnet__Reaction__user_rate_property_iterate_function pf_func,
  void *p_user_data
)
{

  typedef struct {
    Libnucnet__Reaction__user_rate_property_iterate_function pfFunc;
    void *pUserData;
  } work_data;

  work_data *p_work;

  if( !Libnucnet__Reaction__hasUserDefinedRateFunction( self ) )
    LIBNUCNET__REAC__ERROR( "Not a user-defined function" );

  p_work = ( work_data * ) malloc( sizeof( work_data ) );

  p_work->pfFunc = pf_func;
  p_work->pUserData = p_user_data;

  xmlHashScan3(
    self->pRd->pRateFunctionPropertyHash,
    (const xmlChar *) s_name,
    (const xmlChar *) s_tag1,
    (const xmlChar *) s_tag2,
    (xmlHashScanner)
       Libnucnet__Reaction__iterateUserRateFunctionPropertiesHelper,
    p_work
  );

  free( p_work );

}

/*##############################################################################
// Libnucnet__Reaction__iterateUserRateFunctionPropertiesHelper().
//############################################################################*/

void
Libnucnet__Reaction__iterateUserRateFunctionPropertiesHelper(
  const xmlChar *sx_value,
  void *p_data,
  const xmlChar *sx_name,
  const xmlChar *sx_tag1,
  const xmlChar *sx_tag2
)
{

  typedef struct {
    Libnucnet__Reaction__user_rate_property_iterate_function pfFunc;
    void *pUserData;
  } work_data;

  work_data *p_work = ( work_data *) p_data;

  p_work->pfFunc(
    (const char *) sx_name,
    (const char *) sx_tag1,
    (const char *) sx_tag2,
    (const char *) sx_value,
    p_work->pUserData
  );

}

/*##############################################################################
// Libnucnet__Reaction__getRateFunctionKey().
//############################################################################*/

const char *
Libnucnet__Reaction__getRateFunctionKey(
  const Libnucnet__Reaction *self
)
{

  return (const char *) self->sxFunctionKey;

}
  
/*##############################################################################
// Libnucnet__Reac__is_user_defined_rate_function().
//############################################################################*/

int
Libnucnet__Reac__is_user_defined_rate_function(
  const char *s_function_key
)
{

  if(
    strcmp( s_function_key, NON_SMOKER_STRING ) &&
    strcmp( s_function_key, RATE_TABLE_STRING ) &&
    strcmp( s_function_key, SINGLE_RATE_STRING )
  )
    return 1;
  else
    return 0;

}
/*##############################################################################
// Libnucnet__Reaction__hasUserDefinedRateFunction().
//############################################################################*/

int
Libnucnet__Reaction__hasUserDefinedRateFunction(
  const Libnucnet__Reaction *self
)
{

  return
    Libnucnet__Reac__is_user_defined_rate_function(
      Libnucnet__Reaction__getRateFunctionKey( self )
    );

}

/*##############################################################################
// Libnucnet__ReacView__new().
//############################################################################*/

Libnucnet__ReacView *
Libnucnet__ReacView__new(
  const Libnucnet__Reac * self,
  const char * s_reac_xpath
)
{

  xmlDocPtr p_doc;
  Libnucnet__ReacView * p_view;

  p_view =
    ( Libnucnet__ReacView * ) malloc( sizeof( Libnucnet__ReacView ) );

  if( !p_view ) LIBNUCNET__REAC__ERROR( "Couldn't allocate memory for view" );

  if( s_reac_xpath )
    p_view->sxXPath = xmlCharStrdup( s_reac_xpath );
  else
    p_view->sxXPath = xmlCharStrdup( REAC_EMPTY_STRING );

  p_view->pReac = Libnucnet__Reac__createNewReacForView( self );

  p_doc = Libnucnet__Reac__makeXmlDocument( self );

  Libnucnet__Reac__addReactionsToViewFromXml(
    self,
    p_view->pReac->pReactionHash,
    p_doc,
    s_reac_xpath
  );

  xmlFreeDoc( p_doc );

  return p_view;

}
  
/*##############################################################################
// Libnucnet__Reac__createNewReacForView().
//############################################################################*/

Libnucnet__Reac *
Libnucnet__Reac__createNewReacForView(
  const Libnucnet__Reac *self
)
{

  Libnucnet__Reac *p_new_reac;

  p_new_reac = Libnucnet__Reac__new();

  p_new_reac->iUpdate = self->iUpdate;
  p_new_reac->iOwner = 0;
  p_new_reac->pfReactionCompare = self->pfReactionCompare;

  xmlHashFree(
    p_new_reac->pFDHash,
    (xmlHashDeallocator) free
  );
  p_new_reac->pFDHash = self->pFDHash;

  return p_new_reac;

}

/*##############################################################################
// Libnucnet__ReacView__free().
//############################################################################*/

void
Libnucnet__ReacView__free(
  Libnucnet__ReacView * self
)
{

  if( !self ) return;

  Libnucnet__Reac__free( self->pReac );
  xmlFree( self->sxXPath );
 
  free( self );

}

/*##############################################################################
// Libnucnet__Reac__addReactionsToViewFromXml().
//############################################################################*/

void
Libnucnet__Reac__addReactionsToViewFromXml(
  const Libnucnet__Reac *self,
  xmlHashTablePtr p_hash,
  xmlDocPtr p_doc,
  const char *s_xpath_suffix
)
{

  xmlXPathContextPtr p_xpathCtx = NULL;
  xmlXPathObjectPtr p_xpathObj = NULL;
  xmlChar *sx_xpath, *sx_xpath_suffix;
  Libnucnet__Reaction * p_reaction;
  int i;

  /*============================================================================
  // Reaction iteration
  //==========================================================================*/

  sx_xpath = xmlCharStrdup( XPATH_REAC );

  if( !sx_xpath ) {
     LIBNUCNET__REAC__ERROR( "Couldn't allocate memory for xpath string" );
  }

  if( s_xpath_suffix ) {

     sx_xpath_suffix = xmlCharStrdup( s_xpath_suffix );

     sx_xpath = xmlStrcat( sx_xpath, sx_xpath_suffix );
  
     if( !sx_xpath ) {
        LIBNUCNET__REAC__ERROR( "Couldn't allocate memory for xpath string" );
     }

     xmlFree( sx_xpath_suffix );

  }

  p_xpathCtx = xmlXPathNewContext( p_doc );
  p_xpathObj = xmlXPathEvalExpression( sx_xpath, p_xpathCtx );
  xmlFree( sx_xpath );

  if( !p_xpathObj ) {
    LIBNUCNET__REAC__ERROR( "Invalid xpath expression" );
  }

  if( !xmlXPathNodeSetIsEmpty( p_xpathObj->nodesetval ) ) {

    for( i = 0; i < p_xpathObj->nodesetval->nodeNr; i++ ) {

      p_reaction =
        Libnucnet__Reac__new_reaction_from_xml_node(
          p_xpathObj->nodesetval->nodeTab[i]
        );

      xmlHashAddEntry(
        p_hash,
        p_reaction->sxReaction,
        Libnucnet__Reac__getReactionByString(
          self,
          Libnucnet__Reaction__getString( p_reaction )
        )
      );

      Libnucnet__Reaction__free( p_reaction );

    }

  }

  /*============================================================================
  // Clean up
  //==========================================================================*/

  xmlXPathFreeObject( p_xpathObj );
  xmlXPathFreeContext( p_xpathCtx );

}

/*##############################################################################
// Libnucnet__ReacView__getReac().
//############################################################################*/

Libnucnet__Reac *
Libnucnet__ReacView__getReac(
  Libnucnet__ReacView * self
)
{

  if( !self ) LIBNUCNET__REAC__ERROR( "Invalid view" );

  return self->pReac;

}

/*##############################################################################
// Libnucnet__ReacView__getXPath().
//############################################################################*/

const char *
Libnucnet__ReacView__getXPath(
  Libnucnet__ReacView * self
)
{

  if( !self ) LIBNUCNET__REAC__ERROR( "Invalid view" );

  return (const char *) self->sxXPath;

}
