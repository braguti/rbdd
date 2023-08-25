\name{new_variable_from_expression}
\alias{new_variable_from_expression}
\docType{package}
\title{
CREATES A VARIABLE FROM AN EXPRESSION
}
\description{
This instruction is used to create a new variable after evaluating a logical expression. The expression could be introduced explicitly (informing the name of the variables and the logical operations) or using the CNF (Conjunctive Normal Form).
}
\usage{
new_variable_from_expression(string expression, string bdd_name)
}
\arguments{
\item{expression}{The expression to evaluate

If the expression is set from the explicit form, the variables used must exist in the factory, showing an error if some of them do not. It also allows the use of parenthesis "()" to indicate the priority of the operations.

The logical operators implemented are:

	- and ("x and y")
	
	- or ("x or y")

	- not ("not x")

	- xor ("x xor y")

	- nand ("x nand y")

	- nor ("x nor y")

	- xnor ("x xnor y")

	- if then ("if x then y")

	- if then else ("if x then y else z")

	- implies ("x -> y")

	- if and only if <-> ("x iff y")

	- equal ("x = y")

	- true ("x = true")

	- false ("x = false")

With the CNF way, the expression could be informed introducing the name of a file (.cnf) that contains the expression following the syntax rules of that files, or entering the clauses manually, where the variables are informed by their index, that can be consulted with the print_variables() command. It is mandatory to end the expression with a 0.}
\item{bdd_name}{(Optional) Name of the BDD.}
}
\value{
\item{index_var}{Index of the variable created. It returns -1 if case of error.}
}
\examples{
new_variable_from_expression("x and y or (not z and x)")
new_variable_from_expression("1 2 0 -1 3 2 0")
new_variable_from_expression("cnfFile.cnf")
new_variable_from_expression("x and y or (not z and x)", "bdd_1")
}
