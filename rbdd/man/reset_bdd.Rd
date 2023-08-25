\name{reset_bdd}
\alias{reset_bdd}
\docType{package}
\title{
RESTARTS THE BDD FACTORY
}
\description{
It ends the BDD factory and starts it again with the same BDD manager that was chosen in the init_bdd() command.
}
\usage{
reset_bdd(string bdd_name)
}
\arguments{
\item{bdd_name}{(Optional) Name of the BDD.}
}
\value{
N/A
}
\examples{
reset_bdd()
reset_bdd("bdd_1")
}
