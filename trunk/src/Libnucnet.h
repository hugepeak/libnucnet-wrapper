/*//////////////////////////////////////////////////////////////////////////////
// <file type="public">
// <license>
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

#ifndef LIBNUCNET_H
#define LIBNUCNET_H

/*##############################################################################
// Module #include's.
//############################################################################*/

#include "Libnucnet__Nuc.h"
#include "Libnucnet__Reac.h"
#include "WnMatrix.h"

/*##############################################################################
// String definitions.
//############################################################################*/

#define COLON ":"
#define DOUBLE_SLASH "//"
#define LABEL_1 "label1"
#define LABEL_2 "label2"
#define LABEL_3 "label3"
#define LIBNUCNET_INPUT "libnucnet_input"
#define MASS_FRACTIONS "mass_fractions"
#define MASS_FRACTION "x"
#define NET_EMPTY_STRING ""
#define NUCLEAR_NETWORK "nuclear_network"
#define OPTIONAL_PROPERTIES "optional_properties"
#define PROPERTY "property"
#define PROPERTY_NAME "name"
#define PROPERTY_TAG1 "tag1"
#define PROPERTY_TAG2 "tag2"
#define XPATH_REAC_EVOLVE "xpath reaction evolve"
#define XPATH_ZONE_DATA ZONE_DATA "/" ZONE
#define XPATH_ZONE_MASS_FRACTIONS MASS_FRACTIONS "/" NUCLIDE
#define XPATH_OPTIONAL_PROPERTY OPTIONAL_PROPERTIES "/" PROPERTY
#define ZERO "0"
#define ZONE "zone"
#define ZONE_DATA "zone_data"
#define ZONE_DEFAULT_MASS_FRACTION_FORMAT "%g"
#define ZONE_MASS_FRACTION_FORMAT "zone mass fraction format"

/*##############################################################################
// Parameter definitions.
//############################################################################*/

#define MAX_ZONE_LABELS 3
#define BUF_SIZE 256
#define D_LARGE  115.13 /* Cut off for too large a reverse reaction rate */

/*##############################################################################
// XML Schema Info.
//############################################################################*/

#define NET_XSD_VERSION "2011-05-28"

#define LIBNUCNET__PREFIX \
  "http://libnucnet.sourceforge.net/xsd_pub/" NET_XSD_VERSION "/"

#define NET__SCHEMA "libnucnet__net.xsd"
#define ZONE_DATA__SCHEMA "zone_data.xsd"
#define SCHEMA "libnucnet.xsd"

#define LIBNUCNET__NET__SCHEMA LIBNUCNET__PREFIX NET__SCHEMA
#define LIBNUCNET__MASS__FRACTIONS__SCHEMA \
   LIBNUCNET__PREFIX MASS__FRACTIONS__SCHEMA
#define LIBNUCNET__SCHEMA LIBNUCNET__PREFIX SCHEMA
#define LIBNUCNET__ZONE_DATA__SCHEMA \
  LIBNUCNET__PREFIX ZONE_DATA__SCHEMA
#define ZONE_DATA__NAMESPACE "zone_data/"
#define LIBNUCNET__ZONE_DATA__NAMESPACE \
  LIBNUCNET__PREFIX ZONE_DATA__NAMESPACE
#define LIBNUCNET__ZONE_DATA__SCHEMALOCATION1 \
  LIBNUCNET__ZONE_DATA__NAMESPACE " "
#define LIBNUCNET__ZONE_DATA__SCHEMALOCATION \
  LIBNUCNET__ZONE_DATA__SCHEMALOCATION1 LIBNUCNET__ZONE_DATA__SCHEMA

/*##############################################################################
// Macro definitions.
//############################################################################*/

/* To debug, uncomment the following line. */
#define debug

#ifndef debug

#define LIBNUCNET__ERROR( s_reason ) \
       do { \
         fprintf( stderr, "ERROR: %s in %s on line %d\n", \
           s_reason, __FILE__, __LINE__ \
         ); \
         exit( EXIT_FAILURE ); \
       } while (0)


#else

#define LIBNUCNET__ERROR( s_reason ) \
       do { \
         fprintf( stderr, "ERROR: %s in %s on line %d\n", \
           s_reason, __FILE__, __LINE__ \
         ); \
         abort(); \
       } while (0)

#endif

/*##############################################################################
// Structures and user-supplied functions.
//############################################################################*/

/*##############################################################################
// <user_routine name="Libnucnet__Net__screeningFunction()">
//
//   <description>
//     <abstract>
//       Optional user-supplied screening function.
//     </abstract>
//     <keywords>
//       Libnucnet, screen, user, supplied, function
//     </keywords>
//   </description>
//
//   <usage>
//
//     <calling_sequence>
// double
// Libnucnet__Net__screeningFunction(
//   double d_t9,
//   double d_rho,
//   double d_ye,
//   unsigned int i_Z1,
//   unsigned int i_A1,
//   unsigned int i_Z2,
//   unsigned int i_A2,
//   void *p_user_data
// );
//     </calling_sequence>
//
//     <param
//       name="d_t9"
//       kind="in,positional,required"
//     >
//       A double giving the temperature in billions of K at which to
//       compute the screening factor.
//     </param>
//
//     <param
//       name="d_rho"
//       kind="in,positional,required"
//     >
//       A double giving the density in g/cc at which to compute the
//       screening factor.
//     </param>
//
//     <param
//       name="d_ye"
//       kind="in,positional,required"
//     >
//       A double giving the electron-to-baryon ratio Ye at which to compute the
//       screening factor.
//     </param>
//
//     <param
//       name="i_Z1"
//       kind="in,positional,required"
//     >
//       An unsigned int giving the atomic number of the first reactant.
//     </param>
//
//     <param
//       name="i_A1"
//       kind="in,positional,required"
//     >
//       An unsigned int giving the mass number of the first reactant.
//     </param>
//
//     <param
//       name="i_Z2"
//       kind="in,positional,required"
//     >
//       An unsigned int giving the atomic number of the second reactant.
//     </param>
//
//     <param
//       name="i_A2"
//       kind="in,positional,required"
//     >
//       An unsigned int giving the mass number of the second reactant.
//     </param>
//
//     <param
//       name="p_user_data"
//       kind="in,positional,required"
//     >
//       A pointer to a user-defined structure containing extra data for
//       the screening routine.
//     </param>
//
//     <param
//       kind="return"
//       doc="result"
//     />
//
//     <doc kind="post" id="result">
//       User's routine must return a double giving the screening
//       factor for the reaction (i_Z1,i_A1) + (i_Z2,i_A2) at the input
//       temperature, density, and Ye.  The user can call this routine
//       by passing it into Libnucnet__Net__computeScreeningFactorForReaction()
//       or can set it with Libnucnet__Zone__setScreeningFunction for
//       libnucnet to use from Libnucnet__Zone__computeRates().
//     </doc>
//
//     <doc kind="bug" >
//     </doc>
//
//   </usage>
//
// </user_routine>
//############################################################################*/

typedef double ( *Libnucnet__Net__screeningFunction ) (
  double,
  double,
  double,
  unsigned int,
  unsigned int,
  unsigned int,
  unsigned int,
  void *
);

typedef struct Libnucnet__Rates {
  double dForward;
  double dReverse;
  double dScreening;
  double dCorrection;
} Libnucnet__Rates;

/*##############################################################################
// <class name="Libnucnet__Net">
//
//   <description>
//     <abstract>
//       Libnucnet__Net is a structure that stores data about the nuclei and
//       reactions among them in a nuclear reaction network.  A
//       Libnucnet__Net structure comprises a Libnucnet__Nuc structure
//       and a Libnucnet__Reac
//       structure.  The contents of Libnucnet__Net are not made public by
//       the API but rather are accessed through the API routines.
//     </abstract>
//     <keywords>
//       nuclear, network, reactions, rates, xml, nuclei
//     </keywords>
//   </description>
//
//   <authors>
//     <current>
//       <author userid="mbradle" start_date="2007/01/19"/>
//     </current>
//   </authors>
//
//   <compatibility>
//     Compiled successfully in gcc (GCC) 3.4.0.
//   </compatibility>
//
// </class>
//############################################################################*/

typedef struct Libnucnet__Net {
  Libnucnet__Nuc *pNuc;
  Libnucnet__Reac *pReac;
} Libnucnet__Net;

/*##############################################################################
// <class name="Libnucnet__NetView">
//
//   <description>
//     <abstract>
//       Libnucnet__NetView is a structure that stores a view of
//       Libnucnet__Net structure.  A view is a subset of the parent
//       Libnucnet__Net.  The nuclei included are chosen by XPath expressions
//       as are the reactions, with the constraint that the reactions must
//       be valid for the given selection of nuclei. 
//       The view structure contains one element, a Libnucnet__Net, which
//       may be passed into all API routines that take such structures as
//       input.  It is important to note that the Libnucnet__Net member of
//       a view does not own the nuclide or reaction data, so the user must
//       be careful in modifying those data.
//     </abstract>
//     <keywords>
//       nuclear, network, reactions, rates, xml, nuclei
//     </keywords>
//   </description>
//
//   <authors>
//     <current>
//       <author userid="mbradle" start_date="2007/01/19"/>
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
  Libnucnet__Net * pNet;
  Libnucnet__Net * pParentNet;
  xmlChar * sxNucXPath;
  xmlChar * sxReacXPath;
} Libnucnet__NetView;

/*##############################################################################
// <class name="Libnucnet__Zone">
//
//   <description>
//     <abstract>
//       Libnucnet__Zone is a structure that stores data about the abundances
//       of nuclear species and the reaction rates among them in a particular
//       zone.  A zone is labelled by three strings.  If only one zone exists,
//       the default labels are "0", "0", "0"; however, the user may choose
//       other labels for the single zone.  A Libnucnet__Zone structure also
//       contains a pointer to the Libnucnet__Net structure.  A Libnucnet__Zone
//       structure thus contains data that can change with each timestep in
//       a calculation (abundances and reaction rates) along with data that is
//       essentially fixed (Libnucnet__Net data).
//       The contents of a Libnucnet__Zone structure are not made public by
//       the API.
//     </abstract>
//     <keywords>
//       nuclear, network, reactions, rates, xml, zone
//     </keywords>
//   </description>
//
//   <authors>
//     <current>
//       <author userid="mbradle" start_date="2011/05/19"/>
//     </current>
//   </authors>
//
//   <compatibility>
//     Compiled successfully in gcc (GCC) 3.4.0.
//   </compatibility>
//
// </class>
//############################################################################*/

typedef struct Libnucnet__Zone {
  Libnucnet__Net *pNet;
  xmlChar *sxLabel[MAX_ZONE_LABELS];
  xmlChar *sxNucEvolve;
  xmlChar *sxReacEvolve;
  xmlHashTablePtr pAbundanceHash;
  xmlHashTablePtr pAbundanceChangeHash;
  xmlHashTablePtr pRatesHash;
  xmlHashTablePtr pPropertyHash;
  xmlHashTablePtr pUserDataHash;
  xmlHashTablePtr pViewHash;
  Libnucnet__Net__screeningFunction pfScreeningFunction;
  Libnucnet__Species__nseCorrectionFactorFunction
    pfNseCorrectionFactorFunction;
  void *pScreeningData;
  void *pNseCorrectionFactorData;
} Libnucnet__Zone;

/*##############################################################################
// <user_routine name="Libnucnet__Zone__compare_function()">
//
//   <description>
//     <abstract>
//       User-supplied routine to sort the zones during a zone
//       iteration.
//     </abstract>
//     <keywords>
//       Libnucnet, user, supplied, function, data, compare
//     </keywords>
//   </description>
//
//   <usage>
//
//     <calling_sequence>
// int
// Libnucnet__Zone__compare_function(
//   const Libnucnet__Zone *p_zone1,
//   const Libnucnet__Zone *p_zone2
// );
//     </calling_sequence>
//
//     <param
//       name="p_zone1"
//       kind="in,positional,required"
//     >
//       A pointer to the first zone.
//     </param>
//
//     <param
//       name="p_zone2"
//       kind="in,positional,required"
//     >
//       A pointer to the second zone.
//     </param>
//
//     <param
//       kind="return"
//       doc="result"
//     />
//
//     <doc kind="post" id="result">
//       User's routine must return -1 if zone 1 is less than zone 2,
//       0 if the two zones are equal, and 1 if zone 1 is greater than
//       zone 2.  If this routine is not supplied through
//       Libnucnet__setZoneCompareFunction, the default is not to sort
//       the zones but rather to iterate them in the order in which they
//       are stored internally, which is not defined by the user. 
//     </doc>
//
//     <doc kind="bug" >
//     </doc>
//
//   </usage>
//
// </user_routine>
//############################################################################*/

typedef int ( *Libnucnet__Zone__compare_function ) (
  const Libnucnet__Zone *,
  const Libnucnet__Zone *
);

/*##############################################################################
// <class name="Libnucnet">
//
//   <description>
//     <abstract>
//       Libnucnet is a structure that stores data about nuclear reaction
//       networks.  It is in fact composed of a Libnucnet__Net structure and
//       a hash of Libnucnet__Zone structures.
//       The contents of a Libnucnet structure are not made public by
//       the API but rather are accessed through the API routines.
//     </abstract>
//     <keywords>
//       nuclear, network, reactions, rates, xml, zone
//     </keywords>
//   </description>
//
//   <authors>
//     <current>
//       <author userid="mbradle" start_date="2007/01/19"/>
//     </current>
//   </authors>
//
//   <compatibility>
//     Compiled successfully in gcc (GCC) 3.4.0.
//   </compatibility>
//
// </class>
//############################################################################*/

typedef struct Libnucnet {
  Libnucnet__Net *pNet;
  xmlHashTablePtr pZoneHash;
  Libnucnet__Zone__compare_function pfZoneCompare;
  xmlChar *sxMassFractionFormat;
} Libnucnet;

/*##############################################################################
// API Routines.
//############################################################################*/

/*##############################################################################
// <user_routine name="Libnucnet__Zone__iterateFunction()">
//
//   <description>
//     <abstract>
//       User-supplied routine to be applied during an iteration over
//       the zones.
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
// Libnucnet__Zone__iterateFunction(
//   Libnucnet__Zone *self,
//   void *p_user_data
// );
//     </calling_sequence>
//
//     <param
//       name="self"
//       kind="in,positional,required"
//     >
//       A pointer to a Libnucnet structure.
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

typedef int ( *Libnucnet__Zone__iterateFunction ) (
  Libnucnet__Zone *,
  void *
);

/*##############################################################################
// <user_routine name="Libnucnet__Zone__optional_property_iterate_function()">
//
//   <description>
//     <abstract>
//       User-supplied routine to be applied during an iteration over
//       the optional properties in a zone.
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
// Libnucnet__Zone__optional_property_iterate_function(
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
//       The name of the property.
//     </param>
//
//     <param
//       name="s_tag1"
//       kind="in,positional,required"
//     >
//       The first tag of the property.  May be NULL.
//     </param>
//
//     <param
//       name="s_tag2"
//       kind="in,positional,required"
//     >
//       The second tag of the property.  May be NULL.
//     </param>
//
//     <param
//       name="s_value"
//       kind="in,positional,required"
//     >
//       The value of the property.
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
//
//     <doc kind="bug" >
//     </doc>
//
//   </usage>
//
// </user_routine>
//############################################################################*/

typedef void ( *Libnucnet__Zone__optional_property_iterate_function ) (
  const char*, const char *, const char *, const char *, void *
);

/*##############################################################################
// <routine name="Libnucnet__new()">
//
//   <description>
//     <abstract>
//       Creates a new Libnucnet structure.
//     </abstract>
//     <keywords>
//       Libnucnet, nuclear network, xpath, xml, reactions
//     </keywords>
//   </description>
//
//   <usage>
//
//     <calling_sequence>
// Libnucnet *Libnucnet__new( );
//     </calling_sequence>
//
//     <param
//       kind="return"
//       doc="result"
//     />
//
//     <doc kind="post" id="result">
//       Routine returns a pointer to a Libnucnet
//       structure. If the routine cannot allocate sufficient memory,
//       Libnucnet error handling is invoked.
//     </doc>
//
//     <doc kind="bug" >
//     </doc>
//
//     <doc kind="example" id="example1">
//       <synopsis>
//         Create a new network structure p_my_nucnet:
//       </synopsis>
//
//       <code>
// p_my_nucnet = Libnucnet__new( );
//       </code>
//     </doc>
//
//   </usage>
//
// </routine>
//############################################################################*/

Libnucnet *Libnucnet__new( void );

/*##############################################################################
// <routine name="Libnucnet__new_from_xml()">
//
//   <description>
//     <abstract>
//       Reads in nuclear and reaction data from xml file and stores the
//       reactions in a Libnucnet structure.
//       The reactions to be stored may be selected
//       with an xpath expression.  The routine also checks that all reactants
//       and products in a reaction are included in the network and flags that
//       reaction if not.
//     </abstract>
//     <keywords>
//       Libnucnet, nuclear network, xpath, xml, reactions
//     </keywords>
//   </description>
//
//   <usage>
//
//     <calling_sequence>
// Libnucnet *
// Libnucnet__new_from_xml( 
//   const char *s_xml_filename,
//   char *s_nuc_xpath
//   char *s_reac_xpath
//   char *s_zone_xpath
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
//       name="s_nuc_xpath"
//       kind="in,positional,required"
//       doc="s_in"
//     >
//       A string giving an xpath expression for the nuclides desired.
//     </param>
//     <param
//       name="s_reac_xpath"
//       kind="in,positional,required"
//       doc="s_in"
//     >
//       A string giving an xpath expression for the reactants or products
//       in the reaction(s) desired.
//     </param>
//     <param
//       name="s_zone_xpath"
//       kind="in,positional,required"
//       doc="s_in"
//     >
//       A string giving an xpath expression for the zones desired.
//     </param>
//     <param
//       kind="return"
//       doc="result"
//     />
//
//     <doc kind="pre" id="s_in">
//       If all data are desired, this string should be empty or NULL.
//     </doc>
//     <doc kind="post" id="result">
//       If the input is valid, routine returns a pointer to a Libnucnet
//       structure containing nuclides, reactions, and zones selected by the xpath
//       expressions.  If any of the xpath expressions is invalid,
//       error handling is invoked.
//     </doc>
//
//     <doc kind="bug" >
//     </doc>
//
//     <doc kind="example" id="example1">
//       <synopsis>
//       Store all the data from "input_data.xml"
//       to p_my_nucnet:
//       </synopsis>
//
//       <code>
// p_my_nucnet =
//   Libnucnet__new_from_xml( "input_data.xml", NULL, NULL, NULL );
//       </code>
//     </doc>
//
//     <doc kind="example" id="example2">
//       <synopsis>
//       Store the data from "input_data.xml", but only those reactions,
//       containing 'h1' as a reactant, to p_my_nucnet:
//       </synopsis>
//
//       <code>
// p_my_nucnet =
//  Libnucnet__new_from_xml(
//    "input_data.xml", NULL, "[reactant = 'h1']", NULL
//  );
//       </code>
//     </doc>
//
//   </usage>
//
// </routine>
//############################################################################*/

Libnucnet *
Libnucnet__new_from_xml(
  const char *,
  const char *,
  const char *,
  const char *
);

/*##############################################################################
// <routine name="Libnucnet__Zone__new()">
//
//   <description>
//     <abstract>
//       Create a new zone.
//     </abstract>
//     <keywords>
//       nuclear network, zone, hash, abundance
//     </keywords>
//   </description>
//
//   <usage>
//
//     <calling_sequence>
// Libnucnet__Zone *
// Libnucnet__Zone__new(
//   Libnucnet__Net *p_net,
//   const char *s_label1,
//   const char *s_label2,
//   const char *s_label3
// );
//     </calling_sequence>
//
//     <param
//       name="p_net"
//       kind="in,positional,required"
//     >
//       A pointer to a network structure.
//     </param>
//     <param
//       name="s_label1"
//       kind="in,positional,required"
//     >
//       A string giving the first label of the zone. If NULL is input
//       the label will be "0"
//     </param>
//     <param
//       name="s_label2"
//       kind="in,positional,required"
//     >
//       A string giving the second label of the zone.  If NULL is input,
//       the label will be "0".
//     </param>
//     <param
//       name="s_label3"
//       kind="in,positional,required"
//     >
//       A string giving the third label of the zone.  If NULL is input,
//       the label will be "0".
//     </param>
//
//     <param
//       kind="return"
//       doc="result"
//     />
//
//     <doc kind="post" id="result">
//       Routine returns a pointer to the new zone.  If the zone could not be 
//       created, routine returns NULL.
//       If any other input is invalid, Libnucnet error handling is invoked.
//     </doc>
//
//     <doc kind="bug" >
//     </doc>
//
//     <doc kind="example" id="example">
//       <synopsis>
//       Create a new 1 dimensional zone with label "Z" with data from
//       the network p_net:
//       </synopsis>
//
//       <code>
// p_new_zone =
//   Libnucnet__Zone__new( p_net, "Z", NULL, NULL );
//       </code>
//     </doc>
//
//   </usage>
//
// </routine>
//############################################################################*/

