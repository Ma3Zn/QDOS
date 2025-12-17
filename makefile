EXE_DIR := ./

LINKFLAGS 	:= -o
LINK_LIB	:= -lgsl -lgslcblas -lm
LINKER		:= gcc

.PHONY:	build debug build_subsystem debug_subsystem clean clean_build

build: 	build_subsystem
debug: 	debug_subsystem

clean_build: clean build_subsystem

build_subsystem:
	cd ./Common				&& 		$(MAKE) build
	cd ./Custom\ Logic 		&&		$(MAKE) build
	cd ./Decomposer			&& 		$(MAKE) build
	cd ./Gate\ Built-in		&& 		$(MAKE) build
	cd ./Main				&& 		$(MAKE) build
	cd ./Parser 			&& 		$(MAKE) build
	cd ./P-S\ Interface 	&& 		$(MAKE) build
	cd ./Scheduler 			&& 		$(MAKE) build
	cd ./Simulator 			&& 		$(MAKE) build

debug_subsystem:
	cd ./Common				&& 		$(MAKE) debug
	cd ./Custom\ Logic 		&&		$(MAKE) debug
	cd ./Decomposer			&& 		$(MAKE) debug
	cd ./Gate\ Built-in		&& 		$(MAKE) debug
	cd ./Main				&& 		$(MAKE) debug
	cd ./Parser 			&& 		$(MAKE) debug
	cd ./P-S\ Interface 	&& 		$(MAKE) debug
	cd ./Scheduler 			&& 		$(MAKE) debug
	cd ./Simulator 			&& 		$(MAKE) debug

clean:
	cd ./Common				&& 		$(MAKE) clean
	cd ./Custom\ Logic 		&&		$(MAKE) clean
	cd ./Decomposer			&& 		$(MAKE) clean
	cd ./Gate\ Built-in		&& 		$(MAKE) clean
	cd ./Main				&& 		$(MAKE) clean
	cd ./Parser 			&& 		$(MAKE) clean
	cd ./P-S\ Interface 	&& 		$(MAKE) clean
	cd ./Scheduler 			&& 		$(MAKE) clean
	cd ./Simulator 			&& 		$(MAKE) clean

install: 
	$(LINKER) $(LINKFLAGS) $(EXE_DIR)qdos ./*/*/*.o $(LINK_LIB)
	cp $(EXE_DIR)qdos /usr/local/bin/qdos
	rm $(EXE_DIR)qdos
	cd ./Doc && ./install_man

install_blis:
	cd ./Main				&&		$(MAKE) build_blis
	$(LINKER) $(LINKFLAGS) $(EXE_DIR)qdos ./*/*/*.o -lgsl -lblis -lm -lpthread -fopenmp
	cp $(EXE_DIR)qdos /usr/local/bin/qdos
	rm $(EXE_DIR)qdos
	cd ./Doc && ./install_man

install_debug: debug
	$(LINKER) $(LINKFLAGS) $(EXE_DIR)qdos ./*/*/*.o $(LINK_LIB)

clean_debug: clean
	rm $(EXE_DIR)qdos

uninstall:
	rm /usr/local/bin/qdos
	cd ./Doc && ./uninstall_man
