\name{bdd_traverse}
\alias{bdd_traverse}
\docType{package}
\title{
EXECUTE THE TRAVERSE ALGORITHM TO A FUNCTION
}
\description{
Function which executes a R function across the BDD following the traverse algorithm. The R function must contain 5 input args, being this condition checked (and the existence of function in the R global environment) on execution time.
}
\usage{
bdd_traverse(value_zero, value_one, function_to_apply, return_node, num_threads, bdd_name, trace)
}
\arguments{
\item{value_zero}{The value(s) of the negative path.}
\item{value_one}{The value(s) of the positive path.}
\item{function_to_apply}{Name of the function to apply to the BDD.}
\item{return_name}{(Optional) Indicates if the node pointer is returned. The default value is FALSE.}
\item{num_threads}{(Optional) The number of threads to execute the method on multi-threading mode. The default value is 1.}
\item{bdd_name}{(Optional) Name of the BDD.}
\item{trace}{(Optional) If it is true, it prints the results of intermediate steps of the traverse algorithm. The default value is false.}
}
\value{
The result of applying the traverse algorithm to the BDD
}
\examples{
\dontrun{bdd_traverse(0, 1, myRFunction)}
\dontrun{bdd_traverse(c(0), c(1), myRFunction, 2)}
\dontrun{bdd_traverse(c(0, 5, 4), c(1, 0, 9), myRFunction)}
\dontrun{bdd_traverse(c(0, 4, 5, 1, 12), c(2, 5, 0, 3, 1), myRFunction, 4, "my_bdd")}
\dontrun{bdd_traverse(c(0, 4, 5, 1, 12), c(2, 5, 0, 3, 1), myRFunction, 4, "my_bdd", TRUE)}

bdd_manager_init(bdd_name="bdd1")
bdd_parse_boolstr(bdd_name="bdd1", "(mobilePhone = colourScreen) and ((mobilePhone -> calls))")
counter <- function(pindex, tindex, eindex, fthen, felse) {
	thenPart <- 2**(tindex - pindex - 1) * fthen[[1]]
	elsePart <- 2**(eindex - pindex -1) * felse[[1]]
	return (thenPart + elsePart)
}
bdd_traverse(bdd_name="bdd1", 0, 1, "counter")
bdd_manager_quit(bdd_name="bdd1")
}
