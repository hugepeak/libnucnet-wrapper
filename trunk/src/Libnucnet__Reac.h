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
// </file>
//////////////////////////////////////////////////////////////////////////////*/

#ifndef LIBNUCNET_REAC_H
#define LIBNUCNET_REAC_H

/*##############################################################################
// Standard library #include's.
//############################################################################*/

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <math.h>
#include <string.h>

/*##############################################################################
// libxml2 #includes.
//############################################################################*/

#include <libxml/hash.h>
#include <libxml/list.h>
#include <libxml/parser.h>
#include <libxml/schemasInternals.h>
#include <libxml/tree.h>
#include <libxml/xpath.h>
#include <libxml/xpathInternals.h>
#include <libxml/xmlschemas.h>
#include <libxml/xmlschemastypes.h>

/*##############################################################################
// GSL #includes
//############################################################################*/

#include <gsl/gsl_errno.h>
#include <gsl/gsl_math.h>
#include <gsl/gsl_sort.h>
#include <gsl/gsl_sort_vector.h>
#include <gsl/gsl_spline.h>
#include <gsl/gsl_vector.h>
#include <gsl/gsl_sf.h>

/*##############################################################################
// Internal definitions.
//############################################################################*/

#define REAC_BUF_SIZE 256

/*##############################################################################
// Macro definitions.
//############################################################################*/

/* To debug, uncomment the following line. */
/* #define debug_reac */

#ifndef debug_reac

#define LIBNUCNET__REAC__ERROR( s_reason ) \
       do { \
         fprintf( stderr, "ERROR: %s in %s on line %d\n", \
           s_reason, __FILE__, __LINE__ \
         ); \
         exit( EXIT_FAILURE ); \
       } while (0)

#else

#define LIBNUCNET__REAC__ERROR( s_reason ) \
       do { \
         fprintf( stderr, "ERROR: %s in %s on line %d\n", \
           s_reason, __FILE__, __LINE__ \
         ); \
         abort(); \
       } while (0)

#endif


/*##############################################################################
// Some #defines.
//############################################################################*/

#define D_THIGHFIT_DEFAULT 10.
#define LARGE 1.e300
#define TINY 1.e-300
#define I_NSF 8

#define GAMMA                 "gamma"
#define ELECTRON              "electron"
#define POSITRON              "positron"
#define MU                    "mu"
#define ANTIMU                "anti-mu"
#define TAU                   "tau"
#define ANTITAU               "anti-tau"
#define ELECTRON_NEUTRINO     "neutrino_e"
#define ELECTRON_ANTINEUTRINO "anti-neutrino_e"
#define MU_NEUTRINO           "neutrino_mu"
#define MU_ANTINEUTRINO       "anti-neutrino_mu"
#define TAU_NEUTRINO          "neutrino_tau"
#define TAU_ANTINEUTRINO      "anti-neutrino_tau"

/*##############################################################################
// String defines.
//############################################################################*/

#define XML_VERSION "1.0"
#define REAC_XSD_VERSION "2011-05-28"

#define ACC "acc"
#define FIT "fit"
#define FUNCTION_KEY "key"
#define FUNCTION_UPDATE "update"
#define NON_SMOKER_STRING "non_smoker_fit"
#define NON_SMOKER_FUNCTION Libnucnet__Reaction__computeNonSmokerRate
#define NOTE "note"
#define PROPERTY_TAG1 "tag1"
#define PROPERTIES "properties"
#define PRODUCT "product"
#define PROPERTY_NAME "name"
#define PROPERTY_TAG2 "tag2"
#define PROPERTY_VALUE "value"
#define RATE_TABLE_STRING "rate_table"
#define RATE_TABLE_FUNCTION Libnucnet__Reaction__computeRateFromTable
#define RATE_TABLE_T9 ".//t9"
#define RATE_TABLE_ENTRY ".//rate"
#define RATE_TABLE_SEF ".//sef"
#define RATE_TABLE_T9_NODE "t9"
#define RATE_TABLE_ENTRY_NODE "rate"
#define RATE_TABLE_SEF_NODE "sef"
#define REAC_EMPTY_STRING ""
#define REAC_POINT "point"
#define REACTANT "reactant"
#define REACTION "reaction"
#define REACTION_DATA "reaction_data"
#define REACTION_SOURCE "source"
#define SINGLE_RATE_STRING "single_rate"
#define SINGLE_RATE_FUNCTION Libnucnet__Reaction__getSingleRate
#define SPINF "spinf"
#define SPINT "spint"
#define S_PLUS " + "
#define S_TO " to "
#define TLOWFIT "Tlowfit"
#define THIGHFIT "Thighfit"
#define TLOWHF "TlowHf"
#define USER_RATE_STRING "user_rate"
#define USER_RATE_PROPERTY "property"
#define USER_RATE_PROPERTY_XPATH ".//" USER_RATE_PROPERTY
#define XPATH_REAC "//" REACTION
#define YES_REAC "yes"

#ifndef XSI
#define XSI "xsi"
#endif

#ifndef XSI_SCHEMA_LOCATION
#define XSI_SCHEMA_LOCATION "xsi:schemaLocation"
#endif

/*##############################################################################
// XML Namespace definitions.
//############################################################################*/

#ifndef W3C__NAMESPACE
#define W3C__NAMESPACE "http://www.w3.org/2001/XMLSchema-instance"
#endif

#define REAC__PREFIX \
  "http://libnucnet.sourceforge.net/xsd_pub/" REAC_XSD_VERSION "/"

#define REAC__SCHEMA "libnucnet__reac.xsd"
#define LIBNUCNET__REAC__SCHEMA REAC__PREFIX REAC__SCHEMA
#define REAC__NAMESPACE "libnucnet__reac/"
#define LIBNUCNET__REAC__NAMESPACE REAC__PREFIX REAC__NAMESPACE
#define LIBNUCNET__REAC__SCHEMALOCATION1 LIBNUCNET__REAC__NAMESPACE " "
#define LIBNUCNET__REAC__SCHEMALOCATION \
  LIBNUCNET__REAC__SCHEMALOCATION1 LIBNUCNET__REAC__SCHEMA

/*##############################################################################
// Enumerations.
//############################################################################*/

enum {
 REACTANT_TYPE, PRODUCT_TYPE
};

/*##############################################################################
// Forward declarations.
//############################################################################*/

typedef struct _Libnucnet__Reac Libnucnet__Reac;

typedef struct _Libnucnet__Reaction Libnucnet__Reaction;

/*##############################################################################
// User-defined routines and structures.
//############################################################################*/

typedef struct Libnucnet__Reaction__RateTable {
  gsl_vector *pT9;
  gsl_vector *pRate;
  gsl_vector *pSef;
} Libnucnet__Reaction__RateTable;

typedef struct Libnucnet__Reaction__NonSmokerFit {
  double *a;
  xmlChar *sxNote;
  double *pSpint;
  double *pSpinf;
  double *pTlowHf;
  double *pTlowfit;
  double *pThighfit;
  double *pAcc;
} Libnucnet__Reaction__NonSmokerFit;

typedef struct Libnucnet__Reaction__Single {
  double dSingleRate;
} Libnucnet__Reaction__Single;

typedef struct Libnucnet__Reaction__RateData {
  Libnucnet__Reaction__Single *pSingle;
  Libnucnet__Reaction__RateTable *pRt;
  xmlHashTablePtr pRateFunctionPropertyHash;
  xmlHashTablePtr pNsfHash;
} Libnucnet__Reaction__RateData;
  
/*##############################################################################
// <class name="Libnucnet__Reaction__Element">
//
//   <description>
//     <abstract>
//       Libnucnet__Reaction__Element is a structure that stores data for a
//       reaction element in a nuclear reaction network.  A reaction
//       element is either a reactant or product in the reaction.
//       The contents of the structure are not made public by the API.
//     </abstract>
//     <keywords>
//       nuclear, network, reactions, rates, xml, element
//     </keywords>
//   </description>
//
//   <authors>
//     <current>
//       <author userid="mbradle" start_date="2008/08/02"/>
//     </current>
//   </authors>
//
//   <compatibility>
//     Compiled successfully in gcc (GCC) 3.4.0.
//   </compatibility>
//
// </class>
//############################################################################*/

typedef struct Libnucnet__Reaction__Element {
  xmlChar *sxName;
} Libnucnet__Reaction__Element;

/*##############################################################################
// <user_routine name="Libnucnet__Reaction__userRateFunction()">
//
//   <description>
//     <abstract>
//       User-supplied routine to calculate the rate of a reaction.
//     </abstract>
//     <keywords>
//       Libnucnet, user, supplied, function, data, rate, reaction
//     </keywords>
//   </description>
//
//   <usage>
//
//     <calling_sequence>
// double
// Libnucnet__Reaction__userRateFunction(
//   Libnucnet__Reaction *self,
//   double d_t9,
//   void *p_user_data
// );
//     </calling_sequence>
//
//     <param
//       name="self"
//       kind="in,positional,required"
//     >
//       A pointer to a Libnucnet__Reaction.
//     </param>
//
//     <param
//       name="d_t9"
//       kind="in,positional,required"
//     >
//       A double giving the temperature (in billions of K) at which to
//       compute the rate.
//     </param>
//
//     <param
//       name="p_user_data"
//       kind="in,positional,required"
//     >
//       A pointer giving user-supplied data for the reaction rate
//       calculation.
//     </param>
//
//     <param
//       kind="return"
//       doc="result"
//     />
//
//     <doc kind="post" id="result">
//       User's routine must return the rate for the reaction at the
//       given temperature and for the extra data.
//     </doc>
//
//     <doc kind="bug" >
//     </doc>
//
//   </usage>
//
// </user_routine>
//############################################################################*/

typedef double ( *Libnucnet__Reaction__rateFunction ) (
  Libnucnet__Reaction *,
  double,
  void *
);

typedef Libnucnet__Reaction__rateFunction Libnucnet__Reaction__userRateFunction;

/*##############################################################################
// <user_routine
//   name="Libnucnet__Reaction__user_rate_function_data_deallocator()"
// >
//
//   <description>
//     <abstract>
//       User-supplied routine to deallocate memory for extra data to a
//       user-defined rate function.
//     </abstract>
//     <keywords>
//       Libnucnet, user, supplied, function, data, rate, reaction
//     </keywords>
//   </description>
//
//   <usage>
//
//     <calling_sequence>
// void
// Libnucnet__Reaction__user_rate_function_data_deallocator(
//   void * p_data
// );
//     </calling_sequence>
//
//     <param
//       name="p_data"
//       kind="in,positional,required"
//     >
//       A pointer to the data.
//     </param>
//
//     <param
//       kind="return"
//       doc="result"
//     />
//
//     <doc kind="post" id="result">
//       User's routine must free the memory for the extra data appropriate
//       for the user's rate function.
//     </doc>
//
//     <doc kind="bug" >
//     </doc>
//
//   </usage>
//
// </user_routine>
//############################################################################*/

typedef void ( *Libnucnet__Reaction__user_rate_function_data_deallocator ) (
  void *
);

/*##############################################################################
// <class name="Libnucnet__Reaction">
//
//   <description>
//     <abstract>
//       Libnucnet__Reaction is a structure that stores data for a
//       reaction in a nuclear reaction network.  Routines act on the
//       structure to retrieve data,
//       update data, or use data to compute reaction rates based on the
//       data. The contents of the structure are not made public by the API.
//     </abstract>
//     <keywords>
//       nuclear, network, reactions, rates, xml, gdome
//     </keywords>
//   </description>
//
//   <authors>
//     <current>
//       <author userid="dcadams" start_date="2005/07/18"/>
//       <author userid="mbradle" start_date="2005/07/18"/>
//     </current>
//   </authors>
//
//   <compatibility>
//     Compiled successfully in gcc (GCC) 3.4.0.
//   </compatibility>
//
// </class>
//############################################################################*/

struct _Libnucnet__Reaction {
  Libnucnet__Reac *pReac;
  xmlChar *sxSource;
  xmlChar *sxReaction;
  xmlListPtr pReactantList;
  xmlListPtr pOtherReactantList;
  xmlListPtr pProductList;
  xmlListPtr pOtherProductList;
  Libnucnet__Reaction__RateData *pRd;
  double dDuplicateReactantFactor;
  double dDuplicateProductFactor;
  Libnucnet__Reaction *pParentDuplicate;
  xmlChar *sxFunctionKey;
};

/*##############################################################################
// <user_routine name="Libnucnet__Reaction__compare_function()">
//
//   <description>
//     <abstract>
//       User-supplied routine to sort reactions during a reaction
//       iteration.
//     </abstract>
//     <keywords>
//       Libnucnet, user, supplied, function, data, compare, reaction
//     </keywords>
//   </description>
//
//   <usage>
//
//     <calling_sequence>
// int
// Libnucnet__Reaction__compare_function(
//   const Libnucnet__Reaction *p_reaction1,
//   const Libnucnet__Reaction *p_reaction2
// );
//     </calling_sequence>
//
//     <param
//       name="p_reaction1"
//       kind="in,positional,required"
//     >
//       A pointer to the first reaction.
//     </param>
//
//     <param
//       name="p_reaction2"
//       kind="in,positional,required"
//     >
//       A pointer to the second reaction.
//     </param>
//
//     <param
//       kind="return"
//       doc="result"
//     />
//
//     <doc kind="post" id="result">
//       User's routine must return -1 if reaction 1 is less than
//       reaction 2, 0 if the two reactions are equal, and 1 if reaction 1
//       is greater than reaction 2.  If this routine is not supplied through
//       Libnucnet__Reac__setReactionCompareFunction, the default is not
//       to sort the reactions but rather to iterate them in the order
//       in which they are stored internally, which is not defined by
//       the user.
//     </doc>
//
//     <doc kind="bug" >
//     </doc>
//
//   </usage>
//
// </user_routine>
//############################################################################*/

typedef int ( *Libnucnet__Reaction__compare_function ) (
  const Libnucnet__Reaction *,
  const Libnucnet__Reaction *
);

