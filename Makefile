bundle:
	$(MAKE) -C src
	cp src/hanoiFones service/ro/
	rm -f ../hanoiFones.tgz
	tar caf ../hanoiFones.tgz *
	@echo "#### Double check ../hanoiFones.tgz and submit it :) ####"
