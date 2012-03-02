#--------------------------------------------------------------------------
#///////////////////////////////////////////////////////////////////////////////
# <file type="public">
#   <license>
#     This is free software; you can redistribute it and/or modify it
#     under the terms of the GNU General Public License as published by
#     the Free Software Foundation; either version 2 of the License, or
#     (at your option) any later version.
# 
#     This software is distributed in the hope that it will be useful,
#     but WITHOUT ANY WARRANTY; without even the implied warranty of
#     MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#     GNU General Public License for more details.
#
#     Please see the src/README.txt file in this distribution for more
#     information.
#   </license>
#
#   <description>
#     <abstract>
#       Makefile for generating libnucnet example codes.
#     </abstract>
#   </description>
#
#   <authors>
#     <current>
#       <author userid="mbradle" start_date="2006/11/07" />
#     </current>
#     <previous>
#     </previous>
#   </authors>
#
#   <compatibility>
#     Tested with GNU Make version 3.79.
#   </compatibility>
#
# </file>
#///////////////////////////////////////////////////////////////////////////////

SHELL = /bin/sh

#///////////////////////////////////////////////////////////////////////////////
# The following lines must be edited according to where you have
# placed your libnucnet and wn_matrix modules.  You may also
# edit to choose a different compiler (e.g., g++), to use Valgrind or
# not, or to profile:
#///////////////////////////////////////////////////////////////////////////////

GC=g++ -fopenmp

MODULES = ../../../modules

LIBNUCNETDIR = ../src/
MATRIXSRCDIR = $(MODULES)/wn_matrix/0.13/src/
VALGRIND= yes
PROFILE= no

#///////////////////////////////////////////////////////////////////////////////
# End of lines to be edited.
#///////////////////////////////////////////////////////////////////////////////

VPATH = $(MATRIXSRCDIR):$(LIBNUCNETDIR)
CINCLUDE= `xml2-config --cflags` `gsl-config --cflags` -I$(MATRIXSRCDIR) -I$(LIBNUCNETDIR) -I .
CLIBS= `xml2-config --libs` `gsl-config --libs`

#===============================================================================
# Compiler flags.
#===============================================================================

CFLAGS1= -ansi -Werror -Wall -W -pedantic\
         -Wconversion \
         -Wpointer-arith -Wcast-qual -Wcast-align \
         -Wwrite-strings \
         -fshort-enums -fno-common -g \

ifeq ($(GC), gcc) 
	CFLAGS2= $(CFLAGS1) -Wmissing-prototypes -Wstrict-prototypes -Wnested-externs
else
	CFLAGS2= $(CFLAGS1)
endif

ifeq ($(VALGRIND), yes)
	CFLAGS3= $(CFLAGS2) -O0
else
	CFLAGS3= $(CFLAGS2) -O2
endif

ifeq ($(PROFILE), yes)
	CFLAGS= $(CFLAGS3) -pg
else
	CFLAGS= $(CFLAGS3)
endif

CC=$(GC) $(CFLAGS) $(CINCLUDE)

#===============================================================================
# TMPDIR is the temporary directory for codes compilation, this is where
# object files are created. 
#===============================================================================

TMPDIR = ./tmp/

TMP_DIR := $(shell mkdir -p $(TMPDIR))

#--------------------------------------------------------------------------

NOBJ =  $(TMPDIR)Libnucnet__Nuc.o   \
        $(TMPDIR)WnMatrix.o

WOBJ =  $(TMPDIR)wrapper_nuc.o      \
        $(TMPDIR)user_functions.o

ROBJ =  $(TMPDIR)Libnucnet__Reac.o  \
        $(TMPDIR)user_rate_functions.o

OBJ =   $(NOBJ)                     \
        $(ROBJ)                     \
        $(TMPDIR)remove_duplicate.o \
        $(TMPDIR)Libnucnet.o     

SCOBJ = $(OBJ)                      \
        $(TMPDIR)screen.o           \
        $(TMPDIR)coul_corr.o

$(SCOBJ): $(TMPDIR)%.o: %.c
	$(CC) -c -o $@ $<

