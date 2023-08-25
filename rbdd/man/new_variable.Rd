\name{new_variable}
\alias{new_variable}
\docType{package}
\title{
CREATES A VARIABLE
}
\description{
This command creates a new variable to be used for the BDD factory.
}
\usage{
new_variable(string variable_name, string var_type, string bdd_name)
}
\arguments{
\item{variable_name}{The name of the variable. It can only contain letters and numbers.}
\item{var_type}{(Optional) Type of the variable. The possible values are "boolean" and "tristate". The default value is "boolean".}
\item{bdd_name}{(Optional) Name of the BDD.}
}
\value{
\item{index_var}{Index of the variable created. It returns -1 in case of error.}
}
\examples{
new_variable("x")
new_variable("x1", "boolean")
new_variable("x2", "tristate")
new_variable("x1", "boolean", "bdd_1")
}
