\name{set_cache_ratio}
\alias{set_cache_ratio}
\docType{package}
\title{
INCREASE THE CACHE RATIO OF THE BDD
}
\description{
This instruction allows to increase the cache ratio of the BDD.
}
\usage{
set_cache_ratio(int cache_ratio, string bdd_name)
}
\arguments{
\item{cache_ratio}{The increasement to apply at the current cache ratio, used in order to improve the speed of the execution of the operations storing them in a temporary memory.}
\item{bdd_name}{(Optional) Name of the BDD.}
}
\value{
N/A
}
\examples{
set_cache_ratio(10)
set_cache_ratio(10, "bdd_1")
}
