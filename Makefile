CC = gcc
CFLAGS = -Wall `pkg-config --cflags gtk+-3.0 libayatana-appindicator3-0.1`
LIBS = `pkg-config --libs gtk+-3.0 libayatana-appindicator3-0.1`

SRC = pm13tray.c
TARGET = pm13tray

# Default installation prefix (can be overridden)
PREFIX ?= /usr/local
BINDIR = $(PREFIX)/bin

all: $(TARGET)

$(TARGET): $(SRC)
	$(CC) $(CFLAGS) -o $@ $^ $(LIBS)

install: $(TARGET)
	install -d $(DESTDIR)$(BINDIR)
	install -m 755 $(TARGET) $(DESTDIR)$(BINDIR)/

uninstall:
	rm -f $(DESTDIR)$(BINDIR)/$(TARGET)

clean:
	rm -f $(TARGET)
