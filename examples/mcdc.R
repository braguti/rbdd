# Loads the library
library(rbdd)

# Creates the BDDfactory
init_bdd()

# Creates variables in the factory
new_variable("a")
new_variable("b")
new_variable("c")
new_variable("d")

# Creates a variable from an expression
z = new_variable_from_expression("(a or b) and (c or d)")

# Creates variables restricting the value of the variable a
restrict_bdd(z, "a", "restrict1")
restrict_bdd(z, "a", "restrict2", FALSE)
fixed_a = new_variable_from_expression("restrict1 xor restrict2")

# Applies the final expression in order to be computed
apply_bdd(fixed_a)

# Prints the solution of the BDD
cat("\nCases when variable a is fixed are:\n")
print_bdd()

# Creates variables restricting the value of the variable b
restrict_bdd(z, "b", "restrict3")
restrict_bdd(z, "b", "restrict4", FALSE)
fixed_b = new_variable_from_expression("restrict3 xor restrict4")

# Applies the final expression in order to be computed
apply_bdd(fixed_b)

# Prints the solution of the BDD
cat("\nCases when variable b is fixed are:\n")
print_bdd()

# Creates variables restricting the value of the variable c
restrict_bdd(z, "c", "restrict5")
restrict_bdd(z, "c", "restrict6", FALSE)
fixed_c = new_variable_from_expression("restrict5 xor restrict6")

# Applies the final expression in order to be computed
apply_bdd(fixed_c)

# Prints the solution of the BDD
cat("\nCases when variable c is fixed are:\n")
print_bdd()

# Creates variables restricting the value of the variable d
restrict_bdd(z, "d", "restrict7")
restrict_bdd(z, "d", "restrict8", FALSE)
fixed_d = new_variable_from_expression("restrict7 xor restrict8")

# Applies the final expression in order to be computed
apply_bdd(fixed_d)

# Prints the solution of the BDD
cat("\nCases when variable d is fixed are:\n")
print_bdd()

# Frees the space used by the BDD
done_bdd()
