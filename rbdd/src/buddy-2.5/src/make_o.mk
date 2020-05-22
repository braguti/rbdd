OBJS = bddio.o bddop.o bddtest.o bvec.o cache.o cppext.o fdd.o imatrix.o kernel.o pairs.o prime.o reorder.o tree.o

bddio.o: bddio.c
	$(PKG_CC) $(PKG_CFLAGS) -I../include -c bddio.c -o $@
bddop.o: bddop.c
	$(PKG_CC) $(PKG_CFLAGS) -I../include -c bddop.c -o $@
bddtest.o: bddtest.cxx
	$(PKG_CXX) $(PKG_CXXFLAGS) -I../include -c bddtest.cxx -o $@
bvec.o: bvec.c
	$(PKG_CC) $(PKG_CFLAGS) -I../include -c bvec.c -o $@
cache.o: cache.c
	$(PKG_CC) $(PKG_CFLAGS) -I../include -c cache.c -o $@
cppext.o: cppext.cxx
	$(PKG_CXX) $(PKG_CXXFLAGS) -I../include -c cppext.cxx -o $@
fdd.o: fdd.c
	$(PKG_CC) $(PKG_CFLAGS) -I../include -c fdd.c -o $@
imatrix.o: imatrix.c
	$(PKG_CC) $(PKG_CFLAGS) -I../include -c imatrix.c -o $@
kernel.o: kernel.c
	$(PKG_CC) $(PKG_CFLAGS) -I../include -c kernel.c -o $@
pairs.o: pairs.c
	$(PKG_CC) $(PKG_CFLAGS) -I../include -c pairs.c -o $@
prime.o: prime.c
	$(PKG_CC) $(PKG_CFLAGS) -I../include -c prime.c -o $@
reorder.o: reorder.c
	$(PKG_CC) $(PKG_CFLAGS) -I../include -c reorder.c -o $@
tree.o: tree.c
	$(PKG_CC) $(PKG_CFLAGS) -I../include -c tree.c -o $@