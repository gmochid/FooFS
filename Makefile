all:
	gcc -o main main.c shm.c fsm.c helper.c model/superblock.c model/inode.c model/block.c

run:
	./main
