
/*********************************************************************************************

    This is public domain software that was developed by or for the U.S. Naval Oceanographic
    Office and/or the U.S. Army Corps of Engineers.

    This is a work of the U.S. Government. In accordance with 17 USC 105, copyright protection
    is not available for any work of the U.S. Government.

    Neither the United States Government, nor any employees of the United States Government,
    nor the author, makes any warranty, express or implied, without even the implied warranty
    of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE, or assumes any liability or
    responsibility for the accuracy, completeness, or usefulness of any information,
    apparatus, product, or process disclosed, or represents that its use would not infringe
    privately-owned rights. Reference herein to any specific commercial products, process,
    or service by trade name, trademark, manufacturer, or otherwise, does not necessarily
    constitute or imply its endorsement, recommendation, or favoring by the United States
    Government. The views and opinions of authors expressed herein do not necessarily state
    or reflect those of the United States Government, and shall not be used for advertising
    or product endorsement purposes.

*********************************************************************************************/

/***************************************************************************\
*                                                                           *
*   Programmer:         Jan C. Depner                                       *
*                                                                           *
*   Date Written:       April 2006                                          *
*                                                                           *
*   Purpose:            This program produces a uniform grid at a specified *
*                       grid spacing from irregularly spaced input data.    *
*                       This program is a combination of work by a number   *
*                       of people, most recently, myself and Dominic Avery. *
*                       In addition, quite a few people have added fixes    *
*                       to the routines that this program relies upon.      *
*                       These include Stephanie Lee (PSI), Steve Nosalik,   *
*                       Bill Rankin and a bunch of others.  If you'd like   *
*                       to see a bit more about the history of this program *
*                       and associated functions take a look at the         *
*                       comments in the misp_funcs.c file in the MISP       *
*                       library.                                            *
*                                                                           *
\***************************************************************************/

#include "nvutility.h"

#include "misp.h"

#include "version.h"