/*##############################################################################
// <class name="Libnucnet__Reac">
//
//   <description>
//     <abstract>
//       Libnucnet__Reac is a structure that stores a collection of reactions
//       for a nuclear network.  Routines act on the structure to retrieve data,
//       update data, or use data to compute reaction rates based on the
//       data. The contents of the structure are not made public by the API.
//     </abstract>
//     <keywords>
//       nuclear, reaction, network, rates, xml, gdome
//     </keywords>
//   </description>
//
//   <authors>
//     <current>
//       <author userid="dcadams" start_date="2005/07/18"/>
//       <author userid="mbradle" start_date="2005/07/18"/>
//     </current>
//   </authors>
//
//   <compatibility>
//     Compiled successfully in gcc (GCC) 3.4.0.
//   </compatibility>
//
// </class>
//############################################################################*/

struct _Libnucnet__Reac {
  xmlHashTablePtr pReactionHash;
  xmlHashTablePtr pFDHash;
  size_t iUpdate;
  int iOwner;
  Libnucnet__Reaction__compare_function pfReactionCompare;
};

/*##############################################################################
// <class name="Libnucnet__ReacView">
//
//   <description>
//     <abstract>
//       Libnucnet__ReacView is a structure that stores a view of
//       Libnucnet__Reac structure.  A view is a subset of the parent
//       Libnucnet__Reac with the reactions chosen by an XPath expression.
//       The view structure contains a Libnucnet__Reac structure, which
//       may be passed into all API routines that take such structures as
//       input.  It is important to note that the Libnucnet__Reac member of
//       a view does not own the reaction, so modifying the data in the
//       view modifies the data in the parent structure.
//     </abstract>
//     <keywords>
//       nuclear, network, rates, xml, reaction, view
//     </keywords>
//   </description>
//
//   <authors>
//     <current>
//       <author userid="mbradle" start_date="2011/05/22"/>
//     </current>
//   </authors>
//
//   <compatibility>
//     Compiled successfully in gcc (GCC) 3.4.0.
//   </compatibility>
//
// </class>
//############################################################################*/

typedef struct {
  Libnucnet__Reac * pReac;
  xmlChar * sxXPath;
} Libnucnet__ReacView;

/*##############################################################################
// <user_routine name="Libnucnet__Reaction__iterateFunction()">
//
//   <description>
//     <abstract>
//       User-supplied routine to be applied during an iteration over
//       the reactions in the reaction collection.
//     </abstract>
//     <keywords>
//       Libnucnet, user, supplied, function, iterate
//     </keywords>
//   </description>
//
//   <usage>
//
//     <calling_sequence>
// int
// Libnucnet__Reaction__iterateFunction(
//   Libnucnet__Reaction *self,
//   void *p_user_data
// );
//     </calling_sequence>
//
//     <param
//       name="self"
//       kind="in,positional,required"
//     >
//       A pointer to a Libnucnet__Reaction structure.
//     </param>
//
//     <param
//       name="p_user_data"
//       kind="in,positional,required"
//     >
//       A pointer to a user-defined structure containing extra data for
//       the iterate function.
//     </param>
//
//     <param
//       kind="return"
//       doc="result"
//     />
//
//     <doc kind="post" id="result">
//       User's routine must return 1 to continue or 0 to stop.
//     </doc>
//
//     <doc kind="bug" >
//     </doc>
//
//   </usage>
//
// </user_routine>
//############################################################################*/

typedef int ( *Libnucnet__Reaction__iterateFunction ) (
  Libnucnet__Reaction *, void *
);

/*##############################################################################
// <user_routine name="Libnucnet__Reaction__user_rate_property_iterate_function()">
//
//   <description>
//     <abstract>
//       User-supplied routine to be applied during an iteration over
//       the properties for a user-defined rate function.
//     </abstract>
//     <keywords>
//       Libnucnet, user, supplied, function, iterate
//     </keywords>
//   </description>
//
//   <usage>
//
//     <calling_sequence>
// void
// Libnucnet__Reaction__user_rate_property_iterate_function(
//   const char *s_name,
//   const char *s_tag1,
//   const char *s_tag2,
//   const char *s_value,
//   void *p_user_data
// );
//     </calling_sequence>
//
//     <param
//       name="s_name"
//       kind="in,positional,required"
//     >
//       A string giving the property name.
//     </param>
//
//     <param
//       name="s_tag1"
//       kind="in,positional,required"
//     >
//       A string giving the first tag for the property (could be NULL).
//     </param>
//
//     <param
//       name="s_tag2"
//       kind="in,positional,required"
//     >
//       A string giving the second tag for the property (could be NULL).
//     </param>
//
//     <param
//       name="s_value"
//       kind="in,positional,required"
//     >
//       A string giving the value for the property.
//     </param>
//
//     <param
//       name="p_user_data"
//       kind="in,positional,required"
//     >
//       A pointer to a user-defined structure containing extra data for
//       the iterate function.
//     </param>
//
//     <param
//       kind="return"
//       doc="result"
//     />
//
//     <doc kind="post" id="result">
//       On successful return, the function has been applied.
//     </doc>
//
//     <doc kind="bug" >
//     </doc>
//
//   </usage>
//
// </user_routine>
//############################################################################*/

typedef void ( *Libnucnet__Reaction__user_rate_property_iterate_function ) (
  const char *,
  const char *,
  const char *,
  const char *,
  void *
);

typedef struct {
  Libnucnet__Reaction__rateFunction pfFunc;
  Libnucnet__Reaction__user_rate_function_data_deallocator pfDeallocator;
} Libnucnet__Reac__FD;

/*##############################################################################
// <user_routine name="Libnucnet__Reaction__Element__iterateFunction()">
//
//   <description>
//     <abstract>
//       User-supplied routine to be applied during an iteration over
//       the reaction elements (reactants or products) in a reaction.
//     </abstract>
//     <keywords>
//       Libnucnet, user, supplied, function, iterate
//     </keywords>
//   </description>
//
//   <usage>
//
//     <calling_sequence>
// int
// Libnucnet__Reaction__Element__iterateFunction(
//   Libnucnet__Reaction__Element *self,
//   void *p_user_data
// );
//     </calling_sequence>
//
//     <param
//       name="self"
//       kind="in,positional,required"
//     >
//       A pointer to a Libnucnet__Reaction__Element structure.
//     </param>
//
//     <param
//       name="p_user_data"
//       kind="in,positional,required"
//     >
//       A pointer to a user-defined structure containing extra data for
//       the iterate function.
//     </param>
//
//     <param
//       kind="return"
//       doc="result"
//     />
//
//     <doc kind="post" id="result">
//       User's routine must return 1 to continue or 0 to stop.
//     </doc>
//
//     <doc kind="bug" >
//     </doc>
//
//   </usage>
//
// </user_routine>
//############################################################################*/

typedef int ( *Libnucnet__Reaction__Element__iterateFunction ) (
  Libnucnet__Reaction__Element *, void *
);

/*##############################################################################
// API routines.
//############################################################################*/

/*##############################################################################
// <routine name="Libnucnet__Reac__new()">
//
//   <description>
//     <abstract>
//       Create a new Libnucnet__Reac structure.
//     </abstract>
//     <keywords>
//       Libnucnet, nuclear network, reaction, xml, hash
//     </keywords>
//   </description>
//
//   <usage>
//
//     <calling_sequence>
// Libnucnet__Reac *Libnucnet__Reac__new( );
//     </calling_sequence>
//
//     <param
//       kind="return"
//       doc="result"
//     />
//
//     <doc kind="post" id="result">
//       The routine returns a pointer to a Libnucnet reaction structure.
//       If the routine cannot allocate memory for the structure,
//       Libnucnet_Reac error handling is invoked.
//     </doc>
//
//     <doc kind="bug" >
//     </doc>
//
//     <doc kind="example" id="example">
//       <synopsis>
//         Create a Libnucnet reaction structure p_my_reac:
//       </synopsis>
//
//       <code>
// p_my_reac = Libnucnet__Reac__new();
//       </code>
//     </doc>
//
//   </usage>
//
// </routine>
//############################################################################*/

Libnucnet__Reac *Libnucnet__Reac__new( void );

/*##############################################################################
// <routine name="Libnucnet__Reac__new_from_xml()">
//
//   <description>
//     <abstract>
//       Reads in nuclear network reaction data from xml file and stores the
//       reactions in a structure. The reactions to be stored are selected with
//       an xpath expression.
//     </abstract>
//     <keywords>
//       Libnucnet, nuclear network, xml, xpath, reactions
//     </keywords>
//   </description>
//
//   <usage>
//
//     <calling_sequence>
// Libnucnet__Reac *
// Libnucnet__Reac__new_from_xml( 
//   const char *s_xml_filename,
//   const char *s_xpath 
// );
//     </calling_sequence>
//
//     <param
//       name="s_xml_filename" 
//       kind="in,positional,required" 
//     >
//       A string giving the name of the xml file contain the reaction data.
//       This may be the name of a local file or a URL.
//     </param>
//     <param
//       name="s_xpath"
//       kind="in,positional,required"
//       doc="s_in"
//     >
//       A string giving an xpath expression for the reactants or products
//       in the reaction(s) desired.
//     </param>
//     <param
//       kind="return"
//       doc="result"
//     />
//
//     <doc kind="pre" id="s_in">
//       If all reactions are required, this string should be empty or NULL.
//     </doc>
//     <doc kind="post" id="result">
//       If the input is valid, routine returns a pointer to a Libnucnet
//       reaction structure containing reactions selected by the xpath
//       expression. If no reactions were found, p_reactions_list is empty.
//       If the input file or xpath expression is invalid,
//       Libnucnet__Reac error handling is invoked.
//     </doc>
//
//     <doc kind="bug" >
//     </doc>
//
//     <doc kind="example" id="example1">
//       <synopsis>
//       Store all the reactions from "reactions.xml"
//       to p_reactions_list:
//       </synopsis>
//
//       <code>
// p_reactions_list =
//   Libnucnet__Reac__new_from_xml(
//     "reactions.xml",
//     NULL
//   );
//       </code>
//     </doc>
//
//     <doc kind="example" id="example2">
//       <synopsis>
//       Store the reactions from "reactions.xml" containing 'h1' as a reactant
//       to p_h1_reactions_list:
//       </synopsis>
//
//       <code>
// p_h1_reactions_list =
//   Libnucnet__Reac__new_from_xml(
//     "reactions.xml",
//     "[reactant = 'h1']"
//   );
//       </code>
//     </doc>
//
//   </usage>
//
// </routine>
//############################################################################*/

Libnucnet__Reac *Libnucnet__Reac__new_from_xml( 
  const char *, const char *
);

/*##############################################################################
// <routine name="Libnucnet__Reac__updateFromXml()">
//
//   <description>
//     <abstract>
//       Updates nuclear network reaction data in a Libnucnet__Reac structure
//       from and xml file.
//     </abstract>
//     <keywords>
//       Libnucnet, nuclear network, xml, xpath, reactions
//     </keywords>
//   </description>
//
//   <usage>
//
//     <calling_sequence>
// void
// Libnucnet__Reac__updateFromXml( 
//   Libnucnet__Reac *self,
//   const char *s_xml_filename,
//   char *s_xpath 
// );
//     </calling_sequence>
//
//     <param
//       name="self" 
//       kind="in,positional,required" 
//     >
//       A pointer to the pre-existing Libnucnet__Reac structure to be updated.
//     </param>
//     <param
//       name="s_xml_filename" 
//       kind="in,positional,required" 
//     >
//       A string giving the name of the xml file contain the new reaction data.
//       This may be the name of a local file or a URL.
//     </param>
//     <param
//       name="s_xpath"
//       kind="in,positional,required"
//       doc="s_in"
//     >
//       A string giving an xpath expression for the reactants or products
//       in the reaction(s) in the update file desired.
//     </param>
//     <param
//       kind="return"
//       doc="result"
//     />
//
//     <doc kind="pre" id="s_in">
//       If all reactions are required, this string should be empty or NULL.
//     </doc>
//     <doc kind="post" id="result">
//       If the input is valid, the input Libnucnet__Reac structure is
//       updated with the reaction data from the input file.
//     </doc>
//
//     <doc kind="bug" >
//     </doc>
//
//     <doc kind="example" id="example1">
//       <synopsis>
//       Update the reaction data in p_my_reaction, with data from the file
//       "reactions_new.xml"
//       </synopsis>
//
//       <code>
// Libnucnet__Reac__updateFromXml(
//   p_my_reactions,
//   "reactions_new.xml",
//   NULL
// );
//       </code>
//     </doc>
//
//   </usage>
//
// </routine>
//############################################################################*/

void
Libnucnet__Reac__updateFromXml(
  Libnucnet__Reac *, const char *, const char *
);

/*##############################################################################
// <routine name="Libnucnet__Reaction__new()">
//
//   <description>
//     <abstract>
//       Create a new Libnucnet__Reaction structure.
//     </abstract>
//     <keywords>
//       Libnucnet, nuclear network, reaction, xml, hash
//     </keywords>
//   </description>
//
//   <usage>
//
//     <calling_sequence>
// Libnucnet__Reaction *Libnucnet__Reaction__new( );
//     </calling_sequence>
//
//     <param
//       kind="return"
//       doc="result"
//     />
//
//     <doc kind="post" id="result">
//       The routine returns a pointer to a new Libnucnet reaction structure.
//       If the routine cannot allocate memory for the structure,
//       Libnucnet_Reac error handling is invoked.
//     </doc>
//
//     <doc kind="bug" >
//     </doc>
//
//     <doc kind="example" id="example">
//       <synopsis>
//         Create a Libnucnet reaction p_my_reaction:
//       </synopsis>
//
//       <code>
// p_my_reaction = Libnucnet__Reaction__new();
//       </code>
//     </doc>
//
//   </usage>
//
// </routine>
//############################################################################*/

Libnucnet__Reaction *Libnucnet__Reaction__new( void );

