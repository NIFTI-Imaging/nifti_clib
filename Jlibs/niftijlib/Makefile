
JAVAC   =       /usr/java2/bin/javac
JAVADOC   =       /usr/java2/bin/javadoc
JAR     =       /usr/java2/bin/jar

SRCS	=	Nifti1Dataset.java EndianCorrectInputStream.java EndianCorrectOutputStream.java

all: jar

jar: 
	$(JAVAC) *.java 
	${JAR} -cvf niftijlib.jar *.class

docs:	$(SRCS)
	mkdir -p doc
	(cd doc; $(JAVADOC) ../Nifti1Dataset.java ../EndianCorrect*.java )


clean:
	rm *Nifti*.class Endian*.class niftijlib.jar


help:
	@echo 'all:	make the niftijlib jar file'
	@echo 'docs:	make the javadoc API documentation, output is doc/index.html '
	@echo 'clean:	remove .class and .jar files'
