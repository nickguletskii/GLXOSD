--[[
Copyright (C) 2016 Nick Guletskii

Permission is hereby granted, free of charge, to any person obtaining a copy of
this software and associated documentation files (the "Software"), to deal in
the Software without restriction, including without limitation the rights to
use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of
the Software, and to permit persons to whom the Software is furnished to do so,
subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

This file is an adaptation of fontconfig/fontconfig/fontconfig.h
from the fontconfig project. Therefore, the following license applies:

* Copyright © 2001 Keith Packard
*
* Permission to use, copy, modify, distribute, and sell this software and its
* documentation for any purpose is hereby granted without fee, provided that
* the above copyright notice appear in all copies and that both that
* copyright notice and this permission notice appear in supporting
* documentation, and that the name of the author(s) not be used in
* advertising or publicity pertaining to distribution of the software without
* specific, written prior permission.  The authors make no
* representations about the suitability of this software for any purpose.  It
* is provided "as is" without express or implied warranty.
*
* THE AUTHOR(S) DISCLAIMS ALL WARRANTIES WITH REGARD TO THIS SOFTWARE,
* INCLUDING ALL IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS, IN NO
* EVENT SHALL THE AUTHOR(S) BE LIABLE FOR ANY SPECIAL, INDIRECT OR
* CONSEQUENTIAL DAMAGES OR ANY DAMAGES WHATSOEVER RESULTING FROM LOSS OF USE,
* DATA OR PROFITS, WHETHER IN AN ACTION OF CONTRACT, NEGLIGENCE OR OTHER
* TORTIOUS ACTION, ARISING OUT OF OR IN CONNECTION WITH THE USE OR
* PERFORMANCE OF THIS SOFTWARE.
]]

local ffi = require("ffi")