/*##############################################################################
// <routine name="Libnucnet__Reaction__updateSingleRate()">
//
//   <description>
//     <abstract>
//       Update the single reaction rate for a reaction.
//     </abstract>
//     <keywords>
//       Libnucnet, nuclear network, reaction
//     </keywords>
//   </description>
//
//   <usage>
//
//     <calling_sequence>
//  void
//  Libnucnet__Reaction__updateSingleRate(
//     Libnucnet__Reaction *self,
//     double d_forward
//   );
//     </calling_sequence>
//
//     <param
//       name="self"
//       kind="in,positional,required"
//     >
//       A pointer to a Libnucnet__Reac structure.
//     </param>
//     <param
//       name="d_forward"
//       kind="in,positional,required"
//     >
//       A double giving the forward reaction rate.
//     </param>
//     <param
//       kind="return"
//       doc="result"
//     />
//
//     <doc kind="post" id="result">
//       For valid input, the routine updates the Libnucnet__Reaction structure
//       to have the new single rate.  If the reaction data are already in the
//       structure, they are overwritten with this data.  If the input is
//       invalid, Libnucnet__Reac error handling is invoked.
//     </doc>
//
//     <doc kind="bug" >
//     </doc>
//
//     <doc kind="example" id="example">
//       <synopsis>
//         Update Libnucnet__Reaction
//         *p_my_reaction with a forward rate of 0.5:
//       </synopsis>
//
//       <code>
// Libnucnet__Reaction__updateSingleRate(
//   p_my_reaction,
//   0.5
// );
//       </code>
//     </doc>
//
//   </usage>
//
// </routine>
//############################################################################*/

void
Libnucnet__Reaction__updateSingleRate(
  Libnucnet__Reaction *, double
);

/*##############################################################################
// <routine name="Libnucnet__Reaction__updateSource()">
//
//   <description>
//     <abstract>
//       Update the source string for a reaction.
//     </abstract>
//     <keywords>
//       Libnucnet, nuclear network, reaction, source
//     </keywords>
//   </description>
//
//   <usage>
//
//     <calling_sequence>
// void
// Libnucnet__Reaction__updateSource(
//   Libnucnet__Reaction *self,
//   const char *s_source
// );
//     </calling_sequence>
//
//     <param
//       name="self"
//       kind="in,positional,required"
//     >
//       A pointer to a Libnucnet__Reaction structure.
//     </param>
//     <param
//       name="s_source"
//       kind="in,positional,required"
//     >
//       A string giving any information about the source of the data.
//     </param>
//     <param
//       kind="return"
//       doc="result"
//     />
//
//     <doc kind="post" id="result">
//       For valid input, the routine updates the Libnucnet__Reaction structure
//       to have the new source string.  If the reaction source string is
//       already in the structure, it is overwritten with this data.
//       If the input is invalid, Libnucnet__Reac error handling is invoked.
//     </doc>
//
//     <doc kind="bug" >
//     </doc>
//
//     <doc kind="example" id="example">
//       <synopsis>
//         Update the reaction Libnucnet__Reaction
//         *p_my_reaction with a message "My data":
//       </synopsis>
//
//       <code>
// Libnucnet__Reaction__updateSource(
//   p_my_reaction,
//   "My data"
// );
//       </code>
//     </doc>
//
//   </usage>
//
// </routine>
//############################################################################*/

void
Libnucnet__Reaction__updateSource(
  Libnucnet__Reaction *, const char *
);

/*##############################################################################
// <routine name="Libnucnet__Reaction__addReactant()">
//
//   <description>
//     <abstract>
//       Add a reactant to a reaction.
//     </abstract>
//     <keywords>
//       Libnucnet, nuclear network, reaction, reactant
//     </keywords>
//   </description>
//
//   <usage>
//
//     <calling_sequence>
//  void
//  Libnucnet__Reaction__addReactant(
//    Libnucnet__Reaction *self,
//    const char *s_reactant
//  );
//     </calling_sequence>
//
//     <param
//       name="self"
//       kind="in,positional,required"
//     >
//       A pointer to a Libnucnet__Reaction structure.
//     </param>
//     <param
//       name="s_reactant"
//       kind="in,positional,required"
//     >
//       A string giving the name of the reactant to add.
//     </param>
//     <param
//       kind="return"
//       doc="result"
//     />
//
//     <doc kind="post" id="result">
//       For valid input, the routine updates the Libnucnet__Reaction structure
//       to include the reactant.  If the input is
//       invalid or the reactant cannot be added,
//       Libnucnet__Reac error handling is invoked.
//     </doc>
//
//     <doc kind="bug" >
//     </doc>
//
//     <doc kind="example" id="example">
//       <synopsis>
//         Add the reactant "h1" to the reaction p_reaction:
//       </synopsis>
//
//       <code>
// Libnucnet__Reaction__addReactant(
//   p_my_reaction,
//   "h1"
// );
//       </code>
//     </doc>
//
//   </usage>
//
// </routine>
//############################################################################*/

void
Libnucnet__Reaction__addReactant(
  Libnucnet__Reaction *, const char *
);

/*##############################################################################
// <routine name="Libnucnet__Reaction__addProduct()">
//
//   <description>
//     <abstract>
//       Add a product to a reaction.
//     </abstract>
//     <keywords>
//       Libnucnet, nuclear network, reaction, product
//     </keywords>
//   </description>
//
//   <usage>
//
//     <calling_sequence>
//  void
//  Libnucnet__Reaction__addProduct(
//    Libnucnet__Reaction *self,
//    const char *s_product
//  );
//     </calling_sequence>
//
//     <param
//       name="self"
//       kind="in,positional,required"
//     >
//       A pointer to a Libnucnet__Reaction structure.
//     </param>
//     <param
//       name="s_product"
//       kind="in,positional,required"
//     >
//       A string giving the name of the product to add.
//     </param>
//     <param
//       kind="return"
//       doc="result"
//     />
//
//     <doc kind="post" id="result">
//       For valid input, the routine updates the Libnucnet__Reaction structure
//       to include the product.  If the input is
//       invalid or the product cannot be added,
//       Libnucnet__Reac error handling is invoked.
//     </doc>
//
//     <doc kind="bug" >
//     </doc>
//
//     <doc kind="example" id="example">
//       <synopsis>
//         Add the product "n13" to the reaction p_reaction:
//       </synopsis>
//
//       <code>
// Libnucnet__Reaction__addProduct(
//   p_my_reaction,
//   "n13"
// );
//       </code>
//     </doc>
//
//   </usage>
//
// </routine>
//############################################################################*/

void
Libnucnet__Reaction__addProduct(
  Libnucnet__Reaction *, const char *
);

/*##############################################################################
// <routine name="Libnucnet__Reaction__updateRateTable()">
//
//   <description>
//     <abstract>
//       Update rate table for a reaction.
//     </abstract>
//     <keywords>
//       Libnucnet, nuclear network, reaction, rate table
//     </keywords>
//   </description>
//
//   <usage>
//
//     <calling_sequence>
// void
// Libnucnet__Reaction__updateRateTable(
//   Libnucnet__Reaction *self,
//   gsl_vector *p_t9,
//   gsl_vector *p_rate,
//   gsl_vector *p_sef
// );
//     </calling_sequence>
//
//     <param
//       name="self"
//       kind="in,positional,required"
//     >
//       A pointer to a Libnucnet__Reaction structure.
//     </param>
//     <param
//       name="p_t9"
//       kind="in,positional,required"
//     >
//       A gsl_vector containing the t9 values in the table.
//     </param>
//     <param
//       name="p_rate"
//       kind="in,positional,required"
//     >
//       A gsl_vector containing the rate values in the table.
//     </param>
//     <param
//       name="p_sef"
//       kind="in,positional,required"
//     >
//       A gsl_vector containing the sef values in the table.
//     </param>
//     <param
//       kind="return"
//       doc="result"
//     />
//
//     <doc kind="post" id="result">
//       For valid input, the routine updates the rate table for
//       the input Libnucnet__Reaction structure to include the new data.
//       If the data are already in the structure, they are overwritten with
//       these new data.  If the input is
//       invalid or the data cannot be added, Libnucnet__Reac error
//       handling is invoked.
//     </doc>
//
//     <doc kind="bug" >
//     </doc>
//
//     <doc kind="example" id="example">
//       <synopsis>
//         Update a Libnucnet__Reaction p_reaction table with the
//         rate table values contained in the gsl_vectors p_t9, p_rate,
//         and p_sef:
//       </synopsis>
//
//       <code>
// Libnucnet__Reaction__updateRateTable(
//   p_reaction, 
//   p_t9,
//   p_rate,
//   p_sef
// );
//       </code>
//     </doc>
//
//   </usage>
//
// </routine>
//############################################################################*/

void
Libnucnet__Reaction__updateRateTable(
  Libnucnet__Reaction *, 
  gsl_vector *,
  gsl_vector *,
  gsl_vector *
);

/*##############################################################################
// <routine name="Libnucnet__Reaction__addNonSmokerFit()">
//
//   <description>
//     <abstract>
//       Add a non-smoker fit to a Libnucnet__Reaction structure.
//     </abstract>
//     <keywords>
//       Libnucnet, nuclear network, reaction, non-smoker
//     </keywords>
//   </description>
//
//   <usage>
//
//     <calling_sequence>
//  void
//  Libnucnet__Reaction__addNonSmokerFit(
//    Libnucnet__Reaction *self,
//    const char *s_note,
//    double a[],
//    double d_spint,
//    double d_spinf,
//    double d_tlowhf,
//    double d_tlowfit,
//    double d_thighfit,
//    double d_acc
//  );
//     </calling_sequence>
//
//     <param
//       name="self"
//       kind="in,positional,required"
//     >
//       A pointer to a Libnucnet__Reaction structure.
//     </param>
//     <param
//       name="s_note"
//       kind="in,positional,required"
//     >
//       A string containing information about the fit.
//     </param>
//     <param
//       name="a"
//       kind="in,positional,required"
//     >
//       An array of doubles giving the a values.
//     </param>
//     <param
//       name="d_spint"
//       kind="in,positional,required"
//     >
//       A double giving the spint value.
//     </param>
//     <param
//       name="d_spinf"
//       kind="in,positional,required"
//     >
//       A double giving the spinf value.
//     </param>
//     <param
//       name="d_tlowhf"
//       kind="in,positional,required"
//     >
//       A double giving the tlowhf value.
//     </param>
//     <param
//       name="d_tlowfit"
//       kind="in,positional,required"
//     >
//       A double giving the tlowfit value.
//     </param>
//     <param
//       name="d_thighfit"
//       kind="in,positional,required"
//     >
//       A double giving the thighfit value.
//     </param>
//     <param
//       name="d_acc"
//       kind="in,positional,required"
//     >
//       A double giving the acc value.
//     </param>
//     <param
//       kind="return"
//       doc="result"
//     />
//
//     <doc kind="post" id="result">
//       For valid input, the routine updates the Libnucnet__Reaction structure
//       to include the new non-smoker reaction fit data.  If the input is
//       invalid or the reaction data cannot be added, Libnucnet__Reac error
//       handling is invoked.
//     </doc>
//
//     <doc kind="bug" >
//     </doc>
//
//     <doc kind="example" id="example">
//       <synopsis>
//         Add the non-smoker data to a
//         Libnucnet__Reaction *p_reaction with  
//         non-smoker data values a, spint, spinf, tlowhf, tlowfit,
//         thighfit, and acc:
//       </synopsis>
//
//       <code>
// Libnucnet__Reac__addNonSmokerFit(
//   p_reaction,
//   "My fit data",
//   a,
//   spint,
//   spinf,
//   tlowhf,
//   tlowfit,
//   thighfit,
//   acc
// );
//       </code>
//     </doc>
//
//   </usage>
//
// </routine>
//############################################################################*/

void
Libnucnet__Reaction__addNonSmokerFit(
  Libnucnet__Reaction *,
  const char *,
  double [],
  double,
  double,
  double,
  double,
  double,
  double
);

/*##############################################################################
// <routine name="Libnucnet__Reaction__getString()">
//
//   <description>
//     <abstract>
//       Retrieves the reaction string of the specified reaction.
//     </abstract>
//     <keywords>
//       nuclear network, reaction, reaction string
//     </keywords>
//   </description>
//
//   <usage>
//
//     <calling_sequence>
// const char *
// Libnucnet__Reaction__getString( const Libnucnet__Reaction *self ); 
//     </calling_sequence>
//
//     <param
//       name="self"
//       kind="in,positional,required"
//     >
//       A pointer to a Libnucnet reaction structure.
//     </param>
//     <param
//       kind="return"
//       doc="result"
//     />
//
//     <doc kind="post" id="result">
//       Routine returns a char * representing the reaction string.
//       If the reaction is not found, routine returns NULL.
//     </doc>
//
//     <doc kind="bug" >
//     </doc>
//
//     <doc kind="example" id="example">
//       <synopsis>
//       Print reaction string for reaction p_reaction:
//       </synopsis>
//
//       <code>
// printf( "%s\n", Libnucnet__Reaction__getString( p_reaction ) );
//       </code>
//     </doc>
//
//   </usage>
//
// </routine>
//############################################################################*/

const char *
Libnucnet__Reaction__getString( const Libnucnet__Reaction * );

/*##############################################################################
// <routine name="Libnucnet__Reaction__getSource()">
//
//   <description>
//     <abstract>
//       Retrieves the string containing the message about the source
//       of the data for the specified reaction.
//     </abstract>
//     <keywords>
//       nuclear network, reaction, reaction string
//     </keywords>
//   </description>
//
//   <usage>
//
//     <calling_sequence>
// const char *
// Libnucnet__Reaction__getSource( const Libnucnet__Reaction *self ); 
//     </calling_sequence>
//
//     <param
//       name="self"
//       kind="in,positional,required"
//     >
//       A pointer to a Libnucnet reaction structure.
//     </param>
//     <param
//       kind="return"
//       doc="result"
//     />
//
//     <doc kind="post" id="result">
//       Routine returns a char * containing the message about the
//       source of the data for the reaction.  If there is no
//       information about the reaction's source, the routine returns an
//       empty string. If the reaction is not found, error handling is
//       invoked.
//     </doc>
//
//     <doc kind="bug" >
//     </doc>
//
//     <doc kind="example" id="example">
//       <synopsis>
//       Print the data source for p_reaction:
//       </synopsis>
//
//       <code>
// printf(
//   "Data source: %s\n",
//   Libnucnet__Reaction__getSource( p_reaction )
// );
//       </code>
//     </doc>
//
//   </usage>
//
// </routine>
//############################################################################*/

const char *
Libnucnet__Reaction__getSource( const Libnucnet__Reaction * );

