\name{bdd_manager_is_initialized}
\alias{bdd_manager_is_initialized}
\docType{package}
\title{
INDICATES IF THE BDD MANAGER HAS BEEN INITIALIZED
}
\description{
This instruction allows to the user to know if the BDD manager has been initialized.
}
\usage{
bdd_manager_is_initialized(bdd_name)
}
\arguments{
\item{bdd_name}{(Optional) Name of the BDD.}
}
\value{
\item{bdd_manager_is_initialized}{It is true if the factory is initialized and false if it is not.}
}
\examples{
\dontrun{bdd_manager_is_initialized()}
\dontrun{bdd_manager_is_initialized("bdd_1")}

bdd_manager_init(bdd_name="bdd1")
bdd_manager_is_initialized("bdd1")
bdd_manager_quit("bdd1")
}
