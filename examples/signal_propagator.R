# Load the library
library(rbdd)

# Create the BDDfactory
bdd_manager_init("buddy")

z = bdd_parse_boolstr("(a or b) and c")

bdd_restrict(z, "b", "restrict1")
bdd_restrict(z, "b", "restrict2", FALSE)

# Create the expression
bdd_parse_boolstr("(restric2 or not (not restrict2 and restrict1)) and (not restric2 or (not restrict2 and restrict1))")

bdd_info_variables()

# Get the number of nodes
cat("The number of nodes is ")
cat(bdd_info_node_number())
cat("\n")

cat("The solved bdd is:\n")
bdd_print()

bdd_manager_quit()
