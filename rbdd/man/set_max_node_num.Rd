\name{set_max_node_num}
\alias{set_max_node_num}
\docType{package}
\title{
SETS THE MAXIMUM NUMBER OF NODES OF THE BDD
}
\description{
With this command the user can modify the maximum number of nodes of the created BDD.
}
\usage{
set_max_node_num(int size, string bdd_name)
}
\arguments{
\item{size}{The maximum number of nodes to set to the BDD factory, meaning the number of nodes that can be allocated in the structure.}
\item{bdd_name}{(Optional) Name of the BDD.}
}
\value{
N/A
}
\examples{
set_max_node_num(100)
set_max_node_num(100, "bdd_1")
}
