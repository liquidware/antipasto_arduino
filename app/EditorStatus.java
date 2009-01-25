/* -*- mode: jde; c-basic-offset: 2; indent-tabs-mode: nil -*- */

/*
  Part of the Processing project - http://processing.org

  Copyright (c) 2004-05 Ben Fry and Casey Reas
  Copyright (c) 2001-04 Massachusetts Institute of Technology

  This program is free software; you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation; either version 2 of the License, or
  (at your option) any later version.

  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with this program; if not, write to the Free Software Foundation,
  Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
  
  $Id: EditorStatus.java 346 2007-10-06 20:26:45Z mellis $
*/

package processing.app;

import java.io.*;
import java.awt.*;
import java.awt.event.*;
import javax.swing.*;


/**
 * Panel just below the editing area that contains status messages.
 */
public class EditorStatus extends JPanel implements ActionListener {
  static Color bgcolor[];
  static Color fgcolor[];

  static final int NOTICE = 0;
  static final int ERR    = 1;
  static final int PROMPT = 2;
  static final int EDIT   = 3;
  static final int SERIAL = 4;

  static final int YES    = 1;
  static final int NO     = 2;
  static final int CANCEL = 3;
  static final int OK     = 4;
  static final int SEND   = 5;

  static final String NO_MESSAGE = "";

  Editor editor;

  int mode;
  String message;

  Font font;
  FontMetrics metrics;
  int ascent;

  Image offscreen;
  int sizeW, sizeH;
  int imageW, imageH;

  JButton yesButton;
  JButton noButton;
  JButton cancelButton;
  JButton okButton;
  JButton sendButton;
  JTextField editField;
  JTextField serialField;
  JComboBox serialRates;

  //Thread promptThread;
  int response;


  public EditorStatus(Editor editor) {
    this.editor = editor;
    empty();

    if (bgcolor == null) {
      bgcolor = new Color[5];
	  // Arduino 0003 switched to a blue color scheme to visually distinguish
	  // itself from Processing.  Because the image files for certain interface
	  // elements (e.g. buttons and tabs) are distributed with the application
	  // while the preference file that specifies the IDE colors is stored in
	  // the user's home directory and shared across all versions of Arduino,
	  // we need to hardcode certain colors here to match the images.
	  // Otherwise, users who used different multiple versions of the software
	  // would sometimes see colors that didn't match the interface elements.
	  // This is a hack and prevents users from customizing the IDE colors,
	  // however, it obviates the need to provide for version-specific
	  // preferences.
      //bgcolor[0] = Preferences.getColor("status.notice.bgcolor");
      bgcolor[0] = new Color(0x54, 0x91, 0x9e);
      bgcolor[1] = Preferences.getColor("status.error.bgcolor");
      bgcolor[2] = Preferences.getColor("status.prompt.bgcolor");
      bgcolor[3] = Preferences.getColor("status.prompt.bgcolor");
      bgcolor[4] = new Color(0x54, 0x91, 0x9e);

      fgcolor = new Color[5];
      fgcolor[0] = Preferences.getColor("status.notice.fgcolor");
      fgcolor[1] = Preferences.getColor("status.error.fgcolor");
      fgcolor[2] = Preferences.getColor("status.prompt.fgcolor");
      fgcolor[3] = Preferences.getColor("status.prompt.fgcolor");
      fgcolor[4] = Preferences.getColor("status.notice.fgcolor");
    }
  }


  public void empty() {
    mode = NOTICE;
    message = NO_MESSAGE;
    //update();
    repaint();
  }


  public void notice(String message) {
    mode = NOTICE;
    this.message = message;
    //update();
    repaint();
  }

  public void unnotice(String unmessage) {
    if (message.equals(unmessage)) empty();
  }


  public void error(String message) {
    mode = ERR;
    this.message = message;
    repaint();
  }


  public void prompt(String message) {
    mode = PROMPT;
    this.message = message;

    response = 0;
    yesButton.setVisible(true);
    noButton.setVisible(true);
    cancelButton.setVisible(true);
    yesButton.requestFocus();

    repaint();
  }


  // prompt has been handled, re-hide the buttons
  public void unprompt() {
    yesButton.setVisible(false);
    noButton.setVisible(false);
    cancelButton.setVisible(false);
    empty();
  }


