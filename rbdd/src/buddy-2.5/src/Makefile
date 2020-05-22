LIB = ../buddy.a

lib: $(LIB)

include make_o.mk

$(LIB): $(OBJS)
	$(AR) -rcs $(LIB) $(OBJS)
	$(RANLIB) $(LIB)

clean:
	@-rm -f *.o
