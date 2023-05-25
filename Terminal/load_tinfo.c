/*
    Copyright (c) 1994-2003, Jason W. Bacon, Acadix Software Systems
    All rights reserved.

    Redistribution and use in source and binary forms, with or without
    modification, are permitted provided that the following conditions are
    met:

    Redistributions of source code must retain the above copyright notice,
    this list of conditions and the following disclaimer. Redistributions
    in binary form must reproduce the above copyright notice, this list of
    conditions and the following disclaimer in the documentation and/or
    other materials provided with the distribution. 

    THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS
    IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED
    TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A
    PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
    OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
    SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
    LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
    DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
    THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
    (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
    OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE. 
*/

#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <limits.h>         // PATH_MAX on SunOS
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/param.h>
#include <time.h>
#include <stdio.h>
#include <errno.h>
#include <ctype.h>
#include <xtend/string.h>   // strlcpy() on Linux
#include "twintk_term.h"


/* Endian check.  Keep parens for gcc 2.5.6 */
const short   Split = 1;
#define MUST_SWAP   *( ((char *)(&Split)) + 1)

int 
load_tinfo (char *term_name, term_t *terminal)

{
    int     fd = -1, bytes_read, num_strings, i;
    struct header header;
    char    ch, filename[PATH_MAX + 1], term_names[NAME_SIZE], *dir, *path,
	    temp_path[PATH_MAX+1] = "",message[PATH_MAX + 50];
    extern int errno;

    /* Temp space to inhale terminfo data into */
    short   offset[MAX_STRINGS];        /* String offsets in str_table */
    short   numbers[MAX_NUMBERS];       /* Numeric caps */
    char    booleans[MAX_BOOLEANS];     /* Boolean caps */
    char    *strings[MAX_STRINGS];      /* Pointers to string caps */
    
    /* Permanent contiguous space malloc()'d for strings end to end */
    char    *str_table;

    /* Must be fixed length static array so patch can replace with
       potentially longer string */
    static char twin_path[PATH_MAX+1] = "No Twinpath";
    
    /* TWINPATH env overrides built-in default */
    if ( (path = getenv("TWINPATH")) != NULL )
	strlcpy(temp_path,path,PATH_MAX);
    /* Avoid putting "No Path" in again since patch will replace it */
    else if ( memcmp(twin_path,"No ",3) != 0 )
    {
	strlcpy(temp_path,twin_path,PATH_MAX);
    }
    
    dir = strtok(temp_path,":");
    while ( (dir != NULL) && (fd == -1) )
    {
	snprintf(filename, PATH_MAX, "%s/%c/%s", dir, *term_name, term_name);
	if ( (fd = open(filename, O_RDONLY, 0)) == -1 )
	    dir = strtok(NULL,":");
    }

    if ( (dir == NULL) || (fd == -1) )
    {
	dir = TERMINFO_DIR;
	/*
	 * Work around case-insensitive filesystems by putting capitalized
	 * terminal names in their own subdir
	 */
	if ( isupper(*term_name) )
	    snprintf(filename, PATH_MAX, "%s/%s/CAPS/%c/%s", PREFIX, dir, *term_name, term_name);
	else
	    snprintf(filename, PATH_MAX, "%s/%s/%c/%s", PREFIX, dir, *term_name, term_name);
	if ((fd = open(filename, O_RDONLY, 0)) == -1)
	{
	    snprintf(message,PATH_MAX+50,"load_tinfo(): Could not open %s",filename);
	    perror(message);
	    if ( errno == ENOENT )
	    {
		fprintf(stderr,"If you're using a non-standard terminfo directory, you may need to set the\n");
		fprintf(stderr,"environment variable TWINPATH to the directory containing the entry.\n");
		fprintf(stderr,"This should be done in .cshrc and any other shell init scripts.\n");
		fprintf(stderr,"Press return...");
	    }
	    getchar();
	    return (-1);
	}
    }
    
    /* Get header info and swap bytes if big-endian */
    read(fd, &header, sizeof(header));
    if (MUST_SWAP)
    {
	header.magic = SWAP(header.magic);
	header.name_size = SWAP(header.name_size);
	header.bool_count = SWAP(header.bool_count);
	header.num_count = SWAP(header.num_count);
	header.str_count = SWAP(header.str_count);
	header.str_size = SWAP(header.str_size);
    }
    str_table = MALLOC(header.str_size,char);

    /* Verify file is a terminfo binary */
    if (header.magic != TERM_MAGIC)
    {
	close(fd);
	fprintf(stderr,
	    "load_tinfo(): Bad magic number (%o) in terminfo entry %s.\n",
	    header.magic, filename);
	return (-1);
    }

    /* Read terminal names */
    read(fd, term_names, MIN(127, header.name_size));
    term_names[127] = '\0';
    if (header.name_size > 127)
	lseek(fd, (long) (header.name_size - 127), SEEK_CUR);

#ifdef DEBUG
    fprintf(stderr,"Names: %s\n",term_names);
#endif

    /* Read booleans */
    read(fd, booleans, MIN(MAX_BOOLEANS, header.bool_count));
    if (header.bool_count > MAX_BOOLEANS)
	lseek(fd, (long) (header.bool_count - MAX_BOOLEANS), SEEK_CUR);
    else
	for (i = header.bool_count; i < MAX_BOOLEANS; i++)
	    booleans[i] = 0;

    if (ODD(header.name_size + header.bool_count))
	read(fd, &ch, 1);

#ifdef  DEBUG
    fprintf(stderr,"%d Booleans: (Should be all 0 or 1)\n",header.bool_count);
    for (i=0; i<header.bool_count; ++i)
	fprintf(stderr,"%d ",booleans[i]);
#endif

    /* Read numbers */
    read(fd, numbers, MIN(MAX_NUMBERS, header.num_count) * 2);
    if (MUST_SWAP)
    {
	for (i = 0; i < MIN(header.num_count, MAX_NUMBERS); i++)
	    numbers[i] = SWAP(numbers[i]);
    }

    if (header.num_count > MAX_NUMBERS)
	lseek(fd, (long) (2 * (header.num_count - MAX_NUMBERS)), SEEK_CUR);
    else
	for (i = header.num_count; i < MAX_NUMBERS; i++)
	    numbers[i] = -1;

#ifdef DEBUG
    fprintf(stderr,"\n%d Numbers:\n",header.num_count);
    for (i=0; i<header.num_count; ++i)
	fprintf(stderr,"%d ",numbers[i]);
#endif

    /* Read string offsets */
    num_strings = MIN(MAX_STRINGS, header.str_count);
#ifdef DEBUG
    fprintf(stderr,"Reading %d strings...\n",header.str_count);
#endif
    bytes_read = read(fd, offset, sizeof(*offset) * num_strings);
    if (bytes_read <= 0)
    {
	fprintf(stderr,"load_tinfo(): Could not read strings.\n");
	close(fd);
	return (-1);
    }
    
    if (MUST_SWAP)
    {
	for (i = 0; i < bytes_read / 2; i++)
	{
	    offset[i] = SWAP(offset[i]);
	    strings[i] = (offset[i] == -1) ? NULL : str_table + offset[i];
	}
    }
    else
    {
	for (i = 0; i < bytes_read / 2; i++)
	    strings[i] = (offset[i] == -1) ? NULL : str_table + offset[i];
    }
    
    if (header.str_count > MAX_STRINGS)
	lseek(fd, (long) (2 * (header.str_count - MAX_STRINGS)), SEEK_CUR);
    else
	for (i = header.str_count; i < MAX_STRINGS; i++)
	    strings[i] = NULL;

#ifdef DEBUG
#endif

    /* Read and check string table */
    bytes_read = read(fd, str_table, header.str_size);
    close(fd);
    if (bytes_read != header.str_size)
    {
	fprintf(stderr, "load_tinfo(): Error reading string table.\n");
	return (-1);
    }

#ifdef DEBUG
    fprintf(stderr,"\n%d Strings:\n",header.str_count);
    for (i=0; i<header.str_count; ++i)
    {
	char    temp[1024];

	printf("%3d=%3d ",i,offset[i]);
	*temp = '\0';
	strviscpy((unsigned char *)temp, (unsigned char *)strings[i], 1024);
	puts(temp);
    }
#endif

    /* Assign values to named fields in term_t structure */
    init_names(terminal,booleans,numbers,strings);
    return (0);
}
