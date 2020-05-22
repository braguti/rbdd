OBJS =./assert.o ./errno.o ./extract-dbl.o ./invalid.o ./memory.o ./mp_bpl.o ./mp_clz_tab.o ./mp_get_fns.o ./mp_minv_tab.o ./tal-reent.o ./mpf/add.o ./mpf/clear.o ./mpf/cmp_d.o ./mpf/cmp.o ./mpf/div.o ./mpf/get_dfl_prec.o ./mpf/get_prc.o ./mpf/get_str.o ./mpf/init.o ./mpf/init2.o ./mpf/neg.o ./mpf/set_dfl_prec.o ./mpf/set_z.o ./mpf/set.o ./mpf/sub.o ./mpn/add_1.o ./mpn/add_n.o ./mpn/add.o ./mpn/addmul_1.o ./mpn/bdiv_dbm1c.o ./mpn/cmp.o ./mpn/com.o ./mpn/dcpi1_div_q.o ./mpn/dcpi1_div_qr.o ./mpn/dcpi1_divappr_q.o ./mpn/div_q.o ./mpn/dive_1.o ./mpn/divrem_1.o ./mpn/divrem_2.o ./mpn/get_str.o ./mpn/invertappr.o ./mpn/lshift.o ./mpn/lshiftc.o ./mpn/mp_bases.o ./mpn/mu_div_q.o ./mpn/mu_div_qr.o ./mpn/mu_divappr_q.o ./mpn/mul_1.o ./mpn/mul_basecase.o ./mpn/mul_fft.o ./mpn/mul_n.o ./mpn/neg.o ./mpn/mul.o ./mpn/mulmod_bnm1.o ./mpn/nussbaumer_mul.o ./mpn/pre_divrem_1.o ./mpn/rshift.o ./mpn/sbpi1_div_q.o ./mpn/sbpi1_div_qr.o ./mpn/sbpi1_divappr_q.o ./mpn/sqr_basecase.o ./mpn/sqr.o ./mpn/sqrmod_bnm1.o ./mpn/sub_1.o ./mpn/sub_n.o ./mpn/sub.o ./mpn/submul_1.o ./mpn/tdiv_qr.o ./mpn/toom_couple_handling.o ./mpn/toom_eval_dgr3_pm1.o ./mpn/toom_eval_dgr3_pm2.o ./mpn/toom_eval_pm1.o ./mpn/toom_eval_pm2.o ./mpn/toom_eval_pm2exp.o ./mpn/toom_eval_pm2rexp.o ./mpn/toom_interpolate_5pts.o ./mpn/toom_interpolate_6pts.o ./mpn/toom_interpolate_7pts.o ./mpn/toom_interpolate_8pts.o ./mpn/toom_interpolate_12pts.o ./mpn/toom_interpolate_16pts.o ./mpn/toom2_sqr.o ./mpn/toom3_sqr.o ./mpn/toom4_sqr.o ./mpn/toom6_sqr.o ./mpn/toom6h_mul.o ./mpn/toom8_sqr.o ./mpn/toom8h_mul.o ./mpn/toom22_mul.o ./mpn/toom32_mul.o ./mpn/toom33_mul.o ./mpn/toom42_mul.o ./mpn/toom43_mul.o ./mpn/toom44_mul.o ./mpn/toom53_mul.o ./mpn/toom63_mul.o ./mpn/zero_p.o ./mpq/clear.o ./mpz/add.o ./mpz/clear.o ./mpz/get_str.o ./mpz/init.o ./mpz/iset_si.o ./mpz/iset_ui.o ./mpz/iset.o ./mpz/mul.o ./mpz/n_pow_ui.o ./mpz/realloc.o ./mpz/set_si.o ./mpz/set_ui.o ./mpz/set.o ./mpz/tdiv_q_2exp.o ./mpz/tdiv_q_ui.o ./mpz/ui_pow_ui.o

./assert.o: ./assert.c
	$(PKG_CC) $(PKG_CFLAGS) -I../include -c ./assert.c -o $@
./errno.o: ./errno.c
	$(PKG_CC) $(PKG_CFLAGS) -I../include -c ./errno.c -o $@
