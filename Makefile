# project name (generate executable with this name)
TARGET   = testes.out

CC       = gcc
# compiling flags here
CFLAGS   = -lm -I.

LINKER   = gcc
# linking flags here
LFLAGS   = -I. -lm

SOURCES  := $(shell find . -type f -name "*.c")
INCLUDES := $(shell find . -type f -name "*.h")
OBJECTS  := $(SOURCES:.c=.o)
rm       = rm -rf

$(TARGET): $(OBJECTS) $(INCLUDES)
	@mkdir -p $(dir $@)
	@$(LINKER) $(OBJECTS) $(LFLAGS) -o $@
	@echo "Linking complete!"

%.o: %.c
	@mkdir -p $(dir $@)
	@$(CC) $(CFLAGS) -c $< -o $@
	@echo "Compiled \"$<\" successfully!"

.PHONY: clean
clean:
	@$(rm) $(OBJECTS)
	@echo "Cleanup complete!"

.PHONY: remove
remove: clean
	@$(rm) $(BINDIR)/$(TARGET)
	@echo "Executable removed!"

remake: remove $(BINDIR)/$(TARGET)
	@echo "Remaked!"