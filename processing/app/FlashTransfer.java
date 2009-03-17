package processing.app;

import java.io.File;
import java.io.FileFilter;
import java.util.ArrayList;



/*
 Email Notes: 3/5/09
 
 //*************************************************************
 // A new flash transfer object, needs an available serial port.
 FlashTransfer touchTransfer = new FlashTransfer(editor.serial); 
 
 // Usage
 String result;
 int progress;
 String version;
 String[] fileList;
 byte[] fileBytes;
 long availableSpace;
 
 // Methods
 result = touchTransfer.send(File file); //sends 1 file
 result = touchTransfer.format(); 	//formats the disk					
 progress = touchTransfer.getProgress(); //gets the progress (in percent) of the current file send
 version = touchTransfer.getVersionID(); //gets a string of the flash
 fileList = touchTransfer.getFileList(); //gets a directory listing of the files
 fileBytes = touchTransfer.getFile(String fileName); //returns the file, in a byte array
 availableSpace = touchTransfer.getAvailableSpace(); //returns available space, in bytes
 */


public class FlashTransfer {
	Serial serialPort;
	String versionID;
	
	private static boolean killTransfer = false; //if this is set to true, kill the file transfer
	
	/* Constructor.
	 * serialPort : An available serial port.
	 * versionID  : string needs to match the device's id string ex. "FlashFileSystem 0.1.0" */
	public FlashTransfer(){
		//this.serialPort = serialPort;
		//serialPort.rate = 115200;
		//this.versionID = versionID;
	}
	
	/* Sets the serial object 
	 * for the transfer */
	public void setSerial(Serial serial){
		this.serialPort = serial;
	}

	/*
	 * Acts as a kill switch for transfering files to the flash system.
	 */
	public static void killTransfer(){
		killTransfer = true;
	}
	
	/*
	 * Closes the connection.
	 * Device continues code execution.
	 */
	public void close() {
	
		/* Send Exit command */
		serialPort.clear(); //purge
		serialPort.write("E");
		
		while (serialPort.readChar() != 'D') { ; } //wait
		
	}
	
	/*
	 * Identifies the device we're talking to.
	 * @return Returns true if device found.
	 */
	private boolean identifyFlash() {
		String id;
		
		serialPort.clear(); //purge
		
		
		try{ Thread.sleep(100); } catch (Exception e) { }
		
		serialPort.write("I"); //send the Identify command

		try{ Thread.sleep(200); } catch (Exception e) { }
		
		/* Read the string */
		try {
			id = new String(serialPort.readString());
		} catch (Exception e) {
			return false; //not ready
		}	
		
			if (id.indexOf("Flasher") < 0) {
				System.out.println("Arduino Flasher: Device not ready");
			    return false;
			}
	
		/* We found something */
		return true;
	}
	
	/* Loads a file.
	 * 
	 * Returns:
	 * If the file is a bitmap, it returns 
	 * TouchShield friendly bitmap data.
	 * 
	 * Otherwise, returns a byte array of file data.
	 */
	private byte[] loadFile(File file) {
		
		byte[] fileBytes = new byte[0];
		                            
		/* Load the file */
		try { 
			
			/* Image check */
			if (file.getName().indexOf(".bmp") > 0) {
				
				DeviceBMP bmp = new DeviceBMP(file);
					       
				/* Get the parsed data */
				fileBytes = bmp.getBytes();
				
			} else {
				
				/* Get the file bytes */
				fileBytes = Base.grabFile(file);
			}
			
		} catch (Exception e) { 
			System.out.println("Cannot open " + file.getName());
		} 
		
		return fileBytes;
	}
	
