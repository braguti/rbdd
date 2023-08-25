\name{reorder_bdd}
\alias{reorder_bdd}
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
reorder_bdd(string reorder_method, string bdd_name)
}
\arguments{
\item{reorder_method}{(Optional) The method for reordering the BDD. The default value is "none".}
\item{bdd_name}{(Optional) Name of the BDD.}
}
\value{
N/A
}
\examples{
reorder_bdd()
reorder_bdd("window2")
reorder_bdd("window2", "bdd_1")
}
