package antipasto.GUI.ImageListView;

import java.awt.BorderLayout;
import java.awt.Color;
import java.awt.Component;
import java.awt.Graphics2D;
import java.awt.GraphicsConfiguration;
import java.awt.GraphicsDevice;
import java.awt.GraphicsEnvironment;
import java.awt.Image;
import java.awt.image.BufferedImage;
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
import javax.swing.filechooser.FileSystemView;

import processing.app.Base;

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
			imgDescPanel.setOpaque(true);
			
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
				if(Base.isWindows()){
					icon = new ImageIcon(shellFolder.getIcon(false));  
				}else if(Base.isMacOS()){
					//what do we do when we're mac
					FileSystemView view = FileSystemView.getFileSystemView();
					Icon sysIcon = view.getSystemIcon(file);
					icon = new ImageIcon(ImageCellRenderer.iconToImage(sysIcon));
				}else{
					//what do we do when we're linux
					FileSystemView view = FileSystemView.getFileSystemView();
					Icon sysIcon = view.getSystemIcon(file);
					icon = new ImageIcon(ImageCellRenderer.iconToImage(sysIcon));
				}
				/* Test for an image */
				if(file.getName().endsWith(".bmp")){
					try{
						if(!Base.isWindows()){
						/* Read the image as an icon */
						icon = new ImageIcon(ImageIO.read(file));
						
						fileText = fileText + "   " +
								   icon.getIconWidth() + " x " + 
								   icon.getIconHeight() + " px";
						}
					}catch(Exception ex){
					}
				}
				
				
				
				/* Append more description */
				fileText = fileText + "   " + (double)file.length()/1000 + " KB";
				imgDescPanel.add(new JLabel(fileText));
				
				imgLabel = new JLabel(icon);
				
				fileEntry.add(imgLabel, BorderLayout.WEST);
				fileEntry.add(imgDescPanel);
				if (isSelected) {
					fileEntry.setBackground(Color.orange);
					imgLabel.setBackground(Color.orange);
					imgDescPanel.setBackground(Color.orange);
					
				} else {
					fileEntry.setBackground(Color.white);
					imgLabel.setBackground(Color.white);
					imgDescPanel.setBackground(Color.white);
				}
				
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
	
	static Image iconToImage(Icon icon) {
		if (icon instanceof ImageIcon) {
		return ((ImageIcon)icon).getImage();
		} else {
			int w = icon.getIconWidth();
			int h = icon.getIconHeight();
			GraphicsEnvironment ge = 
			GraphicsEnvironment.getLocalGraphicsEnvironment();
			GraphicsDevice gd = ge.getDefaultScreenDevice();
			GraphicsConfiguration gc = gd.getDefaultConfiguration();
			BufferedImage image = gc.createCompatibleImage(w, h);
			Graphics2D g = image.createGraphics();
			icon.paintIcon(null, g, 0, 0);
			g.dispose();
		return image;
		}
		}
}
