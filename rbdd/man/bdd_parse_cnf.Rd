\name{bdd_parse_cnf}
\alias{bdd_parse_cnf}
\docType{package}
\title{
INITIALIZES THE BDD IN CNF FORMAT
}
\description{
This function allow the user to inform a BDD defined in CNF (Conjunctive Normal Form).
}
\usage{
bdd_parse_cnf(file, bdd_name, score, window, reorder, min_nodes, line_length, dyn_comp)
}
\arguments{
\item{file}{The BDD defined in CNF.}
\item{bdd_name}{(Optional) Name of the BDD.}
\item{score}{(Optional) The scoring algorithm. Possible values are "none", "perm", "sifting", "force", "forceblocks", "random", "other" and "file". The default value is "none".}
\item{window}{(Optional) The window size when the scoring algorithm selected is "perm". The default value is 1.}
\item{reorder}{(Optional) The reorder algorithm to apply to the constraints. The possible values are "minspan", "minimax", "remember" and "none". The default value is "minspan".}
\item{min_nodes}{(Optional) Param to indicate the mininum number of nodes that may exist to apply a reordering method. The default value is 100000.}
\item{line_length}{(Optional) Param to format the output. The default value is 27.}
\item{dyn_comp}{(Optional) Flag to indicate if the algorithm has to calculate the connected variables and it reorders each component individually. The default value is true.}
}
\value{
\item{result}{Returns 0 if it creates the BDD successfully, -1 otherwise.}
}
\examples{
\dontrun{bdd_parse_cnf("cnfFile.cnf")}
\dontrun{bdd_parse_cnf("cnfFile.cnf", "bdd_1")}
\dontrun{bdd_parse_cnf("cnfFile.cnf", "bdd_1")}
\dontrun{bdd_parse_cnf("cnfFile.cnf", "bdd_1", "sifting")}
\dontrun{bdd_parse_cnf("cnfFile.cnf", "bdd_1", "perm", 5)}
\dontrun{bdd_parse_cnf("cnfFile.cnf", "bdd_1", "force", "minimax")}
\dontrun{bdd_parse_cnf("cnfFile.cnf", "bdd_1", "other", "remember", 120000)}
\dontrun{bdd_parse_cnf("cnfFile.cnf", "bdd_1", "sifting", "none", 100000, 30)}
\dontrun{bdd_parse_cnf("cnfFile.cnf", "bdd_1", "sifting", "none", 100000, 30, false)}

bdd_manager_init(bdd_name="bdd1")
\dontrun{bdd_parse_cnf("cnfFile.cnf", "bdd1")}
bdd_manager_quit("bdd1")
}