  public void edit(String message, String dflt) {
    mode = EDIT;
    this.message = message;

    response = 0;
    okButton.setVisible(true);
    cancelButton.setVisible(true);
    editField.setVisible(true);
    editField.setText(dflt);
    editField.selectAll();
    editField.requestFocus();

    repaint();
  }

  public void unedit() {
    okButton.setVisible(false);
    cancelButton.setVisible(false);
    editField.setVisible(false);
    empty();
  }
  
  public void serial()
  {
    mode = SERIAL;
    this.message = NO_MESSAGE;
    
    sendButton.setVisible(true);
    serialRates.setVisible(true);
    serialField.setVisible(true);
    serialField.setText("");
    serialField.requestFocus();

    repaint();
  }
  
  public void unserial()
  {
    sendButton.setVisible(false);
    serialField.setVisible(false);
    serialRates.setVisible(false);
    empty();
  }


  /*
  public void update() {
    Graphics g = this.getGraphics();
    try {
      setBackground(bgcolor[mode]);
    } catch (NullPointerException e) { } // if not ready yet
    if (g != null) paint(g);
  }

  public void update(Graphics g) {
    paint(g);
  }
  */


  public void paintComponent(Graphics screen) {
    //if (screen == null) return;
    if (yesButton == null) setup();

    //System.out.println("status.paintComponent");

    Dimension size = getSize();
    if ((size.width != sizeW) || (size.height != sizeH)) {
      // component has been resized

      if ((size.width > imageW) || (size.height > imageH)) {
        // nix the image and recreate, it's too small
        offscreen = null;

      } else {
        // who cares, just resize
        sizeW = size.width;
        sizeH = size.height;
        setButtonBounds();
      }
    }

    if (offscreen == null) {
      sizeW = size.width;
      sizeH = size.height;
      setButtonBounds();
      imageW = sizeW;
      imageH = sizeH;
      offscreen = createImage(imageW, imageH);
    }

    Graphics g = offscreen.getGraphics();
    if (font == null) {
      font = Preferences.getFont("status.font");
      //new Font("SansSerif", Font.PLAIN, 12));
      g.setFont(font);
      metrics = g.getFontMetrics();
      ascent = metrics.getAscent();
    }

    //setBackground(bgcolor[mode]);  // does nothing

    g.setColor(bgcolor[mode]);
    g.fillRect(0, 0, imageW, imageH);

    g.setColor(fgcolor[mode]);
    g.setFont(font); // needs to be set each time on osx
    g.drawString(message, Preferences.GUI_SMALL, (sizeH + ascent) / 2);

    screen.drawImage(offscreen, 0, 0, null);
  }