Libnucnet__Zone *
Libnucnet__Zone__new(
  Libnucnet__Net *,
  const char *,
  const char *,
  const char *
);

/*##############################################################################
// <routine name="Libnucnet__Zone__free()">
//
//   <description>
//     <abstract>
//       Free a zone.
//     </abstract>
//     <keywords>
//       nuclear network, zone, free, abundance
//     </keywords>
//   </description>
//
//   <usage>
//
//     <calling_sequence>
// void
// Libnucnet__Zone__free(
//   Libnucnet__Zone *self
// );
//     </calling_sequence>
//
//     <param
//       name="sel"
//       kind="in,positional,required"
//     >
//       A pointer to a zone structure.
//     </param>
//
//     <param
//       kind="return"
//       doc="result"
//     />
//
//     <doc kind="post" id="result">
//       Upon successful return, the zone has been freed.
//       If the input zone is invalid, Libnucnet error handling is invoked.
//     </doc>
//
//     <doc kind="bug" >
//     </doc>
//
//     <doc kind="example" id="example">
//       <synopsis>
//       Free the zone p_zone:
//       </synopsis>
//
//       <code>
// Libnucnet__Zone__free( p_zone );
//       </code>
//     </doc>
//
//   </usage>
//
// </routine>
//############################################################################*/

void
Libnucnet__Zone__free(
  Libnucnet__Zone *
);

/*##############################################################################
// <routine name="Libnucnet__addZone()">
//
//   <description>
//     <abstract>
//       Add a zone to a Libnucnet structure.
//     </abstract>
//     <keywords>
//       nuclear network, zone, hash, abundance
//     </keywords>
//   </description>
//
//   <usage>
//
//     <calling_sequence>
// int
// Libnucnet__addZone(
//   Libnucnet *self,
//   Libnucnet__Zone *p_zone
// );
//     </calling_sequence>
//
//     <param
//       name="self"
//       kind="in,positional,required"
//     >
//       A pointer to a Libnucnet network structure.
//     </param>
//     <param
//       name="p_zone"
//       kind="in,positional,required"
//     >
//       A pointer to the zone to be added.
//     </param>
//
//     <param
//       kind="return"
//       doc="result"
//     />
//
//     <doc kind="post" id="result">
//       Routine returns 1 if the addition succeeded and 0 if not.
//       If any other input is invalid, Libnucnet error handling is invoked.
//     </doc>
//
//     <doc kind="bug" >
//     </doc>
//
//     <doc kind="example" id="example">
//       <synopsis>
//       Add p_zone to Libnucnet *p_my_nucnet:
//       </synopsis>
//
//       <code>
// if( !Libnucnet__addZone( p_my_nucnet, p_zone ) )
//   printf( "Zone addition failed!\n" );
//       </code>
//     </doc>
//
//   </usage>
//
// </routine>
//############################################################################*/

int
Libnucnet__addZone( Libnucnet *, Libnucnet__Zone * );

/*##############################################################################
// <routine name="Libnucnet__Net__new()">
//
//   <description>
//     <abstract>
//       Creates a new Libnucnet__Net structure.
//     </abstract>
//     <keywords>
//       Libnucnet, nuclear network, xpath, xml, reactions
//     </keywords>
//   </description>
//
//   <usage>
//
//     <calling_sequence>
// Libnucnet__Net *Libnucnet__Net__new( );
//     </calling_sequence>
//
//     <param
//       kind="return"
//       doc="result"
//     />
//
//     <doc kind="post" id="result">
//       Routine returns a pointer to a new Libnucnet__Net
//       structure. If the new structure cannot be created,
//       error handling is invoked.
//     </doc>
//
//     <doc kind="bug" >
//     </doc>
//
//     <doc kind="example" id="example1">
//       <synopsis>
//         Create a new network structure p_my_network:
//       </synopsis>
//
//       <code>
// p_my_network = Libnucnet__Net__new( );
//       </code>
//     </doc>
//
//   </usage>
//
// </routine>
//############################################################################*/

Libnucnet__Net *Libnucnet__Net__new( void );

/*##############################################################################
// <routine name="Libnucnet__Net__new_from_xml()">
//
//   <description>
//     <abstract>
//       Reads in nuclear and reaction data from an xml file and stores the
//       reactions in a Libnucnet__Net structure.
//       The species and the reactions to be stored may be selected
//       with an xpath expression.
//     </abstract>
//     <keywords>
//       Libnucnet, nuclear network, xpath, xml, reactions
//     </keywords>
//   </description>
//
//   <usage>
//
//     <calling_sequence>
// Libnucnet__Net *
// Libnucnet__Net__new_from_xml( 
//   const char *s_xml_filename,
//   const char *s_xpath_nuc,
//   const char *s_xpath_reac
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
//       name="s_xpath_nuc"
//       kind="in,positional,required"
//       doc="s_in_nuc"
//     >
//       A string giving an xpath expression for the nuclides desired.
//     </param>
//     <param
//       name="s_xpath_reac"
//       kind="in,positional,required"
//       doc="s_in_reac"
//     >
//       A string giving an xpath expression for the reactants or products
//       in the reaction(s) desired.
//     </param>
//     <param
//       kind="return"
//       doc="result"
//     />
//
//     <doc kind="pre" id="s_in_nuc">
//       If all reactions are required, this string should be empty or NULL.
//     </doc>
//     <doc kind="pre" id="s_in_reac">
//       If all reactions are required, this string should be empty or NULL.
//     </doc>
//     <doc kind="post" id="result">
//       If the input is valid, routine returns a pointer to a Libnucnet__Net
//       structure containing data selected by the xpath
//       expressions. If no data were found, the return is NULL.
//     </doc>
//
//     <doc kind="bug" >
//     </doc>
//
//     <doc kind="example" id="example1">
//       <synopsis>
//       Store all the data from "input_data.xml"
//       to p_my_network:
//       </synopsis>
//
//       <code>
// p_my_network =
//   Libnucnet__Net__new_from_xml(
//     "input_data.xml",
//     NULL,
//     NULL
//   );
//       </code>
//     </doc>
//
//     <doc kind="example" id="example2">
//       <synopsis>
//       Store the data from "input_data.xml", but only those nuclides
//       with Z less than 4 and only reactions
//       containing 'h1' as a reactant, to p_my_network:
//       </synopsis>
//
//       <code>
// p_my_network =
//   Libnucnet__Net__new_from_xml(
//     "input_data.xml",
//     "[z < 4]",
//     "[reactant = 'h1']"
//   );
//       </code>
//     </doc>
//
//   </usage>
//
// </routine>
//############################################################################*/

Libnucnet__Net *
Libnucnet__Net__new_from_xml(
  const char *, const char *, const char *
);

/*##############################################################################
// <routine name="Libnucnet__Net__updateFromXml()">
//
//   <description>
//     <abstract>
//       Reads in nuclear and reaction data from xml file and updates the
//       data in the input Libnucnet__Net structure. 
//       The nuclides and reactions to be stored may be selected
//       with an xpath expression.
//     </abstract>
//     <keywords>
//       Libnucnet, nuclear network, xpath, xml, reactions
//     </keywords>
//   </description>
//
//   <usage>
//
//     <calling_sequence>
// void
// Libnucnet__Net__updateFromXml(
//   Libnucnet__Net *self,
//   const char *s_xml_filename,
//   const char *s_xpath_nuc,
//   const char *s_xpath_reac
// );
//     </calling_sequence>
//
//     <param
//       name="self"
//       kind="in,positional,required"
//     >
//       A pointer to a valid, pre-existing Libnucnet__Net structure.
//     </param>
//     <param
//       name="s_xml_filename"
//       kind="in,positional,required"
//     >
//       A string giving the name of the xml file contain the reaction data.
//       This may be the name of a local file or a URL.
//     </param>
//     <param
//       name="s_xpath_nuc"
//       kind="in,positional,required"
//       doc="s_in_nuc"
//     >
//       A string giving an xpath expression for the nuclides desired.
//     </param>
//     <param
//       name="s_xpath_reac"
//       kind="in,positional,required"
//       doc="s_in_reac"
//     >
//       A string giving an xpath expression for the reactants or products
//       in the reaction(s) desired.
//     </param>
//     <param
//       kind="return"
//       doc="result"
//     />
//
//     <doc kind="pre" id="s_in_nuc">
//       If all nuclides are required, this string should be empty or NULL.
//     </doc>
//     <doc kind="pre" id="s_in_reac">
//       If all reactions are required, this string should be empty or NULL.
//     </doc>
//     <doc kind="post" id="result">
//       If the input is valid, routine updates data in the structure
//       with that contained in the input file.
//       If the input structure is invalid or the input file does not exist,
//       error handling is invoked.
//     </doc>
//
//     <doc kind="bug" >
//     </doc>
//
//     <doc kind="example" id="example1">
//       <synopsis>
//       Update the data in Libnucnet__Net *p_my_network with data from
//       "input_new_data.xml":
//       </synopsis>
//
//       <code>
// Libnucnet__Net__new_from_xml(
//   p_my_network, "input_new_data.xml", NULL, NULL
// );
//       </code>
//     </doc>
//
//     <doc kind="example" id="example2">
//       <synopsis>
//       Update the data in Libnucnet__Net *p_my_network with data for
//       nuclides with Z less than 10 from "input_new_data.xml":
//       </synopsis>
//
//       <code>
// Libnucnet__Net__updateFromXml(
//   p_my_network, "input_new_data.xml", "[z < 10]", NULL
// );
//       </code>
//     </doc>
//
//   </usage>
//
// </routine>
//############################################################################*/

void Libnucnet__Net__updateFromXml(
  Libnucnet__Net *, const char *, const char *, const char *
);

/*##############################################################################
// <routine name="Libnucnet__Net__getNumberOfValidReactions()">
//
//   <description>
//     <abstract>
//       Routine to count the number of valid reactions, that is, those
//       reactions that are between species
//       within the network.  Exclude those that are not from use in the
//       network.  Also check reactions for conservation of nucleon number,
//       charge, and lepton number.
//     </abstract>
//     <keywords>
//       Libnucnet, nuclear network, xpath, xml, reactions, lepton, charge,
//       nucleon number, baryon
//     </keywords>
//   </description>
//
//   <usage>
//
//     <calling_sequence>
// size_t
// Libnucnet__Net__getNumberOfValidReactions(
//   Libnucnet__Net *self
// );
//     </calling_sequence>
//
//     <param
//       name="self"
//       kind="in,positional,required"
//       doc="self"
//     >
//       A pointer to a valid Libnucnet__Net structure.
//     </param>
//     <param
//       kind="return"
//       doc="result"
//     />
//
//     <doc kind="post" id="result">
//       Routine returns the number of valid reactions.
//       If the input network structure is not valid, Libnucnet__Net error
//       handling is invoked.
//     </doc>
//
//     <doc kind="bug" >
//     </doc>
//
//     <doc kind="example" id="example1">
//       <synopsis>
//         Print the number of valid reactions in network structure
//         p_my_network:
//       </synopsis>
//
//       <code>
// printf(
//   "Number of valid reactions = %lu\n",
//   (unsigned long)
//   Libnucnet__Net__getNumberOfValidReactions( p_my_network )
// );
//       </code>
//     </doc>
//
//   </usage>
//
// </routine>
//############################################################################*/

size_t Libnucnet__Net__getNumberOfValidReactions( Libnucnet__Net * );

/*##############################################################################
// <routine name="Libnucnet__Zone__computeRates()">
//
//   <description>
//     <abstract>
//       Compute and store the rates for all valid reactions in a given
//       zone for the input temperature and density.
//     </abstract>
//     <keywords>
//       Libnucnet, nuclear network, zone, reaction
//     </keywords>
//   </description>
//
//   <usage>
//
//     <calling_sequence>
// void
// Libnucnet__Zone__computeRates(
//   Libnucnet__Zone *self,
//   double d_t9,
//   double d_rho
// );
//     </calling_sequence>
//
//     <param
//       name="self"
//       kind="in,positional,required"
//       doc="self"
//     >
//       A pointer to a valid Libnucnet__Zone structure.
//     </param>
//     <param
//       name="d_t9"
//       kind="in,positional,required"
//       doc="self"
//     >
//       The temperature in billions of K at which to compute the rates.
//     </param>
//     <param
//       name="d_rho"
//       kind="in,positional,required"
//       doc="self"
//     >
//       The density in grams per cc at which to compute the rates.
//     </param>
//     <param
//       kind="return"
//       doc="result"
//     />
//
//     <doc kind="post" id="result">
//       On successful return, the rates for each reaction stored in the
//       input structure have been updated.
//     </doc>
//
//     <doc kind="bug" >
//     </doc>
//
//     <doc kind="example" id="example1">
//       <synopsis>
//         Update the rates for the zone with labels "0", "0", "0" for the
//         temperature T9 = 1 and density rho = 2.e6 g/cc in Libnucnet
//         structure p_my_zones:
//       </synopsis>
//
//       <code>
// p_zone = Libnucnet__getZoneByLabels( p_my_zones, "0", "0", "0" );
// Libnucnet__Zone__computeRates( p_zone, 1., 2.e6 );
//       </code>
//     </doc>
//
//   </usage>
//
// </routine>
//############################################################################*/

void
Libnucnet__Zone__computeRates( Libnucnet__Zone *, double, double );

/*##############################################################################
// <routine name="Libnucnet__Net__computeRatesForReaction()">
//
//   <description>
//     <abstract>
//       Compute the forward and reverse rates for a valid reaction
//       at the input temperature and density.  The rates are those
//       between reacting species.
//     </abstract>
//     <keywords>
//       Libnucnet, nuclear network, zone, reaction
//     </keywords>
//   </description>
//
//   <usage>
//
//     <calling_sequence>
// void Libnucnet__Net__computeRatesForReaction(
//   Libnucnet__Net *self,
//   Libnucnet__Reaction *p_reaction,
//   double d_t9,
//   double d_rho,
//   void *p_extra_data,
//   double *p_forward,
//   double *p_reverse
// );
//     </calling_sequence>
//
//     <param
//       name="self"
//       kind="in,positional,required"
//       doc="self"
//     >
//       A pointer to a valid Libnucnet__Net structure.
//     </param>
//     <param
//       name="p_reaction"
//       kind="in,positional,required"
//       doc="p_reaction"
//     >
//       A pointer to a valid Libnucnet__Reaction.
//     </param>
//     <param
//       name="d_t9"
//       kind="in,positional,required"
//       doc="d_t9"
//     >
//       The temperature in billions of K at which to compute the rates.
//     </param>
//     <param
//       name="d_rho"
//       kind="in,positional,required"
//       doc="d_rho"
//     >
//       The density in grams per cc at which to compute the rates.
//     </param>
//     <param
//       name="p_extra_data"
//       kind="in,positional,required"
//       doc="p_extra_data"
//     >
//       A pointer to extra user-defined data for the rate function for the
//       reaction.
//     </param>
//     <param
//       name="p_forward"
//       kind="out,positional,required"
//       doc="p_forward"
//     >
//       A pointer to the double containing the computed forward rate.
//     </param>
//     <param
//       name="p_reverse"
//       kind="out,positional,required"
//       doc="p_reverse"
//     >
//       A pointer to the double containing the computed reverse rate.
//     </param>
//     <param
//       kind="return"
//       doc="result"
//     />
//
//     <doc kind="post" id="result">
//       On successful return, the routine returns the computed forward
//       and reverse rates.  If any input is invalid, error handling is
//       invoked.
//     </doc>
//
//     <doc kind="bug" >
//     </doc>
//
//     <doc kind="example" id="example1">
//       <synopsis>
//         Compute the forward and reverse rates for c12 + h1 -> n13 + gamma 
//         at a temperature T9 = 1 and density rho = 2.e6 g/cc in
//         Libnucnet__Net *p_my_nucnet:
//       </synopsis>
//
//       <code>
// p_reaction =
//   Libnucnet__Reac__getReactionByString(
//     Libnucnet__Net__getReac( p_my_nucnet ),
//     "c12 + h1 -> n13 + gamma"
//   );
// Libnucnet__Net__computeRatesForReaction(
//   p_my_nucnet,
//   p_reaction,
//   1.,
//   2.e6,
//   NULL,
//   &d_forward,
//   &d_reverse
// );
//       </code>
//     </doc>
//
//   </usage>
//
// </routine>
//############################################################################*/

void Libnucnet__Net__computeRatesForReaction(
  Libnucnet__Net *,
  Libnucnet__Reaction *,
  double,
  double,
  void *,
  double *,
  double *
);

/*##############################################################################
// <routine name="Libnucnet__Net__computeReactionQValue()">
//
//   <description>
//     <abstract>
//       Compute the Q value (in MeV) for the specified reaction in the
//       Libnucnet__Net structure.
//     </abstract>
//     <keywords>
//       nuclear network, rates, Q, value, reaction
//     </keywords>
//   </description>
//
//   <usage>
//
//     <calling_sequence>
// double
// Libnucnet__Net__computeReactionQValue(
//   Libnucnet__Net *self,  Libnucnet__Reaction *p_reaction,
// );
//     </calling_sequence>
//
//     <param
//       name="self"
//       kind="in,positional,required"
//     >
//       A pointer to a Libnucnet__Net structure.
//     </param>
//     <param
//       name="p_reaction"
//       kind="in,positional,required"
//     >
//       A pointer to a Libnucnet reaction structure.
//     </param>
//
//     <param
//       kind="return"
//       doc="result"
//     >
//       Routine returns a double containing the Q value of the
//       reaction.  If either the Net or Reaction input structure is
//       invalid, error handling is invoked.
//     </param>
//
//     <doc kind="bug" >
//     </doc>
//
//     <doc kind="example" id="example">
//       <synopsis>
//         Compute the Q value of the reaction ne20 + he4 -> mg24 +
//         gamma from Libnucnet__Net *p_my_net and store the value in
//         d_Qvalue:
//       </synopsis>
//
//       <code>
// d_Qvalue =
//   Libnucnet__Net__computeReactionQValue(
//     p_my_net,
//     Libnucnet__Reac__getReactionByString(
//       Libnucnet__Net__getReac( p_my_net ),
//       "ne20 + he4 -> mg24 + gamma"
//     )
//   );
//       </code>
//     </doc>
//
//   </usage>
//
// </routine>
//############################################################################*/

double Libnucnet__Net__computeReactionQValue(
  Libnucnet__Net *, Libnucnet__Reaction *
);

/*##############################################################################
// <routine name="Libnucnet__Zone__getRatesForReaction()">
//
//   <description>
//     <abstract>
//       Retrieve the rates for the specified reaction from the input
//       zone.
//     </abstract>
//     <keywords>
//       nuclear network, rates, get, retrieve
//     </keywords>
//   </description>
//
//   <usage>
//
//     <calling_sequence>
// void
// Libnucnet__Zone__getRatesForReaction( 
//   Libnucnet__Zone *self,
//   Libnucnet__Reaction *p_reaction,
//   double *p_forward,
//   double *p_reverse
// );
//     </calling_sequence>
//
//     <param
//       name="self"
//       kind="in,positional,required"
//     >
//       A pointer to a Libnucnet__Zone structure.
//     </param>
//     <param
//       name="p_reaction"
//       kind="in,positional,required"
//     >
//       A pointer to a Libnucnet reaction structure.
//     </param>
//     <param
//       name="p_forward"
//       kind="out,positional,required"
//     >
//       A pointer to a double where the retrieved forward rate will be stored.
//     </param>
//     <param
//       name="p_reverse"
//       kind="out,positional,required"
//     >
//       A pointer to a double where the retrieved reverse rate will be stored.
//     </param>
//
//     <param
//       kind="return"
//       doc="result"
//     />
//
//     <doc kind="bug" >
//     </doc>
//
//     <doc kind="example" id="example">
//       <synopsis>
//         Retrieve the forward and reverse rates for reaction p_reaction in
//         zone labelled "1", "1", "2" in p_zones, and store them in
//         p_forward and p_reverse:
//       </synopsis>
//
//       <code>
// p_zone = Libnucnet__getZoneByLabels( p_zones, "1", "1", "2" );
// Libnucnet__Zone__getRatesForReaction( 
//   p_zone, p_reaction, p_forward, p_reverse 
// );
//       </code>
//     </doc>
//
//   </usage>
//
// </routine>
//############################################################################*/

