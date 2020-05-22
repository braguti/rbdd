\name{bdd_info_variable_number}
\alias{bdd_info_variable_number}
\docType{package}
\title{
GETS THE NUMBER OF VARIABLES OF THE BDD
}
\description{
Gets the number of variables.
}
\usage{
bdd_info_variable_number(bdd_name)
}
\arguments{
\item{bdd_name}{(Optional) Name of the BDD.}
}
\value{
\item{variable_num}{Number of variable of the BDD.}
}
\examples{
\dontrun{bdd_info_variable_number()}
\dontrun{bdd_info_variable_number("bdd_1")}

bdd_manager_init(bdd_name="bdd1")
bdd_parse_boolstr("x and y", "bdd1")

bdd_info_variable_number("bdd1")
bdd_manager_quit("bdd1")
}
