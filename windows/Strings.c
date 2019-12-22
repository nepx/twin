/*    
	Strings.c	2.21
    	Copyright 1997 Willows Software, Inc. 

This library is free software; you can redistribute it and/or
modify it under the terms of the GNU Library General Public License as
published by the Free Software Foundation; either version 2 of the
License, or (at your option) any later version.

This library is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
Library General Public License for more details.

You should have received a copy of the GNU Library General Public
License along with this library; see the file COPYING.LIB.  If
not, write to the Free Software Foundation, Inc., 675 Mass Ave,
Cambridge, MA 02139, USA.


For more information about the Willows Twin Libraries.

	http://www.willows.com	

To send email to the maintainer of the Willows Twin Libraries.

	mailto:twin@willows.com 

 */

#include "windows.h"
#include "Log.h"

#include <stdio.h>
#include <string.h>
#include <ctype.h>


static const unsigned char CT_CType2_LUT[] = {
  C2_NOTAPPLICABLE, /*   -   0 */
  C2_NOTAPPLICABLE, /*   -   1 */
  C2_NOTAPPLICABLE, /*   -   2 */
  C2_NOTAPPLICABLE, /*   -   3 */
  C2_NOTAPPLICABLE, /*   -   4 */
  C2_NOTAPPLICABLE, /*   -   5 */
  C2_NOTAPPLICABLE, /*   -   6 */
  C2_NOTAPPLICABLE, /*   -   7 */
  C2_NOTAPPLICABLE, /*   -   8 */
  C2_SEGMENTSEPARATOR, /*   -   9 */
  C2_NOTAPPLICABLE, /*   -  10 */
  C2_NOTAPPLICABLE, /*   -  11 */
  C2_NOTAPPLICABLE, /*   -  12 */
  C2_NOTAPPLICABLE, /*   -  13 */
  C2_NOTAPPLICABLE, /*   -  14 */
  C2_NOTAPPLICABLE, /*   -  15 */
  C2_NOTAPPLICABLE, /*   -  16 */
  C2_NOTAPPLICABLE, /*   -  17 */
  C2_NOTAPPLICABLE, /*   -  18 */
  C2_NOTAPPLICABLE, /*   -  19 */
  C2_NOTAPPLICABLE, /*   -  20 */
  C2_NOTAPPLICABLE, /*   -  21 */
  C2_NOTAPPLICABLE, /*   -  22 */
  C2_NOTAPPLICABLE, /*   -  23 */
  C2_NOTAPPLICABLE, /*   -  24 */
  C2_NOTAPPLICABLE, /*   -  25 */
  C2_NOTAPPLICABLE, /*   -  26 */
  C2_NOTAPPLICABLE, /*   -  27 */
  C2_NOTAPPLICABLE, /*   -  28 */
  C2_NOTAPPLICABLE, /*   -  29 */
  C2_NOTAPPLICABLE, /*   -  30 */
  C2_NOTAPPLICABLE, /*   -  31 */
  C2_WHITESPACE, /*   -  32 */
  C2_OTHERNEUTRAL, /* ! -  33 */
  C2_OTHERNEUTRAL, /* " -  34 */ /* " */
  C2_EUROPETERMINATOR, /* # -  35 */
  C2_EUROPETERMINATOR, /* $ -  36 */
  C2_EUROPETERMINATOR, /* % -  37 */
  C2_LEFTTORIGHT, /* & -  38 */
  C2_OTHERNEUTRAL, /* ' -  39 */
  C2_OTHERNEUTRAL, /* ( -  40 */
  C2_OTHERNEUTRAL, /* ) -  41 */
  C2_OTHERNEUTRAL, /* * -  42 */
  C2_EUROPETERMINATOR, /* + -  43 */
  C2_COMMONSEPARATOR, /* , -  44 */
  C2_EUROPETERMINATOR, /* - -  45 */
  C2_EUROPESEPARATOR, /* . -  46 */
  C2_EUROPESEPARATOR, /* / -  47 */
  C2_EUROPENUMBER, /* 0 -  48 */
  C2_EUROPENUMBER, /* 1 -  49 */
  C2_EUROPENUMBER, /* 2 -  50 */
  C2_EUROPENUMBER, /* 3 -  51 */
  C2_EUROPENUMBER, /* 4 -  52 */
  C2_EUROPENUMBER, /* 5 -  53 */
  C2_EUROPENUMBER, /* 6 -  54 */
  C2_EUROPENUMBER, /* 7 -  55 */
  C2_EUROPENUMBER, /* 8 -  56 */
  C2_EUROPENUMBER, /* 9 -  57 */
  C2_COMMONSEPARATOR, /* : -  58 */
  C2_OTHERNEUTRAL, /* ; -  59 */
  C2_OTHERNEUTRAL, /* < -  60 */
  C2_OTHERNEUTRAL, /* = -  61 */
  C2_OTHERNEUTRAL, /* > -  62 */
  C2_OTHERNEUTRAL, /* ? -  63 */
  C2_LEFTTORIGHT, /* @ -  64 */
  C2_LEFTTORIGHT, /* A -  65 */
  C2_LEFTTORIGHT, /* B -  66 */
  C2_LEFTTORIGHT, /* C -  67 */
  C2_LEFTTORIGHT, /* D -  68 */
  C2_LEFTTORIGHT, /* E -  69 */
  C2_LEFTTORIGHT, /* F -  70 */
  C2_LEFTTORIGHT, /* G -  71 */
  C2_LEFTTORIGHT, /* H -  72 */
  C2_LEFTTORIGHT, /* I -  73 */
  C2_LEFTTORIGHT, /* J -  74 */
  C2_LEFTTORIGHT, /* K -  75 */
  C2_LEFTTORIGHT, /* L -  76 */
  C2_LEFTTORIGHT, /* M -  77 */
  C2_LEFTTORIGHT, /* N -  78 */
  C2_LEFTTORIGHT, /* O -  79 */
  C2_LEFTTORIGHT, /* P -  80 */
  C2_LEFTTORIGHT, /* Q -  81 */
  C2_LEFTTORIGHT, /* R -  82 */
  C2_LEFTTORIGHT, /* S -  83 */
  C2_LEFTTORIGHT, /* T -  84 */
  C2_LEFTTORIGHT, /* U -  85 */
  C2_LEFTTORIGHT, /* V -  86 */
  C2_LEFTTORIGHT, /* W -  87 */
  C2_LEFTTORIGHT, /* X -  88 */
  C2_LEFTTORIGHT, /* Y -  89 */
  C2_LEFTTORIGHT, /* Z -  90 */
  C2_OTHERNEUTRAL, /* [ -  91 */
  C2_OTHERNEUTRAL, /* \ -  92 */
  C2_OTHERNEUTRAL, /* ] -  93 */
  C2_OTHERNEUTRAL, /* ^ -  94 */
  C2_OTHERNEUTRAL, /* _ -  95 */
  C2_OTHERNEUTRAL, /* ` -  96 */
  C2_LEFTTORIGHT, /* a -  97 */
  C2_LEFTTORIGHT, /* b -  98 */
  C2_LEFTTORIGHT, /* c -  99 */
  C2_LEFTTORIGHT, /* d - 100 */
  C2_LEFTTORIGHT, /* e - 101 */
  C2_LEFTTORIGHT, /* f - 102 */
  C2_LEFTTORIGHT, /* g - 103 */
  C2_LEFTTORIGHT, /* h - 104 */
  C2_LEFTTORIGHT, /* i - 105 */
  C2_LEFTTORIGHT, /* j - 106 */
  C2_LEFTTORIGHT, /* k - 107 */
  C2_LEFTTORIGHT, /* l - 108 */
  C2_LEFTTORIGHT, /* m - 109 */
  C2_LEFTTORIGHT, /* n - 110 */
  C2_LEFTTORIGHT, /* o - 111 */
  C2_LEFTTORIGHT, /* p - 112 */
  C2_LEFTTORIGHT, /* q - 113 */
  C2_LEFTTORIGHT, /* r - 114 */
  C2_LEFTTORIGHT, /* s - 115 */
  C2_LEFTTORIGHT, /* t - 116 */
  C2_LEFTTORIGHT, /* u - 117 */
  C2_LEFTTORIGHT, /* v - 118 */
  C2_LEFTTORIGHT, /* w - 119 */
  C2_LEFTTORIGHT, /* x - 120 */
  C2_LEFTTORIGHT, /* y - 121 */
  C2_LEFTTORIGHT, /* z - 122 */
  C2_OTHERNEUTRAL, /* { - 123 */
  C2_OTHERNEUTRAL, /* | - 124 */
  C2_OTHERNEUTRAL, /* } - 125 */
  C2_OTHERNEUTRAL, /* ~ - 126 */
  C2_NOTAPPLICABLE, /*  - 127 */
  C2_NOTAPPLICABLE, /* Ä - 128 */
  C2_NOTAPPLICABLE, /* Å - 129 */
  C2_OTHERNEUTRAL, /* Ç - 130 */
  C2_LEFTTORIGHT, /* É - 131 */
  C2_OTHERNEUTRAL, /* Ñ - 132 */
  C2_OTHERNEUTRAL, /* Ö - 133 */
  C2_OTHERNEUTRAL, /* Ü - 134 */
  C2_OTHERNEUTRAL, /* á - 135 */
  C2_LEFTTORIGHT, /* à - 136 */
  C2_EUROPETERMINATOR, /* â - 137 */
  C2_LEFTTORIGHT, /* ä - 138 */
  C2_OTHERNEUTRAL, /* ã - 139 */
  C2_LEFTTORIGHT, /* å - 140 */
  C2_NOTAPPLICABLE, /* ç - 141 */
  C2_NOTAPPLICABLE, /* é - 142 */
  C2_NOTAPPLICABLE, /* è - 143 */
  C2_NOTAPPLICABLE, /* ê - 144 */
  C2_OTHERNEUTRAL, /* ë - 145 */
  C2_OTHERNEUTRAL, /* í - 146 */
  C2_OTHERNEUTRAL, /* ì - 147 */
  C2_OTHERNEUTRAL, /* î - 148 */
  C2_OTHERNEUTRAL, /* ï - 149 */
  C2_OTHERNEUTRAL, /* ñ - 150 */
  C2_OTHERNEUTRAL, /* ó - 151 */
  C2_LEFTTORIGHT, /* ò - 152 */
  C2_OTHERNEUTRAL, /* ô - 153 */
  C2_LEFTTORIGHT, /* ö - 154 */
  C2_OTHERNEUTRAL, /* õ - 155 */
  C2_LEFTTORIGHT, /* ú - 156 */
  C2_NOTAPPLICABLE, /* ù - 157 */
  C2_NOTAPPLICABLE, /* û - 158 */
  C2_LEFTTORIGHT, /* ü - 159 */
  C2_WHITESPACE, /* † - 160 */
  C2_OTHERNEUTRAL, /* ° - 161 */
  C2_EUROPETERMINATOR, /* ¢ - 162 */
  C2_EUROPETERMINATOR, /* £ - 163 */
  C2_EUROPETERMINATOR, /* § - 164 */
  C2_EUROPETERMINATOR, /* • - 165 */
  C2_OTHERNEUTRAL, /* ¶ - 166 */
  C2_OTHERNEUTRAL, /* ß - 167 */
  C2_OTHERNEUTRAL, /* ® - 168 */
  C2_OTHERNEUTRAL, /* © - 169 */
  C2_OTHERNEUTRAL, /* ™ - 170 */
  C2_OTHERNEUTRAL, /* ´ - 171 */
  C2_OTHERNEUTRAL, /* ¨ - 172 */
  C2_OTHERNEUTRAL, /* ≠ - 173 */
  C2_OTHERNEUTRAL, /* Æ - 174 */
  C2_OTHERNEUTRAL, /* Ø - 175 */
  C2_EUROPETERMINATOR, /* ∞ - 176 */
  C2_EUROPETERMINATOR, /* ± - 177 */
  C2_EUROPENUMBER, /* ≤ - 178 */
  C2_EUROPENUMBER, /* ≥ - 179 */
  C2_OTHERNEUTRAL, /* ¥ - 180 */
  C2_OTHERNEUTRAL, /* µ - 181 */
  C2_OTHERNEUTRAL, /* ∂ - 182 */
  C2_OTHERNEUTRAL, /* ∑ - 183 */
  C2_OTHERNEUTRAL, /* ∏ - 184 */
  C2_EUROPENUMBER, /* π - 185 */
  C2_OTHERNEUTRAL, /* ∫ - 186 */
  C2_OTHERNEUTRAL, /* ª - 187 */
  C2_OTHERNEUTRAL, /* º - 188 */
  C2_OTHERNEUTRAL, /* Ω - 189 */
  C2_OTHERNEUTRAL, /* æ - 190 */
  C2_OTHERNEUTRAL, /* ø - 191 */
  C2_LEFTTORIGHT, /* ¿ - 192 */
  C2_LEFTTORIGHT, /* ¡ - 193 */
  C2_LEFTTORIGHT, /* ¬ - 194 */
  C2_LEFTTORIGHT, /* √ - 195 */
  C2_LEFTTORIGHT, /* ƒ - 196 */
  C2_LEFTTORIGHT, /* ≈ - 197 */
  C2_LEFTTORIGHT, /* ∆ - 198 */
  C2_LEFTTORIGHT, /* « - 199 */
  C2_LEFTTORIGHT, /* » - 200 */
  C2_LEFTTORIGHT, /* … - 201 */
  C2_LEFTTORIGHT, /*   - 202 */
  C2_LEFTTORIGHT, /* À - 203 */
  C2_LEFTTORIGHT, /* Ã - 204 */
  C2_LEFTTORIGHT, /* Õ - 205 */
  C2_LEFTTORIGHT, /* Œ - 206 */
  C2_LEFTTORIGHT, /* œ - 207 */
  C2_LEFTTORIGHT, /* – - 208 */
  C2_LEFTTORIGHT, /* — - 209 */
  C2_LEFTTORIGHT, /* “ - 210 */
  C2_LEFTTORIGHT, /* ” - 211 */
  C2_LEFTTORIGHT, /* ‘ - 212 */
  C2_LEFTTORIGHT, /* ’ - 213 */
  C2_LEFTTORIGHT, /* ÷ - 214 */
  C2_OTHERNEUTRAL, /* ◊ - 215 */
  C2_LEFTTORIGHT, /* ÿ - 216 */
  C2_LEFTTORIGHT, /* Ÿ - 217 */
  C2_LEFTTORIGHT, /* ⁄ - 218 */
  C2_LEFTTORIGHT, /* € - 219 */
  C2_LEFTTORIGHT, /* ‹ - 220 */
  C2_LEFTTORIGHT, /* › - 221 */
  C2_LEFTTORIGHT, /* ﬁ - 222 */
  C2_LEFTTORIGHT, /* ﬂ - 223 */
  C2_LEFTTORIGHT, /* ‡ - 224 */
  C2_LEFTTORIGHT, /* · - 225 */
  C2_LEFTTORIGHT, /* ‚ - 226 */
  C2_LEFTTORIGHT, /* „ - 227 */
  C2_LEFTTORIGHT, /* ‰ - 228 */
  C2_LEFTTORIGHT, /* Â - 229 */
  C2_LEFTTORIGHT, /* Ê - 230 */
  C2_LEFTTORIGHT, /* Á - 231 */
  C2_LEFTTORIGHT, /* Ë - 232 */
  C2_LEFTTORIGHT, /* È - 233 */
  C2_LEFTTORIGHT, /* Í - 234 */
  C2_LEFTTORIGHT, /* Î - 235 */
  C2_LEFTTORIGHT, /* Ï - 236 */
  C2_LEFTTORIGHT, /* Ì - 237 */
  C2_LEFTTORIGHT, /* Ó - 238 */
  C2_LEFTTORIGHT, /* Ô - 239 */
  C2_LEFTTORIGHT, /*  - 240 */
  C2_LEFTTORIGHT, /* Ò - 241 */
  C2_LEFTTORIGHT, /* Ú - 242 */
  C2_LEFTTORIGHT, /* Û - 243 */
  C2_LEFTTORIGHT, /* Ù - 244 */
  C2_LEFTTORIGHT, /* ı - 245 */
  C2_LEFTTORIGHT, /* ˆ - 246 */
  C2_OTHERNEUTRAL, /* ˜ - 247 */
  C2_LEFTTORIGHT, /* ¯ - 248 */
  C2_LEFTTORIGHT, /* ˘ - 249 */
  C2_LEFTTORIGHT, /* ˙ - 250 */
  C2_LEFTTORIGHT, /* ˚ - 251 */
  C2_LEFTTORIGHT, /* ¸ - 252 */
  C2_LEFTTORIGHT, /* ˝ - 253 */
  C2_LEFTTORIGHT, /* ˛ - 254 */
  C2_LEFTTORIGHT /* ˇ - 255 */
};

