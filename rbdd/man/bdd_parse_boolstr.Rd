\name{bdd_parse_boolstr}
\alias{bdd_parse_boolstr}
\docType{package}
\title{
CREATES A BDD FROM A LOGIC EXPRESSION
}
\description{
This instruction is used to populates a BDD after evaluating a logical expression.
}
\usage{
bdd_parse_boolstr(expression, bdd_name)
}
\arguments{
\item{expression}{The expression to evaluate

If the variables used does not exist in the factory, the method created them. It also allows the use of parenthesis "()" to indicate the priority of the operations.

The logical operators implemented are:

	- and ("x and y")
	
	- or ("x or y")

	- not ("not x")

	- xor ("xor(x y)")

	- if then ("if x then y")

	- if then else ("if x then y else z")

	- implies ("x -> y")

	- equal ("x = y")}
\item{bdd_name}{(Optional) Name of the BDD.}
}
\value{
\item{result}{It return 0 if it is parsed successfully, or -1 if something went wrong.}
}
\examples{
\dontrun{bdd_parse_boolstr("x and y or (not z and x)")}
\dontrun{bdd_parse_boolstr("x and y or (not z and x)", "bdd_1")}

bdd_manager_init(bdd_name="bdd1")
bdd_parse_boolstr("x or y", "bdd1")
bdd_manager_quit("bdd1")
}
