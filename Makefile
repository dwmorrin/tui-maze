exe = maze

srcDir = src
objDir = obj

src = $(wildcard $(srcDir)/*.c)
objs = $(src:$(srcDir)/%.c=$(objDir)/%.o)

CPPFLAGS += -Iinclude
CFLAGS += -Wall
LDLIBS += -lncurses

.PHONY: all clean

all: $(exe)

$(exe): $(objs)
	$(CC) $(LDFLAGS) $^ $(LDLIBS) -o $@

$(objDir)/%.o: $(srcDir)/%.c include/*.h
	$(CC) $(CPPFLAGS) $(CFLAGS) -c $< -o $@

$(objs): | $(objDir)

$(objDir):
	mkdir $(objDir)

clean:
	$(RM) $(objs) $(exe)