void Libnucnet__Zone__getRatesForReaction( 
  const Libnucnet__Zone *,
  const Libnucnet__Reaction *,
  double *,
  double *
);

/*##############################################################################
// <routine name="Libnucnet__Zone__updateRatesForReaction()">
//
//   <description>
//     <abstract>
//       Updates the rates for a given reaction to the specified values in the
//       input zone.
//     </abstract>
//     <keywords>
//       nuclear network, rates, modify, change, update
//     </keywords>
//   </description>
//
//   <usage>
//
//     <calling_sequence>
// void
// Libnucnet__Zone__updateRatesForReaction( 
//   Libnucnet__Zone *self,
//   Libnucnet__Reaction *p_reaction,
//   double d_forward,
//   double d_reverse
// );
//     </calling_sequence>
//
//     <param
//       name="self"
//       kind="in,positional,required"
//     >
//       A pointer to a Libnucnet__Zone structure.
//     </param>
//     <param
//       name="p_reaction"
//       kind="in,positional,required"
//     >
//       A pointer to a Libnucnet reaction structure.
//     </param>
//     <param
//       name="d_forward"
//       kind="out,positional,required"
//     >
//       A double representing the new forward rate.
//     </param>
//     <param
//       name="d_reverse"
//       kind="out,positional,required"
//     >
//       A double representing the new reverse rate.
//     </param>
//
//     <param
//       kind="return"
//       doc="result"
//     />
//
//     <doc kind="bug" >
//     </doc>
//
//     <doc kind="example" id="example">
//       <synopsis>
//         Double the forward and reverse rates for reaction p_reaction in
//         zone labelled "1", "2", "3" in p_zones:
//       </synopsis>
//
//       <code>
// p_zone = Libnucnet__getZoneByLabels( p_zones, "1", "2", "3" );
// Libnucnet__Zone__getRatesForReaction(
//   p_zone, p_reaction, &d_forward, &d_reverse
// );
// Libnucnet__Zone__updateRatesForReaction(
//   p_zone, p_reaction, 2. * d_forward, 2. * d_reverse
// );
//       </code>
//     </doc>
//
//   </usage>
//
// </routine>
//############################################################################*/

void Libnucnet__Zone__updateRatesForReaction( 
  Libnucnet__Zone *,
  Libnucnet__Reaction *,
  double,
  double 
);

/*##############################################################################
// <routine name="Libnucnet__Zone__computeJacobianMatrix()">
//
//   <description>
//     <abstract>
//       Creates the Jacobian matrix for the zone.  The elements in this matrix
//       differ from those in the regular Jacobian (computed by 
//       Libnucnet__Zone__computeJacobian) by a minus sign.
//     </abstract>
//     <keywords>
//       nuclear, network, species, reactions, rate, Jacobian, matrix, zone
//     </keywords>
//   </description>
//
//   <usage>
//
//     <calling_sequence>
// WnMatrix *
// Libnucnet__Zone__computeJacobianMatrix(
//   Libnucnet__Zone *self
// );
//     </calling_sequence>
//
//     <param
//       name="self"
//       kind="in,positional,required"
//     >
//       A pointer to a Libnucnet__Zone structure.
//     </param>
//     <param
//       kind="return"
//       doc="result"
//     />
//
//     <doc kind="post" id="result">
//       If the input is valid, the routine returns a pointer to a
//       new WnMatrix structure containing the Jacobian matrix.  It is
//       the caller's reponsibility to free the matrix with WnMatrix__free()
//       when no longer in use.
//     </doc>
//
//     <doc kind="bug" >
//     </doc>
//
//     <doc kind="example" id="example">
//       <synopsis>
//       Create the Jacobian matrix for reaction rates and abundances in
//       Libnucnet__Zone *p_zone and store it in WnMatrix *p_matrix:
//       </synopsis>
//
//       <code>
// p_matrix = Libnucnet__Zone__computeJacobianMatrix( p_zone );
//       </code>
//     </doc>
//
//   </usage>
//
// </routine>
//############################################################################*/

WnMatrix *Libnucnet__Zone__computeJacobianMatrix( Libnucnet__Zone * );

/*##############################################################################
// <routine name="Libnucnet__Zone__computeFlowVector()">
//
//   <description>
//     <abstract>
//       Creates the flow vector for the zone.  Elements of the
//       vector are the sum of all flows into the species minus the sum
//       of all flows out of the species.  The sums are computed from
//       the individual reactions separately for best accuracy.
//     </abstract>
//     <keywords>
//       nuclear, network, species, reactions, rate, flow, vector, zone
//     </keywords>
//   </description>
//
//   <usage>
//
//     <calling_sequence>
// gsl_vector *
// Libnucnet__Zone__computeFlowVector( Libnucnet__Zone *self );
//     </calling_sequence>
//
//     <param
//       name="self"
//       kind="in,positional,required"
//     >
//       A pointer to a Libnucnet__Zone structure.
//     </param>
//     <param
//       kind="return"
//       doc="result"
//     />
//
//     <doc kind="post" id="result">
//       If the input is valid, the routine returns a pointer to a new
//       gsl_vector containing the flow vector.  It is the caller's
//       responsibility to free the vector with gsl_vector_free
//       when no longer in use.  If the
//       input is not valid, error handling is invoked.
//     </doc>
//
//     <doc kind="bug" >
//     </doc>
//
//     <doc kind="example" id="example">
//       <synopsis>
//       Create the flow vector for reaction rates and abundances in
//       Libnucnet__Zone *p_zone and store it in gsl_vector *p_flow_vector:
//       </synopsis>
//
//       <code>
// p_flow_vector = Libnucnet__Zone__computeFlowVector( p_zone );
//       </code>
//     </doc>
//
//   </usage>
//
// </routine>
//############################################################################*/

gsl_vector *
Libnucnet__Zone__computeFlowVector( Libnucnet__Zone * );

/*##############################################################################
// <routine name="Libnucnet__Zone__updateTimeStep()">
//
//   <description>
//     <abstract>
//       Calculates the new time step for the evolution of the nuclear network.
//     </abstract>
//     <keywords>
//       nuclear network, timestep, reactions, rates, dt
//     </keywords>
//   </description>
//
//   <usage>
//
//     <calling_sequence>
//       void
//       Libnucnet__Zone__updateTimeStep(
//         Libnucnet__Zone *self,
//         double *d_dt,
//         double d_regt,
//         double d_regy,
//         double d_ymin
//       );
//     </calling_sequence>
//
//     <param name="self" kind="in,positional,required" doc="zone" />
//     <param name="d_dt" kind="in,positional,required" doc="dt" />
//     <param name="d_regt" kind="in,positional,required" doc="regt" />
//     <param name="d_regy" kind="in,positional,required" doc="regy" />
//     <param name="d_ymin" kind="in,positional,required" doc="ymin" />
//
//     <doc kind="pre" id="zone">
//       A pointer to a Libnucnet__Zone structure containing the nuclear network
//       abundances necessary to calculate the new timestep.
//     </doc>
//     <doc kind="pre" id="dt">
//       A double containing the old timestep.
//     </doc>
//     <doc kind="pre" id="regt">
//       A double containing the maximum fractional change in the
//       timestep.
//     </doc>
//     <doc kind="pre" id="regy">
//       A double containing the factor by which to allow abundances
//       larger than ymin to grow over the next timestep.
//     </doc>
//     <doc kind="pre" id="ymin">
//       A double containing the minimum value for which an abundance
//       will be taken into consideration when calculating the timestep.
//     </doc>
//
//     <param
//       kind="return"
//       doc="result"
//     />
//
//     <doc kind="post" id="result">
//       On successful return, the timestep has been updated.
//       If input is not valid, error handling is invoked.
//     </doc>
//     <doc kind="bug" >
//     </doc>
//
//     <doc kind="example" id="example">
//       <synopsis>
//       Update the time step for p_zone given old timestep of d_dt,
//       a maximum allowed increase in the timestep of 15%, and a maximum
//       allowed increase in the abundance of a species with abundance greater
//       than 1.e-10 of 10%:
//       </synopsis>
//       <code>
// Libnucnet__Zone__updateTimeStep(
//   p_zone, &d_dt, 0.15, 0.1, 1.e-10
// );
//       </code>
//     </doc>
//
//   </usage>
//
// </routine>
//############################################################################*/

void Libnucnet__Zone__updateTimeStep(
  Libnucnet__Zone *,
  double *,
  double,
  double,
  double
);

/*##############################################################################
// <routine name="Libnucnet__getNumberOfZones()">
//
//   <description>
//     <abstract>
//       Retrieves the number of zones in the network.
//     </abstract>
//     <keywords>
//       nuclear, data, species, number
//     </keywords>
//   </description>
//
//   <usage>
//
//     <calling_sequence>
// size_t
// Libnucnet__getNumberOfZones( const Libnucnet *self );
//     </calling_sequence>
//
//     <param
//       name="self"
//       kind="in,positional,required"
//     >
//       A pointer to a Libnucnet structure.
//     </param>
//
//     <param
//       kind="return"
//       doc="result"
//     />
//
//     <doc kind="post" id="result">
//       Routine returns an integer representing the number of zones in the
//       network.  If the input stucture is not valid, error
//       handling is invoked.
//     </doc>
//
//     <doc kind="bug" >
//     </doc>
//
//     <doc kind="example" id="example">
//       <synopsis>
//       Print number of zones in Libnucnet *p_my_nucnet:
//       </synopsis>
//
//       <code>
// printf(
//   "Number of zones = %d\n",
//   Libnucnet__getNumberOfZones( p_my_nucnet )
// );
//       </code>
//     </doc>
//
//   </usage>
//
// </routine>
//############################################################################*/

size_t Libnucnet__getNumberOfZones( const Libnucnet * );

/*##############################################################################
// <routine name="Libnucnet__removeZone()">
//
//   <description>
//     <abstract>
//       Remove a zone from a Libnucnet structure.
//     </abstract>
//     <keywords>
//       nuclear, data, zone, remove
//     </keywords>
//   </description>
//
//   <usage>
//
//     <calling_sequence>
// void
// Libnucnet__removeZone(
//   Libnucnet *self,
//   Libnucnet__Zone *p_zone
// );
//     </calling_sequence>
//
//     <param
//       name="self"
//       kind="in,positional,required"
//     >
//       A pointer to a Libnucnet structure.
//     </param>
//
//     <param
//       name="p_zone"
//       kind="in,positional,required"
//     >
//       A pointer to a Libnucnet zone structure to be removed from self.
//     </param>
//
//     <param
//       kind="return"
//       doc="result"
//     />
//
//     <doc kind="post" id="result">
//       Upon successful return, the zone has been removed from the
//       Libnucnet structure.  If the input zone is invalid, Libnucnet
//       error handling is invoked.
//     </doc>
//
//     <doc kind="bug" >
//     </doc>
//
//     <doc kind="example" id="example">
//       <synopsis>
//         Remove the zone labeled "x1", "y1", "z1" from Libnucnet *p_my_nucnet:
//       </synopsis>
//
//       <code>
// Libnucnet__removeZone(
//   p_my_nucnet,
//   Libnucnet__getZoneByLabels( p_my_nucnet, "x1", "y1", "z1" )
// );
//       </code>
//     </doc>
//
//   </usage>
//
// </routine>
//############################################################################*/

void
Libnucnet__removeZone( Libnucnet *, Libnucnet__Zone * );

/*##############################################################################
// <routine name="Libnucnet__Zone__updateSpeciesAbundance()">
//
//   <description>
//     <abstract>
//       Assigns the new species abundance to the Libnucnet__Zone struct.
//     </abstract>
//     <keywords>
//       nuclear network, abundance, Y, zone
//     </keywords>
//   </description>
//
//   <usage>
//
//     <calling_sequence>
// void
// Libnucnet__Zone__updateSpeciesAbundance(
//   Libnucnet__Zone *self,
//   Libnucnet__Species * p_species,
//   double d_y
// );
//     </calling_sequence>
//
//     <param name="self" kind="in,positional,required" doc="zone" />
//     <param name="p_species" kind="in,positional,required" doc="p_species" />
//     <param name="d_y" kind="in,positional,required" doc="y" />
//
//     <doc kind="pre" id="zone">
//       A pointer to a Libnucnet__Zone structure.
//     </doc>
//     <doc kind="pre" id="p_species">
//       A pointer to a Libnucnet__Species structure.
//     </doc>
//     <doc kind="pre" id="y">
//       d_y is a double containing the new abundance.
//     </doc>
//
//     <param
//       kind="return"
//       doc="result"
//     />
//
//     <doc kind="post" id="result">
//       On successful return, the abundance of the species in the zone
//       is modified by the value in d_y.
//     </doc>
//
//     <doc kind="bug" >
//     </doc>
//
//     <doc kind="example" id="example">
//       <synopsis>
//       Update the abundance of species p_species for p_zone with d_y:
//       </synopsis>
//       <code>
// Libnucnet__Zone__updateSpeciesAbundance(
//   p_zone,
//   p_species,
//   d_y
// );
//       </code>
//     </doc>
//
//   </usage>
//
// </routine>
//############################################################################*/

void Libnucnet__Zone__updateSpeciesAbundance(
  Libnucnet__Zone *, Libnucnet__Species *, double
);

/*##############################################################################
// <routine name="Libnucnet__Zone__updateSpeciesAbundanceChange()">
//
//   <description>
//     <abstract>
//       Assigns the new species abundance change to the Libnucnet__Zone struct.
//     </abstract>
//     <keywords>
//       nuclear network, abundance, change, dY, zone
//     </keywords>
//   </description>
//
//   <usage>
//
//     <calling_sequence>
// void
// Libnucnet__Zone__updateSpeciesChangeAbundance(
//   Libnucnet__Zone *self,
//   Libnucnet__Species * p_species,
//   double d_dy
// );
//     </calling_sequence>
//
//     <param name="self" kind="in,positional,required" doc="zone" />
//     <param name="p_species" kind="in,positional,required" doc="p_species" />
//     <param name="d_dy" kind="in,positional,required" doc="dy" />
//
//     <doc kind="pre" id="zone">
//       A pointer to a Libnucnet__Zone structure.
//     </doc>
//     <doc kind="pre" id="p_species">
//       A pointer to a Libnucnet__Species structure.
//     </doc>
//     <doc kind="pre" id="dy">
//       d_dy is a double containing the new abundance change.
//     </doc>
//
//     <param
//       kind="return"
//       doc="result"
//     />
//
//     <doc kind="post" id="result">
//       On successful return, the abundance change of the species in the zone
//       is modified by the value in d_dy.
//     </doc>
//
//     <doc kind="bug" >
//     </doc>
//
//     <doc kind="example" id="example">
//       <synopsis>
//       Update the abundance change of species p_species for p_zone with d_dy:
//       </synopsis>
//       <code>
// Libnucnet__Zone__updateSpeciesAbundanceChange(
//   p_zone,
//   p_species,
//   d_dy
// );
//       </code>
//     </doc>
//
//   </usage>
//
// </routine>
//############################################################################*/

void Libnucnet__Zone__updateSpeciesAbundanceChange(
  Libnucnet__Zone *, Libnucnet__Species *, double
);

/*##############################################################################
// <routine name="Libnucnet__Zone__getSpeciesAbundance()">
//
//   <description>
//     <abstract>
//       Retrieves the specified species abundance.
//     </abstract>
//     <keywords>
//       nuclear, data, abundance, zone, species, hash
//     </keywords>
//   </description>
//
//   <usage>
//
//     <calling_sequence>
// double
// Libnucnet__Zone__getSpeciesAbundance(
//   const Libnucnet__Zone *self,
//   const Libnucnet__Species *p_species
// );
//     </calling_sequence>
//
//     <param
//       name="self"
//       kind="in,positional,required"
//     >
//       A pointer to a Libnucnet__Zone structure.
//     </param>
//     <param
//       name="p_species"
//       kind="in,positional,required"
//     >
//       A pointer to the species whose abundance is to be retrieved.
//     </param>
//
//     <param
//       kind="return"
//       doc="result"
//     />
//
//     <doc kind="post" id="result">
//       Routine returns a double giving the abundance of the species
//       in the specified zone.
//       If any input is invalid, Libnucnet error handling is invoked.
//     </doc>
//
//     <doc kind="bug" >
//     </doc>
//
//     <doc kind="example" id="example">
//       <synopsis>
//       Retrieve he4 abundance in zone p_zone from the network p_my_nucnet
//       and store in d_abundance:
//       </synopsis>
//
//       <code>
// d_abundance =
//   Libnucnet__Zone__getSpeciesAbundance(
//     p_zone,
//     Libnucnet__Nuc__getSpeciesByName(
//       Libnucnet__Net__getNuc(
//         Libnucnet__Zone__getNet( p_my_nucnet )
//       ),
//       "he4"
//     )
//   );
//       </code>
//     </doc>
//
//   </usage>
//
// </routine>
//############################################################################*/

double Libnucnet__Zone__getSpeciesAbundance(
  const Libnucnet__Zone *,
  const Libnucnet__Species *
);

/*##############################################################################
// <routine name="Libnucnet__Zone__getSpeciesAbundanceChange()">
//
//   <description>
//     <abstract>
//       Retrieves the specified species abundance change.
//     </abstract>
//     <keywords>
//       nuclear, data, abundance, zone, species, hash, change
//     </keywords>
//   </description>
//
//   <usage>
//
//     <calling_sequence>
// double
// Libnucnet__Zone__getSpeciesAbundanceChange(
//   const Libnucnet__Zone *self,
//   const Libnucnet__Species *p_species
// );
//     </calling_sequence>
//
//     <param
//       name="self"
//       kind="in,positional,required"
//     >
//       A pointer to a Libnucnet__Zone structure.
//     </param>
//     <param
//       name="p_species"
//       kind="in,positional,required"
//     >
//       A pointer to the species whose abundance change is to be retrieved.
//     </param>
//
//     <param
//       kind="return"
//       doc="result"
//     />
//
//     <doc kind="post" id="result">
//       Routine returns a double giving the change in abundance of the species
//       in the specified zone.
//       If any input is invalid, Libnucnet error handling is invoked.
//     </doc>
//
//     <doc kind="bug" >
//     </doc>
//
//     <doc kind="example" id="example">
//       <synopsis>
//       Retrieve he4 abundance change in zone p_zone:
//       </synopsis>
//
//       <code>
// if(
//     (
//        p_he4 =
//          Libnucnet__Nuc__getSpeciesByName(
//            Libnucnet__Net__getNuc(
//              Libnucnet__Zone__getNet( p_zone )
//            ),
//            "he4"
//          )
//     )
// ) {
//   d_abundance_change =
//     Libnucnet__Zone__getSpeciesAbundanceChange(
//       p_zone,
//       p_he4
//     );
// }
//       </code>
//     </doc>
//
//   </usage>
//
// </routine>
//############################################################################*/

double Libnucnet__Zone__getSpeciesAbundanceChange(
  const Libnucnet__Zone *,
  const Libnucnet__Species *
);