$(WOBJ): $(TMPDIR)%.o: %.cpp
	$(CC) -c -o $@ $<

#===============================================================================
# Compile all net codes.
#===============================================================================

EXEC_NUC = \
  create_nuc_collection \
  create_nuc_xml_from_text \
  validate_nuc_xml \
  print_nuclides \
  print_nuclear_energies \
  print_largest_nucleon_numbers \
  sort_nuclides \
  modify_nuc \
  extract_nuc_subset \
  update_nuc_data \
  print_nuc_data_by_name \
  print_nuc_data_by_z_and_a \
  compute_partf_by_name compute_partf_by_z_and_a

.PHONY all_nuc : $(EXEC_NUC)

#===============================================================================
# Compile all reac codes.
#===============================================================================

EXEC_REAC = \
  create_reac_xml_from_text \
  validate_reac_xml \
  print_reactions \
  number_reactions_in_view \
  remove_dup_reactions \
  update_reac_data \
  print_dup_factors \
  print_reactants_and_products \
  print_reac_data_by_string \
  print_reac_data_by_xpath \
  print_rates_at_t9 \
  print_rates_by_string \
  print_rates_by_xpath

.PHONY all_reac : $(EXEC_REAC)
     
#===============================================================================
# Compile all libnucnet codes.
#===============================================================================

EXEC = \
  print_valid_reactions \
  merge_net \
  print_Qvalues \
  print_forward_and_reverse_at_t9 \
  print_forward_and_reverse_by_string \
  print_forward_and_reverse_by_xpath \
  check_rates \
  double_rates \
  merge_full \
  create_zones \
  print_zone_data \
  print_zone_mass_fractions_by_labels \
  print_zone_quantum_abunds \
  print_nuclide_mass_fractions_in_zones \
  star_read \
  extract_zone_nuclide_subset \
  average_zones

EXEC_SCREEN = \
  print_screened_rates \
  print_zone_screening \
  print_jacobian \
  run_single_zone \
  run_mod_rate_single_zone \
  run_table_single_zone \
  run_multi_zone 

.PHONY all_libnucnet : $(EXEC) $(EXEC_SCREEN)

#===============================================================================
# Compile all.
#===============================================================================

all : all_nuc all_reac all_libnucnet
.PHONY : all
     
#===============================================================================
# Compile codes depending only on Libnucnet__Nuc.
#===============================================================================

$(EXEC_NUC) : $(NOBJ) $(WOBJ)
	$(CC) $(NOBJ) $(WOBJ) $(CLIBS) -o $@ $@.cpp

#===============================================================================
# Compile codes depending only on Libnucnet__Reac.
#===============================================================================

$(EXEC_REAC) : $(ROBJ)
	$(CC) -c -o $(TMPDIR)$@.o $@.c
	$(CC) $(ROBJ) $(TMPDIR)$@.o $(CLIBS) -o $@

#===============================================================================
# Compile codes depending on Libnucnet__Nuc, Libnucnet__Reac, and Libnucnet.
#===============================================================================

$(EXEC) : $(OBJ)
	$(CC) -c -o $(TMPDIR)$@.o $@.c
	$(CC) $(OBJ) $(TMPDIR)$@.o $(CLIBS) -o $@

$(EXEC_SCREEN) : $(SCOBJ)
	$(CC) -c -o $(TMPDIR)$@.o $@.c
	$(CC) $(SCOBJ) $(TMPDIR)$@.o $(CLIBS) -o $@

#===============================================================================
# Get data.
#===============================================================================

DATA_VERSION=2011-05-28

data:
	mkdir -p ../data_pub
	wget http://libnucnet.sourceforge.net/data_pub/$(DATA_VERSION)/data.tar.gz -O ../data_pub/data.tar.gz
	tar xz -C ../data_pub -f ../data_pub/data.tar.gz

#===============================================================================
# Clean up.
#===============================================================================

.PHONY: clean cleanall

clean: 
	rm -fr $(TMPDIR)

cleanall: clean
	rm -f $(EXEC_NUC) $(EXEC_REAC) $(EXEC) $(EXEC_SCREEN) *.exe

clean_data:
	rm -fr ../data_pub