/*##############################################################################
// <routine name="Libnucnet__Reaction__printRateData()">
//
//   <description>
//     <abstract>
//       Prints the rate data for a reaction.
//     </abstract>
//     <keywords>
//       nuclear network, reaction, rate data
//     </keywords>
//   </description>
//
//   <usage>
//
//     <calling_sequence>
// void
// Libnucnet__Reaction__printRateData( Libnucnet__Reaction *self );
//     </calling_sequence>
//
//     <param
//       name="self"
//       kind="in,positional,required"
//     >
//       A pointer to a Libnucnet__Reaction structure.
//     </param>
//
//     <doc kind="bug" >
//     </doc>
//
//     <doc kind="example" id="example">
//       <synopsis>
//       Print rate data for the reaction p_reaction:
//       </synopsis>
//
//     <doc kind="post" id="result">
//       Routine prints out rate data for the reaction.  If the reaction
//       is not valid, error handling is invoked.
//     </doc>
//
//       <code>
// Libnucnet__Reaction__printRateData( p_reaction );
//       </code>
//     </doc>
//
//   </usage>
//
// </routine>
//############################################################################*/

void
Libnucnet__Reaction__printRateData( Libnucnet__Reaction * );

/*##############################################################################
// <routine name="Libnucnet__Reac__getNumberOfReactions()">
//
//   <description>
//     <abstract>
//       Retrieves the number of reactions.
//     </abstract>
//     <keywords>
//       nuclear network, reactions, number
//     </keywords>
//   </description>
//
//   <usage>
//
//     <calling_sequence>
// size_t
// Libnucnet__Reac__getNumberOfReactions(
//   const Libnucnet__Reac *self
// );
//     </calling_sequence>
//
//     <param
//       name="self"
//       kind="in,positional,required"
//     >
//       A pointer to a Libnucnet reaction structure.
//     </param>
//     <param
//       kind="return"
//       doc="result"
//     />
//
//     <doc kind="post" id="result">
//       Routine returns a size_t integer giving the number of reactions.
//       If the input reaction pointer is invalid, error handling is invoked.
//     </doc>
//
//     <doc kind="bug" >
//     </doc>
//
//     <doc kind="example" id="example">
//       <synopsis>
//       Retrieve the number of reactions stored in p_reaction_list:
//       </synopsis>
//
//       <code>
// size_t i_num_reac;
// i_num_reac =
//   Libnucnet__Reac__getNumberOfReactions(
//     p_reaction_list
//   );
//       </code>
//     </doc>
//
//   </usage>
//
// </routine>
//############################################################################*/

size_t
Libnucnet__Reac__getNumberOfReactions( const Libnucnet__Reac * );

/*##############################################################################
// <routine name="Libnucnet__Reaction__computeRate()">
//
//   <description>
//     <abstract>
//       Computes the rate of the reaction at the input temperature.
//     </abstract>
//     <keywords>
//       nuclear network, reactions, rate
//     </keywords>
//   </description>
//
//   <usage>
//
//     <calling_sequence>
// double
// Libnucnet__Reaction__computeRate(
//   Libnucnet__Reaction *self,
//   double d_t9,
//   void *p_extra_data
// );
//     </calling_sequence>
//
//     <param
//       name="self"
//       kind="in,positional,required"
//     >
//       A pointer to a Libnucnet reaction structure.
//     </param>
//     <param
//       name="d_t9"
//       kind="in,positional,required"
//     >
//       A double giving the temperature (in billions of K) at which to
//       compute the rate.
//     </param>
//     <param
//       name="p_data"
//       kind="in,positional,required"
//     >
//       A pointer to the extra data used to compute the rate.
//     </param>
//     <param
//       kind="return"
//       doc="result"
//     />
//
//     <doc kind="post" id="result">
//       Routine returns the rate for the input temperature.  If the
//       input reaction or the temperature is invalid,
//       Libnucnet__Reac error handling is invoked.
//     </doc>
//
//     <doc kind="bug" >
//     </doc>
//
//     <doc kind="example" id="example">
//       <synopsis>
//         Print the rate for Libnucnet__Reaction * p_reaction at t9 = 3.:
//       </synopsis>
//
//       <code>
// printf(
//   "Rate = %e\n",
//   Libnucnet__Reaction__computeRate( p_reaction, 3., NULL )
// );
//       </code>
//     </doc>
//
//   </usage>
//
// </routine>
//############################################################################*/

double
Libnucnet__Reaction__computeRate( Libnucnet__Reaction *, double, void * );

/*##############################################################################
// <routine name="Libnucnet__Reac__removeReaction()">
//
//   <description>
//     <abstract>
//       Remove a reaction from a Libnucnet__Reac structure.
//     </abstract>
//     <keywords>
//       nuclear network, reaction, remove
//     </keywords>
//   </description>
//
//   <usage>
//
//     <calling_sequence>
// int
// Libnucnet__Reac__removeReaction(
//   Libnucnet__Reac *self,
//   Libnucnet__Reaction *p_reaction
// );
//     </calling_sequence>
//
//     <param
//       name="self"
//       kind="in,positional,required"
//     >
//       A pointer to a Libnucnet reac structure.
//     </param>
//
//     <param
//       name="p_reaction"
//       kind="in,positional,required"
//     >
//       A pointer to a Libnucnet reaction structure to be removed from self.
//     </param>
//
//     <param
//       kind="return"
//       doc="result"
//     />
//
//     <doc kind="post" id="result">
//       Routine returns 1 for successful removal and 0 for failure.
//       Upon successful return, the reaction has been removed from the
//       Libnucnet__Reac structure. 
//     </doc>
//
//     <doc kind="bug" >
//     </doc>
//
//     <doc kind="example" id="example">
//       <synopsis>
//         Remove the reaction p_reaction from Libnucnet__Reac *p_my_reac:
//       </synopsis>
//
//       <code>
// if( Libnucnet__Reac__removeReaction( p_my_reac, p_reaction ) )
//   printf( "Reaction successfully removed.\n" );
//       </code>
//     </doc>
//
//   </usage>
//
// </routine>
//############################################################################*/

int
Libnucnet__Reac__removeReaction( 
  Libnucnet__Reac *, Libnucnet__Reaction * 
);

/*##############################################################################
// <routine name="Libnucnet__Reac__free()">
//
//   <description>
//     <abstract>
//       Free the memory allocated for a Libnucnet reaction collection
//       structure.
//     </abstract>
//     <keywords>
//       reaction network, remove, delete, free
//     </keywords>
//   </description>
//
//   <usage>
//
//     <calling_sequence>
// void
// Libnucnet__Reac__free( Libnucnet__Reac *self );
//     </calling_sequence>
//
//     <param
//       name="self"
//       kind="in,positional,required"
//     >
//       A pointer to a Libnucnet reac structure.
//     </param>
//
//     <param
//       kind="return"
//       doc="result"
//     />
//
//     <doc kind="post" id="result">
//       Upon successful return, the memory for the network has been freed.
//       If the input is invalid, Libnucnet__Reac error handling is invoked.
//     </doc>
//
//     <doc kind="bug" >
//     </doc>
//
//     <doc kind="example" id="example">
//       <synopsis>
//         Free up memory used for Libnucnet__Reac *p_my_reac:
//       </synopsis>
//
//       <code>
// Libnucnet__Reac__free( p_my_reac );
//       </code>
//     </doc>
//
//   </usage>
//
// </routine>
//############################################################################*/

void
Libnucnet__Reac__free( Libnucnet__Reac * );

/*##############################################################################
// <routine name="Libnucnet__Reac__getReactionByString()">
//
//   <description>
//     <abstract>
//       Routine to retrieve a reaction by its string.
//     </abstract>
//     <keywords>
//       reaction, network, string
//     </keywords>
//   </description>
//
//   <usage>
//
//     <calling_sequence>
// Libnucnet__Reaction *
// Libnucnet__Reac__getReactionByString(
//   const Libnucnet__Reac * self,
//   const char * s_reaction
// );
//     </calling_sequence>
//
//     <param
//       name="self"
//       kind="in,positional,required"
//     >
//       A pointer to a Libnucnet reac structure.
//     </param>
//
//     <param
//       name="s_reaction"
//       kind="in,positional,required"
//     >
//       A string giving the reaction to be retrieved.
//     </param>
//
//     <param
//       kind="return"
//       doc="result"
//     />
//
//     <doc kind="post" id="result">
//       For valid input, the routine returns a pointer to the
//       requested reaction.  If the reaction does not exist in the
//       Libnucnet__Reac structure, the routine returns NULL.  If the
//       input Libnucnet__Reac structure is not valid, error handling
//       is invoked.
//     </doc>
//
//     <doc kind="bug" >
//     </doc>
//
//     <doc kind="example" id="example">
//       <synopsis>
//         Return a pointer to the reaction li7 + h1 -> he4 + he4 from
//         the Libnucnet__Reac structure p_my_reactions:
//       </synopsis>
//
//       <code>
// p_reaction =
//   Libnucnet__Reac__getReactionByString(
//     p_my_reactions, "li7 + h1 -> he4 + he4"
//   );
//       </code>
//     </doc>
//
//   </usage>
//
// </routine>
//############################################################################*/

Libnucnet__Reaction *
Libnucnet__Reac__getReactionByString(
  const Libnucnet__Reac *, const char *
);

/*##############################################################################
// <routine name="Libnucnet__Reaction__getDuplicateReactantFactor()">
//
//   <description>
//     <abstract>
//       Routine to retrieve the factor by which to divide a forward
//       reaction rate to account for duplicate reactants.
//     </abstract>
//     <keywords>
//       reaction, network, duplicate, reactants
//     </keywords>
//   </description>
//
//   <usage>
//
//     <calling_sequence>
// double
// Libnucnet__Reaction__getDuplicateReactantFactor(
//   const Libnucnet__Reaction * self
// );
//     </calling_sequence>
//
//     <param
//       name="self"
//       kind="in,positional,required"
//     >
//       A pointer to a Libnucnet reaction structure.
//     </param>
//
//     <param
//       kind="return"
//       doc="result"
//     />
//
//     <doc kind="post" id="result">
//       For valid input, the routine returns a double giving the
//       appropriate duplicate reactant factor for the reaction.
//       If the reaction does not exist in the
//       Libnucnet__Reac structure, error handling is invoked.
//     </doc>
//
//     <doc kind="bug" >
//     </doc>
//
//     <doc kind="example" id="example">
//       <synopsis>
//         Print the duplicate reactant factor for the reaction
//         he4 + he4 + he4 -> c12 + gamma from
//         the Libnucnet__Reac structure p_my_reactions (the result is 3! = 6
//         since there are three he4 reactants):
//       </synopsis>
//
//       <code>
// printf(
//   "Duplicate reactant factor = %e\n",
//   Libnucnet__Reaction__getDuplicateReactantFactor(
//     Libnucnet__Reac__getReactionByString(
//       p_my_reactions, "he4 + he4 + he4 -> c12 + gamma"
//     )
//   )
// );
//       </code>
//     </doc>
//
//   </usage>
//
// </routine>
//############################################################################*/

double
Libnucnet__Reaction__getDuplicateReactantFactor( const Libnucnet__Reaction * ); 

/*##############################################################################
// <routine name="Libnucnet__Reaction__getDuplicateProductFactor()">
//
//   <description>
//     <abstract>
//       Routine to retrieve the factor by which to divide a reverse
//       reaction rate to account for duplicate products.
//     </abstract>
//     <keywords>
//       reaction, network, duplicate, products
//     </keywords>
//   </description>
//
//   <usage>
//
//     <calling_sequence>
// double
// Libnucnet__Reaction__getDuplicateProductFactor(
//   const Libnucnet__Reaction * self
// );
//     </calling_sequence>
//
//     <param
//       name="self"
//       kind="in,positional,required"
//     >
//       A pointer to a Libnucnet reaction structure.
//     </param>
//
//     <param
//       kind="return"
//       doc="result"
//     />
//
//     <doc kind="post" id="result">
//       For valid input, the routine returns a double giving the
//       appropriate duplicate product factor for the reaction.
//       If the reaction does not exist in the
//       Libnucnet__Reac structure, error handling is invoked.
//     </doc>
//
//     <doc kind="bug" >
//     </doc>
//
//     <doc kind="example" id="example">
//       <synopsis>
//         Print the duplicate product factor for the reaction
//         he3 + he3 -> h1 + h1 + he4
//         the Libnucnet__Reac structure p_my_reactions (the result is 2! = 2
//         since there are two h1 products):
//       </synopsis>
//
//       <code>
// printf(
//   "Duplicate product factor = %e\n",
//   Libnucnet__Reaction__getDuplicateProductFactor(
//     Libnucnet__Reac__getReactionByString(
//       p_my_reactions, "he3 + he3 -> h1 + h1 + he4"
//     )
//   )
// );
//       </code>
//     </doc>
//
//   </usage>
//
// </routine>
//############################################################################*/

double
Libnucnet__Reaction__getDuplicateProductFactor( const Libnucnet__Reaction * ); 

/*##############################################################################
// <routine name="Libnucnet__Reac__writeToXmlFile()">
//
//   <description>
//     <abstract>
//       Output a Libnucnet__Reac structure to an xml file.
//     </abstract>
//     <keywords>
//       nuclear, data, write, file, reaction
//     </keywords>
//   </description>
//
//   <usage>
//
//     <calling_sequence>
// void
// Libnucnet__Reac__writeToXmlFile(
//   const Libnucnet__Reac *self,
//   const char *s_xml_filename
// );
//     </calling_sequence>
//
//     <param
//       name="self"
//       kind="in,positional,required"
//     >
//       A pointer to a Libnucnet nuclear reaction collection.
//     </param>
//     <param
//       name="s_xml_filename"
//       kind="in,positional,required"
//     >
//       The name of the output xml file.
//     </param>
//
//     <param
//       kind="return"
//       doc="result"
//     />
//
//     <doc kind="post" id="result">
//       Upon successful return, the contents of the collection of reactions
//       are written to s_xml_filename.  Reactions are output in the order
//       determined by the currently defined
//       Libnucnet__Reaction__compare_function.
//     </doc>
//
//     <doc kind="bug" >
//     </doc>
//
//     <doc kind="example" id="example">
//       <synopsis>
//         Dump the contents of Libnucnet__Reac *p_my_reactions to the
//         xml file my.xml:
//       </synopsis>
//
//       <code>
// Libnucnet__Reac__writeToXmlFile( p_my_reactions, "my.xml" );
//       </code>
//     </doc>
//
//   </usage>
//
// </routine>
//############################################################################*/

void
Libnucnet__Reac__writeToXmlFile( const Libnucnet__Reac *, const char * );

