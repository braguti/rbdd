\name{traverse_bdd}
\alias{traverse_bdd}
\docType{package}
\title{
EXECUTE THE TRAVERSE ALGORITHM TO A FUNCTION
}
\description{
Function which executes a R function across the BDD following the traverse algorithm.
}
\usage{
traverse_bdd(vector vector_zero, vector vector_one, string function_to_apply, int num_threads, string bdd_name)
}
\arguments{
\item{vector_zero}{The value(s) of the negative path.}
\item{vector_one}{The value(s) of the positive path.}
\item{function_to_apply}{Name of the function to apply to the BDD.}
\item{num_threads}{(Optional) The number of threads to execute the method on multi-threading mode. The default value is 1.}
\item{bdd_name}{(Optional) Name of the BDD.}
}
\value{
The result of applying the traverse algorithm to the BDD
}
\examples{
traverse_bdd(0, 1, myRFunction)
traverse_bdd(c(0), c(1), MyRFunction, 2)
traverse_bdd(c(0, 5, 4), c(1, 0, 9), MyRFunction)
traverse_bdd(c(0, 4, 5, 1, 12), c(2, 5, 0, 3, 1), MyRFunction, 4, "my_bdd")
}
