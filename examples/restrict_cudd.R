# Load the library
library(rbdd)

# Create the BDDfactory
bdd_manager_init()

bdd_parse_boolstr("(a or b) and c")

bdd_restrict(restriction="a", restriction_name="restrict1")

bdd_compute_commonality(restriction="restrict1")

bdd_manager_quit()
