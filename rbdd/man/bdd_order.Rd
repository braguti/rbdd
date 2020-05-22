\name{bdd_order}
\alias{bdd_order}
\docType{package}
\title{
REORDERS THE BDD WITH THE GIVEN ORDER
}
\description{
This instruction allows to reorder the BDD depending on the method specified on the input parameter (if it is informed). The possible methods are:

	- "none"

	- "window2"

	- "window3"

	- "sift"

	- "random"

To execute the instruction, the BDD must have variables assigned.
}
\usage{
bdd_order(reorder_method, bdd_name)
}
\arguments{
\item{reorder_method}{(Optional) The method for reordering the BDD. The default value is "sift".}
\item{bdd_name}{(Optional) Name of the BDD.}
}
\value{
N/A
}
\examples{
\dontrun{bdd_order()}
\dontrun{bdd_order("window2")}
\dontrun{bdd_order("window2", "bdd_1")}

bdd_manager_init(bdd_name="bdd1")
bdd_parse_boolstr("x or y", "bdd1")
bdd_order("window2", "bdd1")
bdd_manager_quit("bdd1")
}
