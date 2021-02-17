SRCS := tee.c get_num.c

OBJS := $(patsubst %.c,%.o,$(SRCS))

APP = tee

all: $(APP)

$(APP): $(OBJS)
	echo "$(OBJS)"
	gcc $(OBJS) -o $@

%.o: %.c
	gcc -c $< -o $(@F)

clean: 
	rm *.o $(APP)
