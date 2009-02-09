package antipasto.GUI.Panels;

import java.awt.Color;
import java.awt.datatransfer.DataFlavor;
import java.awt.datatransfer.UnsupportedFlavorException;
import java.awt.dnd.DropTarget;
import java.awt.dnd.DropTargetDragEvent;
import java.awt.dnd.DropTargetDropEvent;
import java.awt.dnd.DropTargetEvent;
import java.awt.dnd.DropTargetListener;
import java.io.IOException;
import java.util.TooManyListenersException;

import javax.swing.JList;

public class NewGadgetList extends JList implements DropTargetListener{

	DropTarget dt = new DropTarget();
	public NewGadgetList()
	{
		try {
			dt.addDropTargetListener(this);
		} catch (TooManyListenersException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
		this.setDropTarget(dt);
		this.setTransferHandler(new ModuleTransferHandler());
		this.setDragEnabled(true);
		this.setBackground(Color.black);
	}
	public void dragEnter(DropTargetDragEvent dtde) {
	}

	public void dragExit(DropTargetEvent dte) {
	}

	public void dragOver(DropTargetDragEvent dtde) {
	}

	public void drop(DropTargetDropEvent dtde) {
		dtde.acceptDrop(1);
		this.getTransferHandler().importData(null, dtde.getTransferable());
	}

	public void dropActionChanged(DropTargetDragEvent dtde) {
		// TODO Auto-generated method stub
		
	}

}
