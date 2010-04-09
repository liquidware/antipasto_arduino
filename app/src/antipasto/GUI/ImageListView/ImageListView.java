package antipasto.GUI.ImageListView;

import java.awt.Color;
import java.io.File;
import java.util.ArrayList;

import javax.swing.JList;

import antipasto.Util.*;

import java.awt.dnd.DropTarget;
import java.awt.dnd.DropTargetDragEvent;
import java.awt.dnd.DropTargetDropEvent;
import java.awt.dnd.DropTargetEvent;
import java.awt.dnd.DropTargetListener;

import javax.swing.*;
import java.awt.datatransfer.*;
import java.awt.datatransfer.DataFlavor;
import java.awt.datatransfer.Transferable;

import java.util.List;
import java.util.Iterator;
import java.net.*;
import java.awt.Component;
import java.awt.Dimension;
import java.math.*;
import javax.swing.TransferHandler;

import java.io.*;

public class ImageListView extends JList implements DropTargetListener {
    ArrayList imageFiles = new ArrayList();
    File imageDir;

    public ImageListView(File imagePath){
        setDirectory(imagePath);
    }

    public void setDirectory(File imagePath) {
        imageDir = imagePath;               //store the directory
        if (!imageDir.exists()) {
            imageDir.mkdir();               //create if doesn't exist
        }
        this.setTransferHandler(new ImageListPanelTransferHandler());
        this.loadList(imageDir.listFiles());
    }

    public File getDirectory() {
        return imageDir;
    }

    private void loadList(File[] files){
        this.setCellRenderer(new ImageCellRenderer());
        ImageListModel listModel = new ImageListModel(files);
        this.setModel(listModel);
        this.repaint();
    }

    public void removeSelected(){
        try {
            int index = this.getSelectedIndex();
            ImageListModel model = ((ImageListModel)this.getModel());
            model.remove(index);

        } catch (Exception ex) {
            ex.printStackTrace();
        }
        this.repaint();
    }

    public void dragEnter(DropTargetDragEvent dtde) {
    }

    public void dragExit(DropTargetEvent dte) {
    }

    public void dragOver(DropTargetDragEvent dtde) {
    }

    public void drop(DropTargetDropEvent dtde) {
    }

    public void dropActionChanged(DropTargetDragEvent dtde) {
    }

    /**
     * Add a file to the current directory
     *
     * @param file
     */
    public void addFile(File file) {
        String dest = imageDir.getPath() + File.separator + file.getName();

        try {
            System.out.println("Copying " + file.getName() + " to " + dest);
            FileCopy.copy(file.getPath(), dest);
        } catch (Exception ex) {
            System.out.println(ex.getMessage());
        }
    }

    //nested class
    private class ImageListPanelTransferHandler extends TransferHandler {
        public void TransferHandler(){

        }

        public boolean canImport(JComponent arg0, DataFlavor[] arg1) {
            return true;
        }

        public boolean importData(JComponent comp, Transferable t) {
            if (comp instanceof ImageListView) {
                DataFlavor[] flavors = t.getTransferDataFlavors();
                for (int i = 0; i < flavors.length; i++) {
                    DataFlavor flavor = flavors[i];
                    try {
                        if (flavor.equals(DataFlavor.javaFileListFlavor)) {

                            List l = (List) t.getTransferData(DataFlavor.javaFileListFlavor);
                            Iterator iter = l.iterator();
                            File dir = ((ImageListView)comp).getDirectory();

                            while (iter.hasNext()) {
                                File file = (File) iter.next();
                                // Now do something with the file...
                                ((ImageListView)comp).addFile(file);
                                ((ImageListView)comp).setDirectory(dir);   //Refresh the files
                            }
                            return true;
                        } else if (flavor.equals(DataFlavor.stringFlavor)) {
                            String fileOrURL = (String) t.getTransferData(flavor);
                            try {
                                URL url = new URL(fileOrURL);
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
                        System.out.println("IOError getting data: " + ex);
                    } catch (UnsupportedFlavorException e) {
                        System.out.println("Unsupported Flavor: " + e);
                    }
                }
                // If you get here, I didn't like the flavor.
                System.out.println("Dont support....");
                return false;
            }
            System.out.println("Couldn't load....");
            return false;
        }

    }

}
