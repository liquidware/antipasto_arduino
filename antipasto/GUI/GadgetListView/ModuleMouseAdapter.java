package antipasto.GUI.GadgetListView;

import java.awt.event.MouseAdapter;
import java.awt.event.MouseEvent;

import javax.swing.JComponent;
import javax.swing.TransferHandler;

public class ModuleMouseAdapter extends MouseAdapter{
	public void mousePressed(MouseEvent e) {
	      JComponent c = (JComponent) e.getSource();
	      TransferHandler handler = c.getTransferHandler();
	      handler.exportAsDrag(c, e, TransferHandler.COPY);
	    }
}
