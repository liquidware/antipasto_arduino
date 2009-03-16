package antipasto.GUI.GadgetListView;

import java.awt.Component;

import javax.swing.DefaultListCellRenderer;
import javax.swing.ImageIcon;
import javax.swing.JLabel;
import javax.swing.JList;

import antipasto.Interfaces.IModule;

public class LibraryGadgetCellRenderer extends DefaultListCellRenderer  {
	public Component getListCellRendererComponent(JList list, Object value, int index,
	            boolean isSelected, boolean hasFocus) {
	JLabel label = (JLabel)super.getListCellRendererComponent(list,
	             value,
	             index,
	             isSelected,
	             hasFocus);
	if (value instanceof IModule) {
	IModule gadget = (IModule)value;
	ImageIcon icon = new ImageIcon(gadget.getImage());
	
	
	label.setIcon(icon);
	label.setText("");
	//label.setText(gadget.getName());
	} else {
	// Clear old icon; needed in 1st release of JDK 1.2
	label.setIcon(null);
	}
	return(label);
	}
}
