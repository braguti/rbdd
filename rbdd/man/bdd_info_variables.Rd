\name{bdd_info_variables}
\alias{bdd_info_variables}
\docType{package}
\title{
PRINTS AND RETURNS THE VARIABLES ASSOCIATED TO THE BDD
}
\description{
This function prints a table showing the index and the content of the variables created and returns a list with the variable list.
}
\usage{
bdd_info_variables(xverbose, bdd_name)
}
\arguments{
\item{xverbose}{(Optional) Flag to print the variables in the terminal. The default value is TRUE.}
\item{bdd_name}{(Optional) Name of the BDD.}
}
\value{
\item{variable_list}{List containing the variables of the BDD.}
}
\examples{
\dontrun{bdd_info_variables()}
\dontrun{bdd_info_variables("bdd_1")}
\dontrun{bdd_info_variables(FALSE, "bdd_1")}

bdd_manager_init(bdd_name="bdd1")
bdd_new_variable("x", "bdd1")
bdd_new_variable("y", "bdd1")
bdd_info_variables(TRUE, "bdd1")
bdd_manager_quit("bdd1")
}
