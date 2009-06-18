package antipasto.GUI.GadgetListView;
import java.awt.BorderLayout;
import java.awt.Color;
import java.awt.Dimension;
import java.awt.Font;
import java.awt.FontMetrics;
import java.awt.Graphics;
import java.awt.Graphics2D;
import java.awt.event.ComponentEvent;
import java.awt.event.ComponentListener;
import java.awt.event.MouseEvent;
import java.awt.event.MouseListener;
import java.awt.font.FontRenderContext;
import java.awt.geom.Rectangle2D;
import java.awt.geom.RoundRectangle2D;
import java.io.File;

import javax.swing.JComponent;
import javax.swing.JFrame;
import javax.swing.JLabel;
import javax.swing.JPanel;
import javax.swing.event.DocumentEvent;
import javax.swing.event.DocumentListener;
import javax.swing.event.EventListenerList;

import antipasto.Interfaces.*;

public class WingFooter extends JPanel {
	
	boolean isUpdated = false;
	String text;
	Font font;
	
	private Color bgDefaultColor = new Color(0x04, 0x4F, 0x6F);
	private Color bgHoverColor = Color.WHITE;
	private Color bgColor;
	
	private Color textDefaultColor = Color.WHITE;
	private Color textHoverColor = Color.BLACK;
	private Color textColor;
	
	private double width = 300; 
	private double height = 15;
	private boolean isFocused = false;
	private boolean isSelected = false;
	
	private JLabel label;
	
	/**
	 * Creates a new Wing Footer
	 * 
	 * @param text	The message to display
	 * @param width The width of the footer
	 * @param height The height of the footer
	 */
	public WingFooter(String text, int width, int height){

		this.width = width;
		this.height = height;
		
		/* Setup the panel */
		setSize(width, height);
		setBackground(bgDefaultColor);
		setLayout(new BorderLayout());
		
		/* Setup the label */
		label = new JLabel(text);	
		label.setForeground(textDefaultColor);
		
		/* Add */
		add(label,BorderLayout.WEST);
		
	}
	
	/**
	 * 
	 * @param text Changes the text displayed in the footer
	 */
	void setText(String text) {
		label.setText(text);
		this.repaint();
	}
}
