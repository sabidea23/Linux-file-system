build:
	gcc linux.c -g -o linux

clean:
	rm linux

checker:
	python3 checker.py

valgrind:
	valgrind --leak-check=yes --show-leak-kinds=all --track-origins=yes ./tema