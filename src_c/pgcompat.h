/* Python 2.x/3.x compatibility tools (internal)
 */
#ifndef PGCOMPAT_INTERNAL_H
#define PGCOMPAT_INTERNAL_H

#include "include/pgcompat.h"

/* Weakrefs flags changed in 3.x */
#define Py_TPFLAGS_HAVE_WEAKREFS 0

/* Module init function returns new module instance. */
#define MODINIT_DEFINE(mod_name) PyMODINIT_FUNC PyInit_##mod_name (void)

#ifndef PYPY_VERSION
#define Text_FromLocale(s) PyUnicode_DecodeLocale((s), "strict")
#else /* PYPY_VERSION */
/* workaround: missing function for pypy */
#define Text_FromLocale PyUnicode_FromString
#endif /* PYPY_VERSION */

/* Defaults for unicode file path encoding */
#if defined(MS_WIN32)
#define UNICODE_DEF_FS_ERROR "replace"
#else
#define UNICODE_DEF_FS_ERROR "surrogateescape"
#endif

/* Pep 3123: Making PyObject_HEAD conform to standard C */
#if !defined(Py_TYPE)
#define Py_TYPE(o)    (((PyObject *)(o))->ob_type)
#define Py_REFCNT(o)  (((PyObject *)(o))->ob_refcnt)
#define Py_SIZE(o)    (((PyVarObject *)(o))->ob_size)
#endif

/* Encode a unicode file path */
#define Unicode_AsEncodedPath(u) \
    PyUnicode_AsEncodedString ((u), Py_FileSystemDefaultEncoding, UNICODE_DEF_FS_ERROR)

#define RELATIVE_MODULE(m) ("." m)

#define HAVE_OLD_BUFPROTO 0

#if !defined(PG_ENABLE_OLDBUF)  /* allow for command line override */
#if HAVE_OLD_BUFPROTO
#define PG_ENABLE_OLDBUF 1
#else
#define PG_ENABLE_OLDBUF 0
#endif
#endif

#ifndef Py_TPFLAGS_HAVE_NEWBUFFER
#define Py_TPFLAGS_HAVE_NEWBUFFER 0
#endif

#ifndef Py_TPFLAGS_HAVE_CLASS
#define Py_TPFLAGS_HAVE_CLASS 0
#endif

#ifndef Py_TPFLAGS_CHECKTYPES
#define Py_TPFLAGS_CHECKTYPES 0
#endif

#define Slice_GET_INDICES_EX(slice, length, start, stop, step, slicelength) \
    PySlice_GetIndicesEx(slice, length, start, stop, step, slicelength)

#if defined(SDL_VERSION_ATLEAST)
#if !(SDL_VERSION_ATLEAST(2, 0, 5))
/* These functions require SDL 2.0.5 or greater.

  https://wiki.libsdl.org/SDL_SetWindowResizable
*/
void SDL_SetWindowResizable(SDL_Window *window, SDL_bool resizable);
int SDL_GetWindowOpacity(SDL_Window *window, float *opacity);
int SDL_SetWindowOpacity(SDL_Window *window, float opacity);
int SDL_SetWindowModalFor(SDL_Window *modal_window, SDL_Window *parent_window);
int SDL_SetWindowInputFocus(SDL_Window *window);
SDL_Surface * SDL_CreateRGBSurfaceWithFormat(Uint32 flags, int width, int height, int depth,
                               Uint32 format);
#endif /* !(SDL_VERSION_ATLEAST(2, 0, 5)) */
#endif /* defined(SDL_VERSION_ATLEAST) */

// Currently needed to build scrap.c, event.c, display.c
// with Windows SDK 10.0.18362.0 and SDL1 build
#ifdef _MSC_VER
    #ifndef WINDOWS_IGNORE_PACKING_MISMATCH
        #define WINDOWS_IGNORE_PACKING_MISMATCH
    #endif
#endif

#endif /* ~PGCOMPAT_INTERNAL_H */
