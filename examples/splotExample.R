# Load the library
library(rbdd)

# Create the BDDfactory
bdd_manager_init()

# Create the expression
#bdd_parse_splot("examples/basic.xml")
bdd_parse_splot("examples/splotmodel.xml")

# Get the number of nodes
cat("The number of nodes is ")
cat(bdd_info_node_number())
cat("\n")

# Save the BDD in a file
#bdd_print()

bdd_manager_quit()

