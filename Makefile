bundle:
	$(MAKE) -C src
	cp src/hanoiFones service/ro/
	strip service/ro/hanoiFones
	rm -f ../hanoiFones.tgz
	tar caf ../hanoiFones.tgz *
	@echo "#### Double check ../hanoiFones.tgz and submit it :) ####"
