# Loads the rbdd library
library(rbdd)

# Loads the gmp library (GNU Multiple Precision Arithmetic)
# NOTE: Before installing the library, run this command:
# $ sudo apt-get install libgmp3-dev
suppressMessages(library(gmp))

# Loads the GNU MPFR library (GNU Multiple Precision Floating-Point)
# NOTE: Before installing the library, ensure you have the file /usr/include/mpfr.h or /usr/local/include/mpfr.h. If not, follow the steps defined in this link:
# https://www.mpfr.org/mpfr-current/mpfr.html#Installing-MPFR
suppressMessages(library(Rmpfr))

bdd_read("axtls.dddmp")

commonality <- bdd_compute_commonality()
distribution <- bdd_compute_distribution()

bdd_manager_quit()

