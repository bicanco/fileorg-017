all:
	gcc interface.c CSV.c registro.c indicador.c delimitador.c numeroFixo.c caractere.c -o dominios -I./

test:
	gcc test.c CSV.c registro.c -o dominiostest -I./

run:
	./dominios

runtest:
	./dominiostest

runcl:
	clear
	@./dominios