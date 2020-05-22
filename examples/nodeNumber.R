library(rbdd)
library(ggplot2)

axtls_file = file("axtls.cnf")
axtls_length = length(readLines(axtls_file))
clauses = readLines(axtls_file, n = 1)
last_space = gregexpr(" ", clauses, fixed=TRUE)
loc<-last_space[[1]]
space<-loc[length(loc)]
clauses = substr(clauses, 0, space - 1)

number_nodes_axtls_none = 0
number_nodes_axtls_sift = 0
number_nodes_axtls_rand = 0
index = 1

for (line_number in 1:(axtls_length - 1)) {
	bdd_manager_init(library = "buddy", bdd_name = "axtls")
	clauses_new = paste(clauses, line_number)
	liness = readLines(axtls_file, n = line_number + 1)
	liness[1] = clauses_new
	fileCon<-file("output_axtls.cnf")
	writeLines(liness, fileCon)
	close(fileCon)
	bdd_parse_cnf("output_axtls.cnf", bdd_name = "axtls")
	bdd_reorder("none", bdd_name = "axtls")
	number_nodes_axtls_none[index] = bdd_info_node_number(bdd_name = "axtls")
	bdd_reorder("sift", bdd_name = "axtls")
	number_nodes_axtls_sift[index] = bdd_info_node_number(bdd_name = "axtls")
	bdd_reorder("random", bdd_name = "axtls")
	number_nodes_axtls_rand[index] = bdd_info_node_number(bdd_name = "axtls")
	index = index + 1
	bdd_manager_quit(bdd_name = "axtls")
}

close(axtls_file)

number_expressions <- seq(1, axtls_length - 1)
axtls_none.data <- data.frame(number_expressions, number_nodes_axtls_none)
axtls_sift.data <- data.frame(number_expressions, number_nodes_axtls_sift)
axtls_rand.data <- data.frame(number_expressions, number_nodes_axtls_rand)
axtls_graph <- ggplot() + 
	geom_line(data=axtls_none.data, aes(x=number_expressions, y=number_nodes_axtls_none, colour="None"), color="red") +
	geom_point(aes(x=number_expressions, y=number_nodes_axtls_none, colour="None"), size=1) + 
	geom_line(data=axtls_sift.data, aes(x=number_expressions, y=number_nodes_axtls_sift, colour="Sift"), color="blue") +
	geom_point(aes(x=number_expressions, y=number_nodes_axtls_sift, colour="Sift"), size=1) + 
	geom_line(data=axtls_rand.data, aes(x=number_expressions, y=number_nodes_axtls_rand, colour="Random"), color="green") +
	geom_point(aes(x=number_expressions, y=number_nodes_axtls_rand, colour="Random"), size=1) + 
	scale_colour_manual(name="Ordering", values=c("None"="red", "Sift"="blue", "Random"="green")) +
	labs(x = "Number of expressions", y = "Number of nodes") + 
	theme_bw() + theme(axis.title.x = element_text(size = 15, vjust=-.2)) + theme(axis.title.y = element_text(size = 15, vjust=0.3))

fiasco_file = file("fiasco.cnf")
fiasco_length = length(readLines(fiasco_file))
clauses = readLines(fiasco_file, n = 1)
last_space = gregexpr(" ", clauses, fixed=TRUE)
loc<-last_space[[1]]
space<-loc[length(loc)]
clauses = substr(clauses, 0, space - 1)

number_nodes_fiasco_none = 0
number_nodes_fiasco_sift = 0
number_nodes_fiasco_rand = 0
index = 1

for (line_number in 1:(fiasco_length - 1)) {
	bdd_manager_init(library = "buddy", bdd_name = "fiasco")
	clauses_new = paste(clauses, line_number)
	liness = readLines(fiasco_file, n = line_number + 1)
	liness[1] = clauses_new
	fileCon<-file("output_fiasco.cnf")
	writeLines(liness, fileCon)
	close(fileCon)
	fiasco_exp = bdd_parse_cnf("output_fiasco.cnf", bdd_name = "fiasco")
	bdd_reorder("none", bdd_name = "fiasco")
	number_nodes_fiasco_none[index] = bdd_info_node_number(bdd_name = "fiasco")
	bdd_reorder("sift", bdd_name = "fiasco")
	number_nodes_fiasco_sift[index] = bdd_info_node_number(bdd_name = "fiasco")
	bdd_reorder("random", bdd_name = "fiasco")
	number_nodes_fiasco_rand[index] = bdd_info_node_number(bdd_name = "fiasco")
	index = index + 1
	bdd_manager_quit(bdd_name = "fiasco")
}

