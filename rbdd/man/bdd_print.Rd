\name{bdd_print}
\alias{bdd_print}
\docType{package}
\title{
PRINT A SOLVED BDD
}
\description{
This instruction prints the solution of a BDD.
}
\usage{
bdd_print(bdd_name)
}
\arguments{
\item{bdd_name}{(Optional) Name of the BDD.}
}
\value{
N/A.
}
\examples{
\dontrun{bdd_print("bdd_1")}
\dontrun{bdd_print()}

bdd_manager_init(bdd_name="bdd1")
bdd_parse_boolstr("x or y", "bdd1")
bdd_print("bdd1")
bdd_manager_quit("bdd1")
}
