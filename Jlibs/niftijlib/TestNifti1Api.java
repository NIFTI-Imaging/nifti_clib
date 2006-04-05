
import java.io.*;
import java.text.*;
import java.util.zip.*;



public class TestNifti1Api {

	//////////////////////////////////////////////////////////////////
	/**
	* main routine is used for testing
	* @param args 2 arguments <input dataset> <output dataset>
	*/
	public static void main(String[] args) {
		
		double data[][][];
		double tmcrs[];
		int i,j,k,l;
		int icode;
		byte b[];

	if (args.length < 1) {
		args = new String[1];
		args[0] = new String("help");
	}


	///////// print header
	if (args[0].equals("print")) {
		if (args.length != 2) {
			System.out.println("\nError, print command takes exactly one parameter: print <dataset name>");
			return;
		}
		Nifti1Dataset nds = new Nifti1Dataset(args[1]);
		try {
			nds.readHeader();
			nds.printHeader();
		}
		catch (IOException ex) {
			System.out.println("\nCould not read header file for "+args[1]+": "+ex.getMessage());
		}

	return;
	}


	/// read a volume, peek a voxel
	if (args[0].equals("peek")) {
		if (args.length != 6) {
			System.out.println("\nError, peek command takes exactly five parameters: peek <dataset name> X Y Z T (0-based index)");
			return;
		}
		Nifti1Dataset nds = new Nifti1Dataset(args[1]);
		try {
			nds.readHeader();
			data = nds.readDoubleVol(Short.parseShort(args[5]));
			System.out.println("\n\nPeek "+args[2]+" "+args[3]+" "+args[4]+" "+args[5]+": "+data[Short.parseShort(args[4])][Short.parseShort(args[3])][Short.parseShort(args[2])]);
		}
		catch (IOException ex) {
			System.out.println("\nCould not read volume from "+args[1]+": "+ex.getMessage());
		}

	return;
	}


	/// read and print a timecourse at loc XYZ
	if (args[0].equals("timecourse")) {
		if (args.length != 5) {
			System.out.println("\nError, timecourse command takes exactly four parameters: timecourse <dataset name> X Y Z (0-based index)");
			return;
		}
		Nifti1Dataset nds = new Nifti1Dataset(args[1]);
		try {
			nds.readHeader();
			tmcrs = nds.readDoubleTmcrs(Short.parseShort(args[2]), Short.parseShort(args[3]), Short.parseShort(args[4]));
			nds.printDoubleTmcrs(tmcrs);
		}
		catch (IOException ex) {
			System.out.println("\nCould not read timecourse from "+args[1]+": "+ex.getMessage());
		}

	return;
	}


	/// copy a dataset
	if (args[0].equals("copy")) {
		if (args.length != 3) {
			System.out.println("\nError, copy command takes exactly two parameters: copy <dataset name> <new dataset name>");
			return;
		}
		Nifti1Dataset nds = new Nifti1Dataset(args[1]);
		try {
			nds.readHeader();
			b = nds.readData();
			nds.setHeaderFilename(args[2]);
			nds.setDataFilename(args[2]);
			nds.writeHeader();
			nds.writeData(b);
		}
		catch (IOException ex) {
			System.out.println("\nCould not copy "+args[1]+" to "+args[2]+": "+ex.getMessage());
		}

	return;
	}


	/////// add an extension to a dataset
	if (args[0].equals("extend")) {
		if (args.length != 4) {
			System.out.println("\nError, copy command takes exactly three parameters: extend <dataset name> <filename> <code>");
			return;
		}
		icode = Integer.parseInt(args[3]);
		Nifti1Dataset nds = new Nifti1Dataset(args[1]);
		try {

			nds.readHeader();

			/// nii has to save/write data blob
			if (nds.ds_is_nii) {
				b = nds.readData();
				nds.addExtension(icode,args[2]);
				nds.writeHeader();
				nds.writeData(b);
			}
	
			/// n+1 can just append to header
			else {
			nds.addExtension(icode,args[2]);
			nds.writeHeader();
			}
		}
		catch (IOException ex) {
			System.out.println("\nCould not extend "+args[1]+" with "+args[2]+": "+ex.getMessage());
		}

	return;
	}


	/// create a dataset
	short xx,yy,zz,tt;
	if (args[0].equals("create")) {
		if (args.length != 8) {
			System.out.println("\nError, create command takes exactly seven parameters: create <new dataset name> <datatype> X Y Z T <A|B|X>");
			return;
		}
		Nifti1Dataset nds = new Nifti1Dataset();
		try {
			xx = Short.parseShort(args[3]);
			yy = Short.parseShort(args[4]);
			zz = Short.parseShort(args[5]);
			tt = Short.parseShort(args[6]);

			nds.setHeaderFilename(args[1]);
			nds.setDataFilename(args[1]);
			nds.setDatatype(Short.parseShort(args[2]));
			nds.setDims((short)4,xx,yy,zz,tt,(short)0,(short)0,(short)0);
			nds.descrip = new StringBuffer("Created by TestNifti1Api");
			nds.writeHeader();

			

			data = new double[zz][yy][xx];
			int ctr = 0;
			for (l=0; l<tt; l++) {
				
				ctr = 0;
				for (k=0; k<zz; k++)  
				for (j=0; j<yy; j++) 
				for (i=0; i<xx; i++)  {
					/// vals are slice #
					if (args[7].equals("A")) {
						data[k][j][i] = l;
					}
					else if (args[7].equals("B")) {
						data[k][j][i] = ctr++;
					}
					else {
						data[k][j][i] = 0;
					}
				}


				nds.writeVol(data, (short)l);
			}
		}
		catch (IOException ex) {
			System.out.println("\nCould not create "+args[1]+": "+ex.getMessage());
		}

	return;
	}


	/////// if they get here they need help
	System.out.println("\n\nUsage: TestNifti1Api <command> <dataset> [params]");
	System.out.println("\n");
	System.out.println("\tprint <dataset>");
	System.out.println("\t\tprint dataset header fields");

	System.out.println("\tpeek <dataset> X Y Z T");
	System.out.println("\t\tprint value at voxel location X Y Z T (0-based index)");
	System.out.println("\ttimecourse <dataset> X Y Z");
	System.out.println("\t\tprint timecourse at voxel location X Y Z (0-based index)");

	System.out.println("\tcopy <dataset> <dataset2>");
	System.out.println("\t\tdisk copy of dataset to dataset2 ");

	System.out.println("\tcreate <new dataset> <datatype> X Y Z T <A|B|X>");
	System.out.println("\t\tcreate a new dataset of dimension XYZT.");
	System.out.println("\t\tVoxel values are determined by last param.");
	System.out.println("\t\tA -- volumes are constant val, equal to their T index.");
	System.out.println("\t\tB -- each volume has voxels set from 0 to n, where n=(X*Y*Z)-1");
	System.out.println("\t\tX -- voxels are 0");

	System.out.println("\textend <dataset> <extension_file> <code>");
	System.out.println("\t\ttake <extension_file> and stuff it into the header of <dataset>\n\t\tas an extension of type <code>");
	System.out.println("\t\t<code> is an identifying code for the extension type (ask Bob).");

	System.out.println("\n");

	return;
	}

}