const WORD OLE2NLS_CT_CType3_LUT[] = { 
  0x0000, /*   -   0 */
  0x0000, /*   -   1 */
  0x0000, /*   -   2 */
  0x0000, /*   -   3 */
  0x0000, /*   -   4 */
  0x0000, /*   -   5 */
  0x0000, /*   -   6 */
  0x0000, /*   -   7 */
  0x0000, /*   -   8 */
  0x0008, /*   -   9 */
  0x0008, /*   -  10 */
  0x0008, /*   -  11 */
  0x0008, /*   -  12 */
  0x0008, /*   -  13 */
  0x0000, /*   -  14 */
  0x0000, /*   -  15 */
  0x0000, /*   -  16 */
  0x0000, /*   -  17 */
  0x0000, /*   -  18 */
  0x0000, /*   -  19 */
  0x0000, /*   -  20 */
  0x0000, /*   -  21 */
  0x0000, /*   -  22 */
  0x0000, /*   -  23 */
  0x0000, /*   -  24 */
  0x0000, /*   -  25 */
  0x0000, /*   -  26 */
  0x0000, /*   -  27 */
  0x0000, /*   -  28 */
  0x0000, /*   -  29 */
  0x0000, /*   -  30 */
  0x0000, /*   -  31 */
  0x0048, /*   -  32 */
  0x0048, /* ! -  33 */
  0x0448, /* " -  34 */ /* " */
  0x0048, /* # -  35 */
  0x0448, /* $ -  36 */
  0x0048, /* % -  37 */
  0x0048, /* & -  38 */
  0x0440, /* ' -  39 */
  0x0048, /* ( -  40 */
  0x0048, /* ) -  41 */
  0x0048, /* * -  42 */
  0x0048, /* + -  43 */
  0x0048, /* , -  44 */
  0x0440, /* - -  45 */
  0x0048, /* . -  46 */
  0x0448, /* / -  47 */
  0x0040, /* 0 -  48 */
  0x0040, /* 1 -  49 */
  0x0040, /* 2 -  50 */
  0x0040, /* 3 -  51 */
  0x0040, /* 4 -  52 */
  0x0040, /* 5 -  53 */
  0x0040, /* 6 -  54 */
  0x0040, /* 7 -  55 */
  0x0040, /* 8 -  56 */
  0x0040, /* 9 -  57 */
  0x0048, /* : -  58 */
  0x0048, /* ; -  59 */
  0x0048, /* < -  60 */
  0x0448, /* = -  61 */
  0x0048, /* > -  62 */
  0x0048, /* ? -  63 */
  0x0448, /* @ -  64 */
  0x8040, /* A -  65 */
  0x8040, /* B -  66 */
  0x8040, /* C -  67 */
  0x8040, /* D -  68 */
  0x8040, /* E -  69 */
  0x8040, /* F -  70 */
  0x8040, /* G -  71 */
  0x8040, /* H -  72 */
  0x8040, /* I -  73 */
  0x8040, /* J -  74 */
  0x8040, /* K -  75 */
  0x8040, /* L -  76 */
  0x8040, /* M -  77 */
  0x8040, /* N -  78 */
  0x8040, /* O -  79 */
  0x8040, /* P -  80 */
  0x8040, /* Q -  81 */
  0x8040, /* R -  82 */
  0x8040, /* S -  83 */
  0x8040, /* T -  84 */
  0x8040, /* U -  85 */
  0x8040, /* V -  86 */
  0x8040, /* W -  87 */
  0x8040, /* X -  88 */
  0x8040, /* Y -  89 */
  0x8040, /* Z -  90 */
  0x0048, /* [ -  91 */
  0x0448, /* \ -  92 */
  0x0048, /* ] -  93 */
  0x0448, /* ^ -  94 */
  0x0448, /* _ -  95 */
  0x0448, /* ` -  96 */
  0x8040, /* a -  97 */
  0x8040, /* b -  98 */
  0x8040, /* c -  99 */
  0x8040, /* d - 100 */
  0x8040, /* e - 101 */
  0x8040, /* f - 102 */
  0x8040, /* g - 103 */
  0x8040, /* h - 104 */
  0x8040, /* i - 105 */
  0x8040, /* j - 106 */
  0x8040, /* k - 107 */
  0x8040, /* l - 108 */
  0x8040, /* m - 109 */
  0x8040, /* n - 110 */
  0x8040, /* o - 111 */
  0x8040, /* p - 112 */
  0x8040, /* q - 113 */
  0x8040, /* r - 114 */
  0x8040, /* s - 115 */
  0x8040, /* t - 116 */
  0x8040, /* u - 117 */
  0x8040, /* v - 118 */
  0x8040, /* w - 119 */
  0x8040, /* x - 120 */
  0x8040, /* y - 121 */
  0x8040, /* z - 122 */
  0x0048, /* { - 123 */
  0x0048, /* | - 124 */
  0x0048, /* } - 125 */
  0x0448, /* ~ - 126 */
  0x0000, /*  - 127 */
  0x0000, /* Ä - 128 */
  0x0000, /* Å - 129 */
  0x0008, /* Ç - 130 */
  0x8000, /* É - 131 */
  0x0008, /* Ñ - 132 */
  0x0008, /* Ö - 133 */
  0x0008, /* Ü - 134 */
  0x0008, /* á - 135 */
  0x0001, /* à - 136 */
  0x0008, /* â - 137 */
  0x8003, /* ä - 138 */
  0x0008, /* ã - 139 */
  0x8000, /* å - 140 */
  0x0000, /* ç - 141 */
  0x0000, /* é - 142 */
  0x0000, /* è - 143 */
  0x0000, /* ê - 144 */
  0x0088, /* ë - 145 */
  0x0088, /* í - 146 */
  0x0088, /* ì - 147 */
  0x0088, /* î - 148 */
  0x0008, /* ï - 149 */
  0x0400, /* ñ - 150 */
  0x0400, /* ó - 151 */
  0x0408, /* ò - 152 */
  0x0000, /* ô - 153 */
  0x8003, /* ö - 154 */
  0x0008, /* õ - 155 */
  0x8000, /* ú - 156 */
  0x0000, /* ù - 157 */
  0x0000, /* û - 158 */
  0x8003, /* ü - 159 */
  0x0008, /* † - 160 */
  0x0008, /* ° - 161 */
  0x0048, /* ¢ - 162 */
  0x0048, /* £ - 163 */
  0x0008, /* § - 164 */
  0x0048, /* • - 165 */
  0x0048, /* ¶ - 166 */
  0x0008, /* ß - 167 */
  0x0408, /* ® - 168 */
  0x0008, /* © - 169 */
  0x0400, /* ™ - 170 */
  0x0008, /* ´ - 171 */
  0x0048, /* ¨ - 172 */
  0x0408, /* ≠ - 173 */
  0x0008, /* Æ - 174 */
  0x0448, /* Ø - 175 */
  0x0008, /* ∞ - 176 */
  0x0008, /* ± - 177 */
  0x0000, /* ≤ - 178 */
  0x0000, /* ≥ - 179 */
  0x0408, /* ¥ - 180 */
  0x0008, /* µ - 181 */
  0x0008, /* ∂ - 182 */
  0x0008, /* ∑ - 183 */
  0x0408, /* ∏ - 184 */
  0x0000, /* π - 185 */
  0x0400, /* ∫ - 186 */
  0x0008, /* ª - 187 */
  0x0000, /* º - 188 */
  0x0000, /* Ω - 189 */
  0x0000, /* æ - 190 */
  0x0008, /* ø - 191 */
  0x8003, /* ¿ - 192 */
  0x8003, /* ¡ - 193 */
  0x8003, /* ¬ - 194 */
  0x8003, /* √ - 195 */
  0x8003, /* ƒ - 196 */
  0x8003, /* ≈ - 197 */
  0x8000, /* ∆ - 198 */
  0x8003, /* « - 199 */
  0x8003, /* » - 200 */
  0x8003, /* … - 201 */
  0x8003, /*   - 202 */
  0x8003, /* À - 203 */
  0x8003, /* Ã - 204 */
  0x8003, /* Õ - 205 */
  0x8003, /* Œ - 206 */
  0x8003, /* œ - 207 */
  0x8000, /* – - 208 */
  0x8003, /* — - 209 */
  0x8003, /* “ - 210 */
  0x8003, /* ” - 211 */
  0x8003, /* ‘ - 212 */
  0x8003, /* ’ - 213 */
  0x8003, /* ÷ - 214 */
  0x0008, /* ◊ - 215 */
  0x8003, /* ÿ - 216 */
  0x8003, /* Ÿ - 217 */
  0x8003, /* ⁄ - 218 */
  0x8003, /* € - 219 */
  0x8003, /* ‹ - 220 */
  0x8003, /* › - 221 */
  0x8000, /* ﬁ - 222 */
  0x8000, /* ﬂ - 223 */
  0x8003, /* ‡ - 224 */
  0x8003, /* · - 225 */
  0x8003, /* ‚ - 226 */
  0x8003, /* „ - 227 */
  0x8003, /* ‰ - 228 */
  0x8003, /* Â - 229 */
  0x8000, /* Ê - 230 */
  0x8003, /* Á - 231 */
  0x8003, /* Ë - 232 */
  0x8003, /* È - 233 */
  0x8003, /* Í - 234 */
  0x8003, /* Î - 235 */
  0x8003, /* Ï - 236 */
  0x8003, /* Ì - 237 */
  0x8003, /* Ó - 238 */
  0x8003, /* Ô - 239 */
  0x8000, /*  - 240 */
  0x8003, /* Ò - 241 */
  0x8003, /* Ú - 242 */
  0x8003, /* Û - 243 */
  0x8003, /* Ù - 244 */
  0x8003, /* ı - 245 */
  0x8003, /* ˆ - 246 */
  0x0008, /* ˜ - 247 */
  0x8003, /* ¯ - 248 */
  0x8003, /* ˘ - 249 */
  0x8003, /* ˙ - 250 */
  0x8003, /* ˚ - 251 */
  0x8003, /* ¸ - 252 */
  0x8003, /* ˝ - 253 */
  0x8000, /* ˛ - 254 */
  0x8003  /* ˇ - 255 */
};

