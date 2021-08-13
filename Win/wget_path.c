#include <string.h>
#include <fnmatch.h>
#include <unistd.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <dirent.h>
#include <sys/param.h>
#include <xtend/string.h>   // strlcpy() on Linux
#include "twintk.h"

/****************************************************
 * List files matching filespec and enter a filename
 ****************************************************/

int     tw_get_pathname(win_t *win, char *file_name, char *file_spec)

{
    win_t   *names_win, *dirs_win;
    tw_panel_t panel = TWC_PANEL_INIT;
    char    old_file_spec[TWC_SPEC_LEN + 1] = "",
	    old_dir[PATH_MAX+1] = "",
	    current_dir[PATH_MAX+1] = "",
	    request_dir[PATH_MAX+1] = "",
	    temp_name[PATH_MAX+1] = "",
	    menu_request_dir[PATH_MAX+1] = "",
	    *file_names[TWC_MAX_FILENAMES+1],
	    *dir_names[TWC_MAX_FILENAMES+1],
	    **p;
    int     dir_spec_changed, file_spec_changed, last_field, status;

    if ( getcwd(current_dir,PATH_MAX-1) == NULL )
    {
	/*sprintw(2,TWC_ST_LEN,"Cannot determine current working directory.");*/
	return TWC_INPUT_CANCEL;
    }
    strlcpy(old_dir,current_dir,PATH_MAX);
    
    /* Create subwindows with same attributes, minus drop tw_shadows */
    names_win = tw_sub_win(win,TW_LINES(win)-8,
		TW_COLS(win)-20,1,1,NO_AUTO_SCROLL);
    dirs_win = tw_sub_win(win,TW_LINES(win)-8,18,1,
		TW_COLS(win)-19,NO_AUTO_SCROLL);
    /* Give subwindows the same default attributes as the main window */
    names_win->win_attr = win->win_attr;
    dirs_win->win_attr = win->win_attr;

    /* Set up scroll bars */
    TW_NEW_BOTTOM_SCROLL_BAR(names_win);
    TW_NEW_BOTTOM_SCROLL_BAR(dirs_win);
    
    TW_SET_BORDER(win,TW_BORDER(win));
    TW_SET_BORDER(names_win,TW_BORDER(win));
    TW_SET_BORDER(dirs_win,TW_BORDER(win));
    if ( TCOLOR_TERM(win->terminal) && !TW_NO_COLOR(win) )
    {
	tw_set_foreground(win, TW_CUR_FOREGROUND(win));
	tw_set_background(win, TW_CUR_BACKGROUND(win));
	tw_set_foreground(names_win, TW_CUR_FOREGROUND(win));
	tw_set_background(names_win, TW_CUR_BACKGROUND(win));
	tw_set_foreground(dirs_win, TW_CUR_FOREGROUND(win));
	tw_set_background(dirs_win, TW_CUR_BACKGROUND(win));
    }
    else if (THAS_REVERSE_MODE(win->terminal))
    {
	TW_SET_MODES(win, REVERSE_MODE);
	TW_SET_MODES(names_win, REVERSE_MODE);
	TW_SET_MODES(dirs_win, REVERSE_MODE);
    }
    else
    {
	TW_SET_MODES(win, HIGHLIGHT_MODE);
	TW_SET_MODES(names_win, HIGHLIGHT_MODE);
	TW_SET_MODES(dirs_win, HIGHLIGHT_MODE);
    }
    iclear_image(&win->image);
    /*iclear_image(&names_win->image);
    iclear_image(&dirs_win->image);*/
    
    /* Get file_name */
    *file_name = '\0';
    do
    {   
	TW_RESET_PANEL(&panel);
	
	/* List files in current directory */
	strlcpy(request_dir,current_dir,PATH_MAX);
	tw_init_string(&panel, TW_LINES(win) - 6, 3, PATH_MAX,
		TW_COLS(win)-18, TWC_VERBATIM, "Filename? ",
		"Enter the name of the file to load.", temp_name);
	tw_init_menu(&panel, names_win, PATH_MAX,
		file_names, "Use arrows to move, <Ctrl>+d to open file, TAB to exit menu.",temp_name);
	tw_init_menu(&panel, dirs_win, PATH_MAX,
		dir_names, "Use arrows to move, <Ctrl>+d to open directory, TAB to exit menu.",menu_request_dir);
	tw_init_string(&panel, TW_LINES(win) - 7, 3, PATH_MAX,
		30, TWC_VERBATIM, "Directory? ",
		"Enter a full or relative directory name, then type <Ctrl>+d.",
		request_dir);
	tw_init_string(&panel, TW_LINES(win) - 7, 46, TWC_SPEC_LEN,
		TW_COLS(win)-61, TWC_VERBATIM, "File spec? ",
		"Specify which files to list, then type <Ctrl>+d.",
		file_spec);
	
	/* Set up file window */
	if ( TW_HAS_SHADOW(win) )
	    tw_shadow(win);
	tw_draw_border(win);
	
	/* Get list of matching files */
	list_files(file_names,file_spec,S_IFREG,TWC_MAX_FILENAMES);
	if ( *file_names == NULL )
	{
	    /* Put a freeable name in the list for later free() calls */
	    *file_names = strdup("untitled");
	    file_names[1] = NULL;
	}
	strlcpy(old_file_spec, file_spec, TWC_SPEC_LEN);
	*menu_request_dir = '\0';
	list_files(dir_names,"*",S_IFDIR,TWC_MAX_FILENAMES);
	
	/* Window drawing can be choppy.  Better here at the end of the
	   file listing than elsewhere */
	TW_RESTORE_WIN(win);
	TW_FLUSH_OUT(win);  

	/* Invoke input panel */
	status = tw_input_panel(win, &panel, TW_LINES(win)-3);
	last_field = TW_LAST_FIELD(status);
	
	/* See if user used the directory menu */
	if ( (panel.fields[last_field].win == dirs_win) &&
	     (*menu_request_dir != '\0') )
	{
	    /*sprintw(2,50,"Changing request_dir to %s",menu_request_dir);
	    tgetc(term);*/
	    strlcpy(request_dir,menu_request_dir,PATH_MAX);
	}
	
	file_spec_changed = strcmp(old_file_spec, file_spec);
	if ( (dir_spec_changed = strcmp(current_dir, request_dir)) != 0 )
	{
	    if ( chdir(request_dir) == -1 )
	    {
		/*sprintw(2,TWC_ST_LEN,"Cannot open directory \"%s\".",request_dir);*/
	    }
	    else
		getcwd(current_dir,PATH_MAX-1);
	}

	/* Free file and directory names */
	for (p=file_names; *p != NULL; ++p)
	    free(*p);
	for (p=dir_names; *p != NULL; ++p)
	    free(*p);
    }   while (dir_spec_changed || file_spec_changed);
    
    /* Make sure directory hasn't changed if no new file is loaded */
    if ( (*temp_name == '\0') || (TW_EXIT_KEY(status) == TWC_INPUT_CANCEL) )
	chdir(old_dir);
    else
	strshellcpy(file_name,temp_name,PATH_MAX);
    tw_del_win(&dirs_win);
    tw_del_win(&names_win);
    return status;
}


