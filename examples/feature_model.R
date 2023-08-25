# Loads the library
library(rbdd)

# Creates the BDDfactory
init_bdd()

# Creates variables in the factory
new_variable("mobilePhone")
new_variable("calls")
new_variable("gps")
new_variable("screen")
new_variable("media")
new_variable("basic")
new_variable("colour")
new_variable("highResolution")
new_variable("camera")
new_variable("mp3")

# Creates a variable from an expression
expression = new_variable_from_expression("(mobilePhone = true) and (mobilePhone iff calls) and (gps -> mobilePhone) and (mobilePhone iff screen) and (media -> mobilePhone) and ((basic iff (not colour and not highResolution and screen)) and (colour iff (not basic and not highResolution and screen)) and (highResolution iff (not basic and not colour and screen))) and (media iff (camera or mp3)) and not(gps and basic) and (camera -> highResolution)")

# Applies the expression in order to be computed
apply_bdd(expression)

# Prints the solution of the BDD
cat("\nThe solution of the feature model is:\n")
print_bdd()

# Frees the space used by the BDD
done_bdd()
