package antipasto.GUI.GadgetListView;
import javax.swing.JEditorPane;
import java.awt.FlowLayout;
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
import java.io.*;
import java.net.*;
import java.util.Properties;
import java.util.*;
import javax.swing.text.html.*;

import javax.swing.JComponent;
import javax.swing.JFrame;
import javax.swing.JLabel;
import javax.swing.JList;
import javax.swing.JPanel;
import javax.swing.JScrollPane;
import javax.swing.*;
import javax.swing.event.DocumentEvent;
import javax.swing.event.DocumentListener;
import javax.swing.event.EventListenerList;
import javax.swing.event.HyperlinkEvent;
import javax.swing.event.HyperlinkListener;


import processing.app.Base;

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
	final JTextPane textArea = new JTextPane();
	final List <URL> htmlHistory = new ArrayList <URL>();


	/**
	 * Creates a new Wing Panel for Reference
	 * @param text The text string that contains the reference.
	 * @param wingFooter The panel to display status to the user
	 * @param width The width of the panel
	 * @param height The height of the panel
	 */
	public WingPanelReference(Object wingFooter,
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
	    headerPanel = initPanelHeader("<html><u>&lt;-Back </u></html>");

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
	    headerPanel.setLayout(new FlowLayout(FlowLayout.LEFT, 0,0));

	    headerLabel = new JLabel(textDisplay);

	    MouseListener mouseListener = new MouseListener() {
		    public void mouseClicked(MouseEvent arg0) {

			    try {
				//Go Back
				if ( (htmlHistory.size()-1) > 0) {
				    htmlHistory.remove(htmlHistory.size()-1);
				    setPage(htmlHistory.get(htmlHistory.size()-1).toString());
				}
			    } catch (Exception ex) {
				ex.printStackTrace();
			    }

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

		    }

		    public void mouseReleased(MouseEvent arg0) {

		    }

		};


	    headerLabel.addMouseListener(mouseListener);

	    headerLabel.setForeground(new Color(0xFF, 0xFF, 0xFF));
	    headerPanel.add(headerLabel);

	    return headerPanel;
	}

	/**
	 * Initialize the body of the panel.
	 */
	private void initBody() {

            HyperlinkListener listener = new HyperlinkListener() {
              public void hyperlinkUpdate(HyperlinkEvent e) {
                if (e.getEventType() == HyperlinkEvent.EventType.ACTIVATED) {
                    setPage(e.getURL().toString());
                }
              }
            };
            textArea.addHyperlinkListener(listener);
            textArea.setVisible(true);
            textArea.setEditable(false);
	    textArea.setEditorKit(new HTMLEditorKit());

            scrollPane = new JScrollPane(textArea);
            scrollPane.setVerticalScrollBarPolicy(JScrollPane.VERTICAL_SCROLLBAR_AS_NEEDED);
            scrollPane.setHorizontalScrollBarPolicy(JScrollPane.HORIZONTAL_SCROLLBAR_AS_NEEDED);
            scrollPane.setVisible(true);
	}

	/**
	 *
	 * @param url - The url of the page to display.
	 */
	public void setPage(String url) {
	    try {
		wingFooter.setText("Loading...");
		textArea.setPage(url);
		htmlHistory.add(textArea.getPage());
	    } catch (IOException ioe) {
		System.out.println("Error loading reference URL: '" + url + "' " + ioe);

	    }
	    wingFooter.setText("Done.");
	}

	/**
	 * Clears html page history in the docs browser. Usually called
	 * when switching boards.
	 *
	 * @author Chris (9/7/2009)
	 */
	public void clearHistory(){
	    //Clear the history
	    while(htmlHistory.size() > 0) {
		htmlHistory.remove(htmlHistory.size() - 1);
	    }
	}

}