static UINT uCodePage = 0;
char *GetTwinFilename();

/* 1252 is the default US Windows ANSI code page */
static UINT SetCodePage(void)
{
    if ( !uCodePage )
	uCodePage = GetPrivateProfileInt("boot.description", "CodePage",
					1252, GetTwinFilename());
    return uCodePage != 1252;
}

LPSTR WINAPI
AnsiNext(LPCSTR lpchCurrentChar)
{
    if (!lpchCurrentChar)
	return (LPSTR)0;

    if (*lpchCurrentChar) {
	if ( IsDBCSLeadByte(*lpchCurrentChar) )
	    return (LPSTR)(lpchCurrentChar+2);
	else
	    return (LPSTR)(lpchCurrentChar+1);
    }
    else
	return (LPSTR)lpchCurrentChar;
}

LPSTR WINAPI
AnsiPrev(LPCSTR lpchStart, LPCSTR lpchCurrentChar)
{
    LPSTR lpPrev = (LPSTR)lpchStart;
    LPSTR lpNext;

    if (lpchStart == lpchCurrentChar)
	return (LPSTR)lpchStart;

    if ( SetCodePage() ) {
	while ((lpNext = AnsiNext((LPCSTR)lpPrev)) != (LPSTR)lpchCurrentChar)
	    lpPrev = lpNext;
	return lpPrev;
    }

    return (LPSTR)(lpchCurrentChar-1);
}
LPSTR   WINAPI 
AnsiUpper(LPSTR lpstr)
{
	LPSTR p;
	if(HIWORD(lpstr) == 0)
		return (LPSTR) toupper(LOWORD((DWORD)lpstr));	
	p = lpstr;
	while((*p = toupper(*p))) p++;
	return lpstr;
}


