\name{bdd_get_core_dead}
\alias{bdd_get_core_dead}
\docType{package}
\title{
GETS THE CORE AND DEAD FEATURES OF A BDD
}
\description{
Function which applies the core and dead algorithm across the BDD following the traverse algorithm.
}
\usage{
bdd_get_core_dead(num_threads, trace, fast, bdd_name, restriction)
}
\arguments{
\item{num_threads}{(Optional) The number of threads to execute the method on multi-threading mode. The default value is 1.}
\item{trace}{(Optional) If it is true, it prints the results of intermediate steps of the traverse algorithm. The default value is FALSE.}
\item{fast}{(Optional) Flag that indicates if the algorithm must be run in fast mode. The default value is FALSE.}
\item{bdd_name}{(Optional) Name of the BDD.}
}
\value{
The list with the core and dead features of the BDD
}
\examples{
\dontrun{bdd_get_core_dead()}
\dontrun{bdd_get_core_dead(1)}
\dontrun{bdd_get_core_dead(1, TRUE)}
\dontrun{bdd_get_core_dead(1, TRUE, TRUE)}
}
