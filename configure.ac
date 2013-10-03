#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ(2.59)
AC_INIT([GLoid2], [1.1], [kalamara@users.sourceforge.net])
AC_CONFIG_SRCDIR([src/gloid.h])
AC_CONFIG_HEADER([config.h])

# Checks for programs.
AC_PROG_CXX
AC_PROG_CC

# Checks for libraries.
# FIXME: Replace `main' with a function in `-lGL':
AC_CHECK_LIB([GL], [glEnable])
# FIXME: Replace `main' with a function in `-lGLU':
AC_CHECK_LIB([GLU], [gluLookAt])
# FIXME: Replace `main' with a function in `-lSDL':
AC_CHECK_LIB([SDL], [SDL_Init])
# FIXME: Replace `main' with a function in `-lSDL_ttf':
AC_CHECK_LIB([SDL_ttf], [TTF_Init])

# Checks for header files.
AC_HEADER_STDC
AC_CHECK_HEADERS([stdio.h stdlib.h string.h stdarg.h ctype.h math.h]) #? GL/gl.h GL/glu.h SDL.h SDL_ttf.h SDL_audio.h])

# Checks for typedefs, structures, and compiler characteristics.
AC_C_CONST
AC_HEADER_STDBOOL

# Checks for library functions.
AC_FUNC_MALLOC
AC_FUNC_VPRINTF
AC_CHECK_FUNCS([atexit memset sqrt strstr])

AC_CONFIG_FILES([Makefile])
AC_OUTPUT
