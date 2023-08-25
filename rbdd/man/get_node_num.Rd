\name{get_node_num}
\alias{get_node_num}
\docType{package}
\title{
GETS THE NUMBER OF NODES IN USE
}
\description{
Gets the number of active nodes in use.
}
\usage{
get_node_num(string bdd_name)
}
\arguments{
\item{bdd_name}{(Optional) Name of the BDD.}
}
\value{
\item{node_num}{Number of active nodes in use.}
}
\examples{
get_node_num()
get_node_num("bdd_1")
}
