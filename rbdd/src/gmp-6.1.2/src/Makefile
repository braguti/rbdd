LIB = ../gmp.a

lib: $(LIB)

include make_o.mk

$(LIB): $(OBJS)
	$(AR) -rcs $(LIB) $(OBJS)
	$(RANLIB) $(LIB)

clean:
	@-rm -f ./*.o
	@-rm -f ./mini-gmp/*.o
	@-rm -f ./cxx/*.o
	@-rm -f ./mpf/*.o
	@-rm -f ./mpn/*.o
	@-rm -f ./mpq/*.o
	@-rm -f ./mpz/*.o
	@-rm -f ./printf/*.o
	@-rm -f ./rand/*.o
	@-rm -f ./scanf/*.o
