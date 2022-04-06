#Script for turning move list representations into bitboards
#usage: python board_translator.py [input file] [output file]

from sys import argv

COL = lambda col: sum(1<<(6*col - 6 + row) for row in range(6))

with open(argv[1]) as infile:
	with open(argv[2], "w+") as outfile:
		for line in infile:
			if line.strip()=="":
				continue
			boards = [0, 0] #red, yellow
			moves, result = line.strip().split(" ")
			for index, move in enumerate(moves):
				boards[index%2] += (1<<(6*int(move) - 6)) + ((boards[0] | boards[1]) & COL(int(move)))
				#This uses the fact that the bitboard increases by column (the first column is 1, 2, 4, 8, 16, 32) to calculate the value of the next move
				#The -6 comes from the movelist being 1-based
			outfile.write(f"{boards[0]} {boards[1]} {int(result) * (1 if len(moves)%2==0 else -1)}\n")