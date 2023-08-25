\name{expression_to_string}
\alias{expression_to_string}
\docType{package}
\title{
PRINT THE CONTENT OF A VARIABLE
}
\description{
With this command the content of a variable is printed.
}
\usage{
expression_to_string(int expression, string bdd_name)
}
\arguments{
\item{expression}{The index of the variable to print.}
\item{bdd_name}{(Optional) Name of the BDD.}
}
\value{
N/A
}
\examples{
expression_to_string(1)
expression_to_string(1, "bdd_1")
}
