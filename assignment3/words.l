%top{ 
// This file was provided by Professor Long
# include <stdio.h> 
# include "words.h"
}

LET [A-Za-z_]
DIG [0-9] 
WS [\t\r\n] 
PUNC [~‘!@#$%^&*()_+-={}|\\\[\]:;\"\’<>,./?]

%%

{LET}+							{ return WORD; } 
{LET}+"'"({LET}|{LET}{LET})		{ return WORD; } 
{LET}+({DIG}+|{LET}+)			{ return WORD; } 
{PUNC}+							{} 
{WS}+							{} 
<<EOF>>							{ return EOF; } 
.								{}

%%

// int main(void) { int res = yylex (); printf ("%d - %s", res , yytext);}