DEPEND1 = lib
DEPEND2 = prog/server
DEPEND3 = prog/client
DEPEND4 = include

all: $(DEPEND1) $(DEPEND2) $(DEPEND3) $(DEPEND4)
	cd $(DEPEND1); make
	cd $(DEPEND2); make
	cd $(DEPEND3); make
	
clean: 
	cd $(DEPEND1); make clean
	cd $(DEPEND2); make clean
	cd $(DEPEND3); make clean