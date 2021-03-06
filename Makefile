all:
	(cd timerLib; make install)
	(cd lcdLib; make install)
	(cd shapeLib; make install)
	(cd circleLib; make install)
	(cd p2swLib; make install)
	(cd snake; make)

load: all
	(cd snake; make load)
doc:
	rm -rf doxygen_docs
	doxygen Doxyfile
clean:
	(cd timerLib; make clean)
	(cd lcdLib; make clean)
	(cd shapeLib; make clean)
	(cd p2swLib; make clean)
	(cd snake; make clean)
	(cd circleLib; make clean)
	rm -rf lib h
	rm -rf doxygen_docs/*