int32_t main (int32_t argn, char *argv[])
{
  FILE          *finptr, *chp_fp;

  int32_t       i, j, k, m, error_control, gridcols, gridrows, reg_multfact, weight_factor, dn, up, bw, fw, 
                numfiles, out_of_area, num_points, endian, size, nibble, percent, old_percent, tmp_i;

  double        delta, gridmin, y_gridmin, x_gridmin, latdeg_south, latmin_south, latdeg_north, center_x, center_y,
                latmin_north, londeg_west, lonmin_west, londeg_east, lonmin_east, latminutes_south, latminutes_north,
                lonminutes_west, lonminutes_east, latdecdeg_south, latdecdeg_north, londecdeg_west, londecdeg_east,
                maxvalue, minvalue, search_radius, tmp_pos, x, y, dist, az, in_gridmin = 0.0, in_gridmeter = 0.0;

  float         *array, z_min, z_max, tmp, null_val, **val_array;

  uint8_t       ScR_flag, input_file_flag, force_original_value = NVFalse, nominal = NVFalse, dateline, found;

  NV_F64_XYMBR  mbr;

  NV_F64_COORD3 xyz;

  char          finfile[512], *input_filenames[4000], chp_file[512], varin[1024], info[1024];


  void getparam (double *, double *, int32_t *, double *, int32_t *, double *, double *, int32_t *, double *, double *,
                 double *, double *, double *, double *, double *, double *, int32_t *);
  int32_t reader (NV_F64_COORD3 *, int32_t, char *[], int32_t, uint8_t);
  int32_t ScR_reader (NV_F64_COORD3 *, char *[], int32_t);
  void loadfiles (char *[], int32_t *);



  printf ("\n\n %s \n\n", VERSION);



  /*  Set default values.  */

  error_control = 20;
  reg_multfact = 4;
  weight_factor = 2;
  delta = 0.05;
  gridmin = 1.0;
  maxvalue = 999999.0;
  minvalue = -999999.0;
  search_radius = 20.0;
  latmin_south = 0.0;
  latmin_north = 0.0;
  lonmin_west = 0.0;
  lonmin_east = 0.0;
  ScR_flag = NVFalse;
  out_of_area = 0;
  num_points = 0;
  null_val = CHRTRNULL;
  bit_set (&null_val, 0, 0);

  xyz.x = xyz.y = xyz.z = 0.0;


  /*  If there is an argument to the program it will be a chrtrGUI parameter file.   */

  if (argn > 1) 
    {
      strcpy (chp_file, argv[1]);

      if ((chp_fp = fopen (chp_file, "r")) == NULL)
        {
          perror (chp_file);
          exit (-1);
        }

      input_file_flag = NVFalse;
      numfiles = 0;
      while (ngets (varin, sizeof (varin), chp_fp) != NULL)
        {
        
          /*  Put everything to the right of the equals sign in 'info'.   */

          if (strchr (varin, '=') != NULL) strcpy (info, (strchr (varin, '=') + 1));

          if (strstr (varin, "[gridmin]")) sscanf (info, "%lf", &in_gridmin);
          if (strstr (varin, "[gridmeter]")) sscanf (info, "%lf", &in_gridmeter);
          if (strstr (varin, "[delta]")) sscanf (info, "%lf", &delta);
          if (strstr (varin, "[reg_mutfact]")) sscanf (info, "%d", &reg_multfact);
          if (strstr (varin, "[search_radius]")) sscanf (info, "%lf", &search_radius);
          if (strstr (varin, "[error_control]")) sscanf (info, "%d", &error_control);
          if (strstr (varin, "[weight_factor]")) sscanf (info, "%d", &weight_factor);
          if (strstr (varin, "[force_original_value]"))
	    {
	      sscanf (info, "%d", &tmp_i);
	      force_original_value = (uint8_t) tmp_i;
	    }
          if (strstr (varin, "[nibble_value]")) sscanf (info, "%d", &nibble);

          if (strstr (varin, "[nominal_depth]"))
	    {
	      sscanf (info, "%d", &tmp_i);
	      nominal = (uint8_t) tmp_i;
	    }
          if (strstr (varin, "[minvalue]")) sscanf (info, "%lf", &minvalue);
          if (strstr (varin, "[maxvalue]")) sscanf (info, "%lf", &maxvalue);
          if (strstr (varin, "[lat_south]")) 
            {
              posfix (info, &tmp_pos, POS_LAT);
              latdeg_south = tmp_pos;
            }
          if (strstr (varin, "[lat_north]"))
            {
              posfix (info, &tmp_pos, POS_LAT);
              latdeg_north = tmp_pos;
            }
          if (strstr (varin, "[lon_west]"))
            {
              posfix (info, &tmp_pos, POS_LON);
              londeg_west = tmp_pos;
            }
          if (strstr (varin, "[lon_east]"))
            {
              posfix (info, &tmp_pos, POS_LON);
              londeg_east = tmp_pos;
            }
          if (strstr (varin, "[output_file]")) get_string (varin, finfile);

          if (input_file_flag)
            {
              if (strstr (varin, "**  End Input Files  **")) break;

              input_filenames[numfiles] = (char *) malloc (strlen (varin) + 1);
              strcpy (input_filenames[numfiles], varin);
              numfiles++;
            }

          if (strstr (varin, "**  Input Files  **")) input_file_flag = NVTrue;
        }

      if (force_original_value) weight_factor = -weight_factor;


      /*  If we have meters input instead of minutes, Convert meters to the equivalent minutes of longitude at the
          center of the area.  */

      if (in_gridmeter > 0.00001)
        {
          /*  Convert from meters.    */

          center_x = londeg_west + (londeg_east - londeg_west) / 2.0;
          center_y = latdeg_south + (latdeg_north - latdeg_south) / 2.0;


          newgp (center_y, center_x, 90.0, in_gridmeter, &y, &x);


          gridmin = (x - center_x) * 60.0;
        }
      else
        {
          gridmin = in_gridmin;
        }
    }
  else
    {
    
      /*  Input all the parameters used to create a final file.  */

      printf ("Enter input parameters, enter end to stop:\n");
      getparam (&gridmin, &delta, &reg_multfact, &search_radius, &error_control,
                &maxvalue, &minvalue, &weight_factor, 
                &latdeg_south, &latmin_south, &latdeg_north,
                &latmin_north, &londeg_west, &lonmin_west, &londeg_east, &lonmin_east, &nibble);
    }


  /*  Convert chart boundaries to minutes and decimal degrees.  */

  dateline = NVFalse;
  if (latdeg_south < 0.0) latmin_south = - (fabs ((double) latmin_south));
  if (latdeg_north < 0.0) latmin_north = - (fabs ((double) latmin_north));
  if (londeg_west < 0.0) lonmin_west = - (fabs ((double) lonmin_west));
  if (londeg_east < 0.0) lonmin_east = - (fabs ((double) lonmin_east));

  latminutes_south = (latdeg_south * 60.0) + latmin_south;
  latminutes_north = (latdeg_north * 60.0) + latmin_north;
  lonminutes_west  = (londeg_west * 60.0) + lonmin_west;
  lonminutes_east  = (londeg_east * 60.0) + lonmin_east;
  latdecdeg_south  = latminutes_south / 60.0;
  latdecdeg_north  = latminutes_north / 60.0;
  londecdeg_west   = lonminutes_west / 60.0;
  londecdeg_east   = lonminutes_east / 60.0;


  /*  Adjust the boundaries if the chart crosses over the dateline.  */

  if (lonminutes_west >= lonminutes_east)
    {
      lonminutes_east += 21600.0;
      dateline = NVTrue;
    }


  /*  Calculate grid rows of final file.  */

  y_gridmin = gridmin;
  x_gridmin = gridmin;
  gridrows = (int32_t) ((latminutes_north - latminutes_south) / y_gridmin + 0.5);


  /*  WARNING - the following code is non-functional (you can't get lats larger than 90.0).  We may use this in the
      future for doing proportional grids.  */

  /*****************************************************************************************************************/

  /*  Calculate proportional X grid size based on Y grid size at southern boundary.  If we're in the 
      southern hemisphere we'll use the Y grid size at the northern boundary.  In this way we'll have 
      a minimum bin size that is approximately square (spatially).  */

  if (latdecdeg_south >= 90.0 || latdecdeg_north <= -90.0)
    {
      if (latdecdeg_north < 0.0)
	{
	  invgp (NV_A0, NV_B0, latdecdeg_north, londecdeg_west, latdecdeg_north - (y_gridmin / 60.0), londecdeg_west,
                 &dist, &az);
	}
      else
	{
	  invgp (NV_A0, NV_B0, latdecdeg_south, londecdeg_west, latdecdeg_south + (y_gridmin / 60.0), londecdeg_west,
                 &dist, &az);
	}

      newgp (latdecdeg_south, londecdeg_west, 90.0, dist, &y, &x);

      x_gridmin = (x - londecdeg_west) * 60.0;
    }

  /*****************************************************************************************************************/


  gridcols = (int32_t) ((lonminutes_east - lonminutes_west) / x_gridmin + 0.5);


  /*  Get output filename if it's on the command line.  */

  if (argn < 2)
    {
      printf ("Enter file name for output grid file: \n");
      ngets (finfile, sizeof (finfile), stdin);
    }


  if (strstr (finfile, ".fin") == NULL) strcat (finfile, ".fin");


  /*  Read in the input filenames.    */
    
  if (argn < 2) loadfiles (input_filenames, &numfiles);


  /*  Check the first file for the SCRBLR data type extension, set the flag if it is.  */

  if (strstr (input_filenames[0], ".ScR") != NULL) ScR_flag = 1;



  /*  We're going to let MISP handle everything in zero based units of the bin size.  That is, we subtract off the
      west lon from longitudes then divide by the grid size in the X direction.  We do the same with the latitude using
      the south latitude.  This will give us values that range from 0.0 to gridcols in longitude and 0.0 to gridrows
      in latitude.  This allows us to use bins that are spatially nearly square but in terms of latitude and longitude
      would be stretched rectangles.  We'll use this to allow us to use different latitudinal and longitudinal bin sizes
      for areas in the extreme latitudes.  */

  mbr.min_x = 0.0;
  mbr.min_y = 0.0;
  mbr.max_x = (double) gridcols;
  mbr.max_y = (double) gridrows;


  /*  Initialize the MISP engine.  */


  if (misp_init (1.0, 1.0, (float) delta, reg_multfact, (float) search_radius, error_control, 
                 (float) maxvalue, (float) minvalue, weight_factor, mbr)) return (-1);


  /*  Load all the data from the given input files.  */

  while (1)
    {
      /*  If the SCRBLR flag is set, read in SCRBLR format files.  */

      if (ScR_flag)
        {
          if (ScR_reader (&xyz, input_filenames, numfiles)) break;
        }
      else
        {
          if (reader (&xyz, dateline, input_filenames, numfiles, nominal)) break;
        }


      /*  Move the lat and lon minutes into the grid domain.  */

      xyz.x = (xyz.x - lonminutes_west) / x_gridmin;
      xyz.y = (xyz.y - latminutes_south) / y_gridmin;


      /*  Load data and check for out of area conditions.  */

      if (!misp_load (xyz))
        {
          out_of_area++;
        }
      else
        {
          num_points++;
        }
    }


  if (num_points == 0)
    {
      fprintf (stderr, "\n\nNo data points within specified bounds.\n");
      fprintf (stderr, "Check input area boundaries and/or min and max values.\n");
      fprintf (stderr, "Terminating!\n\n");
      exit (-1);
    }


  /*  Bump and grind!  */

  misp_proc ();



  /*  Make chart width and height inclusive prior to retrieval.      */

  gridcols++;
  gridrows++;


  finptr = fopen (finfile, "wb+");
  printf ("\n\nOutput file: %s\n", finfile);


  /*  Pad the first record to keep the FORTRAN geeks happy.  Write it out to save the header slot in the file.  */

  j = 0;
  for (i = 0 ; i < gridcols ; i++) fwrite (&j, sizeof (int32_t), 1, finptr);


  /*  Process all the rows for the grid.  */

  size = gridcols * sizeof (float);

  array = (float *) malloc (size);

  if (array == NULL)
    {
      perror ("Allocating retrieval array");
      exit (-1);
    }


  z_min = maxvalue + 1.0;
  z_max = minvalue - 1.0;

  while (misp_rtrv (array))
    {
      for (i = 0 ; i < gridcols ; i++) 
        {
          if (array[i] < z_min) z_min = array[i];
          if (array[i] > z_max) z_max = array[i];
        }

      fwrite (array, size, 1, finptr);
    }


  /*  Stop!  Nibble time!  */

  if (nibble)
    {
      /*  Nibble out the cells that aren't within our optional nibbling distance from a cell with valid data.  */

      /*  This can get into a fair amount of memory but it is the most efficient way to do this since we're doing, 
	  basically, sequential I/O on both ends.  If you have a 2000 by 2000 cell bin this will allocate 16MB of
	  memory.  Hopefully your machine can handle that.  If not, buy a new machine.  */

      val_array = (float **) calloc (gridrows, sizeof (float *));

      if (val_array == NULL)
        {
          perror ("Allocating val_array");
          exit (-1);
        }



      fprintf(stderr,"\n\nNibbling                                                         \n\n");


      for (i = 0 ; i < gridrows ; i++)
        {
          val_array[i] = (float *) calloc (gridcols, sizeof (float));

          if (val_array[i] == NULL)
            {
              perror ("Allocating memory for nibbler");
              exit (-1);
            }
        }


      percent = 0;
      old_percent = -1;

      for (i = 0 ; i < gridrows ; i++)
        {
	  fseek (finptr, (i + 1) * gridcols * sizeof (float), SEEK_SET);

	  if (!fread (val_array[i], sizeof (float), gridcols, finptr))
	    {
	      fprintf (stderr, "Bad return in file %s, function %s at line %d.  This should never happen!", __FILE__, __FUNCTION__, __LINE__ - 2);
	      fflush (stderr);
	      exit (-1);
	    }

          percent = ((float) (i) / (float) gridrows) * 100.0;
	  if (old_percent != percent) 
            {
              fprintf (stderr, "%03d%% read             \r", percent);
              fflush (stderr);
              old_percent = percent;
            }
	}


      percent = 0;
      old_percent = -1;

      for (i = 0 ; i < gridrows ; i++)
        {
          dn = MAX (i - nibble, 0);
          up = MIN (i + nibble, gridrows - 1);

          for (j = 0 ; j < gridcols ; j++)
            {
              if (!bit_test (val_array[i][j], 0))
                {
                  bw = MAX (j - nibble, 0);
                  fw = MIN (j + nibble, gridcols - 1);

                  found = NVFalse;
                  for (k = dn ; k <= up ; k++)
                    {
                      for (m = bw ; m <= fw ; m++)
                        {
                          if (bit_test (val_array[k][m], 0))
                            {
                              found = NVTrue;
                              break;
                            }
                        }
                      if (found) break;
                    }

                  if (!found)
                    {
                      val_array[i][j] = null_val;
                    }
                }
            }

          percent = ((float) (i) / (float) gridrows) * 100.0;
	  if (old_percent != percent) 
            {
              fprintf (stderr, "%03d%% nibbled             \r", percent);
              fflush (stderr);
              old_percent = percent;
            }
        }


      percent = 0;
      old_percent = -1;
      z_min = CHRTRNULL;
      z_max = -CHRTRNULL;


      for (i = 0 ; i < gridrows ; i++)
        {
	  fseek (finptr, (i + 1) * gridcols * sizeof (float), SEEK_SET);

	  fwrite (val_array[i], sizeof (float), gridcols, finptr);


	  /*  We need to recompute the mins and maxes.  */

	  for (j = 0 ; j < gridcols ; j++)
	    {
	      if (val_array[i][j] < CHRTRNULL)
		{
		  if (val_array[i][j] < z_min) z_min = val_array[i][j];
		  if (val_array[i][j] > z_max) z_max = val_array[i][j];
		}
	    }


          percent = ((float) (i) / (float) gridrows) * 100.0;
          if (old_percent != percent) 
            {
              fprintf (stderr, "%03d%% written             \r", percent);
              fflush (stderr);
              old_percent = percent;
            }
        }


      for (i = 0 ; i < gridrows ; i++) free (val_array[i]);
      free (val_array);

      fprintf (stderr, "Completed                      \n");
    }


  /*  Write the header.  */

  fseek (finptr, 0, SEEK_SET);

  endian = 0x00010203;

  tmp = londecdeg_west;
  fwrite (&tmp, sizeof (float), 1, finptr);
  tmp = londecdeg_east;
  fwrite (&tmp, sizeof (float), 1, finptr);
  tmp = latdecdeg_south;
  fwrite (&tmp, sizeof (float), 1, finptr);
  tmp = latdecdeg_north;
  fwrite (&tmp, sizeof (float), 1, finptr);
  tmp = gridmin;
  fwrite (&tmp, sizeof (float), 1, finptr);
  fwrite (&gridcols, sizeof (int), 1, finptr);
  fwrite (&gridrows, sizeof (int), 1, finptr);
  fwrite (&endian, sizeof (int), 1, finptr);
  fwrite (&z_min, sizeof (float), 1, finptr);
  fwrite (&z_max, sizeof (float), 1, finptr);

  fclose (finptr);


  printf ("\n\n       **%6.4f**\n", latdecdeg_north);
  printf ("       *        *\n");
  printf ("       *        *\n");
  printf ("    %6.4f   %6.4f\n", londecdeg_west, londecdeg_east);
  printf ("       *        *\n");
  printf ("       *        *\n");
  printf ("       **%6.4f**\n\n", latdecdeg_south);


  return (0);
}   
