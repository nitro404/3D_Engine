//
// strcasecmp.cc
//
// strcasecmp: replacement of the strcasecmp functions for architectures that do
//             not have it.
//
// Part of the ht://Dig package   <http://www.htdig.org/>
// Copyright (c) 1999-2004 The ht://Dig Group
// For copyright details, see the file COPYING in your distribution
// or the GNU Library General Public License (LGPL) version 2 or later 
// <http://www.gnu.org/copyleft/lgpl.html>
//
// $Id: strcasecmp.cc,v 1.10 2004/05/28 13:15:22 lha Exp $
//

#ifndef _STRCASECMP_H
#define _STRCASECMP_H

/*
#ifdef HAVE_CONFIG_H
#include "htconfig.h"
#endif // HAVE_CONFIG_H 

#include "lib.h"
*/

#include <string.h>
#include <ctype.h>

//*****************************************************************************
//
int strcasecmp(const char *str1, const char *str2);


//#define tolower(ch)	(isupper(ch) ? (ch) + 'a' - 'A' : (ch))
//*****************************************************************************
//
int strncasecmp(const char *str1, const char *str2, int n);

/*
//*****************************************************************************
// char *strdup(char *str)
//
char *strdup(char *str)
{
    char	*p = new char[strlen(str) + 1];
    strcpy(p, str);
    return p;
}


//*****************************************************************************
// char *strcasestr(const char *s, const char *pattern)
//
const char *
mystrcasestr(const char *s, const char *pattern)
{
    int		length = strlen(pattern);

    while (*s)
    {
	if (mystrncasecmp(s, pattern, length) == 0)
	    return s;
	s++;
    }
    return 0;
}
*/

#endif