LPSTR   WINAPI 
AnsiLower(LPSTR lpstr)
{
	LPSTR p;
	if(HIWORD(lpstr) == 0)
		return (LPSTR) tolower(LOWORD((DWORD)lpstr));	
	p = lpstr;
	while((*p = tolower(*p))) p++;
	return lpstr;
}

UINT    WINAPI 
AnsiUpperBuff(LPSTR lpstr, UINT n)
{
	UINT count = n;
	
	if(count == 0)
		count = 65536;	
	while(count) {
		*lpstr = toupper(*lpstr);	
		lpstr++;
		count--;
	}
	return n;
}

UINT    WINAPI 
AnsiLowerBuff(LPSTR lpstr, UINT n) 
{
	UINT count = n;
	
	if(count == 0)
		count = 65536;	
	while(count) {
		*lpstr = tolower(*lpstr);	
		lpstr++;
		count--;
	}
	return n;
}

BOOL    WINAPI 
IsCharUpper(char ch)
{
	APISTR((LF_API,"IsCharUpper(char %c)\n",ch));
	return (BOOL) isupper((int) ch);	
}

#ifdef TWIN32
LPTSTR
CharUpperW(LPTSTR s)
{
	APISTR((LF_API,"CharUpperW(%p)\n",s));
	return CharUpper(s);
}
#endif