./extract-dbl.o: ./extract-dbl.c
	$(PKG_CC) $(PKG_CFLAGS) -I../include -c ./extract-dbl.c -o $@
./invalid.o: ./invalid.c
	$(PKG_CC) $(PKG_CFLAGS) -I../include -c ./invalid.c -o $@
./memory.o: ./memory.c
	$(PKG_CC) $(PKG_CFLAGS) -I../include -c ./memory.c -o $@
./mp_bpl.o: ./mp_bpl.c
	$(PKG_CC) $(PKG_CFLAGS) -I../include -c ./mp_bpl.c -o $@
./mp_clz_tab.o: ./mp_clz_tab.c
	$(PKG_CC) $(PKG_CFLAGS) -I../include -c ./mp_clz_tab.c -o $@
./mp_get_fns.o: ./mp_get_fns.c
	$(PKG_CC) $(PKG_CFLAGS) -I../include -c ./mp_get_fns.c -o $@
./mp_minv_tab.o: ./mp_minv_tab.c
	$(PKG_CC) $(PKG_CFLAGS) -I../include -c ./mp_minv_tab.c -o $@
./tal-reent.o: ./tal-reent.c
	$(PKG_CC) $(PKG_CFLAGS) -I../include -c ./tal-reent.c -o $@
./mpf/add.o: ./mpf/add.c
	$(PKG_CC) $(PKG_CFLAGS) -I../include -c ./mpf/add.c -o $@
./mpf/clear.o: ./mpf/clear.c
	$(PKG_CC) $(PKG_CFLAGS) -I../include -c ./mpf/clear.c -o $@
./mpf/cmp_d.o: ./mpf/cmp_d.c
	$(PKG_CC) $(PKG_CFLAGS) -I../include -c ./mpf/cmp_d.c -o $@
./mpf/cmp.o: ./mpf/cmp.c
	$(PKG_CC) $(PKG_CFLAGS) -I../include -c ./mpf/cmp.c -o $@
./mpf/div.o: ./mpf/div.c
	$(PKG_CC) $(PKG_CFLAGS) -I../include -c ./mpf/div.c -o $@
./mpf/get_dfl_prec.o: ./mpf/get_dfl_prec.c
	$(PKG_CC) $(PKG_CFLAGS) -I../include -c ./mpf/get_dfl_prec.c -o $@
./mpf/get_prc.o: ./mpf/get_prc.c
	$(PKG_CC) $(PKG_CFLAGS) -I../include -c ./mpf/get_prc.c -o $@
./mpf/get_str.o: ./mpf/get_str.c
	$(PKG_CC) $(PKG_CFLAGS) -I../include -c ./mpf/get_str.c -o $@
./mpf/init.o: ./mpf/init.c
	$(PKG_CC) $(PKG_CFLAGS) -I../include -c ./mpf/init.c -o $@
./mpf/init2.o: ./mpf/init2.c
	$(PKG_CC) $(PKG_CFLAGS) -I../include -c ./mpf/init2.c -o $@
./mpf/neg.o: ./mpf/neg.c
	$(PKG_CC) $(PKG_CFLAGS) -I../include -c ./mpf/neg.c -o $@
./mpf/set_dfl_prec.o: ./mpf/set_dfl_prec.c
	$(PKG_CC) $(PKG_CFLAGS) -I../include -c ./mpf/set_dfl_prec.c -o $@
./mpf/set_z.o: ./mpf/set_z.c
	$(PKG_CC) $(PKG_CFLAGS) -I../include -c ./mpf/set_z.c -o $@
./mpf/set.o: ./mpf/set.c
	$(PKG_CC) $(PKG_CFLAGS) -I../include -c ./mpf/set.c -o $@
./mpf/sub.o: ./mpf/sub.c
	$(PKG_CC) $(PKG_CFLAGS) -I../include -c ./mpf/sub.c -o $@
./mpn/add.o: ./mpn/add.c
	$(PKG_CC) $(PKG_CFLAGS) -I../include -c ./mpn/add.c -o $@
./mpn/add_1.o: ./mpn/add_1.c
	$(PKG_CC) $(PKG_CFLAGS) -I../include -c ./mpn/add_1.c -o $@
./mpn/addmul_1.o: ./mpn/addmul_1.c
	$(PKG_CC) $(PKG_CFLAGS) -I../include -c ./mpn/addmul_1.c -o $@
