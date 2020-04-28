
#! /usr/local/cs/bin/python3 

import os
import zlib #compression/depression library
#Problem 1: Discover the .git directory
class CommitNode:
#	def __init__(self, commit_hash, addParents):
	def __init__(self, commit_hash):
		"""
		:type commit-hash: str
		"""
		self.commit_hash = commit_hash
#		self.addParents = addParents
		self.parents = set()
#		self.parents.add(addParents)
	#	self.parents = set()
		self.children = set()
	def __eq__ (self,other):
		return self.commit_hash == other.commit_hash
	def __hash__(self):
		return hash(self.commit_hash)
#	def add(self):
#		if self not in set:
#			set.add(self)

	
#current working directory (where it starts)

#currentDir = os.getcwd()
###############MAKE SURE TO REPLACE LATER############################
#currentDir = "/u/cs/fac/eggert/src/gnu/emacs-CS-35L"
def findGitDirectory():
	target = ".git"
	currentDir = "/u/cs/ugrad/kimberls/cs35L/testing"
#	currentDir = "/u/cs/ugrad/kimberls/cs35L/gitTesting"
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
	#			print ("Not inside a Git repository")
				exit(1)
				break
			else:
				currentDir = parent

	return currentDir
#Problem 2: Get the lsit of local branch names
#getting brances
def getLocalBranchNames():
	currentDir = findGitDirectory()
	localBranches = currentDir + "/.git/refs/heads/"
	pathName = []
	for path,subdirs, files in os.walk(localBranches):
	#pathName.append(path)
		for name in files:
			temp = os.path.join(path,name)
			pathName.append(temp)

#pathName are all the branch names	
	for name in pathName:
		print(name)
	print("==============")
#branch hashes in refs/head
	for fileName in pathName:
		#compres = open(fileName, 'rb').read().decode()
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
	return allObjects

def buildCommitGraph():
	allObjects = getLocalBranchNames()
	allCommits = {}
	for fileHead in allObjects:
		if fileHead != "pack" and fileHead != "info":
			compress = open(fileHead, 'rb').read()
			decompress = zlib.decompress(compress)
			start = b'tree '
			end = b'parent'
			startIndex =  decompress.find(start) + len(start)
			endIndex = decompress.find(end) + len(end)
			findHash = decompress[startIndex:endIndex]
			length = len(b'\nparent')
		#remove the \nparent at the end
			currentHash = (findHash[0:len(findHash) - length]).decode()
			start2 = b'parent'
			end2 = b'author'
			startIndex2 = decompress.find(start2) + len(start2) + 1
			endIndex2 =decompress.find(end2) + len(end2)
			parent = decompress[startIndex2:endIndex2]
			length2 = len(b'\nauthor')
		#remove \nauthor at end
			#print("PRINTING ALL CURRENT AND PARENTS!")
			newParent = (parent[0:len(parent) - length2]).decode()
			if len(currentHash) != 0 and len(newParent) != 0 :
				print("CURRENT: ")
				print(currentHash)
				print("PARENT: ")
				print(newParent)
		#		allCommits.add(CommitNode(currentHash))
				if currentHash not in allCommits:
					allCommits[currentHash] = CommitNode(currentHash)
				allCommits[currentHash].parents.add(newParent)
			#	else:
			#		allCommits[currentHash].parents.add(newParent)
				#check if parent node already exists
		#		if newParent in allCommits.keys():	
				if newParent not in allCommits:	
				#will have to establish relationship
					allCommits[newParent] = CommitNode(newParent)
				allCommits[newParent].children.add(currentHash)
						
			#	else:
			#		allCommits[currentHash] = CommitNode(newParent)
					#allCommits[currentHash].children.add(currentHash)				
			#print("%%%%%%%%%%%%")
			#for key in allCommits:
			#	print(key)
	print("###########")	
	#still need to implement root_commits	
	for currentBranch in allCommits:
		print("Current: " + currentBranch)			
		#print all teh parents
		for parent in allCommits[currentBranch].parents:
			print("Parent: " + parent)
		for children in allCommits[currentBranch].children:
			print("Children: " + children)
		print("$$$$$$$")
	return allCommits	

def topoOrder():
	allCommits = buildCommitGraph()
	printStack =[]
	tempStack = []
	for key in allCommits:
		#children set is empty
		if not  allCommits[key].parents:
			print("APPENDED")
			print(key)
			print("---")
		#	tempStack.append(allCommits[key])
			tempStack.append(key)	

	print("TESTING")
#	while tempStack:
#		print(tempStack.pop())
	
	while tempStack:
		#traverse through the children
		currNode = tempStack.pop()
		#print("HERE: " + currNode.commit_hash)
		print("Here: " + currNode)
		printStack.append(currNode)
		#look through all the parents and remove that node
		childrenSet = allCommits[currNode].children
		
		for node in childrenSet:
			print("CHILD: " + node)
			#find that node's CommitNode and delete the parent
			#childNode = allCommits[node]
			#parentSet = childNode.parents
			parentSet = allCommits[node].parents
			for parent in parentSet:
				print("Parent: " + parent)
				if parent  == currNode:
					allCommits[node].parents.remove(parent)
					#now check if their parent set is now empty
					print("TESTING PARENTS!!")
					for test in allCommits[node].parents:
						print(test)
					if not allCommits[node].parents:
						print("ADD to the stack" + node)
						
						tempStack.append(node)
					print("----")
					break		
		#now the child has no parents	
			#if not parentSet:
			#	tempStack.append(currNode)
		#	print("CURRENT: " + currNode)
		#	printStack.append(currNode)
	#	tempStack.pop()		
	#	print("Current stack: ")
	#	for stack in tempStack:
	#		print(stack)	

#	print("@@@@@@@@@")
#	print len(printStack)
#	while  printStack:
#		print(printStack.top())
#		print(printStack.pop())
	return printStack

def printTopo():
	print("THIS IS IT: ")
	finalStack = topoOrder()
	while finalStack:
		print (finalStack.pop())




buildCommitGraph()
topoOrder()	
printTopo()	



