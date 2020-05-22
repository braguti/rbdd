\name{bdd_read}
\alias{bdd_read}
\docType{package}
\title{
READS A BDD FROM A FILE
}
\description{
Instruction to read a BDD from a file. If a name of BDD is provided, the content of the file will be load on a BDD with that name.
}
\usage{
bdd_read(file_name, bdd_name)
}
\arguments{
\item{file_name}{The name of the input file. The file must end in ".buddy" to store a BuDDy BDD or in ".blif" to store a CUDD BDD.}
\item{bdd_name}{(Optional) Name of the BDD.}
}
\value{
N/A
}
\examples{
\dontrun{bdd_read("buddyBDD.buddy")}
\dontrun{bdd_read("cuddBDD.dddmp")}
\dontrun{bdd_read("buddyBDD.buddy", "bdd_1")}

\dontrun{bdd_read("cuddBdd.dddmp", "bdd1")}
\dontrun{bdd_manager_quit("bdd1")}
}
