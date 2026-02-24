# TODO: will automate the compiler at some point too
CC = gcc

# needed compiler flags. will change this so that it works with clang too.
CFLAGS = -MMD -Wall -Wextra -I./include/ -I./src/

# automatation of source files
SRCS_DIR = src tests
SRCS = $(wildcard $(addsuffix /*.c, $(SRCS_DIR)))

# automation of object files
OBJDIR = obj
OBJS = $(addprefix $(OBJDIR)/, $(notdir $(SRCS:.c=.o)))

# automation of dependencies
DEPS = $(OBJS:.o=.d)
vpath %.c src

test: CFLAGS += -g
test: _tests

MACROS = --define-macro DEBUG_JMALLOC \
	 --define-macro DEBUG_JFREE \
	 --define-macro DEBUG_JREALLOC

debug: CFLAGS += $(MACROS)
debug: test

reg: test

$(OBJDIR):
	mkdir -p $(OBJDIR)

clean:
	rm -f test
	rm -rf $(OBJDIR)

GREEN  := $(shell tput -Txterm setaf 2)
RESET  := $(shell tput -Txterm sgr0)

help:
	@echo ''
	@echo 'Usage:'
	@echo '  ${GREEN}make regular${RESET}    Compile test.c without debugging flags'
	@echo '  ${GREEN}make debug${RESET}      Compile test.c with the correct includes and flags'
	@echo '  ${GREEN}make help${RESET}       Print this message'
	@echo ''

_tests : $(OBJS)
	$(CC) $(CFLAGS) -o test $(OBJS) $(LIBS)

# compile every source file with its dependencies
obj/%.o : %.c | $(OBJDIR)
	$(CC) $(CFLAGS) -c $< -o $@

# cli options
.PHONY: clean help test

-include $(DEPS)
