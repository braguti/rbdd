\name{bdd_traverse_get_var_at_pos}
\alias{bdd_traverse_get_var_at_pos}
\docType{package}
\title{
GETS VAR IN A POSITION
}
\description{
Function which returns the var which is in the given position.
}
\usage{
bdd_traverse_get_var_at_pos(pos, bdd_name)
}
\arguments{
\item{pos}{Position to be consulted.}
\item{bdd_name}{(Optional) Name of the BDD.}
}
\value{
Var of the position.
}
\examples{
\dontrun{bdd_traverse_get_var_at_pos(0)}
\dontrun{bdd_traverse_get_var_at_pos(0, "my_bdd")}

bdd_manager_init(bdd_name="bdd1")
bdd_parse_boolstr(bdd_name="bdd1", "(mobilePhone = colourScreen) and ((mobilePhone -> calls))")
counter <- function(pindex, tindex, eindex, fthen, felse) {
	thenPart <- 2**(tindex - pindex - 1) * fthen[[1]]
	elsePart <- 2**(eindex - pindex -1) * felse[[1]]
	return (thenPart + elsePart)
}
bdd_traverse(bdd_name="bdd1", 0, 1, "counter", TRUE)
bdd_traverse_get_var_at_pos(0, bdd_name="bdd1")
bdd_manager_quit(bdd_name="bdd1")
}
