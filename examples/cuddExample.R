# Load the library
library(rbdd)

# Create the BDDfactory
bdd_manager_init()

# Create variables in the factory
bdd_new_variable("x")
bdd_new_variable("y")
bdd_new_variable("z")

# Create the expression
bdd_parse_boolstr("x and (not x or y) and (not z or not x)")

# Print the variables
bdd_info_variables()

# Get the number of nodes
cat("The number of nodes is ")
cat(bdd_info_node_number())
cat("\n")

# Save the BDD in a file
#bdd_write("executionCudd")

bdd_print()

bdd_manager_quit()

