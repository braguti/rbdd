\name{bdd_info_node_number}
\alias{bdd_info_node_number}
\docType{package}
\title{
GETS THE NUMBER OF NODES IN USE OF THE BDD
}
\description{
Gets the number of active nodes in use.
}
\usage{
bdd_info_node_number(bdd_name)
}
\arguments{
\item{bdd_name}{(Optional) Name of the BDD.}
}
\value{
\item{node_num}{Number of active nodes in use.}
}
\examples{
\dontrun{bdd_info_node_number()}
\dontrun{bdd_info_node_number("bdd_1")}

bdd_manager_init(bdd_name="bdd1")
bdd_parse_boolstr("x and y", "bdd1")

bdd_info_node_number("bdd1")
bdd_manager_quit("bdd1")
}