BOOL    WINAPI 
IsCharLower(char ch)
{
	APISTR((LF_API,"IsCharLower(char %c)\n",ch));
	return (BOOL) islower((int) ch);	
}

BOOL WINAPI
IsDBCSLeadByte(BYTE bTestChar)
{

    APISTR((LF_API,"IsDBCSLeadByte(char %c)\n",bTestChar));

	SetCodePage();

	switch (uCodePage) {

	    case 936:		/* Chinese (Simplified / Mainland) */
		if (bTestChar >= 0xA1 && bTestChar <= 0xFE)
		    return TRUE;
		break;

	    case 950:		/* Chinese (Traditional / Taiwan) */
		if (bTestChar >= 0x81 && bTestChar <= 0xFE)
		    return TRUE;
		break;

	    case 932:		/* Japanese (Shift-JIS) */
		if ((bTestChar >= 0x81 && bTestChar <= 0x9F) ||
		    (bTestChar >= 0xE0 && bTestChar <= 0xFC) )
		    return TRUE;
		break;

	    case 949:		/* Korean (Wansung) */
		if (bTestChar >= 0x81 && bTestChar <= 0xFE)
		    return TRUE;
		break;

	    case 1361:		/* Korean (Johab) */
		if ((bTestChar >= 0x84 && bTestChar <= 0xD3) ||
		    (bTestChar >= 0xD8 && bTestChar <= 0xDE) ||
		    (bTestChar >= 0xE0 && bTestChar <= 0xF9) )
		    return TRUE;
		break;

	}

	return FALSE;

}

