\name{read_bdd}
\alias{read_bdd}
\docType{package}
\title{
READS A BDD FROM A FILE
}
\description{
Instruction to read a BDD from a file. If a name of BDD is provided, the content of the file will be load on a BDD with that name.
}
\usage{
read_bdd(string file_name, string bdd_name)
}
\arguments{
\item{file_name}{The name of the input file. The file must end in ".buddy" to store a BuDDy BDD or in ".blif" to store a CUDD BDD.}
\item{bdd_name}{(Optional) Name of the BDD.}
}
\value{
N/A
}
\examples{
read_bdd("buddyBDD.buddy")
read_bdd("cuddBDD.blif")
read_bdd("buddyBDD.buddy", "bdd_1")
}