	/* Sends one file to the flash disk.
	 * returns a string starting with either
	 * pass or fail */
	public boolean sendFile(File file) throws SerialException {
			serialPort.monitor = false; //disable the console reads
			//serialPort.rate = 57600; 	// This is the speed at which we transfer
			serialPort.rate = 115200; 	// This is the speed at which we transfer
			
		byte[] fileBytes = loadFile(file);
		
		/* Are you there? */
		if (!identifyFlash()) {
			System.out.println("Nobody there.");
			return false;
		}
		
		/* Send STORE command */
		serialPort.clear(); //purge
		serialPort.write("S");
		while (serialPort.readChar() != 'D' && killTransfer != true) { ; } //wait
		
		
		//File Header format:
		// |                18 bytes          |
		// |  SOH       | FileSize | FileName | FileData        |  
		// | 0x01 0x01  | 4 bytes  | 12 bytes | variable length |
		
		
		/* Send SOH */
		serialPort.clear(); //purge
		serialPort.write((char)1);
		serialPort.write((char)1);
		
		/* Send File Size */
		serialPort.clear(); //purge
		serialPort.write((char)((fileBytes.length >> 24) & 255));
		serialPort.write((char)((fileBytes.length >> 16) & 255));
		serialPort.write((char)((fileBytes.length >> 8) & 255));
		serialPort.write((char)(fileBytes.length & 255));
		
		/* Send File Name String */
		serialPort.clear(); //purge
		
	    int fNameSize = 8+4;
	    String fName = file.getName();
	    
	    // Send the file name
	    for (int x=0; x<fNameSize; x++) {				
	    	if(killTransfer != true) break;
	    	if (x < (fName.length()) ) {
	    		serialPort.write(fName.charAt(x));	//send the char
	    	} else {
	    		serialPort.write((char)0 & 255);				//else, pad with null
	    	}
	    }
	    
		//wait for a response
		while (serialPort.readChar() != 'D' && (killTransfer != true)) { ; } //wait
		
		/* Send the file data */
		int index = 0;
		int pageSize = 528;
		int pageCount = fileBytes.length / pageSize;	
		int remainder = fileBytes.length % pageSize;	//Calculate reminder bytes
		
		if (remainder > 0) {
			pageCount++;		//send an extra page, if there's a remainder
		}
		
		/* Send the file bytes */
		for (int x = 0; x < pageCount; x++) {
			byte newPage[] = new byte[pageSize];
			if(killTransfer != true) break;
			
			for (int p=0; p < pageSize; p++) {
				if(killTransfer != true) break;
				if (index < fileBytes.length) {
					newPage[p] = fileBytes[index];
					//serialPort.write((char)fileBytes[index]);	//send data
				} else {
					newPage[p] = 0;
					//serialPort.write((char)0);				//else pad with null
				}
				
				index++;
			}
			
			serialPort.write(newPage);
			
			/* Wait for a response after the page was sent */
			while (serialPort.readChar() != 'D' && killTransfer != true) { ; } //wait
		}
		
		
		return true;
	}
	
	/* Formats the flash disk.
	 * returns a string starting with either:
	 * pass or fail */
	public boolean format() {
		
		
		if (identifyFlash()) {
			
			serialPort.write("F"); //send the Identify command
			
			try{ Thread.sleep(200); } catch (Exception e) { }
			
			/* Wait for a response after the request was sent */
			if (serialPort.readChar() != 'D') { 
				return false;
			}
			
		} else {
			return false;
		}
		
		return true;
	}
	
	/* Gets the current send 
	 * progress of the file, in percent. */
	public int getProgress() {
		return 0;
	}
	
	/* Gets the ID string of the device 
	 * we're talking to. */
	public String getVersionID() {
		return "FlashFileSystem 0.1.0";
	}
	
	/* Gets a directory listing on the Flash disk.
	 * 
	 * directory: "/" for the base directory
	 * 
	 * Returns a string array. The file list is formatted 
	 * into a string array in the following way: 
	 * 	"imageFile.bmp","23000"
	 * 
	 * "23000" = file size in bytes.
	 */
	public String[] getFileList(String directory) {
		return new String[] { "imageFile.bmp","2300","testFile2.txt","1600"};
	}
	
	/* Reads a file from the Flash Disk.
	 * fileName : the string filename.
	 * returns: a byte array of the file. */
	public Byte[] getFile(String fileName) {
		Byte[] fileBytes;
		
		if (fileName.matches("imageFile.bmp")) {
			fileBytes = new Byte[23000];
		}
		else if (fileName.matches("testFile2.txt")) {
			fileBytes = new Byte[1600];
		}
		else {
			fileBytes = new Byte[1];
		}
		
		return  fileBytes;
	}
	