./mpn/add_n.o: ./mpn/add_n.c
	$(PKG_CC) $(PKG_CFLAGS) -I../include -c ./mpn/add_n.c -o $@
./mpn/bdiv_dbm1c.o: ./mpn/bdiv_dbm1c.c
	$(PKG_CC) $(PKG_CFLAGS) -I../include -c ./mpn/bdiv_dbm1c.c -o $@
./mpn/cmp.o: ./mpn/cmp.c
	$(PKG_CC) $(PKG_CFLAGS) -I../include -c ./mpn/cmp.c -o $@
./mpn/com.o: ./mpn/com.c
	$(PKG_CC) $(PKG_CFLAGS) -I../include -c ./mpn/com.c -o $@
./mpn/dcpi1_divappr_q.o: ./mpn/dcpi1_divappr_q.c
	$(PKG_CC) $(PKG_CFLAGS) -I../include -c ./mpn/dcpi1_divappr_q.c -o $@
./mpn/dcpi1_div_q.o: ./mpn/dcpi1_div_q.c
	$(PKG_CC) $(PKG_CFLAGS) -I../include -c ./mpn/dcpi1_div_q.c -o $@
./mpn/dcpi1_div_qr.o: ./mpn/dcpi1_div_qr.c
	$(PKG_CC) $(PKG_CFLAGS) -I../include -c ./mpn/dcpi1_div_qr.c -o $@
./mpn/dive_1.o: ./mpn/dive_1.c
	$(PKG_CC) $(PKG_CFLAGS) -I../include -c ./mpn/dive_1.c -o $@
./mpn/div_q.o: ./mpn/div_q.c
	$(PKG_CC) $(PKG_CFLAGS) -I../include -c ./mpn/div_q.c -o $@
./mpn/divrem_1.o: ./mpn/divrem_1.c
	$(PKG_CC) $(PKG_CFLAGS) -I../include -c ./mpn/divrem_1.c -o $@
./mpn/divrem_2.o: ./mpn/divrem_2.c
	$(PKG_CC) $(PKG_CFLAGS) -I../include -c ./mpn/divrem_2.c -o $@
./mpn/get_str.o: ./mpn/get_str.c
	$(PKG_CC) $(PKG_CFLAGS) -I../include -c ./mpn/get_str.c -o $@
./mpn/invertappr.o: ./mpn/invertappr.c
	$(PKG_CC) $(PKG_CFLAGS) -I../include -c ./mpn/invertappr.c -o $@
./mpn/lshift.o: ./mpn/lshift.c
	$(PKG_CC) $(PKG_CFLAGS) -I../include -c ./mpn/lshift.c -o $@
./mpn/lshiftc.o: ./mpn/lshiftc.c
	$(PKG_CC) $(PKG_CFLAGS) -I../include -c ./mpn/lshiftc.c -o $@
./mpn/mp_bases.o: ./mpn/mp_bases.c
	$(PKG_CC) $(PKG_CFLAGS) -I../include -c ./mpn/mp_bases.c -o $@
./mpn/mu_divappr_q.o: ./mpn/mu_divappr_q.c
	$(PKG_CC) $(PKG_CFLAGS) -I../include -c ./mpn/mu_divappr_q.c -o $@
./mpn/mu_div_q.o: ./mpn/mu_div_q.c
	$(PKG_CC) $(PKG_CFLAGS) -I../include -c ./mpn/mu_div_q.c -o $@
./mpn/mu_div_qr.o: ./mpn/mu_div_qr.c
	$(PKG_CC) $(PKG_CFLAGS) -I../include -c ./mpn/mu_div_qr.c -o $@
./mpn/mul.o: ./mpn/mul.c
	$(PKG_CC) $(PKG_CFLAGS) -I../include -c ./mpn/mul.c -o $@
./mpn/mul_1.o: ./mpn/mul_1.c
	$(PKG_CC) $(PKG_CFLAGS) -I../include -c ./mpn/mul_1.c -o $@
./mpn/mul_basecase.o: ./mpn/mul_basecase.c
	$(PKG_CC) $(PKG_CFLAGS) -I../include -c ./mpn/mul_basecase.c -o $@
