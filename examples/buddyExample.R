# Load the library
library(rbdd)

# Create the BDDfactory
bdd_manager_init("buddy")

# Create variables in the factory
bdd_new_variable("x")
bdd_new_variable("y")
bdd_new_variable("z")

# Create the expression
bdd_parse_cnf("examples/cnfExample.cnf")

# Get the number of nodes
cat("The number of nodes is ")
cat(bdd_info_node_number())
cat("\n")

# Save the BDD in a file
bdd_write("executionBuddy")

#bdd_manager_quit()

