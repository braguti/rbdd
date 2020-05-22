LIB = ../cudd.a

lib: $(LIB)

include make_o.mk

$(LIB): $(OBJS)
	$(AR) -rucs $(LIB) $(OBJS)
	$(RANLIB) $(LIB)

clean:
	@-rm -f ./cplusplus/*.o
	@-rm -f ./cudd/*.o
	@-rm -f ./dddmp/*.o
	@-rm -f ./epd/*.o
	@-rm -f ./mtr/*.o
	@-rm -f ./st/*.o
	@-rm -f ./util/*.o