FcTrue = 1
FcFalse = 0
FC_FAMILY = "family"
FC_STYLE = "style"
FC_SLANT = "slant"
FC_WEIGHT = "weight"
FC_SIZE = "size"
FC_ASPECT = "aspect"
FC_PIXEL_SIZE = "pixelsize"
FC_SPACING = "spacing"
FC_FOUNDRY = "foundry"
FC_ANTIALIAS = "antialias"
FC_HINTING = "hinting"
FC_HINT_STYLE = "hintstyle"
FC_VERTICAL_LAYOUT = "verticallayout"
FC_AUTOHINT = "autohint"
FC_GLOBAL_ADVANCE = "globaladvance"
FC_WIDTH = "width"
FC_FILE = "file"
FC_INDEX = "index"
FC_FT_FACE = "ftface"
FC_RASTERIZER = "rasterizer"
FC_OUTLINE = "outline"
FC_SCALABLE = "scalable"
FC_COLOR = "color"
FC_SCALE = "scale"
FC_SYMBOL = "symbol"
FC_DPI = "dpi"
FC_RGBA = "rgba"
FC_MINSPACE = "minspace"
FC_SOURCE = "source"
FC_CHARSET = "charset"
FC_LANG = "lang"
FC_FONTVERSION = "fontversion"
FC_FULLNAME = "fullname"
FC_FAMILYLANG = "familylang"
FC_STYLELANG = "stylelang"
FC_FULLNAMELANG = "fullnamelang"
FC_CAPABILITY = "capability"
FC_FONTFORMAT = "fontformat"
FC_EMBOLDEN = "embolden"
FC_EMBEDDED_BITMAP = "embeddedbitmap"
FC_DECORATIVE = "decorative"
FC_LCD_FILTER = "lcdfilter"
FC_FONT_FEATURES = "fontfeatures"
FC_NAMELANG = "namelang"
FC_PRGNAME = "prgname"
FC_HASH = "hash"
FC_POSTSCRIPT_NAME = "postscriptname"
FC_CACHE_SUFFIX = ".cache-"
FC_DIR_CACHE_FILE = "fonts.cache-"
FC_USER_CACHE_FILE = ".fonts.cache-"
FC_CHAR_WIDTH = "charwidth"
FC_CHAR_HEIGHT = "charheight"
FC_MATRIX = "matrix"
FC_WEIGHT_THIN = 0
FC_WEIGHT_EXTRALIGHT = 40
FC_WEIGHT_ULTRALIGHT = FC_WEIGHT_EXTRALIGHT
FC_WEIGHT_LIGHT = 50
FC_WEIGHT_DEMILIGHT = 55
FC_WEIGHT_SEMILIGHT = FC_WEIGHT_DEMILIGHT
FC_WEIGHT_BOOK = 75
FC_WEIGHT_REGULAR = 80
FC_WEIGHT_NORMAL = FC_WEIGHT_REGULAR
FC_WEIGHT_MEDIUM = 100
FC_WEIGHT_DEMIBOLD = 180
FC_WEIGHT_SEMIBOLD = FC_WEIGHT_DEMIBOLD
FC_WEIGHT_BOLD = 200
FC_WEIGHT_EXTRABOLD = 205
FC_WEIGHT_ULTRABOLD = FC_WEIGHT_EXTRABOLD
FC_WEIGHT_BLACK = 210
FC_WEIGHT_HEAVY = FC_WEIGHT_BLACK
FC_WEIGHT_EXTRABLACK = 215
FC_WEIGHT_ULTRABLACK = FC_WEIGHT_EXTRABLACK
FC_SLANT_ROMAN = 0
FC_SLANT_ITALIC = 100
FC_SLANT_OBLIQUE = 110
FC_WIDTH_ULTRACONDENSED = 50
FC_WIDTH_EXTRACONDENSED = 63
FC_WIDTH_CONDENSED = 75
FC_WIDTH_SEMICONDENSED = 87
FC_WIDTH_NORMAL = 100
FC_WIDTH_SEMIEXPANDED = 113
FC_WIDTH_EXPANDED = 125
FC_WIDTH_EXTRAEXPANDED = 150
FC_WIDTH_ULTRAEXPANDED = 200
FC_PROPORTIONAL = 0
FC_DUAL = 90
FC_MONO = 100
FC_CHARCELL = 110
FC_RGBA_UNKNOWN = 0
FC_RGBA_RGB = 1
FC_RGBA_BGR = 2
FC_RGBA_VRGB = 3
FC_RGBA_VBGR = 4
FC_RGBA_NONE = 5
FC_HINT_NONE = 0
FC_HINT_SLIGHT = 1
FC_HINT_MEDIUM = 2
FC_HINT_FULL = 3
FC_LCD_NONE = 0
FC_LCD_DEFAULT = 1
FC_LCD_LIGHT = 2
FC_LCD_LEGACY = 3
FC_UTF8_MAX_LEN = 6

