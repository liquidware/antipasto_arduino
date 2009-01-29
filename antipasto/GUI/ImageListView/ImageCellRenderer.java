package antipasto.GUI.ImageListView;

import java.awt.Component;
import java.io.File;
import java.io.IOException;

import javax.imageio.ImageIO;
import javax.swing.DefaultListCellRenderer;
import javax.swing.ImageIcon;
import javax.swing.JLabel;
import javax.swing.JList;

import antipasto.Interfaces.IModule;


public class ImageCellRenderer extends DefaultListCellRenderer{
	public Component getListCellRendererComponent(JList list,
            Object value,
            int index,
            boolean isSelected,
            boolean hasFocus) {
			JLabel label =
			(JLabel)super.getListCellRendererComponent(list,
			             value,
			             index,
			             isSelected,
			             hasFocus);
			if (value instanceof File) {
			File file = (File)value;
			ImageIcon icon;
			try {
				icon = new ImageIcon(ImageIO.read(file));
				label.setIcon(icon);
				label.setText(file.getName());
			} catch (IOException e) {
				// TODO Auto-generated catch block
				e.printStackTrace();
			}
			} else {
			// Clear old icon; needed in 1st release of JDK 1.2
			label.setIcon(null);
			}
			return(label);
			}
}
