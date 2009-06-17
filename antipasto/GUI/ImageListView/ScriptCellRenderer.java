package antipasto.GUI.ImageListView;

import java.awt.*;
import java.awt.image.BufferedImage;
import java.io.File;
import java.io.IOException;

import javax.imageio.ImageIO;
import javax.swing.*;

import processing.app.Base;


public class ScriptCellRenderer extends DefaultListCellRenderer {
    //final static ImageIcon longIcon = new ImageIcon("long.gif");
    final static ImageIcon shortIcon = new ImageIcon("ruby.bmp");

    /* This is the only method defined by ListCellRenderer.  We just
     * reconfigure the JLabel each time we're called.
     */
    public Component getListCellRendererComponent(
        JList list,
	Object value,   // value to display
	int index,      // cell index
	boolean isSelected,    // is the cell selected
	boolean chf)    // the list and the cell have the focus
    {
        /* The DefaultListCellRenderer class will take care of
         * the JLabels text property, it's foreground and background
         * colors, and so on.
         */
        JLabel label = (JLabel)super.getListCellRendererComponent(list, value, index, isSelected, chf);

        /* Modify the JLabel to your liking here.. 
         * */
        JPanel panel = new JPanel();
        panel.setLayout(new BoxLayout(panel,BoxLayout.X_AXIS));
        panel.setOpaque(true);
        
        if (!isSelected) {
        	panel.setBackground(new Color(0xFF, 0xFF, 0xFF));
        } else {
        	panel.setBackground(Color.orange);
        }
        
        JLabel iconLabel = new JLabel();
		ImageIcon iconImg = new ImageIcon(Base.getImage("ruby.png", this));
		iconLabel = new JLabel(iconImg);
		
		JLabel iconText = new JLabel(" " + value.toString());

		panel.add(iconLabel);
		panel.add(iconText);

	return panel;
    }
    
}
