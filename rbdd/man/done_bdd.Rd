\name{done_bdd}
\alias{done_bdd}
\docType{package}
\title{
ENDS THE BDD
}
\description{
This command finishes a BDD, liberating the memory space that it was using.
}
\usage{
done_bdd(string bdd_name)
}
\arguments{
\item{bdd_name}{(Optional) Name of the BDD.}
}
\value{
N/A
}
\examples{
done_bdd()
done_bdd("bdd_1")
}