/*##############################################################################
// <routine name="Libnucnet__getZoneByLabels()">
//
//   <description>
//     <abstract>
//       Retrieves the specified zone.
//     </abstract>
//     <keywords>
//       nuclear, data, zone, name, hash
//     </keywords>
//   </description>
//
//   <usage>
//
//     <calling_sequence>
// Libnucnet__Zone *
// Libnucnet__getZoneByLabels(
//   const Libnucnet *self,
//   const char *s_label_1,
//   const char *s_label_2,
//   const char *s_label_3
// );
//     </calling_sequence>
//
//     <param
//       name="self"
//       kind="in,positional,required"
//     >
//       A pointer to a Libnucnet structure.
//     </param>
//     <param
//       name="s_label_1"
//       kind="in,positional,required"
//     >
//       A string giving the first label of the zone to be retrieved.
//     </param>
//     <param
//       name="s_label_2"
//       kind="in,positional,required"
//     >
//       A string giving the second label of the zone to be retrieved.
//     </param>
//     <param
//       name="s_label_3"
//       kind="in,positional,required"
//     >
//       A string giving the third label of the zone to be retrieved.
//     </param>
//
//     <param
//       kind="return"
//       doc="result"
//     />
//
//     <doc kind="post" id="result">
//       Routine returns a pointer to a Libnucnet__Zone structure.
//       If the input is invalid, Libnucnet error handling is invoked.
//       If the zone is not found, routine returns NULL.
//     </doc>
//
//     <doc kind="bug" >
//     </doc>
//
//     <doc kind="example" id="example">
//       <synopsis>
//       Retrieve zone with labels "x", "y", and "z" from Libnucnet *p_nucnet:
//       </synopsis>
//
//       <code>
// p_zone =
//   Libnucnet__getZoneByLabels( p_nucnet, "x", "y", "z" );
//       </code>
//     </doc>
//
//   </usage>
//
// </routine>
//############################################################################*/

Libnucnet__Zone *
Libnucnet__getZoneByLabels(
  const Libnucnet *,
  const char *,
  const char *,
  const char *
);

/*##############################################################################
// <routine name="Libnucnet__Net__isValidReaction()">
//
//   <description>
//     <abstract>
//       Determine whether a reaction is valid, that is, that it is
//       between species in the network and conserves charge and baryon
//       and lepton number.
//     </abstract>
//     <keywords>
//       nuclear, reaction, valid, baryon, lepton, network
//     </keywords>
//   </description>
//
//   <usage>
//
//     <calling_sequence>
// int
// Libnucnet__Net__isValidReaction(
//   const Libnucnet__Net *self,
//   const Libnucnet__Reaction *p_reaction
// );
//     </calling_sequence>
//
//     <param
//       name="self"
//       kind="in,positional,required"
//     >
//       A pointer to a Libnucnet__Net structure.
//     </param>
//
//     <param
//       name="p_reaction"
//       kind="in,positional,required"
//     >
//       A pointer to a reaction.
//     </param>
//
//     <param
//       kind="return"
//       doc="result"
//     />
//
//     <doc kind="post" id="result">
//       For valid input, routine returns 1 if the reaction is valid and
//       0 if it is not.
//     </doc>
//
//     <doc kind="bug" >
//     </doc>
//
//     <doc kind="example" id="example">
//       <synopsis>
//         Determine whether the reaction "o18 + he4 -> ne22 + gamma" is
//         valid in the network Libnucnet__Net *p_my_net, and if so,
//         print it out:
//       </synopsis>
//
//       <code>
// p_reaction =
//   Libnucnet__Reac__getReactionByString(
//     Libnucnet__Net__getReac( p_my_net ),
//     "o18 + he4 -> ne22 + gamma"
//   );
// if( Libnucnet__Net__isValidReaction( p_my_net, p_reaction ) )
// {
//   printf(
//     "%s is a valid reaction!\n",
//     Libnucnet__Reaction__getString( p_reaction )
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
Libnucnet__Net__isValidReaction(
  const Libnucnet__Net *,
  const Libnucnet__Reaction *
);

/*##############################################################################
// <routine name="Libnucnet__Zone__getLabel()">
//
//   <description>
//     <abstract>
//       Retrieves a label of a zone.
//     </abstract>
//     <keywords>
//       nuclear, data, zone, name, label
//     </keywords>
//   </description>
//
//   <usage>
//
//     <calling_sequence>
// const char *
// Libnucnet__Zone__getLabel(
//   Libnucnet__Zone *self, int i_label
// );
//     </calling_sequence>
//
//     <param
//       name="self"
//       kind="in,positional,required"
//     >
//       A pointer to a Libnucnet__Zone structure.
//     </param>
//     <param
//       name="i_label"
//       kind="in,positional,required"
//     >
//       An integer giving the label number desired.
//     </param>
//
//     <param
//       kind="return"
//       doc="result"
//     />
//
//     <doc kind="post" id="result">
//       Routine returns a string giving the label of the zone.
//       If any input is invalid, Libnucnet error handling is invoked.
//     </doc>
//
//     <doc kind="bug" >
//     </doc>
//
//     <doc kind="example" id="example">
//       <synopsis>
//       Print the second label of the zone pointed to by p_zone:
//       </synopsis>
//
//       <code>
// printf(
//   "Second zone label is %s\n",
//   Libnucnet__Zone__getLabel( p_zone, 2 )
// );
//       </code>
//     </doc>
//
//   </usage>
//
// </routine>
//############################################################################*/

const char *
Libnucnet__Zone__getLabel( const Libnucnet__Zone *, int );

/*##############################################################################
// <routine name="Libnucnet__Net__free()">
//
//   <description>
//     <abstract>
//       Free the memory for a Libnucnet__Net structure.
//     </abstract>
//     <keywords>
//       nuclear, free, network
//     </keywords>
//   </description>
//
//   <usage>
//
//     <calling_sequence>
// void Libnucnet__Net__free( Libnucnet__Net *self );
//     </calling_sequence>
//
//     <param
//       name="self"
//       kind="in,positional,required"
//     >
//       A pointer to a Libnucnet__Net structure.
//     </param>
//
//     <param
//       kind="return"
//       doc="result"
//     />
//
//     <doc kind="post" id="result">
//       On return, the memory allocated for the network has been cleared. 
//       If the input pointer is not valid, Libnucnet error handling is invoked.
//     </doc>
//
//     <doc kind="bug" >
//     </doc>
//
//     <doc kind="example" id="example">
//       <synopsis>
//       Free the memory for the network in p_my_nucnet:
//       </synopsis>
//
//       <code>
// Libnucnet__Net__free( p_my_nucnet );
//       </code>
//     </doc>
//
//   </usage>
//
// </routine>
//############################################################################*/

void Libnucnet__Net__free( Libnucnet__Net * );

/*##############################################################################
// <routine name="Libnucnet__free()">
//
//   <description>
//     <abstract>
//       Free the memory for a Libnucnet structure.
//     </abstract>
//     <keywords>
//       nuclear, free, network
//     </keywords>
//   </description>
//
//   <usage>
//
//     <calling_sequence>
// void Libnucnet__free( Libnucnet *self );
//     </calling_sequence>
//
//     <param
//       name="self"
//       kind="in,positional,required"
//     >
//       A pointer to a Libnucnet structure.
//     </param>
//
//     <param
//       kind="return"
//       doc="result"
//     />
//
//     <doc kind="post" id="result">
//       On return, the memory allocated for the network has been cleared. 
//       If the input pointer is not valid, Libnucnet error handling is invoked.
//     </doc>
//
//     <doc kind="bug" >
//     </doc>
//
//     <doc kind="example" id="example">
//       <synopsis>
//       Free the memory for the network in p_my_nucnet:
//       </synopsis>
//
//       <code>
// Libnucnet__free( p_my_nucnet );
//       </code>
//     </doc>
//
//   </usage>
//
// </routine>
//############################################################################*/

void Libnucnet__free( Libnucnet * );

/*##############################################################################
// <routine name="Libnucnet__freeAllZones()">
//
//   <description>
//     <abstract>
//       Free the memory for all zones in a Libnucnet structure.
//     </abstract>
//     <keywords>
//       nuclear, free, zone
//     </keywords>
//   </description>
//
//   <usage>
//
//     <calling_sequence>
// void Libnucnet__freeAllZones( Libnucnet *self );
//     </calling_sequence>
//
//     <param
//       name="self"
//       kind="in,positional,required"
//     >
//       A pointer to a Libnucnet structure.
//     </param>
//
//     <param
//       kind="return"
//       doc="result"
//     />
//
//     <doc kind="post" id="result">
//       On return, the memory allocated for all zones has been cleared but
//       self is still available for use.
//       If the input pointer is not valid, Libnucnet error handling is invoked.
//     </doc>
//
//     <doc kind="bug" >
//     </doc>
//
//     <doc kind="example" id="example">
//       <synopsis>
//       Free the memory for all zones in p_my_nucnet:
//       </synopsis>
//
//       <code>
// Libnucnet__freeAllZones( p_my_nucnet );
//       </code>
//     </doc>
//
//   </usage>
//
// </routine>
//############################################################################*/

void Libnucnet__freeAllZones( Libnucnet * );

/*##############################################################################
// <routine name="Libnucnet__Net__getNuc()">
//
//   <description>
//     <abstract>
//       Retrieve the Libnucnet__Nuc structure from a Libnucnet__Net structure.
//     </abstract>
//     <keywords>
//       nuclear, network, retrieve
//     </keywords>
//   </description>
//
//   <usage>
//
//     <calling_sequence>
// Libnucnet__Nuc *
// Libnucnet__Net__getNuc( const Libnucnet__Net *self );
//     </calling_sequence>
//
//     <param
//       name="self"
//       kind="in,positional,required"
//     >
//       A pointer to a Libnucnet__Net structure.
//     </param>
//
//     <param
//       kind="return"
//       doc="result"
//     />
//
//     <doc kind="post" id="result">
//       Routine returns the pointer to the Libnucnet__Nuc structure contained
//       in the Libnucnet__Net structure.
//       If the input pointer is not valid, Libnucnet error handling is invoked.
//     </doc>
//
//     <doc kind="bug" >
//     </doc>
//
//     <doc kind="example" id="example">
//       <synopsis>
//       Retrieve the nuclear data structure p_my_nuclei from the network
//       structure p_my_network:
//       </synopsis>
//
//       <code>
// p_my_nuclei = Libnucnet__Net__getNuc( p_my_network );
//       </code>
//     </doc>
//
//   </usage>
//
// </routine>
//############################################################################*/

Libnucnet__Nuc *Libnucnet__Net__getNuc( const Libnucnet__Net * );

/*##############################################################################
// <routine name="Libnucnet__Net__getReac()">
//
//   <description>
//     <abstract>
//       Retrieve the Libnucnet__Reac structure from a Libnucnet__Net structure.
//     </abstract>
//     <keywords>
//       nuclear, network, retrieve, reaction
//     </keywords>
//   </description>
//
//   <usage>
//
//     <calling_sequence>
// Libnucnet__Reac *
// Libnucnet__Net__getReac( const Libnucnet__Net *self );
//     </calling_sequence>
//
//     <param
//       name="self"
//       kind="in,positional,required"
//     >
//       A pointer to a Libnucnet__Net structure.
//     </param>
//
//     <param
//       kind="return"
//       doc="result"
//     />
//
//     <doc kind="post" id="result">
//       Routine returns the pointer to the Libnucnet__Reac structure contained
//       in the Libnucnet__Net structure.
//       If the input pointer is not valid, Libnucnet error handling is invoked.
//     </doc>
//
//     <doc kind="bug" >
//     </doc>
//
//     <doc kind="example" id="example">
//       <synopsis>
//       Retrieve the nuclear reaction data structure p_my_reactions from the
//       network structure p_my_network:
//       </synopsis>
//
//       <code>
// p_my_reactions = Libnucnet__Net__getReac( p_my_network );
//       </code>
//     </doc>
//
//   </usage>
//
// </routine>
//############################################################################*/

Libnucnet__Reac *
Libnucnet__Net__getReac( const Libnucnet__Net * );

/*##############################################################################
// <routine name="Libnucnet__Zone__getNet()">
//
//   <description>
//     <abstract>
//       Retrieve the Libnucnet__Net structure from a Libnucnet__Zone structure.
//     </abstract>
//     <keywords>
//       nuclear, network, retrieve, zone
//     </keywords>
//   </description>
//
//   <usage>
//
//     <calling_sequence>
// Libnucnet__Net *
// Libnucnet__Zone__getNet( const Libnucnet__Zone *self );
//     </calling_sequence>
//
//     <param
//       name="self"
//       kind="in,positional,required"
//     >
//       A pointer to a Libnucnet__Zone structure.
//     </param>
//
//     <param
//       kind="return"
//       doc="result"
//     />
//
//     <doc kind="post" id="result">
//       Routine returns the pointer to the Libnucnet__Net structure contained
//       in the Libnucnet__Zone structure.
//       If the input pointer is not valid, Libnucnet error handling is invoked.
//     </doc>
//
//     <doc kind="bug" >
//     </doc>
//
//     <doc kind="example" id="example">
//       <synopsis>
//       Retrieve the nuclear network data structure p_my_network from the
//       zone p_my_zone:
//       </synopsis>
//
//       <code>
// p_my_network = Libnucnet__Zone__getNet( p_my_zone );
//       </code>
//     </doc>
//
//   </usage>
//
// </routine>
//############################################################################*/

Libnucnet__Net *
Libnucnet__Zone__getNet( const Libnucnet__Zone * );

/*##############################################################################
// <routine name="Libnucnet__getNet()">
//
//   <description>
//     <abstract>
//       Retrieve the Libnucnet__Net structure from a Libnucnet structure.
//     </abstract>
//     <keywords>
//       nuclear, network, retrieve
//     </keywords>
//   </description>
//
//   <usage>
//
//     <calling_sequence>
// Libnucnet__Net *Libnucnet__getNet( Libnucnet *self );
//     </calling_sequence>
//
//     <param
//       name="self"
//       kind="in,positional,required"
//     >
//       A pointer to a Libnucnet structure.
//     </param>
//
//     <param
//       kind="return"
//       doc="result"
//     />
//
//     <doc kind="post" id="result">
//       Routine returns the pointer to the Libnucnet__Net structure contained
//       in the Libnucnet structure.
//       If the input pointer is not valid, Libnucnet error handling is invoked.
//     </doc>
//
//     <doc kind="bug" >
//     </doc>
//
//     <doc kind="example" id="example">
//       <synopsis>
//       Retrieve the nuclear network data structure p_my_network from the
//       libnucnet structure p_my_nucnet:
//       </synopsis>
//
//       <code>
// p_my_network = Libnucnet__getNet( p_my_nucnet );
//       </code>
//     </doc>
//
//   </usage>
//
// </routine>
//############################################################################*/

Libnucnet__Net *Libnucnet__getNet( Libnucnet * );

/*##############################################################################
// <routine name="Libnucnet__Net__is_valid_input_xml()">
//
//   <description>
//     <abstract>
//       Validate an xml file for Libnucnet__Net.
//     </abstract>
//     <keywords>
//       Libnucnet, nuclear, xml, hash, validate, network
//     </keywords>
//   </description>
//
//   <usage>
//
//     <calling_sequence>
// int
// Libnucnet__Net__is_valid_input_xml(
//   const char *s_xml_filename
// );
//     </calling_sequence>
//
//     <param
//       name="s_xml_filename" 
//       kind="in,positional,required" 
//     >
//       A string giving the name of the xml file containing the nuclear
//       network data. 
//     </param>
//     <param
//       kind="return"
//       doc="result"
//     />
//
//     <doc kind="post" id="result">
//       For a valid input nuclear network data xml file, the routine returns
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
//         Validate the input Libnucnet__Net xml file "network_data.xml":
//       </synopsis>
//
//       <code>
// if( Libnucnet__Net__is_valid_input_xml( "network_data.xml" ) )
//     printf( "Valid xml!\n" );
//       </code>
//     </doc>
//
//   </usage>
//
// </routine>
//############################################################################*/

int Libnucnet__Net__is_valid_input_xml( const char * );

/*##############################################################################
// <routine name="Libnucnet__is_valid_input_xml()">
//
//   <description>
//     <abstract>
//       Validate an xml file for Libnucnet.
//     </abstract>
//     <keywords>
//       Libnucnet, nuclear, xml, hash, validate
//     </keywords>
//   </description>
//
//   <usage>
//
//     <calling_sequence>
// int
// Libnucnet__is_valid_input_xml(
//   const char *s_xml_filename
// );
//     </calling_sequence>
//
//     <param
//       name="s_xml_filename" 
//       kind="in,positional,required" 
//     >
//       A string giving the name of the xml file containing the nuclear
//       network and zone data. 
//     </param>
//     <param
//       kind="return"
//       doc="result"
//     />
//
//     <doc kind="post" id="result">
//       For a valid input Libnucnet data xml file, the routine returns
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
//         Validate the input xml file "libnucnet_input.xml":
//       </synopsis>
//
//       <code>
// if( Libnucnet__is_valid_input_xml( "libnucnet_input.xml" ) ) {
//     printf( "Valid xml!\n" );
// }
//       </code>
//     </doc>
//
//   </usage>
//
// </routine>
//############################################################################*/

int Libnucnet__is_valid_input_xml( const char * );

/*##############################################################################
// <routine name="Libnucnet__is_valid_zone_data_xml()">
//
//   <description>
//     <abstract>
//       Validate an xml file for zone data.
//     </abstract>
//     <keywords>
//       Libnucnet, nuclear, xml, hash, validate, zone, data
//     </keywords>
//   </description>
//
//   <usage>
//
//     <calling_sequence>
// int
// Libnucnet__is_valid_zone_data_xml(
//   const char *s_xml_filename
// );
//     </calling_sequence>
//
//     <param
//       name="s_xml_filename" 
//       kind="in,positional,required" 
//     >
//       A string giving the name of the xml file containing the
//       zone data. 
//     </param>
//     <param
//       kind="return"
//       doc="result"
//     />
//
//     <doc kind="post" id="result">
//       For a valid input zone data xml file, the routine returns
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
//         Validate the input xml file "my_zone_data.xml":
//       </synopsis>
//
//       <code>
// if( Libnucnet__is_valid_zone_data_xml( "my_zone_data.xml" ) ) {
//     printf( "Valid xml!\n" );
// }
//       </code>
//     </doc>
//
//   </usage>
//
// </routine>
//############################################################################*/

int Libnucnet__is_valid_zone_data_xml( const char * );

/*##############################################################################
// <routine name="Libnucnet__assignZoneDataFromXml()">
//
//   <description>
//     <abstract>
//       Reads in zone data from an XML file, creates
//       zones, and stores the data in a Libnucnet structure.
//     </abstract>
//     <keywords>
//       Libnucnet, nuclear network, xpath, xml, reactions
//     </keywords>
//   </description>
//
//   <usage>
//
//     <calling_sequence>
// void
// Libnucnet__assignZoneDataFromXml(
//   Libnucnet *self,
//   const char *s_xml_filename,
//   const char *s_zone_xpath
// );
//     </calling_sequence>
//
//     <param
//       name="self"
//       kind="in,positional,required"
//     >
//       A pointer to a valid, pre-existing Libnucnet structure.
//     </param>
//     <param
//       name="s_xml_filename"
//       kind="in,positional,required"
//     >
//       A string giving the name of the xml file containing the initial
//       mass fraction data.  This may be the name of a local file or a URL.
//     </param>
//     <param
//       name="s_zone_xpath"
//       kind="in,positional,required"
//     >
//       A string giving the xpath expression to be applied to the zone data.
//     </param>
//     <param
//       kind="return"
//       doc="result"
//     />
//
//     <doc kind="post" id="result">
//       If the input is valid, the Libnucnet structure has been updated
//       with the zones.
//     </doc>
//
//     <doc kind="bug" >
//     </doc>
//
//     <doc kind="example" id="example1">
//       <synopsis>
//       Store the initial mass fraction data in input_mass.xml
//       in p_my_nucnet:
//       </synopsis>
//
//       <code>
// Libnucnet__assignZoneDataFromXml(
//   p_my_nucnet,
//   "input_mass.xml",
//   NULL
// );
//       </code>
//     </doc>
//
//     <doc kind="example" id="example2">
//       <synopsis>
//       Store the initial mass fraction data from mass_fractions.xml but
//       only from zones with value 0 for
//       label 1:
//       </synopsis>
//
//       <code>
// Libnucnet__assignZoneDataFromXml(
//   p_my_nucnet,
//   "mass_fractions.xml",
//   "[@label1='0']"
// );
//       </code>
//     </doc>
//   </usage>
//
// </routine>
//############################################################################*/

void
Libnucnet__assignZoneDataFromXml(
  Libnucnet *,
  const char *,
  const char *
);

