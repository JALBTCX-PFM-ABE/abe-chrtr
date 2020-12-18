
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
*   Date Written:       April 1994                                          *
*                                                                           *
*   Module Name:        loadfiles                                           *
*                                                                           *
*   Module Security                                                         *
*   Classification:     Unclassified                                        *
*                                                                           *
*   Data Security                                                           *
*   Classification:     Unknown                                             *
*                                                                           *
*   Purpose:            Input all the files from the parameter list.        *
*                                                                           *
*   Inputs:             input_filenames     -   input filenames             *
*                       numfiles            -   number of input files       *
*                                                                           *
*   Outputs:            int         -   1 on end of last file               *
*                                                                           *
*   Calling Routines:   main                                                *
*                                                                           * 
*   Glossary:           filecount   - Counter for number of file.           *
*                                                                           *
*   Method:             Input all the files in the parameter list and       *
*                       store them in an array.                             *
*                                                                           *
*                                                                           * 
\***************************************************************************/

#include "nvutility.h"


void loadfiles (char *input_filenames[], int32_t *numfiles)
{
    char           file[256];
    
    /* Input files from the console until eof is reached.  */

    printf ("\nEnter input file names or 'eof' to end\n");

    *numfiles = -1;

    do
    {
        (*numfiles)++;
        ngets (file, sizeof (file), stdin);
        if (strncmp (file, "eof", 3) && strncmp (file, "EOF", 3))
        {
            input_filenames[*numfiles] = (char *) malloc (strlen (file) + 1);
            strcpy (input_filenames[*numfiles], file);
        }
    }
    while (strncmp (file, "eof", 3) && strncmp (file, "EOF", 3));
}
