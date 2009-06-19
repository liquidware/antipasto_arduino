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
import java.io.FilenameFilter;

import javax.swing.JComponent;
import javax.swing.JFrame;
import javax.swing.JLabel;
import javax.swing.JList;
import javax.swing.JPanel;
import javax.swing.JScrollPane;
import javax.swing.JTextArea;
import javax.swing.event.DocumentEvent;
import javax.swing.event.DocumentListener;
import javax.swing.event.EventListenerList;

import processing.app.Base;
import processing.app.Sketchbook;

import antipasto.ScriptRunner;
import antipasto.GUI.ImageListView.ScriptCellRenderer;
import antipasto.Interfaces.*;
import antipasto.Util.Utils;

public class WingPanelReference extends JPanel {
	
	boolean isUpdated = false;
	String text;
	Font font;
	
	private Color bgDefaultColor =Color.WHITE;
	private Color bgHoverColor = Color.WHITE;
	private Color bgColor;
	
	private Color textDefaultColor = Color.BLACK;
	private Color textHoverColor = Color.WHITE;
	private Color textColor;
	
	private double width = 300; 
	private double height = 450;
	private boolean isFocused = false;
	private boolean isSelected = false;
	
	private String referenceFile; 
	
	private JPanel     headerPanel;
	private JLabel     headerLabel;
	private JScrollPane scrollPane;
	private WingFooter  wingFooter;
	private JTextArea textArea;
	
	/**
	 * Creates a new Wing Panel for Reference
	 * @param referenceFile The file that contains the reference.
	 * @param wingFooter The panel to display status to the user
	 * @param width The width of the panel
	 * @param height The height of the panel
	 */
	public WingPanelReference(String text,
							  Object wingFooter, 
							  int width, int height){

		this.wingFooter = (WingFooter)wingFooter;
		this.text = text;
		this.width = width;
		this.height = height;

		
		/* Setup the panel */
		setSize(width, height);
		setBackground(bgDefaultColor);
		setLayout(new BorderLayout());
		
		/* Build the header */
		headerPanel = initPanelHeader(" <- Back  ");
		
		/* Build the body */
		initBody();
		
		/* Assemble the Panel */
		add(headerPanel, BorderLayout.NORTH);
		add(scrollPane, BorderLayout.CENTER);
	}
	
	/**
	 * Initialize a Panel Header
	 * @param textDisplay The button text
	 * @return: a JPanel with a button
	 */
	JPanel initPanelHeader(String textDisplay) {
		
		JPanel headerPanel = new JPanel();
		headerPanel.setBackground(new Color(0x04, 0x4F, 0x6F));
		headerPanel.setLayout(new BorderLayout());
		
		headerLabel = new JLabel(textDisplay);
		
		headerLabel.addMouseListener(new MouseListener() {

			public void mouseClicked(MouseEvent arg0) {
				// TODO Auto-generated method stub
				
			}

			public void mouseEntered(MouseEvent arg0) {
			
				headerLabel.setForeground(Color.orange);
				
				wingFooter.setText(" Go Back ");
				
			}

			public void mouseExited(MouseEvent arg0) {
				headerLabel.setForeground(Color.white);
				wingFooter.setText(" ");
			}

			public void mousePressed(MouseEvent arg0) {
				
					wingFooter.setText("Ready.");
			}

			public void mouseReleased(MouseEvent arg0) {

			}
			
			});
		
		headerLabel.setForeground(new Color(0xFF, 0xFF, 0xFF));
		headerPanel.add(headerLabel, BorderLayout.CENTER);		
		
		return headerPanel;
	}
	
	/**
	 * Initialize the body of the panel.
	 */
	private void initBody() {
		
		textArea = new JTextArea(text);
		scrollPane = new JScrollPane(textArea);
		scrollPane
				.setVerticalScrollBarPolicy(JScrollPane.VERTICAL_SCROLLBAR_AS_NEEDED);
		scrollPane
				.setHorizontalScrollBarPolicy(JScrollPane.HORIZONTAL_SCROLLBAR_NEVER);

		textArea.setWrapStyleWord(true);
		textArea.setLineWrap(true);
		textArea.setBounds(scrollPane.getBounds());

		textArea.setVisible(true);
		scrollPane.setVisible(true);
	}
	
	/**
	 * 
	 * @param text The reference text to display.
	 */
	public void setText(String text) {
		textArea.setText(text);
	}
	
}
