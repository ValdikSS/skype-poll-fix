OS := $(shell uname -s)

all: skype-poll-fix.c
ifeq ($(OS),Linux)
	@echo "Building 32-bit linux shared-library skype-poll-fix.so..."
	$(CC) -shared -m32 -O2 -ldl -o skype-poll-fix.so skype-poll-fix.c
endif
ifeq ($(OS),Darwin)
	@echo "Building Darwing shared-library skype-poll-fix.dylib..."
	$(CC) -arch i386 -arch x86_64 -shared -O2 -ldl -o skype-poll-fix.dylib skype-poll-fix.c
endif

.PHONY: clean

clean:
	rm -f *.o *~
