/*
===============================================================================

  FILE:  laswriter.hpp
  
  CONTENTS:
  
    Interface to write LIDAR points to the LAS format versions 1.0 - 1.3 and
    per on-the-fly conversion to simple ASCII files.

  PROGRAMMERS:

    martin.isenburg@rapidlasso.com  -  http://rapidlasso.com

  COPYRIGHT:

    (c) 2007-2012, martin isenburg, rapidlasso - tools to catch reality

    This is free software; you can redistribute and/or modify it under the
    terms of the GNU Lesser General Licence as published by the Free Software
    Foundation. See the LICENSE.txt file for more information.

    This software is distributed WITHOUT ANY WARRANTY and without even the
    implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
  
  CHANGE HISTORY:
  
    5 September 2011 -- support for writing Terrasolid's BIN format
    11 June 2011 -- billion point support: p_count & npoints are 64 bit counters
    8 May 2011 -- added an option for variable chunking via chunk()
    9 April 2011 -- added capability to write on-the-fly conversion to ASCII
    24 January 2011 -- introduced LASwriteOpener
    21 January 2011 -- turned into abstract reader to support multiple files
    3 December 2010 -- updated to (somewhat) support LAS format 1.3
    7 September 2008 -- updated to support LAS format 1.2 
    21 February 2007 -- created after eating Sarah's veggies with peanutsauce
  
===============================================================================
*/
#ifndef LAS_WRITER_HPP
#define LAS_WRITER_HPP

#include "lasdefinitions.hpp"

#include "lasutility.hpp"

class LASwriter
{
public:
  LASquantizer quantizer;
  I64 npoints;
  I64 p_count;
  LASinventory inventory;

  virtual BOOL write_point(const LASpoint* point) = 0;
  virtual void update_inventory(const LASpoint* point) { inventory.add(point); };
  virtual BOOL chunk() = 0;

  virtual BOOL update_header(const LASheader* header, BOOL use_inventory=TRUE, BOOL update_extra_bytes=FALSE) = 0;
  virtual I64 close(BOOL update_npoints=TRUE) = 0;

  LASwriter() { npoints = 0; p_count = 0; };
  virtual ~LASwriter() {};
};

#include "laswaveform13writer.hpp"

class LASwriteOpener
{
public:
  void set_directory(const CHAR* directory);
  void set_file_name(const CHAR* file_name);
  void set_appendix(const CHAR* appendix);
  void set_cut(U32 cut);
  BOOL set_format(I32 format);
  BOOL set_format(const CHAR* format);
  void set_force(BOOL force);
  void set_chunk_size(U32 chunk_size);
  void make_numbered_file_name(const CHAR* file_name, I32 digits);
  void make_file_name(const CHAR* file_name, I32 file_number=-1);
  const CHAR* get_directory() const;
  const CHAR* get_file_name() const;
  const CHAR* get_appendix() const;
  CHAR* get_file_name_base() const;
  U32 get_cut() const;
  BOOL format_was_specified() const;
  I32 get_format() const;
  const CHAR* get_format_name() const;
  void set_parse_string(const CHAR* parse_string);
  void set_separator(const CHAR* separator);
  void set_scale_rgb(F32 scale_rgb);
  void usage() const;
  BOOL parse(int argc, char* argv[]);
  BOOL active() const;
  BOOL is_piped() const;
  LASwriter* open(LASheader* header);
  LASwaveform13writer* open_waveform13(const LASheader* lasheader);
  LASwriteOpener();
  ~LASwriteOpener();
private:
  void add_directory(const CHAR* directory=0);
  void add_appendix(const CHAR* appendix=0);
  void cut_characters(U32 cut=0);
  CHAR* directory;
  CHAR* file_name;
  CHAR* appendix;
  U32 cut;
  BOOL opts;
  BOOL optx;
  CHAR* parse_string;
  CHAR* separator;
  F32 scale_rgb;
  U32 format;
  BOOL specified;
  BOOL force;
  U32 chunk_size;
  BOOL use_chunking;
  BOOL use_stdout;
  BOOL use_nil;
  BOOL buffered;
  BOOL use_v1;
};

#endif