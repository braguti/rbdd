OBJS = ./cplusplus/cuddObj.o ./cudd/cuddAddAbs.o ./cudd/cuddAddApply.o ./cudd/cuddAddFind.o ./cudd/cuddAddInv.o ./cudd/cuddAddIte.o ./cudd/cuddAddNeg.o ./cudd/cuddAddWalsh.o ./cudd/cuddAndAbs.o ./cudd/cuddAnneal.o ./cudd/cuddApa.o ./cudd/cuddAPI.o ./cudd/cuddApprox.o ./cudd/cuddBddAbs.o ./cudd/cuddBddCorr.o ./cudd/cuddBddIte.o ./cudd/cuddBridge.o ./cudd/cuddCache.o ./cudd/cuddCheck.o ./cudd/cuddClip.o ./cudd/cuddCof.o ./cudd/cuddCompose.o ./cudd/cuddDecomp.o ./cudd/cuddEssent.o ./cudd/cuddExact.o ./cudd/cuddExport.o ./cudd/cuddGenCof.o ./cudd/cuddGenetic.o ./cudd/cuddGroup.o ./cudd/cuddHarwell.o ./cudd/cuddInit.o ./cudd/cuddInteract.o ./cudd/cuddInteract.min.max.o ./cudd/cuddLCache.o ./cudd/cuddLevelQ.o ./cudd/cuddLinear.o ./cudd/cuddLiteral.o ./cudd/cuddMatMult.o ./cudd/cuddPriority.o ./cudd/cuddRead.o ./cudd/cuddRef.o ./cudd/cuddReorder.o ./cudd/cuddSat.o ./cudd/cuddSign.o ./cudd/cuddSolve.o ./cudd/cuddSplit.o ./cudd/cuddSubsetHB.o ./cudd/cuddSubsetSP.o ./cudd/cuddSymmetry.o ./cudd/cuddTable.o ./cudd/cuddUtil.o ./cudd/cuddWindow.o ./cudd/cuddZddCount.o ./cudd/cuddZddFuncs.o ./cudd/cuddZddGroup.o ./cudd/cuddZddIsop.o ./cudd/cuddZddLin.o ./cudd/cuddZddMisc.o ./cudd/cuddZddPort.o ./cudd/cuddZddReord.o ./cudd/cuddZddSetop.o ./cudd/cuddZddSymm.o ./cudd/cuddZddUtil.o ./dddmp/dddmpBinary.o ./dddmp/dddmpConvert.o ./dddmp/dddmpDbg.o ./dddmp/dddmpLoad.o ./dddmp/dddmpLoadCnf.o ./dddmp/dddmpNodeAdd.o ./dddmp/dddmpNodeBdd.o ./dddmp/dddmpNodeCnf.o ./dddmp/dddmpStoreAdd.o ./dddmp/dddmpStoreBdd.o ./dddmp/dddmpStoreCnf.o ./dddmp/dddmpStoreMisc.o ./dddmp/dddmpUtil.o ./epd/epd.o ./mtr/mtrBasic.o ./mtr/mtrGroup.o ./st/st.o ./util/cpu_stats.o ./util/cpu_time.o ./util/cstringstream.o ./util/datalimit.o ./util/pathsearch.o ./util/pipefork.o ./util/prtime.o ./util/safe_mem.o ./util/strsav.o ./util/texpand.o ./util/ucbqsort.o

./cplusplus/cuddObj.o: ./cplusplus/cuddObj.cc
	$(PKG_CXX) $(PKG_CXXFLAGS) -I../include -c ./cplusplus/cuddObj.cc -o $@
./cudd/cuddAddAbs.o: ./cudd/cuddAddAbs.c
	$(PKG_CC) $(PKG_CFLAGS) -I../include -c ./cudd/cuddAddAbs.c -o $@
