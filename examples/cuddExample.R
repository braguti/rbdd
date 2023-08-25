# Load the library
library(rbdd)

# Create the BDDfactory
init_bdd()

# Create variables in the factory
new_variable("x")
new_variable("y")
new_variable("z")

# Create the expression
myExpression = new_variable_from_expression("x and (not x or y) and (not z or not x)")
# CNF
#myExpression = newVariableFromExpression("1 0 -1 2 0 -3 -1 0")
# CNF file
#myExpression = newVariableFromExpression("cnfExample.cnf")

# Print the expression
expression_to_string(myExpression)

# Print the variables
print_variables()

# Apply function of the factory
apply_bdd(myExpression)

# Get the number of nodes
cat("The number of nodes is ")
cat(get_node_num())
cat("\n")

# Save the BDD in a file
save_bdd("executionCudd")

