\name{same_bdd}
\alias{same_bdd}
\docType{package}
\title{
COMPARES TWO BDDS
}
\description{
This function compares two BDDs. The BDDs could be BDDs created with the init_bdd() or the read_bdd() commands, expressions which involve BDDs or in the case of the second expression, the constant BDDs "true" and "false".

The logic operations allowed between BDDs are:

	- ! ("!bdd_1")

	- && ("bdd_1 && bdd_2")

	- || ("bdd_1 || bdd_2")

	- != ("bdd_1 != bdd_2")

	- == ("bdd_1 == bdd_2")

	- < ("bdd_1 < bdd_2")

	- > ("bdd_1 > bdd_2")
}
\usage{
same_bdd(string name_bdd_1, string name_bdd_2)
}
\arguments{
\item{name_bdd_1}{The name of the first BDD.}
\item{name_bdd_2}{The name of the second BDD.}
}
\value{
\item{result}{The result of comparing the BDDs.}
}
\examples{
same_bdd("bdd_1", "bdd_2")
same_bdd("!bdd_1 && bdd_2", "bdd_3")
same_bdd("!bdd_1", "true")
same_bdd("!bdd_1", "false")
}