void    WINAPI 
hmemcpy(void _huge* d, const void _huge* s, long n)
{
	memcpy(d,s,n);
}

LPSTR
itoa(int value, LPSTR string, int radix)
{
    char temp[256];
    int i=0, j=0;

    switch (radix) {
	case 2:
    sprintf(temp, "%x", value);
    for(i = 0; i < (int)strlen(temp); i++)
    {
      switch(temp[i])
      {
      case '0':
	string[j]   = '0';
	string[j+1] = '0';
	string[j+2] = '0';
	string[j+3] = '0';
	j += 4;
	break;
      case '1':
	string[j]   = '0';
	string[j+1] = '0';
	string[j+2] = '0';
	string[j+3] = '1';
	j += 4;
	break;
      case '2':
	string[j]   = '0';
	string[j+1] = '0';
	string[j+2] = '1';
	string[j+3] = '0';
	j += 4;
	break;
      case '3':
	string[j]   = '0';
	string[j+1] = '0';
	string[j+2] = '1';
	string[j+3] = '1';
	j += 4;
	break;
      case '4':
	string[j]   = '0';
	string[j+1] = '0';
	string[j+2] = '1';
	string[j+3] = '0';
	j += 4;
	break;
      case '5':
	string[j]   = '0';
	string[j+1] = '1';
	string[j+2] = '0';
	string[j+3] = '1';
	j += 4;
	break;
      case '6':
	string[j]   = '0';
	string[j+1] = '1';
	string[j+2] = '1';
	string[j+3] = '0';
	j += 4;
	break;
      case '7':
	string[j] = '0';
	string[j+1] = '1';
	string[j+2] = '1';
	string[j+3] = '1';
	j += 4;
	break;
      case '8':
	string[j]   = '1';
	string[j+1] = '0';
	string[j+2] = '0';
	string[j+3] = '0';
	j += 4;
	break;
      case '9':
       	string[j]   = '1';
	string[j+1] = '0';
	string[j+2] = '0';
	string[j+3] = '1';
	j += 4;
	break;
      case 'A': case 'a':
       	string[j]   = '1';
	string[j+1] = '0';
	string[j+2] = '1';
	string[j+3] = '0';
	j += 4;
	break;
      case 'B': case 'b':
       	string[j]   = '1';
	string[j+1] = '0';
	string[j+2] = '1';
	string[j+3] = '1';
	j += 4;
	break;
      case 'C': case 'c':
       	string[j]   = '1';
	string[j+1] = '1';
	string[j+2] = '0';
	string[j+3] = '0';
	j += 4;
	break;
      case 'D': case 'd':
       	string[j]   = '1';
	string[j+1] = '1';
	string[j+2] = '0';
	string[j+3] = '1';
	j += 4;
	break;
      case 'E': case 'e':
       	string[j]   = '1';
	string[j+1] = '1';
	string[j+2] = '1';
	string[j+3] = '0';
	j += 4;
	break;
      case 'F': case 'f':
       	string[j]   = '1';
	string[j+1] = '1';
	string[j+2] = '1';
	string[j+3] = '1';
	j += 4;
	break;
      }
    }
    string[j] = '\0';
    break;
	case 8:
	    sprintf(string,"%o",value);
	    break;
	case 10:
	    sprintf(string,"%d",value);
	    break;
	case 16:
	    sprintf(string,"%x",value);
	    break;
	default: 
	    sprintf(string,"%d[%d]",value,radix);
	    break;
	}

    return string;
}

LPSTR
strpbrkr(LPCSTR szStr, LPCSTR szTempl)
{
	int nLen, i, j, nEq = 0;

	nLen = strlen(szTempl);
	if (strlen(szStr) == 0)
	    	return (LPSTR)NULL;
	
	for ( i = strlen(szStr) - 1; i >= 0; i-- )
	    {
		for ( j = 0, nEq = 0; j < nLen; j++ )
			if ( *(szStr+i) == *(szTempl+j) )
			   {
				nEq++;
				break;
			   }
		if ( nEq )
			break;
		else
			nEq = 0;
	    }
	if ( nEq )
		return (LPSTR)(szStr+i);
	else
		return (LPSTR)NULL;
}
 
