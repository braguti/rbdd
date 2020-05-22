\name{bdd_new_variable}
\alias{bdd_new_variable}
\docType{package}
\title{
CREATES A VARIABLE
}
\description{
This command creates a new variable to be used for the BDD manager.
}
\usage{
bdd_new_variable(variable_name, var_type, bdd_name)
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
\dontrun{bdd_new_variable("x")}
\dontrun{bdd_new_variable("x1", "boolean")}
\dontrun{bdd_new_variable("x2", "tristate")}
\dontrun{bdd_new_variable("x1", "boolean", "bdd_1")}

bdd_manager_init(bdd_name="bdd1")
bdd_new_variable("x", "bdd1")
bdd_new_variable("y", "bdd1")
bdd_parse_boolstr("x or y", "bdd1")
bdd_manager_quit("bdd1")
}