/*##############################################################################
// <routine name="Libnucnet__Zone__computeZMoment()">
//
//   <description>
//     <abstract>
//      Compute the Z moment (Z to an integer power times abundance) for a zone.
//     </abstract>
//     <keywords>
//       Libnucnet, moment, xml, hash, zone
//     </keywords>
//   </description>
//
//   <usage>
//
//     <calling_sequence>
// double
// Libnucnet__Zone__computeZMoment(
//   const Libnucnet__Zone *self,
//   int n
// );
//     </calling_sequence>
//
//     <param
//       name="self" 
//       kind="in,positional,required" 
//     >
//       A pointer to a Libnucnet__Zone structure.
//     </param>
//     <param
//       name="n" 
//       kind="in,positional,required" 
//     >
//       The power to which to raise the Z of each species in computing
//       the moment.
//     </param>
//     <param
//       kind="return"
//       doc="result"
//     />
//
//     <doc kind="post" id="result">
//       For valid input, the routine returns the sum over all species
//       of Z of the species raised to power n times the abundance of that
//       species in the zone. For invalid input, error handling is invoked.
//     </doc>
//
//     <doc kind="bug" >
//     </doc>
//
//     <doc kind="example" id="example1">
//       <synopsis>
//         Compute the Ye for zone p_zone:
//       </synopsis>
//
//       <code>
// d_ye =
//   Libnucnet__Zone__computeZMoment(
//     p_zone, 1
//   );
//       </code>
//     </doc>
//
//   </usage>
//
// </routine>
//############################################################################*/

double
Libnucnet__Zone__computeZMoment( const Libnucnet__Zone *, int );

/*##############################################################################
// <routine name="Libnucnet__Zone__computeAMoment()">
//
//   <description>
//     <abstract>
//      Compute the A moment (A to an integer power times abundance) for a zone.
//     </abstract>
//     <keywords>
//       Libnucnet, moment, xml, hash, zone
//     </keywords>
//   </description>
//
//   <usage>
//
//     <calling_sequence>
// double
// Libnucnet__Zone__computeAMoment(
//   const Libnucnet__Zone *self, int n
// );
//     </calling_sequence>
//
//     <param
//       name="self" 
//       kind="in,positional,required" 
//     >
//       A pointer to a Libnucnet__Zone structure.
//     </param>
//     <param
//       name="n" 
//       kind="in,positional,required" 
//     >
//       The power to which to raise the A of each species in computing
//       the moment.
//     </param>
//     <param
//       kind="return"
//       doc="result"
//     />
//
//     <doc kind="post" id="result">
//       For valid input, the routine returns the sum over all species
//       of A of the species raised to power n times the abundance of that
//       species in the zone. For invalid input, error handling is invoked.
//     </doc>
//
//     <doc kind="bug" >
//     </doc>
//
//     <doc kind="example" id="example1">
//       <synopsis>
//         Compute the sum of mass fractions for zone p_zone:
//       </synopsis>
//
//       <code>
// d_xsum =
//   Libnucnet__Zone__computeAMoment(
//     p_zone, 1
//   );
//       </code>
//     </doc>
//
//   </usage>
//
// </routine>
//############################################################################*/

double
Libnucnet__Zone__computeAMoment( const Libnucnet__Zone *, int );

/*##############################################################################
// <routine name="Libnucnet__Zone__setScreeningFunction()">
//
//   <description>
//     <abstract>
//       Set the user-supplied screening factor function and its associated
//       data.
//     </abstract>
//     <keywords>
//       Libnucnet, screening, factor, zone, data
//     </keywords>
//   </description>
//
//   <usage>
//
//     <calling_sequence>
// void
// Libnucnet__Zone__setScreeningFunction(
//   Libnucnet__Zone *self,
//   Libnucnet__Net__screeningFunction pf_user_function,
//   void *p_user_data
// );
//     </calling_sequence>
//
//     <param
//       name="self" 
//       kind="in,positional,required" 
//     >
//       A pointer to a Libnucnet__Zone structure.
//     </param>
//     <param
//       name="pf_user_function" 
//       kind="in,positional,required" 
//     >
//       The name of the user-supplied screening function.
//     </param>
//     <param
//       name="p_user_data" 
//       kind="in,positional,required" 
//     >
//       A pointer to the user-supplied data structure containing extra
//       data for the screening function (or NULL if no other data).
//       The user must cast the type for this data structure in the
//       prototype for the user-supplied function and in the function itself.
//     </param>
//     <param
//       kind="return"
//       doc="result"
//     />
//
//     <doc kind="post" id="result">
//       For valid input, the routine sets the screening function to
//       that supplied and the data to the user's data structure.  These
//       are applied in the routine Libnucnet__Zone__computeRates().
//     </doc>
//
//     <doc kind="bug" >
//     </doc>
//
//     <doc kind="example" id="example1">
//       <synopsis>
//         Set the user-supplied screening routine to my_screening_function
//         and pass no extra data for rate calculations in Libnucnet__Zone
//         p_zone:
//       </synopsis>
//
//       <code>
// Libnucnet__Zone__setScreeningFunction(
//   p_zone,
//   (Libnucnet__Net__screeningFunction) my_screening_function,
//   NULL
// ); 
//       </code>
//     </doc>
//
//     <doc kind="example" id="example2">
//       <synopsis>
//         Set the user-supplied screening routine to my_screening_function
//         and pass extra data from the structure my_data for rate
//         calculations in Libnucnet__Zone p_zone:
//       </synopsis>
//
//       <code>
// struct my_data = { double x; double y; };
// my_data.x = 1.;
// my_data.y = 2.;
// Libnucnet__Zone__setScreeningFunction(
//   p_zone,
//   (Libnucnet__Net__screeningFunction) my_screening_function,
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
Libnucnet__Zone__setScreeningFunction(
  Libnucnet__Zone *,
  Libnucnet__Net__screeningFunction,
  void *
);

/*##############################################################################
// <routine name="Libnucnet__Zone__setNseCorrectionFactorFunction()">
//
//   <description>
//     <abstract>
//       Set the user-supplied correction factor function and its associated
//       data.
//     </abstract>
//     <keywords>
//       Libnucnet, Coulomb, correction, factor, zone, data
//     </keywords>
//   </description>
//
//   <usage>
//
//     <calling_sequence>
// void
// Libnucnet__Zone__setNseCorrectionFactorFunction(
//   Libnucnet__Zone *self,
//   Libnucnet__Species__nseCorrectionFactorFunction pf_user_function,
//   void *p_user_data
// );
//     </calling_sequence>
//
//     <param
//       name="self" 
//       kind="in,positional,required" 
//     >
//       A pointer to a Libnucnet__Zone structure.
//     </param>
//     <param
//       name="pf_user_function" 
//       kind="in,positional,required" 
//     >
//       The name of the user-supplied correction factor function.
//     </param>
//     <param
//       name="p_user_data" 
//       kind="in,positional,required" 
//     >
//       A pointer to the user-supplied data structure containing extra
//       data for the correction factor function (or NULL if no other data).
//       The user must cast the type for this data structure in the
//       prototype for the user-supplied function and in the function itself.
//     </param>
//     <param
//       kind="return"
//       doc="result"
//     />
//
//     <doc kind="post" id="result">
//       For valid input, the routine sets the correction factor function to
//       that supplied and the data to the user's data structure.  These
//       are applied in the routine Libnucnet__Zone__computeRates().
//     </doc>
//
//     <doc kind="bug" >
//     </doc>
//
//     <doc kind="example" id="example1">
//       <synopsis>
//         Set the user-supplied correction factor routine to
//         my_correction_function and pass no extra data for rate calculations
//         in Libnucnet__Zone  p_zone:
//       </synopsis>
//
//       <code>
// Libnucnet__Zone__setNseCorrectionFactorFunction(
//   p_zone,
//   (Libnucnet__Species__nseCorrectionFactorFunction)
//     my_correction_function,
//   NULL
// ); 
//       </code>
//     </doc>
//
//     <doc kind="example" id="example2">
//       <synopsis>
//         Set the user-supplied correction factor routine to
//         my_screening_function and pass extra data from the structure
//         my_data for rate calculations in Libnucnet__Zone p_zone:
//       </synopsis>
//
//       <code>
// struct my_data = { double x; double y; };
// my_data.x = 1.;
// my_data.y = 2.;
// Libnucnet__Zone__setNseCorrectionFactorFunction(
//   p_zone,
//   (Libnucnet__Species__nseCorrectionFactorFunction)
//     my_correction_function,
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
Libnucnet__Zone__setNseCorrectionFactorFunction(
  Libnucnet__Zone *,
  Libnucnet__Species__nseCorrectionFactorFunction,
  void *
);

/*##############################################################################
// <routine name="Libnucnet__Zone__getScreeningFactorForReaction()">
//
//   <description>
//     <abstract>
//       Retrieve the screening factor applied to a reaction in a zone.
//     </abstract>
//     <keywords>
//       Libnucnet, screening, factor, zone, data
//     </keywords>
//   </description>
//
//   <usage>
//
//     <calling_sequence>
// double
// Libnucnet__Zone__getScreeningFactorForReaction(
//   const Libnucnet__Zone *self,
//   const Libnucnet__Reaction *p_reaction
// );
//     </calling_sequence>
//
//     <param
//       name="self" 
//       kind="in,positional,required" 
//     >
//       A pointer to a Libnucnet__Zone structure.
//     </param>
//     <param
//       name="p_reaction"
//       kind="in,positional,required" 
//     >
//       A pointer to the reaction whose screening factor is desired.
//     </param>
//     <param
//       kind="return"
//       doc="result"
//     />
//
//     <doc kind="post" id="result">
//       For valid input, the routine returns the screening factor that
//       was applied in computing the rates for a given zone.  If any
//       input is invalid, error handling is invoked.
//     </doc>
//
//     <doc kind="bug" >
//     </doc>
//
//     <doc kind="example" id="example1">
//       <synopsis>
//         Print the screening factor that was applied to the reaction
//         c12 + he4 -> o16 + gamma in the Libnucnet__Zone p_zone:
//       </synopsis>
//
//       <code>
// printf(
//   "Screening factor = %e\n",
//   Libnucnet__Zone__getScreeningFactorForReaction(
//     p_zone,
//     Libnucnet__Reac__getReactionByString(
//       Libnucnet__Net__getReac(
//         Libnucnet__Zone__getNet( p_zone )
//       ),
//       "c12 + he4 -> o16 + gamma"
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
Libnucnet__Zone__getScreeningFactorForReaction(
  const Libnucnet__Zone *,
  const Libnucnet__Reaction *
);

/*##############################################################################
// <routine
//    name="Libnucnet__Zone__getReverseRatioCorrectionFactorForReaction()"
// >
//
//   <description>
//     <abstract>
//       Retrieve the correction factor applied to a reaction in a zone.
//     </abstract>
//     <keywords>
//       Libnucnet, correction, factor, zone, data
//     </keywords>
//   </description>
//
//   <usage>
//
//     <calling_sequence>
// double
// Libnucnet__Zone__getReverseRatioCorrectionFactorForReaction(
//   const Libnucnet__Zone *self,
//   const Libnucnet__Reaction *p_reaction
// );
//     </calling_sequence>
//
//     <param
//       name="self" 
//       kind="in,positional,required" 
//     >
//       A pointer to a Libnucnet__Zone structure.
//     </param>
//     <param
//       name="p_reaction"
//       kind="in,positional,required" 
//     >
//       A pointer to the reaction whose correction factor is desired.
//     </param>
//     <param
//       kind="return"
//       doc="result"
//     />
//
//     <doc kind="post" id="result">
//       For valid input, the routine returns the correction factor that
//       was applied in computing the rates for a given zone.  If any
//       input is invalid, error handling is invoked.
//     </doc>
//
//     <doc kind="bug" >
//     </doc>
//
//     <doc kind="example" id="example1">
//       <synopsis>
//         Print the correction factor that was applied to the reaction
//         si28 + he4 -> s32 + gamma in the Libnucnet__Zone p_zone:
//       </synopsis>
//
//       <code>
// printf(
//   "Correction factor = %e\n",
//   Libnucnet__Zone__getReverseRatioCorrectionFactorForReaction(
//     p_zone,
//     Libnucnet__Reac__getReactionByString(
//       Libnucnet__Net__getReac(
//         Libnucnet__Zone__getNet( p_zone )
//       ),
//       "si28 + he4 -> s32 + gamma"
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
Libnucnet__Zone__getReverseRatioCorrectionFactorForReaction(
  const Libnucnet__Zone *,
  const Libnucnet__Reaction *
);

/*##############################################################################
// <routine name="Libnucnet__Zone__clearScreeningFunction()">
//
//   <description>
//     <abstract>
//       Clear the screening function for a zone.
//     </abstract>
//     <keywords>
//       nuclear, clear, network, screening, function, zone
//     </keywords>
//   </description>
//
//   <usage>
//
//     <calling_sequence>
// void
// Libnucnet__Zone__clearScreeningFunction(
//   Libnucnet__Zone *self
// );
//     </calling_sequence>
//
//     <param
//       name="self"
//       kind="in,positional,required"
//     >
//       A pointer to a Libnucnet__Zone structure.
//     </param>
//
//     <param
//       kind="return"
//       doc="result"
//     />
//
//     <doc kind="post" id="result">
//       On return, the screening function has been cleared and reset to
//       the default (no screening).
//       If the input pointer is not valid, Libnucnet error handling is invoked.
//     </doc>
//
//     <doc kind="bug" >
//     </doc>
//
//     <doc kind="example" id="example">
//       <synopsis>
//       Clear the screening function for Libnucnet__Zone p_zone:
//       </synopsis>
//
//       <code>
// Libnucnet__Zone__clearScreeningFunction( p_zone );
//       </code>
//     </doc>
//
//   </usage>
//
// </routine>
//############################################################################*/

void
Libnucnet__Zone__clearScreeningFunction( Libnucnet__Zone * );

/*##############################################################################
// <routine name="Libnucnet__Zone__clearNseCorrectionFactorFunction()">
//
//   <description>
//     <abstract>
//       Clear the NSE correction factor function for a zone.
//     </abstract>
//     <keywords>
//       nuclear, clear, network, NSE, correction, factor, function, zone
//     </keywords>
//   </description>
//
//   <usage>
//
//     <calling_sequence>
// void
// Libnucnet__Zone__clearNseCorrectionFactorFunction(
//   Libnucnet__Zone *self
// );
//     </calling_sequence>
//
//     <param
//       name="self"
//       kind="in,positional,required"
//     >
//       A pointer to a Libnucnet__Zone structure.
//     </param>
//
//     <param
//       kind="return"
//       doc="result"
//     />
//
//     <doc kind="post" id="result">
//       On return, the NSE correction factor function has been cleared
//       and reset to the default (no correction).
//       If the input pointer is not valid, Libnucnet error handling is invoked.
//     </doc>
//
//     <doc kind="bug" >
//     </doc>
//
//     <doc kind="example" id="example">
//       <synopsis>
//       Clear the NSE correction factor function for Libnucnet__Zone p_zone:
//       </synopsis>
//
//       <code>
// Libnucnet__Zone__clearNseCorrectionFactorFunction( p_zone );
//       </code>
//     </doc>
//
//   </usage>
//
// </routine>
//############################################################################*/

void
Libnucnet__Zone__clearNseCorrectionFactorFunction( Libnucnet__Zone * );

/*##############################################################################
// <routine name="Libnucnet__Net__computeScreeningFactorForReaction()">
//
//   <description>
//     <abstract>
//       Use the user-supplied screening factor function and its associated
//       data to compute the screening factor for a reaction.
//     </abstract>
//     <keywords>
//       Libnucnet, screening, factor, zone, data, reaction
//     </keywords>
//   </description>
//
//   <usage>
//
//     <calling_sequence>
// double
// Libnucnet__Net__computeScreeningFactorForReaction(
//   Libnucnet__Net *self,
//   Libnucnet__Reaction *p_reaction,
//   double d_t9,
//   double d_rho,
//   double d_ye,
//   Libnucnet__Net__screeningFunction p_user_function,
//   void *p_user_data
// );
//     </calling_sequence>
//
//     <param
//       name="self" 
//       kind="in,positional,required" 
//     >
//       A pointer to a Libnucnet__Zone structure.
//     </param>
//     <param
//       name="p_reaction" 
//       kind="in,positional,required" 
//     >
//       A pointer to a Libnucnet__Reaction structure.
//     </param>
//     <param
//       name="d_t9" 
//       kind="in,positional,required" 
//     >
//       A double giving the temperature in billions of K at which to
//       compute the screening factor.
//     </param>
//     <param
//       name="d_rho" 
//       kind="in,positional,required" 
//     >
//       A double giving the density in g/cc at which to
//       compute the screening factor.
//     </param>
//     <param
//       name="d_ye" 
//       kind="in,positional,required" 
//     >
//       A double giving the electron-to-baryon ratio at which to
//       compute the screening factor.
//     </param>
//     <param
//       name="p_user_function" 
//       kind="in,positional,required" 
//     >
//       The name of the user-supplied screening function.
//     </param>
//     <param
//       name="p_user_data" 
//       kind="in,positional,required" 
//     >
//       A pointer to the user-supplied data structure containing extra
//       data for the screening function (or NULL if no other data).
//       The user must cast the type for this data structure in the
//       prototype for the user-supplied function and in the function itself.
//     </param>
//     <param
//       kind="return"
//       doc="result"
//     />
//
//     <doc kind="post" id="result">
//       For valid input, the routine returns a double with the screening
//       factor as computed by the user's supplied function and data.
//     </doc>
//
//     <doc kind="bug" >
//     </doc>
//
//     <doc kind="example" id="example1">
//       <synopsis>
//         Compute the screening factor for the reaction c12 + h1 -> n13
//         + gamma at t9 = 2, rho = 1.e11, and Ye = 0.5.  Use the
//         user-supplied screening routine my_screening_function
//         and pass no extra data for the screening calculation:
//       </synopsis>
//
//       <code>
// p_reaction =
//   Libnucnet__Reac__getReactionByString(
//     Libnucnet__Net__getReac( p_my_net ),
//     "c12 + h1 -> n13 + gamma"
//   );
// d_screen =
//   Libnucnet__Net__computeScreeningFactorForReaction(
//     p_my_net,
//     p_reaction,
//     2.,
//     1.e11,
//     0.5,
//     (Libnucnet__Net__screeningFunction) my_screening_function,
//     NULL
//   ); 
//       </code>
//     </doc>
//
//     <doc kind="example" id="example2">
//       <synopsis>
//         Compute the screening factor for the reaction c12 + h1 -> n13
//         + gamma at t9 = 2, rho = 1.e11, and Ye = 0.5.  Use the
//         user-supplied screening routine my_screening_function
//         and pass extra data for the screening calculation through the
//         data structure my_data:
//       </synopsis>
//
//       <code>
// struct my_data = { double x = 1.; double y = 2.; };
// p_reaction =
//   Libnucnet__Reac__getReactionByString(
//     Libnucnet__Net__getReac( p_my_net ),
//     "c12 + h1 -> n13 + gamma"
//   );
// d_screen =
//   Libnucnet__Net__computeScreeningFactorForReaction(
//     p_my_net,
//     p_reaction,
//     2.,
//     1.e11,
//     0.5,
//     (Libnucnet__Net__screeningFunction) my_screening_function,
//     &my_data
//   ); 
//       </code>
//     </doc>
//
//   </usage>
//
// </routine>
//############################################################################*/

double
Libnucnet__Net__computeScreeningFactorForReaction(
  Libnucnet__Net *,
  Libnucnet__Reaction *,
  double,
  double,
  double,
  Libnucnet__Net__screeningFunction,
  void *
);

