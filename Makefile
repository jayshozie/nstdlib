# nstdlib Makefile
# Copyright (C) 2026 Emir Baha Yıldırım

CC := gcc
CFLAGS := -std=gnu23 -MMD -Wall -Wextra -I./include/ -I./src/ -g
OBJDIR := obj

# Expose these to shell autocomplete
JMM_SUB  := jmalloc jfree jrealloc
JSTR_SUB := jmemcpy jmemmove jmemset jmemcmp jstrlen jstpcpy jstrcpy jstrcat \
            jstrncpy jstpncpy jstrcmp jstrncmp jstrchr jstrrchr jstrchrnul \
            jstrsep jstrdup jstrndup
MODULES  := jmm jstring all

.PHONY: lib tests clean help FORCE $(MODULES) $(JMM_SUB) $(JSTR_SUB)

# Silently consume module and function names so Make doesn't error out
ifeq (tests,$(firstword $(MAKECMDGOALS)))
  $(MODULES) $(JMM_SUB) $(JSTR_SUB): ; @:
endif

# Always compile the entire library to prevent undefined linker references
LIB_SRCS := $(shell find src -name '*.c')
TEST_SRCS := 
DEBUG_FLAGS := 

ifneq (,$(filter tests,$(MAKECMDGOALS)))
    TEST_SRCS += tests/test.c
    
    SPEC_JMM := $(filter $(JMM_SUB),$(MAKECMDGOALS))
    SPEC_JSTR := $(filter $(JSTR_SUB),$(MAKECMDGOALS))
    HAS_JMM := $(filter jmm,$(MAKECMDGOALS))
    HAS_JSTR := $(filter jstring,$(MAKECMDGOALS))
    HAS_ALL := $(filter all,$(MAKECMDGOALS))

    # JMM Module
    ifneq (,$(HAS_JMM)$(SPEC_JMM))
        DEBUG_FLAGS += -D__JMM_DEBUG
        TEST_SRCS += tests/jmm_tests.c
        ifneq (,$(SPEC_JMM))
            DEBUG_FLAGS += $(foreach t,$(SPEC_JMM),-D__TEST_$(shell echo $(t) | tr 'a-z' 'A-Z'))
        else
            DEBUG_FLAGS += -D__TEST_JMALLOC -D__TEST_JFREE -D__TEST_JREALLOC
        endif
    endif

    # JString Module
    ifneq (,$(HAS_JSTR)$(SPEC_JSTR))
        DEBUG_FLAGS += -D__JSTR_DEBUG
        TEST_SRCS += tests/jstring_tests.c
        ifneq (,$(SPEC_JSTR))
            # Strips the 'j' and uppercases (e.g. jmemcpy -> __TEST_MEMCPY)
            DEBUG_FLAGS += $(foreach t,$(SPEC_JSTR),-D__TEST_$(shell echo $(t) | sed 's/^j//' | tr 'a-z' 'A-Z'))
        else
            DEBUG_FLAGS += -D__TEST_MEMCPY -D__TEST_MEMMOVE -D__TEST_MEMSET -D__TEST_MEMCMP \
                           -D__TEST_STRLEN -D__TEST_STRCPY -D__TEST_STPCPY -D__TEST_STRCAT -D__TEST_STRNCPY \
                           -D__TEST_STPNCPY -D__TEST_STRCMP -D__TEST_STRNCMP -D__TEST_STRCHR \
                           -D__TEST_STRRCHR -D__TEST_STRCHRNUL -D__TEST_STRSEP -D__TEST_STRDUP \
                           -D__TEST_STRNDUP
        endif
    endif

    # Default to ALL if no specific modules/functions were passed
    ifeq (,$(HAS_JMM)$(SPEC_JMM)$(HAS_JSTR)$(SPEC_JSTR))
        HAS_ALL := all
    endif

    ifneq (,$(HAS_ALL))
        DEBUG_FLAGS += -D__JMM_DEBUG -D__JSTR_DEBUG
        DEBUG_FLAGS += -D__TEST_JMALLOC -D__TEST_JFREE -D__TEST_JREALLOC
        DEBUG_FLAGS += -D__TEST_MEMCPY -D__TEST_MEMMOVE -D__TEST_MEMSET -D__TEST_MEMCMP \
                       -D__TEST_STRLEN -D__TEST_STRCPY -D__TEST_STPCPY -D__TEST_STRCAT -D__TEST_STRNCPY \
                       -D__TEST_STPNCPY -D__TEST_STRCMP -D__TEST_STRNCMP -D__TEST_STRCHR \
                       -D__TEST_STRRCHR -D__TEST_STRCHRNUL -D__TEST_STRSEP -D__TEST_STRDUP \
                       -D__TEST_STRNDUP
        TEST_SRCS := $(shell find tests -name '*.c')
    endif

    CFLAGS += $(DEBUG_FLAGS)
endif

LIB_SRCS := $(sort $(LIB_SRCS))
TEST_SRCS := $(sort $(TEST_SRCS))

LIB_OBJS := $(addprefix $(OBJDIR)/, $(LIB_SRCS:.c=.o))
TEST_OBJS := $(addprefix $(OBJDIR)/, $(TEST_SRCS:.c=.o))

.DEFAULT_GOAL := lib

lib: $(LIB_OBJS)
	@echo "Linking library libnstdlib.a..."
	ar rcs libnstdlib.a $(LIB_OBJS)

tests: $(LIB_OBJS) $(TEST_OBJS)
	@echo "Linking test binary with flags: $(DEBUG_FLAGS)"
	$(CC) $(CFLAGS) -o test $(LIB_OBJS) $(TEST_OBJS)

$(OBJDIR)/%.o: %.c $(OBJDIR)/.flags
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -rf $(OBJDIR) test libnstdlib.a

help:
	@echo "Usage:"
	@echo "  make lib                  Build libnstdlib.a"
	@echo "  make tests [module/func]  Build test suite (e.g., make tests jstring jmemcpy)"
	@echo "  make clean                Cleanup"

$(OBJDIR)/.flags: FORCE
	@mkdir -p $(OBJDIR)
	@echo "$(CFLAGS) $(LIB_SRCS)" | cmp -s - $@ || echo "$(CFLAGS) $(LIB_SRCS)" > $@

FORCE:

-include $(LIB_OBJS:.o=.d) $(TEST_OBJS:.o=.d)