./cudd/cuddAddApply.o: ./cudd/cuddAddApply.c
	$(PKG_CC) $(PKG_CFLAGS) -I../include -c ./cudd/cuddAddApply.c -o $@
 ./cudd/cuddAddFind.o: ./cudd/cuddAddFind.c
	$(PKG_CC) $(PKG_CFLAGS) -I../include -c ./cudd/cuddAddFind.c -o $@
 ./cudd/cuddAddInv.o: ./cudd/cuddAddInv.c
	$(PKG_CC) $(PKG_CFLAGS) -I../include -c ./cudd/cuddAddInv.c -o $@
 ./cudd/cuddAddIte.o: ./cudd/cuddAddIte.c
	$(PKG_CC) $(PKG_CFLAGS) -I../include -c ./cudd/cuddAddIte.c -o $@
 ./cudd/cuddAddNeg.o: ./cudd/cuddAddNeg.c
	$(PKG_CC) $(PKG_CFLAGS) -I../include -c ./cudd/cuddAddNeg.c -o $@
 ./cudd/cuddAddWalsh.o: ./cudd/cuddAddWalsh.c
	$(PKG_CC) $(PKG_CFLAGS) -I../include -c ./cudd/cuddAddWalsh.c -o $@
 ./cudd/cuddAndAbs.o: ./cudd/cuddAndAbs.c
	$(PKG_CC) $(PKG_CFLAGS) -I../include -c ./cudd/cuddAndAbs.c -o $@
 ./cudd/cuddAnneal.o: ./cudd/cuddAnneal.c
	$(PKG_CC) $(PKG_CFLAGS) -I../include -c ./cudd/cuddAnneal.c -o $@
 ./cudd/cuddApa.o: ./cudd/cuddApa.c
	$(PKG_CC) $(PKG_CFLAGS) -I../include -c ./cudd/cuddApa.c -o $@
 ./cudd/cuddAPI.o: ./cudd/cuddAPI.c
	$(PKG_CC) $(PKG_CFLAGS) -I../include -c ./cudd/cuddAPI.c -o $@
 ./cudd/cuddApprox.o: ./cudd/cuddApprox.c
	$(PKG_CC) $(PKG_CFLAGS) -I../include -c ./cudd/cuddApprox.c -o $@
 ./cudd/cuddBddAbs.o: ./cudd/cuddBddAbs.c
	$(PKG_CC) $(PKG_CFLAGS) -I../include -c ./cudd/cuddBddAbs.c -o $@
 ./cudd/cuddBddCorr.o: ./cudd/cuddBddCorr.c
	$(PKG_CC) $(PKG_CFLAGS) -I../include -c ./cudd/cuddBddCorr.c -o $@
 ./cudd/cuddBddIte.o: ./cudd/cuddBddIte.c
	$(PKG_CC) $(PKG_CFLAGS) -I../include -c ./cudd/cuddBddIte.c -o $@
 ./cudd/cuddBridge.o: ./cudd/cuddBridge.c
	$(PKG_CC) $(PKG_CFLAGS) -I../include -c ./cudd/cuddBridge.c -o $@
 ./cudd/cuddCache.o: ./cudd/cuddCache.c
	$(PKG_CC) $(PKG_CFLAGS) -I../include -c ./cudd/cuddCache.c -o $@
 ./cudd/cuddCheck.o: ./cudd/cuddCheck.c
	$(PKG_CC) $(PKG_CFLAGS) -I../include -c ./cudd/cuddCheck.c -o $@
 ./cudd/cuddClip.o: ./cudd/cuddClip.c
	$(PKG_CC) $(PKG_CFLAGS) -I../include -c ./cudd/cuddClip.c -o $@
 ./cudd/cuddCof.o: ./cudd/cuddCof.c
	$(PKG_CC) $(PKG_CFLAGS) -I../include -c ./cudd/cuddCof.c -o $@
 ./cudd/cuddCompose.o: ./cudd/cuddCompose.c
	$(PKG_CC) $(PKG_CFLAGS) -I../include -c ./cudd/cuddCompose.c -o $@
 ./cudd/cuddDecomp.o: ./cudd/cuddDecomp.c
	$(PKG_CC) $(PKG_CFLAGS) -I../include -c ./cudd/cuddDecomp.c -o $@
 ./cudd/cuddEssent.o: ./cudd/cuddEssent.c
	$(PKG_CC) $(PKG_CFLAGS) -I../include -c ./cudd/cuddEssent.c -o $@
 ./cudd/cuddExact.o: ./cudd/cuddExact.c
	$(PKG_CC) $(PKG_CFLAGS) -I../include -c ./cudd/cuddExact.c -o $@
 ./cudd/cuddExport.o: ./cudd/cuddExport.c
	$(PKG_CC) $(PKG_CFLAGS) -I../include -c ./cudd/cuddExport.c -o $@
 ./cudd/cuddGenCof.o: ./cudd/cuddGenCof.c
	$(PKG_CC) $(PKG_CFLAGS) -I../include -c ./cudd/cuddGenCof.c -o $@
 ./cudd/cuddGenetic.o: ./cudd/cuddGenetic.c
	$(PKG_CC) $(PKG_CFLAGS) -I../include -c ./cudd/cuddGenetic.c -o $@
 ./cudd/cuddGroup.o: ./cudd/cuddGroup.c
	$(PKG_CC) $(PKG_CFLAGS) -I../include -c ./cudd/cuddGroup.c -o $@
 ./cudd/cuddHarwell.o: ./cudd/cuddHarwell.c
	$(PKG_CC) $(PKG_CFLAGS) -I../include -c ./cudd/cuddHarwell.c -o $@
 ./cudd/cuddInit.o: ./cudd/cuddInit.c
	$(PKG_CC) $(PKG_CFLAGS) -I../include -c ./cudd/cuddInit.c -o $@
 ./cudd/cuddInteract.o: ./cudd/cuddInteract.c
	$(PKG_CC) $(PKG_CFLAGS) -I../include -c ./cudd/cuddInteract.c -o $@
 ./cudd/cuddInteract.min.max.o: ./cudd/cuddInteract.min.max.c
	$(PKG_CC) $(PKG_CFLAGS) -I../include -c ./cudd/cuddInteract.min.max.c -o $@
 ./cudd/cuddLCache.o: ./cudd/cuddLCache.c
	$(PKG_CC) $(PKG_CFLAGS) -I../include -c ./cudd/cuddLCache.c -o $@
 ./cudd/cuddLevelQ.o: ./cudd/cuddLevelQ.c
	$(PKG_CC) $(PKG_CFLAGS) -I../include -c ./cudd/cuddLevelQ.c -o $@
