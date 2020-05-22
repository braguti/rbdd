\name{bdd_traverse_root_node}
\alias{bdd_traverse_root_node}
\docType{package}
\title{
RETURNS THE TRAVERSE NODE OF THE BDD
}
\description{
Function which gets the traverse root node of the BDD.
}
\usage{
bdd_traverse_root_node(bdd_name)
}
\arguments{
\item{bdd_name}{(Optional) Name of the BDD.}
}
\value{
The pointer of the traverse root node.
}
\examples{
\dontrun{bdd_traverse_root_node()}
\dontrun{bdd_traverse_root_node("my_bdd")}

bdd_manager_init(bdd_name="bdd1")
bdd_parse_boolstr(bdd_name="bdd1", "(mobilePhone = colourScreen) and ((mobilePhone -> calls))")
counter <- function(pindex, tindex, eindex, fthen, felse) {
	thenPart <- 2**(tindex - pindex - 1) * fthen[[1]]
	elsePart <- 2**(eindex - pindex -1) * felse[[1]]
	return (thenPart + elsePart)
}
bdd_traverse(bdd_name="bdd1", 0, 1, "counter")
bdd_traverse_root_node(bdd_name="bdd1")
bdd_manager_quit(bdd_name="bdd1")
}