ffi.cdef[[
/*
 * fontconfig/fontconfig/fontconfig.h
 *
 * Copyright © 2001 Keith Packard
 *
 * Permission to use, copy, modify, distribute, and sell this software and its
 * documentation for any purpose is hereby granted without fee, provided that
 * the above copyright notice appear in all copies and that both that
 * copyright notice and this permission notice appear in supporting
 * documentation, and that the name of the author(s) not be used in
 * advertising or publicity pertaining to distribution of the software without
 * specific, written prior permission.  The authors make no
 * representations about the suitability of this software for any purpose.  It
 * is provided "as is" without express or implied warranty.
 *
 * THE AUTHOR(S) DISCLAIMS ALL WARRANTIES WITH REGARD TO THIS SOFTWARE,
 * INCLUDING ALL IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS, IN NO
 * EVENT SHALL THE AUTHOR(S) BE LIABLE FOR ANY SPECIAL, INDIRECT OR
 * CONSEQUENTIAL DAMAGES OR ANY DAMAGES WHATSOEVER RESULTING FROM LOSS OF USE,
 * DATA OR PROFITS, WHETHER IN AN ACTION OF CONTRACT, NEGLIGENCE OR OTHER
 * TORTIOUS ACTION, ARISING OUT OF OR IN CONNECTION WITH THE USE OR
 * PERFORMANCE OF THIS SOFTWARE.
 */

typedef unsigned char	FcChar8;
typedef unsigned short	FcChar16;
typedef unsigned int	FcChar32;
typedef int		FcBool;

typedef enum _FcType {
    FcTypeUnknown = -1,
    FcTypeVoid,
    FcTypeInteger,
    FcTypeDouble,
    FcTypeString,
    FcTypeBool,
    FcTypeMatrix,
    FcTypeCharSet,
    FcTypeFTFace,
    FcTypeLangSet,
    FcTypeRange
} FcType;

typedef struct _FcMatrix {
    double xx, xy, yx, yy;
} FcMatrix;

typedef struct _FcCharSet FcCharSet;

typedef struct _FcObjectType {
    char	*object;
    FcType	type;
} FcObjectType;

typedef struct _FcConstant {
    const FcChar8  *name;
    const char	*object;
    int		value;
} FcConstant;

typedef enum _FcResult {
    FcResultMatch, FcResultNoMatch, FcResultTypeMismatch, FcResultNoId,
    FcResultOutOfMemory
} FcResult;

typedef struct _FcPattern   FcPattern;

typedef struct _FcLangSet   FcLangSet;

typedef struct _FcRange	    FcRange;

typedef struct _FcValue {
    FcType	type;
    union {
	const FcChar8	*s;
	int		i;
	FcBool		b;
	double		d;
	const FcMatrix	*m;
	const FcCharSet	*c;
	void		*f;
	const FcLangSet	*l;
	const FcRange	*r;
    } u;
} FcValue;

typedef struct _FcFontSet {
    int		nfont;
    int		sfont;
    FcPattern	**fonts;
} FcFontSet;

typedef struct _FcObjectSet {
    int		nobject;
    int		sobject;
    const char	**objects;
} FcObjectSet;
    
typedef enum _FcMatchKind {
    FcMatchPattern, FcMatchFont, FcMatchScan
} FcMatchKind;

typedef enum _FcLangResult {
    FcLangEqual = 0,
    FcLangDifferentCountry = 1,
    FcLangDifferentTerritory = 1,
    FcLangDifferentLang = 2
} FcLangResult;

typedef enum _FcSetName {
    FcSetSystem = 0,
    FcSetApplication = 1
} FcSetName;

typedef struct _FcAtomic FcAtomic;

typedef enum { FcEndianBig, FcEndianLittle } FcEndian;

typedef struct _FcConfig    FcConfig;

typedef struct _FcGlobalCache	FcFileCache;

typedef struct _FcBlanks    FcBlanks;

typedef struct _FcStrList   FcStrList;

typedef struct _FcStrSet    FcStrSet;

typedef struct _FcCache	    FcCache;


/* fcblanks.c */
FcBlanks *
FcBlanksCreate (void);

void
FcBlanksDestroy (FcBlanks *b);

FcBool
FcBlanksAdd (FcBlanks *b, FcChar32 ucs4);

FcBool
FcBlanksIsMember (FcBlanks *b, FcChar32 ucs4);

/* fccache.c */

const FcChar8 *
FcCacheDir(const FcCache *c);

FcFontSet *
FcCacheCopySet(const FcCache *c);

const FcChar8 *
FcCacheSubdir (const FcCache *c, int i);

int
FcCacheNumSubdir (const FcCache *c);

int
FcCacheNumFont (const FcCache *c);

FcBool
FcDirCacheUnlink (const FcChar8 *dir, FcConfig *config);

FcBool
FcDirCacheValid (const FcChar8 *cache_file);

FcBool
FcDirCacheClean (const FcChar8 *cache_dir, FcBool verbose);

void
FcCacheCreateTagFile (const FcConfig *config);

/* fccfg.c */
FcChar8 *
FcConfigHome (void);

FcBool
FcConfigEnableHome (FcBool enable);

FcChar8 *
FcConfigFilename (const FcChar8 *url);
    
FcConfig *
FcConfigCreate (void);

FcConfig *
FcConfigReference (FcConfig *config);

void
FcConfigDestroy (FcConfig *config);

FcBool
FcConfigSetCurrent (FcConfig *config);

FcConfig *
FcConfigGetCurrent (void);

FcBool
FcConfigUptoDate (FcConfig *config);
    
FcBool
FcConfigBuildFonts (FcConfig *config);

FcStrList *
FcConfigGetFontDirs (FcConfig   *config);

FcStrList *
FcConfigGetConfigDirs (FcConfig   *config);

FcStrList *
FcConfigGetConfigFiles (FcConfig    *config);

FcChar8 *
FcConfigGetCache (FcConfig  *config);

FcBlanks *
FcConfigGetBlanks (FcConfig *config);

FcStrList *
FcConfigGetCacheDirs (const FcConfig	*config);

int
FcConfigGetRescanInterval (FcConfig *config);

FcBool
FcConfigSetRescanInterval (FcConfig *config, int rescanInterval);

FcFontSet *
FcConfigGetFonts (FcConfig	*config,
		  FcSetName	set);

FcBool
FcConfigAppFontAddFile (FcConfig    *config,
			const FcChar8  *file);

FcBool
FcConfigAppFontAddDir (FcConfig	    *config,
		       const FcChar8   *dir);

void
FcConfigAppFontClear (FcConfig	    *config);

FcBool
FcConfigSubstituteWithPat (FcConfig	*config,
			   FcPattern	*p,
			   FcPattern	*p_pat,
			   FcMatchKind	kind);

FcBool
FcConfigSubstitute (FcConfig	*config,
		    FcPattern	*p,
		    FcMatchKind	kind);

const FcChar8 *
FcConfigGetSysRoot (const FcConfig *config);

void
FcConfigSetSysRoot (FcConfig      *config,
		    const FcChar8 *sysroot);

/* fccharset.c */
FcCharSet*
FcCharSetCreate (void);

/* deprecated alias for FcCharSetCreate */
FcCharSet *
FcCharSetNew (void);

void
FcCharSetDestroy (FcCharSet *fcs);

FcBool
FcCharSetAddChar (FcCharSet *fcs, FcChar32 ucs4);

FcBool
FcCharSetDelChar (FcCharSet *fcs, FcChar32 ucs4);

FcCharSet*
FcCharSetCopy (FcCharSet *src);

FcBool
FcCharSetEqual (const FcCharSet *a, const FcCharSet *b);

FcCharSet*
FcCharSetIntersect (const FcCharSet *a, const FcCharSet *b);

FcCharSet*
FcCharSetUnion (const FcCharSet *a, const FcCharSet *b);

FcCharSet*
FcCharSetSubtract (const FcCharSet *a, const FcCharSet *b);

FcBool
FcCharSetMerge (FcCharSet *a, const FcCharSet *b, FcBool *changed);

FcBool
FcCharSetHasChar (const FcCharSet *fcs, FcChar32 ucs4);

FcChar32
FcCharSetCount (const FcCharSet *a);

FcChar32
FcCharSetIntersectCount (const FcCharSet *a, const FcCharSet *b);

FcChar32
FcCharSetSubtractCount (const FcCharSet *a, const FcCharSet *b);

FcBool
FcCharSetIsSubset (const FcCharSet *a, const FcCharSet *b);


/*
 * old coverage API, rather hard to use correctly
 */

FcChar32
FcCharSetCoverage (const FcCharSet *a, FcChar32 page, FcChar32 *result);

/* fcdbg.c */
void
FcValuePrint (const FcValue v);

void
FcPatternPrint (const FcPattern *p);

void
FcFontSetPrint (const FcFontSet *s);

/* fcdefault.c */
FcStrSet *
FcGetDefaultLangs (void);

void
FcDefaultSubstitute (FcPattern *pattern);

/* fcdir.c */
FcBool
FcFileIsDir (const FcChar8 *file);

FcBool
FcFileScan (FcFontSet	    *set,
	    FcStrSet	    *dirs,
	    FcFileCache	    *cache,
	    FcBlanks	    *blanks,
	    const FcChar8   *file,
	    FcBool	    force);

FcBool
FcDirScan (FcFontSet	    *set,
	   FcStrSet	    *dirs,
	   FcFileCache	    *cache,
	   FcBlanks	    *blanks,
	   const FcChar8    *dir,
	   FcBool	    force);

FcBool
FcDirSave (FcFontSet *set, FcStrSet *dirs, const FcChar8 *dir);

FcCache *
FcDirCacheLoad (const FcChar8 *dir, FcConfig *config, FcChar8 **cache_file);

FcCache *
FcDirCacheRescan (const FcChar8 *dir, FcConfig *config);
    
FcCache *
FcDirCacheRead (const FcChar8 *dir, FcBool force, FcConfig *config);

FcCache *
FcDirCacheLoadFile (const FcChar8 *cache_file, struct stat *file_stat);

void
FcDirCacheUnload (FcCache *cache);

/* fcfreetype.c */
FcPattern *
FcFreeTypeQuery (const FcChar8 *file, int id, FcBlanks *blanks, int *count);

/* fcfs.c */

FcFontSet *
FcFontSetCreate (void);

void
FcFontSetDestroy (FcFontSet *s);

FcBool
FcFontSetAdd (FcFontSet *s, FcPattern *font);

/* fcinit.c */
FcConfig *
FcInitLoadConfig (void);

FcConfig *
FcInitLoadConfigAndFonts (void);

FcBool
FcInit (void);

void
FcFini (void);

int
FcGetVersion (void);

FcBool
FcInitReinitialize (void);

FcBool
FcInitBringUptoDate (void);

/* fclang.c */
FcStrSet *
FcGetLangs (void);

FcChar8 *
FcLangNormalize (const FcChar8 *lang);

const FcCharSet *
FcLangGetCharSet (const FcChar8 *lang);

FcLangSet*
FcLangSetCreate (void);

void
FcLangSetDestroy (FcLangSet *ls);

FcLangSet*
FcLangSetCopy (const FcLangSet *ls);

FcBool
FcLangSetAdd (FcLangSet *ls, const FcChar8 *lang);

FcBool
FcLangSetDel (FcLangSet *ls, const FcChar8 *lang);

FcLangResult
FcLangSetHasLang (const FcLangSet *ls, const FcChar8 *lang);

FcLangResult
FcLangSetCompare (const FcLangSet *lsa, const FcLangSet *lsb);

FcBool
FcLangSetContains (const FcLangSet *lsa, const FcLangSet *lsb);

FcBool
FcLangSetEqual (const FcLangSet *lsa, const FcLangSet *lsb);

FcChar32
FcLangSetHash (const FcLangSet *ls);

FcStrSet *
FcLangSetGetLangs (const FcLangSet *ls);

FcLangSet *
FcLangSetUnion (const FcLangSet *a, const FcLangSet *b);

FcLangSet *
FcLangSetSubtract (const FcLangSet *a, const FcLangSet *b);

/* fclist.c */
FcObjectSet *
FcObjectSetCreate (void);

FcBool
FcObjectSetAdd (FcObjectSet *os, const char *object);

void
FcObjectSetDestroy (FcObjectSet *os);

FcObjectSet *
FcObjectSetVaBuild (const char *first, va_list va);

FcObjectSet *
FcObjectSetBuild (const char *first, ...);

FcFontSet *
FcFontSetList (FcConfig	    *config,
	       FcFontSet    **sets,
	       int	    nsets,
	       FcPattern    *p,
	       FcObjectSet  *os);

FcFontSet *
FcFontList (FcConfig	*config,
	    FcPattern	*p,
	    FcObjectSet *os);

/* fcatomic.c */

FcAtomic *
FcAtomicCreate (const FcChar8   *file);

FcBool
FcAtomicLock (FcAtomic *atomic);

FcChar8 *
FcAtomicNewFile (FcAtomic *atomic);

FcChar8 *
FcAtomicOrigFile (FcAtomic *atomic);

FcBool
FcAtomicReplaceOrig (FcAtomic *atomic);

void
FcAtomicDeleteNew (FcAtomic *atomic);

void
FcAtomicUnlock (FcAtomic *atomic);

void
FcAtomicDestroy (FcAtomic *atomic);

/* fcmatch.c */
FcPattern *
FcFontSetMatch (FcConfig    *config,
		FcFontSet   **sets,
		int	    nsets,
		FcPattern   *p,
		FcResult    *result);

FcPattern *
FcFontMatch (FcConfig	*config,
	     FcPattern	*p, 
	     FcResult	*result);

FcPattern *
FcFontRenderPrepare (FcConfig	    *config,
		     FcPattern	    *pat,
		     FcPattern	    *font);

FcFontSet *
FcFontSetSort (FcConfig	    *config,
	       FcFontSet    **sets,
	       int	    nsets,
	       FcPattern    *p,
	       FcBool	    trim,
	       FcCharSet    **csp,
	       FcResult	    *result);

FcFontSet *
FcFontSort (FcConfig	 *config,
	    FcPattern    *p,
	    FcBool	 trim,
	    FcCharSet    **csp,
	    FcResult	 *result);

void
FcFontSetSortDestroy (FcFontSet *fs);

/* fcmatrix.c */
FcMatrix *
FcMatrixCopy (const FcMatrix *mat);

FcBool
FcMatrixEqual (const FcMatrix *mat1, const FcMatrix *mat2);

void
FcMatrixMultiply (FcMatrix *result, const FcMatrix *a, const FcMatrix *b);

void
FcMatrixRotate (FcMatrix *m, double c, double s);

void
FcMatrixScale (FcMatrix *m, double sx, double sy);

void
FcMatrixShear (FcMatrix *m, double sh, double sv);

/* fcname.c */

const FcConstant *
FcNameGetConstant (const FcChar8 *string);

FcBool
FcNameConstant (const FcChar8 *string, int *result);

FcPattern *
FcNameParse (const FcChar8 *name);

FcChar8 *
FcNameUnparse (FcPattern *pat);

/* fcpat.c */
FcPattern *
FcPatternCreate (void);

FcPattern *
FcPatternDuplicate (const FcPattern *p);

void
FcPatternReference (FcPattern *p);

FcPattern *
FcPatternFilter (FcPattern *p, const FcObjectSet *os);

void
FcValueDestroy (FcValue v);

FcBool
FcValueEqual (FcValue va, FcValue vb);

FcValue
FcValueSave (FcValue v);

void
FcPatternDestroy (FcPattern *p);

FcBool
FcPatternEqual (const FcPattern *pa, const FcPattern *pb);

FcBool
FcPatternEqualSubset (const FcPattern *pa, const FcPattern *pb, const FcObjectSet *os);

FcChar32
FcPatternHash (const FcPattern *p);

FcBool
FcPatternAdd (FcPattern *p, const char *object, FcValue value, FcBool append);
    
FcBool
FcPatternAddWeak (FcPattern *p, const char *object, FcValue value, FcBool append);
    
FcResult
FcPatternGet (const FcPattern *p, const char *object, int id, FcValue *v);
    
FcBool
FcPatternDel (FcPattern *p, const char *object);

FcBool
FcPatternRemove (FcPattern *p, const char *object, int id);

FcBool
FcPatternAddInteger (FcPattern *p, const char *object, int i);

FcBool
FcPatternAddDouble (FcPattern *p, const char *object, double d);

FcBool
FcPatternAddString (FcPattern *p, const char *object, const FcChar8 *s);

FcBool
FcPatternAddMatrix (FcPattern *p, const char *object, const FcMatrix *s);

FcBool
FcPatternAddCharSet (FcPattern *p, const char *object, const FcCharSet *c);

FcBool
FcPatternAddBool (FcPattern *p, const char *object, FcBool b);

FcBool
FcPatternAddLangSet (FcPattern *p, const char *object, const FcLangSet *ls);

FcBool
FcPatternAddRange (FcPattern *p, const char *object, const FcRange *r);

FcResult
FcPatternGetInteger (const FcPattern *p, const char *object, int n, int *i);

FcResult
FcPatternGetDouble (const FcPattern *p, const char *object, int n, double *d);

FcResult
FcPatternGetString (const FcPattern *p, const char *object, int n, FcChar8 ** s);

FcResult
FcPatternGetMatrix (const FcPattern *p, const char *object, int n, FcMatrix **s);

FcResult
FcPatternGetCharSet (const FcPattern *p, const char *object, int n, FcCharSet **c);

FcResult
FcPatternGetBool (const FcPattern *p, const char *object, int n, FcBool *b);

FcResult
FcPatternGetLangSet (const FcPattern *p, const char *object, int n, FcLangSet **ls);

FcResult
FcPatternGetRange (const FcPattern *p, const char *object, int id, FcRange **r);

FcPattern *
FcPatternVaBuild (FcPattern *p, va_list va);
    
FcPattern *
FcPatternBuild (FcPattern *p, ...);

FcChar8 *
FcPatternFormat (FcPattern *pat, const FcChar8 *format);

/* fcrange.c */
FcRange *
FcRangeCreateDouble (double begin, double end);

FcRange *
FcRangeCreateInteger (FcChar32 begin, FcChar32 end);

void
FcRangeDestroy (FcRange *range);

FcRange *
FcRangeCopy (const FcRange *r);

FcBool
FcRangeGetDouble(const FcRange *range, double *begin, double *end);

/* fcweight.c */

int
FcWeightFromOpenType (int ot_weight);

int
FcWeightToOpenType (int fc_weight);


FcChar8 *
FcStrCopy (const FcChar8 *s);

FcChar8 *
FcStrCopyFilename (const FcChar8 *s);
    
FcChar8 *
FcStrPlus (const FcChar8 *s1, const FcChar8 *s2);
    
void
FcStrFree (FcChar8 *s);

FcChar8 *
FcStrDowncase (const FcChar8 *s);

int
FcStrCmpIgnoreCase (const FcChar8 *s1, const FcChar8 *s2);

int
FcStrCmp (const FcChar8 *s1, const FcChar8 *s2);

const FcChar8 *
FcStrStrIgnoreCase (const FcChar8 *s1, const FcChar8 *s2);

const FcChar8 *
FcStrStr (const FcChar8 *s1, const FcChar8 *s2);

int
FcUtf8ToUcs4 (const FcChar8 *src_orig,
	      FcChar32	    *dst,
	      int	    len);

FcBool
FcUtf8Len (const FcChar8    *string,
	   int		    len,
	   int		    *nchar,
	   int		    *wchar);


int
FcUtf16ToUcs4 (const FcChar8	*src_orig,
	       FcEndian		endian,
	       FcChar32		*dst,
	       int		len);	    /* in bytes */

FcBool
FcUtf16Len (const FcChar8   *string,
	    FcEndian	    endian,
	    int		    len,	    /* in bytes */
	    int		    *nchar,
	    int		    *wchar);

FcChar8 *
FcStrDirname (const FcChar8 *file);

FcChar8 *
FcStrBasename (const FcChar8 *file);

FcStrSet *
FcStrSetCreate (void);

FcBool
FcStrSetMember (FcStrSet *set, const FcChar8 *s);

FcBool
FcStrSetEqual (FcStrSet *sa, FcStrSet *sb);

FcBool
FcStrSetAdd (FcStrSet *set, const FcChar8 *s);

FcBool
FcStrSetAddFilename (FcStrSet *set, const FcChar8 *s);

FcBool
FcStrSetDel (FcStrSet *set, const FcChar8 *s);

void
FcStrSetDestroy (FcStrSet *set);

FcStrList *
FcStrListCreate (FcStrSet *set);

void
FcStrListFirst (FcStrList *list);

FcChar8 *
FcStrListNext (FcStrList *list);

void
FcStrListDone (FcStrList *list);

/* fcxml.c */
FcBool
FcConfigParseAndLoad (FcConfig *config, const FcChar8 *file, FcBool complain);
]]
return {}