/*
	This routine is used to swap all occurences of a specific
	character in the first string with a replacement character.

	For example:	"\BIN\TEST.EXE" -> "/BIN/TEST.EXE"

		strswpchr("\BIN\TEST.EXE", '\\', '/');
*/
int
strswpchr(LPSTR szStr, char chFind, char chSwap)
{
	LPSTR szPtr;
	int Count = 0;

	szPtr = szStr;

	while ( *szPtr ) {
		if ( *szPtr == chFind ) {
			*szPtr = chSwap;
			Count++;
		}
		szPtr++;
	}

	return Count;
}

LPSTR WINAPI
lstrcpy(LPSTR lpszString1, LPCSTR lpszString2)
{
    if (!lpszString1 || !lpszString2)
	return (LPSTR)0;

    return strcpy(lpszString1,lpszString2);
}

LPSTR WINAPI
lstrcpyn(LPSTR lpszString1, LPCSTR lpszString2, int cChars)
{
    if (!lpszString1 || !lpszString2)
	return (LPSTR)0;

    strncpy(lpszString1,lpszString2,cChars);

    lpszString1[cChars-1] = '\0';	/* force null character */

    return lpszString1;
}

int WINAPI
lstrcmp(LPCSTR lpszString1,LPCSTR lpszString2)
{
    if (!lpszString1 || !lpszString2)
	return (lpszString2-lpszString1);

    LOGSTR((LF_API,"lstrcmp: %s %s\n",lpszString1,lpszString2));

    return strcmp(lpszString1,lpszString2);
}

int WINAPI
lstrcmpi(LPCSTR lpszString1,LPCSTR lpszString2)
{
    if (!lpszString1 || !lpszString2)
	return (lpszString2-lpszString1);

    LOGSTR((LF_API,"lstrcmpi: %s %s\n",lpszString1,lpszString2));

    return strcasecmp(lpszString1,lpszString2);
}

int WINAPI
lstrlen(LPCSTR lpszString)
{
    if (!lpszString)
	return 0;

    return strlen(lpszString);
}

LPSTR WINAPI
lstrcat(LPSTR lpszString1, LPCSTR lpszString2)
{
    return strcat(lpszString1,lpszString2);
}

/* (WIN32) Character Conversion ******************************************** */

LPTSTR	WINAPI
CharLower(LPTSTR lpstr)
{
	LPTSTR str;

	APISTR((LF_API, "CharLower: (API) lpstr %p\n", (void *) lpstr));

	if (HIWORD(lpstr) == 0)
		return ((LPTSTR) tolower(LOWORD((DWORD)lpstr)));	
	for (str = lpstr; (*str = tolower(*str)); str++)
		;
	return (lpstr);

}

LPTSTR	WINAPI
CharUpper(LPTSTR lpstr)
{
	LPTSTR str;

	APISTR((LF_API, "CharUpper: (API) lpstr %p\n", (void *) lpstr));

	if (HIWORD(lpstr) == 0)
		return ((LPTSTR) toupper(LOWORD((DWORD)lpstr)));	
	for (str = lpstr; (*str = toupper(*str)); str++)
		;
	return (lpstr);

}

DWORD	WINAPI
CharLowerBuff(LPTSTR lpstr, DWORD len)
{
	DWORD i;

	APISTR((LF_API, "CharLowerBuff: (API) lpstr %p len %ld\n",
		(void *) lpstr, len));

	if (!lpstr)
		return (0);
	for (i = 0; i < len; i++, lpstr++)
		*lpstr = tolower(*lpstr);
	return (i);

}

DWORD	WINAPI
CharUpperBuff(LPTSTR lpstr, DWORD len)
{
	DWORD i;

	APISTR((LF_API, "CharUpperBuff: (API) lpstr %p len %ld\n",
		(void *) lpstr, len));

	if (!lpstr)
		return (0);
	for (i = 0; i < len; i++, lpstr++)
		*lpstr = toupper(*lpstr);
	return (i);

}

LPTSTR	WINAPI
CharNext(LPCTSTR lpstr)
{
	APISTR((LF_API, "CharNext: (API) lpstr %p\n", (void *) lpstr));
	if (!lpstr)
		return ((LPTSTR) 0);
	if (*lpstr)
		return ((LPTSTR) (lpstr + 1));
	return ((LPTSTR) lpstr);
}

LPTSTR	WINAPI
CharNextEx(WORD CodePage,LPCTSTR lpstr,DWORD dwFlags)
{
	APISTR((LF_API, "CharNext: (API) lpstr %p\n", (void *) lpstr));
	if (!lpstr)
		return ((LPTSTR) 0);
	if (*lpstr)
		return ((LPTSTR) (lpstr + 1));
	return ((LPTSTR) lpstr);
}

LPTSTR	WINAPI
CharPrev(LPCTSTR lpstr0, LPCTSTR lpstr)
{
	APISTR((LF_API, "CharPrev: (API) lpstr0 %p lpstr %p\n",
		(void *) lpstr0, (void *) lpstr));
	if (!lpstr0 || !lpstr)
		return ((LPTSTR) 0);
	if (lpstr0 == lpstr)
		return ((LPTSTR) lpstr);
	return ((LPTSTR) (lpstr - 1));
}

LPTSTR	WINAPI
CharPrevEx(DWORD CodePage,LPCTSTR lpstr0, LPCTSTR lpstr,DWORD dwFlags)
{
	APISTR((LF_API, "CharPrevEx: (API) lpstr0 %p lpstr %p\n",
		(void *) lpstr0, (void *) lpstr));
	if (!lpstr0 || !lpstr)
		return ((LPTSTR) 0);
	if (lpstr0 == lpstr)
		return ((LPTSTR) lpstr);
	return ((LPTSTR) (lpstr - 1));
}

/***********************************************************************
 *           IsCharAlpha   (USER32.331)
 * FIXME: handle current locale
 */
BOOL WINAPI
IsCharAlpha(char x)
{
    APISTR((LF_API,"IsCharAlpha(char=%c)\n",x));
    return (OLE2NLS_CT_CType3_LUT[(unsigned char) x] & C3_ALPHA);
}

/***********************************************************************
 *           IsCharAlphaNumeric   (USER32.332)
 * FIXME: handle current locale
 */