/*##############################################################################
// <routine name="Libnucnet__Reac__is_valid_input_xml()">
//
//   <description>
//     <abstract>
//       Validate an xml file for Libnucnet__Reac.
//     </abstract>
//     <keywords>
//       Libnucnet, nuclear, xml, hash, validate, reaction
//     </keywords>
//   </description>
//
//   <usage>
//
//     <calling_sequence>
// int
// Libnucnet__Reac__is_valid_input_xml(
//   const char *s_xml_filename
// );
//     </calling_sequence>
//
//     <param
//       name="s_xml_filename" 
//       kind="in,positional,required" 
//     >
//       A string giving the name of the xml file containing the nuclear
//       reaction data. 
//     </param>
//     <param
//       kind="return"
//       doc="result"
//     />
//
//     <doc kind="post" id="result">
//       For a valid input nuclear reaction data xml file, the routine returns
//       1 (true).  For an invalid file, routine returns 0 (false).
//       If the schema file is invalid, or if the schema file cannot be
//       read over the web, routine stops and prints error
//       message. 
//     </doc>
//
//     <doc kind="bug" >
//     </doc>
//
//     <doc kind="example" id="example1">
//       <synopsis>
//         Validate the input xml file "reaction_data.xml":
//       </synopsis>
//
//       <code>
// if( Libnucnet__Reac__is_valid_input_xml( "reaction_data.xml" ) ) {
//     printf( "Valid xml!\" );
// }
//       </code>
//     </doc>
//
//   </usage>
//
// </routine>
//############################################################################*/

int
Libnucnet__Reac__is_valid_input_xml( const char * );

/*##############################################################################
// <routine name="Libnucnet__Reac__getDuplicateReactions()">
//
//   <description>
//     <abstract>
//       Routine to get duplicate reactions in a structure.  A duplicate
//       reaction is one that either has the same reactants and products
//       as another in the reaction collection.  For example,
//       c12 + h1 to n13 + gamma and h1 + c12 to n13 + gamma are duplicate
//       reactions.  Alternatively, a duplicate reaction may be the reverse
//       of one already in the reaction collection since libnucnet
//       computes reverse rates from detailed balance.  For example,
//       c14 + h1 to n14 + n and n14 + n to c14 + h1 are duplicate
//       reactions.
//     </abstract>
//     <keywords>
//       nuclear network, number, copy, pointer, reaction
//     </keywords>
//   </description>
//
//   <usage>
//
//     <calling_sequence>
// Libnucnet__Reac *
// Libnucnet__Reac__getDuplicateReactions(
//   const Libnucnet__Reac *self
// );
//     </calling_sequence>
//
//     <param
//       name="self"
//       kind="in,positional,required"
//     >
//       A pointer to a Libnucnet reaction collection structure.
//     </param>
//
//     <param
//       kind="return"
//       doc="result"
//     />
//
//     <doc kind="post" id="result">
//       Returns the pointer to a new Libnucnet__Reac structure.  The
//       new structure contains the duplicate reactions.
//       If the input structure is invalid, error handling is invoked.
//     </doc>
//
//     <doc kind="bug" >
//     </doc>
//
//     <doc kind="example" id="example">
//       <synopsis>
//       Given p_my_reactions, get Libnucnet__Reac structure
//       p_my_duplicates containing the duplicate reactions.
//       </synopsis>
//
//       <code>
//  p_my_duplicates =
//    Libnucnet__Reac__getDuplicateReactions( p_my_reactions );
//       </code>
//     </doc>
//
//   </usage>
//
// </routine>
//############################################################################*/

Libnucnet__Reac *
Libnucnet__Reac__getDuplicateReactions( const Libnucnet__Reac * );

/*##############################################################################
// <routine name="Libnucnet__Reac__addReaction()">
//
//   <description>
//     <abstract>
//       Add a reaction to a reaction collection.
//     </abstract>
//     <keywords>
//       nuclear network, number, add, reaction
//     </keywords>
//   </description>
//
//   <usage>
//
//     <calling_sequence>
// int
// Libnucnet__Reac__addReaction(
//   Libnucnet__Reac *self,
//   Libnucnet__Reaction *p_reaction_to_add
// );
//     </calling_sequence>
//
//     <param
//       name="self"
//       kind="in,positional,required"
//     >
//       A pointer to the Libnucnet reaction collection structure to
//       which to add the reaction.
//     </param>
//
//     <param
//       name="p_reaction_to_add"
//       kind="in,positional,required"
//     >
//       A pointer to the Libnucnet reaction structure containing
//       the reaction to add.
//     </param>
//
//     <param
//       kind="return"
//       doc="result"
//     />
//
//     <doc kind="post" id="result">
//       Routine returns 1 for successful addition, 0 for failure.
//       Upon successful return, the reaction pointed to by p_reaction_to_add
//       has been added to self. If any input is invalid, error handling is
//       invoked.
//     </doc>
//
//     <doc kind="bug" >
//     </doc>
//
//     <doc kind="example" id="example">
//       <synopsis>
//       Add the reaction p_reaction_to_add to p_my_reactions:
//       </synopsis>
//
//       <code>
// if(
//   Libnucnet__Reac__addReaction(
//      p_my_reactions, p_reaction_to_add
//   )
// )
//   printf( "Successful addition.\n" ); 
//       </code>
//     </doc>
//
//   </usage>
//
// </routine>
//############################################################################*/

int
Libnucnet__Reac__addReaction( Libnucnet__Reac *, Libnucnet__Reaction * );

/*##############################################################################
// <routine name="Libnucnet__Reac__updateReaction()">
//
//   <description>
//     <abstract>
//       Update a reaction in a reaction collection.
//     </abstract>
//     <keywords>
//       nuclear network, number, add, reaction
//     </keywords>
//   </description>
//
//   <usage>
//
//     <calling_sequence>
// int
// Libnucnet__Reac__updateReaction(
//   Libnucnet__Reac *self,
//   Libnucnet__Reaction *p_reaction
// );
//     </calling_sequence>
//
//     <param
//       name="self"
//       kind="in,positional,required"
//     >
//       A pointer to the Libnucnet reaction collection structure to
//       which to add the reaction.
//     </param>
//
//     <param
//       name="p_reaction_to_add"
//       kind="in,positional,required"
//     >
//       A pointer to the Libnucnet reaction structure containing
//       the reaction.
//     </param>
//
//     <param
//       kind="return"
//       doc="result"
//     />
//
//     <doc kind="post" id="result">
//       Routine returns 1 for successful addition, 0 for failure.
//       Upon successful return, the data for the reaction
//       has been replaced with this new version if it previously existed
//       in the collection or has been added if it did not.
//     </doc>
//
//     <doc kind="bug" >
//     </doc>
//
//     <doc kind="example" id="example">
//       <synopsis>
//       Update the reaction p_reaction to in p_my_reactions:
//       </synopsis>
//
//       <code>
// if(
//   Libnucnet__Reac__updateReaction(
//      p_my_reactions, p_reaction
//   )
// )
//   printf( "Successful update.\n" ); 
//       </code>
//     </doc>
//
//   </usage>
//
// </routine>
//############################################################################*/

int
Libnucnet__Reac__updateReaction( Libnucnet__Reac *, Libnucnet__Reaction * );

/*##############################################################################
// <routine name="Libnucnet__Reac__iterateReactions()">
//
//   <description>
//     <abstract>
//       Iterate through the reactions and apply the user-supplied iterate
//       function.
//     </abstract>
//     <keywords>
//       Libnucnet, reaction, xml, iterate
//     </keywords>
//   </description>
//
//   <usage>
//
//     <calling_sequence>
// void
// Libnucnet__Reac__iterateReactions(
//   const Libnucnet__Reac *self,
//   Libnucnet__Reaction__iterateFunction pfFunc,
//   void *p_user_data
// );
//     </calling_sequence>
//
//     <param
//       name="self" 
//       kind="in,positional,required" 
//     >
//       A pointer to a Libnucnet__Reac structure.
//     </param>
//
//     <param
//       name="pfFunc" 
//       kind="in,positional,required" 
//     >
//       The name of the user-supplied function to apply.
//     </param>
//     <param
//       name="p_user_data" 
//       kind="in,positional,required" 
//     >
//       A pointer to the user-defined data structure containing extra
//       data for the user's iterate function.
//     </param>
//     <param
//       kind="return"
//       doc="result"
//     />
//
//     <doc kind="post" id="result">
//       The routine iterates through the reaction collection and applies
//       the user-supplied routine to each reaction.  The reactions are
//       iterated in the order defined by the user-defined comparison
//       function set with Libnucnet__Reac__setReactionCompareFunction().
//     </doc>
//
//     <doc kind="bug" >
//     </doc>
//
//     <doc kind="example" id="example1">
//       <synopsis>
//         Iterate through the reactions in p_my_reactions and apply the
//         function my_iterate_function and the extra data in
//         p_user_data:
//       </synopsis>
//
//       <code>
// Libnucnet__Reac__iterateReactions(
//   p_my_reactions,
//   (Libnucnet__Reaction__iterateFunction) my_iterate_function,
//   p_user_data
// );
//       </code>
//     </doc>
//
//   </usage>
//
// </routine>
//############################################################################*/

void
Libnucnet__Reac__iterateReactions(
  const Libnucnet__Reac *,
  Libnucnet__Reaction__iterateFunction,
  void *
);

/*##############################################################################
// <routine name="Libnucnet__Reaction__getParentDuplicate()">
//
//   <description>
//     <abstract>
//       Get the reaction of which the current reaction is a duplicate.
//     </abstract>
//     <keywords>
//       Libnucnet, reaction, duplicate, parent
//     </keywords>
//   </description>
//
//   <usage>
//
//     <calling_sequence>
// Libnucnet__Reaction *
// Libnucnet__Reaction__getParentDuplicate(
//   const Libnucnet__Reaction *self
// );
//     </calling_sequence>
//
//     <param
//       name="self" 
//       kind="in,positional,required" 
//     >
//       A pointer to a Libnucnet__Reaction structure.
//     </param>
//
//     <param
//       kind="return"
//       doc="result"
//     />
//
//     <doc kind="post" id="result">
//       The routine returns a pointer to the reaction the current
//       reaction is a duplicate of (in the Libnucnet__Reac structure
//       that was originally scanned for duplicates).  If the parent
//       reaction is no longer present, the routine returns NULL. If the input
//       reaction is invalid, error handling is invoked.
//     </doc>
//
//     <doc kind="bug" >
//     </doc>
//
//     <doc kind="example" id="example1">
//       <synopsis>
//         Print the reaction of which p_reaction is a duplicate:
//       </synopsis>
//
//       <code>
// printf(
//   "%s is a duplicate of %s\n",
//   Libnucnet__Reaction__getString( p_reaction ),
//   Libnucnet__Reaction__getString(
//     Libnucnet__Reaction__getParentDuplicate( p_reaction )
//   )
// );
//       </code>
//     </doc>
//
//   </usage>
//
// </routine>
//############################################################################*/

Libnucnet__Reaction *
Libnucnet__Reaction__getParentDuplicate( const Libnucnet__Reaction * );

/*##############################################################################
// <routine name="Libnucnet__Reaction__Element__isNuclide()">
//
//   <description>
//     <abstract>
//       Determine whether a reaction element is a nuclide (like c12)
//       or not (like electron or neutrino_e).
//     </abstract>
//     <keywords>
//       Libnucnet, reaction, element, nuclide
//     </keywords>
//   </description>
//
//   <usage>
//
//     <calling_sequence>
// int
// Libnucnet__Reaction__Element__isNuclide(
//   const Libnucnet__Reaction__Element *self
// );
//     </calling_sequence>
//
//     <param
//       name="self" 
//       kind="in,positional,required" 
//     >
//       A pointer to a Libnucnet__Reaction__Element structure.
//     </param>
//
//     <param
//       kind="return"
//       doc="result"
//     />
//
//     <doc kind="post" id="result">
//       The routine returns 1 (true) if the reactant is a nuclide or
//       0 (false) if not.  If the input is invalid, error handling is
//       invoked.
//     </doc>
//
//     <doc kind="bug" >
//     </doc>
//
//     <doc kind="example" id="example1">
//       <synopsis>
//         Determine whether Libnucnet__Reaction__Element *p_element
//         is a nuclide or not:
//       </synopsis>
//
//       <code>
// if( Libnucnet__Reaction__Element__isNuclide( p_element ) ) {
//   printf(
//     "%s is a nuclide!\n",
//     Libnucnet__Reaction__Element__getName( p_element )
//   );
// } else {
//   printf(
//     "%s is not a nuclide!\n",
//     Libnucnet__Reaction__Element__getName( p_element )
//   );
// }
//       </code>
//     </doc>
//
//   </usage>
//
// </routine>
//############################################################################*/

int
Libnucnet__Reaction__Element__isNuclide( const Libnucnet__Reaction__Element * );

/*##############################################################################
// <routine name="Libnucnet__Reaction__Element__getName()">
//
//   <description>
//     <abstract>
//       Retrieve the name of a reaction element.
//     </abstract>
//     <keywords>
//       Libnucnet, reaction, element, name
//     </keywords>
//   </description>
//
//   <usage>
//
//     <calling_sequence>
// const char *
// Libnucnet__Reaction__Element__getName(
//   const Libnucnet__Reaction__Element *self
// );
//     </calling_sequence>
//
//     <param
//       name="self" 
//       kind="in,positional,required" 
//     >
//       A pointer to a Libnucnet__Reaction__Element structure.
//     </param>
//
//     <param
//       kind="return"
//       doc="result"
//     />
//
//     <doc kind="post" id="result">
//       The routine returns a string containing the name of the reaction
//       element.  If the input is invalid, error handling is
//       invoked.
//     </doc>
//
//     <doc kind="bug" >
//     </doc>
//
//     <doc kind="example" id="example1">
//       <synopsis>
//         Print the name of Libnucnet__Reaction__Element *p_element:
//       </synopsis>
//
//       <code>
// printf(
//   "The elements name is %s\n",
//   Libnucnet__Reaction__Element__getName( p_element )
// );
//       </code>
//     </doc>
//
//   </usage>
//
// </routine>
//############################################################################*/

const char *
Libnucnet__Reaction__Element__getName( const Libnucnet__Reaction__Element * );

