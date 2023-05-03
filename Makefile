-include config.mk

CFLAGS += -Wall

all: config.mk outdir libpam.so

config.mk:
	@if ! test -e config.mk; then printf "\033[31;1mERROR:\033[0m you have to run ./configure\n"; exit 1; fi

OBJ = out/main.o

$(OBJ):
	$(QUIET_CC)$(CC) $(CFLAGS) -c src/$(@F:.o=.c) -o $@

libpam.so: $(OBJ)
	$(QUIET_LINK)$(CC) -shared -Wl,-soname,$@ $^ $(LIBS) -lpam -o out/$@

outdir:
	@mkdir -p out

install:
	@echo installing libpam
	@mkdir -p $(DESTDIR)$(LIB_DIR)
	@cp -f out/libpam.so $(DESTDIR)$(LIB_DIR)/lua/5.3/libpam.so
	@strip -s $(DESTDIR)$(LIB_DIR)/lua/5.3/libpam.so
	@chmod 755 $(DESTDIR)$(LIB_DIR)/lua/5.3/libpam.so

uninstall:
	@echo uninstalling libpam
	@rm -f $(DESTDIR)$(LIB_DIR)/lua/5.3/libpam.so

clean:
	@echo removing xprop output files..
	@rm -f out/*.o
	@rm -f out/libpam.so

distclean: clean
	@echo removing config.mk include file
	@rm -f config.mk

.PHONY: all clean distclean install uninstall
