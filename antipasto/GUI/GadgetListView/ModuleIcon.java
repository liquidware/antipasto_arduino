package antipasto.GUI.GadgetListView;

import java.awt.datatransfer.DataFlavor;
import java.awt.datatransfer.Transferable;
import java.awt.datatransfer.UnsupportedFlavorException;
import java.awt.dnd.DnDConstants;
import java.awt.dnd.DragGestureEvent;
import java.awt.dnd.DragGestureListener;
import java.awt.dnd.DragSource;
import java.awt.dnd.DragSourceDragEvent;
import java.awt.dnd.DragSourceDropEvent;
import java.awt.dnd.DragSourceEvent;
import java.awt.dnd.DragSourceListener;
import java.awt.event.MouseEvent;
import java.awt.event.MouseListener;
import java.io.IOException;

import javax.swing.ImageIcon;
import javax.swing.JComponent;
import javax.swing.JLabel;
import javax.swing.TransferHandler;
import javax.swing.event.EventListenerList;

import antipasto.Interfaces.IModule;

public class ModuleIcon extends JLabel implements Transferable, DragGestureListener, DragSourceListener {
	IModule internalModule;
	EventListenerList listeners = new EventListenerList();
	DragSource ds;
	GadgetList list;
	public ModuleIcon(IModule module, GadgetList list){
		super(new ImageIcon(module.getImage()));
		internalModule = module;
		ds = new DragSource();
		ds.createDefaultDragGestureRecognizer(
                this, DnDConstants.ACTION_COPY_OR_MOVE, this);
		this.list = list;
		this.addMouseListener(new MouseListener(){
			public void mouseClicked(MouseEvent arg0) {
				fireSelectedEvent();
				System.out.println("Firing selection event");
			}
			public void mouseEntered(MouseEvent arg0) {
			}
			public void mouseExited(MouseEvent arg0) {
			}
			public void mousePressed(MouseEvent e) {
				JComponent jc = (JComponent)e.getSource();
		        TransferHandler th = jc.getTransferHandler();
//		        th.exportAsDrag(jc, e, TransferHandler.COPY);
		    }
			public void mouseReleased(MouseEvent arg0) {
				
			}
		});
	}
	
	public IModule getModule(){
		return this.internalModule;
	}
	
	public void addSelectionListener(ISelectedItemListener item){
		listeners.add(ISelectedItemListener.class, item);
	}
	
	private void fireSelectedEvent(){
		Object[] listeners = this.listeners.getListenerList();
	     // loop through each listener and pass on the event if needed
	     int numListeners = listeners.length;
	     for (int i = 0; i<numListeners; i+=2) 
	     {
	          if (listeners[i]==ISelectedItemListener.class) 
	          {
	               // pass the event to the listeners event dispatch method
	                ((ISelectedItemListener)listeners[i+1]).onSelected(this);
	          }            
	     }

	}

	public Object getTransferData(DataFlavor arg0)
			throws UnsupportedFlavorException, IOException {
		return this.internalModule;
	}

	public DataFlavor[] getTransferDataFlavors() {
		return null;
	}

	public boolean isDataFlavorSupported(DataFlavor arg0) {
		return false;
	}

	public void dragGestureRecognized(DragGestureEvent arg0) {
		Transferable t = this;
        ds.startDrag(arg0, DragSource.DefaultCopyDrop, t, this);

	}

	public void dragDropEnd(DragSourceDropEvent arg0) {
		int x = arg0.getX();
		int y = arg0.getY();
		if(list.bounds().inside(x, y)){
			list.doImportDragDrop(this.internalModule);
		}
	}

	public void dragEnter(DragSourceDragEvent arg0) {
		
	}

	public void dragExit(DragSourceEvent arg0) {
		
	}

	public void dragOver(DragSourceDragEvent arg0) {
		
	}

	public void dropActionChanged(DragSourceDragEvent arg0) {
		
	}
}
