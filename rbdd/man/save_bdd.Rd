\name{save_bdd}
\alias{save_bdd}
\docType{package}
\title{
SAVES A BDD
}
\description{
Instruction to save a BDD to a file. If BuDDy is chosen as BDD manager, the output extension is ".buddy". If CUDD is the manager, the extension will be ".blif".

The file is saved in the current R's working directory.
}
\usage{
save_bdd(string file_name, string bdd_name)
}
\arguments{
\item{file_name}{The name of the output file.}
\item{bdd_name}{(Optional) Name of the BDD.}
}
\value{
N/A
}
\examples{
save_bdd("buddyExecution")
save_bdd("buddyExecution", "bdd_1")
}
