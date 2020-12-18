
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
*   Programmer(s):      Jan C. Depner                                       *
*                                                                           *
*   Date Written:       May 1994                                            *
*                                                                           *
*   Last Modification:                                                      *
*                                                                           *
*   Module Name:        ScR_reader                                          *
*                                                                           *
*   Module Security                                                         *
*   Classification:     Unclassified                                        *
*                                                                           *
*   Data Security                                                           *
*   Classification:     Unknown                                             *
*                                                                           *
*   Purpose:            Input all the data from the given files in the      *
*                       input parameter list.  These are ISCRBLR format     *
*                       files.                                              *
*                                                                           *
*   Inputs:             lat             -   latitude value to pass to       *
*                                           caller                          *
*                       lon             -   longitude value to pass to      *
*                                           caller                          *
*                       zvalue          -   z value to pass to caller       *
*                       input_filenames -   array of pointers to input      *
*                                           filenames                       *
*                       num_files       -   number of input filenames       *
*                                                                           *
*   Outputs:            int             -   1 on end of last data file      *
*                                                                           *
*   Calling Routines:   main                                                *
*                                                                           * 
*   Glossary:           fileptr         -   Pointer for the current file    *
*                                           being processed.                *
*                       firstfile       -   Indicate that the first file is *
*                                           being processed.                *
*                                                                           *
\***************************************************************************/

#include "nvutility.h"

int32_t ScR_reader (NV_F64_COORD3 *xyz, char *input_filenames[], int32_t numfiles)
{
  static  FILE         *fileptr;
  static  int32_t      firstfile = 1;
  float                in[3];

  int32_t open_ScR_files (FILE **, char *[], int32_t);
  

  /* Input and open a file.                                           */
    
  if (firstfile)
    {
      firstfile = open_ScR_files (&fileptr, input_filenames, numfiles);
      if (firstfile) return (1);
    }

  if (!fread (in, sizeof (in), 1, fileptr))
    {
      firstfile = open_ScR_files (&fileptr, input_filenames, numfiles);
      if (firstfile) return (1);
      if (!fread (in, sizeof (in), 1, fileptr))
	{
	  fprintf (stderr, "Bad return in file %s, function %s at line %d.  This should never happen!", __FILE__, __FUNCTION__, __LINE__ - 2);
	  fflush (stderr);
	  exit (-1);
	}
    }

  xyz->y = in[0];
  xyz->x = in[1];
  xyz->z = in[2];
    
  return (0);
}



/***************************************************************************\
*                                                                           *
*   Programmer(s):      Jan C. Depner                                       *
*                                                                           *
*   Date Written:       May 1994                                            *
*                                                                           *
*   Module Name:        open_ScR_files                                      *
*                                                                           *
*   Module Security                                                         *
*   Classification:     Unclassified                                        *
*                                                                           *
*   Data Security                                                           *
*   Classification:     Unknown                                             *
*                                                                           *
*   Purpose:            Open each input file.                               *
*                                                                           *
*   Inputs:             fileptr         -   file pointer                    *
*                       input_filenames -   array of input filenames        *
*                       num_files       -   number of input filenames       *
*                                                                           *
*   Outputs:            int32_t          -   1 on end of last file               *
*                                                                           *
*   Calling Routines:   ScR_reader                                          *
*                                                                           * 
*   Glossary:           filecount   - Counter for number of file.           *
*                                                                           *
\***************************************************************************/

int32_t open_ScR_files (FILE **fileptr, char *input_filenames[], int32_t numfiles)
{
  char             file[512];
  static int32_t   filecount = 0;


  if (*fileptr) fclose(*fileptr);

  if (filecount == numfiles) return(1);

  strcpy (file, input_filenames[filecount]);
    

  /* Open an input file.                                              */

  if ((*fileptr = fopen (file, "r")) == NULL)
    {
      perror (file);
      exit (-1);
    }

  filecount++;
  return (0);
}
