\name{bdd_write}
\alias{bdd_write}
\docType{package}
\title{
SAVES A BDD INTO A FILE
}
\description{
Instruction to save a BDD to a file. If BuDDy is chosen as BDD manager, the output extension is ".buddy". If CUDD is the manager, the extension will be ".blif".

The file is saved in the current R's working directory.
}
\usage{
bdd_write(file_name, bdd_name)
}
\arguments{
\item{file_name}{The name of the output file.}
\item{bdd_name}{(Optional) Name of the BDD.}
}
\value{
N/A
}
\examples{
\dontrun{bdd_write("buddyExecution")}
\dontrun{bdd_write("buddyExecution", "bdd_1")}

bdd_manager_init(bdd_name="bdd1")
bdd_parse_boolstr("x and y", "bdd1")
bdd_write("test", "bdd1")
bdd_manager_quit(bdd_name="bdd1")
}
