# Loads the library
library(rbdd)

# Creates the BDDfactory
bdd_manager_init()

# Creates a variable from an expression
bdd_parse_boolstr("(mobilePhone = TRUE) and ((mobilePhone -> calls) and (calls -> mobilePhone)) and (gps -> mobilePhone) and ((mobilePhone -> screen) and (screen -> mobilePhone)) and (media -> mobilePhone) and ((((basic -> (not colour and not highResolution and screen)) and ((not colour and not highResolution and screen) -> basic))) and (((colour -> (not basic and not highResolution and screen)) and ((not basic and not highResolution and screen) -> colour))) and (((highResolution -> (not basic and not colour and screen)) and ((not basic and not colour and screen) -> highResolution)))) and (((media -> (camera or mp3)) and ((camera or mp3) -> media))) and not(gps and basic) and (camera -> highResolution)")

counter <- function(pindex, tindex, eindex, fthen, felse) {
	thenPart <- 2**(tindex - pindex - 1) * fthen
	elsePart <- 2**(eindex - pindex -1) * felse
	return (thenPart + elsePart)
}

cat(bdd_traverse(0, 1, "counter", trace = TRUE))
cat("\n")

bdd_manager_quit()

