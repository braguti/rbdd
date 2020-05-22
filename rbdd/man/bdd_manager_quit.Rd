\name{bdd_manager_quit}
\alias{bdd_manager_quit}
\docType{package}
\title{
ENDS THE BDD
}
\description{
This command finishes a BDD, liberating the memory space that it was using.
}
\usage{
bdd_manager_quit(bdd_name)
}
\arguments{
\item{bdd_name}{(Optional) Name of the BDD.}
}
\value{
N/A
}
\examples{
\dontrun{bdd_manager_quit()}
\dontrun{bdd_manager_quit("bdd_1")}

bdd_manager_init(bdd_name="bdd1")
bdd_manager_quit("bdd1")
}
