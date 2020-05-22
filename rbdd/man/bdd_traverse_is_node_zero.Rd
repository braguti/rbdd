\name{bdd_traverse_is_node_zero}
\alias{bdd_traverse_is_node_zero}
\docType{package}
\title{
CHECKS IF THE NODE IS THE ZERO NODE
}
\description{
Function which checks if the given node is the zero node.
}
\usage{
bdd_traverse_is_node_zero(node, bdd_name)
}
\arguments{
\item{node}{Index of the node to check.}
\item{bdd_name}{(Optional) Name of the BDD.}
}
\value{
Result of the comparisson.
}
\examples{
\dontrun{bdd_traverse_is_node_zero(0)}
\dontrun{bdd_traverse_is_node_zero(0, "my_bdd")}

bdd_manager_init(bdd_name="bdd1")
bdd_parse_boolstr(bdd_name="bdd1", "(mobilePhone = colourScreen) and ((mobilePhone -> calls))")
counter <- function(pindex, tindex, eindex, fthen, felse) {
	thenPart <- 2**(tindex - pindex - 1) * fthen[[1]]
	elsePart <- 2**(eindex - pindex -1) * felse[[1]]
	return (thenPart + elsePart)
}
bdd_traverse(bdd_name="bdd1", 0, 1, "counter", TRUE)
bdd_traverse_is_node_zero(0, bdd_name="bdd1")
bdd_manager_quit(bdd_name="bdd1")
}
