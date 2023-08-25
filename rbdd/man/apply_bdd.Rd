\name{apply_bdd}
\alias{apply_bdd}
\docType{package}
\title{
EXECUTES A LOGICAL OPERATION
}
\description{
This function executes a logical operation expressed as a variable and assocaited to the BDD manager through the new_variable() or new_variable_from_expression() instruction.
}
\usage{
apply_bdd(int expression, string bdd_name)
}
\arguments{
\item{expression}{The index of the variable with the expression to execute.}
\item{bdd_name}{(Optional) Name of the BDD.}
}
\value{
N/A
}
\examples{
apply_bdd(1)
apply_bdd(1, "bdd_1")
}