  protected void setup() {
    if (yesButton == null) {
      yesButton    = new JButton(Preferences.PROMPT_YES);
      noButton     = new JButton(Preferences.PROMPT_NO);
      cancelButton = new JButton(Preferences.PROMPT_CANCEL);
      okButton     = new JButton(Preferences.PROMPT_OK);
      sendButton   = new JButton(Preferences.PROMPT_SEND);

      // !@#(* aqua ui #($*(( that turtle-neck wearing #(** (#$@)(
      // os9 seems to work if bg of component is set, but x still a bastard
      if (Base.isMacOS()) {
        yesButton.setBackground(bgcolor[PROMPT]);
        noButton.setBackground(bgcolor[PROMPT]);
        cancelButton.setBackground(bgcolor[PROMPT]);
        okButton.setBackground(bgcolor[PROMPT]);
        sendButton.setBackground(bgcolor[SERIAL]);
      }
      setLayout(null);

      yesButton.addActionListener(this);
      noButton.addActionListener(this);
      cancelButton.addActionListener(this);
      okButton.addActionListener(this);
      sendButton.addActionListener(this);

      add(yesButton);
      add(noButton);
      add(cancelButton);
      add(okButton);
      add(sendButton);

      yesButton.setVisible(false);
      noButton.setVisible(false);
      cancelButton.setVisible(false);
      okButton.setVisible(false);
      sendButton.setVisible(false);

      editField = new JTextField();
      editField.addActionListener(this);

      //if (Base.platform != Base.MACOSX) {
      editField.addKeyListener(new KeyAdapter() {
          // no-op implemented because of a jikes bug
          //protected void noop() { }

          //public void keyPressed(KeyEvent event) {
          //System.out.println("pressed " + event + "  " + KeyEvent.VK_SPACE);
          //}

          // use keyTyped to catch when the feller is actually
          // added to the text field. with keyTyped, as opposed to
          // keyPressed, the keyCode will be zero, even if it's
          // enter or backspace or whatever, so the keychar should
          // be used instead. grr.
          public void keyTyped(KeyEvent event) {
            //System.out.println("got event " + event + "  " +
            // KeyEvent.VK_SPACE);
            int c = event.getKeyChar();
            
            if (mode == EDIT) {
              if (c == KeyEvent.VK_ENTER) {  // accept the input
                String answer = editField.getText();
                editor.sketch.nameCode(answer);
                unedit();
                event.consume();

                // easier to test the affirmative case than the negative
              } else if ((c == KeyEvent.VK_BACK_SPACE) ||
                         (c == KeyEvent.VK_DELETE) ||
                         (c == KeyEvent.VK_RIGHT) ||
                         (c == KeyEvent.VK_LEFT) ||
                         (c == KeyEvent.VK_UP) ||
                         (c == KeyEvent.VK_DOWN) ||
                         (c == KeyEvent.VK_HOME) ||
                         (c == KeyEvent.VK_END) ||
                         (c == KeyEvent.VK_SHIFT)) {
                //System.out.println("nothing to see here");
                //noop();

              } else if (c == KeyEvent.VK_ESCAPE) {
                unedit();
                editor.buttons.clear();
                event.consume();

              } else if (c == KeyEvent.VK_SPACE) {
                //System.out.println("got a space");
                // if a space, insert an underscore
                //editField.insert("_", editField.getCaretPosition());
                /* tried to play nice and see where it got me
                   editField.dispatchEvent(new KeyEvent(editField,
                   KeyEvent.KEY_PRESSED,
                   System.currentTimeMillis(),
                   0, 45, '_'));
                */
                //System.out.println("start/end = " +
                //                 editField.getSelectionStart() + " " +
                //                 editField.getSelectionEnd());
                String t = editField.getText();
                //int p = editField.getCaretPosition();
                //editField.setText(t.substring(0, p) + "_" + t.substring(p));
                //editField.setCaretPosition(p+1);
                int start = editField.getSelectionStart();
                int end = editField.getSelectionEnd();
                editField.setText(t.substring(0, start) + "_" +
                                  t.substring(end));
                editField.setCaretPosition(start+1);
                //System.out.println("consuming event");
                event.consume();

              } else if ((c == '_') || (c == '.') ||  // allow .pde and .java
                         ((c >= 'A') && (c <= 'Z')) ||
                         ((c >= 'a') && (c <= 'z'))) {
                // everything fine, catches upper and lower
                //noop();

              } else if ((c >= '0') && (c <= '9')) {
                // getCaretPosition == 0 means that it's the first char
                // and the field is empty.
                // getSelectionStart means that it *will be* the first
                // char, because the selection is about to be replaced
                // with whatever is typed.
                if ((editField.getCaretPosition() == 0) ||
                    (editField.getSelectionStart() == 0)) {
                  // number not allowed as first digit
                  //System.out.println("bad number bad");
                  event.consume();
                }
              } else {
                event.consume();
                //System.out.println("code is " + code + "  char = " + c);
              }
            } 
            //System.out.println("code is " + code + "  char = " + c);
          }
        });
      add(editField);
      editField.setVisible(false);
            
      serialField = new JTextField();
      serialField.addActionListener(this);

      serialField.addKeyListener(new KeyAdapter() {
        public void keyTyped(KeyEvent event) {
          int c = event.getKeyChar();
            
          if (c == KeyEvent.VK_ENTER) {  // accept the input
            editor.serialPort.write(serialField.getText());
            event.consume();
            serialField.setText("");
          } 
        }});
        
      add(serialField);
      serialField.setVisible(false);

      String[] serialRateStrings = {
        "300","1200","2400","4800","9600","14400",
        "19200","28800","38400","57600","115200"
      };
      
      serialRates = new JComboBox();

      if (Base.isMacOS())
        serialRates.setBackground(bgcolor[SERIAL]);
        
      for (int i = 0; i < serialRateStrings.length; i++)
        serialRates.addItem(serialRateStrings[i] + " baud");

      serialRates.setSelectedItem(
        Preferences.get("serial.debug_rate") + " baud");
      serialRates.addActionListener(this);      
      add(serialRates);
      serialRates.setVisible(false);
    }
  }


