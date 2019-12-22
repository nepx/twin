
#include "Strings32.h"

#ifndef _locale_h_
#define _locale_h_

/*
 *  Locale Enumeration Flags.
 */
#define LCID_INSTALLED          0x00000001  /* installed locale ids */
#define LCID_SUPPORTED          0x00000002  /* supported locale ids */

/*
 *  Code Page Enumeration Flags.
 */
#define CP_INSTALLED            0x00000001  /* installed code page ids */
#define CP_SUPPORTED            0x00000002  /* supported code page ids */

#define CP_ACP               0              /* default to ANSI code page */
#define CP_OEMCP             1              /* default to OEM  code page */
#define CP_MACCP             2              /* default to MAC  code page */

/*
 *  Country Codes.
 */
#define CTRY_DEFAULT                     0

#define CTRY_AUSTRALIA                   61      /* Australia */
#define CTRY_AUSTRIA                     43      /* Austria */
#define CTRY_BELGIUM                     32      /* Belgium */
#define CTRY_BRAZIL                      55      /* Brazil */
#define CTRY_BULGARIA                    359     /* Bulgaria */
#define CTRY_CANADA                      2       /* Canada */
#define CTRY_CROATIA                     385     /* Croatia */
#define CTRY_CZECH                       42      /* Czech Republic */
#define CTRY_DENMARK                     45      /* Denmark */
#define CTRY_FINLAND                     358     /* Finland */
#define CTRY_FRANCE                      33      /* France */
#define CTRY_GERMANY                     49      /* Germany */
#define CTRY_GREECE                      30      /* Greece */
#define CTRY_HONG_KONG                   852     /* Hong Kong */
#define CTRY_HUNGARY                     36      /* Hungary */
#define CTRY_ICELAND                     354     /* Iceland */
#define CTRY_IRELAND                     353     /* Ireland */
#define CTRY_ITALY                       39      /* Italy */
#define CTRY_JAPAN                       81      /* Japan */
#define CTRY_MEXICO                      52      /* Mexico */
#define CTRY_NETHERLANDS                 31      /* Netherlands */
#define CTRY_NEW_ZEALAND                 64      /* New Zealand */
#define CTRY_NORWAY                      47      /* Norway */
#define CTRY_POLAND                      48      /* Poland */
#define CTRY_PORTUGAL                    351     /* Portugal */
#define CTRY_PRCHINA                     86      /* Peoples' Republic of China */
#define CTRY_ROMANIA                     40      /* Romania */
#define CTRY_RUSSIA                      7       /* Russia */
#define CTRY_SINGAPORE                   65      /* Singapore */
#define CTRY_SLOVAK                      42      /* Slovak Republic */
#define CTRY_SLOVENIA                    386     /* Slovenia */
#define CTRY_SOUTH_KOREA                 82      /* South Korea */
#define CTRY_SPAIN                       34      /* Spain */
#define CTRY_SWEDEN                      46      /* Sweden */
#define CTRY_SWITZERLAND                 41      /* Switzerland */
#define CTRY_TAIWAN                      886     /* Taiwan */
#define CTRY_TURKEY                      90      /* Turkey */
#define CTRY_UNITED_KINGDOM              44      /* United Kingdom */
#define CTRY_UNITED_STATES               1       /* United States */


#define LOCALE_NOUSEROVERRIDE       0x80000000   /* do not use user overrides */
#define LOCALE_USE_CP_ACP           0x40000000   /* use the system ACP */

/*
 *  Locale Types.
 */
#define LOCALE_ILANGUAGE            0x00000001   /* language id */
#define LOCALE_SLANGUAGE            0x00000002   /* localized name of language */
#define LOCALE_SENGLANGUAGE         0x00001001   /* English name of language */
#define LOCALE_SABBREVLANGNAME      0x00000003   /* abbreviated language name */
#define LOCALE_SNATIVELANGNAME      0x00000004   /* native name of language */

#define LOCALE_ICOUNTRY             0x00000005   /* country code */
#define LOCALE_SCOUNTRY             0x00000006   /* localized name of country */
#define LOCALE_SENGCOUNTRY          0x00001002   /* English name of country */
#define LOCALE_SABBREVCTRYNAME      0x00000007   /* abbreviated country name */
#define LOCALE_SNATIVECTRYNAME      0x00000008   /* native name of country */