/*##############################################################################
// <routine name="Libnucnet__Reaction__iterateReactants()">
//
//   <description>
//     <abstract>
//       Iterate over the reactants in a reaction.
//     </abstract>
//     <keywords>
//       Libnucnet, reaction, element, iterate
//     </keywords>
//   </description>
//
//   <usage>
//
//     <calling_sequence>
// void
// Libnucnet__Reaction__iterateReactants(
//   const Libnucnet__Reaction *self,
//   Libnucnet__Reaction__Element__iterateFunction pf_func,
//   void *p_user_data
// );
//     </calling_sequence>
//
//     <param
//       name="self" 
//       kind="in,positional,required" 
//     >
//       A pointer to a Libnucnet__Reaction structure.
//     </param>
//
//     <param
//       name="pf_func" 
//       kind="in,positional,required" 
//     >
//       The name of the user-supplied iterate function to be applied
//       during the iteration.
//     </param>
//
//     <param
//       name="p_user_data" 
//       kind="in,positional,required" 
//     >
//       A pointer to the user-supplied data structure containing the
//       extra data to be applied during the iteration.
//     </param>
//
//     <param
//       kind="return"
//       doc="result"
//     />
//
//     <doc kind="post" id="result">
//       The routine iterates over the reactants in order and applies
//       the user-supplied function and data.
//       If the input is invalid, error handling is
//       invoked.
//     </doc>
//
//     <doc kind="bug" >
//     </doc>
//
//     <doc kind="example" id="example1">
//       <synopsis>
//         Iterate over the reactants in Libnucnet__Reaction
//         *p_reaction and apply the function my_function and the data
//         my_data:
//       </synopsis>
//
//       <code>
// Libnucnet__Reaction__iterateReactants(
//   p_reaction,
//   ( Libnucnet__Reaction__Element__iterateFunction ) my_function,
//   &my_data
// );
//       </code>
//     </doc>
//
//   </usage>
//
// </routine>
//############################################################################*/

void
Libnucnet__Reaction__iterateReactants(
  const Libnucnet__Reaction *,
  Libnucnet__Reaction__Element__iterateFunction,
  void *
);

/*##############################################################################
// <routine name="Libnucnet__Reaction__iterateProducts()">
//
//   <description>
//     <abstract>
//       Iterate over the products in a reaction.
//     </abstract>
//     <keywords>
//       Libnucnet, reaction, element, iterate
//     </keywords>
//   </description>
//
//   <usage>
//
//     <calling_sequence>
// void
// Libnucnet__Reaction__iterateProducts(
//   const Libnucnet__Reaction *self,
//   Libnucnet__Reaction__Element__iterateFunction pf_func,
//   void *p_user_data
// );
//     </calling_sequence>
//
//     <param
//       name="self" 
//       kind="in,positional,required" 
//     >
//       A pointer to a Libnucnet__Reaction structure.
//     </param>
//
//     <param
//       name="pf_func" 
//       kind="in,positional,required" 
//     >
//       The name of the user-supplied iterate function to be applied
//       during the iteration.
//     </param>
//
//     <param
//       name="p_user_data" 
//       kind="in,positional,required" 
//     >
//       A pointer to the user-supplied data structure containing the
//       extra data to be applied during the iteration.
//     </param>
//
//     <param
//       kind="return"
//       doc="result"
//     />
//
//     <doc kind="post" id="result">
//       The routine iterates over the products in order and applies
//       the user-supplied function and data.
//       If the input is invalid, error handling is
//       invoked.
//     </doc>
//
//     <doc kind="bug" >
//     </doc>
//
//     <doc kind="example" id="example1">
//       <synopsis>
//         Iterate over the products in Libnucnet__Reaction
//         *p_reaction and apply the function my_function and the data
//         my_data:
//       </synopsis>
//
//       <code>
// Libnucnet__Reaction__iterateProducts(
//   p_reaction,
//   ( Libnucnet__Reaction__Element__iterateFunction ) my_function,
//   &my_data
// );
//       </code>
//     </doc>
//
//   </usage>
//
// </routine>
//############################################################################*/

void
Libnucnet__Reaction__iterateProducts(
  const Libnucnet__Reaction *,
  Libnucnet__Reaction__Element__iterateFunction,
  void *
);

/*##############################################################################
// <routine name="Libnucnet__Reac__extractSubset()">
//
//   <description>
//     <abstract>
//       Extract a subset collection of reactions from another collection by
//       an XPath expression.
//     </abstract>
//     <keywords>
//       Libnucnet, reaction, xml, hash, extract, subset
//     </keywords>
//   </description>
//
//   <usage>
//
//     <calling_sequence>
// Libnucnet__Reac *
// Libnucnet__Reac__extractSubset(
//   const Libnucnet__Reac *self
//   const char *s_xpath
// );
//     </calling_sequence>
//
//     <param
//       name="self" 
//       kind="in,positional,required" 
//     >
//       A pointer to a pre-existing Libnucnet__Reac structure.
//     </param>
//     <param
//       name="s_xpath" 
//       kind="in,positional,required" 
//     >
//       A string giving an xpath expression to apply to the data or NULL
//       to obtain a complete copy.
//     </param>
//     <param
//       kind="return"
//       doc="result"
//     />
//
//     <doc kind="post" id="result">
//       For a valid Libnucnet__Reac structure, the routine returns a new
//       Libnucnet__Reac structure containing reactions selected from the parent
//       collection by the XPath expression.  The user must free
//       the structure with Libnucnet__Reac__free() when finished with it.
//       If the input structure is not valid,
//       Libnucnet__Reac error handling is invoked.
//     </doc>
//
//     <doc kind="bug" >
//     </doc>
//
//     <doc kind="example" id="example1">
//       <synopsis>
//         Extract a complete copy of Libnucnet_Reac *p_parent and call it
//         p_copy:
//       </synopsis>
//
//       <code>
// p_copy =
//   Libnucnet__Reac__extractSubset(
//     p_parent,
//     NULL
//   );
//       </code>
//     </doc>
//
//     <doc kind="example" id="example2">
//       <synopsis>
//         Extract a subset of Libnucnet__Reac *p_parent that includes
//         only reactions with h2 as a reactant:
//       </synopsis>
//
//       <code>
// p_copy =
//   Libnucnet__Reac__extractSubset(
//     p_parent,
//     "[reactant = 'h2']"
//   );
//       </code>
//     </doc>
//
//   </usage>
//
// </routine>
//############################################################################*/

Libnucnet__Reac *
Libnucnet__Reac__extractSubset(
  const Libnucnet__Reac *,
  const char *
);

/*##############################################################################
// <routine name="Libnucnet__Reaction__free()">
//
//   <description>
//     <abstract>
//       Free the memory allocated for a Libnucnet reaction.
//     </abstract>
//     <keywords>
//       reaction network, remove, delete, free
//     </keywords>
//   </description>
//
//   <usage>
//
//     <calling_sequence>
// void
// Libnucnet__Reaction__free( Libnucnet__Reaction *self );
//     </calling_sequence>
//
//     <param
//       name="self"
//       kind="in,positional,required"
//     >
//       A pointer to a Libnucnet reaction structure.
//     </param>
//
//     <param
//       kind="return"
//       doc="result"
//     />
//
//     <doc kind="post" id="result">
//       Upon successful return, the memory for the reaction has been freed.
//       If the input is invalid, Libnucnet__Reac error handling is invoked.
//     </doc>
//
//     <doc kind="bug" >
//     </doc>
//
//     <doc kind="example" id="example">
//       <synopsis>
//         Free up memory used for Libnucnet__Reaction *p_reaction:
//       </synopsis>
//
//       <code>
// Libnucnet__Reaction__free( p_reaction );
//       </code>
//     </doc>
//
//   </usage>
//
// </routine>
//############################################################################*/

void Libnucnet__Reaction__free( Libnucnet__Reaction * );

/*##############################################################################
// <routine name="Libnucnet__Reaction__copy()">
//
//   <description>
//     <abstract>
//       Copy a Libnucnet__Reaction.
//     </abstract>
//     <keywords>
//       Libnucnet, nuclear, data, reaction, copy
//     </keywords>
//   </description>
//
//   <usage>
//
//     <calling_sequence>
// Libnucnet__Reaction *
// Libnucnet__Reaction__copy( const Libnucnet__Reaction *p_reaction );
//     </calling_sequence>
//
//     <param
//       name="p_reaction"
//       kind="in,positional,required"
//     >
//       A pointer to the reaction to be copied.
//     </param>
//     <param
//       kind="return"
//       doc="result"
//     />
//
//     <doc kind="post" id="result">
//       The routine returns a pointer to a new Libnucnet__Reaction structure
//       that is a copy of the input structure.
//       If it is not possible to allocate memory for the new structure,
//       error handling is invoked.
//     </doc>
//
//     <doc kind="bug" >
//     </doc>
//
//     <doc kind="example" id="example">
//       <synopsis>
//         Create a copy of p_reaction:
//       </synopsis>
//
//       <code>
// p_copy = Libnucnet__Reaction__copy( p_reaction );
//       </code>
//     </doc>
//
//   </usage>
//
// </routine>
//############################################################################*/

Libnucnet__Reaction *
Libnucnet__Reaction__copy( const Libnucnet__Reaction * );

/*##############################################################################
// <routine name="Libnucnet__Reac__setReactionCompareFunction()">
//
//   <description>
//     <abstract>
//       Set the function to sort the reactions during a reaction
//       iteration.
//     </abstract>
//     <keywords>
//       Libnucnet, nuclear, xml, sort
//     </keywords>
//   </description>
//
//   <usage>
//
//     <calling_sequence>
// void
// Libnucnet__Reac__setReactionCompareFunction(
//   Libnucnet__Reac *self,
//   Libnucnet__Reaction__compare_function pfFunc
// );
//     </calling_sequence>
//
//     <param
//       name="self" 
//       kind="in,positional,required" 
//     >
//       A pointer to a Libnucnet__Reac structure.
//     </param>
//
//     <param
//       name="pfFunc" 
//       kind="in,positional,required" 
//     >
//       The name of the user-supplied function to apply.
//     </param>
//     <param
//       kind="return"
//       doc="result"
//     />
//
//     <doc kind="post" id="result">
//       Upon successful return, the data compare function for reactions
//       in the collection has been set to the input function.
//       If any input is invalid, error handling is invoked.
//     </doc>
//
//     <doc kind="bug" >
//     </doc>
//
//     <doc kind="example" id="example1">
//       <synopsis>
//         Set the reaction compare function in p_my_reac to my_sort_function:
//       </synopsis>
//
//       <code>
// Libnucnet__Reac__setReactionCompareFunction(
//   p_my_reac,
//   (Libnucnet__Reaction__compare_function) my_sort_function
// );
//       </code>
//     </doc>
//
//   </usage>
//
// </routine>
//############################################################################*/

void
Libnucnet__Reac__setReactionCompareFunction(
  Libnucnet__Reac *,
  Libnucnet__Reaction__compare_function
);

/*##############################################################################
// <routine name="Libnucnet__Reac__clearReactionCompareFunction()">
//
//   <description>
//     <abstract>
//       Restore the default function to sort reactions during a
//       reaction iteration.
//     </abstract>
//     <keywords>
//       Libnucnet, nuclear, reaction, xml, sort
//     </keywords>
//   </description>
//
//   <usage>
//
//     <calling_sequence>
// void
// Libnucnet__Reac__clearReactionCompareFunction(
//   Libnucnet__Reac *self
// );
//     </calling_sequence>
//
//     <param
//       name="self" 
//       kind="in,positional,required" 
//     >
//       A pointer to a Libnucnet__Reac structure.
//     </param>
//
//     <param
//       kind="return"
//       doc="result"
//     />
//
//     <doc kind="post" id="result">
//       Upon successful return, the reaction compare function for
//       the collection has been restored to the default function.
//       If any input is invalid, error handling is invoked.
//     </doc>
//
//     <doc kind="bug" >
//     </doc>
//
//     <doc kind="example" id="example1">
//       <synopsis>
//         Clear the reaction compare function in p_my_reactions:
//       </synopsis>
//
//       <code>
// Libnucnet__Reac__clearReactionCompareFunction(
//   p_my_reactions
// );
//       </code>
//     </doc>
//
//   </usage>
//
// </routine>
//############################################################################*/

void
Libnucnet__Reac__clearReactionCompareFunction(
  Libnucnet__Reac *
);

/*##############################################################################
// <routine name="Libnucnet__Reac__registerUserRateFunction()">
//
//   <description>
//     <abstract>
//       Register a function to be used in rate calculations.
//     </abstract>
//     <keywords>
//       Libnucnet, nuclear, reaction, rate, function, register
//     </keywords>
//   </description>
//
//   <usage>
//
//     <calling_sequence>
// int
// Libnucnet__Reac__registerUserRateFunction(
//   Libnucnet__Reac *self,
//   const char *s_function_key,
//   Libnucnet__Reaction__userRateFunction pf_func
// );
//     </calling_sequence>
//
//     <param
//       name="self" 
//       kind="in,positional,required" 
//     >
//       A pointer to a Libnucnet__Reac structure.
//     </param>
//
//     <param
//       name="s_function_key" 
//       kind="in,positional,required" 
//     >
//       A string giving the key for the function.
//     </param>
//
//     <param
//       name="pf_func" 
//       kind="in,positional,required" 
//     >
//       The name of the function to be registered. 
//     </param>
//
//     <param
//       kind="return"
//       doc="result"
//     />
//
//     <doc kind="post" id="result">
//       Routine returns 1 (true) if registration successful or 0 (false)
//       if not.  If the input structure is invalid, error handling is invoked.
//     </doc>
//
//     <doc kind="bug" >
//     </doc>
//
//     <doc kind="example" id="example1">
//       <synopsis>
//         Register the function my_function with key "my rate function"
//         in p_my_reactions:
//       </synopsis>
//
//       <code>
// Libnucnet__Reac__registerUserRateFunction(
//   p_my_reactions,
//   "my rate function",
//   (Libnucnet__Reaction__userRateFunction) my_function
// );
//       </code>
//     </doc>
//
//   </usage>
//
// </routine>
//############################################################################*/

int
Libnucnet__Reac__registerUserRateFunction(
  Libnucnet__Reac *,
  const char *,
  Libnucnet__Reaction__userRateFunction
);

