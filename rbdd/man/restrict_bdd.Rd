\name{restrict_bdd}
\alias{restrict_bdd}
\docType{package}
\title{
CREATES A VARIABLE FROM AN EXPRESSION RESTRICTING THE VALUE OF A VARIABLE
}
\description{
This command creates a new variable to be used for the BDD factory. It restricts the value of a variable.
}
\usage{
restrict_bdd(int expression, string var_to_restrict, string variable_name, bool positive_form, string bdd_name)
}
\arguments{
\item{expression}{Index of the expression to apply the restriction.}
\item{var_to_restrict}{Name of the variable to restrict in the expression.}
\item{variable_name}{The name of the variable. It can only contain letters and numbers.}
\item{positive_form}{(Optional) Indicates if the value to restrict is in its positive or negative form.}
\item{bdd_name}{(Optional) Name of the BDD.}
}
\value{
\item{index_var}{Index of the variable created. It returns -1 in case of error.}
}
\examples{
restrict_bdd(1, "x", "restrictVariable")
restrict_bdd(2, "y", "restrictVariable", FALSE)
restrict_bdd(2, "y", "restrictVariable", FALSE, "bdd_1")
}
