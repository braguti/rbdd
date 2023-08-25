\name{is_initialized}
\alias{is_initialized}
\docType{package}
\title{
NDICATES IF THE BDD FACTORY HAS BEEN INITIALIZED
}
\description{
This instruction allows to the user to know if the BDD factory has been initialized.
}
\usage{
is_initialized(string bdd_name)
}
\arguments{
\item{bdd_name}{(Optional) Name of the BDD.}
}
\value{
\item{is_initialized}{It is true if the factory is initialized and false if it is not.}
}
\examples{
is_initialized()
is_initialized("bdd_1")
}
