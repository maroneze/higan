include nall/Makefile

fc  := fc
sfc := sfc
gb  := gb
gba := gba

profile := balanced
target  := ethos

# sharedpath defines where some common files will be installed.
# Is it ignored when compiling to Windows.
# Leave it empty to use higan's default values.
sharedpath := _build/usr/share

# DESTDIR is only used by the 'install' target.
# Leave it empty to use Makefile's default values.
DESTDIR := _build

# Uncomment when debugging (enables compilation with debugging symbols,
# prevents stripping of the final binary, and passes -DDEBUG to g++).
#DEBUG := 1

ifeq ($(platform),windows)
  sharedpath := 
else 
  ifeq ($(platform),macosx)
    ifeq ($(sharedpath),)
      sharedpath := "/Library/Application\ Support"
    endif
  else
    ifeq ($(sharedpath),)
      sharedpath := /usr/share
    endif
  endif
endif

# options += debugger
# arch := x86
# console := true

# when debugging, add debugging flags, do not strip executable (-s),
# and avoid optimizations (for faster compilation)
ifneq ($(DEBUG),)
  flags   += -g -ggdb -DDEBUG
  link    += -g -ggdb
else
  flags   += -O3 -fomit-frame-pointer
  link    += -s
endif

# compiler
flags   += -I.
link    += 
objects := libco

# profile-guided optimization mode
# pgo := instrument
# pgo := optimize

ifeq ($(pgo),instrument)
  flags += -fprofile-generate
  link += -lgcov
else ifeq ($(pgo),optimize)
  flags += -fprofile-use
endif

# platform
ifeq ($(platform),windows)
  ifeq ($(arch),x86)
    flags += -m32
    link += -m32
  endif
  ifeq ($(console),true)
    link += -mconsole
  else
    link += -mwindows
  endif
  link += -mthreads -luuid -lkernel32 -luser32 -lgdi32 -lcomctl32 -lcomdlg32 -lshell32 -lole32 -lws2_32
  link += -Wl,-enable-auto-import -Wl,-enable-runtime-pseudo-reloc
else ifeq ($(platform),macosx)
  flags += -march=native
else ifeq ($(platform),linux)
  flags += -march=native -DSHAREDPATH="$(sharedpath)"
  link += -Wl,-export-dynamic -lX11 -lXext -ldl
else ifeq ($(platform),bsd)
  flags += -march=native
  link += -Wl,-export-dynamic -lX11 -lXext
else
  $(error unsupported platform.)
endif

ui := target-$(target)

# implicit rules
compile = \
  $(strip \
    $(if $(filter %.c,$<), \
      $(compiler) $(cflags) $(flags) $1 -c $< -o $@, \
      $(if $(filter %.cpp,$<), \
        $(compiler) $(cppflags) $(flags) $1 -c $< -o $@ \
      ) \
    ) \
  )

%.o: $<; $(call compile)

all: build;

obj/libco.o: libco/libco.c libco/*

include $(ui)/Makefile
flags := $(flags) $(foreach o,$(call strupper,$(options)),-D$o)

# targets
clean:
	-@$(call delete,obj/*.o)
	-@$(call delete,obj/*.a)
	-@$(call delete,obj/*.so)
	-@$(call delete,obj/*.dylib)
	-@$(call delete,obj/*.dll)
	-@$(call delete,*.res)
	-@$(call delete,*.manifest)

archive:
	if [ -f higan.tar.xz ]; then rm higan.tar.xz; fi
	tar -cJf higan.tar.xz `ls`

sync:
ifeq ($(shell id -un),byuu)
	if [ -d ./libco ]; then rm -r ./libco; fi
	if [ -d ./nall ]; then rm -r ./nall; fi
	if [ -d ./ruby ]; then rm -r ./ruby; fi
	if [ -d ./phoenix ]; then rm -r ./phoenix; fi
	cp -r ../libco ./libco
	cp -r ../nall ./nall
	cp -r ../ruby ./ruby
	cp -r ../phoenix ./phoenix
	rm -r libco/doc
	rm -r libco/.test
	rm -r nall/.test
	rm -r ruby/.test
	rm -r phoenix/.test
endif

help:;
