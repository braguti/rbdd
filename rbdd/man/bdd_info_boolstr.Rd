\name{bdd_info_boolstr}
\alias{bdd_info_boolstr}
\docType{package}
\title{
PRINT THE CONTENT OF A VARIABLE
}
\description{
With this command the content of a variable is printed.
}
\usage{
bdd_info_boolstr(expression, bdd_name)
}
\arguments{
\item{expression}{The index of the variable to print.}
\item{bdd_name}{(Optional) Name of the BDD.}
}
\value{
N/A
}
\examples{
\dontrun{bdd_info_boolstr(1)}
\dontrun{bdd_info_boolstr(1, "bdd_1")}

bdd_manager_init(bdd_name="bdd1")
bdd_parse_boolstr("x1 and y1", "bdd1")
bdd_info_boolstr(1, "bdd_1")
bdd_manager_quit("bdd1")
}
