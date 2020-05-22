# Load the library
library(rbdd)

# Create the BDDfactory
bdd_manager_init()

# Create the BDD
bdd_parse_cnf("examples/cnfExample.cnf")

# Get the number of nodes
cat("The number of nodes is ")
cat(bdd_info_node_number())
cat("\n")

bdd_manager_quit()