	/* Reads the current available space 
	 * on the flash disk, in bytes.
	 * 
	 * returns -1 if fail.
	 */
	public long getAvailableSpace() {
		
		return 2000000 - (23000 + 1600); 
	}
	
	
	//	  /*
	//	  Sends the files located
	//	  in the data folder to a device
	//	  */
	//	   public void ModuleImageTransfer(){
	//	      
	//	      /* Design outline
	//	      look for TouchShield
	//	      Find files in the data folder 
	//	      parse files by type
	//	      build images
	//	      build files
	//	      build header
	//	      build data
	//	      split into pages
	//	      send data
	//	      */
	//
	//	     Flash flashData = new Flash(2000000,528*2); //2 meg flash chip, 2 pages for header
	//
	//	     /* Find image files in the data folder */
	//	     File[] imageFiles = getBMPFiles(imageTransferGadget.getData());
	//
	//	     for(int i=0;i<imageFiles.length;i++) {
	//	       DeviceBMP bmp = new DeviceBMP(imageFiles[i]);
	//	       
	//	       /* Add file to the flash */
	//	       flashData.addFile(bmp.name,bmp.getBytes());
	//	     }
	//
	//	      String flasherId;
	//	      
	//	      serialPort.monitor = false; //disable the console reads
	//	      System.out.println("Arduino Flasher: Starting");
	//	      
	//	      serialPort.clear(); //purge
	//	      serialPort.write("I"); //send the Identify command
	//	      System.out.println("Arduino Flasher: Checking for device...");
	//
	//	      try{ Thread.sleep(200); } catch (Exception e) { }
	//
	//	      System.out.println(serialPort.port.getName());
	//	      /* Look for the ID of the device */
	//	      if (serialPort.readString().indexOf("Flasher") < 0) {
	//	         System.out.println("Arduino Flasher: Device not ready");
	//	         return;
	//	      }
	//	      System.out.println("Arduino Flasher: Found Device");
	//
	//	      eraseChip();
	//
	//	      try{ Thread.sleep(200); } catch (Exception e) { }
	//	      
	//	      byte p[] = flashData.getPage(0);
	//	      for(int y=0; y<528; y++) {
	//	        System.out.print((int)(p[y]&0xFF)); System.out.print(" ");
	//	      }
	//	      
	//	      int numOfPages = flashData.getNumPages();
	//
	//	      /* Write each page */
	//	      for (int pageNum=0; pageNum<numOfPages; pageNum++) {
	//	    	  System.out.println(pageNum);
	//	        writePage(flashData.getPage(pageNum), pageNum);
	//	      }
	//	      
	//	      /* Exit Flasher */
	//	      close();
	//
	//	      System.out.println("Arduino Flasher: Complete!");
	//	   }
	//	   
	//
	//	   private int close(){
	//
	//	     /* Send close command */
	//	     System.out.println("Arduino Flasher: Closing");
	//	     serialPort.clear(); //purge
	//	     serialPort.write("E");
	//	     while (serialPort.readChar() != 'D') { ; } //wait
	//	     System.out.println("Arduino Flasher: Closed");
	//
	//	     return 1;
	//
	//	   }
	//
	//	   private int eraseChip() {
	//
	//	     /* Send Erase Chip command */
	//	     System.out.println("Arduino Flasher: Erasing Chip");
	//	     serialPort.clear(); //purge
	//	     serialPort.write("R");
	//	     while (serialPort.readChar() != 'D') { ; } //wait
	//	     System.out.println("Arduino Flasher: Chip erase complete");
	//
	//	     return 1;
	//	   }
	//
	//	   private int writePage(byte buff[], int pageNum){
	//	     
	//	     /* Calculate checksum */
	//	     byte checksum=0;
	//	     byte verify=0;
	//	     for (int i=0; i< buff.length; i++) {
	//	       checksum += buff[i];
	//	     }
	//	     checksum = (byte)(checksum ^ 255);
	//	     checksum +=1;
	//	     
	//	     /* Send the STORE command */
	//	     System.out.println("Arduino Flasher: Sending STORE");
	//
	//	     /* Send the page number */
	//	     System.out.println("Arduino Flasher: Sending pagenum: " + pageNum);
	//	     serialPort.clear(); //purge
	//	     serialPort.write((char)(pageNum & 255));
	//	     serialPort.write((char)((pageNum >> 8) & 255));
	//	     while (serialPort.readChar() != 'D') { ; } //wait
	//
	//	     /* Send the page data */
	//	     System.out.println("Arduino Flasher: Sending page data");
	//	     serialPort.clear(); //purge
	//	     //serialPort.write(buff);
	//	     for (int i=0; i< buff.length; i++) {
	//	       this.serialPort.write(buff[i]);
	//	       System.out.println(i);
	//	       try{ Thread.sleep(5); } catch (Exception e) { }
	//	     }
	//	     while (serialPort.readChar() != 'D') { ; } //wait
	//
	//	     /* Ask for checksum */
	//	     System.out.println("Arduino Flasher: asking for checksum");
	//	     serialPort.clear(); //purge
	//	     serialPort.write("C");
	//	     while(serialPort.available() < 1) { ; } 
	//	     verify = (byte)serialPort.readChar();
	//
	//	     if (verify != checksum) {
	//	        System.out.println("Arduino Flasher: checksum error at " + pageNum + "Expected: " +checksum + "Got: " + verify);
	//	        return -1;
	//	     }
	//	     
	//	     System.out.println("Arduino Flasher: Verified: " + pageNum);
	//
	//	     /* No errors */
	//	     return 1;
	//	   }
	//
	//	   private class Flash {
	//	       byte flashChip[];
	//	       int pageSize = 528;
	//	       int flashHeaderIndex=0;
	//	       int flashChipIndex;
	//
	//	       public Flash(int flashSize, int headerSize){
	//	         flashChip = new byte[flashSize];
	//	         flashChipIndex = headerSize; 
	//	       }
	//	       
	//	       /* Add a file to the flash data
	//	          The file name you want to add, and the file byte data.
	//	          This will add it to the header and copy data */
	//	       public int addFile(String file, byte[] fileBytes){
	//	         try {
	//
	//	           /* Copy file to Header */
	//	           /* Copy Name */
	//	           byte fileNameBytes[] = file.getBytes();
	//	           System.arraycopy(fileNameBytes,0,flashChip,flashHeaderIndex,fileNameBytes.length);
	//	           flashHeaderIndex+=fileNameBytes.length;
	//	           flashChip[flashHeaderIndex] = 0; //terminate the file name string
	//	           flashHeaderIndex++;
	//	    
	//	           /* Copy the data offset */
	//	           flashChip[flashHeaderIndex] = (byte)((flashChipIndex>>24) & 0xFF);
	//	           flashHeaderIndex++;
	//	           flashChip[flashHeaderIndex] = (byte)((flashChipIndex>>16) & 0xFF);
	//	           flashHeaderIndex++;
	//	           flashChip[flashHeaderIndex] = (byte)((flashChipIndex>>8) & 0xFF);
	//	           flashHeaderIndex++;
	//	           flashChip[flashHeaderIndex] = (byte)((flashChipIndex) & 0xFF);
	//	           flashHeaderIndex++;
	//	           
	//	           /* Copy file data */
	//	           System.arraycopy(fileBytes,0,flashChip,flashChipIndex,fileBytes.length);
	//	           flashChipIndex+= fileBytes.length;
	//
	//	           return 1;
	//	         } catch (Exception e) {
	//	           //Do something graceful here
	//	           return -1;
	//	         }
	//	       }
	//
	//	       /* Return 528 bytes
	//	          of flash data. */
	//	       public byte[] getPage(int pageNum){
	//	          byte page[] = new byte[pageSize];
	//	          System.arraycopy(flashChip,pageNum*pageSize,page,0,page.length); //copy the page
	//	          return page;
	//	       }
	//
	//	       /* gets the current
	//	          page total */
	//	       public int getNumPages() {
	//	         return (flashChipIndex / pageSize) + 1;
	//	       }
	//
	//
	//	     }
	//
	//
	//	   /* A class for a 24-bit bitmap image
	//	      Formats the image for fast display
	//	      on the TouchShield. */
	//	   private class DeviceBMP {
	//	     public int width;
	//	     public int height;
	//	     public String name;
	//	     File image;
	//	     byte imgBytes[];
	//	     
	//	        public DeviceBMP(File image) {
	//	            this.image = image;  
	//	            try {
	//	                imgBytes = Base.grabFile(image);
	//	            } catch (Exception e) {
	//	            }
	//	            name = image.getName(); 
	//	            width = (imgBytes[21]<<24) &0xFF  |
	//	                    (imgBytes[20]<<16) &0xFF  |
	//	                    (imgBytes[19]<<8)  &0xFF  |
	//	                    (imgBytes[18]<<0)  &0xFF;
	//
	//	            height =(imgBytes[25]<<24) &0xFF |
	//	                    (imgBytes[24]<<16) &0xFF |
	//	                    (imgBytes[23]<<8)  &0xFF |
	//	                    (imgBytes[22]<<0)  &0xFF; 
	//	        }
	//	     
	//	     /*
	//	      Return Format:
	//	      |Image width(uint16_t)|Image height(uint16_t)|Image Data (byte[])| */
	//	        public byte[] getBytes() {
	//
	//	            System.out.println("Flasher: Parsing: " + name);
	//
	//	            int offset =(imgBytes[13]<<24) &0xFF |
	//	                        (imgBytes[12]<<16) &0xFF |
	//	                        (imgBytes[11]<<8)  &0xFF |
	//	                        (imgBytes[10]<<0)  &0xFF;
	//
	//	            int imSize = width * height * 3;
	//
	//	            int rowBytes =  width * 3;
	//	            int skipCount = width % 4;
	//
	//	            int  pcRowSize = rowBytes + skipCount;
	//	            int deviceRowSize = rowBytes;
	//
	//	            byte pcRow[] = new byte[pcRowSize];  
	//	            byte deviceRow[] = new byte[deviceRowSize];
	//	            byte bmpData[] = new byte[pcRowSize*height];
	//
	//	            /* the bitmap data to send
	//	               to the device with special
	//	               width & height fields in it's header */
	//	            byte deviceData[] = new byte[pcRowSize*height+4];
	//	            deviceData[0] = (byte)((width>>8) & 255);
	//	            deviceData[1] = (byte)(width & 255);
	//	            deviceData[2] = (byte)((height>>8) & 255);
	//	            deviceData[3] = (byte)(height & 255); 
	//	            int deviceDataIndex = 4; //past the width and heigh in the header
	//
	//	            /* Read the bitmap data from the file into a local array */
	//	            System.arraycopy(imgBytes,offset,bmpData,0,(pcRowSize*height));                                                                                                                 
	//
	//	            /* Reverse the entire bitmap data */
	//	            byte tbmpData[] = new byte[bmpData.length];
	//	            for (int y = 0; y<bmpData.length; y++) {
	//	                tbmpData[y] = bmpData[(bmpData.length-1)-y];
	//	            }
	//	            System.arraycopy(tbmpData,0,bmpData,0,bmpData.length);
	//
	//	            int pcIndex=0;
	//	            int deviceIndex=0;
	//
	//	            /* for each row */
	//	            for (int row = 0; row< height; row++) {
	//	                /* make the indexes */
	//	                pcIndex = (row*pcRowSize);
	//	                deviceIndex = (row*deviceRowSize) + 3;
	//
	//	                /* load the pc row */
	//	                System.arraycopy(bmpData,pcIndex,pcRow,0,pcRowSize);
	//
	//	                /* build the device row, then reverse it */
	//	                System.arraycopy(pcRow,skipCount,deviceRow,0,deviceRowSize);
	//	                byte tRow[] = new byte[deviceRowSize];
	//	                for (int r = 0; r<deviceRowSize; r++) {
	//	                    tRow[r] = deviceRow[(deviceRowSize-1)-r];
	//	                }
	//	                System.arraycopy(tRow,0,deviceRow,0,deviceRowSize);
	//
	//	                /* Store the modified rows */
	//	                System.arraycopy(deviceRow,0,deviceData,deviceDataIndex,deviceRowSize);
	//	                deviceDataIndex+= deviceRowSize;
	//	            }
	//	            
	//	            return deviceData;
	//	        }
	//
	//	     }   
	
}
