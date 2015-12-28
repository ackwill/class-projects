import shutil
import subprocess
import os
import sys

projectname = sys.argv[1]
directory = "../" + projectname


def makeProject():
	print "--- Copying Sources ---"
	shutil.copyfile(projectname+'.cpp', directory+"/"+projectname+'.cpp')
	os.chdir(directory)
	subprocess.call('cmake .')
	print "-- Building Project ---"
	subprocess.call('make')
	print "---Running "+projectname+" ---"
	subprocess.call('./'+projectname)

def createMakefile():
	contents = "cmake_minimum_required(VERSION 2.8)\nproject( "+ projectname+ " )\nfind_package( OpenCV )\ninclude_directories( ${OpenCV_INCLUDE_DIRS} )\nadd_executable( "+projectname+" "+projectname+".cpp )\ntarget_link_libraries( "+projectname+" ${OpenCV_LIBS} )"
	with open(directory+"/CMakeLists.txt","w+") as f:
		f.write(contents)

if __name__ == "__main__":
	if os.path.isdir(directory):
		print "Info: Project Exists"
		makeProject()
	else:
		print "Info: Project Not Found"
		print "Info: Creating New Project"
		print "--- Creating Project Directory ---"
		os.makedirs(directory)
		createMakefile()
		makeProject()




# copy shutil.copyfile()
#cd os.chdir()
#run command subprocess.call('make')
