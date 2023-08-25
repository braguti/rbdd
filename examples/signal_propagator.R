# Load the library
library(rbdd)

# Create the BDDfactory
init_bdd()

# Create variables in the factory
new_variable("a")
new_variable("b")
new_variable("c")

z = new_variable_from_expression("(a or b) and c")

restrict_bdd(z, "b", "restrict1")
restrict_bdd(z, "b", "restrict2", FALSE)

# Create the expression
fixed_b = new_variable_from_expression("restrict1 xor restrict2")

print_variables()

# Apply function of the factory
apply_bdd(fixed_b)

# Get the number of nodes
cat("The number of nodes is ")
cat(get_node_num())
cat("\n")

cat("The solved bdd is:\n")
print_bdd()