#define LOCALE_IDEFAULTLANGUAGE     0x00000009   /* default language id */
#define LOCALE_IDEFAULTCOUNTRY      0x0000000A   /* default country code */
#define LOCALE_IDEFAULTCODEPAGE     0x0000000B   /* default oem code page */
#define LOCALE_IDEFAULTANSICODEPAGE 0x00001004   /* default ansi code page */

#define LOCALE_SLIST                0x0000000C   /* list item separator */
#define LOCALE_IMEASURE             0x0000000D   /* 0 = metric, 1 = US */

#define LOCALE_SDECIMAL             0x0000000E   /* decimal separator */
#define LOCALE_STHOUSAND            0x0000000F   /* thousand separator */
#define LOCALE_SGROUPING            0x00000010   /* digit grouping */
#define LOCALE_IDIGITS              0x00000011   /* number of fractional digits */
#define LOCALE_ILZERO               0x00000012   /* leading zeros for decimal */
#define LOCALE_INEGNUMBER           0x00001010   /* negative number mode */
#define LOCALE_SNATIVEDIGITS        0x00000013   /* native ascii 0-9 */

#define LOCALE_SCURRENCY            0x00000014   /* local monetary symbol */
#define LOCALE_SINTLSYMBOL          0x00000015   /* intl monetary symbol */
#define LOCALE_SMONDECIMALSEP       0x00000016   /* monetary decimal separator */
#define LOCALE_SMONTHOUSANDSEP      0x00000017   /* monetary thousand separator */
#define LOCALE_SMONGROUPING         0x00000018   /* monetary grouping */
#define LOCALE_ICURRDIGITS          0x00000019   /* # local monetary digits */
#define LOCALE_IINTLCURRDIGITS      0x0000001A   /* # intl monetary digits */
#define LOCALE_ICURRENCY            0x0000001B   /* positive currency mode */
#define LOCALE_INEGCURR             0x0000001C   /* negative currency mode */

#define LOCALE_SDATE                0x0000001D   /* date separator */
#define LOCALE_STIME                0x0000001E   /* time separator */
#define LOCALE_SSHORTDATE           0x0000001F   /* short date format string */
#define LOCALE_SLONGDATE            0x00000020   /* long date format string */
#define LOCALE_STIMEFORMAT          0x00001003   /* time format string */
#define LOCALE_IDATE                0x00000021   /* short date format ordering */
#define LOCALE_ILDATE               0x00000022   /* long date format ordering */
#define LOCALE_ITIME                0x00000023   /* time format specifier */
#define LOCALE_ITIMEMARKPOSN        0x00001005   /* time marker position */
#define LOCALE_ICENTURY             0x00000024   /* century format specifier (short date) */
#define LOCALE_ITLZERO              0x00000025   /* leading zeros in time field */
#define LOCALE_IDAYLZERO            0x00000026   /* leading zeros in day field (short date) */
#define LOCALE_IMONLZERO            0x00000027   /* leading zeros in month field (short date) */
#define LOCALE_S1159                0x00000028   /* AM designator */
#define LOCALE_S2359                0x00000029   /* PM designator */

#define LOCALE_ICALENDARTYPE        0x00001009   /* type of calendar specifier */
#define LOCALE_IOPTIONALCALENDAR    0x0000100B   /* additional calendar types specifier */
#define LOCALE_IFIRSTDAYOFWEEK      0x0000100C   /* first day of week specifier */
#define LOCALE_IFIRSTWEEKOFYEAR     0x0000100D   /* first week of year specifier */