BOOL WINAPI
IsCharAlphaNumeric(char ch)
{
    APISTR((LF_API,"IsCharAlphaNumeric(char=%c)\n",ch));
    return IsCharAlpha(ch) || isdigit(ch) ;
}

static const BYTE STRING_Oem2Ansi[256] =
"\000\001\002\003\004\005\006\007\010\011\012\013\014\015\016\244"
"\020\021\022\023\266\247\026\027\030\031\032\033\034\035\036\037"
"\040\041\042\043\044\045\046\047\050\051\052\053\054\055\056\057"
"\060\061\062\063\064\065\066\067\070\071\072\073\074\075\076\077"
"\100\101\102\103\104\105\106\107\110\111\112\113\114\115\116\117"
"\120\121\122\123\124\125\126\127\130\131\132\133\134\135\136\137"
"\140\141\142\143\144\145\146\147\150\151\152\153\154\155\156\157"
"\160\161\162\163\164\165\166\167\170\171\172\173\174\175\176\177"
"\307\374\351\342\344\340\345\347\352\353\350\357\356\354\304\305"
"\311\346\306\364\366\362\373\371\377\326\334\242\243\245\120\203"
"\341\355\363\372\361\321\252\272\277\137\254\275\274\241\253\273"
"\137\137\137\246\246\246\246\053\053\246\246\053\053\053\053\053"
"\053\055\055\053\055\053\246\246\053\053\055\055\246\055\053\055"
"\055\055\055\053\053\053\053\053\053\053\053\137\137\246\137\137"
"\137\337\137\266\137\137\265\137\137\137\137\137\137\137\137\137"
"\137\261\137\137\137\137\367\137\260\225\267\137\156\262\137\137";

static const BYTE STRING_Ansi2Oem[256] =
"\000\001\002\003\004\005\006\007\010\011\012\013\014\015\016\017"
"\020\021\022\023\024\025\026\027\030\031\032\033\034\035\036\037"
"\040\041\042\043\044\045\046\047\050\051\052\053\054\055\056\057"
"\060\061\062\063\064\065\066\067\070\071\072\073\074\075\076\077"
"\100\101\102\103\104\105\106\107\110\111\112\113\114\115\116\117"
"\120\121\122\123\124\125\126\127\130\131\132\133\134\135\136\137"
"\140\141\142\143\144\145\146\147\150\151\152\153\154\155\156\157"
"\160\161\162\163\164\165\166\167\170\171\172\173\174\175\176\177"
"\200\201\054\237\054\137\375\374\210\045\123\074\117\215\216\217"
"\220\140\047\042\042\371\055\137\230\231\163\076\157\235\236\131"
"\040\255\233\234\017\235\335\025\042\143\246\256\252\055\162\137"
"\370\361\375\063\047\346\024\372\054\061\247\257\254\253\137\250"
"\101\101\101\101\216\217\222\200\105\220\105\105\111\111\111\111"
"\104\245\117\117\117\117\231\170\117\125\125\125\232\131\137\341"
"\205\240\203\141\204\206\221\207\212\202\210\211\215\241\214\213"
"\144\244\225\242\223\157\224\366\157\227\243\226\201\171\137\230";

#define OEM_TO_ANSI(ch) (STRING_Oem2Ansi[(unsigned char)(ch)])
#define ANSI_TO_OEM(ch) (STRING_Ansi2Oem[(unsigned char)(ch)])

/***********************************************************************
 *           OemToChar   (USER32.402)
 */
BOOL WINAPI
OemToChar( LPCSTR s, LPSTR d )
{
    while ((*d++ = OEM_TO_ANSI(*s++)));
    return TRUE;
}


/***********************************************************************
 *           OemToCharBuff   (USER32.403)
 */
BOOL WINAPI
OemToCharBuff( LPCSTR s, LPSTR d, DWORD len )
{
    while (len--) *d++ = OEM_TO_ANSI(*s++);
    return TRUE;
}

BOOL WINAPI 
GetStringTypeEx(LCID locale,DWORD dwInfoType,LPCSTR src,
                                 INT cchSrc,LPWORD chartype)
{
	int	i;
	
	if ((src==NULL) || (chartype==NULL) || (src==(LPSTR)chartype))
	{
	  SetLastError(ERROR_INVALID_PARAMETER);
	  return FALSE;
	}

	if (cchSrc==-1)
	  cchSrc=lstrlen(src)+1;
	  
	switch (dwInfoType) {
	case CT_CTYPE1:
	  for (i=0;i<cchSrc;i++) 
	  {
	    chartype[i] = 0;
	    if (isdigit(src[i])) chartype[i]|=C1_DIGIT;
	    if (isalpha(src[i])) chartype[i]|=C1_ALPHA;
	    if (islower(src[i])) chartype[i]|=C1_LOWER;
	    if (isupper(src[i])) chartype[i]|=C1_UPPER;
	    if (isspace(src[i])) chartype[i]|=C1_SPACE;
	    if (ispunct(src[i])) chartype[i]|=C1_PUNCT;
	    if (iscntrl(src[i])) chartype[i]|=C1_CNTRL;
/* FIXME: isblank() is a GNU extension */
/*		if (isblank(src[i])) chartype[i]|=C1_BLANK; */
	    if ((src[i] == ' ') || (src[i] == '\t')) chartype[i]|=C1_BLANK;
	    /* C1_XDIGIT */
	}
	return TRUE;

	case CT_CTYPE2:
	  for (i=0;i<cchSrc;i++) 
	  {
	    chartype[i]=(WORD)CT_CType2_LUT[i];
	  }
	  return TRUE;

	case CT_CTYPE3:
	  for (i=0;i<cchSrc;i++) 
	  {
	    chartype[i]=OLE2NLS_CT_CType3_LUT[i];
	  }
	  return TRUE;

	default:
	  logstr(LF_WARNING,"Unknown dwInfoType:%ld\n",dwInfoType);
	  return FALSE;
	}
}

BOOL
CharToOem(LPCTSTR lpszSrc,LPSTR lpszDst)
{
	memcpy(lpszDst,lpszSrc,strlen(lpszSrc));
	return TRUE;
}
