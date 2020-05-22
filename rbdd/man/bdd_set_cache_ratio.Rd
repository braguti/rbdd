\name{bdd_set_cache_ratio}
\alias{bdd_set_cache_ratio}
\docType{package}
\title{
INCREASE THE CACHE RATIO OF THE BDD
}
\description{
This instruction allows to increase the cache ratio of the BDD.
}
\usage{
bdd_set_cache_ratio(cache_ratio, bdd_name)
}
\arguments{
\item{cache_ratio}{The increasement to apply at the current cache ratio, used in order to improve the speed of the execution of the operations storing them in a temporary memory.}
\item{bdd_name}{(Optional) Name of the BDD.}
}
\value{
N/A
}
\examples{
\dontrun{bdd_set_cache_ratio(10)}
\dontrun{bdd_set_cache_ratio(10, "bdd_1")}

bdd_manager_init(bdd_name="bdd1")
bdd_set_cache_ratio(5000, "bdd1")
bdd_manager_quit(bdd_name="bdd1")
}
