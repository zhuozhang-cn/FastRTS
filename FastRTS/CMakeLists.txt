cmake_minimum_required(VERSION 3.16)
project(FastRTS)

set(CMAKE_CXX_STANDARD 14)
#set(CMAKE_BUILD_TYPE "Release")

add_executable(FastRTS main.cpp MyVector.h MyHugeVector.h QuadTreeLog.cpp QuadTreeLog.h Query.cpp Query.h TreeNode.cpp TreeNode.h Buckets.cpp Buckets.h GloVars.cpp GloVars.h BucketElement.h LogGen_IO.cpp LogGen_IO.h
	IndexablePoint.h 
	IndexablePoint.cpp 
	RangeTree.h
	RangeTree.cpp
	RTNode.h
	RTNode.cpp
	)