/*##############################################################################
// <routine
//   name="Libnucnet__Net__computeReverseRatioCorrectionFactorForReaction()"
// >
//
//   <description>
//     <abstract>
//       Use the user-supplied NSE correction factor function and its associated
//       data to compute the reverse ratio correction factor for a reaction.
//     </abstract>
//     <keywords>
//       Libnucnet, Nse, reverse, factor, zone, data, reaction
//     </keywords>
//   </description>
//
//   <usage>
//
//     <calling_sequence>
// double
// Libnucnet__Net__computeReverseRatioCorrectionFactorForReaction(
//   Libnucnet__Net *self,
//   Libnucnet__Reaction *p_reaction,
//   double d_t9,
//   double d_rho,
//   double d_ye,
//   Libnucnet__Species__nseCorrectionFactorFunction p_user_function,
//   void *p_user_data
// );
//     </calling_sequence>
//
//     <param
//       name="self" 
//       kind="in,positional,required" 
//     >
//       A pointer to a Libnucnet__Zone structure.
//     </param>
//     <param
//       name="p_reaction" 
//       kind="in,positional,required" 
//     >
//       A pointer to a Libnucnet__Reaction structure.
//     </param>
//     <param
//       name="d_t9" 
//       kind="in,positional,required" 
//     >
//       A double giving the temperature in billions of K at which to
//       compute the reverse ratio correction factor.
//     </param>
//     <param
//       name="d_rho" 
//       kind="in,positional,required" 
//     >
//       A double giving the density in g/cc at which to
//       compute the reverse ratio correction factor.
//     </param>
//     <param
//       name="d_ye" 
//       kind="in,positional,required" 
//     >
//       A double giving the electron-to-baryon ratio at which to
//       compute the reverse ratio correction factor.
//     </param>
//     <param
//       name="p_user_function" 
//       kind="in,positional,required" 
//     >
//       The name of the user-supplied NSE correction factor function.
//     </param>
//     <param
//       name="p_user_data" 
//       kind="in,positional,required" 
//     >
//       A pointer to the user-supplied data structure containing extra
//       data for the NSE correction factor function (or NULL if no other data).
//       The user must cast the type for this data structure in the
//       prototype for the user-supplied function and in the function itself.
//     </param>
//     <param
//       kind="return"
//       doc="result"
//     />
//
//     <doc kind="post" id="result">
//       For valid input, the routine returns a double with the reverse ratio
//       correction factor as computed by the user's supplied function and data.
//     </doc>
//
//     <doc kind="bug" >
//     </doc>
//
//     <doc kind="example" id="example1">
//       <synopsis>
//         Compute the reverse ratio correction
//         factor for the reaction c12 + h1 -> n13
//         + gamma at t9 = 2, rho = 1.e11, and Ye = 0.5.  Use the
//         user-supplied screening routine my_correction_function
//         and pass no extra data for the correction calculation:
//       </synopsis>
//
//       <code>
// p_reaction =
//   Libnucnet__Reac__getReactionByString(
//     Libnucnet__Net__getReac( p_my_net ),
//     "c12 + h1 -> n13 + gamma"
//   );
// d_correction =
//   Libnucnet__Net__computeReverseRatioCorrectionFactorForReaction(
//     p_my_net,
//     p_reaction,
//     2.,
//     1.e11,
//     0.5,
//     (Libnucnet__Species__nseCorrectionFactorFunction)
//       my_correction_function,
//     NULL
//   ); 
//       </code>
//     </doc>
//
//     <doc kind="example" id="example2">
//       <synopsis>
//         Compute the reverse ratio correction
//         factor for the reaction c12 + h1 -> n13
//         + gamma at t9 = 2, rho = 1.e11, and Ye = 0.5.  Use the
//         user-supplied correction routine my_correction_function
//         and pass extra data for the correction calculation through the
//         data structure my_data:
//       </synopsis>
//
//       <code>
// struct my_data = { double x; double y; };
// my_data.x = 1.;
// my_data.y = 2.;
// p_reaction =
//   Libnucnet__Reac__getReactionByString(
//     Libnucnet__Net__getReac( p_my_net ),
//     "c12 + h1 -> n13 + gamma"
//   );
// d_correction =
//   Libnucnet__Net__computeReverseRatioCorrectionFactorForReaction(
//     p_my_net,
//     p_reaction,
//     2.,
//     1.e11,
//     0.5,
//     (Libnucnet__Species__nseCorrectionFactorFunction)
//       my_correction_function,
//     &my_data
//   ); 
//       </code>
//     </doc>
//
//   </usage>
//
// </routine>
//############################################################################*/

double
Libnucnet__Net__computeReverseRatioCorrectionFactorForReaction(
  Libnucnet__Net *,
  Libnucnet__Reaction *,
  double,
  double,
  double,
  Libnucnet__Species__nseCorrectionFactorFunction,
  void *
);

/*##############################################################################
// <routine name="Libnucnet__Zone__getAbundances()">
//
//   <description>
//     <abstract>
//       Routine to return a gsl_vector containing the abundances
//       of a given zone.
//     </abstract>
//     <keywords>
//       Libnucnet, zone, abundances
//     </keywords>
//   </description>
//
//   <usage>
//
//     <calling_sequence>
// gsl_vector *
// Libnucnet__Zone__getAbundances( const Libnucnet__Zone *self );
//     </calling_sequence>
//
//     <param
//       name="self"
//       kind="in,positional,required"
//       doc="self"
//     >
//       A pointer to a valid Libnucnet__Zone structure.
//     </param>
//     <param
//       kind="return"
//       doc="result"
//     />
//
//     <doc kind="post" id="result">
//       Routine returns a new gsl_vector containing the current
//       abundances in the zone.  The abundances are ordered according
//       to their index.  If the input zone is not valid, Libnucnet error
//       handling is invoked.
//     </doc>
//
//     <doc kind="bug" >
//     </doc>
//
//     <doc kind="example" id="example1">
//       <synopsis>
//         Retrieve from Libnucnet *p_my_zones the abundances in the zone
//         with labels "x1", "y1", "z1":
//       </synopsis>
//
//       <code>
// if(
//   (
//     p_zone =
//       Libnucnet__getZoneByLabels( p_my_zones, "x1", "y1", "z1" )
//   )
// )
//   p_abunds =
//     Libnucnet__Zone__getAbundances( p_zone );
//       </code>
//     </doc>
//
//   </usage>
//
// </routine>
//############################################################################*/

gsl_vector *
Libnucnet__Zone__getAbundances( const Libnucnet__Zone * );

/*##############################################################################
// <routine name="Libnucnet__Zone__getAbundanceChanges()">
//
//   <description>
//     <abstract>
//       Routine to return a gsl_vector containing the changes in the abundances
//       of a given zone.
//     </abstract>
//     <keywords>
//       Libnucnet, zone, abundances
//     </keywords>
//   </description>
//
//   <usage>
//
//     <calling_sequence>
// gsl_vector *
// Libnucnet__Zone__getAbundanceChanges( const Libnucnet__Zone *self );
//     </calling_sequence>
//
//     <param
//       name="self"
//       kind="in,positional,required"
//       doc="self"
//     >
//       A pointer to a valid Libnucnet__Zone structure.
//     </param>
//     <param
//       kind="return"
//       doc="result"
//     />
//
//     <doc kind="post" id="result">
//       Routine returns a new gsl_vector containing the current
//       abundances in the zone.  The abundances are ordered according
//       to their index.  If the input zone is not valid, Libnucnet error
//       handling is invoked.
//     </doc>
//
//     <doc kind="bug" >
//     </doc>
//
//     <doc kind="example" id="example1">
//       <synopsis>
//         Retrieve from Libnucnet *p_my_zones the abundances in the zone
//         with labels "x1", "y1", "z1":
//       </synopsis>
//
//       <code>
// if(
//   (
//     p_zone =
//       Libnucnet__getZoneByLabels( p_my_zones, "x1", "y1", "z1" )
//   )
// )
//   p_abund_changes =
//     Libnucnet__Zone__getAbundanceChanges( p_zone );
//       </code>
//     </doc>
//
//   </usage>
//
// </routine>
//############################################################################*/

gsl_vector *
Libnucnet__Zone__getAbundanceChanges( const Libnucnet__Zone * );

/*##############################################################################
// <routine name="Libnucnet__Zone__updateAbundances()">
//
//   <description>
//     <abstract>
//       Routine to update the abundances of a given zone from an
//       input gsl_vector.
//     </abstract>
//     <keywords>
//       Libnucnet, zone, abundances
//     </keywords>
//   </description>
//
//   <usage>
//
//     <calling_sequence>
// void
// Libnucnet__Zone__updateAbundances(
//   Libnucnet__Zone *self
//   gsl_vector *p_abunds
// );
//     </calling_sequence>
//
//     <param
//       name="self"
//       kind="in,positional,required"
//       doc="self"
//     >
//       A pointer to a valid Libnucnet__Zone structure.
//     </param>
//     <param
//       name="p_abunds"
//       kind="in,positional,required"
//       doc="abunds"
//     >
//       A pointer to a gsl_vector containing the new abundances.
//     </param>
//     <param
//       kind="return"
//       doc="result"
//     />
//
//     <doc kind="post" id="result">
//       Upon successful return, the abundances have been updated to
//       those in the input vector. If the input zone is not valid, or
//       if the number of elements in the input vector is not equal to
//       the number of species in the zone's network, Libnucnet error
//       handling is invoked.
//     </doc>
//
//     <doc kind="bug" >
//     </doc>
//
//     <doc kind="example" id="example1">
//       <synopsis>
//         Update the abundances in Libnucnet__Zone *p_zone with the
//         abundances in gsl_vector *p_my_new_abundances:
//       </synopsis>
//
//       <code>
// Libnucnet__Zone__updateAbundances(
//   p_zone,
//   p_my_new_abundances
// );
//       </code>
//     </doc>
//
//   </usage>
//
// </routine>
//############################################################################*/

void
Libnucnet__Zone__updateAbundances( Libnucnet__Zone *, gsl_vector * );

/*##############################################################################
// <routine name="Libnucnet__Zone__updateAbundanceChanges()">
//
//   <description>
//     <abstract>
//       Routine to update the abundance changes of a given zone from an
//       input gsl_vector.
//     </abstract>
//     <keywords>
//       Libnucnet, zone, abundance, changes
//     </keywords>
//   </description>
//
//   <usage>
//
//     <calling_sequence>
// void
// Libnucnet__Zone__updateAbundanceChanges(
//   Libnucnet__Zone *self
//   gsl_vector *p_abunds
// );
//     </calling_sequence>
//
//     <param
//       name="self"
//       kind="in,positional,required"
//       doc="self"
//     >
//       A pointer to a valid Libnucnet__Zone structure.
//     </param>
//     <param
//       name="p_abunds"
//       kind="in,positional,required"
//       doc="abunds"
//     >
//       A pointer to a gsl_vector containing the new abundance changes.
//     </param>
//     <param
//       kind="return"
//       doc="result"
//     />
//
//     <doc kind="post" id="result">
//       Upon successful return, the abundance changes have been updated to
//       those in the input vector. If the input zone is not valid, or
//       if the number of elements in the input vector is not equal to
//       the number of species in the zone's network, Libnucnet error
//       handling is invoked.
//     </doc>
//
//     <doc kind="bug" >
//     </doc>
//
//     <doc kind="example" id="example1">
//       <synopsis>
//         Update the abundance changes in Libnucnet__Zone *p_zone with the
//         abundance changes in gsl_vector *p_my_new_abundance_changes:
//       </synopsis>
//
//       <code>
// Libnucnet__Zone__updateAbundanceChanges(
//   p_zone,
//   p_my_new_abundance_changes
// );
//       </code>
//     </doc>
//
//   </usage>
//
// </routine>
//############################################################################*/

void
Libnucnet__Zone__updateAbundanceChanges( Libnucnet__Zone *, gsl_vector * );

/*##############################################################################
// <routine name="Libnucnet__iterateZones()">
//
//   <description>
//     <abstract>
//       Iterate through the zones and apply the user-supplied iterate
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
// Libnucnet__iterateZones(
//   const Libnucnet *self,
//   Libnucnet__Zone__iterateFunction pf_func,
//   void *p_user_data
// );
//     </calling_sequence>
//
//     <param
//       name="self" 
//       kind="in,positional,required" 
//     >
//       A pointer to a Libnucnet structure.
//     </param>
//
//     <param
//       name="pf_func" 
//       kind="in,positional,required" 
//     >
//       The name of the user-supplied function to apply.
//     </param>
//     <param
//       name="p_user_data" 
//       kind="in,positional,required" 
//     >
//       A pointer to the user-defined extra data.
//     </param>
//     <param
//       kind="return"
//       doc="result"
//     />
//
//     <doc kind="post" id="result">
//       The routine iterates through the zones and applies
//       the user-supplied routine to each.  If any input is
//       invalid, error handling is invoked.
//     </doc>
//
//     <doc kind="bug" >
//     </doc>
//
//     <doc kind="example" id="example1">
//       <synopsis>
//         Iterate through the zones in p_my_nucnet and apply the
//         function my_iterate_function and the extra data in
//         p_user_data:
//       </synopsis>
//
//       <code>
// Libnucnet__iterateZones(
//   p_my_nucnet,
//   (Libnucnet__Zone__iterateFunction) my_iterate_function,
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
Libnucnet__iterateZones(
  const Libnucnet *,
  Libnucnet__Zone__iterateFunction,
  void *
);

/*##############################################################################
// <routine name="Libnucnet__writeZoneDataToXmlFile()">
//
//   <description>
//     <abstract>
//       Dump the current data in all zones to an xml file.
//     </abstract>
//     <keywords>
//       Libnucnet, zone, mass, fractions, abundances, xml
//     </keywords>
//   </description>
//
//   <usage>
//
//     <calling_sequence>
// void
// Libnucnet__writeZoneDataToXmlFile(
//   const Libnucnet *self
//   const char *s_xml_file
// );
//     </calling_sequence>
//
//     <param
//       name="self"
//       kind="in,positional,required"
//       doc="self"
//     >
//       A pointer to a valid Libnucnet structure.
//     </param>
//     <param
//       name="s_xml_file"
//       kind="in,positional,required"
//       doc="abunds"
//     >
//       The name of the xml file to which the abundances are to be written.
//     </param>
//     <param
//       kind="return"
//       doc="result"
//     />
//
//     <doc kind="post" id="result">
//       Upon successful return, the mass fractions have been dumped to the
//       xml file.  Zones will be written out in the order defined by the
//       currently defined Libnucnet__Zone__compare_function.  If the input
//       structure is not valid, or the xml file cannot be created,
//       error handling is invoked.
//     </doc>
//
//     <doc kind="bug" >
//     </doc>
//
//     <doc kind="example" id="example1">
//       <synopsis>
//         Dump the current zone data in p_my_libnucnet to the file
//         zone_data.xml.
//       </synopsis>
//
//       <code>
// Libnucnet__writeZoneDataToXmlFile(
//   p_my_libnucnet,
//   "zone_data.xml"
// );
//       </code>
//     </doc>
//
//   </usage>
//
// </routine>
//############################################################################*/

void
Libnucnet__writeZoneDataToXmlFile(
  const Libnucnet *, const char *
);

/*##############################################################################
// <routine name="Libnucnet__Zone__updateProperty()">
//
//   <description>
//     <abstract>
//       Update a zone optional property.
//     </abstract>
//     <keywords>
//       Libnucnet, property, xml, update
//     </keywords>
//   </description>
//
//   <usage>
//
//     <calling_sequence>
// int
// Libnucnet__Zone__updateProperty(
//   Libnucnet__Zone *self,
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
//       A pointer to a Libnucnet__Zone structure.
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
//       about the property.  This is set to NULL if no tag is to be added.
//     </param>
//     <param
//       name="s_tag2" 
//       kind="in,positional,required" 
//     >
//       The name of a second tag for the property containing extra information
//       about the property.  This is set to NULL if no tag is to be added.
//     </param>
//     <param
//       name="s_value" 
//       kind="in,positional,required" 
//     >
//       The value to which the property is to be updated.
//     </param>
//     <param
//       kind="return"
//       doc="result"
//     />
//
//     <doc kind="post" id="result">
//       Routine returns 1 if the update was successful and 0 if not.
//       On successful return, the property has been added to the zone if
//       it did not previously exist or has been updated if it did.
//       If the input are invalid, error handling is invoked.
//     </doc>
//
//     <doc kind="bug" >
//     </doc>
//
//     <doc kind="example" id="example1">
//       <synopsis>
//         Update the optional property t9 to 4.5 in p_zone:
//       </synopsis>
//
//       <code>
// Libnucnet__Zone__updateProperty(
//   p_zone,
//   "t9",
//   NULL,
//   NULL,
//   "4.5"
// );
//       </code>
//     </doc>
//
//   </usage>
//
// </routine>
//############################################################################*/

int
Libnucnet__Zone__updateProperty(
  Libnucnet__Zone *,
  const char *,
  const char *,
  const char *,
  const char *
);

/*##############################################################################
// <routine name="Libnucnet__Zone__getProperty()">
//
//   <description>
//     <abstract>
//       Retrieve a zone optional property.
//     </abstract>
//     <keywords>
//       Libnucnet, property, xml, retrieve
//     </keywords>
//   </description>
//
//   <usage>
//
//     <calling_sequence>
// const char *
// Libnucnet__Zone__getProperty(
//   const Libnucnet__Zone *self,
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
//       A pointer to a Libnucnet__Zone structure.
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
//       about the property or NULL.
//     </param>
//     <param
//       name="s_tag2" 
//       kind="in,positional,required" 
//     >
//       The name of a second tag for the property containing extra information
//       about the property or NULL.
//     </param>
//     <param
//       kind="return"
//       doc="result"
//     />
//
//     <doc kind="post" id="result">
//       Routine returns a const char * containing the value of the property.
//       If the property is not found, routine returns NULL.
//     </doc>
//
//     <doc kind="bug" >
//     </doc>
//
//     <doc kind="example" id="example1">
//       <synopsis>
//         Print the optional property t9 in p_zone:
//       </synopsis>
//
//       <code>
// printf(
//   "t9 = %g\n",
//   atof(
//     Libnucnet__Zone__getProperty(
//       p_zone,
//       "t9",
//       NULL,
//       NULL
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

const char *
Libnucnet__Zone__getProperty(
  const Libnucnet__Zone *,
  const char *,
  const char *,
  const char *
);

/*##############################################################################
// <routine name="Libnucnet__Zone__removeProperty()">
//
//   <description>
//     <abstract>
//       Remove a zone optional property.
//     </abstract>
//     <keywords>
//       Libnucnet, property, xml, remove
//     </keywords>
//   </description>
//
//   <usage>
//
//     <calling_sequence>
// int
// Libnucnet__Zone__removeProperty(
//   Libnucnet__Zone *self,
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
//       A pointer to a Libnucnet__Zone structure.
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
//       about the property or NULL.
//     </param>
//     <param
//       name="s_tag2" 
//       kind="in,positional,required" 
//     >
//       The name of a second tag for the property containing extra information
//       about the property or NULL.
//     </param>
//     <param
//       kind="return"
//       doc="result"
//     />
//
//     <doc kind="post" id="result">
//       Routine returns 1 if removal was successful and 0 if not.
//       On successful return, the property has been removed from the zone.
//       If the input are invalid, error handling is invoked.
//     </doc>
//
//     <doc kind="bug" >
//     </doc>
//
//     <doc kind="example" id="example1">
//       <synopsis>
//         Remove the optional property t9 from p_zone:
//       </synopsis>
//
//       <code>
// if
//   !Libnucnet__Zone__removeProperty(
//     p_zone,
//     "t9",
//     NULL,
//     NULL,
//   )
// )
//   fprintf( stderr, "Unsuccessful removal!\n" );
//       </code>
//     </doc>
//
//   </usage>
//
// </routine>
//############################################################################*/

int
Libnucnet__Zone__removeProperty(
  Libnucnet__Zone *,
  const char *,
  const char *,
  const char *
);

/*##############################################################################
// <routine name="Libnucnet__Zone__iterateOptionalProperties()">
//
//   <description>
//     <abstract>
//       Iterate over the optional properties in a zone.
//     </abstract>
//     <keywords>
//       Libnucnet, property, xml, iterate
//     </keywords>
//   </description>
//
//   <usage>
//
//     <calling_sequence>
// void
// Libnucnet__Zone__iterateOptionalProperties(
//   const Libnucnet__Zone *self,
//   const char *s_name,
//   const char *s_tag1,
//   const char *s_tag2,
//   Libnucnet__Zone__optional_property_iterate_function
//     pf_func,
//   void *p_user_data
// );
//     </calling_sequence>
//
//     <param
//       name="self" 
//       kind="in,positional,required" 
//     >
//       A pointer to a Libnucnet__Zone structure.
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
//       about the property or NULL.
//     </param>
//     <param
//       name="s_tag2" 
//       kind="in,positional,required" 
//     >
//       The name of a second tag for the property containing extra information
//       about the property.
//     </param>
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
//     <param
//       kind="return"
//       doc="result"
//     />
//
//     <doc kind="post" id="result">
//       Routine iterates over the optional properties in the zone and
//       applies the user-supplied function.  The properties iterated over
//       are those that match s_name, s_tag1, and s_tag2.  If any of these
//       is NULL, the comparison is a match.  The properties are iterated
//       in the order they are stored internally over which the user has no
//       control.
//     </doc>
//
//     <doc kind="bug" >
//     </doc>
//
//     <doc kind="example" id="example1">
//       <synopsis>
//         Iterate over the all optional properties in zone p_zone and
//         apply my_func and the extra data pointed to by p_user_data:
//       </synopsis>
//
//       <code>
// Libnucnet__Zone__iterateOptionalProperties(
//   p_zone,
//   NULL,
//   NULL,
//   NULL,
//   (Libnucnet__Zone__optional_property_iterate_function) my_func,
//   p_user_data
// );
//       </code>
//     </doc>
//
//     <doc kind="example" id="example2">
//       <synopsis>
//         Iterate over all the optional properties that match "t9"
//         as a name in zone p_zone and
//         apply my_func and the extra data pointed to by p_user_data:
//       </synopsis>
//
//       <code>
// Libnucnet__Zone__iterateOptionalProperties(
//   p_zone,
//   "t9",
//   NULL,
//   NULL,
//   (Libnucnet__Zone__optional_property_iterate_function) my_func,
//   p_user_data
// );
//       </code>
//     </doc>
//   </usage>
//
// </routine>
//############################################################################*/

