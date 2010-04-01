package processing.app;

import java.io.File;

public class DeviceBMP {
    public int width;
    public int height;
    public String name;
    File image;
    byte imgBytes[];

       public DeviceBMP(File image) {
           this.image = image;
           try {
               imgBytes = Base.loadBytesRaw(image);
           } catch (Exception e) {
           }
           name = image.getName();
           width = (imgBytes[21]<<24) &0xFF  |
                   (imgBytes[20]<<16) &0xFF  |
                   (imgBytes[19]<<8)  &0xFF  |
                   (imgBytes[18]<<0)  &0xFF;

           height =(imgBytes[25]<<24) &0xFF |
                   (imgBytes[24]<<16) &0xFF |
                   (imgBytes[23]<<8)  &0xFF |
                   (imgBytes[22]<<0)  &0xFF;
       }

    /*
     Return Format:
     |Image width(uint16_t)|Image height(uint16_t)|Image Data (byte[])| */
       public byte[] getBytes() {

           System.out.println("Flasher: Parsing: " + name);

           int offset =(imgBytes[13]<<24) &0xFF |
                       (imgBytes[12]<<16) &0xFF |
                       (imgBytes[11]<<8)  &0xFF |
                       (imgBytes[10]<<0)  &0xFF;

           int imSize = width * height * 3;

           int rowBytes =  width * 3;
           int skipCount = width % 4;

           int  pcRowSize = rowBytes + skipCount;
           int deviceRowSize = rowBytes;

           byte pcRow[] = new byte[pcRowSize];
           byte deviceRow[] = new byte[deviceRowSize];
           byte bmpData[] = new byte[pcRowSize*height];

           /* the bitmap data to send
              to the device with special
              width & height fields in it's header */
           byte deviceData[] = new byte[pcRowSize*height+4];
           deviceData[0] = (byte)((width>>8) & 255);
           deviceData[1] = (byte)(width & 255);
           deviceData[2] = (byte)((height>>8) & 255);
           deviceData[3] = (byte)(height & 255);
           int deviceDataIndex = 4; //past the width and heigh in the header

           /* Read the bitmap data from the file into a local array */
           System.arraycopy(imgBytes,offset,bmpData,0,(pcRowSize*height));

           /* Reverse the entire bitmap data */
           byte tbmpData[] = new byte[bmpData.length];
           for (int y = 0; y<bmpData.length; y++) {
               tbmpData[y] = bmpData[(bmpData.length-1)-y];
           }
           System.arraycopy(tbmpData,0,bmpData,0,bmpData.length);

           int pcIndex=0;
           int deviceIndex=0;

           /* for each row */
           for (int row = 0; row< height; row++) {
               /* make the indexes */
               pcIndex = (row*pcRowSize);
               deviceIndex = (row*deviceRowSize) + 3;

               /* load the pc row */
               System.arraycopy(bmpData,pcIndex,pcRow,0,pcRowSize);

               /* build the device row, then reverse it */
               System.arraycopy(pcRow,skipCount,deviceRow,0,deviceRowSize);
               byte tRow[] = new byte[deviceRowSize];
               for (int r = 0; r<deviceRowSize; r++) {
                   tRow[r] = deviceRow[(deviceRowSize-1)-r];
               }
               System.arraycopy(tRow,0,deviceRow,0,deviceRowSize);

               /* Store the modified rows */
               System.arraycopy(deviceRow,0,deviceData,deviceDataIndex,deviceRowSize);
               deviceDataIndex+= deviceRowSize;
           }

           return deviceData;
       }

    }