/*##############################################################################
// <routine name="Libnucnet__Reac__setUserRateFunctionDataDeallocator()">
//
//   <description>
//     <abstract>
//       Routine to set the deallocator for the data for a user-defined
//       rate function.
//     </abstract>
//     <keywords>
//       Libnucnet, user, supplied, function, data, compare, reaction
//     </keywords>
//   </description>
//
//   <usage>
//
//     <calling_sequence>
// int
// Libnucnet__Reac__setUserRateFunctionDataDeallocator(
//   Libnucnet__Reac * self,
//   const char * s_function_key,
//   Libnucnet__Reaction__user_rate_function_data_deallocator
//      pf_deallocator
// );
//     </calling_sequence>
//
//     <param
//       name="self"
//       kind="in,positional,required"
//     >
//       A pointer to a reaction collection.
//     </param>
//
//     <param
//       name="s_function_key"
//       kind="in,positional,required"
//     >
//       The key to the previously register rate function.
//     </param>
//
//     <param
//       name="pf_deallocator"
//       kind="in,positional,required"
//     >
//       The pointer to the data deallocator.
//     </param>
//
//     <param
//       kind="return"
//       doc="result"
//     />
//
//     <doc kind="post" id="result">
//       Routine returns 1 (true) if the deallocator was successfully set
//       and 0 (false) if not.
//     </doc>
//
//     <doc kind="bug" >
//     </doc>
//
//     <doc kind="example" id="example1">
//       <synopsis>
//         Set the deallocator for the function with key "my rate function"
//         in p_my_reactions to my_deallocator:
//       </synopsis>
//
//       <code>
// Libnucnet__Reac__setUserRateFunctionDataDeallocator(
//   p_my_reactions,
//   "my rate function",
//   (Libnucnet__Reaction__user_rate_function_data_deallocator) my_deallocator
// );
//       </code>
//     </doc>
//
//   </usage>
//
// </routine>
//############################################################################*/

int
Libnucnet__Reac__setUserRateFunctionDataDeallocator(
  Libnucnet__Reac *,
  const char *,
  Libnucnet__Reaction__user_rate_function_data_deallocator
);

/*##############################################################################
// <routine name="Libnucnet__Reaction__updateUserRateFunctionProperty()">
//
//   <description>
//     <abstract>
//       Update a rate function property for a reaction.
//     </abstract>
//     <keywords>
//       Libnucnet, nuclear, reaction, rate, function, property, update
//     </keywords>
//   </description>
//
//   <usage>
//
//     <calling_sequence>
// int
// Libnucnet__Reaction__updateUserRateFunctionProperty(
//   Libnucnet__Reaction *self,
//   const char *s_name,
//   const char *s_tag1,
//   const char *s_tag2,
//   const char *s_value
// );
//     </calling_sequence>
//
//     <param
//       name="self" 
//       kind="in,positional,required" 
//     >
//       A pointer to a Libnucnet__Reac structure.
//     </param>
//
//     <param
//       name="s_name"
//       kind="in,positional,required" 
//     >
//       A string giving the name of the property.
//     </param>
//
//     <param
//       name="s_tag1"
//       kind="in,positional,required" 
//     >
//       A string giving the first tag of the property (could be NULL).
//     </param>
//
//     <param
//       name="s_tag2"
//       kind="in,positional,required" 
//     >
//       A string giving the second tag of the property (could be NULL).
//     </param>
//
//     <param
//       name="s_value"
//       kind="in,positional,required" 
//     >
//       A string giving the new value of the property.
//     </param>
//
//     <param
//       kind="return"
//       doc="result"
//     />
//
//     <doc kind="post" id="result">
//       Routine returns 1 (true) if update successful and 0 (false) if not.
//       If the input structure is invalid, error handling is invoked.
//     </doc>
//
//     <doc kind="bug" >
//     </doc>
//
//     <doc kind="example" id="example1">
//       <synopsis>
//         Update the data for the property with name "prop", tag1
//         "0", and no tag2 for reaction p_my_reaction with "new value":
//       </synopsis>
//
//       <code>
// if(
//   Libnucnet__Reaction__updateUserRateFunctionProperty(
//     p_my_reaction,
//     "prop",
//     "0",
//     NULL,
//     "new value"
//   )
// )
//   printf( "Successful update!\n" );
//       </code>
//     </doc>
//
//   </usage>
//
// </routine>
//############################################################################*/

int
Libnucnet__Reaction__updateUserRateFunctionProperty(
  Libnucnet__Reaction *,
  const char *,
  const char *,
  const char *,
  const char *
);

/*##############################################################################
// <routine name="Libnucnet__Reaction__removeUserRateFunctionProperty()">
//
//   <description>
//     <abstract>
//       Remove a rate function property for a reaction.
//     </abstract>
//     <keywords>
//       Libnucnet, nuclear, reaction, rate, function, property, remove
//     </keywords>
//   </description>
//
//   <usage>
//
//     <calling_sequence>
// int
// Libnucnet__Reac__removeUserRateFunctionProperty(
//   Libnucnet__Reaction *self,
//   const char *s_name,
//   const char *s_tag1,
//   const char *s_tag2
// );
//     </calling_sequence>
//
//     <param
//       name="self" 
//       kind="in,positional,required" 
//     >
//       A pointer to a Libnucnet__Reac structure.
//     </param>
//
//     <param
//       name="s_name"
//       kind="in,positional,required" 
//     >
//       A string giving the name of the property.
//     </param>
//
//     <param
//       name="s_tag1"
//       kind="in,positional,required" 
//     >
//       A string giving the first tag of the property (could be NULL).
//     </param>
//
//     <param
//       name="s_tag2"
//       kind="in,positional,required" 
//     >
//       A string giving the second tag of the property (could be NULL).
//     </param>
//
//     <param
//       kind="return"
//       doc="result"
//     />
//
//     <doc kind="post" id="result">
//       Routine returns 1 (true) if update successful and 0 (false) if not.
//       If the input structure is invalid, error handling is invoked.
//     </doc>
//
//     <doc kind="bug" >
//     </doc>
//
//     <doc kind="example" id="example1">
//       <synopsis>
//         Remove the property with name "prop", tag1
//         "0", and no tag2 for reaction p_my_reaction:
//       </synopsis>
//
//       <code>
// if(
//   Libnucnet__Reaction__removeUserRateFunctionProperty(
//     p_my_reaction,
//     "prop",
//     "0",
//     NULL
//   )
// )
//   printf( "Successful removal!\n" );
//       </code>
//     </doc>
//
//   </usage>
//
// </routine>
//############################################################################*/

int
Libnucnet__Reaction__removeUserRateFunctionProperty(
  Libnucnet__Reaction *,
  const char *,
  const char *,
  const char *
);

/*##############################################################################
// <routine name="Libnucnet__Reaction__setUserRateFunctionKey()">
//
//   <description>
//     <abstract>
//       Set the user-supplied rate function for a reaction.
//     </abstract>
//     <keywords>
//       Libnucnet, nuclear, reaction, rate, function, register
//     </keywords>
//   </description>
//
//   <usage>
//
//     <calling_sequence>
// void
// Libnucnet__Reaction__setUserRateFunctionKey(
//   Libnucnet__Reaction *self,
//   const char *s_function_key
// );
//     </calling_sequence>
//
//     <param
//       name="self" 
//       kind="in,positional,required" 
//     >
//       A pointer to a Libnucnet__Reaction structure.
//     </param>
//
//     <param
//       name="s_function_key" 
//       kind="in,positional,required" 
//     >
//       A string giving the key for the function.
//     </param>
//
//     <param
//       kind="return"
//       doc="result"
//     />
//
//     <doc kind="post" id="result">
//       Upon successful return, the rate function key for the reaction has
//       been set to the input.
//       If the input structure is invalid, error handling is invoked.
//     </doc>
//
//     <doc kind="bug" >
//     </doc>
//
//     <doc kind="example" id="example1">
//       <synopsis>
//         For p_reaction, set the rate function for the reaction to
//         my_function with key "my rate function":
//       </synopsis>
//
//       <code>
// Libnucnet__Reaction__setUserRateFunctionKey(
//   p_reaction,
//   "my rate function"
// );
//       </code>
//     </doc>
//
//   </usage>
//
// </routine>
//############################################################################*/

void
Libnucnet__Reaction__setUserRateFunctionKey(
  Libnucnet__Reaction *,
  const char *
);

/*##############################################################################
// <routine name="Libnucnet__Reaction__getUserRateFunctionProperty()">
//
//   <description>
//     <abstract>
//       Get a rate function property for a reaction.
//     </abstract>
//     <keywords>
//       Libnucnet, nuclear, reaction, rate, function, property, get
//     </keywords>
//   </description>
//
//   <usage>
//
//     <calling_sequence>
// const char *
// Libnucnet__Reaction__getUserRateFunctionProperty(
//   const Libnucnet__Reaction *self,
//   const char *s_name,
//   const char *s_tag1,
//   const char *s_tag2
// );
//     </calling_sequence>
//
//     <param
//       name="self" 
//       kind="in,positional,required" 
//     >
//       A pointer to a Libnucnet__Reaction structure.
//     </param>
//
//     <param
//       name="s_name"
//       kind="in,positional,required" 
//     >
//       A string giving the name of the property.
//     </param>
//
//     <param
//       name="s_tag1"
//       kind="in,positional,required" 
//     >
//       A string giving the first tag of the property (could be NULL).
//     </param>
//
//     <param
//       name="s_tag2"
//       kind="in,positional,required" 
//     >
//       A string giving the second tag of the property (could be NULL).
//     </param>
//
//     <doc
//       kind="pre"
//       id="self"
//     >
//       Reaction must be a user-supplied one (for which properties are
//       defined).
//     </doc>
//
//     <param
//       kind="return"
//       doc="result"
//     />
//
//     <doc kind="post" id="result">
//       Routine returns the value of a rate function property.
//       If the input reaction structure is invalid, error handling is invoked.
//     </doc>
//
//     <doc kind="bug" >
//     </doc>
//
//     <doc kind="example" id="example1">
//       <synopsis>
//         Print the value for the property with name "prop", tag1
//         "8", and no tag2 for reaction p_my_reaction:
//       </synopsis>
//
//       <code>
// printf(
//   "%s\n",
//   Libnucnet__Reaction__getUserRateFunctionProperty(
//     p_my_reaction,
//     "prop",
//     "8",
//     NULL
//   )
//       </code>
//     </doc>
//   </usage>
//
// </routine>
//############################################################################*/

const char *
Libnucnet__Reaction__getUserRateFunctionProperty(
  const Libnucnet__Reaction *,
  const char *,
  const char *,
  const char *
);

/*##############################################################################
// <routine name="Libnucnet__Reaction__iterateUserRateFunctionProperties()">
//
//   <description>
//     <abstract>
//       Iterate over the properties of a user-supplied rate function.
//     </abstract>
//     <keywords>
//       Libnucnet, property, iterate, reaction
//     </keywords>
//   </description>
//
//   <usage>
//
//     <calling_sequence>
// void
// Libnucnet__Reaction__iterateUserRateFunctionProperties(
//   const Libnucnet__Reaction *self,
//   const char *s_name,
//   const char *s_tag1,
//   const char *s_tag2,
//   Libnucnet__Reaction__user_rate_property_iterate_function
//     pf_func,
//   void *p_user_data
// );
//     </calling_sequence>
//
//     <param
//       name="self" 
//       kind="in,positional,required" 
//     >
//       A pointer to a Libnucnet__Reaction structure.
//     </param>
//
//     <param
//       name="s_name" 
//       kind="in,positional,required" 
//     >
//       The name of the property.
//     </param>
//     <param
//       name="s_tag1" 
//       kind="in,positional,required" 
//     >
//       The name of a tag for the property containing extra information
//       about the property.
//     </param>
//     <param
//       name="s_tag2" 
//       kind="in,positional,required" 
//     >
//       The name of a second tag for the property containing extra information
//       about the property.
//     </param>
//
//     <param
//       name="pf_func" 
//       kind="in,positional,required" 
//     >
//       A pointer to the function to be applied during the iteration.
//     </param>
//     <param
//       name="p_user_data" 
//       kind="in,positional,required" 
//     >
//       A pointer to the user-supplied extra data for the iterate function.
//     </param>
//
//     <param
//       kind="return"
//       doc="result"
//     />
//
//     <doc
//       kind="pre"
//       id="self"
//     >
//       Reaction must be a user-supplied one (for which properties are
//       defined).
//     </doc>
//
//     <doc kind="post" id="result">
//       Routine iterates over the rate function properties in the reaction and
//       applies the user-supplied function.  The properties iterated over
//       are those that match s_name, s_tag1, and s_tag2.  If any of these
//       is NULL, the comparison is a match.  The properties are iterated
//       in the order in which they are internally stored.  The user does not
//       have control over this order.
//     </doc>
//
//     <doc kind="bug" >
//     </doc>
//
//     <doc kind="example" id="example1">
//       <synopsis>
//         Iterate over the all rate function properties in p_reaction and
//         apply my_func and the extra data pointed to by p_user_data:
//       </synopsis>
//
//       <code>
// Libnucnet__Reaction__iterateUserRateFunctionProperties(
//   p_reaction,
//   NULL,
//   NULL,
//   NULL,
//   (Libnucnet__Reaction__user_rate_property_iterate_function) my_func,
//   p_user_data
// );
//       </code>
//     </doc>
//
//     <doc kind="example" id="example2">
//       <synopsis>
//         Iterate over all the rate function properties that match "t9"
//         as a name in p_reaction and
//         apply my_func and the extra data pointed to by p_user_data:
//       </synopsis>
//
//       <code>
// Libnucnet__Reaction__iterateUserRateFunctionProperties(
//   p_reaction,
//   "t9",
//   NULL,
//   NULL,
//   (Libnucnet__Reaction__user_rate_property_iterate_function) my_func,
//   p_user_data
// );
//       </code>
//     </doc>
//   </usage>
//
// </routine>
//############################################################################*/

void
Libnucnet__Reaction__iterateUserRateFunctionProperties(
  const Libnucnet__Reaction *,
  const char *,
  const char *,
  const char *,
  Libnucnet__Reaction__user_rate_property_iterate_function,
  void *
);

