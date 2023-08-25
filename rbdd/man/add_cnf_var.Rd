\name{add_cnf_var}
\alias{add_cnf_var}
\docType{package}
\title{
ADDS A CNF VARIABLE
}
\description{
This command adds an intermediate CNF variable that is not the result of the evaluation of the CNF expression.
}
\usage{
add_cnf_var(string name, string bdd_name)
}
\arguments{
\item{name}{The name of the variable.}
\item{bdd_name}{(Optional) Name of the BDD.}
}
\value{
\item{index_var}{Index of the variable created. It returns -1 in case of error.}
}
\examples{
add_cnf_var("1_1")
add_cnf_var("1_1", "bdd_1")
}
