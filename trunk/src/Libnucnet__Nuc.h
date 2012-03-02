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
//        Source code for the Libnucnet part of the libnucnet module.
//        For documentation, see the header file Libnucnet.h.
//     </abstract>
//   </description>
// </file>
//////////////////////////////////////////////////////////////////////////////*/

#ifndef LIBNUCNET_NUC_H
#define LIBNUCNET_NUC_H
 
/*##############################################################################
// Standard library #include's.
//############################################################################*/

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <math.h>
#include <string.h>

/*##############################################################################
// Libxml #includes.
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

#include <gsl/gsl_const_cgsm.h>
#include <gsl/gsl_const_num.h>
#include <gsl/gsl_errno.h>
#include <gsl/gsl_math.h>
#include <gsl/gsl_roots.h>
#include <gsl/gsl_sf_exp.h>
#include <gsl/gsl_sort.h>
#include <gsl/gsl_sort_vector.h>
#include <gsl/gsl_spline.h>
#include <gsl/gsl_vector.h>

/*##############################################################################
// Other #includes
//############################################################################*/

#include <WnMatrix.h>

/*##############################################################################
// State definitions.
//############################################################################*/

#define SINGLE_STATE 0
#define GROUND_STATE 1
#define EXCITED_STATE 2

/*##############################################################################
// String definitions.
//############################################################################*/

#define XML_VERSION "1.0"
#define NUC_XSD_VERSION "2011-05-28"

#define ATOMIC_NUMBER "z"
#define COLON ":"
#define DOUBLE_SLASH "//"
#define ID "id"
#define MASS_EXCESS "mass_excess"
#define MASS_NUMBER "a"
#define NEUT "n"
#define NEUTRON_NUMBER "n"
#define NUC "nuc"
#define NUC_DEC "xmlns:" NUC
#define NUC_EMPTY_STRING ""
#define NUCLEAR_DATA "nuclear_data"
#define NUCLIDE "nuclide"
#define PARTF ".//log10_partf"
#define PARTF_POINT "point"
#define PARTF_TABLE "partf_table"
#define PARTF_T9 "t9"
#define PARTF_ENTRY "log10_partf"
#define PROT "h1"
#define SINGLE_SLASH "/"
#define SOURCE "source"
#define SPECIES_NAME "name"
#define SPIN "spin"
#define STATE "state"
#define STATES "states"
#define XPATH_T9 ".//t9"
#define XPATH_NUCLEAR_DATA NUCLEAR_DATA "/" NUCLIDE
#define XPATH_STATE ".//" STATE

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

#define NUC__PREFIX \
  "http://libnucnet.sourceforge.net/xsd_pub/" NUC_XSD_VERSION "/"

#define NUC__SCHEMA "libnucnet__nuc.xsd"
#define LIBNUCNET__NUC__SCHEMA NUC__PREFIX NUC__SCHEMA
#define NUC__NAMESPACE "libnucnet__nuc/"
#define LIBNUCNET__NUC__NAMESPACE NUC__PREFIX NUC__NAMESPACE
#define LIBNUCNET__NUC__SCHEMALOCATION1 LIBNUCNET__NUC__NAMESPACE " "
#define LIBNUCNET__NUC__SCHEMALOCATION \
  LIBNUCNET__NUC__SCHEMALOCATION1 LIBNUCNET__NUC__SCHEMA

/*##############################################################################
// Number definitions.
//############################################################################*/

#define WN_MEV_TO_ERGS GSL_CONST_NUM_MEGA * GSL_CONST_CGSM_ELECTRON_VOLT
#define WN_ERGS_TO_MEV GSL_CONST_NUM_MICRO / GSL_CONST_CGSM_ELECTRON_VOLT
#define WN_AMU_TO_MEV \
  GSL_CONST_CGSM_UNIFIED_ATOMIC_MASS * \
  GSL_CONST_CGSM_SPEED_OF_LIGHT * GSL_CONST_CGSM_SPEED_OF_LIGHT * \
  WN_ERGS_TO_MEV
#define WN_MASS_ELECTRON_MEV \
  GSL_CONST_CGSM_MASS_ELECTRON * \
  GSL_CONST_CGSM_SPEED_OF_LIGHT * GSL_CONST_CGSM_SPEED_OF_LIGHT * \
  WN_ERGS_TO_MEV

/*##############################################################################
// Internal definitions.
//############################################################################*/

#define NUC_BUF_SIZE 256

/*##############################################################################
// Macro definitions.
//############################################################################*/

/* To debug, uncomment following line. */
#define debug_nuc

#ifndef debug_nuc

#define LIBNUCNET__NUC__ERROR( s_reason ) \
       do { \
         fprintf( stderr, "ERROR: %s in %s on line %d\n", \
           s_reason, __FILE__, __LINE__ \
         ); \
         exit( EXIT_FAILURE ); \
       } while (0)

#else

#define LIBNUCNET__NUC__ERROR( s_reason ) \
       do { \
         fprintf( stderr, "ERROR: %s in %s on line %d\n", \
           s_reason, __FILE__, __LINE__ \
         ); \
         abort(); \
       } while (0)

#endif

/*##############################################################################
// Typedefs.
//############################################################################*/

/*##############################################################################
// <class name="Libnucnet__Species">
//
//   <description>
//     <abstract>
//       Libnucnet__Species is a structure that stores nuclear data
//       for a particular nuclear species.  Routines act
//       on the structure to retrieve data,
//       update data, or use data to compute functions of the nuclear data,
//       such as the nuclear partition function.
//       The contents of the structure are not made public by the API.
//     </abstract>
//     <keywords>
//       species, nuclide, xml, gdome
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

typedef struct Libnucnet__Species {
  unsigned int iZ;
  unsigned int iA;
  size_t iIndex;
  xmlChar *sxName;
  xmlChar *sxBaseName;
  xmlChar *sxState;
  xmlChar *sxSource;
  double dMassExcess;
  double dSpin; 
  gsl_vector *pT9;
  gsl_vector *pLog10Partf;
} Libnucnet__Species;

/*##############################################################################
// <user_routine name="Libnucnet__Species__compare_function()">
//
//   <description>
//     <abstract>
//       User-supplied routine to be applied during a sort of
//       the species in the species collection.
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
// Libnucnet__Species__compare_function(
//   const Libnucnet__Species *p_species1,
//   const Libnucnet__Species *p_species2
// );
//     </calling_sequence>
//
//     <param
//       name="p_species1"
//       kind="in,positional,required"
//     >
//       A pointer to the first species.
//     </param>
//
//     <param
//       name="p_species2"
//       kind="in,positional,required"
//     >
//       A pointer to the second species.
//     </param>
//
//     <param
//       kind="return"
//       doc="result"
//     />
//
//     <doc kind="post" id="result">
//       User's routine must return -1 if species 1 is less than species 2,
//       0 if the two species are equal, and 1 if species 1 is greater than
//       species 2.  If this routine is not supplied through
//       Libnucnet__Nuc__setSpeciesCompareFunction, the default function, which
//       sorts in increasing Z and A, is used.
//     </doc>
//
//     <doc kind="bug" >
//     </doc>
//
//   </usage>
//
// </user_routine>
//############################################################################*/

typedef int ( *Libnucnet__Species__compare_function ) (
  const Libnucnet__Species *,
  const Libnucnet__Species *
);

/*##############################################################################
// <user_routine name="Libnucnet__Species__iterateFunction()">
//
//   <description>
//     <abstract>
//       User-supplied routine to be applied during an iteration over
//       the species in the species collection.
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
// Libnucnet__Species__iterateFunction(
//   Libnucnet__Species *self,
//   void *p_user_data
// );
//     </calling_sequence>
//
//     <param
//       name="self"
//       kind="in,positional,required"
//     >
//       A pointer to a Libnucnet__Species.
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

typedef int ( *Libnucnet__Species__iterateFunction ) (
  Libnucnet__Species *, void *
);

/*##############################################################################
// <user_routine name="Libnucnet__Species__nseCorrectionFactorFunction()">
//
//   <description>
//     <abstract>
//       Optional user-supplied routine to compute corrections to the NSE
//       factor for a species.
//     </abstract>
//     <keywords>
//       Libnucnet, NSE, correction, user, supplied, function
//     </keywords>
//   </description>
//
//   <usage>
//
//     <calling_sequence>
// double
// Libnucnet__Species__nseCorrectionFactorFunction(
//   Libnucnet__Species *self,
//   double d_t9,
//   double d_rho,
//   double d_ye,
//   void *p_user_data
// );
//     </calling_sequence>
//
//     <param
//       name="self"
//       kind="in,positional,required"
//     >
//       A pointer to the Libnucnet__Species whose NSE
//       correction factor is required.
//     </param>
//
//     <param
//       name="d_t9"
//       kind="in,positional,required"
//     >
//       A double giving the temperature in billions of K at which to
//       compute the NSE correction factor.
//     </param>
//
//     <param
//       name="d_rho"
//       kind="in,positional,required"
//     >
//       A double giving the density in g/cc at which to compute the
//       NSE correction factor.
//     </param>
//
//     <param
//       name="d_ye"
//       kind="in,positional,required"
//     >
//       A double giving the electron-to-baryon ratio at which to compute the
//       NSE correction factor.
//     </param>
//
//     <param
//       name="p_user_data"
//       kind="in,positional,required"
//     >
//       A pointer to a user-defined structure containing extra data for
//       the NSE correction factor function.
//     </param>
//
//     <param
//       kind="return"
//       doc="result"
//     />
//
//     <doc kind="post" id="result">
//       User's routine must return a double giving the NSE
//       correction factor for the input temperature, density, and
//       electron-to-baryon ratio.
//     </doc>
//
//     <doc kind="bug" >
//     </doc>
//
//   </usage>
//
// </user_routine>
//############################################################################*/

typedef double ( *Libnucnet__Species__nseCorrectionFactorFunction ) (
  Libnucnet__Species *, double, double, double, void *
);