./mpn/mul_fft.o: ./mpn/mul_fft.c
	$(PKG_CC) $(PKG_CFLAGS) -I../include -c ./mpn/mul_fft.c -o $@
./mpn/mulmod_bnm1.o: ./mpn/mulmod_bnm1.c
	$(PKG_CC) $(PKG_CFLAGS) -I../include -c ./mpn/mulmod_bnm1.c -o $@
./mpn/mul_n.o: ./mpn/mul_n.c
	$(PKG_CC) $(PKG_CFLAGS) -I../include -c ./mpn/mul_n.c -o $@
./mpn/neg.o: ./mpn/neg.c
	$(PKG_CC) $(PKG_CFLAGS) -I../include -c ./mpn/neg.c -o $@
./mpn/nussbaumer_mul.o: ./mpn/nussbaumer_mul.c
	$(PKG_CC) $(PKG_CFLAGS) -I../include -c ./mpn/nussbaumer_mul.c -o $@
./mpn/pre_divrem_1.o: ./mpn/pre_divrem_1.c
	$(PKG_CC) $(PKG_CFLAGS) -I../include -c ./mpn/pre_divrem_1.c -o $@
./mpn/rshift.o: ./mpn/rshift.c
	$(PKG_CC) $(PKG_CFLAGS) -I../include -c ./mpn/rshift.c -o $@
./mpn/sbpi1_divappr_q.o: ./mpn/sbpi1_divappr_q.c
	$(PKG_CC) $(PKG_CFLAGS) -I../include -c ./mpn/sbpi1_divappr_q.c -o $@
./mpn/sbpi1_div_q.o: ./mpn/sbpi1_div_q.c
	$(PKG_CC) $(PKG_CFLAGS) -I../include -c ./mpn/sbpi1_div_q.c -o $@
./mpn/sbpi1_div_qr.o: ./mpn/sbpi1_div_qr.c
	$(PKG_CC) $(PKG_CFLAGS) -I../include -c ./mpn/sbpi1_div_qr.c -o $@
./mpn/sqr.o: ./mpn/sqr.c
	$(PKG_CC) $(PKG_CFLAGS) -I../include -c ./mpn/sqr.c -o $@
./mpn/sqr_basecase.o: ./mpn/sqr_basecase.c
	$(PKG_CC) $(PKG_CFLAGS) -I../include -c ./mpn/sqr_basecase.c -o $@
./mpn/sqrmod_bnm1.o: ./mpn/sqrmod_bnm1.c
	$(PKG_CC) $(PKG_CFLAGS) -I../include -c ./mpn/sqrmod_bnm1.c -o $@
./mpn/sub.o: ./mpn/sub.c
	$(PKG_CC) $(PKG_CFLAGS) -I../include -c ./mpn/sub.c -o $@
./mpn/sub_1.o: ./mpn/sub_1.c
	$(PKG_CC) $(PKG_CFLAGS) -I../include -c ./mpn/sub_1.c -o $@
./mpn/submul_1.o: ./mpn/submul_1.c
	$(PKG_CC) $(PKG_CFLAGS) -I../include -c ./mpn/submul_1.c -o $@
./mpn/sub_n.o: ./mpn/sub_n.c
	$(PKG_CC) $(PKG_CFLAGS) -I../include -c ./mpn/sub_n.c -o $@
./mpn/tdiv_qr.o: ./mpn/tdiv_qr.c
	$(PKG_CC) $(PKG_CFLAGS) -I../include -c ./mpn/tdiv_qr.c -o $@
./mpn/toom2_sqr.o: ./mpn/toom2_sqr.c
	$(PKG_CC) $(PKG_CFLAGS) -I../include -c ./mpn/toom2_sqr.c -o $@
./mpn/toom3_sqr.o: ./mpn/toom3_sqr.c
	$(PKG_CC) $(PKG_CFLAGS) -I../include -c ./mpn/toom3_sqr.c -o $@
./mpn/toom4_sqr.o: ./mpn/toom4_sqr.c
	$(PKG_CC) $(PKG_CFLAGS) -I../include -c ./mpn/toom4_sqr.c -o $@
./mpn/toom6h_mul.o: ./mpn/toom6h_mul.c
	$(PKG_CC) $(PKG_CFLAGS) -I../include -c ./mpn/toom6h_mul.c -o $@
