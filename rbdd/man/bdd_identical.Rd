\name{bdd_identical}
\alias{bdd_identical}
\docType{package}
\title{
COMPARES TWO BDDS
}
\description{
This function compares two BDDs. The BDDs could be BDDs created with the bdd_manager_init() and populated with the bdd_read() or the bdd_parse commands, expressions which involve BDDs or in the case of the second expression, the constant BDDs "true" and "false".

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
bdd_identical(name_bdd_1, name_bdd_2)
}
\arguments{
\item{name_bdd_1}{The name of the first BDD.}
\item{name_bdd_2}{The name of the second BDD.}
}
\value{
\item{result}{The result of comparing the BDDs.}
}
\examples{
\dontrun{bdd_identical("bdd_1", "bdd_2")}
\dontrun{bdd_identical("!bdd_1 && bdd_2", "bdd_3")}
\dontrun{bdd_identical("!bdd_1", "true")}
\dontrun{bdd_identical("!bdd_1", "false")}

bdd_manager_init(bdd_name="bdd1")
bdd_parse_boolstr("x1 and y1", "bdd1")
bdd_manager_init(bdd_name="bdd2")
bdd_parse_boolstr("x2 or y2", "bdd2")
bdd_identical("bdd1", "bdd2")
bdd_manager_quit("bdd1")
bdd_manager_quit("bdd2")
}