/**************************************************************
 * List files matching the file specification and file type
 **************************************************************/

int     list_files(names,pattern,file_type,max_names)
char    *names[], *pattern;
int     file_type;
size_t  max_names;

{
    int     maxlen = 0, len, c;
    DIR     *dp;
    struct dirent   *dir_entry;
    struct stat     st;
    char    **p = names, temp_pattern[TWC_SPEC_LEN+1],*specs[TWC_MAX_SPECS];
    
    if ( (dp = opendir(".")) != NULL )
    {
	/* Separate filespecs */
	strlcpy(temp_pattern,pattern,TWC_SPEC_LEN);
	specs[0] = strtok(temp_pattern," \t");
	for (c=1; (specs[c] = strtok(NULL," \t")) != NULL; ++c)
	    ;
	
	while ( (dir_entry = readdir(dp)) != NULL )
	{
	    /* See if filename matches one of the specs */
	    for (c=0; (specs[c]!=NULL) &&
		      fnmatch(specs[c],dir_entry->d_name,FNM_PERIOD); ++c)
		;
	    
	    /* If match found, and not a .o file, add to list
	       Force ".." in to work around PNM_PERIOD flag which would
	       rule it out otherwise. */
	    if ( (((specs[c]!=NULL) &&
		  (fnmatch("*.o",dir_entry->d_name,0) != 0)) ||
		  (strcmp(dir_entry->d_name,"..") == 0)) &&
		 (stat(dir_entry->d_name,&st) != -1) &&
		 (p-names < max_names) )
		if (st.st_mode & file_type)
		{
		    *p++ = strdup(dir_entry->d_name);
		    if ( (len=strlen(dir_entry->d_name)) > maxlen )
			maxlen = len;
		}
	}
	qsort(names,p-names,sizeof(char *),(int (*)())strptrcmp);
	*p = NULL;
	closedir(dp);
    }
    return maxlen;
}