./mpn/toom6_sqr.o: ./mpn/toom6_sqr.c
	$(PKG_CC) $(PKG_CFLAGS) -I../include -c ./mpn/toom6_sqr.c -o $@
./mpn/toom8h_mul.o: ./mpn/toom8h_mul.c
	$(PKG_CC) $(PKG_CFLAGS) -I../include -c ./mpn/toom8h_mul.c -o $@
./mpn/toom8_sqr.o: ./mpn/toom8_sqr.c
	$(PKG_CC) $(PKG_CFLAGS) -I../include -c ./mpn/toom8_sqr.c -o $@
./mpn/toom22_mul.o: ./mpn/toom22_mul.c
	$(PKG_CC) $(PKG_CFLAGS) -I../include -c ./mpn/toom22_mul.c -o $@
./mpn/toom32_mul.o: ./mpn/toom32_mul.c
	$(PKG_CC) $(PKG_CFLAGS) -I../include -c ./mpn/toom32_mul.c -o $@
./mpn/toom33_mul.o: ./mpn/toom33_mul.c
	$(PKG_CC) $(PKG_CFLAGS) -I../include -c ./mpn/toom33_mul.c -o $@
./mpn/toom42_mul.o: ./mpn/toom42_mul.c
	$(PKG_CC) $(PKG_CFLAGS) -I../include -c ./mpn/toom42_mul.c -o $@
./mpn/toom43_mul.o: ./mpn/toom43_mul.c
	$(PKG_CC) $(PKG_CFLAGS) -I../include -c ./mpn/toom43_mul.c -o $@
./mpn/toom44_mul.o: ./mpn/toom44_mul.c
	$(PKG_CC) $(PKG_CFLAGS) -I../include -c ./mpn/toom44_mul.c -o $@
./mpn/toom53_mul.o: ./mpn/toom53_mul.c
	$(PKG_CC) $(PKG_CFLAGS) -I../include -c ./mpn/toom53_mul.c -o $@
./mpn/toom63_mul.o: ./mpn/toom63_mul.c
	$(PKG_CC) $(PKG_CFLAGS) -I../include -c ./mpn/toom63_mul.c -o $@
./mpn/toom_couple_handling.o: ./mpn/toom_couple_handling.c
	$(PKG_CC) $(PKG_CFLAGS) -I../include -c ./mpn/toom_couple_handling.c -o $@
./mpn/toom_eval_dgr3_pm1.o: ./mpn/toom_eval_dgr3_pm1.c
	$(PKG_CC) $(PKG_CFLAGS) -I../include -c ./mpn/toom_eval_dgr3_pm1.c -o $@
./mpn/toom_eval_dgr3_pm2.o: ./mpn/toom_eval_dgr3_pm2.c
	$(PKG_CC) $(PKG_CFLAGS) -I../include -c ./mpn/toom_eval_dgr3_pm2.c -o $@
./mpn/toom_eval_pm1.o: ./mpn/toom_eval_pm1.c
	$(PKG_CC) $(PKG_CFLAGS) -I../include -c ./mpn/toom_eval_pm1.c -o $@
./mpn/toom_eval_pm2.o: ./mpn/toom_eval_pm2.c
	$(PKG_CC) $(PKG_CFLAGS) -I../include -c ./mpn/toom_eval_pm2.c -o $@
./mpn/toom_eval_pm2exp.o: ./mpn/toom_eval_pm2exp.c
	$(PKG_CC) $(PKG_CFLAGS) -I../include -c ./mpn/toom_eval_pm2exp.c -o $@
./mpn/toom_eval_pm2rexp.o: ./mpn/toom_eval_pm2rexp.c
	$(PKG_CC) $(PKG_CFLAGS) -I../include -c ./mpn/toom_eval_pm2rexp.c -o $@
./mpn/toom_interpolate_5pts.o: ./mpn/toom_interpolate_5pts.c
	$(PKG_CC) $(PKG_CFLAGS) -I../include -c ./mpn/toom_interpolate_5pts.c -o $@
./mpn/toom_interpolate_6pts.o: ./mpn/toom_interpolate_6pts.c
	$(PKG_CC) $(PKG_CFLAGS) -I../include -c ./mpn/toom_interpolate_6pts.c -o $@
