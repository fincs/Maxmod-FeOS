all:
	@make -C arm7
	@make -f mainlib.mk

clean:
	@make -C arm7 clean
	@make -f mainlib.mk clean

install:
	@make -C arm7 install
	@make -f mainlib.mk install