  protected void setButtonBounds() {
    int top = (sizeH - Preferences.BUTTON_HEIGHT) / 2;
    int eachButton = Preferences.GUI_SMALL + Preferences.BUTTON_WIDTH;

    int cancelLeft = sizeW      - eachButton;
    int noLeft     = cancelLeft - eachButton;
    int yesLeft    = noLeft     - eachButton;

    yesButton.setLocation(yesLeft, top);
    noButton.setLocation(noLeft, top);
    cancelButton.setLocation(cancelLeft, top);
    editField.setLocation(yesLeft - Preferences.BUTTON_WIDTH, top);
    serialField.setLocation(yesLeft - Preferences.BUTTON_WIDTH, top);
    okButton.setLocation(noLeft, top);
    serialRates.setLocation(0, top);
    sendButton.setLocation(cancelLeft, top);

    yesButton.setSize(      Preferences.BUTTON_WIDTH, Preferences.BUTTON_HEIGHT);
    noButton.setSize(       Preferences.BUTTON_WIDTH, Preferences.BUTTON_HEIGHT);
    cancelButton.setSize(   Preferences.BUTTON_WIDTH, Preferences.BUTTON_HEIGHT);
    okButton.setSize(       Preferences.BUTTON_WIDTH, Preferences.BUTTON_HEIGHT);
    sendButton.setSize(     Preferences.BUTTON_WIDTH, Preferences.BUTTON_HEIGHT);
    serialRates.setSize(  3*Preferences.BUTTON_WIDTH/2, Preferences.BUTTON_HEIGHT);
    editField.setSize(    2*Preferences.BUTTON_WIDTH, Preferences.BUTTON_HEIGHT);
    serialField.setSize(  3*Preferences.BUTTON_WIDTH, Preferences.BUTTON_HEIGHT);
  }


  public Dimension getPreferredSize() {
    return getMinimumSize();
  }

  public Dimension getMinimumSize() {
    return new Dimension(300, Preferences.GRID_SIZE);
  }

  public Dimension getMaximumSize() {
    return new Dimension(3000, Preferences.GRID_SIZE);
  }


  public void actionPerformed(ActionEvent e) {
    if (e.getSource() == noButton) {
      // shut everything down, clear status, and return
      unprompt();
      // don't need to save changes
      editor.checkModified2();

    } else if (e.getSource() == yesButton) {
      // answer was in response to "save changes?"
      unprompt();
      editor.handleSave(true);
      editor.checkModified2();

    } else if (e.getSource() == cancelButton) {
      // don't do anything, don't continue with checkModified2
      if (mode == PROMPT) unprompt();
      else if (mode == EDIT) unedit();
      editor.buttons.clear();

    } else if (e.getSource() == okButton) {
      // answering to "save as..." question
      String answer = editField.getText();
      //editor.handleSaveAs2(answer);
      editor.sketch.nameCode(answer);
      unedit();
    } else if (e.getSource() == sendButton) {
      editor.serialPort.write(serialField.getText());
      //arduinoFlasher();
      //ARMDownloader();
      serialField.setText("");
    } else if (e.getSource() == serialRates) {
      String wholeString = (String) serialRates.getSelectedItem();
      String rateString = wholeString.substring(0, wholeString.indexOf(' '));
      int rate = Integer.parseInt(rateString);
      Preferences.set("serial.debug_rate", rateString);
      editor.serialPort.dispose();
      try {
        editor.serialPort = new Serial(true);
      } catch (SerialException err) {
        editor.error(err);
      }
    }
  }

