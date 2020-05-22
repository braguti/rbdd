\name{bdd_restrict}
\alias{bdd_restrict}
\docType{package}
\title{
CREATES A VARIABLE FROM AN EXPRESSION RESTRICTING THE VALUE OF A VARIABLE
}
\description{
This command creates a new variable to be used for the BDD factory. It restricts the value of a variable.
}
\usage{
bdd_restrict(restriction, restriction_name, expression, positive_form, bdd_name)
}
\arguments{
\item{restriction}{Name of the variable to restrict in the expression.}
\item{restriction_name}{The name of the variable. It can only contain letters and numbers.}
\item{expression}{Index of the expression to apply the restriction. Only required when the BDD manager is BuDDy.}
\item{positive_form}{(Optional) Indicates if the value to restrict is in its positive or negative form.}
\item{bdd_name}{(Optional) Name of the BDD.}
}
\value{
\item{index_var}{Index of the variable created. It returns -1 in case of error.}
}
\examples{
\dontrun{bdd_restrict("x", "restrict")}

bdd_manager_init(bdd_name="bdd1")
bdd_new_variable("x", "bdd1")
bdd_new_variable("y", "bdd1")
bdd_restrict("x", "restrict1", positive_form=FALSE, bdd_name="bdd1")
bdd_manager_quit("bdd1")
}
