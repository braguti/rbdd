\name{bdd_get_uniform_random_sampling}
\alias{bdd_get_uniform_random_sampling}
\docType{package}
\title{
EXECUTES THE UNIFORM RANDOM SAMPLING ALGORITHM TO A BDD
}
\description{
Function which applies the Uniform Random Sampling algorithm across the BDD following the traverse algorithm.
}
\usage{
bdd_get_uniform_random_sampling(num_threads, trace, fast, bdd_name, restriction)
}
\arguments{
\item{name}{(Optional) Falg to indicate if the result must returned as a string or a list of bool. The default value is FALSE.}
\item{num_threads}{(Optional) The number of threads to execute the method on multi-threading mode. The default value is 1.}
\item{trace}{(Optional) If it is true, it prints the results of intermediate steps of the traverse algorithm. The default value is FALSE.}
\item{fast}{(Optional) Flag that indicates if the algorithm must be run in fast mode. The default value is FALSE.}
\item{bdd_name}{(Optional) Name of the BDD.}
}
\value{
The list with the Uniform Random Sampling algorithm aplied to the BDD
}
\examples{
\dontrun{bdd_get_uniform_random_sampling()}
\dontrun{bdd_get_uniform_random_sampling(FALSE)}
\dontrun{bdd_get_uniform_random_sampling(TRUE, 1)}
\dontrun{bdd_get_uniform_random_sampling(TRUE, 1, TRUE)}
\dontrun{bdd_get_uniform_random_sampling(FALSE, 1, TRUE, TRUE)}
}