/*##############################################################################
// <class name="Libnucnet__Nuc">
//
//   <description>
//     <abstract>
//       Libnucnet__Nuc is a structure that stores the collection of nuclear
//       species.  Routines act on the structure to
//       retrieve species or add or remove them.
//       The contents of the structure are not made public by the API.
//     </abstract>
//     <keywords>
//       isotope chain, nuclear, data, nuclide, xml, gdome
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

typedef struct Libnucnet__Nuc {
  xmlHashTablePtr pSpeciesHash;
  Libnucnet__Species__compare_function pfSpeciesCompare;
  size_t iUpdate;
  int iOwner;
} Libnucnet__Nuc;

/*##############################################################################
// <class name="Libnucnet__NucView">
//
//   <description>
//     <abstract>
//       Libnucnet__NucView is a structure that stores a view of
//       Libnucnet__Nuc structure.  A view is a subset of the parent
//       Libnucnet__Nuc with the nuclei chosen by an XPath expression.
//       The view structure contains a Libnucnet__Nuc pointer, which
//       may be passed into all API routines that take such structures as
//       input.  It is important to note that the Libnucnet__Nuc member of
//       a view does not own the nuclide, so modifying the data in the
//       view modifies the data in the parent structure.
//     </abstract>
//     <keywords>
//       nuclear, network, rates, xml, nuclei, view
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
  Libnucnet__Nuc *pNuc;
  xmlChar *sxXPath;
} Libnucnet__NucView;

/*##############################################################################
// API routines.
//############################################################################*/
  
/*##############################################################################
// <routine name="Libnucnet__Nuc__new()">
//
//   <description>
//     <abstract>
//       Create a new Libnucnet__Nuc structure.
//     </abstract>
//     <keywords>
//       Libnucnet, nuclear, data, xml, hash
//     </keywords>
//   </description>
//
//   <usage>
//
//     <calling_sequence>
// Libnucnet__Nuc *Libnucnet__Nuc__new( );
//     </calling_sequence>
//
//     <param
//       kind="return"
//       doc="result"
//     />
//
//     <doc kind="post" id="result">
//       The routine returns a pointer to a new Libnucnet__Nuc structure,
//       that is, a collection of nuclear species.
//       If it is not possible to allocate memory for the new structure,
//       Libnucnet error handling is invoked.
//     </doc>
//
//     <doc kind="bug" >
//     </doc>
//
//     <doc kind="example" id="example">
//       <synopsis>
//         Create a Libnucnet__Nuc structure p_my_nuclei:
//       </synopsis>
//
//       <code>
// p_my_nuclei = Libnucnet__Nuc__new( );
//       </code>
//     </doc>
//
//   </usage>
//
// </routine>
//############################################################################*/

Libnucnet__Nuc *Libnucnet__Nuc__new( void );

/*##############################################################################
// <routine name="Libnucnet__Nuc__new_from_xml()">
//
//   <description>
//     <abstract>
//       Creates a Libnucnet__Nuc structure from an input xml nuclear data file.
//     </abstract>
//     <keywords>
//       Libnucnet, nuclear, xml, hash
//     </keywords>
//   </description>
//
//   <usage>
//
//     <calling_sequence>
// Libnucnet__Nuc *
// Libnucnet__Nuc__new_from_xml(
//   const char *s_xml_filename,
//   const char *s_xpath
// );
//     </calling_sequence>
//
//     <param
//       name="s_xml_filename" 
//       kind="in,positional,required" 
//     >
//       A string giving the name of the xml file containing the nuclear data.
//       This may be the name of a local file or a URL.
//     </param>
//     <param
//       name="s_xpath" 
//       kind="in,positional,required" 
//     >
//       A string giving an xpath expression to apply.
//     </param>
//     <param
//       kind="return"
//       doc="result"
//     />
//
//     <doc kind="post" id="result">
//       For a valid input nuclear data xml file, the routine returns a pointer
//       to a new Libnucnet__Nuc structure.  If the routine cannot allocate
//       enough memory or if the xpath expression is invalid,
//       Libnucnet__Nuc error handling is invoked.
//     </doc>
//
//     <doc kind="bug" >
//     </doc>
//
//     <doc kind="example" id="example1">
//       <synopsis>
//         Store the nuclear data in "nuclear_data.xml" to p_my_nuclei:
//       </synopsis>
//
//       <code>
// p_my_nuclei =
//   Libnucnet__Nuc__new_from_xml( "nuclear_data.xml", NULL );
//       </code>
//     </doc>
//
//     <doc kind="example" id="example2">
//       <synopsis>
//         Store the nuclear data for neutrons and protons and nuclei with
//         z >= 6 in "nuclear_data.xml" to p_my_nuclei:
//       </synopsis>
//
//       <code>
// p_my_nuclei =
//   Libnucnet__Nuc__new_from_xml(
//     "nuclear_data.xml", "[ a = 1 or z >= 6 ]"
//   );
//       </code>
//     </doc>
//
//   </usage>
//
// </routine>
//############################################################################*/

Libnucnet__Nuc *Libnucnet__Nuc__new_from_xml( const char *, const char * );

/*##############################################################################
// <routine name="Libnucnet__Nuc__updateFromXml()">
//
//   <description>
//     <abstract>
//       Updates the data in a Libnucnet__Nuc structure from an input xml
//       nuclear data file.
//     </abstract>
//     <keywords>
//       Libnucnet, nuclear, xml, hash, update
//     </keywords>
//   </description>
//
//   <usage>
//
//     <calling_sequence>
// void
// Libnucnet__Nuc__updateFromXml(
//   Libnucnet__Nuc *self,
//   const char *s_xml_filename,
//   const char *s_xpath
// );
//     </calling_sequence>
//
//     <param
//       name="self" 
//       kind="in,positional,required" 
//     >
//       A pointer to a pre-existing Libnucnet__Nuc structure.
//     </param>
//     <param
//       name="s_xml_filename" 
//       kind="in,positional,required" 
//     >
//       A string giving the name of the xml file containing the nuclear data.
//       This may be the name of a local file or a URL.
//     </param>
//     <param
//       name="s_xpath" 
//       kind="in,positional,required" 
//     >
//       A string giving an xpath expression to apply to the data in
//       s_xml_filename.
//     </param>
//     <param
//       kind="return"
//       doc="result"
//     />
//
//     <doc kind="post" id="result">
//       For a valid Libnucnet__Nuc structure and a valid input nuclear data
//       xml file, the routine updates the structure with the data in the
//       input data file. If the routine cannot allocate
//       enough memory or if the xpath expression is invalid,
//       Libnucnet__Nuc error handling is invoked.
//     </doc>
//
//     <doc kind="bug" >
//     </doc>
//
//     <doc kind="example" id="example1">
//       <synopsis>
//         Update the data in p_my_nuclei with data in "new_nuclear_data.xml":
//       </synopsis>
//
//       <code>
// Libnucnet__Nuc__updateFromXml(
//   p_my_nuclei, "new_nuclear_data.xml", NULL
// );
//       </code>
//     </doc>
//
//   </usage>
//
// </routine>
//############################################################################*/

void Libnucnet__Nuc__updateFromXml(
  Libnucnet__Nuc *, const char *, const char *
);

/*##############################################################################
// <routine name="Libnucnet__Species__new()">
//
//   <description>
//     <abstract>
//       Create a new species from the input data.
//     </abstract>
//     <keywords>
//       Libnucnet, nuclear, data , species
//     </keywords>
//   </description>
//
//   <usage>
//
//     <calling_sequence>
// Libnucnet__Species *
// Libnucnet__Species__new(
//     unsigned int i_z,
//     unsigned int i_a,
//     const char *s_source,
//     int i_state_flag,
//     const char *s_state,
//     double d_mass_excess,
//     double d_spin,
//     gsl_vector *p_t9,
//     gsl_vector *p_log10_partf
//   );
//     </calling_sequence>
//
//     <param
//       name="i_z" 
//       kind="in,positional,required" 
//     >
//       An unsigned int giving the atomic number of the species.
//     </param>
//     <param
//       name="i_a" 
//       kind="in,positional,required" 
//     >
//       An unsigned int giving the atomic mass of the species.
//     </param>
//     <param
//       name="s_source" 
//       kind="in,positional,required" 
//     >
//       A string giving a message containing information about the source
//       of the data or NULL.
//     </param>
//     <param
//       name="i_state_flag" 
//       kind="in,positional,required" 
//     >
//       An int indicating whether the nuclear species has only one state
//       included in the collection (0) or a ground state and at least one
//       excited state included in the collection (1).
//     </param>
//     <param
//       name="s_state" 
//       kind="in,positional,required" 
//     >
//       A string giving the state id of the species.  This
//       string should be empty (or NULL) for no state suffix.
//     </param>
//     <param
//       name="d_spin" 
//       kind="in,positional,required" 
//     >
//       A double giving the spin of the species.
//     </param>
//     <param
//       name="d_mass_excess" 
//       kind="in,positional,required" 
//     >
//       A double giving the mass excess (in MeV) of the species.
//     </param>
//     <param
//       name="p_t9" 
//       kind="in,positional,required" 
//     >
//       A gsl_vector giving the temperatures in 10^9 K at which the
//       partition function of the species is evaluated.
//     </param>
//     <param
//       name="p_log10_partf" 
//       kind="in,positional,required" 
//     >
//       A gsl_vector giving the log10 of the partition function
//       factor of the species.  The partition function
//       is 10 raised to this factor times (2*spin + 1) of the state.
//     </param>
//     <param
//       kind="return"
//       doc="result"
//     />
//
//     <doc kind="post" id="result">
//       For valid input, the routine returns a pointer to a new
//       Libnucnet__Species structure.  If the species cannot be
//       created, error handling is invoked.
//     </doc>
//
//     <doc kind="bug" >
//     </doc>
//
//     <doc kind="example" id="example">
//       <synopsis>
//         Create li7 (Z = 3, A = 7, spin = 3/2, mass
//         excess = 14.908 MeV), and partition function data stored
//         in gsl_vectors p_t9 and p_log10_partf:
//       </synopsis>
//
//       <code>
// p_li7 =
//   Libnucnet__Species__new(
//     3,
//     7,
//     "example",
//     0,
//     "",
//     14.908,
//     1.5,
//     p_t9,
//     p_log10_partf
//   );
//       </code>
//     </doc>
//
//     <doc kind="example" id="example2">
//       <synopsis>
//         Create ground state (g) of al26 (Z = 13, A = 26, spin = 5, mass
//         excess = -12.210 MeV), and partition function data stored in
//         p_t9_g and p_log10_partf_g and metastable state (m) of
//         al26 (Z = 13, A = 26, spin = 0, mass excess = -11.982), and
//         partition function data stored in p_t9_m and p_log10_partf_m:
//       </synopsis>
//
//       <code>
// p_al26g =
//   Libnucnet__Species__new(
//     13,
//     26,
//     "example data",
//     1,
//     "g",
//     -12.210,
//     5.,
//     p_t9_g,
//     p_log10_partf_g
//   );
//
// p_al26m =
//   Libnucnet__Species__new(
//     13,
//     26,
//     "example data",
//     1,
//     "m",
//     -11.982,
//     0.,
//     p_t9_m,
//     p_log10_partf_m
//   );
//       </code>
//     </doc>
//
//   </usage>
//
// </routine>
//############################################################################*/

