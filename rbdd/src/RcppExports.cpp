// Generated by using Rcpp::compileAttributes() -> do not edit by hand
// Generator token: 10BE3573-1514-4C36-9D1C-5A225CD40393

#include <Rcpp.h>

using namespace Rcpp;

// bdd_manager_init
void bdd_manager_init(std::string library, int node_num, int cache_size, std::string bdd_name);
RcppExport SEXP _rbdd_bdd_manager_init(SEXP librarySEXP, SEXP node_numSEXP, SEXP cache_sizeSEXP, SEXP bdd_nameSEXP) {
BEGIN_RCPP
    Rcpp::RNGScope rcpp_rngScope_gen;
    Rcpp::traits::input_parameter< std::string >::type library(librarySEXP);
    Rcpp::traits::input_parameter< int >::type node_num(node_numSEXP);
    Rcpp::traits::input_parameter< int >::type cache_size(cache_sizeSEXP);
    Rcpp::traits::input_parameter< std::string >::type bdd_name(bdd_nameSEXP);
    bdd_manager_init(library, node_num, cache_size, bdd_name);
    return R_NilValue;
END_RCPP
}
// bdd_manager_quit
void bdd_manager_quit(std::string bdd_name);
RcppExport SEXP _rbdd_bdd_manager_quit(SEXP bdd_nameSEXP) {
BEGIN_RCPP
    Rcpp::RNGScope rcpp_rngScope_gen;
    Rcpp::traits::input_parameter< std::string >::type bdd_name(bdd_nameSEXP);
    bdd_manager_quit(bdd_name);
    return R_NilValue;
END_RCPP
}
// bdd_manager_reset
void bdd_manager_reset(std::string bdd_name);
RcppExport SEXP _rbdd_bdd_manager_reset(SEXP bdd_nameSEXP) {
BEGIN_RCPP
    Rcpp::RNGScope rcpp_rngScope_gen;
    Rcpp::traits::input_parameter< std::string >::type bdd_name(bdd_nameSEXP);
    bdd_manager_reset(bdd_name);
    return R_NilValue;
END_RCPP
}
// bdd_new_variable
int bdd_new_variable(std::string variable_name, std::string var_type, std::string bdd_name);
RcppExport SEXP _rbdd_bdd_new_variable(SEXP variable_nameSEXP, SEXP var_typeSEXP, SEXP bdd_nameSEXP) {
BEGIN_RCPP
    Rcpp::RObject rcpp_result_gen;
    Rcpp::RNGScope rcpp_rngScope_gen;
    Rcpp::traits::input_parameter< std::string >::type variable_name(variable_nameSEXP);
    Rcpp::traits::input_parameter< std::string >::type var_type(var_typeSEXP);
    Rcpp::traits::input_parameter< std::string >::type bdd_name(bdd_nameSEXP);
    rcpp_result_gen = Rcpp::wrap(bdd_new_variable(variable_name, var_type, bdd_name));
    return rcpp_result_gen;
END_RCPP
}
// bdd_parse_cnf
int bdd_parse_cnf(std::string file, std::string bdd_name, std::string score, int window, std::string reorder, int min_nodes, int line_length, bool dyn_comp);
RcppExport SEXP _rbdd_bdd_parse_cnf(SEXP fileSEXP, SEXP bdd_nameSEXP, SEXP scoreSEXP, SEXP windowSEXP, SEXP reorderSEXP, SEXP min_nodesSEXP, SEXP line_lengthSEXP, SEXP dyn_compSEXP) {
BEGIN_RCPP
    Rcpp::RObject rcpp_result_gen;
    Rcpp::RNGScope rcpp_rngScope_gen;
    Rcpp::traits::input_parameter< std::string >::type file(fileSEXP);
    Rcpp::traits::input_parameter< std::string >::type bdd_name(bdd_nameSEXP);
    Rcpp::traits::input_parameter< std::string >::type score(scoreSEXP);
    Rcpp::traits::input_parameter< int >::type window(windowSEXP);
    Rcpp::traits::input_parameter< std::string >::type reorder(reorderSEXP);
    Rcpp::traits::input_parameter< int >::type min_nodes(min_nodesSEXP);
    Rcpp::traits::input_parameter< int >::type line_length(line_lengthSEXP);
    Rcpp::traits::input_parameter< bool >::type dyn_comp(dyn_compSEXP);
    rcpp_result_gen = Rcpp::wrap(bdd_parse_cnf(file, bdd_name, score, window, reorder, min_nodes, line_length, dyn_comp));
    return rcpp_result_gen;
END_RCPP
}
// bdd_parse_splot
int bdd_parse_splot(std::string file, std::string bdd_name, std::string score, int window, std::string reorder, int min_nodes, int line_length, bool dyn_comp);
RcppExport SEXP _rbdd_bdd_parse_splot(SEXP fileSEXP, SEXP bdd_nameSEXP, SEXP scoreSEXP, SEXP windowSEXP, SEXP reorderSEXP, SEXP min_nodesSEXP, SEXP line_lengthSEXP, SEXP dyn_compSEXP) {
BEGIN_RCPP
    Rcpp::RObject rcpp_result_gen;
    Rcpp::RNGScope rcpp_rngScope_gen;
    Rcpp::traits::input_parameter< std::string >::type file(fileSEXP);
    Rcpp::traits::input_parameter< std::string >::type bdd_name(bdd_nameSEXP);
    Rcpp::traits::input_parameter< std::string >::type score(scoreSEXP);
    Rcpp::traits::input_parameter< int >::type window(windowSEXP);
    Rcpp::traits::input_parameter< std::string >::type reorder(reorderSEXP);
    Rcpp::traits::input_parameter< int >::type min_nodes(min_nodesSEXP);
    Rcpp::traits::input_parameter< int >::type line_length(line_lengthSEXP);
    Rcpp::traits::input_parameter< bool >::type dyn_comp(dyn_compSEXP);
    rcpp_result_gen = Rcpp::wrap(bdd_parse_splot(file, bdd_name, score, window, reorder, min_nodes, line_length, dyn_comp));
    return rcpp_result_gen;
END_RCPP
}
// bdd_parse_boolstr
int bdd_parse_boolstr(std::string expression, std::string bdd_name);
RcppExport SEXP _rbdd_bdd_parse_boolstr(SEXP expressionSEXP, SEXP bdd_nameSEXP) {
BEGIN_RCPP
    Rcpp::RObject rcpp_result_gen;
    Rcpp::RNGScope rcpp_rngScope_gen;
    Rcpp::traits::input_parameter< std::string >::type expression(expressionSEXP);
    Rcpp::traits::input_parameter< std::string >::type bdd_name(bdd_nameSEXP);
    rcpp_result_gen = Rcpp::wrap(bdd_parse_boolstr(expression, bdd_name));
    return rcpp_result_gen;
END_RCPP
}
// bdd_order
void bdd_order(std::string reorder_method, std::string bdd_name);
RcppExport SEXP _rbdd_bdd_order(SEXP reorder_methodSEXP, SEXP bdd_nameSEXP) {
BEGIN_RCPP
    Rcpp::RNGScope rcpp_rngScope_gen;
    Rcpp::traits::input_parameter< std::string >::type reorder_method(reorder_methodSEXP);
    Rcpp::traits::input_parameter< std::string >::type bdd_name(bdd_nameSEXP);
    bdd_order(reorder_method, bdd_name);
    return R_NilValue;
END_RCPP
}
// bdd_write
void bdd_write(std::string file_name, std::string bdd_name);
RcppExport SEXP _rbdd_bdd_write(SEXP file_nameSEXP, SEXP bdd_nameSEXP) {
BEGIN_RCPP
    Rcpp::RNGScope rcpp_rngScope_gen;
    Rcpp::traits::input_parameter< std::string >::type file_name(file_nameSEXP);
    Rcpp::traits::input_parameter< std::string >::type bdd_name(bdd_nameSEXP);
    bdd_write(file_name, bdd_name);
    return R_NilValue;
END_RCPP
}
// bdd_read
void bdd_read(std::string file_name, std::string bdd_name);
RcppExport SEXP _rbdd_bdd_read(SEXP file_nameSEXP, SEXP bdd_nameSEXP) {
BEGIN_RCPP
    Rcpp::RNGScope rcpp_rngScope_gen;
    Rcpp::traits::input_parameter< std::string >::type file_name(file_nameSEXP);
    Rcpp::traits::input_parameter< std::string >::type bdd_name(bdd_nameSEXP);
    bdd_read(file_name, bdd_name);
    return R_NilValue;
END_RCPP
}
// bdd_identical
bool bdd_identical(std::string name_bdd_1, std::string name_bdd_2);
RcppExport SEXP _rbdd_bdd_identical(SEXP name_bdd_1SEXP, SEXP name_bdd_2SEXP) {
BEGIN_RCPP
    Rcpp::RObject rcpp_result_gen;
    Rcpp::RNGScope rcpp_rngScope_gen;
    Rcpp::traits::input_parameter< std::string >::type name_bdd_1(name_bdd_1SEXP);
    Rcpp::traits::input_parameter< std::string >::type name_bdd_2(name_bdd_2SEXP);
    rcpp_result_gen = Rcpp::wrap(bdd_identical(name_bdd_1, name_bdd_2));
    return rcpp_result_gen;
END_RCPP
}
// bdd_restrict
bool bdd_restrict(std::string restriction, std::string restriction_name, int expression, bool positive_form, std::string bdd_name);
RcppExport SEXP _rbdd_bdd_restrict(SEXP restrictionSEXP, SEXP restriction_nameSEXP, SEXP expressionSEXP, SEXP positive_formSEXP, SEXP bdd_nameSEXP) {
BEGIN_RCPP
    Rcpp::RObject rcpp_result_gen;
    Rcpp::RNGScope rcpp_rngScope_gen;
    Rcpp::traits::input_parameter< std::string >::type restriction(restrictionSEXP);
    Rcpp::traits::input_parameter< std::string >::type restriction_name(restriction_nameSEXP);
    Rcpp::traits::input_parameter< int >::type expression(expressionSEXP);
    Rcpp::traits::input_parameter< bool >::type positive_form(positive_formSEXP);
    Rcpp::traits::input_parameter< std::string >::type bdd_name(bdd_nameSEXP);
    rcpp_result_gen = Rcpp::wrap(bdd_restrict(restriction, restriction_name, expression, positive_form, bdd_name));
    return rcpp_result_gen;
END_RCPP
}
// bdd_traverse
Rcpp::List bdd_traverse(Rcpp::List value_zero, Rcpp::List value_one, std::string function_to_apply, bool return_node, int num_threads, std::string bdd_name, bool trace);
RcppExport SEXP _rbdd_bdd_traverse(SEXP value_zeroSEXP, SEXP value_oneSEXP, SEXP function_to_applySEXP, SEXP return_nodeSEXP, SEXP num_threadsSEXP, SEXP bdd_nameSEXP, SEXP traceSEXP) {
BEGIN_RCPP
    Rcpp::RObject rcpp_result_gen;
    Rcpp::RNGScope rcpp_rngScope_gen;
    Rcpp::traits::input_parameter< Rcpp::List >::type value_zero(value_zeroSEXP);
    Rcpp::traits::input_parameter< Rcpp::List >::type value_one(value_oneSEXP);
    Rcpp::traits::input_parameter< std::string >::type function_to_apply(function_to_applySEXP);
    Rcpp::traits::input_parameter< bool >::type return_node(return_nodeSEXP);
    Rcpp::traits::input_parameter< int >::type num_threads(num_threadsSEXP);
    Rcpp::traits::input_parameter< std::string >::type bdd_name(bdd_nameSEXP);
    Rcpp::traits::input_parameter< bool >::type trace(traceSEXP);
    rcpp_result_gen = Rcpp::wrap(bdd_traverse(value_zero, value_one, function_to_apply, return_node, num_threads, bdd_name, trace));
    return rcpp_result_gen;
END_RCPP
}
// bdd_traverse_root_node
int bdd_traverse_root_node(std::string bdd_name);
RcppExport SEXP _rbdd_bdd_traverse_root_node(SEXP bdd_nameSEXP) {
BEGIN_RCPP
    Rcpp::RObject rcpp_result_gen;
    Rcpp::RNGScope rcpp_rngScope_gen;
    Rcpp::traits::input_parameter< std::string >::type bdd_name(bdd_nameSEXP);
    rcpp_result_gen = Rcpp::wrap(bdd_traverse_root_node(bdd_name));
    return rcpp_result_gen;
END_RCPP
}
// bdd_traverse_is_node_zero
bool bdd_traverse_is_node_zero(int node, std::string bdd_name);
RcppExport SEXP _rbdd_bdd_traverse_is_node_zero(SEXP nodeSEXP, SEXP bdd_nameSEXP) {
BEGIN_RCPP
    Rcpp::RObject rcpp_result_gen;
    Rcpp::RNGScope rcpp_rngScope_gen;
    Rcpp::traits::input_parameter< int >::type node(nodeSEXP);
    Rcpp::traits::input_parameter< std::string >::type bdd_name(bdd_nameSEXP);
    rcpp_result_gen = Rcpp::wrap(bdd_traverse_is_node_zero(node, bdd_name));
    return rcpp_result_gen;
END_RCPP
}
// bdd_traverse_is_node_one
bool bdd_traverse_is_node_one(int node, std::string bdd_name);
RcppExport SEXP _rbdd_bdd_traverse_is_node_one(SEXP nodeSEXP, SEXP bdd_nameSEXP) {
BEGIN_RCPP
    Rcpp::RObject rcpp_result_gen;
    Rcpp::RNGScope rcpp_rngScope_gen;
    Rcpp::traits::input_parameter< int >::type node(nodeSEXP);
    Rcpp::traits::input_parameter< std::string >::type bdd_name(bdd_nameSEXP);
    rcpp_result_gen = Rcpp::wrap(bdd_traverse_is_node_one(node, bdd_name));
    return rcpp_result_gen;
END_RCPP
}
// bdd_traverse_get_level
int bdd_traverse_get_level(int node, std::string bdd_name);
RcppExport SEXP _rbdd_bdd_traverse_get_level(SEXP nodeSEXP, SEXP bdd_nameSEXP) {
BEGIN_RCPP
    Rcpp::RObject rcpp_result_gen;
    Rcpp::RNGScope rcpp_rngScope_gen;
    Rcpp::traits::input_parameter< int >::type node(nodeSEXP);
    Rcpp::traits::input_parameter< std::string >::type bdd_name(bdd_nameSEXP);
    rcpp_result_gen = Rcpp::wrap(bdd_traverse_get_level(node, bdd_name));
    return rcpp_result_gen;
END_RCPP
}
// bdd_traverse_get_var_at_pos
int bdd_traverse_get_var_at_pos(int pos, std::string bdd_name);
RcppExport SEXP _rbdd_bdd_traverse_get_var_at_pos(SEXP posSEXP, SEXP bdd_nameSEXP) {
BEGIN_RCPP
    Rcpp::RObject rcpp_result_gen;
    Rcpp::RNGScope rcpp_rngScope_gen;
    Rcpp::traits::input_parameter< int >::type pos(posSEXP);
    Rcpp::traits::input_parameter< std::string >::type bdd_name(bdd_nameSEXP);
    rcpp_result_gen = Rcpp::wrap(bdd_traverse_get_var_at_pos(pos, bdd_name));
    return rcpp_result_gen;
END_RCPP
}
// bdd_traverse_get_children
Rcpp::NumericVector bdd_traverse_get_children(int node, std::string bdd_name);
RcppExport SEXP _rbdd_bdd_traverse_get_children(SEXP nodeSEXP, SEXP bdd_nameSEXP) {
BEGIN_RCPP
    Rcpp::RObject rcpp_result_gen;
    Rcpp::RNGScope rcpp_rngScope_gen;
    Rcpp::traits::input_parameter< int >::type node(nodeSEXP);
    Rcpp::traits::input_parameter< std::string >::type bdd_name(bdd_nameSEXP);
    rcpp_result_gen = Rcpp::wrap(bdd_traverse_get_children(node, bdd_name));
    return rcpp_result_gen;
END_RCPP
}
// bdd_info_variables
std::vector<std::string> bdd_info_variables(bool xverbose, std::string bdd_name);
RcppExport SEXP _rbdd_bdd_info_variables(SEXP xverboseSEXP, SEXP bdd_nameSEXP) {
BEGIN_RCPP
    Rcpp::RObject rcpp_result_gen;
    Rcpp::RNGScope rcpp_rngScope_gen;
    Rcpp::traits::input_parameter< bool >::type xverbose(xverboseSEXP);
    Rcpp::traits::input_parameter< std::string >::type bdd_name(bdd_nameSEXP);
    rcpp_result_gen = Rcpp::wrap(bdd_info_variables(xverbose, bdd_name));
    return rcpp_result_gen;
END_RCPP
}
// bdd_info_variable_number
int bdd_info_variable_number(std::string bdd_name);
RcppExport SEXP _rbdd_bdd_info_variable_number(SEXP bdd_nameSEXP) {
BEGIN_RCPP
    Rcpp::RObject rcpp_result_gen;
    Rcpp::RNGScope rcpp_rngScope_gen;
    Rcpp::traits::input_parameter< std::string >::type bdd_name(bdd_nameSEXP);
    rcpp_result_gen = Rcpp::wrap(bdd_info_variable_number(bdd_name));
    return rcpp_result_gen;
END_RCPP
}
// bdd_info_boolstr
void bdd_info_boolstr(int expression, std::string bdd_name);
RcppExport SEXP _rbdd_bdd_info_boolstr(SEXP expressionSEXP, SEXP bdd_nameSEXP) {
BEGIN_RCPP
    Rcpp::RNGScope rcpp_rngScope_gen;
    Rcpp::traits::input_parameter< int >::type expression(expressionSEXP);
    Rcpp::traits::input_parameter< std::string >::type bdd_name(bdd_nameSEXP);
    bdd_info_boolstr(expression, bdd_name);
    return R_NilValue;
END_RCPP
}
// bdd_info_node_number
int bdd_info_node_number(std::string bdd_name);
RcppExport SEXP _rbdd_bdd_info_node_number(SEXP bdd_nameSEXP) {
BEGIN_RCPP
    Rcpp::RObject rcpp_result_gen;
    Rcpp::RNGScope rcpp_rngScope_gen;
    Rcpp::traits::input_parameter< std::string >::type bdd_name(bdd_nameSEXP);
    rcpp_result_gen = Rcpp::wrap(bdd_info_node_number(bdd_name));
    return rcpp_result_gen;
END_RCPP
}
// bdd_manager_is_initialized
bool bdd_manager_is_initialized(std::string bdd_name);
RcppExport SEXP _rbdd_bdd_manager_is_initialized(SEXP bdd_nameSEXP) {
BEGIN_RCPP
    Rcpp::RObject rcpp_result_gen;
    Rcpp::RNGScope rcpp_rngScope_gen;
    Rcpp::traits::input_parameter< std::string >::type bdd_name(bdd_nameSEXP);
    rcpp_result_gen = Rcpp::wrap(bdd_manager_is_initialized(bdd_name));
    return rcpp_result_gen;
END_RCPP
}
// bdd_info_manager_library
void bdd_info_manager_library(std::string bdd_name);
RcppExport SEXP _rbdd_bdd_info_manager_library(SEXP bdd_nameSEXP) {
BEGIN_RCPP
    Rcpp::RNGScope rcpp_rngScope_gen;
    Rcpp::traits::input_parameter< std::string >::type bdd_name(bdd_nameSEXP);
    bdd_info_manager_library(bdd_name);
    return R_NilValue;
END_RCPP
}
// bdd_print
void bdd_print(std::string bdd_name);
RcppExport SEXP _rbdd_bdd_print(SEXP bdd_nameSEXP) {
BEGIN_RCPP
    Rcpp::RNGScope rcpp_rngScope_gen;
    Rcpp::traits::input_parameter< std::string >::type bdd_name(bdd_nameSEXP);
    bdd_print(bdd_name);
    return R_NilValue;
END_RCPP
}
// bdd_set_cache_ratio
void bdd_set_cache_ratio(double cache_ratio, std::string bdd_name);
RcppExport SEXP _rbdd_bdd_set_cache_ratio(SEXP cache_ratioSEXP, SEXP bdd_nameSEXP) {
BEGIN_RCPP
    Rcpp::RNGScope rcpp_rngScope_gen;
    Rcpp::traits::input_parameter< double >::type cache_ratio(cache_ratioSEXP);
    Rcpp::traits::input_parameter< std::string >::type bdd_name(bdd_nameSEXP);
    bdd_set_cache_ratio(cache_ratio, bdd_name);
    return R_NilValue;
END_RCPP
}
// bdd_set_max_node_num
void bdd_set_max_node_num(int size, std::string bdd_name);
RcppExport SEXP _rbdd_bdd_set_max_node_num(SEXP sizeSEXP, SEXP bdd_nameSEXP) {
BEGIN_RCPP
    Rcpp::RNGScope rcpp_rngScope_gen;
    Rcpp::traits::input_parameter< int >::type size(sizeSEXP);
    Rcpp::traits::input_parameter< std::string >::type bdd_name(bdd_nameSEXP);
    bdd_set_max_node_num(size, bdd_name);
    return R_NilValue;
END_RCPP
}
// bdd_get_var_probabilities
StringVector bdd_get_var_probabilities(int num_threads, bool trace, bool fast, std::string bdd_name, std::string restriction);
RcppExport SEXP _rbdd_bdd_get_var_probabilities(SEXP num_threadsSEXP, SEXP traceSEXP, SEXP fastSEXP, SEXP bdd_nameSEXP, SEXP restrictionSEXP) {
BEGIN_RCPP
    Rcpp::RObject rcpp_result_gen;
    Rcpp::RNGScope rcpp_rngScope_gen;
    Rcpp::traits::input_parameter< int >::type num_threads(num_threadsSEXP);
    Rcpp::traits::input_parameter< bool >::type trace(traceSEXP);
    Rcpp::traits::input_parameter< bool >::type fast(fastSEXP);
    Rcpp::traits::input_parameter< std::string >::type bdd_name(bdd_nameSEXP);
    Rcpp::traits::input_parameter< std::string >::type restriction(restrictionSEXP);
    rcpp_result_gen = Rcpp::wrap(bdd_get_var_probabilities(num_threads, trace, fast, bdd_name, restriction));
    return rcpp_result_gen;
END_RCPP
}
// bdd_get_sat_distribution
StringVector bdd_get_sat_distribution(int num_threads, bool trace, bool fast, std::string bdd_name, std::string restriction);
RcppExport SEXP _rbdd_bdd_get_sat_distribution(SEXP num_threadsSEXP, SEXP traceSEXP, SEXP fastSEXP, SEXP bdd_nameSEXP, SEXP restrictionSEXP) {
BEGIN_RCPP
    Rcpp::RObject rcpp_result_gen;
    Rcpp::RNGScope rcpp_rngScope_gen;
    Rcpp::traits::input_parameter< int >::type num_threads(num_threadsSEXP);
    Rcpp::traits::input_parameter< bool >::type trace(traceSEXP);
    Rcpp::traits::input_parameter< bool >::type fast(fastSEXP);
    Rcpp::traits::input_parameter< std::string >::type bdd_name(bdd_nameSEXP);
    Rcpp::traits::input_parameter< std::string >::type restriction(restrictionSEXP);
    rcpp_result_gen = Rcpp::wrap(bdd_get_sat_distribution(num_threads, trace, fast, bdd_name, restriction));
    return rcpp_result_gen;
END_RCPP
}
// bdd_get_core_dead
List bdd_get_core_dead(int num_threads, bool trace, bool fast, std::string bdd_name);
RcppExport SEXP _rbdd_bdd_get_core_dead(SEXP num_threadsSEXP, SEXP traceSEXP, SEXP fastSEXP, SEXP bdd_nameSEXP) {
BEGIN_RCPP
    Rcpp::RObject rcpp_result_gen;
    Rcpp::RNGScope rcpp_rngScope_gen;
    Rcpp::traits::input_parameter< int >::type num_threads(num_threadsSEXP);
    Rcpp::traits::input_parameter< bool >::type trace(traceSEXP);
    Rcpp::traits::input_parameter< bool >::type fast(fastSEXP);
    Rcpp::traits::input_parameter< std::string >::type bdd_name(bdd_nameSEXP);
    rcpp_result_gen = Rcpp::wrap(bdd_get_core_dead(num_threads, trace, fast, bdd_name));
    return rcpp_result_gen;
END_RCPP
}
// bdd_get_uniform_random_sampling
List bdd_get_uniform_random_sampling(bool name, int num_threads, bool trace, bool fast, std::string bdd_name);
RcppExport SEXP _rbdd_bdd_get_uniform_random_sampling(SEXP nameSEXP, SEXP num_threadsSEXP, SEXP traceSEXP, SEXP fastSEXP, SEXP bdd_nameSEXP) {
BEGIN_RCPP
    Rcpp::RObject rcpp_result_gen;
    Rcpp::RNGScope rcpp_rngScope_gen;
    Rcpp::traits::input_parameter< bool >::type name(nameSEXP);
    Rcpp::traits::input_parameter< int >::type num_threads(num_threadsSEXP);
    Rcpp::traits::input_parameter< bool >::type trace(traceSEXP);
    Rcpp::traits::input_parameter< bool >::type fast(fastSEXP);
    Rcpp::traits::input_parameter< std::string >::type bdd_name(bdd_nameSEXP);
    rcpp_result_gen = Rcpp::wrap(bdd_get_uniform_random_sampling(name, num_threads, trace, fast, bdd_name));
    return rcpp_result_gen;
END_RCPP
}

