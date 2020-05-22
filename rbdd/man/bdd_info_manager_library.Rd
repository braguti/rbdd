\name{bdd_info_manager_library}
\alias{bdd_info_manager_library}
\docType{package}
\title{
GETS THE BDD MANAGER LIBRARY
}
\description{
This instruction returns the name of the BDD manager chosen.
}
\usage{
bdd_info_manager_library(bdd_name)
}
\arguments{
\item{bdd_name}{(Optional) Name of the BDD.}
}
\value{
N/A.
}
\examples{
\dontrun{bdd_info_manager_library()}
\dontrun{bdd_info_manager_library("bdd_1")}

bdd_manager_init(bdd_name="bdd1")
bdd_info_manager_library("bdd1")
bdd_manager_quit("bdd1")
}
