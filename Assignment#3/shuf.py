#!/usr/bin/env python3

import random, sys
import argparse
import string

class randline:
	def _init_(self, filename):
		f = open(filename, 'r')
		self.lines = f.readlines()
		f.close()
	
	def chooseline():
		return self.lines
def main():
	version_msg = "%prog 1.0"
	usage_msg = """%prog [OPTION] ... FILE Shuffle and output random lines from FILE."""

	parser = argparse.ArgumentParser(description="Write a random permutation of the input liens to standard output. With no file, or when file is -, read standard input.")
	#parser = argparse.ArgumentParser(version=version_msg, usage=usage_msg)
#	parser = argparse.ArgumentParser(description='Shuffle lines')
	parser.add_argument("-i," "--input_range", 
		action="store", dest="inputRange", 
		help="Unsigned decimal integer range lo to hi")

	parser.add_argument("-r", "--repeat",
		action="store_true", dest="repeat", 
		help="Repeat output values select with replacement")

	parser.add_argument("-n", "--head-count", 
		type=int, dest="numlines", 
		help="Output at most count lines")
	#parser.add_argument('file', nargs='?',type=argparse.FileType('r'))
#	parser.add_argument('fileName', nargs='?', type=argparse.FileType('r'))
	parser.add_argument('fileName', nargs="*")
	#parser = argparse.ArgumentParser()	
	num = None
	options = parser.parse_args()
	#fileName = options.file
	readFile = False
	#options = parser.parse_args(sys.argv[1:])
	#Check -n (numlines) is valid)
	#try:
	#	input()
#	except NameError:
#		pass
	try:
		if options.numlines is not None:
			num = int(options.numlines)
	except:
		parser.error("invalid NUMLINES: {0}".
			format(options.numlines))
	if num != None and num < 0:
		parser.error("invalid: negative -n: {0}".
			format(options.numlines))
	universalNumlines = num
	#has to input 2 numbers EXACTLY
	total = []
	if options.inputRange is not None:
		try:
			splitting = str((options.inputRange)).split('-')
			lo = int(splitting[0])
			hi = int(splitting[1])
			total = list(range(lo, hi + 1))
		except:
			parser.error("invalid input range: {0}".
				format(options.inputRange))
		#check that the lo and hi numbers are appropriate
		#lo is higher than hi then hi is lower than lo
		if lo > hi:
			parser.error("lo " + str(lo) + "is higher than hi " + str(hi))
		if lo < 0 or hi < 0:
				parser.error("invalid -i negative numbers: {0}".
					format(options.inputRange))

		actualRange = (hi - lo + 1)
		if num is not None:
			if (actualRange < num):
				num = actualRange
		#covert total list into string with newlines
		for i in range(len(total)):
			total[i] = str(total[i]) + '\n'	
	#check if there are no arguments
	#implies options.inputRange is None
		if not len(sys.argv) > 1:
			parser.error("no arguments were provided: '{0}'".
				format(options.inputRange))
	
#		if not sys.argv[0] == "-":
#			parser.error("invalid arguement: '{0}'".
#				format(options.inputRange))
	
	if options.inputRange is None and len(options.fileName) > 1:
		parser.error("invalid: incorrect number of operands")
	if options.inputRange is not None and len(options.fileName) !=0:
		parser.error("invalid: wrong number of operands")
	if options.inputRange is None:
		if not len(options.fileName) or len(options.fileName) == 1 and options.fileName[0] == "-":
			try:
				stdin = input()
				while True:
					try:
						total.append(str(stdin) + '\n')
						stdin = input()
					except:
						break 
				readFile = True
			except EOFError:
				pass
		else:
			try:
				f = open(options.fileName[0])
				total = f.readlines()
				f.close()
				readFile = True
			except FileNotFoundError:
				parser.error("Invalid: no such file")
	random.shuffle(total)
		#default for store_true is false
		#boolean false = 0
	try:
		if not options.numlines and options.repeat:
			if options.numlines is None:
				num = True
			while num != False:
				sys.stdout.write(total[0])
				random.shuffle(total)
				if isinstance(num,bool) is False:
					num -= 1
		#-n or combo -n -r
		else:
			if num is None:
				num = len(total)
			diff = num
			if readFile is False and options.inputRange is not None and universalNumlines is not None and options.repeat:
				if actualRange < universalNumlines:
					diff = universalNumlines - actualRange
					while diff != 0:
						sys.stdout.write(random.choice(total))
						diff -= 1
			
			#for reading a text file
			#if -n is greater than file lines, will only print up to file lines
			if readFile is True and len(total) < diff:
				if options.repeat:
					diff = diff - len(total)
					while diff != 0:
						sys.stdout.write(random.choice(total))
						diff -= 1
					num = len(total)
				if not options.repeat:
					num = len(total) 	
			try:
				for i in range(0, num):
					sys.stdout.write(total[i])
			except IndexError:
				pass
		#	if options.repeat:
			 	
	except IOError as err:
		parser.erro("I/O error({0}): {1}".
			format(errno, strerror))

if __name__ == "__main__":
	main()
