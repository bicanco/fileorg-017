all:
	gcc interface.c CSV.c registro.c indicador.c -o dominios -I./

test:
	gcc test.c CSV.c registro.c -o dominiostest -I./

run:
	./dominios

runtest:
	./dominiostest

runcl:
	clear
	@./dominios