void
Libnucnet__Zone__iterateOptionalProperties(
  const Libnucnet__Zone *,
  const char *,
  const char *,
  const char *,
  Libnucnet__Zone__optional_property_iterate_function,
  void *
);

/*##############################################################################
// <routine name="Libnucnet__relabelZone()">
//
//   <description>
//     <abstract>
//       Change the labels of a zone.
//     </abstract>
//     <keywords>
//       Libnucnet, property, xml, zone, relabel
//     </keywords>
//   </description>
//
//   <usage>
//
//     <calling_sequence>
// int
// Libnucnet__relabelZone(
//   Libnucnet *self,
//   Libnucnet__Zone *p_zone,
//   const char *s_new_label1,
//   const char *s_new_label2,
//   const char *s_new_label3
// );
//     </calling_sequence>
//
//     <param
//       name="self" 
//       kind="in,positional,required" 
//     >
//       A pointer to a Libnucnet structure.
//     </param>
//
//     <param
//       name="p_zone" 
//       kind="in,positional,required" 
//     >
//       A pointer to the Libnucnet__Zone whose labels are to be changed.
//     </param>
//
//     <param
//       name="s_new_label1" 
//       kind="in,positional,required" 
//     >
//       A string giving the new first label for the zone or NULL.
//     </param>
//     <param
//       name="s_new_label2" 
//       kind="in,positional,required" 
//     >
//       A string giving the new second label for the zone or NULL.
//     </param>
//     <param
//       name="s_new_label3" 
//       kind="in,positional,required" 
//     >
//       A string giving the new third label for the zone or NULL.
//     </param>
//     <param
//       kind="return"
//       doc="result"
//     />
//
//     <doc kind="post" id="result">
//       Routine returns 1 if the relabeling was successful and 0 if not.
//       On successful return, the zone has been relabeled.  On unsuccessful
//       return (for example, if a zone with the new labels already exists
//       in self), the labels have not changed.  If self or p_zone is invalid,
//       error handling is invoked.
//     </doc>
//
//     <doc kind="bug" >
//     </doc>
//
//     <doc kind="example" id="example1">
//       <synopsis>
//         Change the labels of a zone in p_my_nucnet from "x1", "y1", and
//         "z1" to "x2", "y2", and "z2":
//       </synopsis>
//
//       <code>
// p_zone =
//   Libnucnet__getZoneByLabels( p_my_nucnet, "x1", "y1", "z1" );
// if(
//   Libnucnet__relabelZone( p_my_nucnet, p_zone, "x2", "y2"," z2" )
// )
//   printf( "Relabeling sucessful!\n" );
//       </code>
//     </doc>
//
//   </usage>
//
// </routine>
//############################################################################*/

int
Libnucnet__relabelZone(
  Libnucnet *,
  Libnucnet__Zone *,
  const char *,
  const char *,
  const char *
);

/*##############################################################################
// <routine name="Libnucnet__setZoneCompareFunction()">
//
//   <description>
//     <abstract>
//       Set the function to be applied to sort the zones during a zone
//       iteration.
//     </abstract>
//     <keywords>
//       Libnucnet, zone, xml, sort
//     </keywords>
//   </description>
//
//   <usage>
//
//     <calling_sequence>
// void
// Libnucnet__setZoneCompareFunction(
//   Libnucnet *self,
//   Libnucnet__Zone__compare_function pfFunc
// );
//     </calling_sequence>
//
//     <param
//       name="self" 
//       kind="in,positional,required" 
//     >
//       A pointer to a Libnucnet structure.
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
//       Upon successful return, the data compare function for zones
//       in the collection has been set to the input function.
//       If any input is invalid, error handling is invoked.
//     </doc>
//
//     <doc kind="bug" >
//     </doc>
//
//     <doc kind="example" id="example1">
//       <synopsis>
//         Set the zone compare function in p_my_nucnet to my_sort_function:
//       </synopsis>
//
//       <code>
// Libnucnet__setZoneCompareFunction(
//   p_my_nucnet,
//   (Libnucnet__Zone__compare_function) my_sort_function
// );
//       </code>
//     </doc>
//
//   </usage>
//
// </routine>
//############################################################################*/

void
Libnucnet__setZoneCompareFunction(
  Libnucnet *,
  Libnucnet__Zone__compare_function
);

/*##############################################################################
// <routine name="Libnucnet__clearZoneCompareFunction()">
//
//   <description>
//     <abstract>
//       Restore the default function to be applied during a zone sort.
//     </abstract>
//     <keywords>
//       Libnucnet, zone, xml, sort
//     </keywords>
//   </description>
//
//   <usage>
//
//     <calling_sequence>
// void
// Libnucnet__clearZoneCompareFunction(
//   Libnucnet *self
// );
//     </calling_sequence>
//
//     <param
//       name="self" 
//       kind="in,positional,required" 
//     >
//       A pointer to a Libnucnet structure.
//     </param>
//
//     <param
//       kind="return"
//       doc="result"
//     />
//
//     <doc kind="post" id="result">
//       Upon successful return, the zone compare function for
//       the collection has been restored to the default function.
//       If any input is invalid, error handling is invoked.
//     </doc>
//
//     <doc kind="bug" >
//     </doc>
//
//     <doc kind="example" id="example1">
//       <synopsis>
//         Clear the zone compare function in p_my_nucnet:
//       </synopsis>
//
//       <code>
// Libnucnet__clearZoneCompareFunction(
//   p_my_nucnet
// );
//       </code>
//     </doc>
//
//   </usage>
//
// </routine>
//############################################################################*/

void
Libnucnet__clearZoneCompareFunction(
  Libnucnet *
);

/*##############################################################################
// <routine name="Libnucnet__Net__writeToXmlFile()">
//
//   <description>
//     <abstract>
//       Output a Libnucnet__Net structure to an xml file.
//     </abstract>
//     <keywords>
//       nuclear, data, write, file, network
//     </keywords>
//   </description>
//
//   <usage>
//
//     <calling_sequence>
// void
// Libnucnet__Net__writeToXmlFile(
//   const Libnucnet__Net *self,
//   const char *s_xml_filename
// );
//     </calling_sequence>
//
//     <param
//       name="self"
//       kind="in,positional,required"
//     >
//       A pointer to a Libnucnet nuclear network.
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
//       Upon successful return, the contents of the network
//       are written to s_xml_filename.
//     </doc>
//
//     <doc kind="bug" >
//     </doc>
//
//     <doc kind="example" id="example">
//       <synopsis>
//         Dump the contents of Libnucnet__Net *p_my_network to the
//         xml file my.xml:
//       </synopsis>
//
//       <code>
// Libnucnet__Net__writeToXmlFile( p_my_network, "my.xml" );
//       </code>
//     </doc>
//
//   </usage>
//
// </routine>
//############################################################################*/

void
Libnucnet__Net__writeToXmlFile(
  const Libnucnet__Net *,
  const char *
);

/*##############################################################################
// <routine name="Libnucnet__writeToXmlFile()">
//
//   <description>
//     <abstract>
//       Output a Libnucnet structure to an xml file.
//     </abstract>
//     <keywords>
//       nuclear, data, write, file, network
//     </keywords>
//   </description>
//
//   <usage>
//
//     <calling_sequence>
// void
// Libnucnet__writeToXmlFile(
//   const Libnucnet *self,
//   const char *s_xml_filename
// );
//     </calling_sequence>
//
//     <param
//       name="self"
//       kind="in,positional,required"
//     >
//       A pointer to a Libnucnet structure.
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
//       Upon successful return, the contents of the full libnucnet
//       structure are written to s_xml_filename.
//     </doc>
//
//     <doc kind="bug" >
//     </doc>
//
//     <doc kind="example" id="example">
//       <synopsis>
//         Dump the contents of Libnucnet *p_my_nucnet to the
//         xml file my.xml:
//       </synopsis>
//
//       <code>
// Libnucnet__writeToXmlFile( p_my_nucnet, "my.xml" );
//       </code>
//     </doc>
//
//   </usage>
//
// </routine>
//############################################################################*/

void
Libnucnet__writeToXmlFile(
  const Libnucnet *,
  const char *
);

/*##############################################################################
// <routine name="Libnucnet__Zone__copy()">
//
//   <description>
//     <abstract>
//       Copy a zone.
//     </abstract>
//     <keywords>
//       nuclear network, zone, hash, abundance, copy
//     </keywords>
//   </description>
//
//   <usage>
//
//     <calling_sequence>
// Libnucnet__Zone *
// Libnucnet__Zone__copy(
//   const Libnucnet__Zone *self
// );
//     </calling_sequence>
//
//     <param
//       name="self"
//       kind="in,positional,required"
//     >
//       A pointer to a zone.
//     </param>
//
//     <param
//       kind="return"
//       doc="result"
//     />
//
//     <doc kind="post" id="result">
//       Routine returns a pointer to the new zone.  If the zone could not be 
//       created, routine returns NULL.
//       If the input zone is invalid, Libnucnet error handling is invoked.
//     </doc>
//
//     <doc kind="bug" >
//     </doc>
//
//     <doc kind="example" id="example">
//       <synopsis>
//       Get a copy of the zone p_zone:
//       </synopsis>
//
//       <code>
// p_copy =
//   Libnucnet__Zone__copy( p_zone );
//       </code>
//     </doc>
//
//   </usage>
//
// </routine>
//############################################################################*/

Libnucnet__Zone *
Libnucnet__Zone__copy( const Libnucnet__Zone * );

/*##############################################################################
// <routine name="Libnucnet__Zone__updateDataForUserRateFunction()">
//
//   <description>
//     <abstract>
//       Update the data for a user rate function appropriate for the given
//       zone.
//     </abstract>
//     <keywords>
//       nuclear network, zone, hash, user, rate, function
//     </keywords>
//   </description>
//
//   <usage>
//
//     <calling_sequence>
// int
// Libnucnet__Zone__updateDataForUserRateFunction(
//   Libnucnet__Zone * self,
//   const char * s_function_key,
//   void * p_data
// );
//     </calling_sequence>
//
//     <param
//       name="self"
//       kind="in,positional,required"
//     >
//       A pointer to a zone.
//     </param>
//
//     <param
//       name="s_function_key"
//       kind="in,positional,required"
//     >
//       A string giving the key to the rate function.
//     </param>
//
//     <param
//       name="p_data"
//       kind="in,positional,required"
//     >
//       A pointer to the user-defined data structure holding the data
//       for the rate function.
//     </param>
//
//     <param
//       kind="return"
//       doc="result"
//     />
//
//     <doc kind="post" id="result">
//       Routine returns 1 (true) if the update succeeded and 0 (false)
//       if not. If the input zone is invalid,
//       Libnucnet error handling is invoked.
//     </doc>
//
//     <doc kind="bug" >
//     </doc>
//
//     <doc kind="example" id="example">
//       <synopsis>
//       Update the function with key "my_function" with data in structure
//       my_data for zone p_zone:
//       </synopsis>
//
//       <code>
// Libnucnet__Zone__updateDataForUserRateFunction(
//   p_zone,
//   "my_function",
//   &my_data
// );
//       </code>
//     </doc>
//
//   </usage>
//
// </routine>
//############################################################################*/

int
Libnucnet__Zone__updateDataForUserRateFunction(
  Libnucnet__Zone *,
  const char *,
  void *
);

/*##############################################################################
// <routine name="Libnucnet__Zone__getDataForUserRateFunction()">
//
//   <description>
//     <abstract>
//       Retrieve the data for a user rate function appropriate for the given
//       zone.
//     </abstract>
//     <keywords>
//       nuclear network, zone, hash, user, rate, function
//     </keywords>
//   </description>
//
//   <usage>
//
//     <calling_sequence>
// void *
// Libnucnet__Zone__updateDataForUserRateFunction(
//   Libnucnet__Zone * self,
//   const char * s_function_key
// );
//     </calling_sequence>
//
//     <param
//       name="self"
//       kind="in,positional,required"
//     >
//       A pointer to a zone.
//     </param>
//
//     <param
//       name="s_function_key"
//       kind="in,positional,required"
//     >
//       A string giving the key to the rate function.
//     </param>
//
//     <param
//       kind="return"
//       doc="result"
//     />
//
//     <doc kind="post" id="result">
//       Routine returns the pointer to the user-defined data structure
//       holding the data for the rate function. If the input zone is invalid,
//       Libnucnet error handling is invoked.
//     </doc>
//
//     <doc kind="bug" >
//     </doc>
//
//     <doc kind="example" id="example">
//       <synopsis>
//       Retrieve the data (originally stored in the structure my_data)
//       for the function with key "my_function" in p_zone:
//       </synopsis>
//
//       <code>
// p_data =
//   ( my_data * )
//   Libnucnet__Zone__getDataForUserRateFunction(
//     p_zone,
//     "my_function"
//   );
//       </code>
//     </doc>
//
//   </usage>
//
// </routine>
//############################################################################*/

void *
Libnucnet__Zone__getDataForUserRateFunction(
  Libnucnet__Zone *,
  const char * s_function_key
);

/*##############################################################################
// <routine name="Libnucnet__NetView__new()">
//
//   <description>
//     <abstract>
//       Create a new Libnucnet__NetView structure.
//     </abstract>
//     <keywords>
//       nuclear, network, view
//     </keywords>
//   </description>
//
//   <usage>
//
//     <calling_sequence>
// Libnucnet__NetView *
// Libnucnet__NetView__new(
//   Libnucnet__Net * p_net,
//   const char * s_nuc_xpath,
//   const char * s_reac_xpath
// );
//     </calling_sequence>
//
//     <param
//       name="self"
//       kind="in,positional,required"
//     >
//       A pointer to a Libnucnet__NetView structure.
//     </param>
//
//     <param
//       name="s_nuc_xpath"
//       kind="in,positional,required"
//     >
//       A string giving the XPath expression defining the nuclei in the view.
//     </param>
//
//     <param
//       name="s_reac_xpath"
//       kind="in,positional,required"
//     >
//       A string giving the XPath expression defining the reactions in the
//       view.
//     </param>
//
//     <param
//       kind="return"
//       doc="result"
//     />
//
//     <doc kind="post" id="result">
//       Routine returns a new network view.  It is the user's responsibility
//       to free the view once it is no longer needed.
//       If the input is not valid, Libnucnet error handling is invoked.
//     </doc>
//
//     <doc kind="bug" >
//     </doc>
//
//     <doc kind="example" id="example">
//       <synopsis>
//       Create a network view of the parent network p_net containing all
//       nuclei and all valid reactions among them:
//       </synopsis>
//
//       <code>
// p_view = Libnucnet__NetView( p_net, " ", " " );
//       </code>
//     </doc>
//
//   </usage>
//
// </routine>
//############################################################################*/

Libnucnet__NetView *
Libnucnet__NetView__new(
  Libnucnet__Net *,
  const char *,
  const char *
);

/*##############################################################################
// <routine name="Libnucnet__NetView__free()">
//
//   <description>
//     <abstract>
//       Free the memory for a Libnucnet__NetView structure.
//     </abstract>
//     <keywords>
//       nuclear, free, network, view
//     </keywords>
//   </description>
//
//   <usage>
//
//     <calling_sequence>
// void
// Libnucnet__NetView__free( Libnucnet__NetView *self );
//     </calling_sequence>
//
//     <param
//       name="self"
//       kind="in,positional,required"
//     >
//       A pointer to a Libnucnet__NetView structure.
//     </param>
//
//     <param
//       kind="return"
//       doc="result"
//     />
//
//     <doc kind="post" id="result">
//       On return, the memory allocated for the network view has been cleared. 
//       If the input pointer is not valid, Libnucnet error handling is invoked.
//     </doc>
//
//     <doc kind="bug" >
//     </doc>
//
//     <doc kind="example" id="example">
//       <synopsis>
//       Free the memory for the network view p_my_view:
//       </synopsis>
//
//       <code>
// Libnucnet__NetView__free( p_my_view );
//       </code>
//     </doc>
//
//   </usage>
//
// </routine>
//############################################################################*/

void
Libnucnet__NetView__free( Libnucnet__NetView * );

/*##############################################################################
// <routine name="Libnucnet__NetView__getNet()">
//
//   <description>
//     <abstract>
//       Retrieve the Libnucnet__Net in a  Libnucnet__NetView structure.
//     </abstract>
//     <keywords>
//       nuclear, network, view
//     </keywords>
//   </description>
//
//   <usage>
//
//     <calling_sequence>
// Libnucnet__Net *
// Libnucnet__NetView__getNet( Libnucnet__NetView *self );
//     </calling_sequence>
//
//     <param
//       name="self"
//       kind="in,positional,required"
//     >
//       A pointer to a Libnucnet__NetView structure.
//     </param>
//
//     <param
//       kind="return"
//       doc="result"
//     />
//
//     <doc kind="post" id="result">
//       Routine returns the pointer to the Libnucnet__Net structure in
//       a valid network view.
//       If the input pointer is not valid, Libnucnet error handling is invoked.
//     </doc>
//
//     <doc kind="bug" >
//     </doc>
//
//     <doc kind="example" id="example">
//       <synopsis>
//       Get the network in view p_my_view:
//       </synopsis>
//
//       <code>
// p_net = Libnucnet__NetView__getNet( p_my_view );
//       </code>
//     </doc>
//
//   </usage>
//
// </routine>
//############################################################################*/

Libnucnet__Net *
Libnucnet__NetView__getNet( Libnucnet__NetView * );

/*##############################################################################
// <routine name="Libnucnet__Zone__getNetView()">
//
//   <description>
//     <abstract>
//       Retrieve the Libnucnet__Net structure from a Libnucnet__Zone.
//     </abstract>
//     <keywords>
//       nuclear, network, retrieve, zone, view
//     </keywords>
//   </description>
//
//   <usage>
//
//     <calling_sequence>
// Libnucnet__NetView *
// Libnucnet__Zone__getNetView(
//   Libnucnet__Zone * self,
//   const char * s_nuc_xpath,
//   const char * s_reac_xpath
// );
//     </calling_sequence>
//
//     <param
//       name="self"
//       kind="in,positional,required"
//     >
//       A pointer to a Libnucnet__Zone structure.
//     </param>
//
//     <param
//       name="s_nuc_xpath"
//       kind="in,positional,required"
//     >
//       A string giving the XPath expression defining the nuclei in the view.
//     </param>
//
//     <param
//       name="s_reac_xpath"
//       kind="in,positional,required"
//     >
//       A string giving the XPath expression defining the reactions in the
//       view.
//     </param>
//
//     <param
//       kind="return"
//       doc="result"
//     />
//
//     <doc kind="post" id="result">
//       Routine returns the pointer to the Libnucnet__NetView structure.
//       If the input pointer is not valid, Libnucnet error handling is invoked.
//     </doc>
//
//     <doc kind="bug" >
//     </doc>
//
//     <doc kind="example" id="example">
//       <synopsis>
//       Retrieve the network view containing all nuclei but only (n,gamma)
//       reactions from Zone p_zone:
//       </synopsis>
//
//       <code>
// p_my_view =
//   Libnucnet__getNetView(
//     p_zone,
//     " ",
//     "[reactant = 'n' and product = 'gamma']"
//   );
//       </code>
//     </doc>
//
//   </usage>
//
// </routine>
//############################################################################*/

Libnucnet__NetView *
Libnucnet__Zone__getNetView(
  Libnucnet__Zone *,
  const char *,
  const char *
);