./cudd/cuddLinear.o: ./cudd/cuddLinear.c
	$(PKG_CC) $(PKG_CFLAGS) -I../include -c ./cudd/cuddLinear.c -o $@
./cudd/cuddLiteral.o: ./cudd/cuddLiteral.c
	$(PKG_CC) $(PKG_CFLAGS) -I../include -c ./cudd/cuddLiteral.c -o $@
./cudd/cuddMatMult.o: ./cudd/cuddMatMult.c
	$(PKG_CC) $(PKG_CFLAGS) -I../include -c ./cudd/cuddMatMult.c -o $@
./cudd/cuddPriority.o: ./cudd/cuddPriority.c
	$(PKG_CC) $(PKG_CFLAGS) -I../include -c ./cudd/cuddPriority.c -o $@
./cudd/cuddRead.o: ./cudd/cuddRead.c
	$(PKG_CC) $(PKG_CFLAGS) -I../include -c ./cudd/cuddRead.c -o $@
./cudd/cuddRef.o: ./cudd/cuddRef.c
	$(PKG_CC) $(PKG_CFLAGS) -I../include -c ./cudd/cuddRef.c -o $@
./cudd/cuddReorder.o: ./cudd/cuddReorder.c
	$(PKG_CC) $(PKG_CFLAGS) -I../include -c ./cudd/cuddReorder.c -o $@
./cudd/cuddSat.o: ./cudd/cuddSat.c
	$(PKG_CC) $(PKG_CFLAGS) -I../include -c ./cudd/cuddSat.c -o $@
./cudd/cuddSign.o: ./cudd/cuddSign.c
	$(PKG_CC) $(PKG_CFLAGS) -I../include -c ./cudd/cuddSign.c -o $@
./cudd/cuddSolve.o: ./cudd/cuddSolve.c
	$(PKG_CC) $(PKG_CFLAGS) -I../include -c ./cudd/cuddSolve.c -o $@
./cudd/cuddSplit.o: ./cudd/cuddSplit.c
	$(PKG_CC) $(PKG_CFLAGS) -I../include -c ./cudd/cuddSplit.c -o $@
./cudd/cuddSubsetHB.o: ./cudd/cuddSubsetHB.c
	$(PKG_CC) $(PKG_CFLAGS) -I../include -c ./cudd/cuddSubsetHB.c -o $@
