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

#include <windows.h>

#define NAME_LEN    40
#define GENDER_LEN  6

int     get_data(win_t *win,char name[],char gender[],int *age);

int     main()

{
    char    name[NAME_LEN+1] = "",gender[GENDER_LEN+1] = "Male";
    int     age = 0;
    term_t  *term;
    win_t   *win;
    
    /* Initialize terminal */
    if ( (term = init_term(stdin,stdout,stderr,NULL)) == NULL)
        return 1;
    tclear_screen(term);
    
    /* Create a new window */
    if ( (win = new_win(term,10,60,8,10)) == NULL )
        return 1;

    wborder(win);
    
    /* Input data */
    get_data(win,name,gender,&age);
    
    /* Free window */
    del_win(&win);
    return 0;
}


int     get_data(win_t *win,char name[],char gender[],int *age)

{
    field_t fields[3];
    char    *genders[3] = {"Male","Female",NULL};
    
    /* Initialize fields */
    init_string(fields+0,2,2,NAME_LEN,CAPITALIZED,"Full name: ",
        " Enter first name, last name, and middle initial. ",name);
    init_enum(fields+1,3,2,GENDER_LEN,genders,"Gender: ",
        " Press <space> to toggle ",gender);
    init_int(fields+2,4,2,0,130,"Age: ",
        " Enter your age in years ",age);
    
    /* Input the data */
    screen_input(win,fields,3,WLINES(win)-3);
    return 0;
}

