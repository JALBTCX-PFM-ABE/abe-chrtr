
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
*   Programmer(s):      Dominic Avery, Jan C. Depner                        *
*                                                                           *
*   Date Written:       April 1992                                          *
*                                                                           *
*   Module Name:        getparam                                            *
*                                                                           *
*   Module Security                                                         *
*   Classification:     Unclassified                                        *
*                                                                           *
*   Data Security                                                           *
*   Classification:     Unknown                                             *
*                                                                           *
*   Purpose:            Input all parameters used to create the CHRTR       *
*                       file and display their value on the console.        *
*                                                                           *
*   Inputs:             gridmin             - Grid interval in minutes.     *
*                       delta               - The value of the maximum      *
*                                             change in the surface between *
*                                             iterations at which           *
*                                             convergence is accepted in    *
*                                             regional computation (see     *
*                                             iterate function).            *
*                                             (default = 0.05).             *
*                       reg_multfact        - Multiplicative factor for     *
*                                             initial coarse grid in        *
*                                             regional computation          *
*                                             (default = 4).                *
*                       search_radius       - Maximum distance from a grid  *
*                                             point that data points are    *
*                                             to be used for regional       *
*                                             computation.  This is used in *
*                                             the weight_mean function.     *
*                                             (default = 20.0)              *
*                       error_control       - Number of times to retry      *
*                                             min_curve after increasing    *
*                                             search_radius.                *
*                       border_points       - Border overlap in grid        *
*                                             points.                       *
*                       maxvalue            - Largest acceptable data       *
*                                             value (default = 999999.0).   *
*                       minvalue            - Smallest acceptable data      *
*                                             value (default = -999999.0).  *
*                       weight_factor       - Weight factor for merging     *
*                                             the regional field with the   *
*                                             original sparse input data.   *
*                                             The weight factor may be set  *
*                                             to 1,2, or 3.  The larger the *
*                                             value, the more weight that   *
*                                             is put on the input data.     *
*                                             (default = 2).                *
*                       output_reg          - Option to output regional     *
*                                             file (default = 0).           * 
*                                               0 - do not output regional. *
*                                               1 - output regional.        *
*                       prnt_option         - Option for type of output     *
*                                             (default = 0).                *
*                                               0 - no list.                *
*                                               1 - list records that are   *
*                                                   outside of grid limits  *
*                                                   of acceptable data      *
*                                                   value range.            *
*                                               2 - print good records.     *
*                       latdeg_south        - Latitude degrees south.       *
*                       latmin_south        - Latitude minutes south.       *
*                       latdeg_north        - Latitude degrees north.       *
*                       latmin_north        - Latitude minutes north.       *
*                       londeg_west         - Longitude degrees west.       *
*                       lonmin_west         - Longitude minutes west.       *
*                       londeg_east         - Longitude degrees east.       *
*                       lonmin_east         - Longitude minutes east.       *
*                                                                           *
*   Outputs:            none                                                *
*                                                                           *
*   Calling Routines:   main                                                *
*                                                                           * 
*   Glossary:           index1              - utility integer               *
*                       index2              - utility integer               *
*                       strlength           - length of input string        *
*                       parameter1          - input parameter string        *
*                       parameter2          - input parameter string        *
*                                                                           *
*   Method:             Input all parameters until the parameter "END" is   *
*                       encountered.  After inputing all the parameters,    *
*                       output their current value to the console.          *
*                                                                           *
*   Restrictions:       Parameters must be input one per line in the form   *
*                       parameter name = value                              *
*                                                                           * 
\***************************************************************************/

#include "nvutility.h"

