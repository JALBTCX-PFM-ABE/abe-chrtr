
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


/*********************************************************************************************

    This program is public domain software that was developed by 
    the U.S. Naval Oceanographic Office.

    This is a work of the US Government. In accordance with 17 USC 105,
    copyright protection is not available for any work of the US Government.

    This software is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

*********************************************************************************************/

#ifndef VERSION

#define     VERSION     "PFM Software - chrtr V5.24 - 07/29/14"

#endif

/*

    Version 1.0
    Jan C. Depner
    04/12/06

    First version.


    Version 2.0
    Jan C. Depner
    04/21/00

    Modified to read GSF files.


    Version 2.1
    Sam Mangin, PSI
    06/14/01

    Modified to check swap bytes when reading a dpg file.


    Version 2.2
    Stephanie Lee, PSI
    07/10/02

    Updated checkinput.c to cross international date line and greenwich
    meridian.


    Version 3.0
    Jan C. Depner
    08/05/05

    Added ability to read HOF, TOF, and numerous XYZ formats.


    Version 4.0
    Jan C. Depner
    04/12/06

    Removed all of the old chrtr_lib stuff, replaced with misp_lib (no edge matching or sub areas).


    Version 4.01
    Jan C. Depner
    04/13/06

    Fixed a problem reading force_original_value.


    Version 4.02
    Jan C. Depner
    02/01/07

    Added PFM as an input file option.


    Version 4.1
    Jan C. Depner
    02/16/07

    Fixed width problem (inclusive).


    Version 4.11
    Jan C. Depner
    02/20/07

    Input data point count check.  ScR inputs were floats not doubles.  DOH!!!


    Version 4.12
    Jan C. Depner
    02/26/07

    Went one column too far when reading PFM data.


    Version 4.2
    Jan C. Depner
    04/13/07

    Fix really stupid memory leak in reader.c.


    Version 5.0
    Jan C. Depner
    07/23/07

    Removed JJD input and replaced with LLZ.  Accept [gridmeter] or [gridmin] as input grid size.
    Convert gridmeter to gridmin internally.


    Version 5.01
    Jan C. Depner
    07/27/07

    Added nominal depth flag for GSF files.


    Version 5.02
    Jan C. Depner
    10/22/07

    Added fflush calls after prints to stderr since flush is not automatic in Windows.


    Version 5.03
    Jan C. Depner
    03/21/08

    Fixed LLZ open bug and percentage spinner.


    Version 5.10
    Jan C. Depner
    04/03/08

    Added nibbler.


    Version 5.11
    Jan C. Depner
    04/04/08

    Handle nominal and force_original_value flags correctly.


    Version 5.12
    Jan C. Depner
    04/07/08

    Moved get_string, bit_set, and bit_test to utility library.  Replaced single .h files from utility
    library with include of nvutility.h


    Version 5.13
    Jan C. Depner
    06/10/08

    Wasn't closing LLZ files properly.


    Version 5.14
    Stacy Johnson (PSI)
    02/19/09

    Fixed nibble problem.  Wasn't being passed back from getparam.


    Version 5.15
    Jan C. Depner
    06/26/09

    Added support for WLF files.


    Version 5.16
    Jan C. Depner
    07/29/09

    Changed %lf in printf statements to %f.  The MinGW folks broke this even though it works on every
    compiler known to man (including theirs up to the last version).  No compiler warnings or errors
    are issued.  Many thanks to the MinGW folks for fixing what wasn't broken.


    Version 5.17
    Jan C. Depner
    12/17/09

    Fixed one beam multibeam GSF load error.


    Version 5.18
    Jan C. Depner
    04/29/10

    Fixed posfix and fixpos calls to use numeric constants instead of strings for type.


    Version 5.19
    Jan C. Depner (PFM Software)
    03/17/14

    Removed WLF support.  Top o' the mornin' to ye!


    Version 5.20
    Jan C. Depner (PFM Software)
    05/07/14

    Fixed freads and fgets without return check.


    Version 5.21
    Jan C. Depner (PFM Software)
    05/27/14

    Removed UNISIPS support.


    Version 5.22
    Jan C. Depner (PFM Software)
    07/21/14

    - Removed support for old SHOALS files.  It's been gone from pfmLoad for years
      so I don't know why it was left in here.


    Version 5.23
    Jan C. Depner (PFM Software)
    07/23/14

    - Switched from using the old NV_INT64 and NV_U_INT32 type definitions to the C99 standard stdint.h and
      inttypes.h sized data types (e.g. int64_t and uint32_t).


    Version 5.24
    Jan C. Depner (PFM Software)
    07/29/14

    - Fixed errors discovered by cppcheck.

*/