#define LOCALE_SDAYNAME1            0x0000002A   /* long name for Monday */
#define LOCALE_SDAYNAME2            0x0000002B   /* long name for Tuesday */
#define LOCALE_SDAYNAME3            0x0000002C   /* long name for Wednesday */
#define LOCALE_SDAYNAME4            0x0000002D   /* long name for Thursday */
#define LOCALE_SDAYNAME5            0x0000002E   /* long name for Friday */
#define LOCALE_SDAYNAME6            0x0000002F   /* long name for Saturday */
#define LOCALE_SDAYNAME7            0x00000030   /* long name for Sunday */
#define LOCALE_SABBREVDAYNAME1      0x00000031   /* abbreviated name for Monday */
#define LOCALE_SABBREVDAYNAME2      0x00000032   /* abbreviated name for Tuesday */
#define LOCALE_SABBREVDAYNAME3      0x00000033   /* abbreviated name for Wednesday */
#define LOCALE_SABBREVDAYNAME4      0x00000034   /* abbreviated name for Thursday */
#define LOCALE_SABBREVDAYNAME5      0x00000035   /* abbreviated name for Friday */
#define LOCALE_SABBREVDAYNAME6      0x00000036   /* abbreviated name for Saturday */
#define LOCALE_SABBREVDAYNAME7      0x00000037   /* abbreviated name for Sunday */
#define LOCALE_SMONTHNAME1          0x00000038   /* long name for January */
#define LOCALE_SMONTHNAME2          0x00000039   /* long name for February */
#define LOCALE_SMONTHNAME3          0x0000003A   /* long name for March */
#define LOCALE_SMONTHNAME4          0x0000003B   /* long name for April */
#define LOCALE_SMONTHNAME5          0x0000003C   /* long name for May */
#define LOCALE_SMONTHNAME6          0x0000003D   /* long name for June */
#define LOCALE_SMONTHNAME7          0x0000003E   /* long name for July */
#define LOCALE_SMONTHNAME8          0x0000003F   /* long name for August */
#define LOCALE_SMONTHNAME9          0x00000040   /* long name for September */
#define LOCALE_SMONTHNAME10         0x00000041   /* long name for October */
#define LOCALE_SMONTHNAME11         0x00000042   /* long name for November */
#define LOCALE_SMONTHNAME12         0x00000043   /* long name for December */
#define LOCALE_SMONTHNAME13         0x0000100E   /* long name for 13th month (if exists) */
#define LOCALE_SABBREVMONTHNAME1    0x00000044   /* abbreviated name for January */
#define LOCALE_SABBREVMONTHNAME2    0x00000045   /* abbreviated name for February */
#define LOCALE_SABBREVMONTHNAME3    0x00000046   /* abbreviated name for March */
#define LOCALE_SABBREVMONTHNAME4    0x00000047   /* abbreviated name for April */
#define LOCALE_SABBREVMONTHNAME5    0x00000048   /* abbreviated name for May */
#define LOCALE_SABBREVMONTHNAME6    0x00000049   /* abbreviated name for June */
#define LOCALE_SABBREVMONTHNAME7    0x0000004A   /* abbreviated name for July */
#define LOCALE_SABBREVMONTHNAME8    0x0000004B   /* abbreviated name for August */
#define LOCALE_SABBREVMONTHNAME9    0x0000004C   /* abbreviated name for September */
#define LOCALE_SABBREVMONTHNAME10   0x0000004D   /* abbreviated name for October */
#define LOCALE_SABBREVMONTHNAME11   0x0000004E   /* abbreviated name for November */
#define LOCALE_SABBREVMONTHNAME12   0x0000004F   /* abbreviated name for December */
#define LOCALE_SABBREVMONTHNAME13   0x0000100F   /* abbreviated name for 13th month (if exists) */

#define LOCALE_SPOSITIVESIGN        0x00000050   /* positive sign */
#define LOCALE_SNEGATIVESIGN        0x00000051   /* negative sign */
#define LOCALE_IPOSSIGNPOSN         0x00000052   /* positive sign position */
#define LOCALE_INEGSIGNPOSN         0x00000053   /* negative sign position */
#define LOCALE_IPOSSYMPRECEDES      0x00000054   /* mon sym precedes pos amt */
#define LOCALE_IPOSSEPBYSPACE       0x00000055   /* mon sym sep by space from pos amt */
#define LOCALE_INEGSYMPRECEDES      0x00000056   /* mon sym precedes neg amt */
#define LOCALE_INEGSEPBYSPACE       0x00000057   /* mon sym sep by space from neg amt */

#define LOCALE_FONTSIGNATURE        0x00000058   /* font signature */
#define LOCALE_SISO639LANGNAME      0x00000059   /* ISO abbreviated language name */
#define LOCALE_SISO3166CTRYNAME     0x0000005A   /* ISO abbreviated country name */
#endif
