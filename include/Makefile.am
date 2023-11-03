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
	-rm -f libbde.h
	-rm -f libbde/definitions.h
	-rm -f libbde/features.h
	-rm -f libbde/types.h
	-rm -f Makefile

