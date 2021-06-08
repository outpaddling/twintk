#include <string.h>
#include <ctype.h>
#include <stdarg.h>
#include "twintk_term.h"

#ifdef __STDC__
char    *tparm(char *string,...)
#else
char    *tparm(string)
char    *string;
#endif

{
	int             len, number, level, x, y, stack_index = 0, i;
	stack_frame     stack[STACKSIZE];
	static  char    buffer[256];    /* Must be static - address returned */
	static  int     param[10];
	static  char    *bufptr;
	static  int     variable[26];
	va_list list;

	va_start(list,string);
	for (i=0; i<10; ++i)
	{
	    param[i] = va_arg(list,int);
	}
	va_end(list);

#ifdef TRACE
	if (_tracing)
	    _tracef("tparm(%s,%d,%d,%d,%d,%d,%d,%d,%d,%d) called",
			string, param[0], param[1], param[2], param[3],
			param[4], param[5], param[6], param[7], param[8]);
#endif

	bufptr = buffer;

	while (*string != '\0')
	{
	    if (*string != '%')
		*(bufptr++) = *string;
	    else
	    {
		string++;
		switch (*string)
		{
		    case '%':
			*(bufptr++) = '%';
			break;

		    case 'd':
			sprintf(bufptr, "%d", npop());
			bufptr += strlen(bufptr);
			break;

		    case '0':
			string++;
			len = *string;
			if ((len == '2'  ||  len == '3')  &&  *++string == 'd')
			{
			    if (len == '2')
				sprintf(bufptr, "%02d", npop());
			    else
				sprintf(bufptr, "%03d", npop());
			    
			    bufptr += strlen(bufptr);
			}
			break;

		    case '2':
			string++;
			if (*string == 'd')
			{
			    sprintf(bufptr, "%2d", npop());
			    bufptr += strlen(bufptr);
			}
			break;

		    case '3':
			string++;
			if (*string == 'd')
			{
			    sprintf(bufptr, "%3d", npop());
			    bufptr += strlen(bufptr);
			}
			break;

		    case 'c':
			number = npop();
			/* Escape special chars for raw_print() */
			if ( (number == '$') || (number == '\\') ||
			     (number == 0) )
			    *(bufptr++) = '\\';
			*(bufptr++) = (char) number;
			break;

		    case 's':
			strcpy(bufptr, spop());
			bufptr += strlen(bufptr);
			break;

		    case 'p':
			string++;
			if (isdigit(*string))
			{
			    npush(param[*string - '1']);
			}
			break;

		    case 'P':
			string++;
			if (islower(*string))
			    variable[*string - 'a'] = npop();
			break;

		    case 'g':
			string++;
			if (islower(*string))
			    npush(variable[*string - 'a']);
			break;

		    case '\'':
			string++;
			npush(*string);
			string++;
			break;

		    case '{':
			number = 0;
			string++;
			while (isdigit(*string))
			{
			    number = number * 10 + *string - '0';
			    string++;
			}
			npush(number);
			break;

		    case '+':
			x = npop();
			y = npop();
			npush(x + y);
			break;

		    case '-':
			y = npop();     /* Order of eval unpredictable */
			x = npop();     /* so pop each in separate statement */
			npush(x - y);
			break;

		    case '*':
			x = npop();
			y = npop();
			npush(x * y);
			break;

		    case '/':
			y = npop();
			x = npop();
			npush(x / y);
			break;

		    case 'm':
			y = npop();
			x = npop();
			npush(x % y);
			break;

		    case '&':
			// FIXME: Why was this using *?
			x = npop();
			y = npop();
			npush(x & y);
			break;

		    case '|':
			x = npop();
			y = npop();
			npush(x | y);
			break;

		    case '^':
			x = npop();
			y = npop();
			npush(x ^ y);
			break;

		    case '=':
			y = npop();
			x = npop();
			npush(x == y);
			break;

		    case '<':
			y = npop();
			x = npop();
			npush(x < y);
			break;

		    case '>':
			y = npop();
			x = npop();
			npush(x > y);
			break;

		    case '!':
			number = npop();
			npush(! number);
			break;

		    case '~':
			number = npop();
			npush(~ number);
			break;

		    case 'i':
			param[0]++;
			param[1]++;
			break;

		    case '?':
			break;

		    case 't':
			x = npop();
			if (x)
			{
			    /* do nothing; keep executing */
			}
			else
			{
			    /* scan forward for %e or %; at level zero */
				string++;
				level = 0;
				while (*string)
				{
				    if (*string == '%')
				    {
					string++;
					if (*string == '?')
					    level++;
					else if (*string == ';')
					{
					    if (level > 0)
						level--;
					    else
						break;
					}
					else if (*string == 'e'  && level == 0)
					    break;
				    }

				    if (*string)
					string++;
				}
			}
			break;

		    case 'e':
			/* scan forward for a %; at level zero */
			    string++;
			    level = 0;
			    while (*string)
			    {
				if (*string == '%')
				{
				    string++;
				    if (*string == '?')
					level++;
				    else if (*string == ';')
				    {
					if (level > 0)
					    level--;
					else
					    break;
				    }
				}

				if (*string)
				    string++;
			    }
			break;

		    case ';':
			break;

		    default:
			break;


		} /* endswitch (*string) */
	    } /* endelse (*string == '%') */

	    if (*string == '\0')
		break;
	    
	    string++;
	} /* endwhile (*string) */

	*bufptr = '\0';
	return(buffer);
}