./cudd/cuddSubsetSP.o: ./cudd/cuddSubsetSP.c
	$(PKG_CC) $(PKG_CFLAGS) -I../include -c ./cudd/cuddSubsetSP.c -o $@
./cudd/cuddSymmetry.o: ./cudd/cuddSymmetry.c
	$(PKG_CC) $(PKG_CFLAGS) -I../include -c ./cudd/cuddSymmetry.c -o $@
./cudd/cuddTable.o: ./cudd/cuddTable.c
	$(PKG_CC) $(PKG_CFLAGS) -I../include -c ./cudd/cuddTable.c -o $@
./cudd/cuddUtil.o: ./cudd/cuddUtil.c
	$(PKG_CC) $(PKG_CFLAGS) -I../include -c ./cudd/cuddUtil.c -o $@
./cudd/cuddWindow.o: ./cudd/cuddWindow.c
	$(PKG_CC) $(PKG_CFLAGS) -I../include -c ./cudd/cuddWindow.c -o $@
./cudd/cuddZddCount.o: ./cudd/cuddZddCount.c
	$(PKG_CC) $(PKG_CFLAGS) -I../include -c ./cudd/cuddZddCount.c -o $@
./cudd/cuddZddFuncs.o: ./cudd/cuddZddFuncs.c
	$(PKG_CC) $(PKG_CFLAGS) -I../include -c ./cudd/cuddZddFuncs.c -o $@
./cudd/cuddZddGroup.o: ./cudd/cuddZddGroup.c
	$(PKG_CC) $(PKG_CFLAGS) -I../include -c ./cudd/cuddZddGroup.c -o $@
./cudd/cuddZddIsop.o: ./cudd/cuddZddIsop.c
	$(PKG_CC) $(PKG_CFLAGS) -I../include -c ./cudd/cuddZddIsop.c -o $@
./cudd/cuddZddLin.o: ./cudd/cuddZddLin.c
	$(PKG_CC) $(PKG_CFLAGS) -I../include -c ./cudd/cuddZddLin.c -o $@
./cudd/cuddZddMisc.o: ./cudd/cuddZddMisc.c
	$(PKG_CC) $(PKG_CFLAGS) -I../include -c ./cudd/cuddZddMisc.c -o $@
./cudd/cuddZddPort.o: ./cudd/cuddZddPort.c
	$(PKG_CC) $(PKG_CFLAGS) -I../include -c ./cudd/cuddZddPort.c -o $@
./cudd/cuddZddReord.o: ./cudd/cuddZddReord.c
	$(PKG_CC) $(PKG_CFLAGS) -I../include -c ./cudd/cuddZddReord.c -o $@
./cudd/cuddZddSetop.o: ./cudd/cuddZddSetop.c
	$(PKG_CC) $(PKG_CFLAGS) -I../include -c ./cudd/cuddZddSetop.c -o $@
./cudd/cuddZddSymm.o: ./cudd/cuddZddSymm.c
	$(PKG_CC) $(PKG_CFLAGS) -I../include -c ./cudd/cuddZddSymm.c -o $@
./cudd/cuddZddUtil.o: ./cudd/cuddZddUtil.c
	$(PKG_CC) $(PKG_CFLAGS) -I../include -c ./cudd/cuddZddUtil.c -o $@
./dddmp/dddmpBinary.o: ./dddmp/dddmpBinary.c
	$(PKG_CC) $(PKG_CFLAGS) -I../include -c ./dddmp/dddmpBinary.c -o $@
./dddmp/dddmpConvert.o: ./dddmp/dddmpConvert.c
	$(PKG_CC) $(PKG_CFLAGS) -I../include -c ./dddmp/dddmpConvert.c -o $@
./dddmp/dddmpDbg.o: ./dddmp/dddmpDbg.c
	$(PKG_CC) $(PKG_CFLAGS) -I../include -c ./dddmp/dddmpDbg.c -o $@
./dddmp/dddmpLoad.o: ./dddmp/dddmpLoad.c
	$(PKG_CC) $(PKG_CFLAGS) -I../include -c ./dddmp/dddmpLoad.c -o $@
./dddmp/dddmpLoadCnf.o: ./dddmp/dddmpLoadCnf.c
	$(PKG_CC) $(PKG_CFLAGS) -I../include -c ./dddmp/dddmpLoadCnf.c -o $@
