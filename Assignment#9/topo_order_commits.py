
#! /usr/local/cs/bin/python3 

import os
import zlib #compression/depression library
#Problem 1: Discover the .git directory
class CommitNode:
	def __init__(self, commit_hash):
		"""
		:type commit-hash: str
		"""
		self.commit_hash = commit_hash
		self.parents = set()
		self.children = set()

target = ".git"
#current working directory (where it starts)

#currentDir = os.getcwd()
###############MAKE SURE TO REPLACE LATER############################
#currentDir = "/u/cs/fac/eggert/src/gnu/emacs-CS-35L"
currentDir = "/u/cs/ugrad/kimberls/cs35L/testing"

while True:
	files = os.listdir(currentDir)	
#	parent = os.path.dirname(currentDir)
	if target in files:
		#means that have found the .git file
		print(currentDir)
		break
	else:
		#not in the current working directory
		#will need to move to the parent directory
		parent = os.path.dirname(currentDir)
		if currentDir == parent:
			#is already at the end '/' parent
			print ("Not inside a Git repository")
			exit(1)
			break
		else:
			currentDir = parent


#Problem 2: Get the lsit of local branch names
#getting brances
localBranches = currentDir + "/.git/refs/heads/"
pathName = []
for path,subdirs, files in os.walk(localBranches):
	#pathName.append(path)
	for name in files:
		temp = os.path.join(path,name)
		pathName.append(temp)

	
for name in pathName:
	print(name)
print("==============")
#branch hashes in refs/head
for fileName in pathName:
	compres = open(fileName, 'rb').read()
	print(compres)
print("=========")
#objects stored in .git/objects
#git uses zlib to compress objects

objectsDir = currentDir + "/.git/objects/"
allObjects = []
objSubDir = []
#testing = os.walk(objectsDir)
#for testFile in testing:
#	print(testFile)
for path, subdirs, files in os.walk(objectsDir):
	for name in files:
	#	allObjects.append(os.path.join(path, name))
		temp = os.path.join(path,name)
		if "pack" not in temp and "info" not in temp:
			allObjects.append(temp)
	for name in subdirs:
		objSubDir.append(name)
for name in allObjects:
	print(name)
print("========================")

for name in objSubDir:

	print(name)
print("------------------")

for fileHead in allObjects:
	if fileHead != "pack" and fileHead != "info":
	#	compress = open(fileHead, 'rb').read()
	
		#compress = open(fileHead, 'r', encoding="ISO-8859-1")
		with open(fileHead, 'rb') as f:
			compress = f.read()
		decompress = zlib.decompress(compress).decode()
		#start = b'tree '
		#end = b'parent'
		start = 'tree '
		end = 'parent'
		startIndex =  decompress.find(start) + len(start)
		endIndex = decompress.find(end) + len(end)
		findHash = decompress[startIndex:endIndex]
		#length = len(b'\nparent')
		length = len('\nparent')
		#remove the \nparent at the end
		currentHash = findHash[0:len(findHash) - length]
		print(currentHash)
		print("***")
		#start2 = b'parent'
		#end2 = b'author'
		start2 = 'parent'
		end2 = 'author'
		startIndex2 = decompress.find(start2) + len(start2) + 1
		endIndex2 =decompress.find(end2) + len(end2)
		parent = decompress[startIndex2:endIndex2]
		#length2 = len(b'\nauthor')
		length2 = '\nauthor'
		#remove \nauthor at end
		#newParent = parent[0:len(parent) - length2]
	#	if len(currentHash) != 0 and len(newParent) != 0 :
	#		print("CURRENT: ")
	#		print(currentHash)
	#		print("PARENT: ")
	#		print(newParent)
		#get the parent
		
			
	#	print (decompress)
	#	print("&&&&&&&&&&&&&&&&&&")



