all:
	gcc -o main main.c helper.c model/superblock.c model/inode.c model/block.c

run:
	./main