./dddmp/dddmpNodeAdd.o: ./dddmp/dddmpNodeAdd.c
	$(PKG_CC) $(PKG_CFLAGS) -I../include -c ./dddmp/dddmpNodeAdd.c -o $@
./dddmp/dddmpNodeBdd.o: ./dddmp/dddmpNodeBdd.c
	$(PKG_CC) $(PKG_CFLAGS) -I../include -c ./dddmp/dddmpNodeBdd.c -o $@
./dddmp/dddmpNodeCnf.o: ./dddmp/dddmpNodeCnf.c
	$(PKG_CC) $(PKG_CFLAGS) -I../include -c ./dddmp/dddmpNodeCnf.c -o $@
./dddmp/dddmpStoreAdd.o: ./dddmp/dddmpStoreAdd.c
	$(PKG_CC) $(PKG_CFLAGS) -I../include -c ./dddmp/dddmpStoreAdd.c -o $@
./dddmp/dddmpStoreBdd.o: ./dddmp/dddmpStoreBdd.c
	$(PKG_CC) $(PKG_CFLAGS) -I../include -c ./dddmp/dddmpStoreBdd.c -o $@
./dddmp/dddmpStoreCnf.o: ./dddmp/dddmpStoreCnf.c
	$(PKG_CC) $(PKG_CFLAGS) -I../include -c ./dddmp/dddmpStoreCnf.c -o $@
./dddmp/dddmpStoreMisc.o: ./dddmp/dddmpStoreMisc.c
	$(PKG_CC) $(PKG_CFLAGS) -I../include -c ./dddmp/dddmpStoreMisc.c -o $@
./dddmp/dddmpUtil.o: ./dddmp/dddmpUtil.c
	$(PKG_CC) $(PKG_CFLAGS) -I../include -c ./dddmp/dddmpUtil.c -o $@
./epd/epd.o: ./epd/epd.c
	$(PKG_CC) $(PKG_CFLAGS) -I../include -c ./epd/epd.c -o $@
./mtr/mtrBasic.o: ./mtr/mtrBasic.c
	$(PKG_CC) $(PKG_CFLAGS) -I../include -c ./mtr/mtrBasic.c -o $@
./mtr/mtrGroup.o: ./mtr/mtrGroup.c
	$(PKG_CC) $(PKG_CFLAGS) -I../include -c ./mtr/mtrGroup.c -o $@
./st/st.o: ./st/st.c
	$(PKG_CC) $(PKG_CFLAGS) -I../include -c ./st/st.c -o $@
./util/cpu_stats.o: ./util/cpu_stats.c
	$(PKG_CC) $(PKG_CFLAGS) -I../include -c ./util/cpu_stats.c -o $@
./util/cpu_time.o: ./util/cpu_time.c
	$(PKG_CC) $(PKG_CFLAGS) -I../include -c ./util/cpu_time.c -o $@
./util/cstringstream.o: ./util/cstringstream.c
	$(PKG_CC) $(PKG_CFLAGS) -I../include -c ./util/cstringstream.c -o $@
./util/datalimit.o: ./util/datalimit.c
	$(PKG_CC) $(PKG_CFLAGS) -I../include -c ./util/datalimit.c -o $@
./util/pathsearch.o: ./util/pathsearch.c
	$(PKG_CC) $(PKG_CFLAGS) -I../include -c ./util/pathsearch.c -o $@
./util/pipefork.o: ./util/pipefork.c
	$(PKG_CC) $(PKG_CFLAGS) -I../include -c ./util/pipefork.c -o $@
./util/prtime.o: ./util/prtime.c
	$(PKG_CC) $(PKG_CFLAGS) -I../include -c ./util/prtime.c -o $@
./util/safe_mem.o: ./util/safe_mem.c
	$(PKG_CC) $(PKG_CFLAGS) -I../include -c ./util/safe_mem.c -o $@
./util/strsav.o: ./util/strsav.c
	$(PKG_CC) $(PKG_CFLAGS) -I../include -c ./util/strsav.c -o $@
./util/texpand.o: ./util/texpand.c
	$(PKG_CC) $(PKG_CFLAGS) -I../include -c ./util/texpand.c -o $@
./util/ucbqsort.o: ./util/ucbqsort.c
	$(PKG_CC) $(PKG_CFLAGS) -I../include -c ./util/ucbqsort.c -o $@