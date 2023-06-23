DIRS = $(shell find . -mindepth 1 -maxdepth 1 -type d)

all:
	@for dir in $(DIRS); do \
	    if [ -f $$dir/makefile ] || [ -f $$dir/Makefile ]; then \
	        echo "Compiling Dependency $${dir}"; \
	        $(MAKE) -C $$dir; \
	    fi \
	done

clean:
	@for dir in $(DIRS); do \
	    if [ -f $$dir/makefile ] || [ -f $$dir/Makefile ]; then \
	        $(MAKE) -C $$dir clean; \
	    fi \
	done
