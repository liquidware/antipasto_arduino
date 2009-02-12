package antipasto.GUI.GadgetListView;

import java.awt.Toolkit;
import java.awt.datatransfer.DataFlavor;
import java.awt.datatransfer.Transferable;
import java.awt.datatransfer.UnsupportedFlavorException;
import java.io.File;
import java.io.IOException;
import java.net.MalformedURLException;
import java.net.URL;
import java.util.Iterator;
import java.util.List;

import javax.swing.JComponent;
import javax.swing.TransferHandler;
import javax.swing.event.EventListenerList;


class GadgetWindowTransferHandler extends TransferHandler {

  /**
   * @see javax.swing.TransferHandler#canImport(javax.swing.JComponent,
   *      java.awt.datatransfer.DataFlavor[])
   */
  private EventListenerList transferListenerList = new EventListenerList();
    
  public boolean canImport(JComponent arg0, DataFlavor[] arg1) {
	  if(arg1 != null){
	    for (int i = 0; i < arg1.length; i++) {
	      DataFlavor flavor = arg1[i];
	      if (flavor.equals(DataFlavor.javaFileListFlavor)) {
	        System.out.println("canImport: JavaFileList FLAVOR: " + flavor);
	        return true;
	      }
	      if (flavor.equals(DataFlavor.stringFlavor)) {
	        System.out.println("canImport: String FLAVOR: " + flavor);
	        return true;
	      }
	      System.err.println("canImport: Rejected Flavor: " + flavor);
	    }
	  }
    // Didn't find any that match, so:
    return false;
  }

  /**
   * Do the actual import.
   *
   * @see javax.swing.TransferHandler#importData(javax.swing.JComponent,
   *      java.awt.datatransfer.Transferable)
   */
  public boolean importData(JComponent comp, Transferable t) {
    DataFlavor[] flavors = t.getTransferDataFlavors();
    System.out.println("Trying to import:" + t);
    System.out.println("... which has " + flavors.length + " flavors.");
    for (int i = 0; i < flavors.length; i++) {
      DataFlavor flavor = flavors[i];
      try {
        if (flavor.equals(DataFlavor.javaFileListFlavor)) {
          System.out.println("importData: FileListFlavor");

          List l = (List) t
              .getTransferData(DataFlavor.javaFileListFlavor);
          Iterator iter = l.iterator();
          while (iter.hasNext()) {
            File file = (File) iter.next();
            System.out.println("GOT FILE: "
                + file.getCanonicalPath());
              this.fireTransferEvent(new FileTransferObject(this, file));            
          }
          return true;
        } else if (flavor.equals(DataFlavor.stringFlavor)) {
          System.out.println("importData: String Flavor");
          String fileOrURL = (String) t.getTransferData(flavor);
          System.out.println("GOT STRING: " + fileOrURL);
          try {
            URL url = new URL(fileOrURL);
            System.out.println("Valid URL: " + url.toString());
            // Do something with the contents...
            return true;
          } catch (MalformedURLException ex) {
            System.err.println("Not a valid URL");
            return false;
          }
          // now do something with the String.

        } else {
          System.out.println("importData rejected: " + flavor);
          // Don't return; try next flavor.
        }
      } catch (IOException ex) {
        System.err.println("IOError getting data: " + ex);
      } catch (UnsupportedFlavorException e) {
        System.err.println("Unsupported Flavor: " + e);
      }
    }
    // If you get here, I didn't like the flavor.
    Toolkit.getDefaultToolkit().beep();
    return false;
  }

    public void addTransferFileListener(IGadgetWindowTransferFileEventListener listener){
        this.transferListenerList.add(IGadgetWindowTransferFileEventListener.class, listener);
    }

    public void removeTransferFileListener(IGadgetWindowTransferFileEventListener listener){
        this.transferListenerList.remove(IGadgetWindowTransferFileEventListener.class, listener);
    }

    private void fireTransferEvent(FileTransferObject evObj){
        Object[] listeners = transferListenerList.getListenerList();
                    // Each listener occupies two elements - the first is the listener class
                    // and the second is the listener instance
                    for (int i=0; i<listeners.length; i+=2) {
                        if (listeners[i]== IGadgetWindowTransferFileEventListener.class) {
                            ((IGadgetWindowTransferFileEventListener)listeners[i+1]).onFileTransfer(evObj);
                        }
                    }
    }
}
