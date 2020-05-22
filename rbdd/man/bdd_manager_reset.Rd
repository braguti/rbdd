\name{bdd_manager_reset}
\alias{bdd_manager_reset}
\docType{package}
\title{
RESTARTS THE BDD FACTORY
}
\description{
It ends the BDD factory and starts it again with the same BDD manager that was chosen in the init_bdd() command and the same parameters.
}
\usage{
bdd_manager_reset(bdd_name)
}
\arguments{
\item{bdd_name}{(Optional) Name of the BDD.}
}
\value{
N/A
}
\examples{
\dontrun{bdd_manager_reset()}
\dontrun{bdd_manager_reset("bdd_1")}

bdd_manager_init(bdd_name="bdd1")
bdd_manager_is_initialized("bdd1")
bdd_manager_reset("bdd1")
bdd_manager_quit("bdd1")
}