Libnucnet__Species *
Libnucnet__Species__new(
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

/*##############################################################################
// <routine name="Libnucnet__Nuc__addSpecies()">
//
//   <description>
//     <abstract>
//       Add a species to a species collection.
//     </abstract>
//     <keywords>
//       Libnucnet, nuclear, data , species
//     </keywords>
//   </description>
//
//   <usage>
//
//     <calling_sequence>
// int
// Libnucnet__Nuc__addSpecies(
//   Libnucnet__Nuc *self,
//   Libnucnet__Species *p_species
// );
//     </calling_sequence>
//
//     <param
//       name="self" 
//       kind="in,positional,required" 
//     >
//       A pointer to the species collection.
//     </param>
//     <param
//       name="p_species" 
//       kind="in,positional,required" 
//     >
//       A pointer to the species to be added to the collection.
//     </param>
//     <param
//       kind="return"
//       doc="result"
//     />
//
//     <doc kind="post" id="result">
//       Routine returns 1 for successful addition and 0 for failure.
//       On successful return, the routine updates the Libnucnet__Nuc structure
//       to include the new species.
//     </doc>
//
//     <doc kind="example" id="example">
//       <synopsis>
//         Add p_species to the species collection p_my_nuclei:
//       </synopsis>
//
//       <code>
// if( Libnucnet__Nuc__addSpecies( p_my_nuclei, p_species ) )
//   printf( "Addition successful.\n" );
//       </code>
//     </doc>
//
//   </usage>
//
// </routine>
//############################################################################*/

int
Libnucnet__Nuc__addSpecies( Libnucnet__Nuc *, Libnucnet__Species * );

/*##############################################################################
// <routine name="Libnucnet__Nuc__updateSpecies()">
//
//   <description>
//     <abstract>
//       Update a species in a species collection.
//     </abstract>
//     <keywords>
//       Libnucnet, nuclear, data , species
//     </keywords>
//   </description>
//
//   <usage>
//
//     <calling_sequence>
// int
// Libnucnet__Nuc__updateSpecies(
//   Libnucnet__Nuc *self,
//   Libnucnet__Species *p_species
// );
//     </calling_sequence>
//
//     <param
//       name="self" 
//       kind="in,positional,required" 
//     >
//       A pointer to the species collection.
//     </param>
//     <param
//       name="p_species" 
//       kind="in,positional,required" 
//     >
//       A pointer to the species to be added to the collection.
//     </param>
//     <param
//       kind="return"
//       doc="result"
//     />
//
//     <doc kind="post" id="result">
//       Routine returns 1 for successful update and 0 for failure.
//       For valid input, the routine updates the Libnucnet__Nuc structure
//       with the new species.  If the species already exists, its
//       data are replaced with the new data.  If not, the species is added.
//     </doc>
//
//     <doc kind="example" id="example">
//       <synopsis>
//         Update p_species in the species collection p_my_nuclei:
//       </synopsis>
//
//       <code>
// if( Libnucnet__Nuc__updateSpecies( p_my_nuclei, p_species ) )
//   printf( "Species update successful\n." );
//       </code>
//     </doc>
//
//   </usage>
//
// </routine>
//############################################################################*/

int
Libnucnet__Nuc__updateSpecies( Libnucnet__Nuc *, Libnucnet__Species * );

/*##############################################################################
// <routine name="Libnucnet__Nuc__getSpeciesByName()">
//
//   <description>
//     <abstract>
//       Retrieves the specified species.
//     </abstract>
//     <keywords>
//       nuclear, data, species, name, hash
//     </keywords>
//   </description>
//
//   <usage>
//
//     <calling_sequence>
// Libnucnet__Species *
// Libnucnet__Nuc__getSpeciesByName( 
//   const Libnucnet__Nuc *self,
//   const char *s_nucname
// );
//     </calling_sequence>
//
//     <param
//       name="self"
//       kind="in,positional,required"
//     >
//       A pointer to a Libnucnet__Nuc structure.
//     </param>
//     <param
//       name="s_nucname"
//       kind="in,positional,required"
//     >
//       A string giving the name of the particular species to be retrieved.
//     </param>
//
//     <param
//       kind="return"
//       doc="result"
//     />
//
//     <doc kind="post" id="result">
//       Routine returns a pointer to a Libnucnet__Species structure.
//       If the species is not found, routine returns NULL.  If the
//       input structure is invalid, error handling is invoked.
//     </doc>
//
//     <doc kind="bug" >
//     </doc>
//
//     <doc kind="example" id="example">
//       <synopsis>
//       Retrieve he4 from nuclear data collection p_my_nuclei:
//       </synopsis>
//
//       <code>
// p_he4 =
//   Libnucnet__Nuc__getSpeciesByName( p_my_nuclei, "he4" );
//       </code>
//     </doc>
//
//   </usage>
//
// </routine>
//############################################################################*/

Libnucnet__Species *
Libnucnet__Nuc__getSpeciesByName(
  const Libnucnet__Nuc *, const char *
);

/*##############################################################################
// <routine name="Libnucnet__Nuc__getNumberOfSpecies()">
//
//   <description>
//     <abstract>
//       Retrieves the number of species in the collection of nuclear species.
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
// Libnucnet__Nuc__getNumberOfSpecies( const Libnucnet__Nuc *self );
//     </calling_sequence>
//
//     <param
//       name="self"
//       kind="in,positional,required"
//     >
//       A pointer to a Libnucnet__Nuc structure. 
//     </param>
//
//     <param
//       kind="return"
//       doc="result"
//     />
//
//     <doc kind="post" id="result">
//       Routine returns an integer representing the number of species in the
//       collection of nuclear species.  If the input is invalid, error
//       handling is invoked.
//     </doc>
//
//     <doc kind="bug" >
//     </doc>
//
//     <doc kind="example" id="example">
//       <synopsis>
//       Get number of species in nuclear data collection p_my_nuclei:
//       </synopsis>
//
//       <code>
// size_t i_species_count;
// i_species_count =
//   Libnucnet__Nuc__getNumberOfSpecies( p_my_nuclei );
//       </code>
//     </doc>
//
//   </usage>
//
// </routine>
//############################################################################*/

size_t Libnucnet__Nuc__getNumberOfSpecies( const Libnucnet__Nuc * );

/*##############################################################################
// <routine name="Libnucnet__Species__getZ()">
//
//   <description>
//     <abstract>
//       Retrieves the Z (that is, the atomic number) of a species in the
//       collection of nuclear species.
//     </abstract>
//     <keywords>
//       nuclear, data, species, Z, index
//     </keywords>
//   </description>
//
//   <usage>
//
//     <calling_sequence>
// unsigned int
// Libnucnet__Species__getZ( const Libnucnet__Species *self );
//     </calling_sequence>
//
//     <param
//       name="self"
//       kind="in,positional,required"
//     >
//       A pointer to a Libnucnet__Species structure.
//     </param>
//     <param
//       kind="return"
//       doc="result"
//     />
//
//     <doc kind="post" id="result">
//       Routine returns the Z of the input species.
//       If the species is invalid, Libnucnet__Nuc error handling is invoked.
//     </doc>
//
//     <doc kind="bug" >
//     </doc>
//
//     <doc kind="example" id="example">
//       <synopsis>
//       Print the Z of au197 in p_my_nuclei:
//       </synopsis>
//
//       <code>
// p_species =
//   Libnucnet__Nuc__getSpeciesByName( p_my_nuclei, "au197" );
// if( p_species )
//   printf(
//     "Z = %d\n",
//     Libnucnet__Species__getZ( p_species )
//   );
//       </code>
//     </doc>
//
//   </usage>
//
// </routine>
//############################################################################*/

unsigned int Libnucnet__Species__getZ( const Libnucnet__Species * );

/*##############################################################################
// <routine name="Libnucnet__Species__getA()">
//
//   <description>
//     <abstract>
//       Retrieves the A (that is, the mass number) of a species in the
//       collection of nuclear species.
//     </abstract>
//     <keywords>
//       nuclear, data, species, A, index
//     </keywords>
//   </description>
//
//   <usage>
//
//     <calling_sequence>
// unsigned int
// Libnucnet__Species__getA( const Libnucnet__Species *self );
//     </calling_sequence>
//
//     <param
//       name="self"
//       kind="in,positional,required"
//     >
//       A pointer to a Libnucnet__Species structure.
//     </param>
//     <param
//       kind="return"
//       doc="result"
//     />
//
//     <doc kind="post" id="result">
//       Routine returns the A of the input species.
//       If the species is invalid, Libnucnet__Nuc error handling is invoked.
//     </doc>
//
//     <doc kind="bug" >
//     </doc>
//
//     <doc kind="example" id="example">
//       <synopsis>
//       Print the A of au197 in p_my_nuclei (should of course be 197!):
//       </synopsis>
//
//       <code>
// p_species =
//   Libnucnet__Nuc__getSpeciesByName( p_my_nuclei, "au197" );
// if( p_species )
//   printf(
//     "A = %d\n",
//     Libnucnet__Species__getA( p_species )
//   );
//       </code>
//     </doc>
//
//   </usage>
//
// </routine>
//############################################################################*/

unsigned int Libnucnet__Species__getA( const Libnucnet__Species * );

/*##############################################################################
// <routine name="Libnucnet__Species__getMassExcess()">
//
//   <description>
//     <abstract>
//       Retrieves the mass excess of a species in the nuclear species
//       collection.
//     </abstract>
//     <keywords>
//       nuclear, data, species, mass excess
//     </keywords>
//   </description>
//
//   <usage>
//
//     <calling_sequence>
// double
// Libnucnet__Species__getMassExcess( const Libnucnet__Species *self );
//     </calling_sequence>
//
//     <param
//       name="self"
//       kind="in,positional,required"
//     >
//       A pointer to a Libnucnet__Species structure.
//     </param>
//
//     <param
//       kind="return"
//       doc="result"
//     />
//
//     <doc kind="post" id="result">
//       Routine returns a double giving the mass excess of the requested
//       species.  If the input species is invalid, error
//       handling is invoked.
//     </doc>
//
//     <doc kind="bug" >
//     </doc>
//
//     <doc kind="example" id="example">
//       <synopsis>
//       Print the mass excess of sn120 in the structure Libnucnet__Nuc
//       *p_my_nuclei:
//       </synopsis>
//
//
//       <code>
// p_species =
//   Libnucnet__Nuc__getSpeciesByName( p_my_nuclei, "sn120" );
// if( p_species ) {
//   printf(
//     "Mass excess (MeV) = %lf\n,
//     Libnucnet__Species__getMassExcess( p_species )
//   );
// }
//       </code>
//     </doc>
//
//   </usage>
//
// </routine>
//############################################################################*/

double
Libnucnet__Species__getMassExcess( const Libnucnet__Species * );

/*##############################################################################
// <routine name="Libnucnet__Species__getSpin()">
//
//   <description>
//     <abstract>
//       Retrieves the spin of a species in the collection of nuclear species.
//     </abstract>
//     <keywords>
//       nuclear, data, species, spin, index
//     </keywords>
//   </description>
//
//   <usage>
//
//     <calling_sequence>
// double
// Libnucnet__Species__getSpin( const Libnucnet__Species *self );
//     </calling_sequence>
//
//     <param
//       name="self"
//       kind="in,positional,required"
//     >
//       A pointer to a Libnucnet__Species structure.
//     </param>
//
//     <param
//       kind="return"
//       doc="result"
//     />
//
//     <doc kind="post" id="result">
//       Routine returns a double giving the spin of the
//       requested species.  If the species is invalid, Libnucnet__Nuc error
//       handling is invoked.
//     </doc>
//
//     <doc kind="bug" >
//     </doc>
//
//     <doc kind="example" id="example">
//       <synopsis>
//       Print the spin of bi209 in the Libnucnet__Nuc structure
//       *p_my_nuclei:
//       </synopsis>
//
//
//       <code>
// if(
//     ( p_species =
//         Libnucnet__Nuc__getSpeciesByName( p_my_nuclei, "bi209" )
//     )
//   )
// {
//    printf(
//      "Spin = "%lf\n",
//      Libnucnet__Species__getSpin( p_species )
//    );
// }
//       </code>
//     </doc>
//
//   </usage>
//
// </routine>
//############################################################################*/

double
Libnucnet__Species__getSpin( const Libnucnet__Species * );

/*##############################################################################
// <routine name="Libnucnet__Species__getName()">
//
//   <description>
//     <abstract>
//       Retrieves the name of a species in the collection of nuclear species.
//     </abstract>
//     <keywords>
//       nuclear, data, species, name
//     </keywords>
//   </description>
//
//   <usage>
//
//     <calling_sequence>
// const char *
// Libnucnet__Species__getName( const Libnucnet__Species *self );
//     </calling_sequence>
//
//     <param
//       name="self"
//       kind="in,positional,required"
//     >
//       A pointer to a Libnucnet species structure.
//     </param>
//
//     <param
//       kind="return"
//       doc="result"
//     />
//
//     <doc kind="post" id="result">
//       Routine returns a string representing the name of the
//       requested species.  If the species is not valid, Libnucnet__Nuc
//       error handling is invoked.
//     </doc>
//
//     <doc kind="bug" >
//     </doc>
//
//     <doc kind="example" id="example">
//       <synopsis>
//       Print the name of the Libnucnet__Species *p_species:
//       </synopsis>
//
//       <code>
// printf(
//   "Name of species = %s\n",
//   Libnucnet__Species__getName( p_species )
// );
//       </code>
//     </doc>
//
//   </usage>
//
// </routine>
//############################################################################*/

const char *
Libnucnet__Species__getName( const Libnucnet__Species * );

/*##############################################################################
// <routine name="Libnucnet__Species__getSource()">
//
//   <description>
//     <abstract>
//       Retrieves the message about the source of data for a species in the
//       collection of nuclear species.
//     </abstract>
//     <keywords>
//       nuclear, data, species, source
//     </keywords>
//   </description>
//
//   <usage>
//
//     <calling_sequence>
// const char *
// Libnucnet__Species__getSource( const Libnucnet__Species *self );
//     </calling_sequence>
//
//     <param
//       name="self"
//       kind="in,positional,required"
//     >
//       A pointer to a Libnucnet species structure.
//     </param>
//
//     <param
//       kind="return"
//       doc="result"
//     />
//
//     <doc kind="post" id="result">
//       Routine returns a string containing information about the data for the
//       requested species.  If there are no data, an empty string is returned.
//       If the species is not valid, Libnucnet__Nuc
//       error handling is invoked.
//     </doc>
//
//     <doc kind="bug" >
//     </doc>
//
//     <doc kind="example" id="example">
//       <synopsis>
//       Print the source information about the Libnucnet__Species *p_species:
//       </synopsis>
//
//       <code>
// printf(
//   "Data source for species = %s\n",
//   Libnucnet__Species__getSource( p_species )
// );
//       </code>
//     </doc>
//
//   </usage>
//
// </routine>
//############################################################################*/

const char *
Libnucnet__Species__getSource( const Libnucnet__Species * );

/*##############################################################################
// <routine name="Libnucnet__Nuc__getSpeciesByZA()">
//
//   <description>
//     <abstract>
//       Retrieves a species in the collection of species given the Z, A, and
//       state of the species.
//     </abstract>
//     <keywords>
//       nuclear, data, species, Z, A, state
//     </keywords>
//   </description>
//
//   <usage>
//
//     <calling_sequence>
// Libnucnet__Species *
// Libnucnet__Nuc__getSpeciesByZA( 
//   const Libnucnet__Nuc *self,
//   unsigned int i_z,
//   unsigned int i_a,
//   const char *s_state
// );
//     </calling_sequence>
//
//     <param
//       name="self"
//       kind="in,positional,required"
//     >
//       A pointer to a Libnucnet__Nuc structure.
//     </param>
//     <param
//       name="i_z"
//       kind="in,positional,required"
//       doc="z"
//     >
//       The Z of the species desired.
//     </param>
//     <param
//       name="i_a"
//       kind="in,positional,required"
//       doc="a"
//     >
//       The A of the species desired.
//     </param>
//     <param
//       name="s_state"
//       kind="in,positional,required"
//       doc="state"
//     >
//       The state of the species desired.
//     </param>
//
//     <param
//       kind="return"
//       doc="result"
//     />
//
//     <doc kind="pre" id="z">
//       i_z is a valid Z of a species in the collection of species.
//     </doc>
//     <doc kind="pre" id="a">
//       i_a is a valid A of a species in the collection of species.
//     </doc>
//     <doc kind="pre" id="state">
//       s_state is a valid state of a species in the collection of species.
//       If not present, use empty string or NULL.
//     </doc>
//     <doc kind="post" id="result">
//       Routine returns a pointer to the species retrieved.  If no such
//       species is found, the routine returns a NULL.  If the input structure
//       is not valid, error handling is invoked.
//     </doc>
//
//     <doc kind="bug" >
//     </doc>
//
//     <doc kind="example" id="example1">
//       <synopsis>
//       Get the pointer to the species al27 in nuclear data structure
//       p_my_nuclei:
//       </synopsis>
//
//
//       <code>
// Libnucnet__Species *p_al27;
// p_al27 =
//   Libnucnet__Nuc__getSpeciesByZA( p_my_nuclei, 13, 27, NULL ) );
//       </code>
//     </doc>
//
//     <doc kind="example" id="example2">
//       <synopsis>
//       Get the pointer to species al26m in a collection of species
//       p_my_nuclei:
//       </synopsis>
//
//       <code>
// Libnucnet__Species *p_al26m;
// p_al26_m =
//   Libnucnet__Nuc__getSpeciesByZA( p_my_nuclei, 13, 26, 'm' ) );
//       </code>
//     </doc>
//
//   </usage>
//
// </routine>
//############################################################################*/

Libnucnet__Species *
Libnucnet__Nuc__getSpeciesByZA(
  const Libnucnet__Nuc *,
  unsigned int,
  unsigned int,
  const char *
);

/*##############################################################################
// <routine name="Libnucnet__Species__computePartitionFunction()">
//
//   <description>
//     <abstract>
//       Compute the partition function of a species in the collection
//       of nuclear species given the temperature.
//     </abstract>
//     <keywords>
//       nuclear, data, species, t9, partition function
//     </keywords>
//   </description>
//
//   <usage>
//
//     <calling_sequence>
// double
// Libnucnet__Species__computePartitionFunction(
//   const Libnucnet__Species *self,
//   double d_t9
// );
//     </calling_sequence>
//
//     <param
//       name="self"
//       kind="in,positional,required"
//     >
//       A pointer to a Libnucnet species structure.
//     </param>
//     <param
//       name="d_t9"
//       kind="in,positional,required"
//     >
//       The temperature (in billions of K) at which the partition function
//       will be calculated.
//     </param>
//
//     <param
//       kind="return"
//       doc="result"
//     />
//
//     <doc kind="post" id="result">
//       Routine returns a double representing the partition function of the
//       species retrieved.  If input is invalid, error handling is
//       invoked.
//     </doc>
//
//     <doc kind="bug" >
//     </doc>
//
//     <doc kind="example" id="example">
//       <synopsis>
//       Compute the partition function of he4 in the collection of nuclear
//       species p_my_nuclei
//       at a t9 of 0.2:
//       </synopsis>
//
//       <code>
// if(
//     (
//       p_species =
//         Libnucnet__Nuc__getSpeciesByName( p_my_nuclei, "he4" )
//     )
// ) {
//     d_partf =
//       Libnucnet__Species__computePartitionFunction(
//          p_species, 0.2
//       );
// }
//       </code>
//     </doc>
//
//   </usage>
//
// </routine>
//############################################################################*/

double
Libnucnet__Species__computePartitionFunction(
  const Libnucnet__Species *,
  double
);

/*##############################################################################
// <routine name="Libnucnet__Species__getIndex()">
//
//   <description>
//     <abstract>
//       Routine to return the index of a species in a Libnucnet__Nuc structure.
//     </abstract>
//     <keywords>
//       nuclear, data, number, index, species
//     </keywords>
//   </description>
//
//   <usage>
//
//     <calling_sequence>
// size_t
// Libnucnet__Species__getIndex( const Libnucnet__Species *self );
//     </calling_sequence>
//
//     <param
//       name="self"
//       kind="in,positional,required"
//     >
//       A pointer to a Libnucnet species structure.
//     </param>
//
//     <param
//       kind="return"
//       doc="result"
//     />
//
//     <doc kind="post" id="result">
//       Returns the index of the species in the nuclear species collection.
//       If the species is invalid, Libnucnet__Nuc error handling is invoked.
//     </doc>
//
//     <doc kind="bug" >
//     </doc>
//
//     <doc kind="example" id="example">
//       <synopsis>
//       Print the index of he4 in the Libnucnet structure p_my_nuclei:
//       </synopsis>
//
//       <code>
// if( 
//     p_he4 = 
//       Libnucnet__Nuc__getSpeciesByName(
//         p_my_nuclei, "he4"
//       )
// ) {
//   printf(
//     "Index of he4 = %d\n",
//     Libnucnet__Species__getIndex( p_he4 )
//   );
// }
//       </code>
//     </doc>
//
//   </usage>
//
// </routine>
//############################################################################*/

size_t Libnucnet__Species__getIndex( const Libnucnet__Species * );

/*##############################################################################
// <routine name="Libnucnet__Nuc__removeSpecies()">
//
//   <description>
//     <abstract>
//       Remove a species from a Libnucnet__Nuc structure.
//     </abstract>
//     <keywords>
//       nuclear, data, species, remove
//     </keywords>
//   </description>
//
//   <usage>
//
//     <calling_sequence>
// int
// Libnucnet__Nuc__removeSpecies(
//   Libnucnet__Nuc *self, Libnucnet__Species *p_species
// );
//     </calling_sequence>
//
//     <param
//       name="self"
//       kind="in,positional,required"
//     >
//       A pointer to a Libnucnet nuclear species collection.
//     </param>
//
//     <param
//       name="p_species"
//       kind="in,positional,required"
//     >
//       A pointer to a Libnucnet species structure to be removed from self.
//     </param>
//
//     <param
//       kind="return"
//       doc="result"
//     />
//
//     <doc kind="post" id="result">
//       Routine returns 1 for successful removal, 0 for failure.
//       Upon successful return, the species has been removed from the
//       Libnucnet__Nuc structure and the species indices have been updated.
//       If the input is invalid, Libnucnet__Nuc error handling is invoked.
//     </doc>
//
//     <doc kind="bug" >
//     </doc>
//
//     <doc kind="example" id="example">
//       <synopsis>
//         Remove he4 from Libnucnet__Nuc *p_my_nuclei:
//       </synopsis>
//
//       <code>
// Libnucnet__Species *p_he4 =
//   Libnucnet__Nuc__getSpeciesByName( p_my_nuclei, "he4" );
// if( 
//   Libnucnet__Nuc__removeSpecies( p_my_nuclei, p_he4 )
// )
//   printf( "Successful removal.\n" );
//       </code>
//     </doc>
//
//   </usage>
//
// </routine>
//############################################################################*/

int
Libnucnet__Nuc__removeSpecies(
  Libnucnet__Nuc *,
  Libnucnet__Species *
);

/*##############################################################################
// <routine name="Libnucnet__Nuc__free()">
//
//   <description>
//     <abstract>
//       Free the memory allocated for a Libnucnet nuclear species collection.
//     </abstract>
//     <keywords>
//       nuclear, data, remove, delete, free
//     </keywords>
//   </description>
//
//   <usage>
//
//     <calling_sequence>
// void Libnucnet__Nuc__free( Libnucnet__Nuc *self );
//     </calling_sequence>
//
//     <param
//       name="self"
//       kind="in,positional,required"
//     >
//       A pointer to a Libnucnet nuclear species collection.
//     </param>
//
//     <param
//       kind="return"
//       doc="result"
//     />
//
//     <doc kind="post" id="result">
//       Upon successful return, the memory for the collection has been freed.
//     </doc>
//
//     <doc kind="bug" >
//     </doc>
//
//     <doc kind="example" id="example">
//       <synopsis>
//         Free up memory used for Libnucnet__Nuc *p_my_nuclei:
//       </synopsis>
//
//       <code>
// Libnucnet__Nuc__free( p_my_nuclei );
//       </code>
//     </doc>
//
//   </usage>
//
// </routine>
//############################################################################*/

void Libnucnet__Nuc__free( Libnucnet__Nuc * );

/*##############################################################################
// <routine name="Libnucnet__Nuc__writeToXmlFile()">
//
//   <description>
//     <abstract>
//       Output a Libnucnet__Nuc structure to an xml file.
//     </abstract>
//     <keywords>
//       nuclear, data, write, file
//     </keywords>
//   </description>
//
//   <usage>
//
//     <calling_sequence>
// void
// Libnucnet__Nuc__writeToXmlFile(
//   const Libnucnet__Nuc *self,
//   const char *s_xml_filename
// );
//     </calling_sequence>
//
//     <param
//       name="self"
//       kind="in,positional,required"
//     >
//       A pointer to a Libnucnet nuclear species collection.
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
//       Upon successful return, the contents of the collection of species
//       have been written to an xml file. 
//       If the input is invalid, Libnucnet__Nuc error handling is invoked.
//     </doc>
//
//     <doc kind="bug" >
//     </doc>
//
//     <doc kind="example" id="example">
//       <synopsis>
//         Dump the contents of Libnucnet__Nuc *p_my_nuclei to the
//         xml file my.xml:
//       </synopsis>
//
//       <code>
// Libnucnet__Nuc__writeToXmlFile( p_my_nuclei, "my.xml" );
//       </code>
//     </doc>
//
//   </usage>
//
// </routine>
//############################################################################*/

void Libnucnet__Nuc__writeToXmlFile( const Libnucnet__Nuc *, const char * );

/*##############################################################################
// <routine name="Libnucnet__Nuc__is_valid_input_xml()">
//
//   <description>
//     <abstract>
//       Validate an input xml file for Libnucnet__Nuc.
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
// Libnucnet__Nuc__is_valid_input_xml(
//   const char *s_xml_filename
// );
//     </calling_sequence>
//
//     <param
//       name="s_xml_filename" 
//       kind="in,positional,required" 
//     >
//       A string giving the name of the xml file containing the nuclear data.
//     </param>
//     <param
//       kind="return"
//       doc="result"
//     />
//
//     <doc kind="post" id="result">
//       For a valid input nuclear data xml file, the routine returns
//       1 (true).  If the input file is not valid, routine returns 0 (false).
//       For an invalid schema file, or if the schema file cannot be
//       read over the web, routine stops and prints error
//       message. 
//     </doc>
//
//     <doc kind="bug" >
//     </doc>
//
//     <doc kind="example" id="example1">
//       <synopsis>
//         Validate the input xml file "nuclear_data.xml":
//       </synopsis>
//
//       <code>
// if( Libnucnet__Nuc__is_valid_input_xml( "nuclear_data.xml" ) ) {
//     printf( "Valid xml!\n" );
// }
//       </code>
//     </doc>
//
//   </usage>
//
// </routine>
//############################################################################*/

int Libnucnet__Nuc__is_valid_input_xml( const char * );

/*##############################################################################
// <routine name="Libnucnet__Nuc__sortSpecies()">
//
//   <description>
//     <abstract>
//       Sort the species in a Libnucnet__Nuc structure according to the
//       current Libnucnet__Species__compare_function.
//     </abstract>
//     <keywords>
//       Libnucnet, nuclear, xml, hash, sort, list
//     </keywords>
//   </description>
//
//   <usage>
//
//     <calling_sequence>
// void Libnucnet__Nuc__sortSpecies( Libnucnet__Nuc *self );
//     </calling_sequence>
//
//     <param
//       name="self" 
//       kind="in,positional,required" 
//     >
//       A pointer to a Libnucnet__Nuc structure.
//     </param>
//     <param
//       kind="return"
//       doc="result"
//     />
//
//     <doc kind="post" id="result">
//       On successful return, the species in the structure have been sorted
//       according to the current Libnucnet__Species__compare_function.
//       If the default is set, the species will be sorted in increasing Z
//       and A. If the input structure is not valid or memory cannot be
//       allocated for the sorting, error handling is invoked.
//     </doc>
//
//     <doc kind="bug" >
//     </doc>
//
//     <doc kind="example" id="example1">
//       <synopsis>
//         Sort the species in p_my_nuclei:
//       </synopsis>
//
//       <code>
// Libnucnet__Nuc__sortSpecies( p_my_nuclei );
//       </code>
//     </doc>
//
//   </usage>
//
// </routine>
//############################################################################*/

void Libnucnet__Nuc__sortSpecies( Libnucnet__Nuc * );

/*##############################################################################
// <routine name="Libnucnet__Nuc__setSpeciesCompareFunction()">
//
//   <description>
//     <abstract>
//       Set the function to be applied during a species sort.
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
// Libnucnet__Nuc__setSpeciesCompareFunction(
//   Libnucnet__Nuc *self,
//   Libnucnet__Species__compare_function pfFunc
// );
//     </calling_sequence>
//
//     <param
//       name="self" 
//       kind="in,positional,required" 
//     >
//       A pointer to a Libnucnet__Nuc structure.
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
//       Upon successful return, the data compare function for species
//       in the collection has been set to the input function.
//       If any input is invalid, error handling is invoked.
//     </doc>
//
//     <doc kind="bug" >
//     </doc>
//
//     <doc kind="example" id="example1">
//       <synopsis>
//         Set the species compare function in p_my_nuclei to my_sort_function:
//       </synopsis>
//
//       <code>
// Libnucnet__Nuc__setSpeciesCompareFunction(
//   p_my_nuclei,
//   (Libnucnet__Species__compare_function) my_sort_function
// );
//       </code>
//     </doc>
//
//   </usage>
//
// </routine>
//############################################################################*/

void
Libnucnet__Nuc__setSpeciesCompareFunction(
  Libnucnet__Nuc *,
  Libnucnet__Species__compare_function
);

/*##############################################################################
// <routine name="Libnucnet__Nuc__clearSpeciesCompareFunction()">
//
//   <description>
//     <abstract>
//       Restore the default function to be applied during a species sort.
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
// Libnucnet__Nuc__clearSpeciesCompareFunction(
//   Libnucnet__Nuc *self
// );
//     </calling_sequence>
//
//     <param
//       name="self" 
//       kind="in,positional,required" 
//     >
//       A pointer to a Libnucnet__Nuc structure.
//     </param>
//
//     <param
//       kind="return"
//       doc="result"
//     />
//
//     <doc kind="post" id="result">
//       Upon successful return, the species compare function for
//       the collection has been restored to the default function.
//       If any input is invalid, error handling is invoked.
//     </doc>
//
//     <doc kind="bug" >
//     </doc>
//
//     <doc kind="example" id="example1">
//       <synopsis>
//         Clear the species compare function in p_my_nuclei:
//       </synopsis>
//
//       <code>
// Libnucnet__Nuc__clearSpeciesCompareFunction(
//   p_my_nuclei
// );
//       </code>
//     </doc>
//
//   </usage>
//
// </routine>
//############################################################################*/

void
Libnucnet__Nuc__clearSpeciesCompareFunction(
  Libnucnet__Nuc *
);

/*##############################################################################
// <routine name="Libnucnet__Nuc__iterateSpecies()">
//
//   <description>
//     <abstract>
//       Iterate through the species and apply the user-supplied iterate
//       function.
//     </abstract>
//     <keywords>
//       Libnucnet, nuclear, xml, iterate
//     </keywords>
//   </description>
//
//   <usage>
//
//     <calling_sequence>
// void
// Libnucnet__Nuc__iterateSpecies(
//   const Libnucnet__Nuc *self,
//   Libnucnet__Species__iterateFunction pfFunc,
//   void *p_user_data
// );
//     </calling_sequence>
//
//     <param
//       name="self" 
//       kind="in,positional,required" 
//     >
//       A pointer to a Libnucnet__Nuc structure.
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
//       A pointer to the user-supplied data structure.
//     </param>
//     <param
//       kind="return"
//       doc="result"
//     />
//
//     <doc kind="post" id="result">
//       The routine iterates through the species collection and applies
//       the user-supplied routine to each species.  If any input is
//       invalid, error handling is invoked.
//     </doc>
//
//     <doc kind="bug" >
//     </doc>
//
//     <doc kind="example" id="example1">
//       <synopsis>
//         Iterate through the species in p_my_nuclei and apply the
//         function my_iterate_function and the extra data in
//         p_user_data:
//       </synopsis>
//
//       <code>
// Libnucnet__Nuc__iterateSpecies(
//   p_my_nuclei,
//   (Libnucnet__Species__iterateFunction) my_iterate_function,
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
Libnucnet__Nuc__iterateSpecies(
  const Libnucnet__Nuc *,
  Libnucnet__Species__iterateFunction,
  void *
);

/*##############################################################################
// <routine name="Libnucnet__Nuc__extractSubset()">
//
//   <description>
//     <abstract>
//       Extract a subset collection of species from another collection by
//       an XPath expression.
//     </abstract>
//     <keywords>
//       Libnucnet, nuclear, xml, hash, extract, subset
//     </keywords>
//   </description>
//
//   <usage>
//
//     <calling_sequence>
// Libnucnet__Nuc *
// Libnucnet__Nuc__extractSubset(
//   const Libnucnet__Nuc *self
//   const char *s_xpath
// );
//     </calling_sequence>
//
//     <param
//       name="self" 
//       kind="in,positional,required" 
//     >
//       A pointer to a pre-existing Libnucnet__Nuc structure.
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
//       For a valid Libnucnet__Nuc structure, the routine returns a new
//       Libnucnet__Nuc structure containing nuclei selected from the parent
//       species collection by the XPath expression.  The user must free
//       the structure with Libnucnet__Nuc__free() when finished with it.
//       If the input structure is not valid,
//       Libnucnet__Nuc error handling is invoked.
//     </doc>
//
//     <doc kind="bug" >
//     </doc>
//
//     <doc kind="example" id="example1">
//       <synopsis>
//         Extract a complete copy of Libnucnet_Nuc *p_parent and call it
//         p_copy:
//       </synopsis>
//
//       <code>
// p_copy =
//   Libnucnet__Nuc__extractSubset(
//     p_parent,
//     NULL
//   );
//       </code>
//     </doc>
//
//     <doc kind="example" id="example2">
//       <synopsis>
//         Extract a subset of Libnucnet_Nuc *p_parent that includes
//         only nuclei with Z > 10 and A < 20.
//       </synopsis>
//
//       <code>
// p_copy =
//   Libnucnet__Nuc__extractSubset(
//     p_parent,
//     "[z > 10 and a < 20]"
//   );
//       </code>
//     </doc>
//
//   </usage>
//
// </routine>
//############################################################################*/

Libnucnet__Nuc *
Libnucnet__Nuc__extractSubset( const Libnucnet__Nuc *, const char * );

/*##############################################################################
// <routine name="Libnucnet__Species__updateMassExcess()">
//
//   <description>
//     <abstract>
//       Update the mass excess of the species.
//     </abstract>
//     <keywords>
//       nuclear, data, species, mass, excess, update
//     </keywords>
//   </description>
//
//   <usage>
//
//     <calling_sequence>
// void
// Libnucnet__Species__updateMassExcess(
//   Libnucnet__Species *self,
//   double d_new_mass_excess
// );
//     </calling_sequence>
//
//     <param
//       name="self"
//       kind="in,positional,required"
//     >
//       A pointer to a Libnucnet__Species structure.
//     </param>
//
//     <param
//       name="d_new_mass_excess"
//       kind="in,positional,required"
//     >
//       A double giving the new value of the mass excess (in MeV).
//     </param>
//
//     <param
//       kind="return"
//       doc="result"
//     />
//
//     <doc kind="post" id="result">
//       Upon successful return, the mass excess for the species has
//       been updated to the new value.  If the input species is
//       invalid, error handling is invoked.
//     </doc>
//
//     <doc kind="bug" >
//     </doc>
//
//     <doc kind="example" id="example">
//       <synopsis>
//         Update the mass excess of p_species to 8.22 MeV:
//       </synopsis>
//
//       <code>
// Libnucnet__Species__updateMassExcess( p_species, 8.22 );
//       </code>
//     </doc>
//
//   </usage>
//
// </routine>
//############################################################################*/

void
Libnucnet__Species__updateMassExcess( Libnucnet__Species *, double );

/*##############################################################################
// <routine name="Libnucnet__Species__updateSpin()">
//
//   <description>
//     <abstract>
//       Update the spin of the species.
//     </abstract>
//     <keywords>
//       nuclear, data, species, spin, update
//     </keywords>
//   </description>
//
//   <usage>
//
//     <calling_sequence>
// void
// Libnucnet__Species__updateSpin(
//   Libnucnet__Species *self, 
//   double d_new_spin
// );
//     </calling_sequence>
//
//     <param
//       name="self"
//       kind="in,positional,required"
//     >
//       A pointer to a Libnucnet__Species structure.
//     </param>
//
//     <param
//       name="d_spin"
//       kind="in,positional,required"
//     >
//       A double giving the new value of the spin.
//     </param>
//
//     <param
//       kind="return"
//       doc="result"
//     />
//
//     <doc kind="post" id="result">
//       Upon successful return, the spin of the species has
//       been updated to the new value.  If the input species is
//       invalid, error handling is invoked.
//     </doc>
//
//     <doc kind="bug" >
//     </doc>
//
//     <doc kind="example" id="example">
//       <synopsis>
//         Update the spin of p_species to 3/2:
//       </synopsis>
//
//       <code>
// Libnucnet__Species__updateSpin( p_species, 1.5 );
//       </code>
//     </doc>
//
//   </usage>
//
// </routine>
//############################################################################*/

void
Libnucnet__Species__updateSpin( Libnucnet__Species *, double );

/*##############################################################################
// <routine name="Libnucnet__Species__updateSource()">
//
//   <description>
//     <abstract>
//       Update the string giving the data source for the species.
//     </abstract>
//     <keywords>
//       nuclear, data, species, source, update
//     </keywords>
//   </description>
//
//   <usage>
//
//     <calling_sequence>
// void
// Libnucnet__Species__updateSource(
//   Libnucnet__Species *self,
//   const char *s_source
// );
//     </calling_sequence>
//
//     <param
//       name="self"
//       kind="in,positional,required"
//     >
//       A pointer to a Libnucnet__Species structure.
//     </param>
//
//     <param
//       name="s_source"
//       kind="in,positional,required"
//     >
//       A string giving the new source for the data.
//     </param>
//
//     <param
//       kind="return"
//       doc="result"
//     />
//
//     <doc kind="post" id="result">
//       Upon successful return, the data source string for the species has
//       been updated to the new value.  If the input species is
//       invalid, error handling is invoked.
//     </doc>
//
//     <doc kind="bug" >
//     </doc>
//
//     <doc kind="example" id="example">
//       <synopsis>
//         Update the source string for the data to "Updated data
//         source":
//       </synopsis>
//
//       <code>
// Libnucnet__Species__updateSource(
//   p_species,
//   "Updated data source"
// );
//       </code>
//     </doc>
//
//   </usage>
//
// </routine>
//############################################################################*/

void
Libnucnet__Species__updateSource( Libnucnet__Species *, const char * );

/*##############################################################################
// <routine name="Libnucnet__Species__updatePartitionFunctionData()">
//
//   <description>
//     <abstract>
//       Update the partition function data for the species.
//     </abstract>
//     <keywords>
//       nuclear, data, species, partition, function, update
//     </keywords>
//   </description>
//
//   <usage>
//
//     <calling_sequence>
// void
// Libnucnet__Species__updatePartitionFunctionData(
//   Libnucnet__Species *self,
//   gsl_vector *p_new_t9_array,
//   gsl_vector *p_new_log10_partf_array
// );
//     </calling_sequence>
//
//     <param
//       name="self"
//       kind="in,positional,required"
//     >
//       A pointer to a Libnucnet__Species structure.
//     </param>
//
//     <param
//       name="p_new_t9_array"
//       kind="in,positional,required"
//     >
//       A gsl_vector containing the new T9 values for the partition
//       function table.  It must have the same size as the log10 partf
//       array.
//     </param>
//
//     <param
//       name="p_new_log10_partf_array"
//       kind="in,positional,required"
//     >
//       A gsl_vector containing the new log10 values of the partition
//       function for the partition function table.  It must have the same
//       size as the t9 array.
//     </param>
//
//     <param
//       kind="return"
//       doc="result"
//     />
//
//     <doc kind="post" id="result">
//       Upon successful return, the partition function data for the species has
//       been updated to the new values.  If the input species is
//       invalid, or if the new arrays cannot be allocated, error handling is
//       invoked.
//     </doc>
//
//     <doc kind="bug" >
//     </doc>
//
//     <doc kind="example" id="example">
//       <synopsis>
//         Update the t9 and log10 partf arrays for the species
//         p_species to p_new_t9 and p_new_log10_partf:
//       </synopsis>
//
//       <code>
// Libnucnet__Species__updatePartitionFunctionData(
//   p_species,
//   p_new_t9,
//   p_new_log10_partf
// );
//       </code>
//     </doc>
//
//   </usage>
//
// </routine>
//############################################################################*/

void
Libnucnet__Species__updatePartitionFunctionData(
  Libnucnet__Species *,
  gsl_vector *,
  gsl_vector *
);

/*##############################################################################
// <routine name="Libnucnet__Species__free()">
//
//   <description>
//     <abstract>
//       Free the memory allocated for a Libnucnet species.
//     </abstract>
//     <keywords>
//       nuclear, data, remove, delete, free, species
//     </keywords>
//   </description>
//
//   <usage>
//
//     <calling_sequence>
// void
// Libnucnet__Species__free( Libnucnet__Species *self );
//     </calling_sequence>
//
//     <param
//       name="self"
//       kind="in,positional,required"
//     >
//       A pointer to a Libnucnet species.
//     </param>
//
//     <param
//       kind="return"
//       doc="result"
//     />
//
//     <doc kind="post" id="result">
//       Upon successful return, the memory for the species has been freed.
//     </doc>
//
//     <doc kind="bug" >
//     </doc>
//
//     <doc kind="example" id="example">
//       <synopsis>
//         Free up memory used for Libnucnet__Species *p_species:
//       </synopsis>
//
//       <code>
// Libnucnet__Species__free( p_species );
//       </code>
//     </doc>
//
//   </usage>
//
// </routine>
//############################################################################*/

void
Libnucnet__Species__free( Libnucnet__Species * );

/*##############################################################################
// <routine name="Libnucnet__Species__getPartitionFunctionT9()">
//
//   <description>
//     <abstract>
//       Retrieves the pointer to the gsl_vector containing the temperature
//       data for the partition function for a species in the nuclear species
//       collection.
//     </abstract>
//     <keywords>
//       nuclear, data, species, partition, function, temperature
//     </keywords>
//   </description>
//
//   <usage>
//
//     <calling_sequence>
// gsl_vector *
// Libnucnet__Species__getPartitionFunctionT9(
//   const Libnucnet__Species *self
// );
//     </calling_sequence>
//
//     <param
//       name="self"
//       kind="in,positional,required"
//     >
//       A pointer to a Libnucnet__Species structure.
//     </param>
//
//     <param
//       kind="return"
//       doc="result"
//     />
//
//     <doc kind="post" id="result">
//       Routine returns the pointer to the gsl_vector containing the T9
//       points for the partition function data for the
//       species.  If the input species is invalid, error
//       handling is invoked.
//     </doc>
//
//     <doc kind="bug" >
//     </doc>
//
//     <doc kind="example" id="example">
//       <synopsis>
//       Print the gsl_vector t9 array for mo94 in
//       *p_my_nuclei:
//       </synopsis>
//
//
//       <code>
// p_species =
//   Libnucnet__Nuc__getSpeciesByName( p_my_nuclei, "mo94" );
// if( p_species )
//   gsl_vector_fprintf(
//     stdout,
//     Libnucnet__Species__getPartitionFunctionT9( p_species ),
//     "%g"
//   );
//       </code>
//     </doc>
//
//   </usage>
//
// </routine>
//############################################################################*/

gsl_vector *
Libnucnet__Species__getPartitionFunctionT9(
  const Libnucnet__Species *
);

/*##############################################################################
// <routine name="Libnucnet__Species__getPartitionFunctionLog10()">
//
//   <description>
//     <abstract>
//       Retrieves the pointer to the gsl_vector containing the 
//       data for the partition function for a species in the nuclear species
//       collection.
//     </abstract>
//     <keywords>
//       nuclear, data, species, partition, function, temperature
//     </keywords>
//   </description>
//
//   <usage>
//
//     <calling_sequence>
// gsl_vector *
// Libnucnet__Species__getPartitionFunctionLog10(
//   const Libnucnet__Species *self
// );
//     </calling_sequence>
//
//     <param
//       name="self"
//       kind="in,positional,required"
//     >
//       A pointer to a Libnucnet__Species structure.
//     </param>
//
//     <param
//       kind="return"
//       doc="result"
//     />
//
//     <doc kind="post" id="result">
//       Routine returns the pointer to the gsl_vector containing the log10
//       partition function data points for the 
//       species.  If the input species is invalid, error
//       handling is invoked.
//     </doc>
//
//     <doc kind="bug" >
//     </doc>
//
//     <doc kind="example" id="example">
//       <synopsis>
//       Print the log10 partition function data for mo94 in
//       *p_my_nuclei:
//       </synopsis>
//
//
//       <code>
// p_species =
//   Libnucnet__Nuc__getSpeciesByName( p_my_nuclei, "mo94" );
// if( p_species )
//   gsl_vector_fprintf(
//     stdout,
//     Libnucnet__Species__getPartitionFunctionLog10( p_species ),
//     "%g"
//   );
//       </code>
//     </doc>
//
//   </usage>
//
// </routine>
//############################################################################*/

gsl_vector *
Libnucnet__Species__getPartitionFunctionLog10(
  const Libnucnet__Species *
);

/*##############################################################################
// <routine name="Libnucnet__Species__copy()">
//
//   <description>
//     <abstract>
//       Copy a Libnucnet__Species.
//     </abstract>
//     <keywords>
//       Libnucnet, nuclear, data, species, copy
//     </keywords>
//   </description>
//
//   <usage>
//
//     <calling_sequence>
// Libnucnet__Species *
// Libnucnet__Species__copy( const Libnucnet__Species *p_species );
//     </calling_sequence>
//
//     <param
//       name="p_species"
//       kind="in,positional,required"
//     >
//       A pointer to the species to be copied.
//     </param>
//     <param
//       kind="return"
//       doc="result"
//     />
//
//     <doc kind="post" id="result">
//       The routine returns a pointer to a new Libnucnet__Species structure
//       that is a copy of the input structure.
//       If it is not possible to allocate memory for the new structure,
//       Libnucnet error handling is invoked.
//     </doc>
//
//     <doc kind="bug" >
//     </doc>
//
//     <doc kind="example" id="example">
//       <synopsis>
//         Create a copy of p_species:
//       </synopsis>
//
//       <code>
// p_copy = Libnucnet__Species__copy( p_species );
//       </code>
//     </doc>
//
//   </usage>
//
// </routine>
//############################################################################*/

Libnucnet__Species *
Libnucnet__Species__copy( const Libnucnet__Species * );

/*##############################################################################
// <routine name="Libnucnet__Species__computeQuantumAbundance()">
//
//   <description>
//     <abstract>
//       Compute the quantum abundance of a nuclear species at
//       the given the temperature and density
//     </abstract>
//     <keywords>
//       nuclear, data, species, t9, quantum, abundance
//     </keywords>
//   </description>
//
//   <usage>
//
//     <calling_sequence>
// double
// Libnucnet__Species__computeQuantumAbundance(
//   const Libnucnet__Species *self,
//   double d_t9,
//   double d_rho
// );
//     </calling_sequence>
//
//     <param
//       name="self"
//       kind="in,positional,required"
//     >
//       A pointer to a Libnucnet species structure.
//     </param>
//     <param
//       name="d_t9"
//       kind="in,positional,required"
//     >
//       The temperature (in billions of K) at which the quantum abundance
//       will be calculated.
//     </param>
//     <param
//       name="d_rho"
//       kind="in,positional,required"
//     >
//       The mass density (in g/cc) at which the quantum abundance
//       will be calculated.
//     </param>
//
//     <param
//       kind="return"
//       doc="result"
//     />
//
//     <doc kind="post" id="result">
//       Routine returns a double representing the quantum abundance of
//       the given species.  If input is invalid, error handling is
//       invoked.
//     </doc>
//
//     <doc kind="bug" >
//     </doc>
//
//     <doc kind="example" id="example">
//       <synopsis>
//       Compute the quantum abundance of si29 in the collection of nuclear
//       species p_my_nuclei
//       at a t9 of 0.2 and mass density of 1000 g/cc:
//       </synopsis>
//
//       <code>
// if(
//     (
//       p_species =
//         Libnucnet__Nuc__getSpeciesByName( p_my_nuclei, "si29" )
//     )
// ) {
//     d_y_Q =
//       Libnucnet__Species__computeQuantumAbundance(
//          p_species,
//          0.2,
//          1000.
//       );
// }
//       </code>
//     </doc>
//
//   </usage>
//
// </routine>
//############################################################################*/

double
Libnucnet__Species__computeQuantumAbundance(
  const Libnucnet__Species *,
  double d_t9,
  double d_rho
);

/*##############################################################################
// <routine name="Libnucnet__Nuc__computeSpeciesBindingEnergy()">
//
//   <description>
//     <abstract>
//       Compute the binding energy of a nuclear species in MeV.
//     </abstract>
//     <keywords>
//       nuclear, data, species, binding, energy
//     </keywords>
//   </description>
//
//   <usage>
//
//     <calling_sequence>
// double
// Libnucnet__Nuc__computeSpeciesBindingEnergy(
//   const Libnucnet__Nuc *self,
//   const Libnucnet__Species *p_species
// );
//     </calling_sequence>
//
//     <param
//       name="self"
//       kind="in,positional,required"
//     >
//       A pointer to a Libnucnet__Nuc structure.
//     </param>
//     <param
//       name="p_species"
//       kind="in,positional,required"
//     >
//       The species whose binding energy is sought.
//     </param>
//
//     <param
//       kind="return"
//       doc="result"
//     />
//
//     <doc kind="post" id="result">
//       Routine returns a double representing the nuclear binding energy
//       of the input species in MeV.  If input is invalid, error handling is
//       invoked.
//     </doc>
//
//     <doc kind="bug" >
//     </doc>
//
//     <doc kind="example" id="example">
//       <synopsis>
//       Compute the binding eneryg of oxygen-16 in the collection of nuclear
//       species p_my_nuclei:
//       </synopsis>
//
//       <code>
// if(
//     (
//       p_species =
//         Libnucnet__Nuc__getSpeciesByName( p_my_nuclei, "o16" )
//     )
// )
//   fprintf( 
//     stdout,
//     "Binding energy of %s = %f MeV.\n",
//     Libnucnet__Species__getName( p_species ),
//     Libnucnet__Nuc__computeSpeciesBindingEnergy(
//       p_my_nuclei,
//       p_species
//     )
//   );
//       </code>
//     </doc>
//
//   </usage>
//
// </routine>
//############################################################################*/

double
Libnucnet__Nuc__computeSpeciesBindingEnergy(
  const Libnucnet__Nuc *,
  const Libnucnet__Species *
);

/*##############################################################################
// <routine name="Libnucnet__NucView__new()">
//
//   <description>
//     <abstract>
//       Create a new Libnucnet__NucView structure.
//     </abstract>
//     <keywords>
//       Libnucnet, nuclear, data, xml, view
//     </keywords>
//   </description>
//
//   <usage>
//
//     <calling_sequence>
// Libnucnet__NucView *
// Libnucnet__NucView__new(
//   const Libnucnet__Nuc * p_nuc,
//   const char * s_nuc_xpath
// );
//     </calling_sequence>
//
//     <param
//       name="p_nuc"
//       kind="in,positional,required"
//     >
//       A pointer to a nuclide collection.
//     </param>
//
//     <param
//       name="s_nuc_xpath"
//       kind="in,positional,required"
//     >
//       A string giving the XPath expression that definies the nuclides
//       to be included in the view.
//     </param>
//
//     <param
//       kind="return"
//       doc="result"
//     />
//
//     <doc kind="post" id="result">
//       The routine returns a pointer to a new Libnucnet__NucView structure,
//       that is, a view of a parent nuclide collection.
//       If it is not possible to allocate memory for the new structure,
//       Libnucnet error handling is invoked.
//     </doc>
//
//     <doc kind="bug" >
//     </doc>
//
//     <doc kind="example" id="example">
//       <synopsis>
//         Create a view of the neon isotopes in Libnucnet__Nuc structure
//         p_my_nuclei:
//       </synopsis>
//
//       <code>
// p_view =
//   Libnucnet__NucView__new(
//     p_my_nuclei,
//     "[z = 10]"
// );
//       </code>
//     </doc>
//
//   </usage>
//
// </routine>
//############################################################################*/

Libnucnet__NucView *
Libnucnet__NucView__new(
  const Libnucnet__Nuc *,
  const char *
);

/*##############################################################################
// <routine name="Libnucnet__NucView__free()">
//
//   <description>
//     <abstract>
//       Free the memory allocated for a view of a Libnucnet nuclear species
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
// Libnucnet__NucView__free( Libnucnet__NucView *self );
//     </calling_sequence>
//
//     <param
//       name="self"
//       kind="in,positional,required"
//     >
//       A pointer to a view of a Libnucnet nuclear species collection.
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
//         Free up memory used for Libnucnet__NucView *p_my_view:
//       </synopsis>
//
//       <code>
// Libnucnet__NucView__free( p_my_view );
//       </code>
//     </doc>
//
//   </usage>
//
// </routine>
//############################################################################*/

void
Libnucnet__NucView__free( Libnucnet__NucView * );

/*##############################################################################
// <routine name="Libnucnet__NucView__getNuc()">
//
//   <description>
//     <abstract>
//       Get the nuclear collection in a view.
//     </abstract>
//     <keywords>
//       nuclear, view, collection
//     </keywords>
//   </description>
//
//   <usage>
//
//     <calling_sequence>
// Libnucnet__Nuc *
// Libnucnet__NucView__getNuc( Libnucnet__NucView *self );
//     </calling_sequence>
//
//     <param
//       name="self"
//       kind="in,positional,required"
//     >
//       A pointer to a view of a Libnucnet nuclear species collection.
//     </param>
//
//     <param
//       kind="return"
//       doc="result"
//     />
//
//     <doc kind="post" id="result">
//       Routine returns the nuclear collection from a Libnucnet__NucView.
//       If the input view is invalid, error handling is invoked.
//     </doc>
//
//     <doc kind="bug" >
//     </doc>
//
//     <doc kind="example" id="example">
//       <synopsis>
//         Get the collection from Libnucnet__NucView *p_my_view:
//       </synopsis>
//
//       <code>
// p_nuc = Libnucnet__NucView__getNuc( p_my_view );
//       </code>
//     </doc>
//
//   </usage>
//
// </routine>
//############################################################################*/

Libnucnet__Nuc *
Libnucnet__NucView__getNuc( Libnucnet__NucView * );

/*##############################################################################
// <routine name="Libnucnet__NucView__getXPath()">
//
//   <description>
//     <abstract>
//       Get the XPath expression defining a view.
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
// Libnucnet__NucView__getNuc( Libnucnet__NucView *self );
//     </calling_sequence>
//
//     <param
//       name="self"
//       kind="in,positional,required"
//     >
//       A pointer to a view of a Libnucnet nuclear species collection.
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
//         Print the XPath expression defining a Libnucnet__NucView *p_my_view:
//       </synopsis>
//
//       <code>
// printf(
//   "This is the view defined by %s.\n",
//   Libnucnet__NucView__getXPath( p_my_view )
// );
//       </code>
//     </doc>
//
//   </usage>
//
// </routine>
//############################################################################*/

const char *
Libnucnet__NucView__getXPath( Libnucnet__NucView * );

/*##############################################################################
// <routine name="Libnucnet__Nuc__getLargestNucleonNumber()">
//
//   <description>
//     <abstract>
//       Retrieve the largest Z, N, or A for species in the nuclide
//       collection.
//     </abstract>
//     <keywords>
//       nuclear network, zone, nucleon, number, largest
//     </keywords>
//   </description>
//
//   <usage>
//
//     <calling_sequence>
// unsigned int
// Libnucnet__Nuc__getLargestNucleonNumber(
//   Libnucnet__Nuc * self,
//   const char * s_nucleon_type
// );
//     </calling_sequence>
//
//     <param
//       name="self"
//       kind="in,positional,required"
//     >
//       A pointer to a nuclide collection.
//     </param>
//
//     <param
//       name="s_nucleon_type"
//       kind="in,positional,required"
//     >
//       A string determining whether to return the largest Z ("z"), N ("n"),
//       or A ("a").
//     </param>
//
//     <param
//       kind="return"
//       doc="result"
//     />
//
//     <doc kind="post" id="result">
//       Routine returns an unsigned int giving
//       the largest Z, N, or A of the species present
//       in the nuclide collection.
//       If the input zone or input string for the nucleon number type
//       is invalid, error handling is invoked.
//     </doc>
//
//     <doc kind="bug" >
//     </doc>
//
//     <doc kind="example" id="example">
//       <synopsis>
//       Print the largest neutron number of the species present in the
//       collection p_my_nuclei:
//       </synopsis>
//
//       <code>
// fprintf(
//   stdout,
//   "The largest N is: %d\n",
//   Libnucnet__Nuc__getLargestNucleonNumber(
//     p_my_nuclei,
//     "n"
//   )
// );
//       </code>
//     </doc>
//
//   </usage>
//
// </routine>
//############################################################################*/

unsigned int
Libnucnet__Nuc__getLargestNucleonNumber(
  Libnucnet__Nuc *,
  const char *
);

/*##############################################################################
// Non-API routines.
//############################################################################*/

void
Libnucnet__Nuc__updateFromXmlDocument( Libnucnet__Nuc *, xmlDocPtr, const char * );

int Libnucnet__Nuc__assignSpecies( Libnucnet__Nuc *, xmlNode * );

double
Libnucnet__Nuc__computeSpeciesNseFactor(
  const Libnucnet__Nuc *,
  const Libnucnet__Species *,
  double,
  double
);

void Libnucnet__Species__hashFree( Libnucnet__Species *, xmlChar * );

void Libnucnet__Nuc__getStates(
  Libnucnet__Nuc *, xmlNode *, unsigned int, unsigned int, int
);

xmlChar *Libnucnet__Nuc__create_species_name( unsigned int, unsigned int );

xmlChar *
Libnucnet__Nuc__create_unassigned_element_name( unsigned int );

void Libnucnet__Nuc__initializeHash( Libnucnet__Nuc * );

xmlDocPtr Libnucnet__Nuc__makeXmlDocument( const Libnucnet__Nuc * );

int
Libnucnet__Species__make_xml( xmlChar *, void * );

Libnucnet__Species **
Libnucnet__Nuc__createSpeciesArray( const Libnucnet__Nuc * );

void
Libnucnet__Nuc__createSpeciesArrayCallback(
  Libnucnet__Species *,
  void *,
  const xmlChar *
);

void
Libnucnet__Nuc__sortSpeciesArray(
  const Libnucnet__Nuc *,
  Libnucnet__Species **,
  Libnucnet__Species__compare_function
);

int
Libnucnet__Nuc__sort_helper(
  const void *,
  const void *
);

int
Libnucnet__Species__index_compare_function(
  const Libnucnet__Species *,
  const Libnucnet__Species *
);

int
Libnucnet__Species__default_compare_function(
  const Libnucnet__Species *,
  const Libnucnet__Species *
);

void
Libnucnet__Species__get_number_of_states(
  Libnucnet__Species *,
  int *,
  xmlChar *,
  xmlChar *,
  xmlChar *
);

int
Libnucnet__Species__makeStateXml( Libnucnet__Species *, xmlNodePtr );

void
Libnucnet__Species__addStateXmlData( Libnucnet__Species *, xmlNodePtr );

xmlListPtr
Libnucnet__Nuc__createSpeciesStateList( const Libnucnet__Nuc *, xmlChar * );

void
Libnucnet__Nuc__createSpeciesStateListCallback(
  Libnucnet__Species *,
  void *,
  xmlChar *
);

xmlListPtr
Libnucnet__Nuc__createSpeciesBaseNameList( const Libnucnet__Nuc * );

int
Libnucnet__Nuc__createSpeciesBaseNameListIterator(
  Libnucnet__Species *,
  void *
);

int
Libnucnet__Species__removeStates(
  Libnucnet__Species *,
  Libnucnet__Nuc *
);

Libnucnet__Nuc *
Libnucnet__Nuc__createNewNucForView(
  const Libnucnet__Nuc *
);

void
Libnucnet__Nuc__addSpeciesToViewFromXml(
  const Libnucnet__Nuc *,
  xmlHashTablePtr,
  xmlDocPtr,
  const char *
);

int
Libnucnet__Nuc__addSpeciesToView(
  const Libnucnet__Nuc *,
  xmlNode *,
  xmlHashTablePtr
);

void
Libnucnet__Nuc__getLargestNucleonNumberHelper(
  Libnucnet__Species *,
  void *,
  xmlChar *
);

#endif