/*##############################################################################
// <routine name="Libnucnet__Reaction__getRateFunctionKey()">
//
//   <description>
//     <abstract>
//       Get the key for a rate function for a reaction.
//     </abstract>
//     <keywords>
//       Libnucnet, nuclear, reaction, rate, function, key, get
//     </keywords>
//   </description>
//
//   <usage>
//
//     <calling_sequence>
// const char *
// Libnucnet__Reaction__getRateFunctionKey(
//   const Libnucnet__Reaction *self
// );
//     </calling_sequence>
//
//     <param
//       name="self" 
//       kind="in,positional,required" 
//     >
//       A pointer to a Libnucnet__Reaction structure.
//     </param>
//
//     <param
//       kind="return"
//       doc="result"
//     />
//
//     <doc kind="post" id="result">
//       Routine returns a string giving the key for a rate function.
//       If the input reaction structure is invalid, error handling is invoked.
//     </doc>
//
//     <doc kind="bug" >
//     </doc>
//
//     <doc kind="example" id="example1">
//       <synopsis>
//         Print the key for reaction p_my_reaction:
//       </synopsis>
//
//       <code>
// printf(
//   "%s\n",
//   Libnucnet__Reaction__getRateFunctionKey(
//     p_my_reaction
//   )
// );
//       </code>
//     </doc>
//   </usage>
//
// </routine>
//############################################################################*/

const char *
Libnucnet__Reaction__getRateFunctionKey(
  const Libnucnet__Reaction *
);

/*##############################################################################
// <routine name="Libnucnet__ReacView__new()">
//
//   <description>
//     <abstract>
//       Create a new Libnucnet__ReacView structure.
//     </abstract>
//     <keywords>
//       Libnucnet, nuclear, data, xml, view
//     </keywords>
//   </description>
//
//   <usage>
//
//     <calling_sequence>
// Libnucnet__ReacView *
// Libnucnet__ReacView__new(
//   const Libnucnet__Reac * p_reac,
//   const char * s_reac_xpath
// );
//     </calling_sequence>
//
//     <param
//       name="p_reac"
//       kind="in,positional,required"
//     >
//       A pointer to a reaction collection.
//     </param>
//
//     <param
//       name="s_reac_xpath"
//       kind="in,positional,required"
//     >
//       A string giving the XPath expression that definies the reactions
//       to be included in the view.
//     </param>
//
//     <param
//       kind="return"
//       doc="result"
//     />
//
//     <doc kind="post" id="result">
//       The routine returns a pointer to a new Libnucnet__ReacView structure,
//       that is, a view of a parent reaction collection.
//       If it is not possible to allocate memory for the new structure,
//       Libnucnet error handling is invoked.
//     </doc>
//
//     <doc kind="bug" >
//     </doc>
//
//     <doc kind="example" id="example">
//       <synopsis>
//         Create a view of the reactions involving reactant carbon-12 in 
//         p_my_reactions:
//       </synopsis>
//
//       <code>
// p_view =
//   Libnucnet__ReacView__new(
//     p_my_reactions,
//     "[reactant = 'c12']"
// );
//       </code>
//     </doc>
//
//   </usage>
//
// </routine>
//############################################################################*/

Libnucnet__ReacView *
Libnucnet__ReacView__new(
  const Libnucnet__Reac *,
  const char *
);

/*##############################################################################
// <routine name="Libnucnet__ReacView__free()">
//
//   <description>
//     <abstract>
//       Free the memory allocated for a view of a Libnucnet reaction
//       collection.
//     </abstract>
//     <keywords>
//       nuclear, data, remove, delete, free
//     </keywords>
//   </description>
//
//   <usage>
//
//     <calling_sequence>
// void
// Libnucnet__ReacView__free( Libnucnet__ReacView *self );
//     </calling_sequence>
//
//     <param
//       name="self"
//       kind="in,positional,required"
//     >
//       A pointer to a view of a Libnucnet reaction collection.
//     </param>
//
//     <param
//       kind="return"
//       doc="result"
//     />
//
//     <doc kind="post" id="result">
//       Upon successful return, the memory for the view has been freed.
//     </doc>
//
//     <doc kind="bug" >
//     </doc>
//
//     <doc kind="example" id="example">
//       <synopsis>
//         Free up memory used for Libnucnet__ReacView *p_my_view:
//       </synopsis>
//
//       <code>
// Libnucnet__ReacView__free( p_my_view );
//       </code>
//     </doc>
//
//   </usage>
//
// </routine>
//############################################################################*/

void
Libnucnet__ReacView__free(
  Libnucnet__ReacView *
);

/*##############################################################################
// <routine name="Libnucnet__ReacView__getReac()">
//
//   <description>
//     <abstract>
//       Get the reaction collection in a view.
//     </abstract>
//     <keywords>
//       nuclear, view, collection, reaction
//     </keywords>
//   </description>
//
//   <usage>
//
//     <calling_sequence>
// Libnucnet__Reac *
// Libnucnet__ReacView__getReac( Libnucnet__ReacView *self );
//     </calling_sequence>
//
//     <param
//       name="self"
//       kind="in,positional,required"
//     >
//       A pointer to a view of a Libnucnet reaction collection.
//     </param>
//
//     <param
//       kind="return"
//       doc="result"
//     />
//
//     <doc kind="post" id="result">
//       Routine returns the reaction collection from a Libnucnet__ReacView.
//       If the input view is invalid, error handling is invoked.
//     </doc>
//
//     <doc kind="bug" >
//     </doc>
//
//     <doc kind="example" id="example">
//       <synopsis>
//         Get the collection from Libnucnet__ReacView *p_my_view:
//       </synopsis>
//
//       <code>
// p_reac = Libnucnet__ReacView__getReac( p_my_view );
//       </code>
//     </doc>
//
//   </usage>
//
// </routine>
//############################################################################*/

Libnucnet__Reac *
Libnucnet__ReacView__getReac( Libnucnet__ReacView * );

/*##############################################################################
// <routine name="Libnucnet__ReacView__getXPath()">
//
//   <description>
//     <abstract>
//       Get the XPath expression defining a view.
//     </abstract>
//     <keywords>
//       nuclear, view, collection, reaction, XPath
//     </keywords>
//   </description>
//
//   <usage>
//
//     <calling_sequence>
// const char *
// Libnucnet__ReacView__getReac( Libnucnet__ReacView *self );
//     </calling_sequence>
//
//     <param
//       name="self"
//       kind="in,positional,required"
//     >
//       A pointer to a view of a Libnucnet reaction collection.
//     </param>
//
//     <param
//       kind="return"
//       doc="result"
//     />
//
//     <doc kind="post" id="result">
//       Routine returns a string giving the XPath expression that defined
//       the view. If the input view is invalid, error handling is invoked.
//     </doc>
//
//     <doc kind="bug" >
//     </doc>
//
//     <doc kind="example" id="example">
//       <synopsis>
//         Print the XPath expression defining a Libnucnet__ReacView *p_my_view:
//       </synopsis>
//
//       <code>
// printf(
//   "This is the view defined by %s.\n",
//   Libnucnet__ReacView__getXPath( p_my_view )
// );
//       </code>
//     </doc>
//
//   </usage>
//
// </routine>
//############################################################################*/

const char *
Libnucnet__ReacView__getXPath( Libnucnet__ReacView * );

/*##############################################################################
// Non-API routines.
//############################################################################*/

void
Libnucnet__Reac__updateFromXmlDocument(
  Libnucnet__Reac *,xmlDocPtr, const char *
);

void
Libnucnet__Reaction__Element__free(
  xmlLinkPtr
);

Libnucnet__Reaction *
Libnucnet__Reac__new_reaction_from_xml_node(
  xmlNode *
);

Libnucnet__Reaction__RateData *
Libnucnet__Reaction__RateData__new( void );

void Libnucnet__Reaction__hashFree( Libnucnet__Reaction *, xmlChar * );

int Libnucnet__Reac__assignReaction( Libnucnet__Reac *, xmlNode * );

void
Libnucnet__Reaction__addRateDataFromXml( Libnucnet__Reaction *, xmlNodePtr );

void
Libnucnet__Reaction__updateRateTableFromXml( Libnucnet__Reaction *, xmlNode * );

void
Libnucnet__Reaction__addNonSmokerFromXml( Libnucnet__Reaction *, xmlNodePtr );

void
Libnucnet__Reaction__addNonSmokerFitFromXml(
  Libnucnet__Reaction *,
  xmlNodePtr
);

void
Libnucnet__Reaction__updateUserRateFromXml( Libnucnet__Reaction *, xmlNode * );

void Libnucnet__Reaction__updateString( Libnucnet__Reaction * );

double
Libnucnet__Reaction__getSingleRate(
  const Libnucnet__Reaction *, double, void *
);

double
Libnucnet__Reaction__computeRateFromTable(
  Libnucnet__Reaction *, double, void *
);

double
Libnucnet__Reaction__computeNonSmokerRate(
  Libnucnet__Reaction *, double, void *
);

void
Libnucnet__Reaction__NonSmokerFit__free(
  Libnucnet__Reaction__NonSmokerFit *, xmlChar *
);

void
Libnucnet__Reaction__printNonSmokerFitCallback(
  Libnucnet__Reaction__NonSmokerFit *,
  void *,
  xmlChar *
);

Libnucnet__Reaction__NonSmokerFit *
Libnucnet__Reaction__NonSmokerFit__copier(
  Libnucnet__Reaction__NonSmokerFit *,
  xmlChar *
);

void
Libnucnet__Reac__getDuplicateReactionsCallback(
  Libnucnet__Reaction *, void *, xmlChar *
);

void
Libnucnet__Reaction__computeNonSmokerRateCallback(
  Libnucnet__Reaction__NonSmokerFit *,
  void *,
  xmlChar *
);

xmlDocPtr
Libnucnet__Reac__makeXmlDocument( const Libnucnet__Reac * );

int
Libnucnet__Reac__makeXmlDocumentReactionElementWalker(
  Libnucnet__Reaction__Element *, void *
);

int
Libnucnet__Reac__makeXmlDocumentReactionElementWalker(
  Libnucnet__Reaction__Element *, void *
);

void
Libnucnet__Reaction__freeRateData(
  Libnucnet__Reaction *
);

void
Libnucnet__Reaction__NonSmokerFit__makeXmlCallback(
  Libnucnet__Reaction__NonSmokerFit *,
  void *,
  xmlChar *
);

void
Libnucnet__Reaction__NonSmokerFit__makeXmlCallbackHelper(
  Libnucnet__Reaction__NonSmokerFit *,
  xmlNodePtr
);

int
Libnucnet__Reaction__makeXmlDocumentIterator(
  Libnucnet__Reaction *, xmlNodePtr
);

int
Libnucnet__Reac__data_compare(
  xmlChar *, xmlChar *
);

int
Libnucnet__Reaction__Element__data_compare(
  Libnucnet__Reaction__Element *, Libnucnet__Reaction__Element *
);

void
Libnucnet__Reac__assignDuplicateFactorsCallback(
  Libnucnet__Reaction *, void *, xmlChar *
);

void
Libnucnet__Reac__reactionIterateCallback(
  Libnucnet__Reaction *, void *, xmlChar *
);

void
Libnucnet__Reaction__copy_element_list( xmlListPtr, xmlListPtr );

int
Libnucnet__Reaction__copy_element_list_walker(
  Libnucnet__Reaction__Element *, xmlListPtr
);

xmlListPtr
Libnucnet__Reaction__duplicate_element_list( xmlListPtr );

int
Libnucnet__Reaction__duplicate_element_list_walker(
  Libnucnet__Reaction__Element *, xmlListPtr
);

int
Libnucnet__Reaction__Element__is_nuclide( const char * );

int
Libnucnet__Reaction__updateStringWalker(
  Libnucnet__Reaction__Element *,
  Libnucnet__Reaction *
);

int
Libnucnet__Reac__makeXmlDocumentElementWalker(
  xmlChar *, void *
);

int
Libnucnet__Reaction__isWeak( const Libnucnet__Reaction * );

int
Libnucnet__Reaction__isWeakWalker(
  Libnucnet__Reaction__Element *, int *
);

int
Libnucnet__Reaction__isBetaPlus( const Libnucnet__Reaction * );

int
Libnucnet__Reaction__isBetaPlusWalker(
  Libnucnet__Reaction__Element *, int *
);

int
Libnucnet__Reaction__isPositronCapture( const Libnucnet__Reaction * );

int
Libnucnet__Reaction__isPositronCaptureWalker(
  Libnucnet__Reaction__Element *, int *
);

int
Libnucnet__Reaction__Element__walker(
  Libnucnet__Reaction__Element *, void *
);

Libnucnet__Reaction **
Libnucnet__Reac__createReactionArray( const Libnucnet__Reac * );

void
Libnucnet__Reac__createReactionArrayCallback(
  Libnucnet__Reaction *,
  void *,
  const xmlChar *
);

int
Libnucnet__Reac__registerRateFunction(
  Libnucnet__Reac *,
  const char *,
  Libnucnet__Reaction__userRateFunction
);

int
Libnucnet__Reaction__xml_rate_function_property_walker(
  void *,
  xmlNodePtr
);

void
Libnucnet__Reaction__iterateUserRateFunctionPropertiesHelper(
  const xmlChar *,
  void *,
  const xmlChar *,
  const xmlChar *,
  const xmlChar *
);

xmlListPtr
Libnucnet__Reaction__createRateFunctionPropertyList(
  Libnucnet__Reaction *
); 

void
Libnucnet__Reaction__rate_function_property_list_maker(
  xmlChar *,
  xmlListPtr,
  xmlChar *,
  xmlChar *,
  xmlChar *
);

int
Libnucnet__Reaction__rate_function_property_data_compare(
  void *,
  void *
);

void
Libnucnet__Reaction__rate_function_property_data_deallocator(
  xmlLinkPtr
);

int
Libnucnet__Reaction__print_rate_function_property_walker(
  void *,
  void *
);

void
Libnucnet__Reac__sortReactionArray(
  const Libnucnet__Reac *, Libnucnet__Reaction **
);

int
Libnucnet__Reac__sort_helper(
  const void *, const void *
);

int
Libnucnet__Reac__is_user_defined_rate_function(
  const char *
);

int
Libnucnet__Reaction__hasUserDefinedRateFunction(
  const Libnucnet__Reaction *
);

Libnucnet__Reac *
Libnucnet__Reac__createNewReacForView(
  const Libnucnet__Reac *
);

void
Libnucnet__Reac__addReactionsToViewFromXml(
  const Libnucnet__Reac *,
  xmlHashTablePtr,
  xmlDocPtr,
  const char *
);

#endif
