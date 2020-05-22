\name{bdd_get_sat_distribution}
\alias{bdd_get_sat_distribution}
\docType{package}
\title{
EXECUTE THE COMPUTE SAT DISTRIBUTION ALGORITHM TO A BDD
}
\description{
Function which applies the compute SAT distribution algorithm across the BDD following the traverse algorithm.
}
\usage{
bdd_get_sat_distribution(num_threads, trace, fast, bdd_name, restriction)
}
\arguments{
\item{num_threads}{(Optional) The number of threads to execute the method on multi-threading mode. The default value is 1.}
\item{trace}{(Optional) If it is true, it prints the results of intermediate steps of the traverse algorithm. The default value is FALSE.}
\item{fast}{(Optional) Flag that indicates if the algorithm must be run in fast mode. The default value is FALSE.}
\item{bdd_name}{(Optional) Name of the BDD.}
\item{restriction}{(Optional) Restriction BDD to apply the algorithm.}
}
\value{
The list with the distribution of each variable of the BDD
}
\examples{
\dontrun{bdd_get_sat_distribution()}
\dontrun{bdd_get_sat_distribution(1)}
\dontrun{bdd_get_sat_distribution(1, TRUE)}
\dontrun{bdd_get_sat_distribution(1, TRUE, TRUE)}
\dontrun{bdd_get_sat_distribution(1, TRUE, TRUE, "my_bdd")}
}
