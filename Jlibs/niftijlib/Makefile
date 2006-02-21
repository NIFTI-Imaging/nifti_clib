

JAVAC	=       /usr/java/bin/javac
JAVADOC	=       /usr/java/bin/javadoc
JAR	=       /usr/java/bin/jar

SRCS	=	Nifti1Dataset.java EndianCorrectInputStream.java EndianCorrectOutputStream.java

VERSION = 0.1

all: jar

jar: 	compile
	${JAR} -cvf niftijlib.jar *.class
	rm -f *.class

compile:
	$(JAVAC) *.java 

docs:	$(SRCS)
	mkdir -p doc
	(cd doc; $(JAVADOC) ../Nifti1Dataset.java ../EndianCorrect*.java )


clean:
	rm -f *.class
	rm -f niftijlib.jar


tarball:	jar
	(cd .. ; ln -s niftijlib niftijlib-${VERSION} ; tar -cf niftijlib-${VERSION}.tar \
	`find niftijlib-${VERSION}/* \! \( -name CVS -prune \) -type f`; \
	rm -f niftijlib-${VERSION});
	@echo ''
	@echo '../niftijlib-${VERSION}.tar created'
	@echo ''


help:
	@echo ''
	@echo 'all:     make the niftijlib jar file'
	@echo 'compile: make the niftijlib class files, but no jar file'
	@echo 'docs:    make the javadoc API documentation, output is doc/index.html '
	@echo 'clean:   remove .class and .jar files'
	@echo 'tarball: make a tarfile for release (includes jar file)'
	@echo ''