static const R_CallMethodDef CallEntries[] = {
    {"_rbdd_bdd_manager_init", (DL_FUNC) &_rbdd_bdd_manager_init, 4},
    {"_rbdd_bdd_manager_quit", (DL_FUNC) &_rbdd_bdd_manager_quit, 1},
    {"_rbdd_bdd_manager_reset", (DL_FUNC) &_rbdd_bdd_manager_reset, 1},
    {"_rbdd_bdd_new_variable", (DL_FUNC) &_rbdd_bdd_new_variable, 3},
    {"_rbdd_bdd_parse_cnf", (DL_FUNC) &_rbdd_bdd_parse_cnf, 8},
    {"_rbdd_bdd_parse_splot", (DL_FUNC) &_rbdd_bdd_parse_splot, 8},
    {"_rbdd_bdd_parse_boolstr", (DL_FUNC) &_rbdd_bdd_parse_boolstr, 2},
    {"_rbdd_bdd_order", (DL_FUNC) &_rbdd_bdd_order, 2},
    {"_rbdd_bdd_write", (DL_FUNC) &_rbdd_bdd_write, 2},
    {"_rbdd_bdd_read", (DL_FUNC) &_rbdd_bdd_read, 2},
    {"_rbdd_bdd_identical", (DL_FUNC) &_rbdd_bdd_identical, 2},
    {"_rbdd_bdd_restrict", (DL_FUNC) &_rbdd_bdd_restrict, 5},
    {"_rbdd_bdd_traverse", (DL_FUNC) &_rbdd_bdd_traverse, 7},
    {"_rbdd_bdd_traverse_root_node", (DL_FUNC) &_rbdd_bdd_traverse_root_node, 1},
    {"_rbdd_bdd_traverse_is_node_zero", (DL_FUNC) &_rbdd_bdd_traverse_is_node_zero, 2},
    {"_rbdd_bdd_traverse_is_node_one", (DL_FUNC) &_rbdd_bdd_traverse_is_node_one, 2},
    {"_rbdd_bdd_traverse_get_level", (DL_FUNC) &_rbdd_bdd_traverse_get_level, 2},
    {"_rbdd_bdd_traverse_get_var_at_pos", (DL_FUNC) &_rbdd_bdd_traverse_get_var_at_pos, 2},
    {"_rbdd_bdd_traverse_get_children", (DL_FUNC) &_rbdd_bdd_traverse_get_children, 2},
    {"_rbdd_bdd_info_variables", (DL_FUNC) &_rbdd_bdd_info_variables, 2},
    {"_rbdd_bdd_info_variable_number", (DL_FUNC) &_rbdd_bdd_info_variable_number, 1},
    {"_rbdd_bdd_info_boolstr", (DL_FUNC) &_rbdd_bdd_info_boolstr, 2},
    {"_rbdd_bdd_info_node_number", (DL_FUNC) &_rbdd_bdd_info_node_number, 1},
    {"_rbdd_bdd_manager_is_initialized", (DL_FUNC) &_rbdd_bdd_manager_is_initialized, 1},
    {"_rbdd_bdd_info_manager_library", (DL_FUNC) &_rbdd_bdd_info_manager_library, 1},
    {"_rbdd_bdd_print", (DL_FUNC) &_rbdd_bdd_print, 1},
    {"_rbdd_bdd_set_cache_ratio", (DL_FUNC) &_rbdd_bdd_set_cache_ratio, 2},
    {"_rbdd_bdd_set_max_node_num", (DL_FUNC) &_rbdd_bdd_set_max_node_num, 2},
    {"_rbdd_bdd_get_var_probabilities", (DL_FUNC) &_rbdd_bdd_get_var_probabilities, 5},
    {"_rbdd_bdd_get_sat_distribution", (DL_FUNC) &_rbdd_bdd_get_sat_distribution, 5},
    {"_rbdd_bdd_get_core_dead", (DL_FUNC) &_rbdd_bdd_get_core_dead, 4},
    {"_rbdd_bdd_get_uniform_random_sampling", (DL_FUNC) &_rbdd_bdd_get_uniform_random_sampling, 5},
    {NULL, NULL, 0}
};

RcppExport void R_init_rbdd(DllInfo *dll) {
    R_registerRoutines(dll, NULL, CallEntries, NULL, NULL);
    R_useDynamicSymbols(dll, FALSE);
}
