package antipasto.GUI.ImageListView;

import java.awt.BorderLayout;
import java.awt.Color;
import java.awt.Component;
import java.io.File;
import java.io.IOException;

import javax.imageio.ImageIO;
import javax.swing.BoxLayout;
import javax.swing.DefaultListCellRenderer;
import javax.swing.Icon;
import javax.swing.ImageIcon;
import javax.swing.JLabel;
import javax.swing.JList;
import javax.swing.JPanel;

import sun.awt.shell.ShellFolder;

import antipasto.Interfaces.IModule;


public class ImageCellRenderer extends DefaultListCellRenderer{
	public Component getListCellRendererComponent(JList list,
            Object value,
            int index,
            boolean isSelected,
            boolean hasFocus) {
			
			JPanel fileEntry = new JPanel();
			fileEntry.setLayout(new BorderLayout());
			fileEntry.setBackground(Color.white);
			
			JPanel imgDescPanel = new JPanel();
			imgDescPanel.setLayout(new BoxLayout(imgDescPanel, BoxLayout.PAGE_AXIS));
			imgDescPanel.setBackground(Color.white);
			
			JLabel fileDesc;
			JLabel fileName;
			JLabel imgLabel = new JLabel();
			
	        JLabel label =
	            (JLabel)super.getListCellRendererComponent(list,
	                                                       value,
	                                                       index,
	                                                       isSelected,
	                                                       hasFocus);
			
			if (value instanceof File) {
			File file = (File)value;
			Icon icon;
			fileEntry.setLayout(new BorderLayout());
			try {
				
				String fileText = " " + file.getName();
				
				ShellFolder shellFolder = ShellFolder.getShellFolder(file);      
				icon = new ImageIcon(shellFolder.getIcon(true));  
					
				/* Test for an image */
				if(file.getName().endsWith(".bmp")){
					
					/* Read the image as an icon */
					icon = new ImageIcon(ImageIO.read(file));
					
					fileText = fileText + "   " +
							   icon.getIconWidth() + " x " + 
							   icon.getIconHeight() + " px";
				}
				imgLabel.setIcon(icon);
				
				/* Append more description */
				fileText = fileText + "   " + (double)file.length()/1000 + " KB";
				imgDescPanel.add(new JLabel(fileText));
				
				fileEntry.add(imgLabel, BorderLayout.WEST);
				fileEntry.add(imgDescPanel);
				
				
			} catch (IOException e) {
				// TODO Auto-generated catch block
				e.printStackTrace();
			}
			} else {
			// Clear old icon; needed in 1st release of JDK 1.2
			//imgLabel.setIcon(null);
			}
			return(fileEntry);
			}
}