./mpn/toom_interpolate_7pts.o: ./mpn/toom_interpolate_7pts.c
	$(PKG_CC) $(PKG_CFLAGS) -I../include -c ./mpn/toom_interpolate_7pts.c -o $@
./mpn/toom_interpolate_8pts.o: ./mpn/toom_interpolate_8pts.c
	$(PKG_CC) $(PKG_CFLAGS) -I../include -c ./mpn/toom_interpolate_8pts.c -o $@
./mpn/toom_interpolate_12pts.o: ./mpn/toom_interpolate_12pts.c
	$(PKG_CC) $(PKG_CFLAGS) -I../include -c ./mpn/toom_interpolate_12pts.c -o $@
./mpn/toom_interpolate_16pts.o: ./mpn/toom_interpolate_16pts.c
	$(PKG_CC) $(PKG_CFLAGS) -I../include -c ./mpn/toom_interpolate_16pts.c -o $@
./mpn/zero_p.o: ./mpn/zero_p.c
	$(PKG_CC) $(PKG_CFLAGS) -I../include -c ./mpn/zero_p.c -o $@
./mpq/clear.o: ./mpq/clear.c
	$(PKG_CC) $(PKG_CFLAGS) -I../include -c ./mpq/clear.c -o $@
./mpz/add.o: ./mpz/add.c
	$(PKG_CC) $(PKG_CFLAGS) -I../include -c ./mpz/add.c -o $@
./mpz/clear.o: ./mpz/clear.c
	$(PKG_CC) $(PKG_CFLAGS) -I../include -c ./mpz/clear.c -o $@
./mpz/get_str.o: ./mpz/get_str.c
	$(PKG_CC) $(PKG_CFLAGS) -I../include -c ./mpz/get_str.c -o $@
./mpz/init.o: ./mpz/init.c
	$(PKG_CC) $(PKG_CFLAGS) -I../include -c ./mpz/init.c -o $@
./mpz/iset_si.o: ./mpz/iset_si.c
	$(PKG_CC) $(PKG_CFLAGS) -I../include -c ./mpz/iset_si.c -o $@
./mpz/iset_ui.o: ./mpz/iset_ui.c
	$(PKG_CC) $(PKG_CFLAGS) -I../include -c ./mpz/iset_ui.c -o $@
./mpz/iset.o: ./mpz/iset.c
	$(PKG_CC) $(PKG_CFLAGS) -I../include -c ./mpz/iset.c -o $@
./mpz/mul.o: ./mpz/mul.c
	$(PKG_CC) $(PKG_CFLAGS) -I../include -c ./mpz/mul.c -o $@
./mpz/n_pow_ui.o: ./mpz/n_pow_ui.c
	$(PKG_CC) $(PKG_CFLAGS) -I../include -c ./mpz/n_pow_ui.c -o $@
./mpz/realloc.o: ./mpz/realloc.c
	$(PKG_CC) $(PKG_CFLAGS) -I../include -c ./mpz/realloc.c -o $@
./mpz/set.o: ./mpz/set.c
	$(PKG_CC) $(PKG_CFLAGS) -I../include -c ./mpz/set.c -o $@
./mpz/set_si.o: ./mpz/set_si.c
	$(PKG_CC) $(PKG_CFLAGS) -I../include -c ./mpz/set_si.c -o $@
./mpz/set_ui.o: ./mpz/set_ui.c
	$(PKG_CC) $(PKG_CFLAGS) -I../include -c ./mpz/set_ui.c -o $@
./mpz/tdiv_q_2exp.o: ./mpz/tdiv_q_2exp.c
	$(PKG_CC) $(PKG_CFLAGS) -I../include -c ./mpz/tdiv_q_2exp.c -o $@
./mpz/tdiv_q_ui.o: ./mpz/tdiv_q_ui.c
	$(PKG_CC) $(PKG_CFLAGS) -I../include -c ./mpz/tdiv_q_ui.c -o $@
./mpz/ui_pow_ui.o: ./mpz/ui_pow_ui.c
	$(PKG_CC) $(PKG_CFLAGS) -I../include -c ./mpz/ui_pow_ui.c -o $@