  //*******************************************************************************
  // armDownloader
  // 
  // by Chris Ladden
  //  
  public void ARMDownloader() {

        editor.serialPort.monitor = false; //disable the console reads
        System.out.println("ARM Downloader\n Written By: Chris Ladden");

        editor.serialPort.clear(); //purge
        editor.serialPort.write("S"); //send the Identify command
        System.out.println("Checking for ARM device...");

        try{ Thread.sleep(200); } catch (Exception e) { }
        String id = editor.serialPort.readString();
        if (id.indexOf("lpc2368") < 0) {
            System.out.println("Device not ready");
            return;
        } else {
          System.out.println("Device: " + id);
        }

        System.out.println("Erasing Chip");
        editor.serialPort.write("e"); //Chip Erase command
        while (editor.serialPort.readChar() != '\r') { ; } //wait

        for (int ii=8; ii<22; ii++) {
          byte data[] = new byte[4096];
          int address = (ii * 4096)+( (ii-8) * 0x8000);
          data[0] = (byte)(ii& 0xFF);     //assign dumy data
          data[4095] = (byte)(ii& 0xFF); 
          
          System.out.print("Writing Address: " + address);
          editor.serialPort.write("A"); //Address set command
          editor.serialPort.write((char)((address >> 24) & 0xFF)); // high byte of the address
          editor.serialPort.write((char)((address >> 16) & 0xFF)); // 2nd byte of the address
          editor.serialPort.write((char)((address >> 8) & 0xFF));  // 1st byte of the address
          editor.serialPort.write((char)(address & 0xFF));         // low byte of the address 
          while (editor.serialPort.readChar() != '\r') { ; } //wait

          editor.serialPort.write("B"); //Buffer load command
          editor.serialPort.write((char)((4096 >> 8) & 0xFF)); // high byte of the size
          editor.serialPort.write((char)(4096 & 0xFF)); // low byte of the size 
          System.out.println(" with: " + 4096 + " bytes.");
          editor.serialPort.write(data); //send the sector

          while (editor.serialPort.readChar() != '\r') { ; } //wait
        }
        
        System.out.println("Exiting");
        editor.serialPort.write("E"); //Exit command
        while (editor.serialPort.readChar() != '\r') { ; } //wait

        System.out.println("Done.");

  }

  

    /*
   * List of Image files for uploading
   * @return Array of .bmp files as File objects
   */
  private File[] getBMPFiles(File folder)
  {
    FileFilter onlyBMPFiles = new FileFilter() {
      public boolean accept(File file) {
        return (file.getName()).endsWith(".bmp");
      }
    };
    return folder.listFiles(onlyBMPFiles);
  }

  /*
  Sends the files located
  in the data folder to a device
  */
   public void arduinoFlasher(){
      
      /* Design outline
      look for TouchShield
      Find files in the data folder 
      parse files by type
      build images
      build files
      build header
      build data
      split into pages
      send data
      */

     Flash flashData = new Flash(2000000,528*2); //2 meg flash chip, 2 pages for header

     /* Find image files in the data folder */
     File[] imageFiles = getBMPFiles(editor.sketch.dataFolder);

     for(int i=0;i<imageFiles.length;i++) {
       DeviceBMP bmp = new DeviceBMP(imageFiles[i]);
       
       /* Add file to the flash */
       flashData.addFile(bmp.name,bmp.getBytes());
     }

      String flasherId;
      
      editor.serialPort.monitor = false; //disable the console reads
      System.out.println("Arduino Flasher: Starting");
      
      editor.serialPort.clear(); //purge
      editor.serialPort.write("I"); //send the Identify command
      System.out.println("Arduino Flasher: Checking for device...");

      try{ Thread.sleep(200); } catch (Exception e) { }

      /* Look for the ID of the device */
      if (editor.serialPort.readString().indexOf("Flasher") < 0) {
         System.out.println("Arduino Flasher: Device not ready");
         return;
      }
      System.out.println("Arduino Flasher: Found Device");

      eraseChip();

      try{ Thread.sleep(200); } catch (Exception e) { }
      
      byte p[] = flashData.getPage(0);
      for(int y=0; y<528; y++) {
        System.out.print((int)(p[y]&0xFF)); System.out.print(" ");
      }

      /* Write each page */
      for (int pageNum=0; pageNum<flashData.getNumPages(); pageNum++) {
        writePage(flashData.getPage(pageNum), pageNum);
      }
      
      /* Exit Flasher */
      close();

      System.out.println("Arduino Flasher: Complete!");
   }
   

   private int close(){

     /* Send close command */
     System.out.println("Arduino Flasher: Closing");
     editor.serialPort.clear(); //purge
     editor.serialPort.write("E");
     while (editor.serialPort.readChar() != 'D') { ; } //wait
     System.out.println("Arduino Flasher: Closed");

     return 1;

   }

   private int eraseChip() {

     /* Send Erase Chip command */
     System.out.println("Arduino Flasher: Erasing Chip");
     editor.serialPort.clear(); //purge
     editor.serialPort.write("R");
     while (editor.serialPort.readChar() != 'D') { ; } //wait
     System.out.println("Arduino Flasher: Chip erase complete");

     return 1;
   }

