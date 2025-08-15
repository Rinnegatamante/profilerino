TARGET          := libprofilerino
SOURCES         := source

CFILES   := $(foreach dir,$(SOURCES), $(wildcard $(dir)/*.c))
CPPFILES := $(foreach dir,$(SOURCES), $(wildcard $(dir)/*.cpp))
OBJS     := $(CFILES:.c=.o) $(CPPFILES:.cpp=.o)

SAMPLES     := $(foreach dir,$(wildcard samples/*), $(dir).smp)
SAMPLES_CLR := $(foreach dir,$(wildcard samples/*), $(dir).smpc)

PREFIX  = arm-vita-eabi
CC      = $(PREFIX)-gcc
CXX     = $(PREFIX)-g++
AR      = $(PREFIX)-gcc-ar
CFLAGS  = -g -Wl,-q -O3 -ffast-math -mtune=cortex-a9 -mfpu=neon -Wno-incompatible-pointer-types -Wno-stringop-overflow
ASFLAGS = $(CFLAGS)

CXXFLAGS  = $(CFLAGS) -fexceptions -std=gnu++11 -Wno-write-strings

all: $(TARGET).a

$(TARGET).a: $(OBJS)
	$(AR) -rc $@ $^
	
%.smpc:
	@make -C $(@:.smpc=) clean
	
%.smp:
	@make -C $(@:.smp=)
	ls -1 $(@:.smp=)/*.vpk | xargs -L1 -I{} cp {} .
	
clean: $(SAMPLES_CLR)
	@rm -rf $(TARGET).a $(TARGET).elf $(OBJS)
	
install: $(TARGET).a
	@mkdir -p $(VITASDK)/$(PREFIX)/lib/
	cp $(TARGET).a $(VITASDK)/$(PREFIX)/lib/
	@mkdir -p $(VITASDK)/$(PREFIX)/include/
	cp source/profilerino.h $(VITASDK)/$(PREFIX)/include/
	
samples: $(SAMPLES)
