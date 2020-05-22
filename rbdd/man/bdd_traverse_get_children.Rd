\name{bdd_traverse_get_children}
\alias{bdd_traverse_get_children}
\docType{package}
\title{
GETS THE CHILDREN OF A NODE
}
\description{
Function that returns the children of a node.
}
\usage{
bdd_traverse_get_children(node, bdd_name)
}
\arguments{
\item{node}{Index of the node.}
\item{bdd_name}{(Optional) Name of the BDD.}
}
\value{
Children of a node.
}
\examples{
\dontrun{bdd_traverse_get_children(0)}
\dontrun{bdd_traverse_get_children(0, "my_bdd")}

bdd_manager_init(bdd_name="bdd1")
bdd_parse_boolstr(bdd_name="bdd1", "(mobilePhone = colourScreen) and ((mobilePhone -> calls))")
counter <- function(pindex, tindex, eindex, fthen, felse) {
	thenPart <- 2**(tindex - pindex - 1) * fthen[[1]]
	elsePart <- 2**(eindex - pindex -1) * felse[[1]]
	return (thenPart + elsePart)
}
bdd_traverse(bdd_name="bdd1", 0, 1, "counter", TRUE)
bdd_traverse_get_children(0, bdd_name="bdd1")
bdd_manager_quit(bdd_name="bdd1")
}