void getparam (double *gridmin, double *delta, int32_t *reg_multfact, double *search_radius, 
               int32_t *error_control, double *maxvalue, double *minvalue,
               int32_t *weight_factor, double *latdeg_south, double *latmin_south,
               double *latdeg_north, double *latmin_north, double *londeg_west, double *lonmin_west,
               double *londeg_east, double *lonmin_east, int32_t *nibble)
{
  int32_t      index1 = 0, index2 = 0, strlength;
  char         parameter1[512], parameter2[512];

            
  /*  Loop through all the parameters until "end" is encountered.     */

  memset (parameter1, 0, sizeof (parameter1));

  while (strstr (parameter1, "end") == NULL) 
    {
      ngets (parameter1, sizeof (parameter1), stdin);

      /*  Convert all uppercase characters within the string to       */
      /*  lowercase.                                                  */

      strlength = strlen (parameter1);
      for (index1 = 0; index1 < strlength; index1++) *(parameter1 + index1) = tolower(*(parameter1 + index1));

      /*  Remove all the blanks from the parameter string.            */

      index2 = 0;
      memset (parameter2, 0, sizeof (parameter2));

      for (index1 = 0; index1 < strlength; index1++)
        {
          if (*(parameter1 + index1) != ' ')
            {
              *(parameter2 + index2) = *(parameter1 + index1);
              index2++;
            }
        }
      strcpy (parameter1, parameter2);


      /*  Convert all the input parameters from a character string to */
      /*  either a floating or integer type.                          */

      if (strstr (parameter1, "delta") != NULL) sscanf (strstr (parameter1, "=") + 1, "%lf", delta);

      if (strstr (parameter1, "error_control") != NULL) sscanf (strstr (parameter1, "=") + 1, "%d", error_control);

      if (strstr (parameter1, "grid") != NULL) sscanf (strstr (parameter1, "=") + 1, "%lf", gridmin);

      if (strstr (parameter1, "latdeg_south") != NULL) sscanf (strstr (parameter1, "=") + 1, "%lf", latdeg_south);

      if (strstr (parameter1, "latmin_south") != NULL) sscanf (strstr (parameter1, "=") + 1, "%lf", latmin_south);

      if (strstr (parameter1, "latdeg_north") != NULL) sscanf (strstr (parameter1, "=") + 1, "%lf", latdeg_north);

      if (strstr (parameter1, "latmin_north") != NULL) sscanf (strstr (parameter1, "=") + 1, "%lf", latmin_north);

      if (strstr (parameter1, "londeg_west") != NULL) sscanf (strstr (parameter1, "=") + 1, "%lf", londeg_west);

      if (strstr (parameter1, "lonmin_west") != NULL) sscanf (strstr (parameter1, "=") + 1, "%lf", lonmin_west);

      if (strstr (parameter1, "londeg_east") != NULL) sscanf (strstr (parameter1, "=") + 1, "%lf", londeg_east);

      if (strstr (parameter1, "lonmin_east") != NULL) sscanf (strstr (parameter1, "=") + 1, "%lf", lonmin_east);

      if (strstr (parameter1, "maxvalue") != NULL) sscanf (strstr (parameter1, "=") + 1, "%lf", maxvalue);

      if (strstr (parameter1, "minvalue") != NULL) sscanf (strstr (parameter1, "=") + 1, "%lf", minvalue);

      if (strstr (parameter1, "search_radius") != NULL) sscanf (strstr (parameter1, "=") + 1, "%lf", search_radius);

      if (strstr (parameter1, "reg_multfact") != NULL) sscanf (strstr (parameter1, "=") + 1, "%d", reg_multfact);

      if (strstr (parameter1, "weight_factor") != NULL) sscanf (strstr (parameter1, "=") + 1, "%d", weight_factor);

      if (strstr (parameter1, "nibble_value") != NULL) sscanf (strstr (parameter1, "=") + 1, "%d", nibble);      
    }

  /*  Display all the input parameters to the console.                */

  printf ("gridmin           = %f\n", *gridmin);
  printf ("delta             = %f\n", *delta);
  printf ("reg_multfact      = %d\n", *reg_multfact);
  printf ("search_radius     = %f\n", *search_radius);
  printf ("error_control     = %d\n", *error_control);
  printf ("weight_factor     = %d\n", *weight_factor);
  printf ("maxvalue          = %f\n", *maxvalue);
  printf ("minvalue          = %f\n", *minvalue);
  printf ("latdeg_south      = %.9f\n", *latdeg_south);
  printf ("latmin_south      = %.6f\n", *latmin_south);
  printf ("latdeg_north      = %.9f\n", *latdeg_north);
  printf ("latmin_north      = %.6f\n", *latmin_north);
  printf ("londeg_west       = %.9f\n", *londeg_west);
  printf ("lonmin_west       = %.6f\n", *lonmin_west);
  printf ("londeg_east       = %.9f\n", *londeg_east);
  printf ("lonmin_east       = %.6f\n", *lonmin_east);
  printf ("nibble_value      = %d\n\n\n", *nibble);
  return;
}
