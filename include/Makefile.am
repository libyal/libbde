include_HEADERS = \
	libbde.h

pkginclude_HEADERS = \
	libbde/codepage.h \
	libbde/definitions.h \
	libbde/error.h \
	libbde/extern.h \
	libbde/features.h \
	libbde/types.h

EXTRA_DIST = \
	libbde.h.in \
	libbde/definitions.h.in \
	libbde/features.h.in \
	libbde/types.h.in

MAINTAINERCLEANFILES = \
	Makefile.in

distclean: clean
	/bin/rm -f libbde.h
	/bin/rm -f libbde/definitions.h
	/bin/rm -f libbde/features.h
	/bin/rm -f libbde/types.h
	/bin/rm -f Makefile