   private int writePage(byte buff[], int pageNum){
     
     /* Calculate checksum */
     byte checksum=0;
     byte verify=0;
     for (int i=0; i< buff.length; i++) {
       checksum += buff[i];
     }
     checksum = (byte)(checksum ^ 255);
     checksum +=1;
     
     /* Send the STORE command */
     System.out.println("Arduino Flasher: Sending STORE");
     editor.serialPort.clear(); //purge
     editor.serialPort.write("S");
     while (editor.serialPort.readChar() != 'D') { ; } //wait

     /* Send the page number */
     System.out.println("Arduino Flasher: Sending pagenum: " + pageNum);
     editor.serialPort.clear(); //purge
     editor.serialPort.write((char)(pageNum & 255));
     editor.serialPort.write((char)((pageNum >> 8) & 255));
     while (editor.serialPort.readChar() != 'D') { ; } //wait

     /* Send the page data */
     System.out.println("Arduino Flasher: Sending page data");
     editor.serialPort.clear(); //purge
     editor.serialPort.write(buff);
     //for (int i=0; i< buff.length; i++) {
     //  editor.serialPort.write(buff[i]);
       //System.out.println(i);
     //  try{ Thread.sleep(5); } catch (Exception e) { }
     //}
     while (editor.serialPort.readChar() != 'D') { ; } //wait

     /* Ask for checksum */
     System.out.println("Arduino Flasher: asking for checksum");
     editor.serialPort.clear(); //purge
     editor.serialPort.write("C");
     while(editor.serialPort.available() < 1) { ; } 
     verify = (byte)editor.serialPort.readChar();

     if (verify != checksum) {
        System.out.println("Arduino Flasher: checksum error at " + pageNum + "Expected: " +checksum + "Got: " + verify);
        return -1;
     }
     
     System.out.println("Arduino Flasher: Verified: " + pageNum);

     /* No errors */
     return 1;
   }

   private class Flash {
       byte flashChip[];
       int pageSize = 528;
       int flashHeaderIndex=0;
       int flashChipIndex;

       public Flash(int flashSize, int headerSize){
         flashChip = new byte[flashSize];
         flashChipIndex = headerSize; 
       }
       
       /* Add a file to the flash data
          The file name you want to add, and the file byte data.
          This will add it to the header and copy data */
       public int addFile(String file, byte[] fileBytes){
         try {

           /* Copy file to Header */
           /* Copy Name */
           byte fileNameBytes[] = file.getBytes();
           System.arraycopy(fileNameBytes,0,flashChip,flashHeaderIndex,fileNameBytes.length);
           flashHeaderIndex+=fileNameBytes.length;
           flashChip[flashHeaderIndex] = 0; //terminate the file name string
           flashHeaderIndex++;
    
           /* Copy the data offset */
           flashChip[flashHeaderIndex] = (byte)((flashChipIndex>>24) & 0xFF);
           flashHeaderIndex++;
           flashChip[flashHeaderIndex] = (byte)((flashChipIndex>>16) & 0xFF);
           flashHeaderIndex++;
           flashChip[flashHeaderIndex] = (byte)((flashChipIndex>>8) & 0xFF);
           flashHeaderIndex++;
           flashChip[flashHeaderIndex] = (byte)((flashChipIndex) & 0xFF);
           flashHeaderIndex++;
           
           /* Copy file data */
           System.arraycopy(fileBytes,0,flashChip,flashChipIndex,fileBytes.length);
           flashChipIndex+= fileBytes.length;

           return 1;
         } catch (Exception e) {
           //Do something graceful here
           return -1;
         }
       }

       /* Return 528 bytes
          of flash data. */
       public byte[] getPage(int pageNum){
          byte page[] = new byte[pageSize];
          System.arraycopy(flashChip,pageNum*pageSize,page,0,page.length); //copy the page
          return page;
       }

       /* gets the current
          page total */
       public int getNumPages() {
         return (flashChipIndex / pageSize) + 1;
       }


     }


   /* A class for a 24-bit bitmap image
      Formats the image for fast display
      on the TouchShield. */
   private class DeviceBMP {
     public int width;
     public int height;
     public String name;
     File image;
     byte imgBytes[];
     
        public DeviceBMP(File image) {
            this.image = image;  
            try {
                imgBytes = Base.grabFile(image);
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

}