/*##############################################################################
// <routine name="Libnucnet__NetView__getNucXPath()">
//
//   <description>
//     <abstract>
//       Get the nuclear XPath expression defining a network view.
//     </abstract>
//     <keywords>
//       nuclear, view, collection, XPath
//     </keywords>
//   </description>
//
//   <usage>
//
//     <calling_sequence>
// const char *
// Libnucnet__NetView__getNucXPath( Libnucnet__NetView *self );
//     </calling_sequence>
//
//     <param
//       name="self"
//       kind="in,positional,required"
//     >
//       A pointer to a view of a Libnucnet__Net. 
//     </param>
//
//     <param
//       kind="return"
//       doc="result"
//     />
//
//     <doc kind="post" id="result">
//       Routine returns a string giving the nuclear XPath expression that
//       defined the view. If the input view is invalid, error handling is
//       invoked.
//     </doc>
//
//     <doc kind="bug" >
//     </doc>
//
//     <doc kind="example" id="example">
//       <synopsis>
//         Print the nuclear XPath expression defining a
//         Libnucnet__NetView *p_my_view:
//       </synopsis>
//
//       <code>
// printf(
//   "This is the view defined by nuclear XPath expression %s.\n",
//   Libnucnet__NetView__getNucXPath( p_my_view )
// );
//       </code>
//     </doc>
//
//   </usage>
//
// </routine>
//############################################################################*/

const char *
Libnucnet__NetView__getNucXPath( Libnucnet__NetView * );

/*##############################################################################
// <routine name="Libnucnet__NetView__getReacXPath()">
//
//   <description>
//     <abstract>
//       Get the reaction XPath expression defining a network view.
//     </abstract>
//     <keywords>
//       reaction, view, collection, XPath
//     </keywords>
//   </description>
//
//   <usage>
//
//     <calling_sequence>
// const char *
// Libnucnet__NetView__getReacXPath( Libnucnet__NetView *self );
//     </calling_sequence>
//
//     <param
//       name="self"
//       kind="in,positional,required"
//     >
//       A pointer to a view of a Libnucnet__Net. 
//     </param>
//
//     <param
//       kind="return"
//       doc="result"
//     />
//
//     <doc kind="post" id="result">
//       Routine returns a string giving the reaction XPath expression that
//       defined the view. If the input view is invalid, error handling is
//       invoked.
//     </doc>
//
//     <doc kind="bug" >
//     </doc>
//
//     <doc kind="example" id="example">
//       <synopsis>
//         Print the reaction XPath expression defining a
//         Libnucnet__NetView *p_my_view:
//       </synopsis>
//
//       <code>
// printf(
//   "This is the view defined by reaction XPath expression %s.\n",
//   Libnucnet__NetView__getReacXPath( p_my_view )
// );
//       </code>
//     </doc>
//
//   </usage>
//
// </routine>
//############################################################################*/

const char *
Libnucnet__NetView__getReacXPath( Libnucnet__NetView * );

/*##############################################################################
// <routine name="Libnucnet__updateZoneXmlMassFractionFormat()">
//
//   <description>
//     <abstract>
//       Update the format code for output of zone mass fractions to xml.
//     </abstract>
//     <keywords>
//       nuclear network, zone, mass fraction, format
//     </keywords>
//   </description>
//
//   <usage>
//
//     <calling_sequence>
// void
// Libnucnet__updateZoneXmlMassFractionFormat(
//   Libnucnet * self,
//   const char * s_format
// );
//     </calling_sequence>
//
//     <param
//       name="self"
//       kind="in,positional,required"
//     >
//       A pointer to a Libnucnet structure.
//     </param>
//
//     <param
//       name="s_format"
//       kind="in,positional,required"
//     >
//       A string giving the format code.
//     </param>
//
//     <param
//       kind="return"
//       doc="result"
//     />
//
//     <doc kind="post" id="result">
//       On successful return, the format code for the xml output of the
//       zone mass fractions has been updated.  If the input structure is
//       invalid, Libnucnet error handling is invoked.
//     </doc>
//
//     <doc kind="bug" >
//     </doc>
//
//     <doc kind="example" id="example">
//       <synopsis>
//       Update the format code for Libnucnet *p_my_nucnet for xml output of
//       zone mass fractions to %.15e:
//       </synopsis>
//
//       <code>
// Libnucnet__updateZoneXmlMassFractionFormat(
//   p_my_nucnet,
//   "%.15e"
// );
//       </code>
//     </doc>
//
//   </usage>
//
// </routine>
//############################################################################*/

void
Libnucnet__updateZoneXmlMassFractionFormat(
  Libnucnet *,
  const char *
);

/*##############################################################################
// <routine name="Libnucnet__Zone__addCopyOfNetView()">
//
//   <description>
//     <abstract>
//       Add a copy of a net view to a zone.
//     </abstract>
//     <keywords>
//       nuclear network, zone, view, add
//     </keywords>
//   </description>
//
//   <usage>
//
//     <calling_sequence>
// int
// Libnucnet__Zone__addCopyOfNetView(
//   Libnucnet__Zone * self,
//   Libnucnet__NetView * p_view
// );
//     </calling_sequence>
//
//     <param
//       name="self"
//       kind="in,positional,required"
//     >
//       A pointer to a Libnucnet__Zone structure.
//     </param>
//
//     <param
//       name="p_view"
//       kind="in,positional,required"
//     >
//       A pointer to a Libnucnet__NetView structure.
//     </param>
//
//     <param
//       kind="return"
//       doc="result"
//     />
//
//     <doc kind="post" id="result">
//       Routine returns 1 (true) if a copy of the view has been successfully
//       added to the zone.  If the input structure is invalid, the zone
//       already has the view, or if the view cannot be added,
//       Libnucnet error handling is invoked.
//     </doc>
//
//     <doc kind="bug" >
//     </doc>
//
//     <doc kind="example" id="example">
//       <synopsis>
//       Add a copy of Libnucnet__NetView * p_view to all zones in
//       Libnucnet *p_my_nucnet:
//       </synopsis>
//
//       <code>
// Libnucnet__iterateZones(
//   p_my_nucnet,
//   (Libnucnet__Zone__iterateFunction) Libnucnet__Zone__addCopyOfNetView,
//   p_view
// );
//       </code>
//     </doc>
//
//   </usage>
//
// </routine>
//############################################################################*/

int
Libnucnet__Zone__addCopyOfNetView(
  Libnucnet__Zone *,
  Libnucnet__NetView *
);

/*##############################################################################
// <routine name="Libnucnet__Zone__getSummedAbundances()">
//
//   <description>
//     <abstract>
//       Retrieve the abundances for a zone as a function of Z, N, or A.
//     </abstract>
//     <keywords>
//       nuclear network, zone, nucleon, number, abundance
//     </keywords>
//   </description>
//
//   <usage>
//
//     <calling_sequence>
// gsl_vector *
// Libnucnet__Zone__getSummedAbundances(
//   Libnucnet__Zone * self,
//   const char * s_nucleon_type
// );
//     </calling_sequence>
//
//     <param
//       name="self"
//       kind="in,positional,required"
//     >
//       A pointer to a Libnucnet__Zone structure.
//     </param>
//
//     <param
//       name="s_nucleon_type"
//       kind="in,positional,required"
//     >
//       A string determining whether the summing is by Z ("z"), N ("n"),
//       or A ("a").
//     </param>
//
//     <param
//       kind="return"
//       doc="result"
//     />
//
//     <doc kind="post" id="result">
//       Routine returns a new gsl_vector that gives the abundances in
//       the zone summed by Z, N, or A.  The index of a component of the
//       vector is the particular value of Z, N, or A while the value of the
//       component is the summed abundance over that index.
//       If the input zone or input string for the nucleon number type
//       is invalid, error handling is invoked.
//     </doc>
//
//     <doc kind="bug" >
//     </doc>
//
//     <doc kind="example" id="example">
//       <synopsis>
//       Get the abundances for Libnucnet__Zone * p_zone as a function of
//       atomic number:
//       </synopsis>
//
//       <code>
// p_z_vector =
//   Libnucnet__Zone__getSummedAbundances(
//     p_zone,
//     "z"
//   );
//       </code>
//     </doc>
//
//   </usage>
//
// </routine>
//############################################################################*/

gsl_vector *
Libnucnet__Zone__getSummedAbundances(
  Libnucnet__Zone *,
  const char *
);

/*##############################################################################
// <routine name="Libnucnet__Zone__computeJacobian()">
//
//   <description>
//     <abstract>
//       Creates the Jacobian for the zone.
//     </abstract>
//     <keywords>
//       nuclear, network, species, reactions, rate, Jacobian, matrix, zone
//     </keywords>
//   </description>
//
//   <usage>
//
//     <calling_sequence>
// WnMatrix *
// Libnucnet__Zone__computeJacobian(
//   Libnucnet__Zone *self
// );
//     </calling_sequence>
//
//     <param
//       name="self"
//       kind="in,positional,required"
//     >
//       A pointer to a Libnucnet__Zone structure.
//     </param>
//     <param
//       kind="return"
//       doc="result"
//     />
//
//     <doc kind="post" id="result">
//       If the input is valid, the routine returns a pointer to a
//       new WnMatrix structure containing the Jacobian.  It is
//       the caller's reponsibility to free the matrix with WnMatrix__free()
//       when no longer in use.
//     </doc>
//
//     <doc kind="bug" >
//     </doc>
//
//     <doc kind="example" id="example">
//       <synopsis>
//       Create the Jacobian for reaction rates and abundances in
//       Libnucnet__Zone *p_zone and store it in WnMatrix *p_matrix:
//       </synopsis>
//
//       <code>
// p_matrix = Libnucnet__Zone__computeJacobian( p_zone );
//       </code>
//     </doc>
//
//   </usage>
//
// </routine>
//############################################################################*/

WnMatrix * Libnucnet__Zone__computeJacobian( Libnucnet__Zone * );

/*##############################################################################
// <routine name="Libnucnet__Zone__updateEvolutionNetwork()">
//
//   <description>
//     <abstract>
//       Update the restriction on the evolution network for a zone.
//     </abstract>
//     <keywords>
//       nuclear network, zone, evolution
//     </keywords>
//   </description>
//
//   <usage>
//
//     <calling_sequence>
// void
// Libnucnet__Zone__updateEvolutionNetwork(
//   Libnucnet__Zone * self,
//   const char * s_nuc_xpath,
//   const char * s_reac_xpath
// );
//     </calling_sequence>
//
//     <param
//       name="self"
//       kind="in,positional,required"
//     >
//       A pointer to a Libnucnet__Zone structure.
//     </param>
//
//     <param
//       name="s_nuc_xpath"
//       kind="in,positional,required"
//     >
//       A string giving the XPath expression that restricts the nuclei
//       to be used in computing the rates, Jacobian, and flow vector
//       for a zone.
//     </param>
//
//     <param
//       name="s_reac_xpath"
//       kind="in,positional,required"
//     >
//       A string giving the XPath expression that restricts the reactions
//       to be used in computing the rates, Jacobian, and flow vector
//       for a zone.
//     </param>
//
//     <param
//       kind="return"
//       doc="result"
//     />
//
//     <doc kind="post" id="result">
//       On successful return, the network to be used for an evolution step
//       has been set.  If the input structure is invalid,
//       Libnucnet error handling is invoked.
//     </doc>
//
//     <doc kind="bug" >
//     </doc>
//
//     <doc kind="example" id="example">
//       <synopsis>
//       Restrict the network to be used for evolution on zone p_zone
//       to one with all nuclei in the parent network but only use (n,gamma)
//       reactions and their reverses.
//       </synopsis>
//
//       <code>
// Libnucnet__Zone__updateEvolutionNetwork(
//   p_zone,
//   NULL,
//   "[reactant = 'n' and product = 'gamma']"
// );
//       </code>
//     </doc>
//
//   </usage>
//
// </routine>
//############################################################################*/

void
Libnucnet__Zone__updateEvolutionNetwork(
  Libnucnet__Zone *,
  const char *,
  const char *
);

/*##############################################################################
// <routine name="Libnucnet__Zone__restoreDefaultEvolutionNetwork()">
//
//   <description>
//     <abstract>
//       Restore the evolution network to be the default, that is, the
//       one with all nuclei and reactions in the parent network.
//     </abstract>
//     <keywords>
//       nuclear network, zone, evolution, default
//     </keywords>
//   </description>
//
//   <usage>
//
//     <calling_sequence>
// void
// Libnucnet__Zone__restoreDefaultEvolutionNetwork(
//   Libnucnet__Zone * self
// );
//     </calling_sequence>
//
//     <param
//       name="self"
//       kind="in,positional,required"
//     >
//       A pointer to a Libnucnet__Zone structure.
//     </param>
//
//     <param
//       kind="return"
//       doc="result"
//     />
//
//     <doc kind="post" id="result">
//       On successful return, the network to be used for an evolution step
//       has been restored to the default.  If the input structure is invalid,
//       Libnucnet error handling is invoked.
//     </doc>
//
//     <doc kind="bug" >
//     </doc>
//
//     <doc kind="example" id="example">
//       <synopsis>
//       Restore the evolution network for p_zone to the default:
//       </synopsis>
//
//       <code>
// Libnucnet__Zone__restoreDefaultEvolutionNetwork(
//   p_zone
// );
//       </code>
//     </doc>
//
//   </usage>
//
// </routine>
//############################################################################*/

void
Libnucnet__Zone__restoreDefaultEvolutionNetwork(
  Libnucnet__Zone *
);

/*##############################################################################
// Non-API routines.
//############################################################################*/

void
Libnucnet__Net__checkReactionForBaryonConservation(
  const Libnucnet__Net *, const Libnucnet__Reaction *
);

int
Libnucnet__Net__checkReactionForBaryonConservationWalker(
  Libnucnet__Reaction__Element *, void *
);

void
Libnucnet__Net__checkReactionForChargeConservation(
  const Libnucnet__Net *, const Libnucnet__Reaction *
);

int
Libnucnet__Net__checkReactionForChargeConservationWalker(
  Libnucnet__Reaction__Element *, void *
);

void
Libnucnet__Net__checkReactionForLeptonConservation(
  const Libnucnet__Net *, const Libnucnet__Reaction *
);

int
Libnucnet__Net__checkReactionForLeptonConservationWalker(
  Libnucnet__Reaction__Element *, void *
);

double
Libnucnet__Net__computeReverseRate(
  Libnucnet__Net *, Libnucnet__Reaction *, double *, double, double
);

int
Libnucnet__Net__computeReverseRateWalker(
  Libnucnet__Reaction__Element *, void *
);

void Libnucnet__Zone__hashFree( Libnucnet__Zone *, xmlChar * );

void Libnucnet__initializeList( Libnucnet * );

void Libnucnet__initializeHash( Libnucnet * );

void
Libnucnet__assignZoneDataFromXmlDocument(
  Libnucnet *,
  xmlDocPtr,
  const char *
);

void
Libnucnet__Zone__assignAbundancesFromXml( 
  Libnucnet__Zone *,
  xmlNode *
);

void
Libnucnet__Zone__assignOptionalPropertiesFromXml( 
  Libnucnet__Zone *,
  xmlNode *
);

Libnucnet__Zone **
Libnucnet__createZoneArray( const Libnucnet *);

void
Libnucnet__createZoneArrayCallback(
  Libnucnet__Zone *,
  void *,
  const xmlChar *
);

void Libnucnet__Abundance__free( double *, xmlChar * );

void Libnucnet__AbundanceChange__free( double *, xmlChar * );

void Libnucnet__Rates__free( Libnucnet__Rates *, xmlChar * );

void Libnucnet__Zone__computeJacobianMatrixCallback(
  Libnucnet__Reaction *, void *, xmlChar *
);

void
Libnucnet__Zone__computeFlowVectorCallback(
  Libnucnet__Reaction *, void *, xmlChar *
);

int
Libnucnet__Zone__computeFlowVectorWalker(
  Libnucnet__Reaction__Element *, void *
);

int
Libnucnet__Zone__update_abundance_product(
  Libnucnet__Reaction__Element *, void *
);

void Libnucnet__Zone__getUserReactions(
  Libnucnet__Reaction *, xmlHashTablePtr, xmlChar *
);

void Libnucnet__Zone__computeRatesCallback(
  Libnucnet__Reaction *, void *, xmlChar *
);

void Libnucnet__Zone__applyScreeningCallback(
  Libnucnet__Reaction *, void *, xmlChar *
);

int
Libnucnet__Net__computeScreeningFactorForReactionWalker(
  Libnucnet__Reaction__Element *, void *
);

void Libnucnet__Zone__applyReverseRatioCorrectionCallback(
  Libnucnet__Reaction *, void *, xmlChar *
);

int
Libnucnet__Net__computeReverseRatioCorrectionFactorForReactionWalker(
  Libnucnet__Reaction__Element *, void *
);

int
Libnucnet__Net__computeReactionQValueWalker(
  Libnucnet__Reaction__Element *, void *
);

void Libnucnet__Net__setValidReactions( Libnucnet__Net * );

int
Libnucnet__Net__isValidReactionWalker(
  Libnucnet__Reaction__Element *, void *
);

void
Libnucnet__Net__setValidReactionsCallback(
  Libnucnet__Reaction *, Libnucnet__Net *, xmlChar *
);

void Libnucnet__Net__setUpdateReactions( Libnucnet__Net * );

void
Libnucnet__Net__setUpdateReactionsCallback(
  Libnucnet__Reaction *, Libnucnet__Net *, xmlChar *
);

void
Libnucnet__Zone__getAbundancesCallback(
  Libnucnet__Species *, void *, xmlChar *
);

void
Libnucnet__Zone__getAbundanceChangesCallback(
  Libnucnet__Species *, void *, xmlChar *
);

void
Libnucnet__Zone__updateAbundancesCallback(
  Libnucnet__Species *, void *, xmlChar *
);

void
Libnucnet__Zone__updateAbundanceChangesCallback(
  Libnucnet__Species *, void *, xmlChar *
);

void
Libnucnet__Zone__computeZMomentCallback(
  Libnucnet__Species *, void *, xmlChar *
);

void
Libnucnet__Zone__computeAMomentCallback(
  Libnucnet__Species *, void *, xmlChar *
);

void
Libnucnet__Zone__updateTimeStepCallback(
  Libnucnet__Species *, void *, xmlChar *
);

xmlDocPtr
Libnucnet__makeZoneDataXmlDocument( const Libnucnet * );

xmlDocPtr
Libnucnet__newZoneDataXmlDocument( void );

int
Libnucnet__Zone__addToZoneDataXmlIterator(
  Libnucnet__Zone *, void *
);

void
Libnucnet__Zone__addSpeciesMassFractionToXml(
  Libnucnet__Zone *, xmlNodePtr, Libnucnet__Species *
);

void
Libnucnet__Zone__add_property_to_xml(
  xmlChar *, xmlChar *, xmlChar *, xmlChar *, void *
);

void
Libnucnet__Zone__iterateOptionalPropertiesHelper(
  const xmlChar *,
  void *,
  const xmlChar *,
  const xmlChar *,
  const xmlChar *
);

int
Libnucnet__sort_helper( const void *, const void * );

void
Libnucnet__sortZoneArray( const Libnucnet *, Libnucnet__Zone ** );

xmlDocPtr
Libnucnet__Net__makeXmlDocument(
  const Libnucnet__Net *
);

xmlDocPtr
Libnucnet__makeXmlDocument(
  const Libnucnet *
);

void
Libnucnet__Zone__property_copier(
  const char *,
  const char *,
  const char *,
  const char *,
  Libnucnet__Zone *
);

void
Libnucnet__NetView__set_valid_reactions(
  Libnucnet__Reaction *,
  Libnucnet__Net *,
  xmlChar *
);

void
Libnucnet__NetView__hash_element_copier(
  void *,
  xmlHashTablePtr,
  xmlChar *
);

Libnucnet__Reaction *
Libnucnet__NetView__reaction_pointer_copier(
  Libnucnet__Reaction *
);

void
Libnucnet__Zone__user_rate_function_data_free(
  Libnucnet__Reac__FD *,
  Libnucnet__Zone *,
  xmlChar *
);

void
Libnucnet__Zone__view_copier(
  Libnucnet__NetView *,
  Libnucnet__Zone *,
  xmlChar *
);

void
Libnucnet__Zone__getSummedAbundancesHelper(
  Libnucnet__Species *,
  void *,
  xmlChar *
);


#endif
