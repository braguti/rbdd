\name{bdd_manager_init}
\alias{bdd_manager_init}
\docType{package}
\title{
CREATES AN INSTANCE OF A BDD
}
\description{
This function creates an instance of the BDD manager. The user can choose the BDD manager library to work with, and its possible values are "buddy" and "cudd".

Also, the number of nodes and the size of the cache can be provided. If BuDDy is selected as manager, these values are set as 1000, and if the manager selected is CUDD, both values are 32767.

It is mandatory to execute this instruction before executing any other command of this library.
}
\usage{
bdd_manager_init(library, node_num, cache_size, bdd_name)
}
\arguments{
\item{library}{(Optional) The library to use in order to implement the BDD operations. The possible values of this argument are "buddy" or "cudd". Any other value prompts an error message. If this value is ommited, "cudd" manager will be chosen.}
\item{node_num}{(Optional) Number of nodes availables to allocate variables in the BDD. If BuDDy is selected as BDD manager, the default value is 1000 and for CUDD its value is 32767.}
\item{cache_size}{(Optional) Size of the cache of the factory, it improves the speed of the operations when instructions are executed repeatedly. The default value is 1000 for BuDDy and 32767 for CUDD.}
\item{bdd_name}{(Optional) Name of the BDD. It can contain letters, numbers and underscore.}
}
\value{
N/A
}
\examples{
\dontrun{bdd_manager_init()}
\dontrun{bdd_manager_init("buddy")}
\dontrun{bdd_manager_init("cudd")}
\dontrun{bdd_manager_init("buddy", 2000)}
\dontrun{bdd_manager_init("cudd", 2000)}
\dontrun{bdd_manager_init("buddy", 2000, 5000)}
\dontrun{bdd_manager_init("cudd", 2000, 5000)}
\dontrun{bdd_manager_init("buddy", 2000, 5000, "bdd_1")}

bdd_manager_init(bdd_name="ex1")
bdd_new_variable("x", "ex1")
bdd_new_variable("y", "ex1")
bdd_parse_boolstr("x or y", "ex1")
bdd_manager_quit("ex1")
}
