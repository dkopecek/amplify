ACLOCAL_AMFLAGS= -I m4

AM_CPPFLAGS=\
	-I$(top_srcdir)/src \
	-I$(top_srcdir)/third-party \
	@pthread_CFLAGS@

bin_PROGRAMS=\
	ampush

ampush_SOURCES=\
	src/Logging.hpp \
	src/CodeType.cpp \
	src/CodeType.hpp \
	src/CodeTypeImpl.hpp \
	src/ListType.hpp \
	src/ListType.cpp \
	src/ListTypeImpl.hpp \
	src/IntegerType.hpp \
	src/IntegerType.cpp \
	src/IntegerTypeImpl.hpp \
	src/NameType.hpp \
	src/NameType.cpp \
	src/NameTypeImpl.hpp \
	src/BooleanType.hpp \
	src/BooleanType.cpp \
	src/BooleanTypeImpl.hpp \
	src/FloatType.hpp \
	src/FloatType.cpp \
	src/FloatTypeImpl.hpp \
	src/StringType.hpp \
	src/StringType.cpp \
	src/StringTypeImpl.hpp \
	src/Interpreter.cpp \
	src/Interpreter.hpp \
	src/main.cpp \
	src/Parser.h \
	src/Lexer-configuration.hpp \
	src/Lexer.cpp \
	src/Lexer.hpp \
	src/Lexer-token.hpp \
	src/Lexer-token_ids.hpp \
	src/Program.hpp \
	src/Program.cpp \
	src/StackBase.hpp \
	src/ValueStorage.hpp \
	src/Instruction.hpp \
	src/Utility.hpp \
	src/Type.hpp \
	src/CustomTypes.hpp \
	src/Runtime.cpp \
	src/Runtime.hpp \
	src/Random.hpp \
	src/ProgramSelector.hpp \
	src/ProgramTransform.hpp \
	src/FitnessCaseEvaluator.hpp \
	src/FitnessStopEvaluator.hpp \
	src/Typedefs.hpp \
	src/InputGenerator.hpp \
	src/CustomTypes.hpp

#src/GPSystem.cpp
#src/GPSystem.hpp

ampush_LDADD=\
	@pthread_LIBS@ \
	-lm

EXTRA_DIST=\
	src/Lexer.qx \
	src/Parser.y \
	src/Parser.c \
	src/quex \
	third-party/easylogging++.h \
	third-party/quex-0.65.2 \
	third-party/lemon

$(top_builddir)/lemon: $(top_srcdir)/third-party/lemon/lemon.c $(top_srcdir)/third-party/lemon/lempar.c
	$(CC) $(CPPFLAGS) $(CFLAGS) -o $(top_builddir)/lemon $(top_srcdir)/third-party/lemon/lemon.c

$(top_builddir)/quex: $(top_srcdir)/third-party/quex-0.65.2/quex-exe.py
	echo -e "#!/bin/sh\nexport QUEX_PATH=\"$(abs_top_srcdir)/third-party/quex-0.65.2/\"\nexec \"$(abs_top_srcdir)/third-party/quex-0.65.2/quex-exe.py\" \$$@" > "$(top_builddir)/quex"
	chmod a+x "$(top_builddir)/quex"

gensrc: lexers parsers

parsers: $(top_builddir)/lemon $(top_srcdir)/src/Parser.y
	$(abs_top_builddir)/lemon T=$(abs_top_srcdir)/third-party/lemon/lempar.c $(top_srcdir)/src/Parser.y
	touch $(abs_top_srcdir)/src/Parser.h

lexers: parsers $(top_builddir)/quex $(top_srcdir)/src/Parser.h $(top_srcdir)/src/Lexer.qx
	$(abs_top_builddir)/quex --source-package $(abs_top_srcdir)/src/ --fes pp \
		--foreign-token-id-file $(abs_top_srcdir)/src/Parser.h \
		--token-id-prefix AMPUSH_TOKEN_ -i $(abs_top_srcdir)/src/Lexer.qx \
		-o Lexer
	touch $(abs_top_srcdir)/src/Lexer.hpp

gensrc-clean:
	rm -f \
	$(top_builddir)/lemon \
	$(top_builddir)/quex \
	$(top_srcdir)/src/Parser.h \
	$(top_srcdir)/src/Parser.c \
	$(top_srcdir)/src/Lexer.hpp \
	$(top_srcdir)/src/Lexer.cpp \
	$(top_srcdir)/src/Lexer-configuration.hpp \
	$(top_srcdir)/src/Lexer-token.hpp \
	$(top_srcdir)/src/Lexer-token_ids.hpp