close(fiasco_file)

number_expressions <- seq(1, fiasco_length - 1)
fiasco_none.data <- data.frame(number_expressions, number_nodes_fiasco_none)
fiasco_sift.data <- data.frame(number_expressions, number_nodes_fiasco_sift)
fiasco_rand.data <- data.frame(number_expressions, number_nodes_fiasco_rand)
fiasco_graph <- ggplot() + 
	geom_line(data=fiasco_none.data, aes(x=number_expressions, y=number_nodes_fiasco_none, colour="None"), color="red") +
	geom_point(aes(x=number_expressions, y=number_nodes_fiasco_none, colour="None"), size=1) + 
	geom_line(data=fiasco_sift.data, aes(x=number_expressions, y=number_nodes_fiasco_sift, colour="Sift"), color="blue") +
	geom_point(aes(x=number_expressions, y=number_nodes_fiasco_sift, colour="Sift"), size=1) + 
	geom_line(data=fiasco_rand.data, aes(x=number_expressions, y=number_nodes_fiasco_rand, colour="Random"), color="green") +
	geom_point(aes(x=number_expressions, y=number_nodes_fiasco_rand, colour="Random"), size=1) + 
	scale_colour_manual(name="Ordering", values=c("None"="red", "Sift"="blue", "Random"="green")) +
	labs(x = "Number of expressions", y = "Number of nodes") + 
	theme_bw() + theme(axis.title.x = element_text(size = 15, vjust=-.2)) + theme(axis.title.y = element_text(size = 15, vjust=0.3))

uClibc_file = file("uClibc.cnf")
uClibc_length = length(readLines(uClibc_file))
clauses = readLines(uClibc_file, n = 1)
last_space = gregexpr(" ", clauses, fixed=TRUE)
loc<-last_space[[1]]
space<-loc[length(loc)]
clauses = substr(clauses, 0, space - 1)

number_nodes_uClibc_none = 0
number_nodes_uClibc_sift = 0
number_nodes_uClibc_rand = 0
index = 1

for (line_number in 1:(uClibc_length - 1)) {
	bdd_manager_init(library = "buddy", bdd_name = "uClibc")
	clauses_new = paste(clauses, line_number)
	liness = readLines(uClibc_file, n = line_number + 1)
	liness[1] = clauses_new
	fileCon<-file("output_uclibc.cnf")
	writeLines(liness, fileCon)
	close(fileCon)
	uClibc_exp = bdd_parse_cnf("output_uclibc.cnf", bdd_name = "uClibc")
	bdd_reorder("none", bdd_name = "uClibc")
	number_nodes_uClibc_none[index] = bdd_info_node_number(bdd_name = "uClibc")
	bdd_reorder("sift", bdd_name = "uClibc")
	number_nodes_uClibc_sift[index] = bdd_info_node_number(bdd_name = "uClibc")
	bdd_reorder("random", bdd_name = "uClibc")
	number_nodes_uClibc_rand[index] = bdd_info_node_number(bdd_name = "uClibc")
	index = index + 1
	bdd_manager_quit(bdd_name = "uClibc")
}

close(uClibc_file)

number_expressions <- seq(1, uClibc_length - 1)
uClibc_none.data <- data.frame(number_expressions, number_nodes_uClibc_none)
uClibc_sift.data <- data.frame(number_expressions, number_nodes_uClibc_sift)
uClibc_rand.data <- data.frame(number_expressions, number_nodes_uClibc_rand)
uClibc_graph <- ggplot() + 
	geom_line(data=uClibc_none.data, aes(x=number_expressions, y=number_nodes_uClibc_none, colour="None"), color="red") +
	geom_point(aes(x=number_expressions, y=number_nodes_uClibc_none, colour="None"), size=1) + 
	geom_line(data=uClibc_sift.data, aes(x=number_expressions, y=number_nodes_uClibc_sift, colour="Sift"), color="blue") +
	geom_point(aes(x=number_expressions, y=number_nodes_uClibc_sift, colour="Sift"), size=1) + 
	geom_line(data=uClibc_rand.data, aes(x=number_expressions, y=number_nodes_uClibc_rand, colour="Random"), color="green") +
	geom_point(aes(x=number_expressions, y=number_nodes_uClibc_rand, colour="Random"), size=1) + 
	scale_colour_manual(name="Ordering", values=c("None"="red", "Sift"="blue", "Random"="green")) +
	labs(x = "Number of expressions", y = "Number of nodes") + 
	theme_bw() + theme(axis.title.x = element_text(size = 15, vjust=-.2)) + theme(axis.title.y = element_text(size = 15, vjust=0.3))
