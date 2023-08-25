\name{init_bdd}
\alias{init_bdd}
\docType{package}
\title{
CREATES A INSTANCE OF A BDD
}
\description{
This function creates an instance of the BDD factory. The user can choose the BDD manager to work with, and its possible values are "buddy" and "cudd".

Also, the number of nodes and the size of the cache can be provided. If BuDDy is selected as manager, these values are set as 1000, and if the manager selected is CUDD, both values are 32767.

It is mandatory to execute this instruction before executing any other command of this library.
}
\usage{
init_bdd(string library, int node_num, int cache_size, string bdd_name)
}
\arguments{
\item{library}{(Optional) The library to use in order to implement the BDD operations. The possible values of this argument are "buddy" or "cudd". Any other value prompts an error message. If this value is ommited, "buddy" manager will be chosen.}
\item{node_num}{(Optional) Number of nodes availables to allocate variables in the BDD. If BuDDy is selected as BDD manager, the default value is 1000 and for CUDD its value is 32767.}
\item{cache_size}{(Optional) Size of the cache of the factory, it improves the speed of the operations when instructions are executed repeatedly. The default value is 1000 for BuDDy and 32767 for CUDD.}
\item{bdd_name}{(Optional) Name of the BDD. It can contain letters, numbers and underscore.}
}
\value{
N/A
}
\examples{
init_bdd()
init_bdd("buddy")
init_bdd("cudd")
init_bdd("buddy", 2000)
init_bdd("cudd", 2000)
init_bdd("buddy", 2000, 5000)
init_bdd("cudd", 2000, 5000)
init_bdd("buddy", 2000, 5000, "bdd_1")
}
